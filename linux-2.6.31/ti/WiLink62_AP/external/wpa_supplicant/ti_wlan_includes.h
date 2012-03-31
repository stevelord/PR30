/*
 * ti_wlan_includes.h
 *
 * Copyright 2001-2008 Texas Instruments, Inc. - http://www.ti.com/
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *     http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and  
 * limitations under the License.
 */

#ifndef TI_WLAN_INCLUDES_H
#define TI_WLAN_INCLUDES_H


/* located in stad/Export_Inc/privateCmd.h */

/** \def PRIVATE_CMD_SET_FLAG
 * \brief Bitmaks of bit which indicates that the Command is SET Command
 */
#define PRIVATE_CMD_SET_FLAG	0x00000001
/** \def PRIVATE_CMD_GET_FLAG
 * \brief Bitmaks of bit which indicates that the Command is GET Command
 */
#define PRIVATE_CMD_GET_FLAG	0x00000002

/** \struct ti_private_cmd_t
 * \brief TI Private Command
 * 
 * \par Description
 * This Struct defines the Parameters needed for performing TI Private Command 
 * 
 * \sa
 */
typedef struct
{
	u32 	cmd;   			/**< Number of command to execute - configMgr parameter name			*/
	u32 	flags; 			/**< Command action type (PRIVATE_CMD_SET_FLAG | PRIVATE_CMD_GET_FLAG) 	*/
	void* 	in_buffer; 		/**< Pointer to Input Buffer											*/
	u32	in_buffer_len; 		/**< Input buffer length 												*/
	void* 	out_buffer;		/**< Pointer to Output buffer											*/
	u32	out_buffer_len;		/**< Output buffer length 												*/
} ti_private_cmd_t; 

/* located in CUDK/configurationutility/src/cu_common.c */
typedef struct 
{
    u32 temp; /* the place of the IE's id and length */
    u32 MissedBeacons;
    u8 snr;       /* The current average SNR in db - For Data Packets*/
    u8 snrBeacon; /* The current average SNR in db - For Beacon Packets*/
    u8 rssi;      /* The current average RSSI  - For Data Packets*/
    u8 rssiBeacon;  
}TCuCommon_RoamingStatisticsTable;

/* located in TWD/TWDriver/TWDriverScan.h */
#define MAX_NUMBER_OF_CHANNELS_PER_SCAN                     16
#define SCAN_MAX_NUM_OF_NORMAL_CHANNELS_PER_COMMAND         MAX_NUMBER_OF_CHANNELS_PER_SCAN
#define SCAN_MAX_NUM_OF_SPS_CHANNELS_PER_COMMAND            16
#define SCAN_DEFAULT_MIN_CHANNEL_DWELL_TIME                 30000
#define SCAN_DEFAULT_MAX_CHANNEL_DWELL_TIME                 60000
#define SCAN_DEFAULT_EARLY_TERMINATION_EVENT                SCAN_ET_COND_DISABLE
#define SCAN_DEFAULT_EARLY_TERMINATION_NUM_OF_FRAMES        0

#define PERIODIC_SCAN_MAX_SSID_NUM      		8
#define PERIODIC_SCAN_MAX_INTERVAL_NUM  		16
#define PERIODIC_SCAN_MAX_CHANNEL_NUM   		37 /* G-14 + A-23 */

#define MAX_SSID_LEN  					32
/** \enum EScanType
 * \brief Scan Type
 *
 * \par Description
 * This Enumeration defines the available scan types.
 *
 * \sa TFileInfo
 */
typedef enum
{
/*	0	*/	SCAN_TYPE_NORMAL_PASSIVE = 0,   /**< Normal passive scan 	*/
/*	1	*/	SCAN_TYPE_NORMAL_ACTIVE,        /**< Normal active scan 	*/
/*	2	*/	SCAN_TYPE_SPS,                  /**< Scheduled Passive scan */
/*	3	*/	SCAN_TYPE_TRIGGERED_PASSIVE,    /**< Triggered Passive scan */
/*	4	*/	SCAN_TYPE_TRIGGERED_ACTIVE,     /**< Triggered Active scan 	*/
/*	5	*/	SCAN_TYPE_NO_SCAN,              /**< No Scan to perform 	*/
/*	6	*/	SCAN_TYPE_PACTSIVE              /**< Passive + Active Scan (used for DFS - driver internal use only!!!) */

} EScanType;

/* located in stad/Export_Inc/TI_IPC_Api.h */
#define MAX_EVENT_DATA_SIZE 128
typedef struct _IPC_EV_DATA * PIPC_EV_DATA;  
typedef s32 (*TI_EVENT_CALLBACK) (PIPC_EV_DATA  pData);
typedef struct _IPC_EVENT_PARAMS
{
    u32            uEventType;
    void*          uEventID;
    u32            uProcessID;
    u32            uDeliveryType;
    void*          hUserParam;            /* Handle to back reference*/
    TI_EVENT_CALLBACK   pfEventCallback;
}IPC_EVENT_PARAMS;

/* EvParams are assumed to be the first field. Any addtions shoild be made 
    afterwards
 */
typedef struct _IPC_EV_DATA
{
    IPC_EVENT_PARAMS    EvParams;
    u32            uBufferSize;
    u8             uBuffer[MAX_EVENT_DATA_SIZE];
}IPC_EV_DATA;

enum
{
    IPC_EVENT_ASSOCIATED = 0,
    IPC_EVENT_DISASSOCIATED,
    IPC_EVENT_LINK_SPEED,
    IPC_EVENT_AUTH_SUCC,
    IPC_EVENT_SCAN_REPORT,
    IPC_EVENT_SCAN_COMPLETE,
    IPC_EVENT_TIMEOUT,
    IPC_EVENT_CCKM_START,
    IPC_EVENT_MEDIA_SPECIFIC,
    IPC_EVENT_MAX_OS_EVENT = IPC_EVENT_MEDIA_SPECIFIC,
    IPC_EVENT_EAPOL,
    IPC_EVENT_BOUND,
    IPC_EVENT_UNBOUND,
    IPC_EVENT_PREAUTH_EAPOL,
    IPC_EVENT_RESERVED2,
    IPC_EVENT_LOW_RSSI,
    IPC_EVENT_TSPEC_STATUS,
    IPC_EVENT_TSPEC_RATE_STATUS,
    IPC_EVENT_MEDIUM_TIME_CROSS,
    IPC_EVENT_ROAMING_COMPLETE,
    IPC_EVENT_EAP_AUTH_FAILURE,
    IPC_EVENT_WPA2_PREAUTHENTICATION,
    IPC_EVENT_TRAFFIC_INTENSITY_THRESHOLD_CROSSED,
    IPC_EVENT_GWSI,
    IPC_EVENT_WPS_SESSION_OVERLAP,
    IPC_EVENT_RSSI_SNR_TRIGGER_0,
    IPC_EVENT_RSSI_SNR_TRIGGER_1,
    IPC_EVENT_LOGGER,
    IPC_EVENT_NOT_ASSOCIATED,
    IPC_EVENT_MAX
};

/* located in stad/Export_Inc/coreDefaultParams.h */
/** \enum PowerMode_e */
/* MUST be sync with OS_802_11_POWER_PROFILE */
typedef enum 
{
    POWER_MODE_AUTO,        /**< In this mode the power manager module is toggle states
                             * (ACTIVE, SHORT_DOZE and LONG_DOZE) by its own inner algorithm.
                             */

    POWER_MODE_ACTIVE,      /**< In this mode there is no power save, the host interface & the radio
                             * is always active. The TNET is constantly awake. This mode is used,
                             * for example, when the device is powered from an AC power source,
                             * and provides maximum throughput and minimal latency.
                             */

    POWER_MODE_SHORT_DOZE,  /**< In this mode the system is going to ELP state and awakes (by the
                             * FW) every beacon. The F/W wakes up the host on every Beacon passes
                             * the Beacon to the driver and returns to ELP Doze as soon as possible.
                             */

    POWER_MODE_LONG_DOZE,    /**< In this mode the system is going to ELP state and awakes (by the
                             * FW) every DTIM or listen interval. This mode consumes low power,
                             * while still waking-up for Beacons once in a while. The system spends
                             * a lot of time in ELP-Doze, and the F/W rarely wakes up the host.
                             */

    POWER_MODE_PS_ONLY,     /**< In this mode the system is setting the Ps as ON. 
                             * the ELP state is changing to SHORT or LONG DOZE (According to last configuration). 
                             * Auto mode won't be used here.
                             */

    POWER_MODE_MAX
}PowerMgr_PowerMode_e;

/** \enum PowerMgr_Priority_e */
typedef enum 
{
    POWER_MANAGER_USER_PRIORITY,           /**< indicates the default user priority. */
    POWER_MANAGER_SG_PRIORITY,             /**< Indicate the Soft Gemini priority */
    POWER_MANAGER_PS_POLL_FAILURE_PRIORITY,/**< After receiving the PsPoll failure event */
    POWER_MANAGER_MAX_PRIORITY                                                      
}PowerMgr_Priority_e;

/* located in stad/Export_Inc/STADExternalIf.h */
/** \struct TPowerMgr_PowerMode
 * \brief Power Mode Parameters
 * 
 * \par Description
 * 
 * \sa
 */
typedef struct 
{
    PowerMgr_PowerMode_e    PowerMode;		/**< Power Mode	Type		*/
    PowerMgr_Priority_e     PowerMngPriority; 	/**< Power Mode	Priority	*/
} TPowerMgr_PowerMode;

/* located in platforms/os/common/inc/convert.h */
#define TIWLN_802_11_SUPPORTED_RATES                    SITE_MGR_DESIRED_SUPPORTED_RATE_SET_PARAM
#define TIWLN_802_11_SUPPORTED_RATES_SET                SITE_MGR_DESIRED_SUPPORTED_RATE_SET_PARAM
#define TIWLN_802_11_CURRENT_RATES_GET                  SITE_MGR_CURRENT_TX_RATE_PARAM
#define TIWLN_802_11_CHANNEL_GET                        SITE_MGR_CURRENT_CHANNEL_PARAM /* wext in linux */
#define TIWLN_REG_DOMAIN_ENABLE_DISABLE_802_11D         REGULATORY_DOMAIN_ENABLE_DISABLE_802_11D
#define TIWLN_REG_DOMAIN_ENABLE_DISABLE_802_11H         REGULATORY_DOMAIN_ENABLE_DISABLE_802_11H
#define TIWLN_REG_DOMAIN_GET_802_11D                    REGULATORY_DOMAIN_ENABLED_PARAM
#define TIWLN_REG_DOMAIN_GET_802_11H                    REGULATORY_DOMAIN_MANAGEMENT_CAPABILITY_ENABLED_PARAM
#define TIWLN_REG_DOMAIN_GET_COUNTRY_2_4	        REGULATORY_DOMAIN_COUNTRY_2_4_PARAM
#define TIWLN_REG_DOMAIN_SET_COUNTRY_2_4	        REGULATORY_DOMAIN_COUNTRY_2_4_PARAM
#define TIWLN_REG_DOMAIN_GET_COUNTRY_5	                REGULATORY_DOMAIN_COUNTRY_5_PARAM
#define TIWLN_REG_DOMAIN_SET_COUNTRY_5	                REGULATORY_DOMAIN_COUNTRY_5_PARAM
#define TIWLN_REG_DOMAIN_SET_DFS_RANGE	                REGULATORY_DOMAIN_DFS_CHANNELS_RANGE
#define TIWLN_REG_DOMAIN_GET_DFS_RANGE	                REGULATORY_DOMAIN_DFS_CHANNELS_RANGE
#define TIWLN_802_11_POWER_MODE_GET	                POWER_MGR_POWER_MODE
#define TIWLN_802_11_POWER_MODE_SET	                POWER_MGR_POWER_MODE
#define TIWLN_802_11_RSSI                       	TWD_RSSI_LEVEL_PARAM
#define TIWLN_802_11_TX_POWER_DBM_GET           	REGULATORY_DOMAIN_CURRENT_TX_POWER_IN_DBM_PARAM
#define TIWLN_802_11_POWER_MGR_PROFILE          	POWER_MGR_POWER_MODE
                                                        
#define TIWLN_SHORT_SLOT_GET	                        SITE_MGR_DESIRED_SLOT_TIME_PARAM
#define TIWLN_SHORT_SLOT_SET	                        SITE_MGR_DESIRED_SLOT_TIME_PARAM
#define TIWLN_IBSS_PROTECTION_GET               	CTRL_DATA_CURRENT_IBSS_PROTECTION_PARAM /* not implemented in CUDK */
#define TIWLN_IBSS_PROTECTION_SET	                CTRL_DATA_CURRENT_IBSS_PROTECTION_PARAM /* not implemented in CUDK */
#define TIWLN_802_11_MIXED_MODE_SET	                RSN_MIXED_MODE
#define TIWLN_802_11_MIXED_MODE_GET	                RSN_MIXED_MODE
                                                        
