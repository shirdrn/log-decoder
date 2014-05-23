#ifndef _PARAM_IDENTIFIER_H__
#define _PARAM_IDENTIFIER_H__

enum ParamIdentifer
{
	ID_PARAM_UNKNOW   = -1,
//GSM======>
    ID_GSM_CI  = 8,
    ID_GSM_LAC = 9,
	ID_GSM_RXLEVEL_SUB   = 10,
	ID_GSM_RXQUALITY_SUB = 11, 
	ID_GSM_BLER    = 12,
	ID_GSM_MOS     = 13,
	ID_GSM_TXPOWER = 14, //rxPower dbm
	ID_GSM_CBI     = 15,
	ID_GSM_APP_RATE_UL = 16,
	ID_GSM_APP_RATE_DL = 17,
	ID_GSM_FTP_APP_RATE_UL = 18,
	ID_GSM_FTP_APP_RATE_DL = 19,
	ID_GSM_RLC_RATE_UL= 20,
	ID_GSM_RLC_RATE_DL= 21,
	ID_GSM_FTP_RLC_RATE_UL= 22,
	ID_GSM_FTP_RLC_RATE_DL= 23,
	ID_GSM_DELAY_RADIO_ACCESS = 24,
	ID_GSM_RUNTIME_VOICE_BAD_QUALITY = 25,
	ID_GSM_OCCUPY_DURATION = 26,
	ID_GSM_BUS_OCCUPY_DURATION = 27,
	ID_GSM_CS_OCCUPY_DURATION = 28,
	ID_GSM_PS_OCCUPY_DURATION = 29,
	ID_GSM_OCCUPY_DISTANCE = 30,
	ID_GSM_BUS_OCCUPY_DISTANCE = 31,
	ID_GSM_CS_OCCUPY_DISTANCE = 32,
	ID_GSM_PS_OCCUPY_DISTANCE = 33,
	ID_GSM_BUS_CALL_DROP = 34,
	ID_GSM_VOICE_CODE = 35,
	ID_GSM_BUS_CALL_QUALITY_BAD = 36,
	ID_GSM_DELAY_CREL_GSM_TO_WCDMA = 37,
	ID_GSM_AND_WCDAMA_DURATION = 38,
	

	ID_CS_GSM_DELAY_VOICE_END_TO_END = 1001,
	ID_CS_GSM_DELAY_VOICE_ACCESS = 1002,
	ID_PS_GSM_DELAY_PS_ACCESS = 1100,
	ID_PS_GSM_DELAY_PACKET_SERVICE_SETUP = 1101,
	ID_FTP_UL_GSM_DELAY_ACCESS = 1200,
	ID_FTP_UL_GSM_DELAY_SERVERS_RESPONSE = 1201,
	ID_FTP_UL_GSM_TRANSFER_FILE_BYTE = 1202,
	ID_FTP_UL_GSM_TRANSFER_TIME = 1203,
	ID_FTP_DL_GSM_DELAY_ACCESS = 1300,
	ID_FTP_DL_GSM_DELAY_SERVERS_RESPONSE = 1301,
	ID_FTP_DL_GSM_TRANSFER_FILE_BYTE = 1302,
	ID_FTP_DL_GSM_TRANSFER_TIME = 1303,
	ID_HTTP_GSM_DELAY_SETUP     = 1400,
	ID_HTTP_GSM_DELAY_COMPLETE  = 1401,
	ID_PING_GSM_DELAY           = 1500,
	//WCDMA======>
	ID_WCDMA_CI  = 48,
	ID_WCDMA_LAC = 49,
	ID_WCDMA_TOTAL_RSCP = 50, 
	ID_WCDMA_TOTAL_ECIO = 51,
	ID_WCDMA_BEST_RSCP  = 52,
	ID_WCDMA_BEST_ECIO  = 53,
	ID_WCDMA_SIR        = 54,
	ID_WCDMA_BLER       = 55, 
	ID_WCDMA_MOS        = 56,
	ID_WCDMA_RXPOWER    = 57, //RSSI
	ID_WCDMA_CQI        = 58,
	ID_WCDMA_TXPOWER    = 59,
	ID_WCDMA_MPO        = 60,
	ID_WCDMA_APP_RATE_UL = 61,
	ID_WCDMA_APP_RATE_DL = 62,
	ID_WCDMA_FTP_APP_RATE_UL = 63,
	ID_WCDMA_FTP_APP_RATE_DL = 64,
	ID_WCDMA_RLC_RATE_UL= 65,
	ID_WCDMA_RLC_RATE_DL= 66,
	ID_WCDMA_FTP_RLC_RATE_UL= 67,
	ID_WCDMA_FTP_RLC_RATE_DL= 68,
	ID_WCDMA_DELAY_RADIO_ACCESS = 69,
	ID_WCDMA_ACTIVE_SET_PILOE_COUNT = 70,
	ID_WCDMA_PILOT_POLLUTION = 71,
	ID_WCDMA_HANDOVER_WCDMATOGSM_DELAY = 72,
	ID_WCDMA_OCCUPY_DURATION = 73,
	ID_WCDMA_BUS_OCCUPY_DURATION = 74,
	ID_WCDMA_CS_OCCUPY_DURATION = 75,
	ID_WCDMA_PS_OCCUPY_DURATION = 76,
	ID_WCDMA_OCCUPY_DISTANCE = 86,
	ID_WCDMA_BUS_OCCUPY_DISTANCE = 77,
	ID_WCDMA_CS_OCCUPY_DISTANCE = 78,
	ID_WCDMA_PS_OCCUPY_DISTANCE = 79,
	ID_WCDMA_BUS_CALL_DROP = 80,
	ID_WCDMA_BUS_CALL_QUALITY_BAD = 81,
	ID_WCDMA_DELAY_COMPRESS_MODE = 82,
	ID_WCDMA_DELAY_CREL_WCDMA_TO_GSM = 83,
	ID_WCDMA_POWER_OFFSET_UL = 84,
	ID_WCDMA_POWER_OFFSET_DL = 85,
	
	ID_WCDMA_DELAY_VOICE_END_TO_END = 1051,
	ID_WCDMA_DELAY_VOICE_ACCESS     = 1052,
	ID_WCDMA_DELAY_PS_ACCESS        = 1150,
	ID_WCDMA_DELAY_PACKET_SERVICE_SETUP = 1151,
	ID_WCDMA_DELAY_HSDPA_SERVICE_CELL_UPDATE = 1153,
	ID_WCDMA_DELAY_HSUPA_SERVICE_CELL_UPDATE = 1154,
	ID_WCDMA_HS_SCCH_UE_FRAME_USAGE_BLOCK    = 1155,
	ID_WCDMA_FTP_UL_DELAY_ACCESS           = 1250,
	ID_WCDMA_FTP_UL_DELAY_SERVERS_RESPONSE = 1251,
	ID_WCDMA_FTP_UL_TRANSFER_FILE_BYTE     = 1252,
	ID_WCDMA_FTP_UL_TRANSFER_TIME          = 1253,
	ID_WCDMA_FTP_DL_DELAY_ACCESS           = 1350,
	ID_WCDMA_FTP_DL_DELAY_SERVERS_RESPONSE = 1351,
	ID_WCDMA_FTP_DL_TRANSFER_FILE_BYTE     = 1352,
	ID_WCDMA_FTP_DL_TRANSFER_TIME          = 1353,
	ID_WCDMA_DELAY_HTTP_SETUP              = 1450,
	ID_WCDMA_DELAY_HTTP_COMPLETE           = 1451,
	ID_WCDMA_DELAY_PING                    = 1550

};

#endif 


