/* ESPProLib from RicardoOliveira (https://github.com/RicardoOliveira/ESPProLib)
 * Modified and updated by Farossco to work with ESP32
 *
 * https://github.com/Farossco/ESP32ProLib
 */

#define ESPPL_MAC_LEN                    6
#define ESPPL_SSID_LEN                   33
#define ESPPL_CHANNEL_MIN                1
#define ESPPL_CHANNEL_MAX                14
#define ESPPL_CHANNEL_DEFAULT            1
#define ESPPL_MANAGEMENT_MAC_HEADER_SIZE 36

typedef struct
{
	unsigned protocolVersion : 2;
	unsigned type : 2;
	unsigned subtype : 4;
	unsigned toDS : 1;
	unsigned fromDS : 1;
	unsigned moreFragments : 1;
	unsigned retry : 1;
	unsigned powerMngmt : 1;
	unsigned moreData : 1;
	unsigned protectedFrame : 1;
	unsigned htcOrder : 1;
} __attribute__((packed)) esppl_frame_control;

typedef struct
{
	esppl_frame_control fctl;
	int16_t             duration;
	uint8_t             destinationaddr[ESPPL_MAC_LEN];
	uint8_t             sourceaddr[ESPPL_MAC_LEN];
	uint8_t             bssid[ESPPL_MAC_LEN];
	int16_t             seqctl;
	unsigned char       payload[];
} __attribute__((packed)) esppl_frame;

typedef void (* ESPPL_CB_T) (esppl_frame * frame, wifi_pkt_rx_ctrl_t rxCtrl);

// - DS Field
const uint8_t ESPPL_DS_NO     = 0;
const uint8_t ESPPL_DS_TO     = 1;
const uint8_t ESPPL_DS_FROM   = 2;
const uint8_t ESPPL_DS_TOFROM = 3;

// - Frame types
const uint8_t ESPPL_MANAGEMENT = 0;
const uint8_t ESPPL_CONTROL    = 1;
const uint8_t ESPPL_DATA       = 2;

// - Frame subtypes
const uint8_t ESPPL_MANAGEMENT_ASSOCIATION_REQUEST    = 0;
const uint8_t ESPPL_MANAGEMENT_ASSOCIATION_RESPONSE   = 1;
const uint8_t ESPPL_MANAGEMENT_REASSOCIATION_REQUEST  = 2;
const uint8_t ESPPL_MANAGEMENT_REASSOCIATION_RESPONSE = 3;
const uint8_t ESPPL_MANAGEMENT_PROBE_REQUEST          = 4;
const uint8_t ESPPL_MANAGEMENT_PROBE_RESPONSE         = 5;
const uint8_t ESPPL_MANAGEMENT_TIMMING_ADVERTISEMENT  = 6;
const uint8_t ESPPL_MANAGEMENT_RESERVED1              = 7;
const uint8_t ESPPL_MANAGEMENT_BEACON                 = 8;
const uint8_t ESPPL_MANAGEMENT_ATIM                   = 9;
const uint8_t ESPPL_MANAGEMENT_DISASSOCIATION         = 10;
const uint8_t ESPPL_MANAGEMENT_AUTHENTICATION         = 11;
const uint8_t ESPPL_MANAGEMENT_DEAUTHENTICATION       = 12;
const uint8_t ESPPL_MANAGEMENT_ACTION                 = 13;
const uint8_t ESPPL_MANAGEMENT_ACTION_NO_ACK          = 14;
const uint8_t ESPPL_MANAGEMENT_RESERVED2              = 15;

const uint8_t ESPPL_CONTROL_RESERVED1         = 0;
const uint8_t ESPPL_CONTROL_RESERVED2         = 1;
const uint8_t ESPPL_CONTROL_RESERVED3         = 2;
const uint8_t ESPPL_CONTROL_RESERVED4         = 3;
const uint8_t ESPPL_CONTROL_RESERVED5         = 4;
const uint8_t ESPPL_CONTROL_RESERVED6         = 5;
const uint8_t ESPPL_CONTROL_RESERVED7         = 6;
const uint8_t ESPPL_CONTROL_CONTROL_WRAPPER   = 7;
const uint8_t ESPPL_CONTROL_BLOCK_ACK_REQUEST = 8;
const uint8_t ESPPL_CONTROL_BLOCK_ACK         = 9;
const uint8_t ESPPL_CONTROL_PS_POLL           = 10;
const uint8_t ESPPL_CONTROL_RTS               = 11;
const uint8_t ESPPL_CONTROL_CTS               = 12;
const uint8_t ESPPL_CONTROL_ACK               = 13;
const uint8_t ESPPL_CONTROL_CF_END            = 14;
const uint8_t ESPPL_CONTROL_CF_END_CF_ACK     = 15;

const uint8_t ESPPL_DATA_DATA                    = 0;
const uint8_t ESPPL_DATA_DATA_CF_ACK             = 1;
const uint8_t ESPPL_DATA_DATA_CF_POLL            = 2;
const uint8_t ESPPL_DATA_DATA_CF_ACK_CF_POLL     = 3;
const uint8_t ESPPL_DATA_NULL                    = 4;
const uint8_t ESPPL_DATA_CF_ACK                  = 5;
const uint8_t ESPPL_DATA_CF_POLL                 = 6;
const uint8_t ESPPL_DATA_CF_ACK_CF_POLL          = 7;
const uint8_t ESPPL_DATA_QOS_DATA                = 8;
const uint8_t ESPPL_DATA_QOS_DATA_CF_ACK         = 9;
const uint8_t ESPPL_DATA_QOS_DATA_CF_ACK_CF_POLL = 10;
const uint8_t ESPPL_DATA_QOS_NULL                = 11;
const uint8_t ESPPL_DATA_RESERVED1               = 12;
const uint8_t ESPPL_DATA_QOS_CF_POLL             = 13;
const uint8_t ESPPL_DATA_QOS_CF_ACK_CF_POLL      = 14;

// TODO: Reserved