#define TIWLN_802_11_GET_AP_QOS_PARAMS	                QOS_MNGR_AP_QOS_PARAMETERS
#define TIWLN_802_11_GET_AP_QOS_CAPABILITIES	        SITE_MGR_GET_AP_QOS_CAPABILITIES
#define TIWLN_802_11_ADD_TSPEC	                        QOS_MNGR_ADD_TSPEC_REQUEST
#define TIWLN_802_11_GET_TSPEC_PARAMS	                QOS_MNGR_OS_TSPEC_PARAMS
#define TIWLN_802_11_DELETE_TSPEC               	QOS_MNGR_DEL_TSPEC_REQUEST
#define TIWLN_802_11_GET_CURRENT_AC_STATUS	        QOS_MNGR_AC_STATUS
#define TIWLN_802_11_SET_MEDIUM_USAGE_THRESHOLD 	TX_CTRL_SET_MEDIUM_USAGE_THRESHOLD
#define TIWLN_802_11_GET_MEDIUM_USAGE_THRESHOLD 	TX_CTRL_GET_MEDIUM_USAGE_THRESHOLD
#define TIWLN_802_11_GET_DESIRED_PS_MODE                QOS_MNGR_GET_DESIRED_PS_MODE
#define TIWLN_802_11_SET_RX_TIMEOUT                     QOS_SET_RX_TIME_OUT
#define TIWLN_802_11_POWER_LEVEL_DEFAULT_GET            POWER_MGR_POWER_LEVEL_DEFAULT
#define TIWLN_802_11_POWER_LEVEL_DEFAULT_SET            POWER_MGR_POWER_LEVEL_DEFAULT
#define TIWLN_802_11_POWER_LEVEL_PS_SET                 POWER_MGR_POWER_LEVEL_PS
#define TIWLN_802_11_POWER_LEVEL_PS_GET                 POWER_MGR_POWER_LEVEL_PS
#define TIWLN_802_11_BEACON_FILTER_DESIRED_STATE_SET    SITE_MGR_BEACON_FILTER_DESIRED_STATE_PARAM
#define TIWLN_802_11_BEACON_FILTER_DESIRED_STATE_GET    SITE_MGR_BEACON_FILTER_DESIRED_STATE_PARAM
#define TIWLN_802_11_POWER_LEVEL_DOZE_MODE_GET          POWER_MGR_POWER_LEVEL_DOZE_MODE
#define TIWLN_802_11_POWER_LEVEL_DOZE_MODE_SET          POWER_MGR_POWER_LEVEL_DOZE_MODE
#define TIWLN_802_11_SHORT_PREAMBLE_GET                 SITE_MGR_DESIRED_PREAMBLE_TYPE_PARAM
#define TIWLN_802_11_SHORT_PREAMBLE_SET                 SITE_MGR_DESIRED_PREAMBLE_TYPE_PARAM
#define TIWLN_ENABLE_DISABLE_RX_DATA_FILTERS            RX_DATA_ENABLE_DISABLE_RX_DATA_FILTERS
#define TIWLN_ADD_RX_DATA_FILTER                        RX_DATA_ADD_RX_DATA_FILTER
#define TIWLN_REMOVE_RX_DATA_FILTER                     RX_DATA_REMOVE_RX_DATA_FILTER
#define TIWLN_GET_RX_DATA_FILTERS_STATISTICS            RX_DATA_GET_RX_DATA_FILTERS_STATISTICS
#define TIWLN_REPORT_MODULE_SET                         REPORT_MODULE_TABLE_PARAM
#define TIWLN_REPORT_MODULE_GET                         REPORT_MODULE_TABLE_PARAM
#define TIWLN_REPORT_SEVERITY_SET                       REPORT_SEVERITY_TABLE_PARAM
#define TIWLN_REPORT_SEVERITY_GET                       REPORT_SEVERITY_TABLE_PARAM
#define TIWLN_DISPLAY_STATS                             DEBUG_ACTIVATE_FUNCTION
#define TIWLN_802_11_GET_SELECTED_BSSID_INFO            SITE_MGR_GET_SELECTED_BSSID_INFO
#define TIWLN_802_11_TX_STATISTICS                      TX_CTRL_COUNTERS_PARAM
#define TIWLN_802_11_SET_TRAFFIC_INTENSITY_THRESHOLDS   CTRL_DATA_TRAFFIC_INTENSITY_THRESHOLD
#define TIWLN_802_11_GET_TRAFFIC_INTENSITY_THRESHOLDS   CTRL_DATA_TRAFFIC_INTENSITY_THRESHOLD
#define TIWLN_802_11_TOGGLE_TRAFFIC_INTENSITY_EVENTS    CTRL_DATA_TOGGLE_TRAFFIC_INTENSITY_EVENTS
#define TIWLN_802_11_GET_PRIMARY_BSSID_INFO             SITE_MGR_PRIMARY_SITE_PARAM /* not implemented in CUDK */
#define TIWLN_ENABLE_DISABLE_RX_DATA_FILTERS            RX_DATA_ENABLE_DISABLE_RX_DATA_FILTERS
#define TIWLN_ADD_RX_DATA_FILTER                        RX_DATA_ADD_RX_DATA_FILTER
#define TIWLN_REMOVE_RX_DATA_FILTER                     RX_DATA_REMOVE_RX_DATA_FILTER
#define TIWLN_GET_RX_DATA_FILTERS_STATISTICS            RX_DATA_GET_RX_DATA_FILTERS_STATISTICS /* not implemented in CUDK */
#define TIWLN_802_11_START_APP_SCAN_SET                 SCAN_CNCN_START_APP_SCAN           
#define TIWLN_802_11_STOP_APP_SCAN_SET                  SCAN_CNCN_STOP_APP_SCAN
#define TIWLN_802_11_SCAN_POLICY_PARAM_SET              SCAN_MNGR_SET_CONFIGURATION
#define TIWLN_802_11_SCAN_BSS_LIST_GET                  SCAN_MNGR_BSS_LIST_GET
#define TIWLN_802_11_SET_QOS_PARAMS                     QOS_MNGR_SET_OS_PARAMS
#define TIWLN_802_11_CONFIG_TX_CLASS                    CTRL_DATA_CLSFR_CONFIG
#define TIWLN_802_11_REMOVE_CLSFR_ENTRY                 CTRL_DATA_CLSFR_REMOVE_ENTRY

/* located in TWD/TWDriver/TWDriver.h */
/** \enum ETwdParam
 * \brief TWD Control parameter ID
 * 
 * \par Description
 * FW Parmaeter Information Identifier
 * 
 * \sa TWD_SetParam, TWD_GetParam
 */
typedef enum
{
/*	0x01	*/  TWD_RTS_THRESHOLD_PARAM_ID          = 0x01,				/**< */
/*	0x02	*/  TWD_FRAG_THRESHOLD_PARAM_ID,					/**< */
/*	0x03	*/  TWD_COUNTERS_PARAM_ID,						/**< */
/*	0x04	*/  TWD_LISTEN_INTERVAL_PARAM_ID,					/**< */
/*	0x05	*/  TWD_BEACON_INTERVAL_PARAM_ID,					/**< */
/*	0x06	*/  TWD_TX_POWER_PARAM_ID,    						/**< */
/*	0x07	*/  TWD_CLK_RUN_ENABLE_PARAM_ID,					/**< */
/*	0x08	*/  TWD_QUEUES_PARAM_ID, 						/**< */
/*	0x09	*/  TWD_TX_RATE_CLASS_PARAM_ID,						/**< */
/*	0x0A	*/  TWD_MAX_TX_MSDU_LIFE_TIME_PARAM_ID,					/**< */
/*	0x0B	*/  TWD_MAX_RX_MSDU_LIFE_TIME_PARAM_ID,					/**< */
/*	0x0C	*/  TWD_CTS_TO_SELF_PARAM_ID,						/**< */
/*	0x0D	*/  TWD_RX_TIME_OUT_PARAM_ID,						/**< */
/*	0x0E	*/  TWD_BCN_BRC_OPTIONS_PARAM_ID,					/**< */
/*	0x0F	*/  TWD_AID_PARAM_ID,							/**< */
/*	0x10	*/  TWD_RSN_HW_ENC_DEC_ENABLE_PARAM_ID,  				/**< */
/*	0x11	*/  TWD_RSN_KEY_ADD_PARAM_ID,						/**< */
/*	0x12	*/  TWD_RSN_KEY_REMOVE_PARAM_ID,					/**< */
/*	0x13	*/  TWD_RSN_DEFAULT_KEY_ID_PARAM_ID,					/**< */
/*	0x14	*/  TWD_RSN_SECURITY_MODE_PARAM_ID,					/**< */
/*	0x15	*/  TWD_RSN_SECURITY_ALARM_CB_SET_PARAM_ID,				/**< */
/*	0x16	*/  TWD_ACX_STATISTICS_PARAM_ID,					/**< */
/*	0x17	*/  TWD_MEDIUM_OCCUPANCY_PARAM_ID,					/**< */
/*	0x18	*/  TWD_DISABLE_POWER_MANAGEMENT_AUTO_CONFIG_PARAM_ID,			/**< */
/*	0x19	*/  TWD_ENABLE_POWER_MANAGEMENT_AUTO_CONFIG_PARAM_ID,			/**< */
/*	0x1A	*/  TWD_SG_ENABLE_PARAM_ID,						/**< */
/*	0x1B	*/  TWD_SG_CONFIG_PARAM_ID,						/**< */
#ifdef XCC_MODULE_INCLUDED
/*	0x1C	*/  TWD_RSN_XCC_SW_ENC_ENABLE_PARAM_ID,					/**< */
/*	0x1D	*/  TWD_RSN_XCC_MIC_FIELD_ENABLE_PARAM_ID,				/**< */
#endif /* XCC_MODULE_INCLUDED*/
/*	0x1E	*/  TWD_TX_OP_LIMIT_PARAM_ID,						/**< */
/*	0x1F	*/  TWD_NOISE_HISTOGRAM_PARAM_ID,					/**< */
/*	0x20	*/  TWD_TSF_DTIM_MIB_PARAM_ID,						/**< */
/*	0x21	*/  TWD_REVISION_PARAM_ID,						/**< */
/*	0x22	*/  TWD_CURRENT_CHANNEL_PARAM_ID,					/**< */
/*	0x23	*/  TWD_RADIO_TEST_PARAM_ID,						/**< */
/*	0x24	*/  TWD_RSSI_LEVEL_PARAM_ID,						/**< */
/*	0x25	*/  TWD_SNR_RATIO_PARAM_ID,						/**< */
/*	0x26	*/  TWD_COEX_ACTIVITY_PARAM_ID,	    					/**< */

				/* must be last!!! */
/*	0x27	*/  TWD_LAST_PARAM_ID							/**< */
} ETwdParam;


/* located in utils/CmdInterfaceCodes.h */
/* This file contains the definitions for the parameters that can be Set/Get from outside.
    The parmeters that can be Set/Get from inside the driver only are defined in the file paramIn.h */

/****************************************************************************
                                PARAMETERS ISSUE
    Each parameter in the system is defined as UINT32. The parameter
    structue is as following:

 bit   31   30 - 24     23    22 - 16    15 - 8       7 - 0
    +-----+----------+-----+----------+-----------+-----------+
    | Set | Reserved | Get | Reserved | Module    | Parameter |
    | bit |          | bit |          | number    | number    |
    +-----+----------+-----+----------+-----------+-----------+

  The 'set' bit indicates whteher this parameter can be set from OS abstraction layer.
  The 'get' bit indicates whteher this parameter can be get from OS abstraction layer.
  (All the parameters can be Get/Set from insied the driver.)
  The module number indicated who is the oner of the parameter.
  The parameter number is the parameter unique number used to identify it.

****************************************************************************/

/** \def SET_BIT
 * \brief Bitmaks of bit which indicates if the Command is SET Command
 */
#define	SET_BIT         			0x08000000
/** \def GET_BIT
 * \brief Bitmaks of bit which indicates if the Command is GET Command
 */
#define	GET_BIT				        0x00800000
/** \def ASYNC_PARAM
 * \brief Bitmaks of bit which indicates if the access to the Command Parameter is Async
 */
#define ASYNC_PARAM				0x00010000
/** \def ALLOC_NEEDED_PARAM
 * \brief Bitmaks of bit which indicates if that the data is not allocated in the paramInfo structure
 */
#define ALLOC_NEEDED_PARAM			0x00020000


/** \def GET_PARAM_MODULE_NUMBER
 * \brief Macro which gets the Parameter's Module Number from the second byte of x \n
 * x should be taken from Module Parameters Enumeration
 * sa EModuleParam
 */
#define GET_PARAM_MODULE_NUMBER(x)  		((x & 0x0000FF00) >> 8)
/** \def IS_PARAM_ASYNC
 * \brief Macro which returns True if access to the Command Parameter is Async \n
 * Otherwise returns False
 */
#define IS_PARAM_ASYNC(x)			(x & ASYNC_PARAM)
/** \def IS_ALLOC_NEEDED_PARAM
 * \brief Macro which returns True if data is not allocated in the paramInfo structure \n
 * (there is a need to allocate memory for data). Otherwise returns False
 */
#define IS_ALLOC_NEEDED_PARAM(x)		(x & ALLOC_NEEDED_PARAM)	
/** \def IS_PARAM_FOR_MODULE
 * \brief Macro which returns True if input param is for input module. \n
 * Otherwise returns False
 */
#define IS_PARAM_FOR_MODULE(param, module)   	((param & 0x0000FF00) == module)

/** \enum EModuleParam
 * \brief Modules Parameters ID
 * 
 * \par Description
 * This Enumeration defines all available Modules numbers. \n
 * Note that the actual number is held in the second byte (E.g. 0x0000FF00). \n
 * According to these numbers it is decided to which Module the Command Parameter is destined
 * 
 * \sa
 */
