/* ESPProLib from RicardoOliveira (https://github.com/RicardoOliveira/ESPProLib)
 * Modified and updated by Farossco to work with ESP32
 *
 * https://github.com/Farossco/ESP32ProLib
 */

#include "esppl_functions.h"

const wifi_promiscuous_filter_t filter = { WIFI_PROMIS_FILTER_MASK_MGMT | WIFI_PROMIS_FILTER_MASK_DATA };

uint8_t esppl_channel                    = ESPPL_CHANNEL_DEFAULT;
uint8_t esppl_default_mac[ESPPL_MAC_LEN] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
ESPPL_CB_T user_cb;
int frame_waitlist          = 0;
bool esppl_sniffing_enabled = false;

/*
 * Promiscuous RX callback function
 */
void esppl_rx_cb (void * buf, wifi_promiscuous_pkt_type_t type)
{
	wifi_promiscuous_pkt_t * packet = (wifi_promiscuous_pkt_t *) buf;

	if (esppl_sniffing_enabled)
	{
		frame_waitlist++;
		user_cb ((esppl_frame *) packet->payload, &packet->rx_ctrl);
		frame_waitlist--;
	}
}

/*
 * Change channel
 */
void esppl_set_channel (int channel)
{
	esp_wifi_set_channel (channel, WIFI_SECOND_CHAN_NONE);
	esppl_channel = channel;
}

/*
 * Allows some time to process the frames
 */
bool esppl_process_frames ()
{
	delay (10);
	return frame_waitlist != 0;
}

/*
 * Initialization function
 */
void esppl_init (ESPPL_CB_T cb)
{
	wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();

	user_cb        = cb;
	frame_waitlist = 0;

	esp_wifi_init (&cfg);
	// esp_wifi_set_storage (WIFI_STORAGE_RAM);
	esp_wifi_set_mode (WIFI_MODE_STA);
	esp_wifi_set_promiscuous (false);
	esp_wifi_set_promiscuous_rx_cb (esppl_rx_cb);
	esp_wifi_set_promiscuous_filter (&filter);
	esp_wifi_set_promiscuous (true);
	esp_wifi_start();
	esp_wifi_set_channel (esppl_channel, WIFI_SECOND_CHAN_NONE);
	esppl_sniffing_enabled = false;
}

/*
 * Start sniffing
 */
void esppl_sniffing_start ()
{
	esppl_sniffing_enabled = true;
}

/*
 * Stop sniffing
 */
void esppl_sniffing_stop ()
{
	esppl_sniffing_enabled = false;
}