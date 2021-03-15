/*
 * Friend Detector by Ricardo Oliveira, modified by Farossco 2/23/2021
 *
 * A Node MCU microcontroller + mini bread board + 4 pin RGB LED to detect when devices belonging to a target are nearby.
 *
 *
 * The function of this code is to read nearby Wi-Fi traffic in the form of packets. These packets are compared
 * to a list of MAC addresses we wish to track, and if the MAC address of a packet matches one on the list, we turn
 * on a colored LED that is linked to the user owning the device. For example, when my roommate comes home, the
 * transmissions from his phone will be detected and cause the blue LED to turn on until his phone is no longer detected.
 * It can detect more than one phone at a time, meaning if my phone (red) and my roommate's phone (blue) are both home,
 * the LED will show purple.
 */

#include <Arduino.h>
#include <esppl_functions.h>
#include "display.h"

/* If you want to track a specific address, it can be put in the friendMac list */
#define FRIEND_LIST_SIZE   0
#define DETECTED_LIST_SIZE 200
#define RSSI_THRESHOLD     -40
#define PRINT_NEARBY       true

static int detectedListIndex = 0;

/*
 * This is your friend's MAC address list
 * Format it by taking the mac address aa:bb:cc:dd:ee:ff
 * and converting it to 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff
 */
uint8_t friendMac[FRIEND_LIST_SIZE][ESPPL_MAC_LEN] = { };

/* This stores the MAC addresses we already detected to avoid printing them more than once*/
uint8_t detectedmac[DETECTED_LIST_SIZE][ESPPL_MAC_LEN];

/*
 * This is your friend's name list
 * put them in the same order as the MAC addresses
 */
String friendname[FRIEND_LIST_SIZE] = { };

bool maccmp (uint8_t * mac1, uint8_t * mac2)
{
	for (int i = 0; i < ESPPL_MAC_LEN; i++)
	{
		if (mac1[i] != mac2[i])
		{
			return false;
		}
	}
	return true;
}

void maccpy (uint8_t * macS, uint8_t * macD)
{
	for (int i = 0; i < ESPPL_MAC_LEN; i++)
		macD[i] = macS[i];
}

void displayDevice (esppl_frame * frame, wifi_pkt_rx_ctrl_t * rxCtrl, bool near)
{
	displayDeviceOnScreen (frame, rxCtrl, near);

	Serial.printf ("[%s] ", near ? "Near" : "New ");

	Serial.printf ("S : ");
	for (int i = 0; i < ESPPL_MAC_LEN; i++)
		Serial.printf ("%02X", frame->sourceaddr[i]);

	Serial.printf (" - D : ");

	for (int i = 0; i < ESPPL_MAC_LEN; i++)
		Serial.printf ("%02X", frame->destinationaddr[i]);

	Serial.printf (" - BSSID : ");

	for (int i = 0; i < ESPPL_MAC_LEN; i++)
		Serial.printf ("%02X", frame->bssid[i]);

	Serial.printf (" - Type : %2d", frame->fctl.type);

	Serial.printf (" - Subtype : %2d", frame->fctl.subtype);

	Serial.printf (" - RSSI : %3d dBm", rxCtrl->rssi);

	Serial.printf (" - C : %d\n", rxCtrl->channel);
}

void cb (esppl_frame * frame, wifi_pkt_rx_ctrl_t rxCtrl)
{
	if (frame->fctl.type != ESPPL_MANAGEMENT || frame->fctl.subtype != ESPPL_MANAGEMENT_PROBE_REQUEST)
		return;

	bool newMac = true;

	for (int i = 0; i < detectedListIndex; i++)
		if (maccmp (frame->sourceaddr, detectedmac[i]))
			newMac = false;

	// Printing newly detected addresses, these addresses are printed once only
	if (newMac)
	{
		if (detectedListIndex == DETECTED_LIST_SIZE)
		{
			printf ("Detected list size full\n");
		}
		else
		{
			displayDevice (frame, &rxCtrl, false); // Displaying with prefix [New]

			maccpy (frame->sourceaddr, detectedmac[detectedListIndex]);
			detectedListIndex++;
		}
	}

	// Detecting and printing close (RSSI > RSSI_THRESHOLD) devices as many times as we see them
	else if (PRINT_NEARBY && rxCtrl.rssi > RSSI_THRESHOLD)
	{
		displayDevice (frame, &rxCtrl, true); // Displaying with prefix [Near]
	}

	// Prints if a friend device is detected
	for (int i = 0; i < FRIEND_LIST_SIZE; i++)
		if (maccmp (frame->sourceaddr, friendMac[i]))
			Serial.printf ("\n%s is here! :)", friendname[i].c_str());
} // cb

// start variables package - Skickar 2018 hardware LED for NodeMCU on mini breadboard //
void setup ()
{
	delay (500);
	Serial.begin (115200);
	esppl_init (cb);
	esppl_sniffing_start();
	Serial.print ("Program started\n");

	initDisplay();
}

void loop ()
{
	for (int i = ESPPL_CHANNEL_MIN; i <= ESPPL_CHANNEL_MAX; i++)
	{
		esppl_set_channel (i);
		while (esppl_process_frames())
		{ }
	}
}