/* NOTICE! whenever you add a module, you have to increment MAX_PARAM_MODULE_NUMBER as well!!! */
typedef enum
{
    DRIVER_MODULE_PARAM               	= 0x0000,	/**< Driver Module Number						*/
    AUTH_MODULE_PARAM               	= 0x0100,	/**< Authentication Module Number					*/
    ASSOC_MODULE_PARAM              	= 0x0200,	/**< Association Module Number	   					*/
    RX_DATA_MODULE_PARAM            	= 0x0300,	/**< RX Data Module Number						*/
    TX_CTRL_MODULE_PARAM            	= 0x0400,	/**< TX Control Module Number						*/
    CTRL_DATA_MODULE_PARAM          	= 0x0500,	/**< Control Data Module Number						*/
    SITE_MGR_MODULE_PARAM           	= 0x0600,	/**< Site Manager Module Number						*/
    CONN_MODULE_PARAM               	= 0x0700,	/**< Connection Module Number						*/
    RSN_MODULE_PARAM                	= 0x0800,	/**< Robust Security NW (RSN) Module Number				*/
    ADM_CTRL_MODULE_PARAM           	= 0x0900,	/**< ADM Control Module Number						*/
    TWD_MODULE_PARAM                	= 0x0A00,	/**< Report Module Number						*/
    REPORT_MODULE_PARAM             	= 0x0B00,	/**< Report Module Number						*/
    SME_MODULE_PARAM                    = 0x0C00,	/**< SME Module Number							*/
    MLME_SM_MODULE_PARAM            	= 0x0D00,	/**< 802.11 MLME State-Machine Module Number  				*/
    REGULATORY_DOMAIN_MODULE_PARAM  	= 0x0E00,	/**< Regulatory Domain Module Number 					*/
    MEASUREMENT_MODULE_PARAM        	= 0x0F00,	/**< Measurement Module Number						*/
    XCC_MANAGER_MODULE_PARAM        	= 0x1000,	/**< XCC Manager Module Number 						*/
    ROAMING_MANAGER_MODULE_PARAM    	= 0x1100,	/**< Roaming Manager Module Number					*/
    SOFT_GEMINI_PARAM               	= 0x1200,	/**< Soft Gemini Module Number						*/
    QOS_MANAGER_PARAM               	= 0x1300,	/**< Quality Of Service (QoS) Manager Module Number			*/
    POWER_MANAGER_PARAM             	= 0x1400,	/**< Power Manager Module Number					*/
    SCAN_CNCN_PARAM                 	= 0x1500,	/**< Scan Concentrator Module Number					*/
    SCAN_MNGR_PARAM                 	= 0x1600,	/**< Scan Manager Module Number						*/
    MISC_MODULE_PARAM			= 0x1700,	/**< Misc. Module Number						*/
    HEALTH_MONITOR_MODULE_PARAM         = 0x1800,	/**< Health Monitor Module Number					*/

    /*
    Last module - DO NOT TOUCH!
    */
    MODULE_PARAM_LAST_MODULE				/**< LAst Module - Dummy, mast be last					*/

}   EModuleParam;

/** \def MAX_PARAM_MODULE_NUMBER
 * \brief Macro which returns the number of Parameters Modules
 */
#define MAX_PARAM_MODULE_NUMBER             (GET_PARAM_MODULE_NUMBER(MODULE_PARAM_LAST_MODULE))



/** \enum EExternalParam
 * \brief External Parameters
 * 
 * \par Description
 * This Enumeation includes all the eaxternal parameters numbers which are used for Get/Set Commands. 
 * Each module can have 256 parameters	\n
 * PARAMETERS ISSUE:	\n
 * Each parameter in the system is defined as UINT32. The parameter structue is as following:	\n
 * bit 0 - 7: 		Parameter Number - number of parameter inside Module\n
 * bit 8 - 15:		Module number - number of Module\n
 * bit 16:			Async Bit - indicates if command is Async (ON) or Sync (OFF)\n
 * bit 17:			Allocate Bit - indicates if allocation should be done for parameter (ON) or not (OFF)\n
 * bit 18 - 22:		Reserved			\n
 * bit 23:			Get Bit	- indicates if command is Get (ON) or not (OFF)	\n
 * bit 24 - 26:		Reserved	\n
 * bit 27:			Set Bit	- indicates if command is Set (ON) or not (OFF)	\n
 * bit 28 - 31:		Reserved	\n\n
 * The 'set' bit indicates whteher this parameter can be set from OS abstraction layer.
 * The 'get' bit indicates whteher this parameter can be get from OS abstraction layer.
 * (All the parameters can be Get/Set from insied the driver.)
 * The module number indicated who is the oner of the parameter.
 * The parameter number is the parameter unique number used to identify it.
 *
 * \sa
 */
 /* bit | 31 - 28  | 27  | 26 - 24  |  23 | 22 - 18  |    17    |  16   |  15 - 8   |   7 - 0   |
 *      +----------+-----+----------+-----+----------+----------+-------+-----------+-----------+
 *      | Reserved | Set | Reserved | Get | Reserved | Allocate | Async | Module    | Parameter |
 *      |          | bit |          | bit |          |    bit   |  bit  | number    | number    |
 *      +----------+-----+----------+-----+----------+----------+-------+-----------+-----------+
 */
