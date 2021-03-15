#include "display.h"

U8G2_PCD8544_84X48_F_4W_HW_SPI u8g2 (U8G2_R0, /* cs=*/ 10, /* dc=*/ GPIO); // Nokia 5110 Display

#define newline() x = 2; y += 7

void initDisplay ()
{
	pinMode (GPIO, OUTPUT);
	digitalWrite (GPIO, LOW);

	u8g2.begin();
	u8g2.clearBuffer();
}

void writeMac (char * buf, const char * prefix, uint8_t * mac)
{
	int i;

	i = sprintf (buf, "%s", prefix);

	// Checking the absence of error and incrementing buffer pointer
	if (i > 0)
		buf += i;
	else
		return;

	for (int j = 0; j < ESPPL_MAC_LEN; j++)
	{
		i = sprintf (buf, "%02X", mac[j]);

		// Checking the absence of error and incrementing buffer pointer
		if (i > 0)
			buf += i;
		else
			return;
	}
}

void displayDeviceOnScreen (esppl_frame * frame, wifi_pkt_rx_ctrl_t * rxCtrl, bool near)
{
	int x, y;
	char buffer[17]; // 16 caracters is the maximum length of a line with the 5x7 font

	u8g2.clear();
	u8g2.drawFrame (0, 0, 84, 48); // Draws a rectangle on the border of the screen
	u8g2.setFont (u8g2_font_5x7_tf);

	x = 2;
	y = 7;
	writeMac (buffer, "S : ", frame->sourceaddr);
	u8g2.drawStr (x, y, buffer);

	newline();
	writeMac (buffer, "D : ", frame->destinationaddr);
	u8g2.drawStr (x, y, buffer);

	newline();
	writeMac (buffer, "B : ", frame->bssid);
	u8g2.drawStr (x, y, buffer);

	newline();
	sprintf (buffer, "T/S  : %d/%d", frame->fctl.type, frame->fctl.subtype);
	u8g2.drawStr (x, y, buffer);

	newline();
	sprintf (buffer, "RSSI : %3d dBm", rxCtrl->rssi);
	u8g2.drawStr (x, y, buffer);

	newline();
	sprintf (buffer, "Chan : %2d [%s]", rxCtrl->channel, near ? "Near" : "New ");
	u8g2.drawStr (x, y, buffer);

	u8g2.sendBuffer();
} // displayDeviceOnScreen