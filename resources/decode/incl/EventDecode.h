#ifndef _EVENT_DECODE_H_
#define _EVENT_DECODE_H_

#include <stdlib.h>
#include <math.h>
#include <list>
#include <vector>
#include <string>
#include "EventParameterDef.h"
#include "MessageDef.h"
#include "sfl_str.h"
#include "backcompatible.h"
#include "ParamIdentifier.h"
typedef int BOOL;
#define FALSE          0   
#define TRUE           1
#define MAX_PATH       260
#define MAX_TITLE_NAME 128
#define SafeRelease(pInterface) if((pInterface) != NULL) {pInterface->Release(); pInterface=NULL;}
#define SafeDelete(pObject) if((pObject) != NULL) {delete (pObject); (pObject)=NULL;}
#define SafeDeleteArray(pObject) {if((pObject)!=NULL) {delete[](pObject);} (pObject) = NULL;}

#define MAX_CELLMEAS_NUM_CELLS 33
#define MAX_CELLMEAS_NUM_CH    10
// Parameters
#define RANGE_DEFAULT_MIN			0
#define RANGE_DEFAULT_MAX			1

#define RANGE_GSM_CI_MIN			1
#define RANGE_GSM_CI_MAX			65534

#define RANGE_GSM_MNC_MIN			0
#define RANGE_GSM_MNC_MAX			99

#define RANGE_GSM_MCC_MIN			0
#define RANGE_GSM_MCC_MAX			900

#define RANGE_GSM_LAC_MIN			1
#define RANGE_GSM_LAC_MAX			65534

#define RANGE_GSM_RAC_MIN			0
#define RANGE_GSM_RAC_MAX			65534

#define RANGE_GSM_RXLEV_MIN			-140
#define RANGE_GSM_RXLEV_MAX			-30

#define RANGE_GSM_RXQUAL_MIN		0
#define RANGE_GSM_RXQUAL_MAX		7

//TD
#define RANGE_TD_RSCP_MIN			-120
#define RANGE_TD_RSCP_MAX			-50

#define RANGE_TD_RSSI_MIN			-110
#define RANGE_TD_RSSI_MAX			-40

#define RANGE_TD_ISCP_MIN			-116
#define RANGE_TD_ISCP_MAX			-25

//WCDMA
#define RANGE_W_RSCP_MIN			-140
#define RANGE_W_RSCP_MAX			-20

#define RANGE_W_R_QUAL_MIN			-200
#define RANGE_W_R_QUAL_MAX			50

#define RANGE_CDMA_R_RSCP_MIN		-150
#define RANGE_CDMA_R_RSCP_MAX		-20

#define RANGE_CDMA_R_RXPOWER_MIN	-120
#define RANGE_CDMA_R_RXPOWER_MAX	30

#define RANGE_EVDO_R_RSCP_MIN		-150
#define RANGE_EVDO_R_RSCP_MAX		-20

#define RANGE_EVDO_R_RXPOWER_MIN	-120
#define RANGE_EVDO_R_RXPOWER_MAX	30


#define INVALID_VALUE			    -999
#define INVALID_STRING			    "N.A."

#define MAX_CELL_CNT                7  // take care ! Modify the number of cell, the number of parameters must be modified !!!
#define MAX_CH_CNT                  3  // take care ! Modify the number of cell, the number of parameters must be modified !!!

enum EM_LOG_NET_TYPE
{
	EM_GSM_NET_TYPE   = 0x1,
	EM_TD_NET_TYPE    = 0x2,
	EM_WCDMA_NET_TYPE = 0x4,
	EM_CDMA_NET_TYPE  = 0x8,
	EM_EVDO_NET_TYPE  = 0x10,

	EM_TD_GSM_NET_TYPE,
	EM_WCDMA_GSM_NET_TYPE,
	EM_EVDO_CDMA_NET_TYPE,
	EM_NET_TYPE_Unknow
};

