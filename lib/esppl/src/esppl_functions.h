/* ESPProLib from RicardoOliveira (https://github.com/RicardoOliveira/ESPProLib)
 * Modified and updated by Farossco to work with ESP32
 *
 * https://github.com/Farossco/ESP32ProLib
 */

#ifndef ESP32PL_FUNC_
#define ESP32PL_FUNC_

#include <Arduino.h>
#include <esp_wifi.h>
#include "esppl_struct.h"

void esppl_rx_cb (void * buf, wifi_promiscuous_pkt_type_t type);
void esppl_set_channel (int channel);
bool esppl_process_frames ();
void esppl_init (ESPPL_CB_T cb);
void esppl_sniffing_start ();
void esppl_sniffing_stop ();

#endif // ifndef ESP32PL_FUNC_