typedef enum
{
	/* Driver General section */
    DRIVER_INIT_PARAM                           =	SET_BIT | DRIVER_MODULE_PARAM | 0x00,	/**< Driver Init Parameter (Driver General Set Command): \n
																									* Used for setting driver defaults. Done Sync with no memory allocation\n 
																									* Parameter Number:	0x00\n
																									* Module Number: Driver Module Number \n
																									* Async Bit: OFF	\n
																									* Allocate Bit: OFF	\n
																									* GET Bit: OFF	\n
																									* SET Bit: ON	\n
																									*/
    DRIVER_START_PARAM                          =	SET_BIT | DRIVER_MODULE_PARAM | 0x01, /**< Driver Start Parameter (Driver General Set Command): \n
																									* Used for Starting Driver. Done Sync with no memory allocation\n 
																									* Parameter Number:	0x01\n
																									* Module Number: Driver Module Number \n
																									* Async Bit: OFF	\n
																									* Allocate Bit: OFF	\n
																									* GET Bit: OFF	\n
																									* SET Bit: ON	\n
																									*/
    DRIVER_STOP_PARAM                           =	SET_BIT | DRIVER_MODULE_PARAM | 0x02, /**< Driver Stop Parameter (Driver General Set Command): \n
																									* Used for Stopping Driver. Done Sync with no memory allocation \n
																									* Parameter Number:	0x02\n
																									* Module Number: Driver Module Number \n
																									* Async Bit: OFF	\n
																									* Allocate Bit: OFF	\n
																									* GET Bit: OFF	\n
																									* SET Bit: ON	\n
																									*/
    DRIVER_STATUS_PARAM                         =	GET_BIT | DRIVER_MODULE_PARAM | 0x03, /**< Driver Status Parameter (Driver General Get Command): \n
																									* Used for Getting Driver's Status (if running). Done Sync with no memory allocation\n																														Done Sync with no memory allocation\n 
																									* Parameter Number:	0x03\n
																									* Module Number: Driver Module Number \n
																									* Async Bit: OFF	\n
																									* Allocate Bit: OFF	\n
																									* GET Bit: ON	\n
																									* SET Bit: OFF	\n
																									*/
    DRIVER_THREADID_PARAM                       =	GET_BIT | DRIVER_MODULE_PARAM | 0x04, /**< Driver Thread ID Parameter (Driver General Get Command): \n
																									* Used for Getting Driver's Thread ID. Done Sync with no memory allocation\n 
																									* Parameter Number:	0x04\n
																									* Module Number: Driver Module Number \n
																									* Async Bit: OFF	\n
																									* Allocate Bit: OFF	\n
																									* GET Bit: ON	\n
																									* SET Bit: OFF	\n
																									*/

/* Site manager section */	
SITE_MGR_DESIRED_CHANNEL_PARAM			=	SET_BIT | GET_BIT | SITE_MGR_MODULE_PARAM | 0x01,	/**< Site Manager Desired Channel Parameter (Site Manager Module Set/Get Command):\n 
																										* Used for Setting/Getting desired Channel to/from OS abstraction layer\n 
																										* Done Sync with no memory allocation \n
																										* Parameter Number:	0x01\n
																										* Module Number: Site Manager Module Number \n
																										* Async Bit: OFF	\n
																										* Allocate Bit: OFF	\n
																										* GET Bit: ON	\n
																										* SET Bit: ON	\n
																										*/
SITE_MGR_DESIRED_SUPPORTED_RATE_SET_PARAM	=	SET_BIT | GET_BIT | SITE_MGR_MODULE_PARAM | 0x05,	/**< Site Manager Desired Supported Rate Set Parameter (Site Manager Module Set/Get Command):\n 
																										* Used for Setting/Getting Desired Supported Rate to/from OS abstraction layer\n 
																										* Done Sync with no memory allocation \n
																										* Parameter Number:	0x05\n
																										* Module Number: Site Manager Module Number \n
																										* Async Bit: OFF	\n
																										* Allocate Bit: OFF	\n
																										* GET Bit: ON	\n
																										* SET Bit: ON	\n
																										*/
SITE_MGR_DESIRED_PREAMBLE_TYPE_PARAM		= 	SET_BIT | GET_BIT | SITE_MGR_MODULE_PARAM | 0x0A,	/**< Site Manager Desired Preamble Type Parameter (Site Manager Module Set/Get Command): \n 
																										* Used for Setting/Getting Desired Preamble Type to/from OS abstraction layer\n
																										* Done Sync with no memory allocation\n 
																										* Parameter Number:	0x0A\	n
																										* Module Number: Site Manager Module Number \n
																										* Async Bit: OFF	\n
																										* Allocate Bit: OFF	\n
																										* GET Bit: ON	\n
																										* SET Bit: ON	\n
																										*/
SITE_MGR_CURRENT_CHANNEL_PARAM              = 	SET_BIT | GET_BIT | SITE_MGR_MODULE_PARAM | 0x0E,	/**< Site Manager Current Channel Parameter (Site Manager Module Set/Get Command): \n 
																										* Used for Setting/Getting Current Channel to/from OS abstraction layer\n
																										* Done Sync with no memory allocation\n 
																										* Parameter Number:	0x0E	\n
																										* Module Number: Site Manager Module Number \n
																										* Async Bit: OFF	\n
																										* Allocate Bit: OFF	\n
																										* GET Bit: ON	\n
																										* SET Bit: ON	\n
																										*/
SITE_MGR_TI_WLAN_COUNTERS_PARAM            =	GET_BIT | SITE_MGR_MODULE_PARAM | 0x14,	/**< Site Manager TI WLAN Counters Parameter (Site Manager Module Get Command): \n 
																										* Used for Getting TI WLAN Statistics Counters from OS abstraction layer\n
																										* Done Sync with no memory allocation\n 
																										* Parameter Number:	0x14	\n
																										* Module Number: Site Manager Module Number \n
																										* Async Bit: OFF	\n
																										* Allocate Bit: OFF	\n
																										* GET Bit: ON	\n
																										* SET Bit: OFF	\n
																										*/
SITE_MGR_EEPROM_VERSION_PARAM		= GET_BIT | SITE_MGR_MODULE_PARAM | 0x16,  	/**< Site Manager EEPROM Version Parameter (Site Manager Module Get Command): \n 
																										* Used for Getting EEPROM Version from FW\n
																										* Done Sync with no memory allocation\n 
																										* Parameter Number:	0x16	\n
																										* Module Number: Site Manager Module Number \n
																										* Async Bit: OFF	\n
																										* Allocate Bit: OFF	\n
																										* GET Bit: ON	\n
																										* SET Bit: OFF	\n
																										*/ 
SITE_MGR_FIRMWARE_VERSION_PARAM		= GET_BIT | SITE_MGR_MODULE_PARAM | 0x17,	/**< Site Manager FW Version Parameter (Site Manager Module Get Command): \n 
																										* Used for Getting FW Version from FW\n
																										* Done Sync with no memory allocation\n 
																										* Parameter Number:	0x17	\n
																										* Module Number: Site Manager Module Number \n
																										* Async Bit: OFF	\n
																										* Allocate Bit: OFF	\n
																										* GET Bit: ON	\n
																										* SET Bit: OFF	\n
																										*/ 
SITE_MGR_DESIRED_SLOT_TIME_PARAM	= SET_BIT | GET_BIT | SITE_MGR_MODULE_PARAM | 0x1B,	/**< Site Manager Desired Slot Time Parameter (Site Manager Module Set/Get Command): \n 
																										* Used for Getting Desired Slot Time from OS abstraction layer and Setting Desired Slot Time to FW\n
																										* Done Sync with no memory allocation\n 
																										* Parameter Number:	0x1B	\n
																										* Module Number: Site Manager Module Number \n
																										* Async Bit: OFF	\n
																										* Allocate Bit: OFF	\n
																										* GET Bit: ON	\n
																										* SET Bit: ON	\n
																										*/
SITE_MGR_GET_AP_QOS_CAPABILITIES	= GET_BIT | SITE_MGR_MODULE_PARAM | 0x2E,	/**< Site Manager Get AP QoS Cpabilities Parameter (Site Manager Module Get Command): \n  
																										* Used for Getting AP QoS Cpabilities from OS abstraction layer\n
																										* Done Sync with no memory allocation\n 
																										* Parameter Number:	0x2E	\n
																										* Module Number: Site Manager Module Number \n 
																										* Async Bit: OFF	\n
																										* Allocate Bit: OFF	\n
																										* GET Bit: ON	\n
																										* SET Bit: OFF	\n
																										*/
SITE_MGR_CURRENT_TX_RATE_PARAM		= GET_BIT | SITE_MGR_MODULE_PARAM | 0x32,	/**< Site Manager Current TX Rate Parameter (Site Manager Module Get Command): \n  
																										* Used for Getting Current TX Rate from OS abstraction layer\n
																										* Done Sync with no memory allocation\n 
																										* Parameter Number:	0x32	\n
																										* Module Number: Site Manager Module Number \n
																										* Async Bit: OFF	\n
																										* Allocate Bit: OFF	\n
																										* GET Bit: ON	\n
																										* SET Bit: OFF	\n
																										*/
SITE_MGR_BSSID_FULL_LIST_PARAM		= GET_BIT | SITE_MGR_MODULE_PARAM | 0x34,	/**< Site Manager BSSID Full List Parameter (Site Manager Module Get Command): \n  
																										* Used for Getting BSSID Full List from OS abstraction layer\n
																										* Done Sync with no memory allocation\n 
																										* Parameter Number:	0x34	\n
																										* Module Number: Site Manager Module Number \n
																										* Async Bit: OFF	\n
																										* Allocate Bit: OFF	\n
																										* GET Bit: ON	\n
																										* SET Bit: OFF	\n
																										*/
SITE_MGR_BEACON_FILTER_DESIRED_STATE_PARAM = SET_BIT | GET_BIT | SITE_MGR_MODULE_PARAM | 0x35,	/**< Site Manager Beacon Filter Desired State Parameter (Site Manager Module Set/Get Command): \n  
																										* Used for Getting Beacon Filter Desired State from OS abstraction layer or Setting Beacon Filter Desired State to FW\n
																										* Done Sync with no memory allocation\n 
																										* Parameter Number:	0x35	\n
																										* Module Number: Site Manager Module Number \n
																										* Async Bit: OFF	\n
																										* Allocate Bit: OFF	\n
																										* GET Bit: ON	\n
																										* SET Bit: ON	\n
																										*/
SITE_MGR_NETWORK_TYPE_IN_USE		= GET_BIT | SITE_MGR_MODULE_PARAM | 0x36,	/**< Site Manager NW Type in Use Parameter (Site Manager Module Get Command): \n  
																										* Used for Getting NW Type in Use from OS abstraction layer\n
																										* Done Sync with no memory allocation\n 
																										* Parameter Number:	0x36	\n
																										* Module Number: Site Manager Module Number \n
																										* Async Bit: OFF	\n
																										* Allocate Bit: OFF	\n
																										* GET Bit: ON	\n
																										* SET Bit: OFF	\n
																										*/

/* Simple Config module */
SITE_MGR_SIMPLE_CONFIG_MODE		= SET_BIT | GET_BIT | SITE_MGR_MODULE_PARAM | 0x38,	/**< Site Manager Simple Configuration Mode Parameter (Simple Configuration Module Set/Get Command): \n  
																										* Used for Setting/Getting WiFi Simple Configuration Mode\n
																										* Done Sync with no memory allocation\n 
																										* Parameter Number:	0x38	\n
																										* Module Number: Site Manager Module Number \n
																										* Async Bit: OFF	\n
																										* Allocate Bit: OFF	\n
																										* GET Bit: ON	\n
																										* SET Bit: ON	\n
																										*/
SITE_MGR_GET_PRIMARY_SITE		= GET_BIT | SITE_MGR_MODULE_PARAM | 0x40,	/**< Site Manager Get Primary Site Parameter (Simple Configuration Module Get Command): \n  
																										* Used for Getting Primary Site from OS abstraction layer\n
																										* Done Sync with no memory allocation\n 
																										* Parameter Number:	0x40	\n
																										* Module Number: Site Manager Module Number \n
																										* Async Bit: OFF	\n
																										* Allocate Bit: OFF	\n
																										* GET Bit: ON	\n
																										* SET Bit: OFF	\n
																										*/

SITE_MGR_PRIMARY_SITE_HT_SUPPORT	= GET_BIT | SITE_MGR_MODULE_PARAM | 0x41,	/**< Site Manager check if the Primary Site support HT: \n  
                                                                                                        * Used for check if the Primary Site support HT \n
                                                                                                        * Done Sync with no memory allocation\n 
                                                                                                        * Parameter Number:	0x41	\n
                                                                                                        * Module Number: Site Manager Module Number \n
                                                                                                        * Async Bit: OFF	\n
                                                                                                        * Allocate Bit: OFF	\n
                                                                                                        * GET Bit: ON	\n
                                                                                                        * SET Bit: OFF	\n
                                                                                                        */
/* CTRL data section */
CTRL_DATA_CURRENT_BSS_TYPE_PARAM	= SET_BIT | GET_BIT | CTRL_DATA_MODULE_PARAM | 0x04,	/**< Control Data Primary BSS Type Parameter (Control Data Module Set/Get Command): \n  
																										* Used for Setting/Getting Primary BSS Type to/form Control Data Parameters\n
																										* Done Sync with no memory allocation\n 
																										* Parameter Number:	0x04	\n
																										* Module Number: Control Data Module Number \n
																										* Async Bit: OFF	\n
																										* Allocate Bit: OFF	\n
																										* GET Bit: ON	\n
																										* SET Bit: ON	\n
																										*/
CTRL_DATA_MAC_ADDRESS			=  GET_BIT | CTRL_DATA_MODULE_PARAM | 0x08,	/**< Control Data MAC Address Parameter (Control Data Module Get Command): \n  
																										* Used for Getting MAC Address form FW\n
																										* Done Sync with no memory allocation\n 
																										* Parameter Number:	0x08	\n
																										* Module Number: Control Data Module Number \n
																										* Async Bit: OFF	\n
																										* Allocate Bit: OFF	\n
																										* GET Bit: ON	\n
																										* SET Bit: OFF	\n
																										*/
CTRL_DATA_CLSFR_TYPE                        = GET_BIT | CTRL_DATA_MODULE_PARAM | 0x0D,	/**< Control Data Classifier Type Parameter (Control Data Module Set/Get Command): \n  
																										* Used for Setting/Getting Classifier Type to/form Control Data (TX Data Queue) Parameters\n
																										* Done Sync with no memory allocation\n 
																										* Parameter Number:	0x0D	\n
																										* Module Number: Control Data Module Number \n
																										* Async Bit: OFF	\n
																										* Allocate Bit: OFF	\n
																										* GET Bit: ON	\n
																										* SET Bit: OFF	\n
																										*/
CTRL_DATA_CLSFR_CONFIG                      = SET_BIT | CTRL_DATA_MODULE_PARAM | 0x0E,	/**< Control Data Classifier Configure Parameter (Control Data Module Set Command): \n  
																										* Used for adding Classifier entry to Control Data (TX Data Queue) Parameters\n
																										* Done Sync with no memory allocation\n 
																										* Parameter Number:	0x0E	\n
																										* Module Number: Control Data Module Number \n
																										* Async Bit: OFF	\n
																										* Allocate Bit: OFF	\n
																										* GET Bit: OFF	\n
																										* SET Bit: ON	\n
																										*/
CTRL_DATA_CLSFR_REMOVE_ENTRY			= SET_BIT | CTRL_DATA_MODULE_PARAM | 0x0F,	/**< Control Data Classifier Configure Parameter (Control Data Module Set Command): \n  
																										* Used for removing Classifier entry from Control Data (TX Data Queue) Parameters\n
																										* Done Sync with no memory allocation\n 
																										* Parameter Number:	0x0F	\n
																										* Module Number: Control Data Module Number \n
																										* Async Bit: OFF	\n
																										* Allocate Bit: OFF	\n
																										* GET Bit: OFF	\n
																										* SET Bit: ON	\n
																										*/
CTRL_DATA_TRAFFIC_INTENSITY_THRESHOLD		= SET_BIT | GET_BIT | CTRL_DATA_MODULE_PARAM | 0x15,	/**< Control Data Traffic Intensity Threshold Parameter (Control Data Module Set/Get Command): \n  
																										* Used for Setting/Getting Traffic Intensity Threshold to/from Control Data (Traffic Intensity Threshold) Parameters\n
																										* Done Sync with no memory allocation\n 
																										* Parameter Number:	0x15	\n
																										* Module Number: Control Data Module Number \n
																										* Async Bit: OFF	\n
																										* Allocate Bit: OFF	\n
																										* GET Bit: ON	\n
																										* SET Bit: ON	\n
																										*/
CTRL_DATA_TOGGLE_TRAFFIC_INTENSITY_EVENTS	= SET_BIT | CTRL_DATA_MODULE_PARAM | 0x16,	/**< Control Data Toggle Traffic Intensity Events Parameter (Control Data Module Set Command): \n  
																										* Used for Toggle Traffic Intensity Events (turns ON/OFF traffic intensity notification events)	\n
																										* Done Sync with no memory allocation\n 
																										* Parameter Number:	0x16	\n
																										* Module Number: Control Data Module Number \n
																										* Async Bit: OFF	\n
																										* Allocate Bit: OFF	\n
																										* GET Bit: OFF	\n
																										* SET Bit: ON	\n
																										*/

/* SME SM section */    
SME_DESIRED_SSID_ACT_PARAM                  = SET_BIT | GET_BIT | SME_MODULE_PARAM | 0x01,		/**< SME Set SSID and start connection process (SME Module Set/Get Command): \n  
																										* Used for set SSID and start connection or get current SSID \n 
																										* Parameter Number:	0x01 \n
																										* Module Number: SME Module Number \n
																										* Async Bit: OFF \n
																										* Allocate Bit: OFF	\n
																										* GET Bit: ON	\n
																										* SET Bit: ON	\n
																										*/

SME_RADIO_ON_PARAM                          = SET_BIT | GET_BIT | SME_MODULE_PARAM | 0x03,		/**< SME State-Machine Radio ON Parameter (SME Module Set/Get Command): \n  
																										* Used for Setting new and generating State-Machine Event, or Getting current Radio ON\n
																										* Done Sync with no memory allocation\n 
																										* Parameter Number:	0x03	\n
																										* Module Number: SME Module Number \n
																										* Async Bit: OFF	\n
																										* Allocate Bit: OFF	\n
																										* GET Bit: ON	\n
																										* SET Bit: ON	\n
																										*/
SME_CONNECTION_MODE_PARAM                   = SET_BIT | GET_BIT | SME_MODULE_PARAM | 0x04,		/**< SME State-Machine Connection Mode Parameter (SME Module Set/Get Command): \n  
																										* Used for Setting new Connection Mode (and generating disconnect State-Machine event) or Getting current Connection Mode\n
																										* Done Sync with no memory allocation\n 
																										* Parameter Number:	0x04	\n
																										* Module Number: SME Module Number \n
																										* Async Bit: OFF	\n
																										* Allocate Bit: OFF	\n
																										* GET Bit: ON	\n
																										* SET Bit: ON	\n
																										*/
SME_WSC_PB_MODE_PARAM                       = SET_BIT | SME_MODULE_PARAM | 0x07,		/**< SME State-Machine SME on the WPS Mode Parameter (SME Module Set Command): \n  
																										* Used for updating the SME on the WPS mode\n
																										* Done Sync with no memory allocation\n 
																										* Parameter Number:	0x07	\n
																										* Module Number: SME Module Number \n
																										* Async Bit: OFF	\n
																										* Allocate Bit: OFF	\n
																										* GET Bit: OFF	\n
																										* SET Bit: ON	\n
																										*/

SME_DESIRED_SSID_PARAM                      = SET_BIT | SME_MODULE_PARAM | 0x08,		/**< SME Set SSID without start connection process (SME Module Set Command): \n  
                                                                                                        * Used for set SSID without connection \n 
                                                                                                        * Parameter Number:	0x08 \n
                                                                                                        * Module Number: SME Module Number \n
                                                                                                        * Async Bit: OFF \n
                                                                                                        * Allocate Bit: OFF	\n
                                                                                                        * GET Bit: OFF	\n
                                                                                                        * SET Bit: ON	\n
                                                                                                        */

/* Scan Concentrator section */
SCAN_CNCN_START_APP_SCAN		= SET_BIT | SCAN_CNCN_PARAM | 0x01 | ALLOC_NEEDED_PARAM,	/**< Scan Concentrator Start Application Scan Parameter (Scan Concentrator Module Set Command): \n  
																														* Used for start one-shot scan as running application scan client\n
																														* Done Sync with memory allocation\n 
																														* Parameter Number:	0x01	\n
																														* Module Number: Scan Concentrator Module Number \n
																														* Async Bit: OFF	\n
																														* Allocate Bit: ON	\n
																														* GET Bit: OFF	\n
																														* SET Bit: ON	\n
																														*/
SCAN_CNCN_STOP_APP_SCAN                     =   SET_BIT |           SCAN_CNCN_PARAM | 0x02,							/**< Scan Concentrator Stop Application Scan Parameter (Scan Concentrator Module Set Command): \n  
																														* Used for stop one-shot scan as running application scan client\n
																														* Done Sync with no memory allocation\n 
																														* Parameter Number:	0x02	\n
																														* Module Number: Scan Concentrator Module Number \n
																														* Async Bit: OFF	\n
																														* Allocate Bit: OFF	\n
																														* GET Bit: OFF	\n
																														* SET Bit: ON	\n
																														*/
    SCAN_CNCN_START_PERIODIC_SCAN               =   SET_BIT |           SCAN_CNCN_PARAM | 0x03 | ALLOC_NEEDED_PARAM,	/**< Scan Concentrator Start Periodic Scan Parameter (Scan Concentrator Module Set Command): \n  
																														* Used for start periodic scan as running application scan client\n
																														* Done Sync with memory allocation\n 
																														* Parameter Number:	0x03	\n
																														* Module Number: Scan Concentrator Module Number \n
																														* Async Bit: OFF	\n
																														* Allocate Bit: ON	\n
																														* GET Bit: OFF	\n
																														* SET Bit: ON	\n
																														*/
    SCAN_CNCN_STOP_PERIODIC_SCAN                =   SET_BIT |           SCAN_CNCN_PARAM | 0x04,							/**< Scan Concentrator Stop Periodic Scan Parameter (Scan Concentrator Module Set Command): \n  
																														* Used for stop periodic scan as running application scan client\n
																														* Done Sync with no memory allocation\n 
																														* Parameter Number:	0x04	\n
																														* Module Number: Scan Concentrator Module Number \n
																														* Async Bit: OFF	\n
																														* Allocate Bit: OFF	\n
																														* GET Bit: OFF	\n
																														* SET Bit: ON	\n
																														*/
    SCAN_CNCN_BSSID_LIST_SCAN_PARAM             =   SET_BIT |           SCAN_CNCN_PARAM | 0x05,							/**< Scan Concentrator BSSID List Scon Parameter (Scan Concentrator Module Set Command): \n  
																														* Used for start one-shot scan as running application scan client\n
																														* Done Sync with no memory allocation\n 
																														* Parameter Number:	0x05	\n
																														* Module Number: Scan Concentrator Module Number \n
																														* Async Bit: OFF	\n
																														* Allocate Bit: OFF	\n
																														* GET Bit: OFF	\n
																														* SET Bit: ON	\n
																														*/
    SCAN_CNCN_BSSID_LIST_SIZE_PARAM             =   GET_BIT |           SCAN_CNCN_PARAM | 0x06,							/**< Scan Concentrator BSSID List Size Parameter (Scan Concentrator Module Get Command): \n  
																														* Used for retrieving the size to allocate for the application scan result list\n
																														* Done Sync with no memory allocation\n 
																														* Parameter Number:	0x06	\n
																														* Module Number: Scan Concentrator Module Number \n
																														* Async Bit: OFF	\n
																														* Allocate Bit: OFF	\n
																														* GET Bit: ON	\n
																														* SET Bit: OFF	\n
																														*/
    SCAN_CNCN_BSSID_LIST_PARAM                  =   GET_BIT |           SCAN_CNCN_PARAM | 0x07,							/**< Scan Concentrator BSSID List Parameter (Scan Concentrator Module Get Command): \n  
																														* Used for retrieving the application scan result table\n
																														* Done Sync with no memory allocation\n 
																														* Parameter Number:	0x07	\n
																														* Module Number: Scan Concentrator Module Number \n
																														* Async Bit: OFF	\n
																														* Allocate Bit: OFF	\n
																														* GET Bit: ON	\n
																														* SET Bit: OFF	\n
																														*/

	/* Scan Manager module */
    SCAN_MNGR_SET_CONFIGURATION                 =	SET_BIT |           SCAN_MNGR_PARAM | 0x01 | ALLOC_NEEDED_PARAM,	/**< Scan Manager Set Configuration Parameter (Scan Manager Module Set Command): \n  
																														* Used for setting the Scan Policy\n
																														* Done Sync with memory allocation\n 
																														* Parameter Number:	0x01	\n
																														* Module Number: Scan Manager Module Number \n
																														* Async Bit: OFF	\n
																														* Allocate Bit: ON	\n
																														* GET Bit: OFF	\n
																														* SET Bit: ON	\n
																														*/
    SCAN_MNGR_BSS_LIST_GET						=			  GET_BIT | SCAN_MNGR_PARAM | 0x02 | ALLOC_NEEDED_PARAM,	/**< Scan Manager Get BSS List Parameter (Scan Manager Module Get Command): \n  
																														* Used for getting the currently available BSS list\n
																														* Done Sync with memory allocation\n 
																														* Parameter Number:	0x02	\n
																														* Module Number: Scan Manager Module Number \n
																														* Async Bit: OFF	\n
																														* Allocate Bit: ON	\n
																														* GET Bit: ON	\n
																														* SET Bit: OFF	\n
																														*/

	/* regulatory domain section */
	REGULATORY_DOMAIN_MANAGEMENT_CAPABILITY_ENABLED_PARAM	=			  GET_BIT | REGULATORY_DOMAIN_MODULE_PARAM | 0x02,					   	/**< Regulatory Domain Management Capability Enabled Parameter (Regulatory Domain Module Get Command): \n  
																																				* Used for getting indication if Spectrum Management is enabled\n
																																				* Done Sync with no memory allocation\n 
																																				* Parameter Number:	0x02	\n
																																				* Module Number: Regulatory Domain Module Number \n
																																				* Async Bit: OFF	\n
																																				* Allocate Bit: OFF	\n
																																				* GET Bit: ON	\n
																																				* SET Bit: OFF	\n
																																				*/
	REGULATORY_DOMAIN_ENABLED_PARAM							=			  GET_BIT | REGULATORY_DOMAIN_MODULE_PARAM | 0x03,						/**< Regulatory Domain Enabled Parameter (Regulatory Domain Module Get Command): \n  
																																				* Used for getting indication if regulatory domain if 802.11d is in use\n
																																				* Done Sync with no memory allocation\n 
																																				* Parameter Number:	0x03	\n
																																				* Module Number: Regulatory Domain Module Number \n
																																				* Async Bit: OFF	\n
																																				* Allocate Bit: OFF	\n
																																				* GET Bit: ON	\n
																																				* SET Bit: OFF	\n
																																				*/ 		
    REGULATORY_DOMAIN_CURRENT_TX_POWER_LEVEL_PARAM      	= 	SET_BIT | GET_BIT | REGULATORY_DOMAIN_MODULE_PARAM | 0x06,						/**< Regulatory Domain Current TX Power Level Parameter (Regulatory Domain Module Set/Get Command): \n  
																																				* Used for setting/getting current TZ Power Level\n
																																				* Done Sync with no memory allocation\n 
																																				* Parameter Number:	0x06	\n
																																				* Module Number: Regulatory Domain Module Number \n
																																				* Async Bit: OFF	\n
																																				* Allocate Bit: OFF	\n
																																				* GET Bit: ON	\n
																																				* SET Bit: ON	\n
																																				*/ 
    REGULATORY_DOMAIN_CURRENT_TX_POWER_IN_DBM_PARAM			= 	SET_BIT | GET_BIT | REGULATORY_DOMAIN_MODULE_PARAM | 0x08,						/**< Regulatory Domain Current TX Power in DBM Parameter (Regulatory Domain Module Set/Get Command): \n  
																																				* Used for setting/getting current TX Power Level in DBM\n
																																				* Done Sync with no memory allocation\n 
																																				* Parameter Number:	0x08	\n
																																				* Module Number: Regulatory Domain Module Number \n
																																				* Async Bit: OFF	\n
																																				* Allocate Bit: OFF	\n
																																				* GET Bit: ON	\n
																																				* SET Bit: ON	\n
																																				*/
    REGULATORY_DOMAIN_ENABLE_DISABLE_802_11D				=	SET_BIT |           REGULATORY_DOMAIN_MODULE_PARAM | 0x0C,					 	/**< Regulatory Domain Enable/Disable 802.11d Parameter (Regulatory Domain Module Set Command): \n  
																																				* Used for enabling/disabling 802.11d.\n
																																				* Done Sync with no memory allocation\n 
																																				* Parameter Number:	0x0C	\n
																																				* Module Number: Regulatory Domain Module Number \n
																																				* Async Bit: OFF	\n
																																				* Allocate Bit: OFF	\n
																																				* GET Bit: OFF	\n
																																				* SET Bit: ON	\n
																																				*/   	
    REGULATORY_DOMAIN_ENABLE_DISABLE_802_11H				=	SET_BIT |           REGULATORY_DOMAIN_MODULE_PARAM | 0x0D,						/**< Regulatory Domain Enable/Disable 802.11h Parameter (Regulatory Domain Module Set Command): \n  
																																				* Used for enabling/disabling 802.11h (If 802_11h is enabled, enable 802_11d as well)\n
																																				* Done Sync with no memory allocation\n 
																																				* Parameter Number:	0x0D	\n
																																				* Module Number: Regulatory Domain Module Number \n
																																				* Async Bit: OFF	\n
																																				* Allocate Bit: OFF	\n
																																				* GET Bit: OFF	\n
																																				* SET Bit: ON	\n
																																				*/ 
    REGULATORY_DOMAIN_COUNTRY_2_4_PARAM						=	SET_BIT | GET_BIT | REGULATORY_DOMAIN_MODULE_PARAM | 0x0E | ALLOC_NEEDED_PARAM,	/**< Regulatory Domain Country 2-4 Parameter (Regulatory Domain Module Set/Get Command): \n  
																																				* Used for getting Country String or setting the local country IE per band with the Country IE that was detected in the last passive scan\n
																																				* Done Sync with memory allocation\n 
																																				* Parameter Number:	0x0E	\n
																																				* Module Number: Regulatory Domain Module Number \n
																																				* Async Bit: OFF	\n
																																				* Allocate Bit: ON	\n
																																				* GET Bit: ON	\n
																																				* SET Bit: ON	\n
																																				*/		
    REGULATORY_DOMAIN_COUNTRY_5_PARAM						=	SET_BIT | GET_BIT | REGULATORY_DOMAIN_MODULE_PARAM | 0x0F | ALLOC_NEEDED_PARAM,	/**< Regulatory Domain Country 5 Parameter (Regulatory Domain Module Set/Get Command): \n  
																																				* Used for getting Country String or setting the local country IE per band with the Country IE that was detected in the last passive scan\n
																																				* Done Sync with memory allocation\n 
																																				* Parameter Number:	0x0F	\n
																																				* Module Number: Regulatory Domain Module Number \n
																																				* Async Bit: OFF	\n
																																				* Allocate Bit: ON	\n
																																				* GET Bit: ON	\n
																																				* SET Bit: ON	\n
																																				*/	
    REGULATORY_DOMAIN_DFS_CHANNELS_RANGE					=	SET_BIT | GET_BIT | REGULATORY_DOMAIN_MODULE_PARAM | 0x10,						/**< Regulatory Domain DFS Channels Parameter (Regulatory Domain Module Set/Get Command): \n  
																																				* Used for config manager in order to set/get a parameter received from the OS abstraction layer\n
																																				* Done Sync with no memory allocation\n 
																																				* Parameter Number:	0x10	\n
																																				* Module Number: Regulatory Domain Module Number \n
																																				* Async Bit: OFF	\n
																																				* Allocate Bit: OFF	\n
																																				* GET Bit: ON	\n
																																				* SET Bit: ON	\n
																																				*/	
	REGULATORY_DOMAIN_TX_POWER_LEVEL_TABLE_PARAM			=   		  GET_BIT | REGULATORY_DOMAIN_MODULE_PARAM | 0x12,						/**< Regulatory Domain TX Power Level Table Parameter (Regulatory Domain Module Get Command): \n  
																																				* Used for getting TX Power Level Table from FW\n
																																				* Done Sync with no memory allocation\n 
																																				* Parameter Number:	0x12	\n
																																				* Module Number: Regulatory Domain Module Number \n
																																				* Async Bit: OFF	\n
																																				* Allocate Bit: OFF	\n
																																				* GET Bit: ON	\n
																																				* SET Bit: OFF	\n
																																				*/		

	/* Power Manager params */
    POWER_MGR_POWER_MODE							= 	SET_BIT | GET_BIT | POWER_MANAGER_PARAM | 0x01,							/**< Power Manager Power Mode Parameter (Power Manager Module Set/Get Command): \n  
																																* Used for setting/getting the Power Mode to/from Power Manager Module\n
																																* Done Sync with no memory allocation\n 
																																* Parameter Number:	0x01	\n
																																* Module Number: Power Manager Module Number \n
																																* Async Bit: OFF	\n
																																* Allocate Bit: OFF	\n
																																* GET Bit: ON	\n
																																* SET Bit: ON	\n
																																*/
    POWER_MGR_POWER_LEVEL_PS                        =   SET_BIT | GET_BIT | POWER_MANAGER_PARAM | 0x02,							/**< Power Manager Power Level Power-Save Parameter (Power Manager Module Set/Get Command): \n  
																																* Used for getting the Power Level Power-Save from Power Manager Module or setting the Power Level Power-Save to Power Manager Module (and to FW if Power-Save is Enabled)\n
																																* Done Sync with no memory allocation\n 
																																* Parameter Number:	0x02	\n
																																* Module Number: Power Manager Module Number \n
																																* Async Bit: OFF	\n
																																* Allocate Bit: OFF	\n
																																* GET Bit: ON	\n
																																* SET Bit: ON	\n
																																*/
    POWER_MGR_POWER_LEVEL_DEFAULT                   =   SET_BIT | GET_BIT | POWER_MANAGER_PARAM | 0x03,							/**< Power Manager Power Level Default Parameter (Power Manager Module Set/Get Command): \n  
																																* Used for getting the Power Level Default from Power Manager Module or setting the Power Level Default to Power Manager Module (and to FW if Power-Save is Enabled)\n
																																* Done Sync with no memory allocation\n 
																																* Parameter Number:	0x03	\n
																																* Module Number: Power Manager Module Number \n
																																* Async Bit: OFF	\n
																																* Allocate Bit: OFF	\n
																																* GET Bit: ON	\n
																																* SET Bit: ON	\n
																																*/
    POWER_MGR_POWER_LEVEL_DOZE_MODE                 =   SET_BIT | GET_BIT | POWER_MANAGER_PARAM | 0x04,							/**< Power Manager Power Level Doze Mode (short-doze / long-doze) Parameter (Power Manager Module Set/Get Command): \n  
																																* Used for getting the Power Level Doze Mode from Power Manager Module or setting the Power Level Doze Mode to Power Manager Module (and to FW if Power-Save is Enabled)\n
																																* Done Sync with no memory allocation\n 
																																* Parameter Number:	0x04	\n
																																* Module Number: Power Manager Module Number \n
																																* Async Bit: OFF	\n
																																* Allocate Bit: OFF	\n
																																* GET Bit: ON	\n
																																* SET Bit: ON	\n
																																*/
    POWER_MGR_KEEP_ALIVE_ENA_DIS                    =   SET_BIT |           POWER_MANAGER_PARAM | 0x05,							/**< Power Manager Keep Alive Enable/Disable Parameter (Power Manager Module Set Command): \n  
																																* Used for setting the Keep Alive Enable/Disable to Power Manager and FW\n
																																* Done Sync with no memory allocation\n 
																																* Parameter Number:	0x05	\n
																																* Module Number: Power Manager Module Number \n
																																* Async Bit: OFF	\n
																																* Allocate Bit: OFF	\n
																																* GET Bit: OFF	\n
																																* SET Bit: ON	\n
																																*/
    POWER_MGR_KEEP_ALIVE_ADD_REM                    =   SET_BIT |           POWER_MANAGER_PARAM | 0x06 | ALLOC_NEEDED_PARAM,	/**< Power Manager Keep Alive add REM Parameter (Power Manager Module Set Command): \n  
																																* Used for setting addition/removal of a template and global enable/disable flag to Power Manager and FW\n
																																* Done Sync with memory allocation\n 
																																* Parameter Number:	0x06	\n
																																* Module Number: Power Manager Module Number \n
																																* Async Bit: OFF	\n
																																* Allocate Bit: ON	\n
																																* GET Bit: OFF	\n
																																* SET Bit: ON	\n
																																*/
    POWER_MGR_KEEP_ALIVE_GET_CONFIG                 =             GET_BIT | POWER_MANAGER_PARAM | 0x07 | ALLOC_NEEDED_PARAM,	/**< Power Manager Keep Alive Get Configuration Parameter (Power Manager Module Get Command): \n  
																																* Used for getting the Keep Alive current Configuration\n
																																* Done Sync with memory allocation\n 
																																* Parameter Number:	0x07	\n
																																* Module Number: Power Manager Module Number \n
																																* Async Bit: OFF	\n
																																* Allocate Bit: ON	\n
																																* GET Bit: ON	\n
																																* SET Bit: OFF	\n
																																*/

	/* Robust Security NW (RSN) section */
	RSN_ENCRYPTION_STATUS_PARAM						=	SET_BIT | GET_BIT | RSN_MODULE_PARAM | 0x04,							/**< Robust Security NW (RSN) Encryption Status Parameter (RSN Module Set/Get Command): \n  
																																* Used for setting/getting Encryption Status to/from RSN Module\n
																																* Done Sync with no memory allocation\n 
																																* Parameter Number:	0x04	\n
																																* Module Number: RSN Module Number \n
																																* Async Bit: OFF	\n
																																* Allocate Bit: OFF	\n
																																* GET Bit: ON	\n
																																* SET Bit: ON	\n
																																*/
	RSN_ADD_KEY_PARAM								=	SET_BIT | 			RSN_MODULE_PARAM | 0x05,							/**< Robust Security NW (RSN) Add Key Parameter (RSN Module Set Command): \n  
																																* Used for adding RSN Key to FW\n
																																* Done Sync with no memory allocation\n 
																																* Parameter Number:	0x05	\n
																																* Module Number: RSN Module Number \n
																																* Async Bit: OFF	\n
																																* Allocate Bit: OFF	\n
																																* GET Bit: OFF	\n
																																* SET Bit: ON	\n
																																*/
	RSN_REMOVE_KEY_PARAM							=	SET_BIT           | RSN_MODULE_PARAM | 0x06,							/**< Robust Security NW (RSN) Remove Key Parameter (RSN Module Set Command): \n  
																																* Used for removing RSN Key from FW\n
																																* Done Sync with no memory allocation\n 
																																* Parameter Number:	0x06	\n
																																* Module Number: RSN Module Number \n
																																* Async Bit: OFF	\n
																																* Allocate Bit: OFF	\n
																																* GET Bit: OFF	\n
																																* SET Bit: ON	\n
																																*/
    RSN_EXT_AUTHENTICATION_MODE                 	= 	SET_BIT | GET_BIT | RSN_MODULE_PARAM | 0x07,							/**< Robust Security NW (RSN) External Authentication Mode Parameter (RSN Module Set/Get Command): \n  
																																* Used for getting RSN External Authentication Mode from RSN Module or setting RSN External Authentication Mode to FW and RSN Module\n
																																* Done Sync with no memory allocation\n 
																																* Parameter Number:	0x07	\n
																																* Module Number: RSN Module Number \n
																																* Async Bit: OFF	\n
																																* Allocate Bit: OFF	\n
																																* GET Bit: ON	\n
																																* SET Bit: ON	\n
																																*/
	RSN_MIXED_MODE									=	SET_BIT | GET_BIT | RSN_MODULE_PARAM | 0x08,							/**< Robust Security NW (RSN) Mixed Mode Parameter (RSN Module Set/Get Command): \n  
																																* Used for setting/getting RSN Mixed Mode to/from RSN Module\n
																																* Done Sync with no memory allocation\n 
																																* Parameter Number:	0x08	\n
																																* Module Number: RSN Module Number \n
																																* Async Bit: OFF	\n
																																* Allocate Bit: OFF	\n
																																* GET Bit: ON	\n
																																* SET Bit: ON	\n
																																*/
    RSN_DEFAULT_KEY_ID								=	SET_BIT | GET_BIT | RSN_MODULE_PARAM | 0x09,							/**< Robust Security NW (RSN) Defualt Key ID Parameter (RSN Module Set/Get Command): \n  
																																* Used for getting RSN defualt Key ID from RSN Module or setting RSN defualt Key ID to FW and RSN Module\n
																																* Done Sync with no memory allocation\n 
																																* Parameter Number:	0x09	\n
																																* Module Number: RSN Module Number \n
																																* Async Bit: OFF	\n
																																* Allocate Bit: OFF	\n
																																* GET Bit: ON	\n
																																* SET Bit: ON	\n
																																*/
	RSN_XCC_NETWORK_EAP								=	SET_BIT | GET_BIT | RSN_MODULE_PARAM | 0x0A,							/**< Robust Security NW (RSN) XCC NW EAP Parameter (RSN Module Set/Get Command): \n  
																																* Used for setting/getting RSN XCC NW EAP to/from RSN Module\n
																																* Done Sync with no memory allocation\n 
																																* Parameter Number:	0x0A	\n
																																* Module Number: RSN Module Number \n
																																* Async Bit: OFF	\n
																																* Allocate Bit: OFF	\n
																																* GET Bit: ON	\n
																																* SET Bit: ON	\n
																																*/

	/* TWD Control section */
    TWD_RTS_THRESHOLD_PARAM                			=   SET_BIT | GET_BIT | TWD_MODULE_PARAM | TWD_RTS_THRESHOLD_PARAM_ID,		/**< TWD Control RTS Threshold Parameter (TWD Control Module Set/Get Command): \n  
																																* Used for getting RTS Threshold from TWD Control Module or setting RTS Threshold to FW and TWD Control Module\n
																																* Done Sync with no memory allocation\n 
																																* Parameter Number:	TWD_RTS_THRESHOLD_PARAM_ID	\n
																																* Module Number: TWD Control Module Number \n
																																* Async Bit: OFF	\n
																																* Allocate Bit: OFF	\n
																																* GET Bit: ON	\n
																																* SET Bit: ON	\n
																																*/
    TWD_FRAG_THRESHOLD_PARAM               			=   SET_BIT | GET_BIT | TWD_MODULE_PARAM | TWD_FRAG_THRESHOLD_PARAM_ID,		/**< TWD Control Fragmentation Threshold Parameter (TWD Control Module Set/Get Command): \n  
																																* Used for getting Fragmentation Threshold from TWD Control Module or setting Fragmentation Threshold to FW and TWD Control Module\n
																																* Done Sync with no memory allocation\n 
																																* Parameter Number:	TWD_FRAG_THRESHOLD_PARAM_ID	\n
																																* Module Number: TWD Control Module Number \n
																																* Async Bit: OFF	\n
																																* Allocate Bit: OFF	\n
																																* GET Bit: ON	\n
																																* SET Bit: ON	\n
																																*/
    TWD_RSSI_LEVEL_PARAM							=			  GET_BIT | TWD_MODULE_PARAM | TWD_RSSI_LEVEL_PARAM_ID 			| ASYNC_PARAM,	/**< TWD Control RSSI Level Parameter (TWD Control Module Get Command): \n  
																																				* Used for getting RSSI Level From FW\n
																																				* Done Async with no memory allocation\n 
																																				* Parameter Number:	TWD_RSSI_LEVEL_PARAM_ID	\n
																																				* Module Number: TWD Control Module Number \n
																																				* Async Bit: ON	\n
																																				* Allocate Bit: OFF	\n
																																				* GET Bit: ON	\n
																																				* SET Bit: OFF	\n
																																				*/
    TWD_SNR_RATIO_PARAM                    			=			  GET_BIT | TWD_MODULE_PARAM | TWD_SNR_RATIO_PARAM_ID			| ASYNC_PARAM,	/**< TWD Control SNR Radio Parameter (TWD Control Module Get Command): \n  
																																				* Used for getting SNR Radio From FW (same outcome as TWD_RSSI_LEVEL_PARAM)\n
																																				* Done Async with no memory allocation\n 
																																				* Parameter Number:	TWD_SNR_RATIO_PARAM_ID	\n
																																				* Module Number: TWD Control Module Number \n
																																				* Async Bit: ON	\n
																																				* Allocate Bit: OFF	\n
																																				* GET Bit: ON	\n
																																				* SET Bit: OFF	\n
																																				*/

	/*for BIP/PLT/Radio Debug Tests --> supports Set + GET*/
	TWD_RADIO_TEST_PARAM               						=   SET_BIT | GET_BIT | TWD_MODULE_PARAM | TWD_RADIO_TEST_PARAM_ID 	| ASYNC_PARAM,	/**< TWD Control SNR Radio Parameter (TWD Control Module Set/Get Command): \n  
																																				* Used for performing BIP/PLT/Radio Debug Tests\n
																																				* Done Async with no memory allocation\n 
																																				* Parameter Number:	TWD_RADIO_TEST_PARAM_ID	\n
																																				* Module Number: TWD Control Module Number \n
																																				* Async Bit: ON	\n
																																				* Allocate Bit: OFF	\n
																																				* GET Bit: ON	\n
																																				* SET Bit: ON	\n
																																				*/
	
	/* Roaming manager */
    ROAMING_MNGR_APPLICATION_CONFIGURATION		= 	SET_BIT | GET_BIT | ROAMING_MANAGER_MODULE_PARAM | 0x01,	/**< Roaming Manager Application Configuration Parameter (Roaming Manager Module Set/Get Command): \n  
																												* Used for setting/getting Roaming Manager Application Configuration to/from Roaming Manager Module and State-Machine\n
																												* Done Sync with no memory allocation\n 
																												* Parameter Number:	0x01	\n
																												* Module Number: Roaming Manager Module Number \n
																												* Async Bit: OFF	\n
																												* Allocate Bit: OFF	\n
																												* GET Bit: ON	\n
																												* SET Bit: ON	\n
																												*/
    ROAMING_MNGR_USER_DEFINED_TRIGGER     		= 	SET_BIT |           ROAMING_MANAGER_MODULE_PARAM | 0x02,	/**< Roaming Manager User Defined Trigger Parameter (Roaming Manager Module Set Command): \n  
																												* Used for setting user-defined trigger to FW\n
																												* Done Sync with no memory allocation\n 
																												* Parameter Number:	0x02	\n
																												* Module Number: Roaming Manager Module Number \n
																												* Async Bit: OFF	\n
																												* Allocate Bit: OFF	\n
																												* GET Bit: OFF	\n
																												* SET Bit: ON	\n
																												*/

	/* QOS manager params */
    QOS_MNGR_SET_OS_PARAMS						=	SET_BIT |           QOS_MANAGER_PARAM | 0x10,	/**< QoS Manager Set OS Parameter (QoS Module Set Command): \n  
																									* Used for setting Quality Of Service Manager's Parameters\n
																									* Done Sync with no memory allocation\n 
																									* Parameter Number:	0x10	\n
																									* Module Number: QoS Module Number \n
																									* Async Bit: OFF	\n
																									* Allocate Bit: OFF	\n
																									* GET Bit: OFF	\n
																									* SET Bit: ON	\n
																									*/
    QOS_MNGR_AP_QOS_PARAMETERS					=			  GET_BIT | QOS_MANAGER_PARAM | 0x11,	/**< QoS Manager AP QoS Parameter (QoS Module Get Command): \n  
																									* Used for getting current AP QoS Parameters from QoS Module\n
																									* Done Sync with no memory allocation\n 
																									* Parameter Number:	0x11	\n
																									* Module Number: QoS Module Number \n
																									* Async Bit: OFF	\n
																									* Allocate Bit: OFF	\n
																									* GET Bit: ON	\n
																									* SET Bit: OFF	\n
																									*/
    QOS_MNGR_OS_TSPEC_PARAMS					=             GET_BIT | QOS_MANAGER_PARAM | 0x12,	/**< QoS Manager OS TSPEC Parameter (QoS Module Get Command): \n  
																									* Used for getting current OS 802.11 QoS TSPEC Parameters from QoS Module\n
																									* Done Sync with no memory allocation\n 
																									* Parameter Number:	0x12	\n
																									* Module Number: QoS Module Number \n
																									* Async Bit: OFF	\n
																									* Allocate Bit: OFF	\n
																									* GET Bit: ON	\n
																									* SET Bit: OFF	\n
																									*/
    QOS_MNGR_AC_STATUS							=	SET_BIT | GET_BIT | QOS_MANAGER_PARAM | 0x13,	/**< QoS Manager AC Status Parameter (QoS Module Set/Get Command): \n  
																									* Used for setting/getting SC Status\n
																									* Done Sync with no memory allocation\n 
																									* Parameter Number:	0x13	\n
																									* Module Number: QoS Module Number \n
																									* Async Bit: OFF	\n
																									* Allocate Bit: OFF	\n
																									* GET Bit: ON	\n
																									* SET Bit: ON	\n
																									*/
    QOS_MNGR_ADD_TSPEC_REQUEST					=	SET_BIT | 			QOS_MANAGER_PARAM | 0x14,	/**< QoS Manager Add TSPEC Request Parameter (QoS Module Set Command): \n  
																									* Used for setting TSPEC Parameters to QoS Module\n
																									* Done Sync with no memory allocation\n 
																									* Parameter Number:	0x14	\n
																									* Module Number: QoS Module Number \n
																									* Async Bit: OFF	\n
																									* Allocate Bit: OFF	\n
																									* GET Bit: OFF	\n
																									* SET Bit: ON	\n
																									*/
	QOS_MNGR_DEL_TSPEC_REQUEST					=	SET_BIT           | QOS_MANAGER_PARAM | 0x15,	/**< QoS Manager Delete TSPEC Request Parameter (QoS Module Set Command): \n  
																									* Used for deleting TSPEC Parameters from QoS Module\n
																									* Done Sync with no memory allocation\n 
																									* Parameter Number:	0x15	\n
																									* Module Number: QoS Module Number \n
																									* Async Bit: OFF	\n
																									* Allocate Bit: OFF	\n
																									* GET Bit: OFF	\n
																									* SET Bit: ON	\n
																									*/
	QOS_MNGR_GET_DESIRED_PS_MODE				=             GET_BIT | QOS_MANAGER_PARAM | 0x17,	/**< QoS Manager Get Desired Power-Save Mode Parameter (QoS Module Get Command): \n  
																									* Used for getting the current desired Power-Save Mode from QoS Module\n
																									* Done Sync with no memory allocation\n 
																									* Parameter Number:	0x17	\n
																									* Module Number: QoS Module Number \n
																									* Async Bit: OFF	\n
																									* Allocate Bit: OFF	\n
																									* GET Bit: ON	\n
																									* SET Bit: OFF	\n
																									*/
    QOS_SET_RX_TIME_OUT							=	SET_BIT |			QOS_MANAGER_PARAM | 0x18, 	/**< QoS Manager Get Desired Power-Save Mode Parameter (QoS Module Set Command): \n  
																									* Used for setting RX Time Out (PS poll and UPSD) to FW and in QoS Module\n
																									* Done Sync with no memory allocation\n 
																									* Parameter Number:	0x18	\n
																									* Module Number: QoS Module Number \n
																									* Async Bit: OFF	\n
																									* Allocate Bit: OFF	\n
																									* GET Bit: OFF	\n
																									* SET Bit: ON	\n
																									*/
    QOS_MNGR_PS_RX_STREAMING					=	SET_BIT | GET_BIT | QOS_MANAGER_PARAM | 0x19,	/**< QoS Manager Set Power-Save RX Streaming Parameter (QoS Module Set/Get Command): \n  
																									* Used for getting Power-Save RX Streaming or setting Power-Save RX Streaming to FW and in QoS Module\n
																									* Done Sync with no memory allocation\n 
																									* Parameter Number:	0x19	\n
																									* Module Number: QoS Module Number \n
																									* Async Bit: OFF	\n
																									* Allocate Bit: OFF	\n
																									* GET Bit: ON	\n
																									* SET Bit: ON	\n
																									*/

    /* Soft Gemini params */
	SOFT_GEMINI_SET_ENABLE						=	SET_BIT |           SOFT_GEMINI_PARAM	| 0x01,	/**< Soft Gimini Parameters Set Enable Parameter (Soft Gimini Parameters Module Set Command): \n  
																									* Used for configuring Soft Gimini enable Mode (Enable|Disable|Auto) in FW\n
																									* Done Sync with no memory allocation\n 
																									* Parameter Number:	0x01	\n
																									* Module Number: Soft Gimini Parameters Module Number \n
																									* Async Bit: OFF	\n
																									* Allocate Bit: OFF	\n
																									* GET Bit: OFF	\n
																									* SET Bit: ON	\n
																									*/
    SOFT_GEMINI_SET_CONFIG						=	SET_BIT |           SOFT_GEMINI_PARAM   | 0x03,	/**< Soft Gimini Parameters Set Configuration Parameter (Soft Gimini Parameters Module Set Command): \n  
																									* Used for setting Soft Gimini Configuration to FW\n
																									* Done Sync with no memory allocation\n 
																									* Parameter Number:	0x03	\n
																									* Module Number: Soft Gimini Parameters Module Number \n
																									* Async Bit: OFF	\n
																									* Allocate Bit: OFF	\n
																									* GET Bit: OFF	\n
																									* SET Bit: ON	\n
																									*/
    SOFT_GEMINI_GET_CONFIG                      =	GET_BIT |           SOFT_GEMINI_PARAM   | 0x04,	/**< Soft Gimini Parameters Get Configuration Parameter (Soft Gimini Parameters Module Get Command): \n  
																									* Used for getting Soft Gimini Configuration\n
																									* Done Sync with no memory allocation\n 
																									* Parameter Number:	0x04	\n
																									* Module Number: Soft Gimini Parameters Module Number \n
																									* Async Bit: OFF	\n
																									* Allocate Bit: OFF	\n
																									* GET Bit: ON	\n
																									* SET Bit: OFF	\n
																									*/

	/* REPORT section */
	REPORT_MODULE_TABLE_PARAM                   =	SET_BIT | GET_BIT | REPORT_MODULE_PARAM | 0x01,	/**< Report Module Table Parameter (Report Module Set/Get Command): \n  
																									* Used for setting/getting Report Module Table (Tble of all Logged Modules)\n
																									* Done Sync with no memory allocation\n 
																									* Parameter Number:	0x01	\n
																									* Module Number: Report Module Number \n
																									* Async Bit: OFF	\n
																									* Allocate Bit: OFF	\n
																									* GET Bit: ON	\n
																									* SET Bit: ON	\n
																									*/
	REPORT_SEVERITY_TABLE_PARAM                 =	SET_BIT | GET_BIT | REPORT_MODULE_PARAM | 0x02,	/**< Report Severity Table Parameter (Report Module Set/Get Command): \n  
																									* Used for setting/getting the Severity Table (holds availble severity Levels of the event which is reported to user)\n
																									* Done Sync with no memory allocation\n 
																									* Parameter Number:	0x02	\n
																									* Module Number: Report Module Number \n
																									* Async Bit: OFF	\n
																									* Allocate Bit: OFF	\n
																									* GET Bit: ON	\n
																									* SET Bit: ON	\n
																									*/
    REPORT_MODULE_ON_PARAM                      =   SET_BIT | GET_BIT | REPORT_MODULE_PARAM | 0x03,	/**< Report Module ON Parameter (Report Module Set/Get Command): \n  
																									* Used for setting (Enable) ceratin Logged Module in Report Modules Table or getting the Reported Module Status from Table (Enabled/Disabled)\n
																									* Done Sync with no memory allocation\n 
																									* Parameter Number:	0x03	\n
																									* Module Number: Report Module Number \n
																									* Async Bit: OFF	\n
																									* Allocate Bit: OFF	\n
																									* GET Bit: ON	\n
																									* SET Bit: ON	\n
																									*/
    REPORT_MODULE_OFF_PARAM                     =   SET_BIT | GET_BIT | REPORT_MODULE_PARAM | 0x04,	/**< Report Module OFF Parameter (Report Module Set/Get Command): \n  
																									* Used for setting (Disable) ceratin Logged Module in Report Modules Table or getting the Reported Module Status from Table (Enabled/Disabled)\n
																									* Done Sync with no memory allocation\n 
																									* Parameter Number:	0x04	\n
																									* Module Number: Report Module Number \n
																									* Async Bit: OFF	\n
																									* Allocate Bit: OFF	\n
																									* GET Bit: ON	\n
																									* SET Bit: ON	\n
																									*/
    REPORT_PPMODE_VALUE_PARAM                   =   SET_BIT | GET_BIT | REPORT_MODULE_PARAM | 0x05,	/**< Report PP MODE Value Parameter (Report Module Set/Get Command): \n  
																									* Used for setting (Enable/Disable) or Getting the Debug Mode flag, which indicates whether debug module should be used or not\n
																									* Done Sync with no memory allocation\n 
																									* Parameter Number:	0x05	\n
																									* Module Number: Report Module Number \n
																									* Async Bit: OFF	\n
																									* Allocate Bit: OFF	\n
																									* GET Bit: ON	\n
																									* SET Bit: ON	\n
																									*/
	REPORT_OUTPUT_TO_LOGGER_ON                  =   SET_BIT | GET_BIT | REPORT_MODULE_PARAM | 0x06,	/**< Report output ON Parameter (Report Module Set/Get Command): \n  
																									* Used for setting the output of logs to the logger application\n
																									* Done Sync with no memory allocation\n 
																									* Parameter Number:	0x04	\n
																									* Module Number: Report Module Number \n
																									* Async Bit: OFF	\n
																									* Allocate Bit: OFF	\n
																									* GET Bit: ON	\n
																									* SET Bit: ON	\n
																									*/

	REPORT_OUTPUT_TO_LOGGER_OFF                  =   SET_BIT | GET_BIT | REPORT_MODULE_PARAM | 0x07,/**< Report output OFF Parameter (Report Module Set/Get Command): \n  
																									* Used for setting OFF the output of logs to the logger application\n
																									* Done Sync with no memory allocation\n 
																									* Parameter Number:	0x04	\n
																									* Module Number: Report Module Number \n
																									* Async Bit: OFF	\n
																									* Allocate Bit: OFF	\n
																									* GET Bit: ON	\n
																									* SET Bit: ON	\n
																									*/


	/* TX data section */
    TX_CTRL_COUNTERS_PARAM						=			  GET_BIT | TX_CTRL_MODULE_PARAM | 0x01 | ALLOC_NEEDED_PARAM,	/**< TX Control Counters Parameter (TX Control Module Get Command): \n  
																															* Used for getting TX statistics per Tx-queue\n
																															* Done Sync with memory allocation\n 
																															* Parameter Number:	0x01	\n
																															* Module Number: TX Control Module Number \n
																															* Async Bit: OFF	\n
																															* Allocate Bit: ON	\n
																															* GET Bit: ON	\n
																															* SET Bit: OFF	\n
																															*/
    TX_CTRL_RESET_COUNTERS_PARAM                =	SET_BIT 		  | TX_CTRL_MODULE_PARAM | 0x02,						/**< TX Control Reset Counters Parameter (TX Control Module Set Command): \n  
																															* Used for Reset all TX statistics per Tx-queue\n
																															* Done Sync with no memory allocation\n 
																															* Parameter Number:	0x02	\n
																															* Module Number: TX Control Module Number \n
																															* Async Bit: OFF	\n
																															* Allocate Bit: OFF	\n
																															* GET Bit: OFF	\n
																															* SET Bit: ON	\n
																															*/
    TX_CTRL_SET_MEDIUM_USAGE_THRESHOLD			=	SET_BIT           | TX_CTRL_MODULE_PARAM | 0x03,						/**< TX Control Set Medum Usage Threshold Parameter (TX Control Module Set Command): \n  
																															* Used for setting Medum Usage Threshold of AC\n
																															* Done Sync with no memory allocation\n 
																															* Parameter Number:	0x03	\n
																															* Module Number: TX Control Module Number \n
																															* Async Bit: OFF	\n
																															* Allocate Bit: OFF	\n
																															* GET Bit: OFF	\n
																															* SET Bit: ON	\n
																															*/

    /* RX data section */
    RX_DATA_ENABLE_DISABLE_RX_DATA_FILTERS     	=   SET_BIT | GET_BIT | RX_DATA_MODULE_PARAM | 0x04,						/**< RX Data Enable/Disable Filters Parameter (RX Data Module Set/Get Command): \n  
																															* Used for Enabling/Disabling Filters in FW or getting the  Filters Enabling/Disabling current Status\n
																															* Done Sync with no memory allocation\n 
																															* Parameter Number:	0x04	\n
																															* Module Number: RX Data Module Number \n
																															* Async Bit: OFF	\n
																															* Allocate Bit: OFF	\n
																															* GET Bit: ON	\n
																															* SET Bit: ON	\n
																															*/
    RX_DATA_ADD_RX_DATA_FILTER                 	=   SET_BIT           | RX_DATA_MODULE_PARAM | 0x05,						/**< RX Data Add Filter Parameter (RX Data Module Set Command): \n  
																															* Used for adding RX Data Filter to FW\n
																															* Done Sync with no memory allocation\n 
																															* Parameter Number:	0x05	\n
																															* Module Number: RX Data Module Number \n
																															* Async Bit: OFF	\n
																															* Allocate Bit: OFF	\n
																															* GET Bit: OFF	\n
																															* SET Bit: ON	\n
																															*/
    RX_DATA_REMOVE_RX_DATA_FILTER              	=   SET_BIT           | RX_DATA_MODULE_PARAM | 0x06,						/**< RX Data Remove Filter Parameter (RX Data Module Set Command): \n  
																															* Used for removing RX Data Filter from FW\n
																															* Done Sync with no memory allocation\n 
																															* Parameter Number:	0x06	\n
																															* Module Number: RX Data Module Number \n
																															* Async Bit: OFF	\n
																															* Allocate Bit: OFF	\n
																															* GET Bit: OFF	\n
																															* SET Bit: ON	\n
																															*/
    RX_DATA_GET_RX_DATA_FILTERS_STATISTICS     	=             GET_BIT | RX_DATA_MODULE_PARAM | 0x07 | ASYNC_PARAM,			/**< RX Data Get RX Data Filter Statistics Parameter (RX Data Module Get Command): \n  
																															* Used for getting RX Data Filter Statistics from FW\n
																															* Done Async with no memory allocation\n 
																															* Parameter Number:	0x07	\n
																															* Module Number: RX Data Module Number \n
																															* Async Bit: ON	\n
																															* Allocate Bit: OFF	\n
																															* GET Bit: ON	\n
																															* SET Bit: OFF	\n
																															*/

	/* measurement section */
    MEASUREMENT_ENABLE_DISABLE_PARAM			=	SET_BIT |           MEASUREMENT_MODULE_PARAM | 0x01,	/**< Measurement Enable/Disable Parameter (Measurement Module Set Command): \n  
																											* Used for Enabling/Disabling Measurement Management Module\n
																											* Done Sync with no memory allocation\n 
																											* Parameter Number:	0x01	\n
																											* Module Number: Measurement Module Number \n
																											* Async Bit: OFF	\n
																											* Allocate Bit: OFF	\n
																											* GET Bit: OFF	\n
																											* SET Bit: ON	\n
																											*/
	MEASUREMENT_MAX_DURATION_PARAM				=	SET_BIT |           MEASUREMENT_MODULE_PARAM | 0x02,	/**< Measurement Maximum Duration Parameter (Measurement Module Set Command): \n  
																											* Used for updating the Maximum Duration on non serving channel\n
																											* Done Sync with no memory allocation\n 
																											* Parameter Number:	0x02	\n
																											* Module Number: Measurement Module Number \n
																											* Async Bit: OFF	\n
																											* Allocate Bit: OFF	\n
																											* GET Bit: OFF	\n
																											* SET Bit: ON	\n
																											*/

	/* XCC */    
    XCC_CONFIGURATION							=	SET_BIT | GET_BIT | XCC_MANAGER_MODULE_PARAM | 0x01,	/**< XCC Manager Configuration Parameter (XCC Manager Module Set/Get Command): \n  
																											* Used for setting or getting XCC configuration (RogueAP, CCKM, CKIP, All)\n
																											* Done Sync with no memory allocation\n 
																											* Parameter Number:	0x01	\n
																											* Module Number: XCC Manager Module Number \n
																											* Async Bit: OFF	\n
																											* Allocate Bit: OFF	\n
																											* GET Bit: ON	\n
																											* SET Bit: ON	\n
																											*/

	/* MISC section */
	DEBUG_ACTIVATE_FUNCTION						=	SET_BIT | 			MISC_MODULE_PARAM | 0x03,		 	/**< Debug Activate Function Parameter (MISC Module Set Command): \n  
																											* Used for performing debug function\n
																											* Done Sync with no memory allocation\n 
																											* Parameter Number:	0x03	\n
																											* Module Number: MISC Module Number \n
																											* Async Bit: OFF	\n
																											* Allocate Bit: OFF	\n
																											* GET Bit: OFF	\n
																											* SET Bit: ON	\n
																											*/   	

	/* Health Monitoring section */
    HEALTH_MONITOR_CHECK_DEVICE                 =   SET_BIT |           HEALTH_MONITOR_MODULE_PARAM | 0x01,	/**< Health Monitoring Check Device Parameter (Health Monitoring Module Set Command): \n  
																											* Used for sending health check command to FW\n
																											* Done Sync with no memory allocation\n 
																											* Parameter Number:	0x01	\n
																											* Module Number: Health Monitoring Module Number \n
																											* Async Bit: OFF	\n
																											* Allocate Bit: OFF	\n
																											* GET Bit: OFF	\n
																											* SET Bit: ON	\n
																											*/

	/* TWD CoexActivity table */
    TWD_COEX_ACTIVITY_PARAM                			=   SET_BIT | TWD_MODULE_PARAM | TWD_COEX_ACTIVITY_PARAM_ID,		/**< TWD Control CoexActivity Parameter (TWD Control Module Set/Get Command): \n  
																																* Used for getting RTS Threshold from TWD Control Module or setting RTS Threshold to FW and TWD Control Module\n
																																* Done Sync with no memory allocation\n 
																																* Parameter Number:	TWD_COEX_ACTIVITY_PARAM_ID	\n
																																* Module Number: TWD Control Module Number \n
																																* Async Bit: OFF	\n
																																* Allocate Bit: OFF	\n
																																* GET Bit: OFF	\n
																																* SET Bit: ON	\n
																																*/

	LAST_CMD									=	0x00	/**< Last External Parameter - Dummy, Should always stay Last	*/													

}   EExternalParam;