typedef enum E_PARAM_ID
{
	E_PARAM_TIME_LINE = 0,
	E_PARAM_MESSAGE,
	E_PARAM_EVENT,
	E_PARAM_SYSTEM,
	E_PARAM_SERVER_STATE,

	E_PARAM_GPS_LAT,
	E_PARAM_GPS_LON,
	
	E_PARAM_GPS_LAT_A,  // Adjust the latitude and longitude for reducing deviation
	E_PARAM_GPS_LON_A,  // 

	E_PARAM_GPS_DISTANCE,
	E_PARAM_GPS_FIX,
	E_PARAM_GPS_SATELLITES,	
	E_PARAM_GPS_VELOCITY,

	E_PARAM_THROUGHT_APP_DL,
	E_PARAM_THROUGHT_APP_UL,
	E_PARAM_THROUGHT_APP_DL_BYTES,
	E_PARAM_THROUGHT_APP_UL_BYTES,
	E_PARAM_THROUGHT_RLC_DL,
	E_PARAM_THROUGHT_RLC_UL,
	E_PARAM_THROUGHT_RLC_UL_RETRANSMISSION,
	//GSM [ SERVER ] -----------------------------------------------------------------------<<
	E_PARAM_GSM_BAND,
	E_PARAM_GSM_CI,
	E_PARAM_GSM_MNC,
	E_PARAM_GSM_MCC,
	E_PARAM_GSM_LAC,
	E_PARAM_GSM_RAC,
	E_PARAM_GSM_BCCH,
	E_PARAM_GSM_CH_TYPE,
	E_PARAM_GSM_HOPPING_ACTIVITIES,
	E_PARAM_GSM_HOPPING_FREQUENCIES,
	E_PARAM_GSM_MOS,

	E_PARAM_GSM_RXLEV_FULL,
	E_PARAM_GSM_RXLEV_SUB,

	E_PARAM_GSM_RXLEV_N1,
	E_PARAM_GSM_RXLEV_N2,
	E_PARAM_GSM_RXLEV_N3,
	E_PARAM_GSM_RXLEV_N4,
	E_PARAM_GSM_RXLEV_N5,
	E_PARAM_GSM_RXLEV_N6,

	E_PARAM_GSM_RXQUAL_FULL,
	E_PARAM_GSM_RXQUAL_SUB,

	E_PARAM_GSM_FER_FULL, 
	E_PARAM_GSM_FER_SUB,  
	E_PARAM_GSM_FER_TCH,

	E_PARAM_GSM_CBI,
	E_PARAM_GSM_C1,
	E_PARAM_GSM_C2,
	E_PARAM_GSM_DTX,
	E_PARAM_GSM_TN,         /*Timeslot number of voice call*/
	E_PARAM_GSM_TXPOWER,
	E_PARAM_GSM_TXPOWER_EX,
	E_PARAM_GSM_TA,
	E_PARAM_GSM_MAIO,
	E_PARAM_GSM_HSN,
	E_PARAM_GSM_HCH,
	E_PARAM_GSM_CHI_CHANNEL,
	E_PARAM_GSM_VOICE_CODEC,

	E_PARAM_GSM_CRO,
	E_PARAM_GSM_CRH,
	E_PARAM_GSM_RLT,
	E_PARAM_GSM_BLER,

	//GSM EDGE
	E_PARAM_GSM_PTECH,
	E_PARAM_GSM_PRXQUAL,
	E_PARAM_GSM_PCVALUE,
	E_PARAM_GSM_PTSUP,
	E_PARAM_GSM_PTSDOWN,
	E_PARAM_GSM_PCODINGUP,
	E_PARAM_GSM_PCODINGDOWN,
	E_PARAM_GSM_PWINSIZEDL,
	E_PARAM_GSM_PWINSIZEUL,
	E_PARAM_GSM_PBLERDOWN,
	E_PARAM_GSM_P8PSKCVBEP,
	E_PARAM_GSM_PGMSKCVBEP,
	E_PARAM_GSM_P8BSKMEANBEP,
	E_PARAM_GSM_PGMSKMEANBEP,

	//GSM [ CELL & MSG ]
	E_PARAM_GSM_CELLS,
	E_PARAM_GSM_COMPLANIT_CELLS,

	//GSM [ list params : no need list memory]
	E_PARAM_GSM_CELL_CELLTYPE,
	E_PARAM_GSM_CELL_ARFCN,
	E_PARAM_GSM_CELL_ID,
	E_PARAM_GSM_CELL_LAC,
	E_PARAM_GSM_CELL_RAC,
	E_PARAM_GSM_CELL_BSIC,
	E_PARAM_GSM_CELL_RXLEV_FULL,
	E_PARAM_GSM_CELL_RXLEV_SUB,
	E_PARAM_GSM_CELL_C1,
	E_PARAM_GSM_CELL_C2,
	E_PARAM_GSM_CELL_SRXLEV,
	E_PARAM_GSM_CELL_BAND,
	E_PARAM_GSM_CELL_NAME,

	//WCDMA [SERVER]------------------------------------------------------------------------<<
	E_PARAM_W_SERV_UARFCN,
	E_PARAM_W_SERV_SCR,
	E_PARAM_W_SERV_RQUAL,
	E_PARAM_W_SERV_SQUAL,
	E_PARAM_W_SERV_SRXLEV,
	E_PARAM_W_SERV_RRXLEV,
	E_PARAM_W_SERVICE_STATE,
	E_PARAM_W_SERV_CI,
	E_PARAM_W_SERV_BAND,
	E_PARAM_W_SERV_TRESELECTION,
	E_PARAM_W_SERV_ADDITIONWINDOW,
	E_PARAM_W_SERV_LAC,
	E_PARAM_W_SERV_RAC,
	E_PARAM_W_SERV_MCC,
	E_PARAM_W_SERV_MNC,
	E_PARAM_W_SERV_NMO,
	E_PARAM_W_SERV_EC_NO,
	E_PARAM_W_SERV_EC_NO_BEST,
	E_PARAM_W_SERV_EC_NO_COMBIN,
	E_PARAM_W_MOS,

	E_PARAM_W_MAX_ALLOWED_TX_POWER,
	E_PARAM_W_RRC_STATE,

	E_PARAM_W_SERV_RSCP_COMBIN,
	E_PARAM_W_CPICH_RSCP,
	E_PARAM_W_CPICH_RSCP_N1,
	E_PARAM_W_CPICH_RSCP_N2,
	E_PARAM_W_CPICH_RSCP_N3,
	E_PARAM_W_CPICH_RSCP_N4,
	E_PARAM_W_CPICH_RSCP_N5,
	E_PARAM_W_CPICH_RSCP_N6,

	E_PARAM_W_RAC,
	E_PARAM_W_PWROFFSET,
	E_PARAM_W_PLNONMAX,
	E_PARAM_W_EDPCCH_PWROFFSET,	
	E_PARAM_W_HSDSCHSC,
	E_PARAM_W_HRNTI,
	E_PARAM_W_PRIMARY_ERNTI,
	E_PARAM_W_SECONDARY_ERNTI,	
	E_PARAM_W_HAPPYBITDELAYCOND,
	E_PARAM_W_PACKETTECH,
	E_PARAM_W_PACKETSTATE,
	E_PARAM_W_NWOPERATION,
	E_PARAM_W_HSDPAUECATEG,
	E_PARAM_W_ACKNACK_REPETITIONS,
	E_PARAM_W_HSUPAUE_CATEG,
	E_PARAM_W_TTI,
	E_PARAM_W_RATEMATCHING,
	E_PARAM_W_AGCHOVSF,
	E_PARAM_W_ETFCITABLE,
	E_PARAM_W_FER,
	E_PARAM_W_TX_POWER,
	E_PARAM_W_BLER,

	E_PARAM_W_SIR,
	E_PARAM_W_SIR_TARGET,
	E_PARAM_W_DLPWRUP,	
	E_PARAM_W_DLPWRUP_CND,
	E_PARAM_W_DLPWRDOWN_CND,
	E_PARAM_W_DPC_MODE,
	E_PARAM_W_BSDIVSTATE,

	E_PARAM_W_INIT_PWR,
	E_PARAM_W_PWR_RAMP_STEP,
	E_PARAM_W_PRE_CNT,
	E_PARAM_W_RACH_TX_PWR,
	E_PARAM_W_MAX_PREEMBLE,
	E_PARAM_W_UL_INTERF,
	E_PARAM_W_AICH_STATUS,
	E_PARAM_W_DATA_GAIN,
	E_PARAM_W_CTRL_GAIN,
	E_PARAM_W_PWR_OFFSET,
	E_PARAM_W_MSG_LENGTH,
	E_PARAM_W_PRE_CYCLES,

	//[ channels ]
	E_PARAM_W_CHANNELS,		
	
	//[ cells ]
	E_PARAM_W_CELLS,
	E_PARAM_W_COMPLANIT_CELLS,

	//W [ Transport channel information ]
	E_PARAM_W_TRCHI,

	//W [CQI information]
	E_PARAM_W_CQI,
	E_PARAM_W_CQI_1,

	E_PARAM_W_RSSI,
	E_PARAM_W_CH_1,  // RSSI
	E_PARAM_W_CH_2,  // RSSI
	E_PARAM_W_CH_3,  // RSSI
	E_PARAM_W_CH_4,  // RSSI
	E_PARAM_W_CH_5,  // RSSI
	E_PARAM_W_CH_6,  // RSSI

	// W: TITLE [ list params : no need list memory]
	E_PARAM_W_CHANNEL_CHANNEL_NUMBER,
	E_PARAM_W_CHANNEL_RSSI,

	E_PARAM_W_CELL_TYPE,
	E_PARAM_W_CELL_BAND,
	E_PARAM_W_CELL_CH,
	E_PARAM_W_CELL_SCR,
	E_PARAM_W_CELL_EC_NO,
	E_PARAM_W_CELL_STTD,
	E_PARAM_W_CELL_RSCP,

	//TRCHI
	E_PARAM_W_TRCHI_ID,
	E_PARAM_W_TRCHI_BLER,
	E_PARAM_W_TRCHI_BLOCKS_CNT,
	E_PARAM_W_TRCHI_ERRORS_CNT,

	//TD-SCDMA [SERVER]---------------------------------------------------------------------<<
	E_PARAM_TD_SERV_UARFCN,
	E_PARAM_TD_SERV_CELL_PARAMETER_ID,
	
	E_PARAM_TD_PCCPCH_RSCP,
	E_PARAM_TD_PCCPCH_RSCP_N1,
	E_PARAM_TD_PCCPCH_RSCP_N2,
	E_PARAM_TD_PCCPCH_RSCP_N3,
	E_PARAM_TD_PCCPCH_RSCP_N4,
	E_PARAM_TD_PCCPCH_RSCP_N5,
	E_PARAM_TD_PCCPCH_RSCP_N6,

	E_PARAM_TD_DPCH_RSCP,
	E_PARAM_TD_DPCH_RSCP_N1,
	E_PARAM_TD_DPCH_RSCP_N2,
	E_PARAM_TD_DPCH_RSCP_N3,
	E_PARAM_TD_DPCH_RSCP_N4,
	E_PARAM_TD_DPCH_RSCP_N5,
	E_PARAM_TD_DPCH_RSCP_N6,

	E_PARAM_TD_PCCPCH_CBI,	
	E_PARAM_TD_SERV_CI,
	E_PARAM_TD_SERV_LAC,
	E_PARAM_TD_SERV_RAC,
	E_PARAM_TD_SERV_MCC,
	E_PARAM_TD_SERV_MNC,
	E_PARAM_TD_SERV_NMO,
	E_PARAM_TD_SERV_TA,
	E_PARAM_TD_FER,
	E_PARAM_TD_ATTACH_ALLLOWED,
	E_PARAM_TD_SERV_INTRA_SEARCH,
	E_PARAM_TD_SERV_INTER_SEARCH,
	E_PARAM_TD_SERV_SEARCH_HCS,
	E_PARAM_TD_SERV_SEARCH_HCS_RAT,
	E_PARAM_TD_SERV_HCS_SUPPORTED,
	E_PARAM_TD_SERV_Q_HCS,
	E_PARAM_TD_SERV_RAT,
	E_PARAM_TD_SERV_CELL_NAME,
	E_PARAM_TD_UE_MAX_ALLOWED_UL_TX_POWER,
	E_PARAM_TD_SERV_CELL_BARRED,
	E_PARAM_TD_SERV_URAID_COUNT,
	E_PARAM_TD_T3212,
	E_PARAM_TD_SERV_DRX_CYCLE,
	E_PARAM_TD_TX_POWER,
	
	E_PARAM_TD_RSSI,
	E_PARAM_TD_CH_1,  // RSSI
	E_PARAM_TD_CH_2,  // RSSI
	E_PARAM_TD_CH_3,  // RSSI
	E_PARAM_TD_CH_4,  // RSSI
	E_PARAM_TD_CH_5,  // RSSI
	E_PARAM_TD_CH_6,  // RSSI

	E_PARAM_TD_BLER,
	E_PARAM_TD_SIR_TARGET,
	E_PARAM_TD_SIR,
	E_PARAM_TD_PCCPCH_PATHLOSS,
	E_PARAM_TD_S_RX_LEV,
	E_PARAM_TD_R_RX_LEV,
	E_PARAM_TD_H_RX_LEV,
	E_PARAM_TD_TCP_STEP_SIZE,  //Tx power control step size
	E_PARAM_TD_TA,

	//TD-SCDMA [ channels ]
	E_PARAM_TD_CHANNELS,

	//TD-SCDMA [ cells ]
	E_PARAM_TD_CELLS,

	//TD-SCDMA [ TIMESLOT ]
	E_PARAM_TD_TIMESLOT,

	//TD-SCDMA [ Transport channel information ]
	E_PARAM_TD_TRCHI,

	//TD-SCDMA [ list params : no need list memory]
	//channels
	E_PARAM_TD_CHANNEL_CHANNEL_NUMBER,
	E_PARAM_TD_CHANNEL_RSSI,
	E_PARAM_TD_CHANNEL_BAND,
	
	//cells
	E_PARAM_TD_CELL_TYPE,
	E_PARAM_TD_CELL_CHANNEL_NUMBER,
	E_PARAM_TD_CELL_PARAMETERS_ID,
	E_PARAM_TD_CELL_RSCP,
	E_PARAM_TD_CELL_RRXLEV,
	E_PARAM_TD_CELL_SRXLEV,
	E_PARAM_TD_CELL_BAND,

	//timeslote
	E_PARAM_TD_TS_TSL,
	E_PARAM_TD_TS_ISCP,
	E_PARAM_TD_TS_RSCP,

	//TRCHI
	E_PARAM_TD_TRCHI_ID,
	E_PARAM_TD_TRCHI_BLER,
	E_PARAM_TD_TRCHI_BLOCKS_CNT,
	E_PARAM_TD_TRCHI_ERRORS_CNT,

	//CDMA 1X [SERVER]---------------------------------------------------------------------<<

	//CELLMEAS
	E_PARAM_CDMA_SERV_BAND,
	E_PARAM_CDMA_SERV_CHANNEL_NUMBER,
	E_PARAM_CDMA_SERV_PILOT_NUMVER,
	E_PARAM_CDMA_SERV_WALSH,
	E_PARAM_CDMA_SERV_RSCP,
	E_PARAM_CDMA_SERV_ECIO,

	E_PARAM_CDMA_CHANNELS,
	E_PARAM_CDMA_CELLS,

	E_PARAM_CDMA_RXPOWR,
	E_PARAM_CDMA_RXPOWR_1,
	E_PARAM_CDMA_RXPOWR_2,
	E_PARAM_CDMA_RXPOWR_3,
	E_PARAM_CDMA_RXPOWR_4,
	E_PARAM_CDMA_RXPOWR_5,
	E_PARAM_CDMA_RXPOWR_6,

	E_PARAM_CDMA_RSCP,
	E_PARAM_CDMA_RSCP_1,
	E_PARAM_CDMA_RSCP_2,
	E_PARAM_CDMA_RSCP_3,
	E_PARAM_CDMA_RSCP_4,
	E_PARAM_CDMA_RSCP_5,
	E_PARAM_CDMA_RSCP_6,

	// CDMA: TITLE [ list params : no need list memory]
	E_PARAM_CDMA_CHANNEL_BAND,
	E_PARAM_CDMA_CHANNEL_NUMBER,
	E_PARAM_CDMA_CHANNEL_RXPOWER,
	E_PARAM_CDMA_CHANNEL_RX0POWER,
	E_PARAM_CDMA_CHANNEL_RX1POWER,

	E_PARAM_CDMA_CELL_TYPE,
	E_PARAM_CDMA_CELL_BAND,
	E_PARAM_CDMA_CELL_CHANNEL_NUMBER,
	E_PARAM_CDMA_CELL_PILOT_NUMVER,
	E_PARAM_CDMA_CELL_WALSH,
	E_PARAM_CDMA_CELL_RSCP,
	E_PARAM_CDMA_CELL_ECIO,

	//FER
	//------------------------------------------------
	E_PARAM_CDMA_FER_TOTAL,
	E_PARAM_CDMA_FER_FCH,
	E_PARAM_CDMA_FER_SCH,

	//RLPRATE
	//------------------------------------------------
	E_PARAM_CDMA_RLPRATE_RLP_REV_RATE,
	E_PARAM_CDMA_RLPRATE_RLP_FOR_RATE,
	E_PARAM_CDMA_RLPRATE_RLP_REV_RETR_RATE,
	E_PARAM_CDMA_RLPRATE_RLP_FWD_RETR_RATE,

	//TXPC
	E_PARAM_CDMA_TXPC_TXPOWER,
	E_PARAM_CDMA_TXPC_TXADJUST,
	E_PARAM_CDMA_TXPC_TXPWRLIMIT,
	E_PARAM_CDMA_TXPC_R_FCH,
	E_PARAM_CDMA_TXPC_R_SCH0,
	E_PARAM_CDMA_TXPC_R_SCH1,
	E_PARAM_CDMA_TXPC_R_DCCH,

	//RACHI
	E_PARAM_CDMA_RACHI_NOM_PWR,
	E_PARAM_CDMA_RACHI_INIT_PWR,
	E_PARAM_CDMA_RACHI_PWR_STEP,
	E_PARAM_CDMA_RACHI_NUM_STEP,
	E_PARAM_CDMA_RACHI_PROBECOUNT,
	E_PARAM_CDMA_RACHI_PROBESEQCOUNT,
	E_PARAM_CDMA_RACHI_ACCESSCHNUMBER,
	E_PARAM_CDMA_RACHI_RANDOMDELAY,/* PN randomization delay */
	E_PARAM_CDMA_RACHI_TXLEVEL,/* Probe TX levelAccess probe TX level. */
	E_PARAM_CDMA_RACHI_ACCESSRXLEVEL,/* RX level Unit: dB */
	E_PARAM_CDMA_RACHI_ACCESSTXADJ,/* Transmit gain adjust */
	E_PARAM_CDMA_RACHI_PSIST,/* Number of persistence tests performed Range: 0 - 255 */
	E_PARAM_CDMA_RACHI_SEQBACKOFF,/* Sequence backoff Range: 0 - 255 */
	E_PARAM_CDMA_RACHI_PROBBACKOFF,/* Probe backoff Range: 0 - 255 */
	E_PARAM_CDMA_RACHI_INTERCORR,/* Inteference correction Range: 0 - 255 */

	//EVDO [SERVER]---------------------------------------------------------------------<<
	E_PARAM_EVDO_SERV_BAND,
	E_PARAM_EVDO_SERV_CHANNEL_NUMBER,
	E_PARAM_EVDO_SERV_PILOT_NUMVER,
	E_PARAM_EVDO_SERV_RSCP,
	E_PARAM_EVDO_SERV_ECIO,

	E_PARAM_EVDO_CHANNELS,
	E_PARAM_EVDO_CELLS,

	E_PARAM_EVDO_RXPOWR,
	E_PARAM_EVDO_RXPOWR_1,
	E_PARAM_EVDO_RXPOWR_2,
	E_PARAM_EVDO_RXPOWR_3,
	E_PARAM_EVDO_RXPOWR_4,
	E_PARAM_EVDO_RXPOWR_5,
	E_PARAM_EVDO_RXPOWR_6,

	E_PARAM_EVDO_RSCP,
	E_PARAM_EVDO_RSCP_1,
	E_PARAM_EVDO_RSCP_2,
	E_PARAM_EVDO_RSCP_3,
	E_PARAM_EVDO_RSCP_4,
	E_PARAM_EVDO_RSCP_5,
	E_PARAM_EVDO_RSCP_6,

	// EVDO: TITLE [ list params : no need list memory]
	E_PARAM_EVDO_CHANNEL_BAND,
	E_PARAM_EVDO_CHANNEL_NUMBER,
	E_PARAM_EVDO_CHANNEL_RXPOWER,
	E_PARAM_EVDO_CHANNEL_RX0POWER,
	E_PARAM_EVDO_CHANNEL_RX1POWER,

	E_PARAM_EVDO_CELL_TYPE,
	E_PARAM_EVDO_CELL_BAND,
	E_PARAM_EVDO_CELL_CHANNEL_NUMBER,
	E_PARAM_EVDO_CELL_PILOT_NUMVER,
/*	E_PARAM_EVDO_CELL_WALSH,*/
	E_PARAM_EVDO_CELL_RSCP,
	E_PARAM_EVDO_CELL_ECIO,

	//------------------------------------------------
	E_PARAM_EVDO_RLPRATE_RLP_REV_RATE,
	E_PARAM_EVDO_RLPRATE_RLP_FOR_RATE,
	E_PARAM_EVDO_RLPRATE_RLP_REV_RETR_RATE,
	E_PARAM_EVDO_RLPRATE_RLP_FWD_RETR_RATE,

	//TXPC
	E_PARAM_EVDO_TXPC_TX_POWER,
	E_PARAM_EVDO_TXPC_TX_ADJUST,
	E_PARAM_EVDO_TXPC_TX_PILOT,
	E_PARAM_EVDO_TXPC_TX_OPEN_LOOP_POWER,
	E_PARAM_EVDO_TXPC_TX_MAX_POWER_USAGE,
	E_PARAM_EVDO_TXPC_TX_MIN_POWER_USAGE,

	//RACHI
	E_PARAM_EVDO_RACHI_MaxProbeseqs,/* Maximum number of probe sequences */
	E_PARAM_EVDO_RACHI_Duration,/* Time to complete the attempt Unit: slots */
	E_PARAM_EVDO_RACHI_AccessPN,/* PN of the sector that sends the ACAck or the TCA message */
	E_PARAM_EVDO_RACHI_Accesssectorid,/* Sector ID on which the access probe was sent Range: 0 - 16777215 */
	E_PARAM_EVDO_RACHI_MaxProbes,/* Number of probes per probe sequence */
	E_PARAM_EVDO_RACHI_Result,/* E_Probesequenceres */
	E_PARAM_EVDO_RACHI_nProbes,/* Number of probes sent in last probe sequence */
	E_PARAM_EVDO_RACHI_nProbeseqs,/* Number of probe sequences sent for this access attempt */
	E_PARAM_EVDO_RACHI_Accesscolorcode,/* Color code on which the access probe was sent Range: 0 - 255 */
	E_PARAM_MAX,
};
#define MAX_STATISTICS_SYSTEM_CNT 3

