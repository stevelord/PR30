#ifndef _SHAREMEMORY_H_
#define _SHAREMEMORY_H_

#define SHARE_KEY 28146688


typedef enum
{
        wifi_auto_off_off       = 0,
        wifi_auto_off_on        = 1,                
} wifi_auto_off_e;

typedef enum
{
        PIN_ERROR               =-1,
        PIN_READY               = 0,                
        PIN_ENABLE              = 1,
        PIN_BLOCKed             = 2,
        PIN_INVALID             = 3,
} PIN_statue_e;



typedef enum {
    PIN1_STATUS_UNINITIALIZED  = 0x0,
    PIN1_STATUS_DISABLED       = 0x1,
    PIN1_STATUS_ENABLED        = 0x2,
    PIN1_STATUS_BLOCKED        = 0x3,
    PIN1_STATUS_PERM_BLOCKED   = 0x4,
    PIN1_STATUS_VERIFIED       = 0x5,
    PIN1_STATUS_SIM_ERROR      = 0x6,
} PIN1_Status_e;


typedef enum
{
	NONE                    =-1,   /* FOR INTERNAL USE ONLY! */
	NO_SRV		        = 0,   /* No service = NV_MODE_INACTIVE */
	MODE_GSM	        = 1,   /* Global System for Mobile communications (GSM) mode */
	GPRS		        = 2,
	EDGE		        = 3,
	WCDMA		        = 4,   /* Wideband CDMA (WCDMA) mode */
	HSDPA		        = 5,   /* High Data Rate (HDR) mode */
	HSUPA		        = 6,   /* High Data Rate (HDR) mode */	
} RAT_e;

typedef enum 
{
	DisConnect              = 0,
	Connect                 = 5,
} callstate_e;

typedef enum 
{
	Not_Verify              = 0,
	Verified                = 5,
} pin_verify_e;

typedef enum
{
	BELOW_V_360             = 0,
	BETWEEN_V_370_360       = 1,
	BETWEEN_V_385_370       = 2,
	BETWEEN_V_400_385       = 3,
	BETWEEN_V_420_420       = 4,
	V_Charging              = 6,
		
} BatteryCapacity;


/**
 * threeG_info_t
 *
 * 3G information
 *
 * RAT: the registration state (see enum RAT_e)
 * roaming: the roaming status
 *     0: home
 *     1: roaming
 * PIN_status: describe PIN status (see enum PIN_statue_e)
 * PIN_verified: describe PIN verified status (see enum pin_verify_e)
 * PIN1_Status: describe PIN1 status (see enum PIN1_Status_e)
 * SIMLock: describe SIM if lock
 *     0: unlock
 *     1: lock
 * OP_Name: describe the operator name
 * SPN: no use
 * nwk_mcc, nwk_mnc: the current MCC(Mobile Country Code) and MNC(MObile Network
 *                   Code)
 * home_mcc, home_mnc: the home MCC and MNC
 * IMEI: International Mobile Equipment Identity
 * IMSI: International Mobile Subscriber Identity
 * Tech_name: the string of RAT
 *     "SEARCHING..."
 *     "EMERGENCY"
 *     "NO SERVICE"
 *     "GSM"
 *     "GPRS"
 *     "EDGE"
 *     "UMTS"
 *     "HSPA"
 * fw_version: module's firmware version
 */
typedef struct 
{
        int     RAT;
        int     roaming;
        int     PIN_status;
        int     PIN_Verified;
        int     PIN1_Status;
        int     SIMLock;
        char    OP_Name[128+1];
        char    SPN[128+1];
        char    nwk_mcc[4+1];
        char    nwk_mnc[4+1];
        char    nwk_plmn[8+1];
        char    home_mcc[4+1];
        char    home_mnc[4+1];
        char    IMEI[16+1];
        char    IMSI[16+1];
        char    Tech_name[16+1];
        char    fw_version[16+1];
	
} threeG_info_t;

/**
 * threeG_connect_t
 *
 * 3G connection status
 *
 * network_status: network status
 *    -1: for internal use only
 *     0: no service
 *     1: limited service
 *     2: service available
 *     3: limited regional service
 *     4: MS is in power save or deep sleep
 * signal: signal strengths
 * callstate: a call state
 *     0: deactivated
 *     2: activating
 *     3: activation failed
 *     5: activated   
 * sms: no use
 */
typedef struct 
{

	int     network_status;
	int     signal;
	int     callstate;
	int     sms;
#if 0
	int     threeGid;
#endif
} threeG_connect_t;

#if 1
typedef struct
{
	char    SSID[255];
	int     channel;
	char    DATETIME_DEFAULT_CURRENT_TIMESTAMP[14];
	char    active;
	unsigned char	num_sta; // Number of STAs associated to WiFi AP
	int     sleep_time;
        char    wifi_auto_off;
} wifi_t;

typedef struct
{
	char    name[255];
	int     wifi_id;
} wifi_client_t;
#endif

typedef struct
{
	float   voltage;
	int     capacity;
        int     refresh_frequency;
} battery_t;

typedef struct
{
	threeG_info_t           u_threeG_info;
	threeG_connect_t        u_threeG_connect;
#if 1        
	wifi_t                  u_wifi;
	wifi_client_t           u_wifi_client;
#endif        
	battery_t               u_battery;
} all_info_t;
#endif /*_SHAREMEMORY_H_*/