/*************************************/
/* Private Scan Related defenitions  */
/*************************************/

#define RATE_TO_MASK(R)  (1 << ((R) - 1))
#define RATE_MASK_UNSPECIFIED   0
#define DEF_TX_POWER            70

typedef enum
{
    DRV_RATE_AUTO       = 0,                            /**< Auto                                                       */
    DRV_RATE_1M         = 1,                            /**< 1M                                                         */
    DRV_RATE_2M         = 2,                            /**< 2M                                                         */
    DRV_RATE_5_5M       = 3,                            /**< 5.5M                                                       */
    DRV_RATE_11M        = 4,                            /**< 11M                                                        */
    DRV_RATE_22M        = 5,                            /**< 22M                                                        */
    DRV_RATE_6M         = 6,                            /**< 6M                                                         */
    DRV_RATE_9M         = 7,                            /**< 9M                                                         */
    DRV_RATE_12M        = 8,                            /**< 12M                                                        */
    DRV_RATE_18M        = 9,                            /**< 18M                                                        */
    DRV_RATE_24M        = 10,                           /**< 24M                                                        */
    DRV_RATE_36M        = 11,                           /**< 36M                                                        */
    DRV_RATE_48M        = 12,                           /**< 48M                                                        */
    DRV_RATE_54M        = 13,                           /**< 54M                                                        */
    DRV_RATE_MCS_0      = 14,                           /**< 6.5M or  7.2                                       */
    DRV_RATE_MCS_1      = 15,                           /**< 13.0M or 14.4                                      */
    DRV_RATE_MCS_2      = 16,                           /**< 19.5M or 21.7                                      */
    DRV_RATE_MCS_3      = 17,                           /**< 26.0M or 28.9                                      */
    DRV_RATE_MCS_4      = 18,                           /**< 39.0M or 43.3                                      */
    DRV_RATE_MCS_5      = 19,                           /**< 52.0M or 57.8                                      */
    DRV_RATE_MCS_6      = 20,                           /**< 58.5M or 65.0                                      */
    DRV_RATE_MCS_7      = 21,                           /**< 65.0M or 72.2                                      */
    DRV_RATE_MAX        = DRV_RATE_MCS_7,       /**< Maximum Driver's Rate Type         */
    DRV_RATE_INVALID    = 0xFF                          /**< Invalid Driver's Rate Type         */

}ERate;