typedef enum E_SERVER_STATE_TYPE
{
	E_SERVER_STATE_IDLE  = 0,
	E_SERVER_STATE_VOICE,
	E_SERVER_STATE_VIDEO,
	E_SERVER_STATE_DATA,
};

typedef enum E_PARAM_TYPE
{
	E_PARAM_TYPE_TIME = 0,	

	E_PARAM_TYPE_INT,
	E_PARAM_TYPE_FLOAT,

	E_PARAM_TYPE_GSM_CELL,
	E_PARAM_TYPE_GSM_COMPLAINT_CELL,

	E_PARAM_TYPE_TD_CHANNEL,
	E_PARAM_TYPE_TD_CELL,
	E_PARAM_TYPE_TD_TIMESLOT,
	E_PARAM_TYPE_TD_TRCHI,

	E_PARAM_TYPE_W_CHANNEL,
	E_PARAM_TYPE_W_CELL,
	E_PARAM_TYPE_W_COMPLAINT_CELL,
	E_PARAM_TYPE_W_TRCHI,
	E_PARAM_TYPE_W_CQI,

	E_PARAM_TYPE_CDMA_CHANNEL,
	E_PARAM_TYPE_CDMA_CELL,

	E_PARAM_TYPE_EVDO_CHANNEL,
	E_PARAM_TYPE_EVDO_CELL,

	E_PARAM_TYPE_JUST_FOR_TITLE,
};

