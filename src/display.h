#ifndef DISPLAY_H_
#define DISPLAY_H_

#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <U8g2lib.h>
#include <esppl_functions.h>

#define GPIO 32

extern U8G2_PCD8544_84X48_F_4W_HW_SPI u8g2; // Joy-it SBC-LCD 84x48

void initDisplay ();
void drawLogo ();
void drawURL ();
void displayDeviceOnScreen (esppl_frame * frame, wifi_pkt_rx_ctrl_t * rxCtrl, bool near);

#endif // ifndef DISPLAY_H_