typedef enum
{
    DRV_RATE_MASK_AUTO          = DRV_RATE_AUTO,                  /**< 0x000000 */
    DRV_RATE_MASK_1_BARKER      = RATE_TO_MASK(DRV_RATE_1M),      /**< 0x000001 */
    DRV_RATE_MASK_2_BARKER      = RATE_TO_MASK(DRV_RATE_2M),      /**< 0x000002 */
    DRV_RATE_MASK_5_5_CCK       = RATE_TO_MASK(DRV_RATE_5_5M),    /**< 0x000004 */
    DRV_RATE_MASK_11_CCK        = RATE_TO_MASK(DRV_RATE_11M),     /**< 0x000008 */
    DRV_RATE_MASK_22_PBCC       = RATE_TO_MASK(DRV_RATE_22M),     /**< 0x000010 */
    DRV_RATE_MASK_6_OFDM        = RATE_TO_MASK(DRV_RATE_6M),      /**< 0x000020 */
    DRV_RATE_MASK_9_OFDM        = RATE_TO_MASK(DRV_RATE_9M),      /**< 0x000040 */
    DRV_RATE_MASK_12_OFDM       = RATE_TO_MASK(DRV_RATE_12M),     /**< 0x000080 */
    DRV_RATE_MASK_18_OFDM       = RATE_TO_MASK(DRV_RATE_18M),     /**< 0x000100 */
    DRV_RATE_MASK_24_OFDM       = RATE_TO_MASK(DRV_RATE_24M),     /**< 0x000200 */
    DRV_RATE_MASK_36_OFDM       = RATE_TO_MASK(DRV_RATE_36M),     /**< 0x000400 */
    DRV_RATE_MASK_48_OFDM       = RATE_TO_MASK(DRV_RATE_48M),     /**< 0x000800 */
    DRV_RATE_MASK_54_OFDM       = RATE_TO_MASK(DRV_RATE_54M),     /**< 0x001000 */
    DRV_RATE_MASK_MCS_0_OFDM    = RATE_TO_MASK(DRV_RATE_MCS_0),   /**< 0x002000 */
    DRV_RATE_MASK_MCS_1_OFDM    = RATE_TO_MASK(DRV_RATE_MCS_1),   /**< 0x004000 */
    DRV_RATE_MASK_MCS_2_OFDM    = RATE_TO_MASK(DRV_RATE_MCS_2),   /**< 0x008000 */
    DRV_RATE_MASK_MCS_3_OFDM    = RATE_TO_MASK(DRV_RATE_MCS_3),   /**< 0x010000 */
    DRV_RATE_MASK_MCS_4_OFDM    = RATE_TO_MASK(DRV_RATE_MCS_4),   /**< 0x020000 */
    DRV_RATE_MASK_MCS_5_OFDM    = RATE_TO_MASK(DRV_RATE_MCS_5),   /**< 0x040000 */
    DRV_RATE_MASK_MCS_6_OFDM    = RATE_TO_MASK(DRV_RATE_MCS_6),   /**< 0x080000 */
    DRV_RATE_MASK_MCS_7_OFDM    = RATE_TO_MASK(DRV_RATE_MCS_7)    /**< 0x100000 */

}ERateMask;