typedef enum E_PARAM_INDEX_TABLE_ID
{
	E_PARAM_INDEX_TABLE_UNKNOW = 0,
	E_PARAM_INDEX_TABLE_CELL_TYPE,
	E_PARAM_INDEX_TABLE_WCDMA_CELL_TYPE,
	E_PARAM_INDEX_TABLE_CDMA_OR_EVDO_CELL_TYPE,
	E_PARAM_INDEX_TABLE_BAND,
	E_PARAM_INDEX_TABLE_PERSONAL_RANGE,
	E_PARAM_INDEX_TABLE_OTHER,
};

typedef enum E_RESULT_ID
{
	E_RESULT_OK = 0,
	E_RESULT_FILE_NOT_EXISTING,
	E_RESULT_FILE_CAN_NOT_OPEN,
	E_RESULT_FILE_NULL,
	E_RESULT_FILE_INVALID,
	E_RESULT_FILE_NO_NET,
	E_RESULT_MEMORY_FAILURE,
};

typedef struct T_TIME_LINE
{
	unsigned int nTime;
}_T_TIME_LINE;

#define MAX_MESSAGE_RAWDATA_LEN   100
typedef struct T_PARAM_MESSAGE_ITEM
{
	int      iIndex;          // time line of index
	E_UINT8  eMsgID;          // msg id
	E_UINT8  Direction;       // UP  DOWN
	E_UINT8  Type;            // L2 L3 RRC MAC RLC
	E_UINT8  eMsgType;        // E_MESSAGE_TYPE
	unsigned int nTime;       // time
	unsigned int nMsgLen;     // rawdata len
	char    *pName;           //
	char    *pSubChannelName; //
	unsigned char *pRawData;  //
}_T_PARAM_MESSAGE_ITEM;

