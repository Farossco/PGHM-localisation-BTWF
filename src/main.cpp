/*
 * Based on Friend Detector by Ricardo Oliveira, modified by Farossco 23/02/2021
 *
 */

#include <Arduino.h>
#include <esppl_functions.h>
#include "display.h"
#include "utils.h"

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
uint8_t detectedMac[DETECTED_LIST_SIZE][ESPPL_MAC_LEN];

/*
 * This is your friend's name list
 * put them in the same order as the MAC addresses
 */
String friendName[FRIEND_LIST_SIZE] = { };

/* Displays the device on the specified stream
 */
void displayDeviceOnStream (Stream &stream, esppl_frame * frame, wifi_pkt_rx_ctrl_t * rxCtrl, bool near)
{
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

/* Displays the device on Serial and LCD Screen
 */
void displayDevice (esppl_frame * frame, wifi_pkt_rx_ctrl_t * rxCtrl, bool near)
{
	displayDeviceOnScreen (frame, rxCtrl, near);

	displayDeviceOnStream (Serial, frame, rxCtrl, near);
}

/* Callback function
 *
 * This function is called whenever a frame is received.
 * All the informations on the frame are in the "frame" argument.
 * the "rxCtrl" arguments gives information about the reception of the frame (RSSI, channel...)
 */
void cb (esppl_frame * frame, wifi_pkt_rx_ctrl_t * rxCtrl)
{
	if (frame->fctl.type != ESPPL_MANAGEMENT || frame->fctl.subtype != ESPPL_MANAGEMENT_PROBE_REQUEST)
		return;

	bool newMac = true;

	for (int i = 0; i < detectedListIndex; i++)
		if (maccmp (frame->sourceaddr, detectedMac[i]))
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
			displayDevice (frame, rxCtrl, false); // Displaying with prefix [New]

			maccpy (frame->sourceaddr, detectedMac[detectedListIndex]);
			detectedListIndex++;
		}
	}

	// Detecting and printing close (RSSI > RSSI_THRESHOLD) devices as many times as we see them
	else if (PRINT_NEARBY && rxCtrl->rssi > RSSI_THRESHOLD)
	{
		displayDevice (frame, rxCtrl, true); // Displaying with prefix [Near]
	}

	// Prints if a friend device is detected
	for (int i = 0; i < FRIEND_LIST_SIZE; i++)
		if (maccmp (frame->sourceaddr, friendMac[i]))
			Serial.printf ("\n%s is here! :)", friendName[i].c_str());
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