typedef enum
{
    RADIO_BAND_2_4_GHZ                  = 0,
    RADIO_BAND_5_0_GHZ                  = 1,
    RADIO_BAND_DUAL                     = 2,
    RADIO_BAND_NUM_OF_BANDS             = 2
}ERadioBand;

typedef enum
{
    SCAN_ET_COND_DISABLE     = 0x00,        /**< No early termination is not disabled (Do not perform an early termination scan)*/
    SCAN_ET_COND_BEACON      = 0x10,        /**< Early termination scan on beacon reception                                                                     */
    SCAN_ET_COND_PROBE_RESP  = 0x20,        /**< Early termination scan on probe response reception                                                     */
    SCAN_ET_COND_ANY_FRAME   = 0x30,        /**< Early termination scan on both beacon or probe response reception                              */
    SCAN_ET_COND_NUM_OF_CONDS= 0x4          /**< Number of early termination conditions                                                                                 */
}EScanEtCondition;

#define MAC_ADDR_LEN                6
typedef u8                    TMacAddr[MAC_ADDR_LEN];

typedef struct
{
    TMacAddr               bssId;                   /**< BSSID (MAC address) to filter */
    u32              maxChannelDwellTime;     /**< Maximum time to stay on the channel if some frames were
													* received but the early termination limit has not been reached (microseconds)
													*/
    u32              minChannelDwellTime;     /**< Minimum time to stay on the channel if no frames were received (microseconds) */
    EScanEtCondition       earlyTerminationEvent;   /**< Early termination frame type */
    u8               ETMaxNumOfAPframes;      /**< Number of frames from the early termination frame types according to the early
													* Termination Event setting, after which scan is stopped on this channel
													*/
    u8               txPowerDbm;              /**< Power level used to transmit (for active scan only) (0: no change; 1-5: predefined power level */
    u8               channel;                 /**< Channel to scan */

} TScanNormalChannelEntry;