typedef struct T_PARAM_MESSAGE_MATRIX
{
	int          iIndex;
	unsigned int nRealCnt;
	T_PARAM_MESSAGE_ITEM *pList;
}_T_PARAM_MESSAGE_MATRIX;

typedef struct T_PARAM_EVENT_ITEM
{
	int      iIndex;        // time line of index	
	unsigned int nTime;     // time
	unsigned int nEventID;  // event id for searching
	unsigned int nSystem;   // System
	unsigned int nParam1;   //
	unsigned int nParam2;   //
	unsigned int nParam3;
	unsigned int nParam4;

	int			 iParam5;
	int			 iParam6;
	int			 iParam7;
	int			 iParam8;
	int			 iParam9;
	int			 iParam10;

#define MAX_REMARK_LEN   512
	char     szEventName[MAX_TITLE_NAME];   // event name
	char     szRemark[MAX_REMARK_LEN];      // Remark
	T_PARAM_EVENT_ITEM()
	{
		memset(szEventName,sizeof(szEventName),0);
		memset(szRemark,sizeof(szRemark),0);
	}
}_T_PARAM_EVENT_ITEM;

typedef struct T_PARAM_EVENT_MATRIX
{
	int          iIndex;
	unsigned int nRealCnt;
	unsigned int nUnsualEventCnt;
	T_PARAM_EVENT_ITEM *pList;
	T_PARAM_EVENT_ITEM *pUnsualEventList;
}_T_PARAM_EVENT_MATRIX;


//system : GSM/TD  Cell(s) Channel(s)------------------------------------<<
typedef struct T_PARAM_MATRIX
{
	unsigned int iCurTimeIndex; //time line
	unsigned int nCnt;	
	void        *pList;   // NULL : 
}_T_PARAM_MATRIX;

//NORMAL DATA------------------------------------------------------------<<
typedef struct T_PARAM_DATA_INT
{
	int  iValue;
	unsigned int  iCurTimeIndex;   // time line
}_T_PARAM_DATA_INT;

typedef struct T_PARAM_DATA_FLOAT
{
	float fVale;
	unsigned int  iCurTimeIndex;  // time line
}_T_PARAM_DATA_FLOAT;
//----------------------------------------------------------------------->
typedef struct T_COMPLAINT_EVENT_ITEM
{
	int      iIndex;        // time line of index	
	unsigned int nTime;     // time
	unsigned int nEventID;  // event id for searching
	unsigned int nSystem;   // System
	int   iComplaintType;
	int   iSceneType;
	int   iMNC;
	int   iMCC;
	int   iCI;
	int   iLACI;
	float fLon;
	float fLat;
	int   iParam1;//G:RxlevSub W:Rscp
	int   iParam2;//W: Ec/No
	int   iParam3;//W: Sc
	int   iCellCnt;
	TEVENT_CMPEV_GSM_PARAMCELL_STR gsmCmpev[MAX_CELLMEAS_NUM_CELLS];
	TEVENT_CMPMEAS_UTMSFDD_PARAMCELL_STR wcdmaCmpeas[MAX_CELLMEAS_NUM_CELLS];
}_T_COMPLAINT_EVENT_ITEM;

typedef struct T_COMPLAINT_EVENT_MATRIX
{
	int          iIndex;
	unsigned int nRealCnt;
	enum {MAX_COMPLAINT_CNT=10};
	T_COMPLAINT_EVENT_ITEM pCmpalintList[MAX_COMPLAINT_CNT];
}_T_COMPLAINT_EVENT_MATRIX;
//------------------------------------------------------------------------>

typedef struct _T_USRBHV_EVENT_
{
	unsigned int nEventID;  // event id for searching
	unsigned int nTime;     // time
	int      iIndex;        // time line of index	
	int iSystem;
	int iOperationType;
    int iCallType;
	int iCallDirection;
	int iCallDuration;
	int iDataDuration;
	int iFlow;
}T_USRBHV_EVENT;

typedef struct T_USRBHV_EVENT_MATRIX
{
	int          iIndex;
	unsigned int nRealCnt;
	enum {MAX_USRBHV_CNT=100};
	T_USRBHV_EVENT pCmpalintList[MAX_USRBHV_CNT];
}_T_USRBHV_EVENT_MATRIX;

//------------------------------------------------------------------------>
typedef struct T_LOG_INDOOR_INFO
{
	char  szIndoor_Project[MAX_TITLE_NAME];
	char  szIndoor_Domain[MAX_TITLE_NAME];
	char  szIndoor_Building[MAX_TITLE_NAME];
	char  szIndoor_Floor[MAX_TITLE_NAME];
	char  szIndoor_Room[MAX_TITLE_NAME];
	char  szIndoor_Project_Type[MAX_TITLE_NAME];
	char  szIndoor_Project_Task[MAX_TITLE_NAME];
	char  szIndoor_Project_Remark[MAX_TITLE_NAME];
	char  szIndoor_IndoorMap[MAX_PATH];
	char  szAnBuildingID[MAX_PATH];
	char  szAntennaName[MAX_PATH];
	char  szAnRemark[MAX_PATH];
	int   iAnoperateType;
	int   iAnInstallType;
	short nIndoor_map_width;
	short nIndoor_map_height;
}_T_LOG_INDOOR_INFO;

#define MAX_OUT_DOOR_LEN 100

typedef struct _LOG_INSPECTION_INFO_
{
	char szTestType[MAX_OUT_DOOR_LEN];
	char szTestCity[MAX_OUT_DOOR_LEN];
	char szProjectNum[MAX_OUT_DOOR_LEN];
	char szRat[MAX_OUT_DOOR_LEN];
	char szDeviceManufcturer[MAX_OUT_DOOR_LEN];
	char szTestManufacturer[MAX_OUT_DOOR_LEN];
	char szTestMember[MAX_OUT_DOOR_LEN];
	char szTestDoMain[MAX_OUT_DOOR_LEN];
	char szLabel[MAX_OUT_DOOR_LEN];
	char szTestTime[MAX_OUT_DOOR_LEN];
}LOG_INSPECTION_INFO;

typedef struct _LOG_TS_TIME_INFO_
{
	unsigned int nGSM_HOURS;
	unsigned int nWCDMA_HOURS;
	unsigned int nWIFI_HOURS;
}LOG_TS_TIME_INFO;

enum EM_MOBILE_DEVICE_TYPE
{
	EM_MOBILE_DEVICE_UNKNOW     = 0,
 	EM_MOBILE_DEVICE_DEC        = 1,
 	EM_MOBILE_DEVICE_IPHONE_3   = 1001,
 	EM_MOBILE_DEVICE_IPHONE_3GS = 1002,
 	EM_MOBILE_DEVICE_IPHONE_4   = 1003,
	EM_MOBILE_DEVICE_IPHONE_4S  = 1004,
	EM_MOBILE_DEVICE_IPHONE_5   = 1005,
	EM_MOBILE_DEVICE_HTC_S710E  = 1101,
	EM_MOBILE_DEVICE_HTC_Z710E  = 1102
};

enum EM_LOG_SERVICE_TYPE
{
	EM_LOG_SERVICE_normal,
	EM_LOG_SERVICE_inspection,
	EM_LOG_SERVICE_complaint,
	EM_LOG_SERVICE_spotcheck,
	EM_LOG_SERVICE_GD_spotcheck,
	EM_LOG_SERVICE_GD_inspection
};

typedef struct T_LOG_INFO
{
	unsigned int nStartTime;
	unsigned int nEndTime;
	unsigned int nTimePointCnt;
	unsigned int nTimeDuration;
	unsigned int nCurTimePoint;
	unsigned int iProvinceID;
	unsigned int iAreaID;
	unsigned int iCityID;
	unsigned short wYear;
	unsigned short wMonth;
	unsigned short wDay;
	//add 131219 wangyang
	String szLog_Version;
	//add end
	char szLog_Title[MAX_TITLE_NAME];
	char szLog_Filename[MAX_PATH];
	char szEI[MAX_TITLE_NAME];
	char szPhoneRelatedNum[55];
	char szLogName[100];
	char szDeviceName[MAX_TITLE_NAME];
	//add 131225 wangyang
	char szBrand[MAX_TITLE_NAME];
	//add end
	char szLogMD5[34];
	char szPhotoMd5[34];
	char szTOS[64];
	char szTsUSerID[MAX_TITLE_NAME];
	int  iLastBand_2g;
	int  iLogSize;
	int  iLockNetType;
	int  iLockNetTimePonit;
	int  iLogWorkType;
	int  iLogServiceType;
	int  iWorkID;
	int  iProjectID;
	bool bGSM;
	bool bTDSCDMA;
	bool bWCDMA;
	bool bCDMA;
	bool bEVDO;
	bool bLoad;
	bool bIsIndoor;
	bool bIsOutdoor;
	float fLon;//首个经纬度
	float fLat;
	T_LOG_INDOOR_INFO   indoor;
	LOG_INSPECTION_INFO inspectionInfo;   
	EM_MOBILE_DEVICE_TYPE emDeviceType;
	LOG_TS_TIME_INFO    tsTimeInfo;
}_T_LOG_INFO;

#define MAX_PARAM_NAME_LEN  32
typedef struct T_PARAM_LIST
{
	unsigned int nLastUpdataTime;
	unsigned int nDuration;
	char *szParamName; //[MAX_PARAM_NAME_LEN];
	unsigned int nParamID;
	int	         iMin;
	int	         iMax;
	int	         iRealMax;
	ParamIdentifer ParamIdentifier;
	E_PARAM_TYPE   nDataType;
	unsigned int nMaxCnt;
	unsigned int nTail;
	void *pData;
}_T_PARAM_LIST;


typedef struct _UNZIP_ALL_FILE_
{
	char szLogFile[260];
	//char szXMLFile[260];
	std::vector<std::string> strListXMLFile;
	std::vector<std::string> strListPicture;
}UNZIP_ALL_FILE;

//add 131211 wangyang
/*this struct is designed as parameter needed by PreDecodeRx(PreDecodeCfg& cfg),
*PreDecodeRx() called by DispatchFile program to replace PreDecode()
*which add unhandled log path:PreDecodeCfg::str_unhandled_path
*/
#define SERVICE_COMPLAINT "unhandled_complaint"
#define SERVICE_NORMAL "unhandled_normal"       //satuation:service is not complaint
struct PreDecodeCfg
{
	std::string str_log_path;
	std::string str_service;        //instpection normal spotcheck complaint
	std::string str_scene;          //indoor outdoor
	std::string str_province;
	unsigned int u32_p_id ;
	unsigned int u32_c_id;
	double db_lat;
	double db_lon;
	int s32_start_time;
	std::string str_unhandled_path;
	//PreDecodeCfg()
	//{             }
};
//add end