typedef struct
{
    TMacAddr               bssId;                   /**< BSSID (source is MAC address) to filter */
    u32              scanDuration;            /**< Length of time to start scanning the channel (TSF lower 4 bytes) */
    u32              scanStartTime;           /**< Exact time to start scanning the channel (TSF lower 4 bytes) */
    EScanEtCondition       earlyTerminationEvent;   /**< Scan early termination frame type */
    u8               ETMaxNumOfAPframes;      /**< Number of frames from the early termination frame types according to
													* the early Termination Event setting, after which scan is stopped on this channel
													*/
    u8               channel;                 /**< Channel to scan */

}TScanSpsChannelEntry;

typedef union
{
    TScanNormalChannelEntry   normalChannelEntry;       /**< Normal scan parameters: channel parameters for all scan types other than SPS       */
    TScanSpsChannelEntry      SPSChannelEntry;      /**< SPS scan parameters: channel parameters for SPS type   */
}TScanChannelEntry;

typedef struct
{
    u8			len;                                    /**< SSID Length                */
    char		str[ MAX_SSID_LEN ];    /**< SSID string buffer */

}TSsid;

typedef struct
{
    TSsid                  	desiredSsid;                 /**< The SSID to search (optional)                                                                                              */
    EScanType              	scanType;            /**< Desired scan type (normal - active or passive, SPS, triggered - active or passive) */
    ERadioBand             	band;                /**< Band to scan (A / BG)                                                                                                              */
    u8						probeReqNumber;      /**< Number of probe requests to send on each channel (for active scan)                 */
    ERateMask              	probeRequestRate;    /**< The rate at which to send the probe requests                                                               */
    u8						Tid;                 /**< Time at which to trigger the scan (for triggered scan)                                             */
    u64						latestTSFValue;      /**< For SPS scan: the latest TSF at which a frame was received. Used to detect
												* TSF error (AP recovery).
												*/
    u32						SPSScanDuration;     /**< For SPS scan ONLY: the time duration of the scan (in milliseconds), used to
												* Set timer according to. Used to set scan-complete timer
												*/
    u8		               	numOfChannels;       /**< Number of channels to scan                                                                                                                 */
    TScanChannelEntry      	channelEntry[ MAX_NUMBER_OF_CHANNELS_PER_SCAN ];     /**< Channel data array, actual size according to the above field. */

}TScanParams;

typedef TScanParams scan_Params_t;        

/**/
typedef enum
{
	BLUETOOTH_COEXISTENCE_MODE_ENABLED = 0,
	BLUETOOTH_COEXISTENCE_MODE_DISABLED,
	BLUETOOTH_COEXISTENCE_MODE_SENSE
}EUIBTCoexMode;

typedef enum
{
	SG_DISABLE = 0, /* BLUETOOTH_COEXISTENCE_MODE_DISABLED */
	SG_PROTECTIVE,
	SG_OPPORTUNISTIC /* BLUETOOTH_COEXISTENCE_MODE_ENABLED ,BLUETOOTH_COEXISTENCE_MODE_SENSE */
}ESoftGeminiEnableModes;

#endif