SFX_EXT_FUNCTION int GetU16(E_UINT16 u16);
SFX_EXT_FUNCTION int GetI16(E_INT16 i16);
SFX_EXT_FUNCTION int GetI32(E_INT32 i32);
SFX_EXT_FUNCTION float GetFLOAT(E_FLOAT u32FLOAT);
SFX_EXT_FUNCTION int CombinedParam(int *pParam, int nParamNum);
SFX_EXT_FUNCTION char *GetDuration( char *szValue, unsigned int nDelay_a, unsigned int nDelay_b, unsigned int* pDelay = NULL);
SFX_EXT_FUNCTION void GetTimeString(char *szTime, unsigned int nTime);
SFX_EXT_FUNCTION unsigned int timeInterval(unsigned int tBeg, unsigned int tEnd);
SFX_EXT_FUNCTION void TransferSize(char *szValue, int iBitsCnt);
SFX_EXT_FUNCTION void TranslateLogGPS(const char *pszMapOffSet, const char *pszCityID);
SFX_EXT_FUNCTION void TranslateLogGPS(const char *pszMapOffSet, const char *pszCityID, float& fLon, float& flat);
SFX_EXT_FUNCTION void GetRealValue(char *szValue, int iValue);
SFX_EXT_FUNCTION unsigned int GetMS_From_nTime( unsigned int nTime );
SFX_EXT_FUNCTION bool TimeData2SystemTime(unsigned short &h, unsigned short &m, unsigned short &s, unsigned short &ms, unsigned int nTime);
SFX_EXT_FUNCTION void _gConverW2M(char *szTitle, wchar_t *wcTitle, unsigned int nWidthCharCnt, unsigned int nMAXBytes);
SFX_EXT_FUNCTION bool _gLog_DecompZIP(UNZIP_ALL_FILE& unZipFiles, char* szAnalysisFolder, char* pCacheFolder, const char* pSourceZip);
SFX_EXT_FUNCTION bool _gLog_InitInfoCfg();
extern "C"{
SFX_EXT_FUNCTION bool  PreDecode(std::string str_log_path, std::string& str_service, std::string& str_scene, std::string& str_province, unsigned int& u32_p_id , unsigned int& u32_c_id, double& db_lat, double& db_lon, int& s32_start_tiem);
SFX_EXT_FUNCTION bool  PreDecodeEx(PreDecodeCfg &cfg);
}

#if defined(WIN32) || defined(_WIN32)
#define LOG_SEPERATOR_DIR	"\\"
#else
#define LOG_SEPERATOR_DIR	"/"
#endif

class SFX_EXT_CLASS CEventDecode
{
public:
	CEventDecode();
	~CEventDecode();
	bool IsLoadedLog(const char *pSrcFileName);
    int  LoadLogFile(const char *pSrcFileName);
	int  GetLogInfo(const char *pSrcFileName);
	void CloseLogFile();
	BOOL IsIndoorLog();
	T_LOG_INFO  *GetCurLogInfo();
	T_LOG_INDOOR_INFO *GetIndoorInfo();


	unsigned int GetAppointTimeValue(unsigned int nTimeIndex);
	unsigned int GetEventCnt(void);
	unsigned int GetMessageCnt(void);
	unsigned int GetTimePointCnt();
	void         GetGPS(unsigned int nTimeIndex, float &fLon, float &fLat);
    int          GetCurrentSystem(unsigned int nTimeIndex);

	//update at 131223:isUnionKpi 
	float GetParamValue_float(unsigned int nParamID, unsigned int nTimeIndex, bool isUnionKpi = false);
	int   GetParamValue_int(unsigned int nParamID,   unsigned int nTimeIndex, bool isUnionKpi = false);
	//update end

	int   GetParamCount( unsigned int nParamID);
	T_PARAM_DATA_INT*   GetParmList_int(unsigned int nParamID);
	T_PARAM_DATA_FLOAT* GetParmList_float(unsigned int nParamID);

	T_PARAM_LIST*   GetParamList();
	T_PARAM_LIST*   GetParamList(int nParamId, bool bIsParamIDentifier = false);
	T_PARAM_MATRIX* GetParamMatrix( unsigned int nParamID);

	int GetMatrixCount(unsigned int nParamID);
	
	T_PARAM_MATRIX *GetParamMatrix( unsigned int nParamID, int nTimeIndex);

	T_PARAM_EVENT_MATRIX     *GetEventMatrix(void);
	T_PARAM_MESSAGE_MATRIX   *GetMessageMatrix(void);
	T_COMPLAINT_EVENT_MATRIX *GetComplanitEventMatrix(void);
private:
	void InitEventItem( T_PARAM_EVENT_ITEM *pEi , unsigned int nIndex, unsigned int nTime, unsigned int nEventID, unsigned int nParam1, unsigned int nParam2 );
	void PushServerState(unsigned int nIndex, int iServerState);
	void PushEventMatrix(T_PARAM_EVENT_ITEM *pEventItem);
	void PushParam_HOS(unsigned int nIndex, unsigned int nTime, unsigned char *pData);
	void PushParam_HOA(unsigned int nIndex, unsigned int nTime, unsigned char *pData);
	void PushParam_HOF(unsigned int nIndex, unsigned int nTime, unsigned char *pData);
	void PushParam_SHO(unsigned int nIndex, unsigned int nTime, unsigned char *pData);
	void PushParam_CREL(unsigned int nIndex, unsigned int nTime, unsigned char *pData);
	void PushParam_LUA(unsigned int nIndex, unsigned int nTime, unsigned char *pData);
	void PushParam_LUS(unsigned int nIndex, unsigned int nTime, unsigned char *pData);
	void PushParam_LUF(unsigned int nIndex, unsigned int nTime, unsigned char *pData);
	void PushParam_RRA(unsigned int nIndex, unsigned int nTime, unsigned char *pData);
	void PushParam_RRC(unsigned int nIndex, unsigned int nTime, unsigned char *pData);
	void PushParam_RRF(unsigned int nIndex, unsigned int nTime, unsigned char *pData);
	void PushParam_RRD(unsigned int nIndex, unsigned int nTime, unsigned char *pData);
	void PushParam_PAA(unsigned int nIndex, unsigned int nTime, unsigned char *pData);
	void PushParam_PAC(unsigned int nIndex, unsigned int nTime, unsigned char *pData);
	void PushParam_PAF(unsigned int nIndex, unsigned int nTime, unsigned char *pData);
	void PushParam_PAD(unsigned int nIndex, unsigned int nTime, unsigned char *pData);
	void PushParam_GAA(unsigned int nIndex, unsigned int nTime, unsigned char *pData);
	void PushParam_GAC(unsigned int nIndex, unsigned int nTime, unsigned char *pData);
	void PushParam_GAF(unsigned int nIndex, unsigned int nTime, unsigned char *pData);
	void PushParam_GAD(unsigned int nIndex, unsigned int nTime, unsigned char *pData);
	void PushParam_RUA(unsigned int nIndex, unsigned int nTime, unsigned char *pData);
	void PushParam_RUF(unsigned int nIndex, unsigned int nTime, unsigned char *pData);
	void PushParam_RUS(unsigned int nIndex, unsigned int nTime, unsigned char *pData);
	void PushParam_MSGA(unsigned int nIndex, unsigned int nTime, unsigned char *pData);
	void PushParam_MSGS(unsigned int nIndex, unsigned int nTime, unsigned char *pData);
	void PushParam_MSGF(unsigned int nIndex, unsigned int nTime, unsigned char *pData);
	void PushParam_AQUL(unsigned int nIndex, unsigned char *pData);
	void PushParam_AQDL(unsigned int nIndex, unsigned char *pData);
	void PushParam_CMPEV(unsigned int nIndex, unsigned int nTime, unsigned char *pData);
	void PushParam_USRBHVEV(unsigned int nIndex, unsigned char *pData);
	void PushParam_ComplanitHTTP(unsigned int nIndex, unsigned int nTime, unsigned char *pData);
	void PushParam_ComplanitAPPInfo(unsigned int nIndex, unsigned int nTime, unsigned char *pData);
	void PushParam_APPOpenInfo(unsigned int nIndex, unsigned int nTime, unsigned char *pData);
	//wangyang add 131203
	void PushParam_ComplanitAPPInfo_V2(unsigned int nIndex, unsigned int nTime, unsigned char *pData);
	void PushPara_ComplanitHTTP_V2(unsigned int nIndex, unsigned int nTime, unsigned char *pData);
	void PushParam_APPOpenInfo_V2(unsigned int nIndex, unsigned int nTime, unsigned char *pData);
	//add end
	void PushParam_OffNet(unsigned int nIndex, unsigned int nTime, unsigned char *pData);
	void PushParam_UserRate(unsigned int nIndex, unsigned int nTime, unsigned char *pData);
	void PushParam_UserAppRate(unsigned int nIndex, unsigned int nTime, unsigned char *pData);

	void PushParam_INDOOR_MARK(unsigned int nIndex, unsigned char *pData);
	void Delete_Indoor_Mark(unsigned char *pData);
	void PushParam_GPS(unsigned int nIndex, unsigned char *pData);

	void PushParam_DAA(unsigned int nIndex, unsigned int nTime, unsigned char *pData);
	void PushParam_DAC(unsigned int nIndex, unsigned int nTime, unsigned char *pData);
	void PushParam_DAF(unsigned int nIndex, unsigned int nTime, unsigned char *pData);
	void PushParam_DAD(unsigned int nIndex, unsigned int nTime, unsigned char *pData);
	void PushParam_DREQ(unsigned int nIndex, unsigned int nTime, unsigned char *pData);
	void PushParam_DCOMP(unsigned int nIndex, unsigned int nTime, unsigned char *pData);
	void PushParam_CAA(unsigned int nIndex, unsigned int nTime, unsigned char *pData);
	void PushParamGSM_CAC(unsigned int nIndex, unsigned int nTime, unsigned char *pData);
	void PushParam_CAC(unsigned int nIndex, unsigned int nTime, unsigned char *pData);
	void PushParam_CAF(unsigned int nIndex, unsigned int nTime, unsigned char *pData);
	void PushParam_CAD(unsigned int nIndex, unsigned int nTime, unsigned char *pData);
	char TranslateMsgDecodeType( unsigned char &eMsgType, unsigned int nEventID, unsigned int nRawDataLen, unsigned char nDirection, char *szMessageName, char *szSubChannelName);
	char *PushMemory_String(char *pDec, char *pScr );
	char GetCharHexValue( char c );
	unsigned char String2Hex( char *pScr, unsigned int nBitLen );
	unsigned char *PushMemory_RawData(unsigned char *pDec, char *pScr, unsigned int &nMsgLen );
	void PushMessageMatrix(unsigned int nEventID, unsigned int nIndex, unsigned int nTime, E_UINT8 u8Direction, char *szMsgName, char *szMsgSubChName, char *szMsgRawData);
	void PushParam_TAD(unsigned int nIndex, unsigned char *pData);
	void PushParam_TBFI(unsigned int nIndex, unsigned char *pData);
	void PushParam_BEP(unsigned int nIndex, unsigned char *pData);
	void PushParam_PRXQ(unsigned int nIndex, unsigned char *pData);
	void PushParam_FER(unsigned int nIndex, unsigned char *pData);
	void PushParam_RLT(unsigned int nIndex, unsigned char *pData);
	void PushParam_DRATE(unsigned int nIndex, unsigned char *pData);
	void PushParam_RLCRATE(unsigned int nIndex, unsigned char *pData);
	void PushParam_RLPRATE(unsigned int nIndex, unsigned char *pData);
	void PushParam_TXPC(unsigned int nIndex, unsigned char *pData);
	void PushParam_RACHI(unsigned int nIndex, unsigned char *pData);
	void PushParam_RXPC(unsigned int nIndex, unsigned char *pData);
	void PushParam_RLCBLER(unsigned int nIndex, unsigned char *pData);
	void PushParam_HOP(unsigned int nIndex, unsigned char *pData);
	void PushParam_CI(unsigned int nIndex, unsigned char *pData);
	void PushParam_MSP(unsigned int nIndex, unsigned char *pData);
	void PushParam_RXQ(unsigned int nIndex, unsigned char *pData);
	void PushParam_PCHI(unsigned int nIndex, unsigned char *pData);
	void PushParam_SEI(unsigned int nIndex, unsigned char *pData);
	void PushParam_SNPSM(unsigned int nEventID, unsigned int nIndex, unsigned int nTime, unsigned char *pData);
	void PushParam_MACSM(unsigned int nEventID, unsigned int nIndex, unsigned int nTime, unsigned char *pData);
	void PushParam_L3SM(unsigned int nEventID, unsigned int nIndex, unsigned int nTime, unsigned char *pData);
	void PushParam_RRCSM(unsigned int nEventID, unsigned int nIndex, unsigned int nTime, unsigned char *pData);
	void PushParam_CHI(unsigned int nIndex, unsigned char *pData);
	void PushParam_CQI(unsigned int nIndex, unsigned char *pData);
	void PushParam_Value_float(unsigned int nParamID, unsigned int nIndex, float fValue);
	void PushParam_WithRealRange(unsigned int nParamID, unsigned int nIndex, int iValue);
	void PushParam_Value_int(unsigned int nParamID, unsigned int nIndex, int iValue);
	void PushParamMatrix(unsigned int nIndex, unsigned int nParamID, unsigned int nCnt, unsigned char *pData);
	void PushParam_CELLMEAS(unsigned int nIndex, unsigned char *pData);
	void PushParam_CMPMEAS(unsigned int nIndex, unsigned char *pData);

	void PushParam(unsigned int nIndex, unsigned int nTime, unsigned int nParamID, unsigned char *pData);
	void PushTimeLine(unsigned int nIndex, unsigned int ntime);
	void PushSystem(unsigned int nIndex, unsigned int nSystem);
	int  PushParamList(FILE *pFile);
	void ReleaseParamList();
	int  CreateParamList();
	bool CreateSystem(unsigned int nStart, unsigned int nEnd);
	void ReleaseBuffer(unsigned int nStart, unsigned int nEnd);
	bool CreateNodeBuf(unsigned int nParamID);
	void CountParam_Value(unsigned int nParamID);
	void ParseLogBase(char *pPacket);
	void CountParam(unsigned short EventID, unsigned char* pBuf);
	EM_LOG_NET_TYPE GetLogNetType(void);
	void SetNetTpye(unsigned char chValue);
	void ClearNode_Matrix( unsigned int nParamID, unsigned int nIndex );
	char m_szMapOffset[MAX_PATH];
	T_LOG_INFO   m_gLogInfo;
	T_USRBHV_EVENT_MATRIX    m_usrbhvEventMatrix;
	T_COMPLAINT_EVENT_MATRIX m_complaintEventMatrix;
	T_PARAM_MESSAGE_MATRIX   m_MsgMatrix;
	T_PARAM_EVENT_MATRIX     m_gEventMatrix;
	T_PARAM_LIST            *m_pParamList;
	unsigned char            m_chNetFlag;
	int                      m_CurSystem;
};
#endif

