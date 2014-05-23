#include <stdio.h>
#include "EventParameterValue.h"

void GetIndexTableValue(T_INDEX_TABLE *it, int iCnt, int iIndex, char *szName)
{
	int i = 0;
	for( i=0; i<iCnt; ++i)
	{
		if( it[i].iID == iIndex ) 
		{
			sprintf(szName, it[i].szName);
			break;
		}
	}
	
	if( iCnt == i )
	{
		sprintf(szName, "Unknow (%d)", iIndex);
	}
}

void GetSystem(int iIndex, char *szName)
{
	//Cellular system
#define MAX_SYSTEM_CNT 19
	T_INDEX_TABLE it[MAX_SYSTEM_CNT] = 
	{
		{TEVENT_SYSTEM_GSM,          "GSM"},          
		{TEVENT_SYSTEM_TETRA,        "TETRA"},        
		{TEVENT_SYSTEM_UMTSFDD,      "UMTS FDD"},
		{TEVENT_SYSTEM_UMTSTD_SCDMA, "UMTS TD-SCDMA"},
		{TEVENT_SYSTEM_CDMAONE,      "cdmaOne"},
		{TEVENT_SYSTEM_CDMA1X,       "CDMA 1x"},     
		{TEVENT_SYSTEM_EVDO,         "EVDO"},  
		{TEVENT_SYSTEM_WLAN,         "WLAN"},     
		{TEVENT_SYSTEM_GANWLAN,      "GAN WLAN"},
		{TEVENT_SYSTEM_WIMAX,        "WiMAX"}, 
		{TEVENT_SYSTEM_NMT,          "NMT"},    
		{TEVENT_SYSTEM_AMPS,         "AMPS"},      
		{TEVENT_SYSTEM_NAMPS,        "NAMPS"},     
		{TEVENT_SYSTEM_DAMPS,        "DAMPS"},    
		{TEVENT_SYSTEM_ETACS,        "ETACS"},    
		{TEVENT_SYSTEM_IDEN,         "iDEN"},    
		{TEVENT_SYSTEM_PSTN,         "PSTN"},     
		{TEVENT_SYSTEM_ISDN,         "ISDN"},     
		{TEVENT_SYSTEM_DVB_H,        "DVB-H"}, 
	};
	GetIndexTableValue(it, MAX_SYSTEM_CNT, iIndex, szName);
}

void GetCallType(int iIndex, char *szName)
{
	//Call type	
#define MAX_CALLTYPE_CNT 9
	T_INDEX_TABLE it[MAX_CALLTYPE_CNT] = 
	{		
		{TEVENT_CAA_CALLTYPE_VOICECALL,                         "Voice call"},                          
		{TEVENT_CAA_CALLTYPE_MARKOVCALL,                        "Markov call"},                         
		{TEVENT_CAA_CALLTYPE_DATACALL,                          "Data call"},                           
		{TEVENT_CAA_CALLTYPE_FAXCALL,                           "Fax call"},                            
		{TEVENT_CAA_CALLTYPE_DIAL_UPBASEDDATACALL,              "Dial-up based data call"},             
		{TEVENT_CAA_CALLTYPE_LOOPBACKCALL_CDMA_,                "Loopback call (CDMA)"},                
		{TEVENT_CAA_CALLTYPE_VIDEOCALL,                         "Video call"},                          
		{TEVENT_CAA_CALLTYPE_PUSH_TO_TALK,                      "Push-to-talk"},                        
		{TEVENT_CAA_CALLTYPE_PUSH_TO_TALKBETWEENMOBILES_TETRA_, "Push-to-talk between mobiles (TETRA)"},		
	};

	GetIndexTableValue(it, MAX_CALLTYPE_CNT, iIndex, szName);
}

void GetCallConnectionStatus(int iIndex, char *szName)
{
	//Call type	
#define MAX_CALL_CONNECTION_STATUS_CNT 4
	T_INDEX_TABLE it[MAX_CALL_CONNECTION_STATUS_CNT] = 
	{		
		{TEVENT_CAC_CALLCONNECTIONST_DTCH,        "Accessed to a dedicated traffic channel"},
		{TEVENT_CAC_CALLCONNECTIONST_ALERTING,    "Alerting"},
		{TEVENT_CAC_CALLCONNECTIONST_ANSWERED,    "Answered"},
		{TEVENT_CAC_CALLCONNECTIONST_DIAL_UP_EST, "Dial-up connection established"},
	};
	
	GetIndexTableValue(it, MAX_CALL_CONNECTION_STATUS_CNT, iIndex, szName);
}

void GetCallFailureStatus(int iIndex, char *szName)
{
	//CS call attempt failure status
#define MAX_CALL_FAILURE_STATUS_CNT 11
	T_INDEX_TABLE it[MAX_CALL_FAILURE_STATUS_CNT] = 
	{
		{TEVENT_CAF_CSCALLATTEMPTFAI_TIMEOUT,                "Timeout before connection"},
		{TEVENT_CAF_CSCALLATTEMPTFAI_RELEASED,               "Call was released before connection"},
		{TEVENT_CAF_CSCALLATTEMPTFAI_SERVICENOTAVAILABLE,    "Service not available"},
		{TEVENT_CAF_CSCALLATTEMPTFAI_INCOMINGCALLNOTRECEIVED,"Incoming call not received"},
		{TEVENT_CAF_CSCALLATTEMPTFAI_TESTSYSTEMFAILURE,      "Test system failure"},
		{TEVENT_CAF_CSCALLATTEMPTFAI_SDCCHBLOCKING,          "SDCCH blocking"},
		{TEVENT_CAF_CSCALLATTEMPTFAI_TCHBLOCKING,            "TCH blocking"},
		{TEVENT_CAF_CSCALLATTEMPTFAI_RRCCONNECTIONFAILED,    "RRC connection failed"},
		{TEVENT_CAF_CSCALLATTEMPTFAI_RADIOBEARERSETUPFAILED, "Radio bearer setup failed"},
		{TEVENT_CAF_CSCALLATTEMPTFAI_RESERVED,               "Reserved"},
		{TEVENT_CAF_CSCALLATTEMPTFAI_PPPERROR,               "PPP error"},
	};
	
	GetIndexTableValue(it, MAX_CALL_FAILURE_STATUS_CNT, iIndex, szName);
}

void GetCallDisconnectStatus(int iIndex, char *szName)
{
	//CS call disconnect status
#define MAX_CALL_DISCONNECT_STATUS_CNT 12
	T_INDEX_TABLE it[MAX_CALL_DISCONNECT_STATUS_CNT] = 
	{		
		{TEVENT_CAD_CSCALLDISCONNECT_NORMALDISCONNECT,                          "Normal disconnect"},
		{TEVENT_CAD_CSCALLDISCONNECT_DROPPEDCALL_NETWORKRELEASE,                "Dropped call (i.e. network release)"},
		{TEVENT_CAD_CSCALLDISCONNECT_DROPPEDOUTOFSERVICE,                       "Dropped out of service"},
		{TEVENT_CAD_CSCALLDISCONNECT_DROPPEDDURINGHANDOVER_HANDOFF_HARDHANDOVER,"Dropped during handover/handoff/hard handover"},
		{TEVENT_CAD_CSCALLDISCONNECT_TESTSYSTEMFAILURE,                         "Test system failure"},
		{TEVENT_CAD_CSCALLDISCONNECT_TRANSFERTIMEOUTDURINGDATAORFAXTESTCALL,    "Transfer timeout during data or fax test call"},
		{TEVENT_CAD_CSCALLDISCONNECT_PROTOCOLERRORDURINGDATAORFAXTESTCALL,      "Protocol error during data or fax test call"},
		{TEVENT_CAD_CSCALLDISCONNECT_TRANSFERFAILEDDURINGDATAORFAXTESTCALL,     "Transfer failed during data or fax test call"},
		{TEVENT_CAD_CSCALLDISCONNECT_MODEMCONNECTIONFAILURE,                    "Modem connection failure"},
		{TEVENT_CAD_CSCALLDISCONNECT_NETWORKBUSY,                               "Network busy"},
		{TEVENT_CAD_CSCALLDISCONNECT_VOICEQUALITYSYNCHRONIZATIONLOST,           "Voice quality synchronization lost"},
		{TEVENT_CAD_CSCALLDISCONNECT_PPPERROR,                                  "PPP error"},
	};
	
	GetIndexTableValue(it, MAX_CALL_DISCONNECT_STATUS_CNT, iIndex, szName);
}

void GetApplicatiionProtocol(int iIndex, char *szName)
{
	//Application protocol
#define MAX_APPLICATION_PROTOCOL_CNT 18
	T_INDEX_TABLE it[MAX_APPLICATION_PROTOCOL_CNT] = 
	{
		{TEVENT_DCOMP_APPLICATIONPROTO_FTP,          "FTP"},
		{TEVENT_DCOMP_APPLICATIONPROTO_HTTP,         "HTTP"},
		{TEVENT_DCOMP_APPLICATIONPROTO_SMTP,         "SMTP"},
		{TEVENT_DCOMP_APPLICATIONPROTO_POP3,         "POP3"},
		{TEVENT_DCOMP_APPLICATIONPROTO_MMS,          "MMS"},
		{TEVENT_DCOMP_APPLICATIONPROTO_WAP1_0,       "WAP 1.0"},
		{TEVENT_DCOMP_APPLICATIONPROTO_STREAMING,    "Streaming"},
		{TEVENT_DCOMP_APPLICATIONPROTO_WAP2_0,       "WAP 2.0"},
		{TEVENT_DCOMP_APPLICATIONPROTO_HTTPBROWSING, "HTTP browsing"},
		{TEVENT_DCOMP_APPLICATIONPROTO_ICMPPING,     "ICMP ping"},
		{TEVENT_DCOMP_APPLICATIONPROTO_IPERFOVERTCP, "IPerf over TCP"},
		{TEVENT_DCOMP_APPLICATIONPROTO_IPERFOVERUDP, "IPerf over UDP"},
		{TEVENT_PROTOCOL_WAP1_LOGON,				 "WAP 1.0 logon"}, 
		{TEVENT_PROTOCOL_WAP2_LOGON,				 "WAP 2.0 logon"},
		{TEVENT_PROTOCOL_WAP1_REFRSH,				 "WAP 1.0 refresh"}, 
		{TEVENT_PROTOCOL_WAP2_REFRSH,				 "WAP 2.0 refresh"}, 
		{TEVENT_PROTOCOL_WAP1_DOWNLOAD,				 "WAP 1.0 download"}, 
		{TEVENT_PROTOCOL_WAP2_DOWNLOAD,				 "WAP 2.0 download"},
	};
	
	GetIndexTableValue(it, MAX_APPLICATION_PROTOCOL_CNT, iIndex, szName);
}

void GetDataConnectionFailureStatus(int iIndex, char *szName)
{
	//Data connection failure status
#define MAX_DATA_CONNECTION_FAILURE_STATUS_CNT 4
	T_INDEX_TABLE it[MAX_DATA_CONNECTION_FAILURE_STATUS_CNT] = 
	{		
		{1, "User abort"},
		{2, "Socket opening error (socket cause)"},
		{3, "Protocol error or timeout"},
		{4, "Measurement tool failure"},
	};
	
	GetIndexTableValue(it, MAX_DATA_CONNECTION_FAILURE_STATUS_CNT, iIndex, szName);
}

void GetDataConnectionDisconnectStatus(int iIndex, char *szName)
{
	//Data disconnect
#define MAX_DATA_CONNECTION_DISCONNECT_STATUS_CNT 4
	T_INDEX_TABLE it[MAX_DATA_CONNECTION_DISCONNECT_STATUS_CNT] = 
	{		
		{1, "Normal data disconnect"},
		{2, "Socket error (socket cause)"},
		{3, "Protocol error or timeout"},
		{4, "Measurement tool failure"},
	};
	
	GetIndexTableValue(it, MAX_DATA_CONNECTION_DISCONNECT_STATUS_CNT, iIndex, szName);
}

void GetDataTransferStatus(int iIndex, char *szName)
{
	//Data disconnect
#define MAX_DATA_TRANSFER_STATUS_CNT 4
	T_INDEX_TABLE it[MAX_DATA_TRANSFER_STATUS_CNT] = 
	{		
		{TEVENT_DCOMP_DATATRANSFERSTAT_TRANSFERSUCCESSFUL,     "Transfer successful"},
		{TEVENT_DCOMP_DATATRANSFERSTAT_SOCKETERROR,            "Socket error"},
		{TEVENT_DCOMP_DATATRANSFERSTAT_PROTOCOLERRORORTIMEOUT, "Protocol error or timeout"},
		{TEVENT_DCOMP_DATATRANSFERSTAT_MEASUREMENTTOOLFAILURE, "Measurement tool failure"},
	};
	
	GetIndexTableValue(it, MAX_DATA_TRANSFER_STATUS_CNT, iIndex, szName);
}

void GetHOAType(int iIndex, char *szName)
{
	//Data disconnect
#define MAX_HANDOVER_ATTEMP_TYPE_CNT 20
	T_INDEX_TABLE it[MAX_HANDOVER_ATTEMP_TYPE_CNT] = 
	{		
		{TEVENT_HOA_HANDOVER_HANDOFF_GSMINTERNALHANDOVER,                                                         "GSM internal handover"},
		{TEVENT_HOA_HANDOVER_HANDOFF_GSMHANDOVERBETWEENCELLS,                                                     "GSM handover between cells"},
		{TEVENT_HOA_HANDOVER_HANDOFF_GSMHANDOVERBETWEENSYSTEMS,                                                   "GSM handover between systems"},
		{TEVENT_HOA_HANDOVER_HANDOFF_GSMHANDOVERBETWEENBANDS,                                                     "GSM handover between bands"},
		{TEVENT_HOA_HANDOVER_HANDOFF_DAMPSHANDOFFBETWEENSECTORSORHANDOFFTOSMALLDIAMETERCELL_SBI_00_,              "DAMPS handoff between sectors or handoff to small diameter cell (SBI, 00)"},
		{TEVENT_HOA_HANDOVER_HANDOFF_DAMPSHANDOFFTOSMALLDIAMETERCELLORHANDOFFTOLARGEDIAMETERCELL_SBI_01ORSBI_10_, "DAMPS handoff to small diameter cell or handoff to large diameter cell (SBI, 01 or SBI, 10)"},
		{TEVENT_HOA_HANDOVER_HANDOFF_DAMPSHANDOFFBETWEENSYSTEMS,                                                  "DAMPS handoff between systems"},
		{TEVENT_HOA_HANDOVER_HANDOFF_CDMAHARDHANDOFF,                                                             "CDMA hard handoff"},
		{TEVENT_HOA_HANDOVER_HANDOFF_CDMAHANDOFFBETWEENSYSTEMS,                                                   "CDMA handoff between systems"},
		{TEVENT_HOA_HANDOVER_HANDOFF_UMTSHARDHANDOVER,                                                            "UMTS hard handover"},
		{TEVENT_HOA_HANDOVER_HANDOFF_UMTSHANDOVERBETWEENSYSTEMS,                                                  "UMTS handover between systems"},
		{TEVENT_HOA_HANDOVER_HANDOFF_TD_SCDMAHARDINTER_FREQUENCYHANDOVER,                                         "TD-SCDMA hard inter-frequency handover"},
		{TEVENT_HOA_HANDOVER_HANDOFF_TD_SCDMAHARDINTRA_FREQUENCYHANDOVER,                                         "TD-SCDMA hard intra-frequency handover"},
		{TEVENT_HOA_HANDOVER_HANDOFF_TD_SCDMAHANDOVERBETWEENSYSTEMS,                                              "TD-SCDMA handover between systems"},
		{TEVENT_HOA_HANDOVER_HANDOFF_TD_SCDMABATONINTER_FREQUENCYHANDOVER,                                        "TD-SCDMA baton inter-frequency handover"},
		{TEVENT_HOA_HANDOVER_HANDOFF_TD_SCDMABATONINTRA_FREQUENCYHANDOVER,                                        "TD-SCDMA baton intra-frequency handover"},
		{TEVENT_HOA_HANDOVER_HANDOFF_TETRAANNOUNCEDCELLRESELECTIONTYPE1,                                          "TETRA announced cell reselection type 1"},
		{TEVENT_HOA_HANDOVER_HANDOFF_TETRAANNOUNCEDCELLRESELECTIONTYPE2,                                          "TETRA announced cell reselection type 2"},
		{TEVENT_HOA_HANDOVER_HANDOFF_TETRAANNOUNCEDCELLRESELECTIONTYPE3,                                          "TETRA announced cell reselection type 3"},
		{TEVENT_HOA_HANDOVER_HANDOFF_GANHANDOVERBETWEENSYSTEMS,                                                   "GAN handover between systems"},
	};
	
	GetIndexTableValue(it, MAX_HANDOVER_ATTEMP_TYPE_CNT, iIndex, szName);
}

void GetLocationAreaUpdateType(int iIndex, char *szName)
{
	//Location area update type
#define MAX_LUA_TYPE_CNT 10
	T_INDEX_TABLE it[MAX_LUA_TYPE_CNT] = 
	{
		{TEVENT_LUA_LOCATIONAREAUPDA_COMBINEDLOCATIONAREAANDROUTINGAREAUPDATE,        "Combined location area and routing area update"},
		{TEVENT_LUA_LOCATIONAREAUPDA_NORMALLOCATIONAREAUPDATE,                        "Normal location area update"},
		{TEVENT_LUA_LOCATIONAREAUPDA_PERIODICUPDATE,                                  "Periodic update"},
		{TEVENT_LUA_LOCATIONAREAUPDA_IMSI_ITSIATTACH,                                 "IMSI/ITSI attach"},
		{TEVENT_LUA_LOCATIONAREAUPDA_ROAMINGLOCATIONUPDATING_TETRA_,                  "Roaming location updating (TETRA)"},
		{TEVENT_LUA_LOCATIONAREAUPDA_MIGRATINGLOCATIONUPDATING_TETRA_,                "Migrating location updating (TETRA)"},
		{TEVENT_LUA_LOCATIONAREAUPDA_CALLRESTORATIONROAMINGLOCATIONUPDATING_TETRA_,   "Call restoration roaming location updating (TETRA)"},
		{TEVENT_LUA_LOCATIONAREAUPDA_CALLRESTORATIONMIGRATINGLOCATIONUPDATING_TETRA_, "Call restoration migrating location updating (TETRA)"},
		{TEVENT_LUA_LOCATIONAREAUPDA_DEMANDLOCATIONUPDATING_TETRA_,                   "Demand location updating (TETRA)"},
		{TEVENT_LUA_LOCATIONAREAUPDA_DISABLEDMSUPDATING_TETRA_,                       "Disabled MS updating (TETRA)"}
	};
	
	GetIndexTableValue(it, MAX_LUA_TYPE_CNT, iIndex, szName);
}

void GetLUFStatus(int iIndex, char *szName)
{
	//Location update failure status
#define MAX_LUF_STATUS_CNT 3
	T_INDEX_TABLE it[MAX_LUF_STATUS_CNT] = 
	{
		{TEVENT_LUF_LOCATIONUPDATEFA_TIMEOUT,                                                                         "Timeout"},                                                                                       
		{TEVENT_LUF_LOCATIONUPDATEFA_REJECTEDBYNETWORK,                                                               "Rejected by network"},                                                                           
		{TEVENT_LUF_LOCATIONUPDATEFA_REJECTEDBYNETWORKAFTERCOMBINEDLOCATIONANDROUTINGAREAUPDATE_CAUSEVALUEISGMMCAUSE_,"Rejected by network after combined location and routing area update (cause value is GMM cause)"},
	};
	
	GetIndexTableValue(it, MAX_LUF_STATUS_CNT, iIndex, szName);
}

void GetMMCause(int iIndex, char *szName)
{
	//Mobily management cause
//#define MAX_MM_CAUSE_CNT 23
 	T_INDEX_TABLE it[] = 
 	{
 		{TEVENT_LUF_MOBILYMANAGEMENT_UNALLOCATEDTMSI,                               "Unallocated TMSI"},
 		{TEVENT_LUF_MOBILYMANAGEMENT_IMSIUNKNOWNHLR,                                "IMSI unknown HLR"},
 		{TEVENT_LUF_MOBILYMANAGEMENT_ILLEGALMS,                                     "Illegal MS"},
 		{TEVENT_LUF_MOBILYMANAGEMENT_IMSIUNKNOWNINVLR,                              "IMSI unknown in VLR"},
 		{TEVENT_LUF_MOBILYMANAGEMENT_IMEINOTACCEPTED,                               "IMEI not accepted"},
 		{TEVENT_LUF_MOBILYMANAGEMENT_ILLEGALME,                                     "Illegal ME"},
 		{TEVENT_LUF_MOBILYMANAGEMENT_PLMNNOTALLOWED,                                "PLMN not allowed"},
 		{TEVENT_LUF_MOBILYMANAGEMENT_LOCATIONAREANOTALLOWED,                        "Location Area not allowed"},
 		{TEVENT_LUF_MOBILYMANAGEMENT_NATIONALROAMINGNOTALLOWEDINTHISLOCATIONAREA,   "National roaming not allowed in this location area"},
 		{TEVENT_LUF_MOBILYMANAGEMENT_NETWORKFAILURE,                                "Network failure"},
 		{TEVENT_LUF_MOBILYMANAGEMENT_CONGESTION,                                    "Congestion"},
 		{TEVENT_LUF_MOBILYMANAGEMENT_SERVICEOPTIONNOTSUPPORTED,                     "Service option not supported"},
 		{TEVENT_LUF_MOBILYMANAGEMENT_REQUESTEDSERVICEOPTIONNOTSUBSCRIBED,           "Requested service option not subscribed"},
 		{TEVENT_LUF_MOBILYMANAGEMENT_SERVICEOPTIONTEMPORARILYOUTOFORDER,            "Service option temporarily out of order"},
 		{TEVENT_LUF_MOBILYMANAGEMENT_CALLCANNOTBEIDENTIFIED,                        "Call cannot be identified"},
 		{TEVENT_LUF_MOBILYMANAGEMENT_SEMANTICALLYINCORRECTMESSAGE,                  "Semantically incorrect message"},
 		{TEVENT_LUF_MOBILYMANAGEMENT_INVALIDMANDATORYINFORMATION,                   "Invalid mandatory information"},
 		{TEVENT_LUF_MOBILYMANAGEMENT_MESSAGETYPENON_EXISTENTORNOTIMPLEMENTED,       "Message type non-existent or not implemented"},
 		{TEVENT_LUF_MOBILYMANAGEMENT_MESSAGETYPENOTCOMPATIBLEWITHTHEPROTOCOLSTATE,  "Message type not compatible with the protocol state"},
 		{TEVENT_LUF_MOBILYMANAGEMENT_INFORMATIONELEMENTNON_EXISTENTORNOTIMPLEMENTED,"Information element non-existent or not implemented"},
 		{TEVENT_LUF_MOBILYMANAGEMENT_CONDITIONALIEERROR,                            "Conditional IE error"},
 		{TEVENT_LUF_MOBILYMANAGEMENT_MESSAGENOTCOMPATIBLEWITHTHEPROTOCOLSTATE,      "Message not compatible with the protocol state"},
 		{TEVENT_LUF_MOBILYMANAGEMENT_PROTOCOLERROR_UNSPECIFIED,                     "Protocol error, unspecified"},
 	};
 	int iMaxMMCauseCNT = sizeof(it)/sizeof(T_INDEX_TABLE);
	GetIndexTableValue(it, iMaxMMCauseCNT, iIndex, szName);
}

void GetRRAEstablishmentCause(int iIndex, char *szName)
{
	//RRC establishment cause
#define MAX_RRA_ESTABLISHMENT_CAUSE_CNT 20
	T_INDEX_TABLE it[MAX_RRA_ESTABLISHMENT_CAUSE_CNT] = 
	{
		{TEVENT_RRA_RRCESTABLISHMENT_ORIGINATINGCONVERSATIONCALL,      "Originating conversation call"},
		{TEVENT_RRA_RRCESTABLISHMENT_ORIGINATINGSTREAMINGCALL,         "Originating streaming call"},
		{TEVENT_RRA_RRCESTABLISHMENT_ORIGINATINGINTERACTIVECALL,       "Originating interactive call"},
		{TEVENT_RRA_RRCESTABLISHMENT_ORIGINATINGBACKGROUNDCALL,        "Originating background call"},
		{TEVENT_RRA_RRCESTABLISHMENT_ORIGINATINGSUBSCRIBEDTRAFFICCALL, "Originating subscribed traffic call"},
		{TEVENT_RRA_RRCESTABLISHMENT_TERMINATINGCONVERSATIONALCALL,    "Terminating conversational call"},
		{TEVENT_RRA_RRCESTABLISHMENT_TERMINATINGSTREAMINGCALL,         "Terminating streaming call"},
		{TEVENT_RRA_RRCESTABLISHMENT_TERMINATINGINTERACTIVECALL,       "Terminating interactive call"},
		{TEVENT_RRA_RRCESTABLISHMENT_TERMINATINGBACKGROUNDCALL,        "Terminating background call"},
		{TEVENT_RRA_RRCESTABLISHMENT_EMERGENCYCALL,                    "Emergency call"},
		{TEVENT_RRA_RRCESTABLISHMENT_INTER_RATCELLRESELECTION,         "Inter-RAT cell reselection"},
		{TEVENT_RRA_RRCESTABLISHMENT_INTER_RATCELLCHANGEORDER,         "Inter-RAT cell change order"},
		{TEVENT_RRA_RRCESTABLISHMENT_REGISTRATION,                     "Registration"},
		{TEVENT_RRA_RRCESTABLISHMENT_DETACH,                           "Detach"},
		{TEVENT_RRA_RRCESTABLISHMENT_ORIGINATINGHIGHPRIORITYSIGNALING, "Originating high priority signaling"},
		{TEVENT_RRA_RRCESTABLISHMENT_ORIGINATINGLOWPRIORITYSIGNALING,  "Originating low priority signaling"},
		{TEVENT_RRA_RRCESTABLISHMENT_CALLRE_ESTABLISHMENT,             "Call re-establishment"},
		{TEVENT_RRA_RRCESTABLISHMENT_TERMINATINGHIGHPRIORITYSIGNALING, "Terminating high priority signaling"},
		{TEVENT_RRA_RRCESTABLISHMENT_TERMINATINGLOWPRIORITYSIGNALING,  "Terminating low priority signaling"},
		{TEVENT_RRA_RRCESTABLISHMENT_TERMINATING_CAUSEUNKNOWN,         "Terminating - cause unknown"},
	};
	
	GetIndexTableValue(it, MAX_RRA_ESTABLISHMENT_CAUSE_CNT, iIndex, szName);
}

void GetRRDReleaseCause(int iIndex, char *szName)
{
	//RRC establishment cause
#define MAX_RRD_RELEASE_CAUSE_CNT 8
	T_INDEX_TABLE it[MAX_RRD_RELEASE_CAUSE_CNT] = 
	{
		{TEVENT_RRD_RRCRELEASECAUSE_NORMALEVENT,                                 "Normal event"},
		{TEVENT_RRD_RRCRELEASECAUSE_UNSPECIFIED,                                 "Unspecified"},
		{TEVENT_RRD_RRCRELEASECAUSE_PRE_EMPTIVERELEASE,                          "Pre-emptive release"},
		{TEVENT_RRD_RRCRELEASECAUSE_CONGESTION,                                  "Congestion"},
		{TEVENT_RRD_RRCRELEASECAUSE_RE_ESTABLISHMENTREJECT,                      "Re-establishment reject"},
		{TEVENT_RRD_RRCRELEASECAUSE_DIRECTEDSIGNALINGCONNECTIONRE_ESTABLISHMENT, "Directed signaling connection re-establishment"},
		{TEVENT_RRD_RRCRELEASECAUSE_USERINACTIVITY,                              "User inactivity"},
		{TEVENT_RRD_RRCRELEASECAUSE_T313EXPIRED,                                 "T313 expired"},
	};
	
	GetIndexTableValue(it, MAX_RRD_RELEASE_CAUSE_CNT, iIndex, szName);
}

void GetPAFFailureStatus(int iIndex, char *szName)
{
	//Packet session connection failure status
#define MAX_PAF_FAILURE_STATUS_CNT 6
	T_INDEX_TABLE it[MAX_PAF_FAILURE_STATUS_CNT] = 
	{
		{TEVENT_PAF_PACKETSESSIONCON_USERABORT,            		  "User abort"},
		{TEVENT_PAF_PACKETSESSIONCON_NETWORKREJECT_SMCAUSE,		  "Network reject (SM cause)"},
		{TEVENT_PAF_PACKETSESSIONCON_MOBILEREJECT_SMCAUSE, 		  "Mobile reject (SM cause)"},
		{TEVENT_PAF_PACKETSESSIONCON_TIMEOUT,              		  "Timeout"},
		{TEVENT_PAF_PACKETSESSIONCON_PPPERROR_OSRASCAUSE,  		  "PPP error (OS RAS cause)"},
		{TEVENT_PAF_PACKETSESSIONCON_TESTSYSTEMFAILURE_OSRASCAUSE,"Test system failure (OS RAS cause)"},
	};
	
	GetIndexTableValue(it, MAX_PAF_FAILURE_STATUS_CNT, iIndex, szName);
}

void GetPAFFailureCause(int iIndex, char *szName)
{
	sprintf(szName, "Not available");
// 	//Packet session failure cause
// #define MAX_PAF_FAILURE_CAUSE_CNT
// 	T_INDEX_TABLE it[MAX_PAF_FAILURE_CAUSE_CNT] = 
// 	{
/*
"LLC or SNDCP failure"},
"Insufficient resources"},
"Unknown or missing access point name"},
"Unknown PDP address or PDP type"},
"User authentication failed"},
"Activation rejected by GGSN"},
"Activation rejected, unspecified"},
"Service option not supported"},
"Requested service option not subscribed"},
"Service option temporarily out of order"},
"NSAPI already used"},
"Regular PDP context deactivation"},
"QoS not accepted"},
"Network failure"},
"Reactivation requested"},
"Feature not supported"},
"Invalid transaction identifier value"},
"Semantically incorrect message"},
"Invalid mandatory information"},
"Message type non-existent or not implemented"},
"Message not compatible with protocol state."},
"Information element non-existent or not implemented"},
"Conditional IE error"},
"Message not compatible with protocol state"},
"Protocol error, unspecified"},
"An operation is pending."},
"An invalid port handle was detected."},
"The specified port is already open."},
"The caller's buffer is too small."},
"Incorrect information was specified."},
"The port information cannot be set."},
"The specified port is not connected."},
"An invalid event was detected."},
"A device was specified that does not exist."},
"A device type was specified that does not exist."},
"An invalid buffer was specified."},
"A route was specified that is not available."},
"A route was specified that is not allocated."},
"An invalid compression was specified."},
"There were insufficient buffers available."},
"The specified port was not found."},
"An asynchronous request is pending."},
"The modem (or other connecting device) is already disconnecting."},
"The specified port is not open."},
"The specified port is not connected."},
"No endpoints could be determined."},
"The system could not open the phone book file."},
"The system could not load the phone book file."},
"The system could not find the phone book entry for this connection."},
"The system could not update the phone book file."},
"The system found invalid information in the phone book file."},
"A string could not be loaded."},
"A key could not be found."},
"The connection was closed."},
"The connection was closed by the remote computer."},
"The modem (or other connecting device) was disconnected due to hardware failure."},
"The user disconnected the modem (or other connecting device)."},
"An incorrect structure size was detected."},
"The modem (or other connecting device) is already in use or is not configured properly."},
"Your computer could not be registered on the remote network."},
"There was an unknown error."},
"The device attached to the port is not the one expected."},
"A string was detected that could not be converted."},
"The request has timed out."},
"No asynchronous net is available."},
"An error has occurred involving NetBIOS."},
"The server cannot allocate NetBIOS resources needed to support the client."},
"One of your computer's NetBIOS names is already registered on the remote network."},
"A network adapter at the server failed."},
"You will not receive network message popups."},
"There was an internal authentication error."},
"The account is not permitted to log on at this time of day."},
"The account is disabled."},
"The password for this account has expired."},
"The account does not have permission to dial in."},
"The remote access server is not responding."},
"The modem (or other connecting device) has reported an error."},
"There was an unrecognized response from the modem (or other connecting device)."},
"A macro required by the modem (or other connecting device) was not found in the device.INF file."},
"A command or response in the device.INF file section refers to an undefined macro."},
"The macro was not found in the device.INF file section."},
"The macro in the device.INF file section contains an undefined macro."},
"The device.INF file could not be opened."},
"The device name in the device.INF or media.INI file is too long."},
"The media.INI file refers to an unknown device name."},
"The device.INF file contains no responses for the command."},
"The device.INF file is missing a command."},
"There was an attempt to set a macro not listed in device.INF file section."},
"The media.INI file refers to an unknown device type."},
"The system has run out of memory."},
"The modem (or other connecting device) is not properly configured."},
"The modem (or other connecting device) is not functioning."},
"The system was unable to read the media.INI file."},
"The connection was terminated."},
"The usage parameter in the media.INI file is invalid."},
"The system was unable to read the section name from the media.INI file."},
"The system was unable to read the device type from the media.INI file."},
"The system was unable to read the device name from the media.INI file."},
"The system was unable to read the usage from the media.INI file."},
"The system was unable to read the maximum connection BPS rate from the media.INI file."},
"The system was unable to read the maximum carrier connection speed from the media.INI file."},
"The phone line is busy."},
"A person answered instead of a modem (or other connecting device)."},
"There was no answer."},
"The system could not detect the carrier."},
"There was no dial tone."},
"The modem (or other connecting device) reported a general error."},
"There was an error in writing the section name."},
"There was an error in writing the device type."},
"There was an error in writing the device name."},
"There was an error in writing the maximum connection speed."},
"There was an error in writing the maximum carrier speed."},
"There was an error in writing the usage."},
"There was an error in writing the default-off."},
"There was an error in reading the default-off."},
"ERROR_EMPTY_INI_FILE"},
"Access was denied because the username and/or password was invalid on the domain."},
"There was a hardware failure in the modem (or other connecting device)."},
"ERROR_NOT_BINARY_MACRO"},
"ERROR_DCB_NOT_FOUND"},
"The state machines are not started."},
"The state machines are already started."},
"The response looping did not complete."},
"A response keyname in the device.INF file is not in the expected format."},
"The modem (or other connecting device) response caused a buffer overflow."},
"The expanded command in the device.INF file is too long."},
"The modem moved to a connection speed not supported by the COM driver."},
"Device response received when none expected."},
"The connection needs information from you, but the application does not allow user interaction."},
"The callback number is invalid."},
"The authorization state is invalid."},
"ERROR_WRITING_INITBPS"},
"There was an error related to the X.25 protocol."},
"The account has expired."},
"There was an error changing the password on the domain. The password might have been too short or might have matched a previously used password."},
"Serial overrun errors were detected while communicating with the modem."},
"The Remote Access Service Manager could not start. Additional information is provided in the event log."},
"The two-way port is initializing. Wait a few seconds and redial."},
"No active ISDN lines are available."},
"No ISDN channels are available to make the call."},
"Too many errors occurred because of poor phone line quality."},
"The Remote Access Service IP configuration is unusable."},
"No IP addresses are available in the static pool of Remote Access Service IP addresses."},
"The connection timed out waiting for a valid response from the remote computer."},
"The connection was terminated by the remote computer."},
"The connection attempt failed because your computer and the remote computer could not agree on PPP control protocols."},
"The remote computer is not responding."},
"Invalid data was received from the remote computer. This data was ignored."},
"The phone number, including prefix and suffix, is too long."},
"The IPX protocol cannot dial out on the modem (or other connecting device) because this computer is not configured for dialing out (it is an IPX router)."},
"The IPX protocol cannot dial in on the modem (or other connecting device) because this computer is not configured for dialing in (the IPX router is not installed)."},
"The IPX protocol cannot be used for dialing out on more than one modem (or other connecting device) at a time."},
"Cannot access TCPCFG.DLL."},
"The system cannot find an IP adapter."},
"SLIP cannot be used unless the IP protocol is installed."},
"Computer registration is not complete."},
"The protocol is not configured."},
"Your computer and the remote computer could not agree on PPP control protocols."},
"Your computer and the remote computer could not agree on PPP control protocols."},
"The PPP link control protocol was terminated."},
"The requested address was rejected by the server."},
"The remote computer terminated the control protocol."},
"Loopback was detected."},
"The server did not assign an address."},
"The authentication protocol required by the remote server cannot use the stored password. Redial, entering the password explicitly."},
"An invalid dialing rule was detected."},
"The local computer does not support the required data encryption type."},
"The remote computer does not support the required data encryption type."},
"The remote computer requires data encryption."},
"The system cannot use the IPX network number assigned by the remote computer. Additional information is provided in the event log."},
"ERROR_INVALID_SMM"},
"ERROR_SMM_UNINITIALIZED"},
"ERROR_NO_MAC_FOR_PORT"},
"ERROR_SMM_TIMEOUT"},
"ERROR_BAD_PHONE_NUMBER"},
"ERROR_WRONG_MODULE"},
"The callback number contains an invalid character. Only the following 18 characters are allowed: 0 to 9, T, P, W, (, ), -, @, and space."},
"A syntax error was encountered while processing a script."},
"The connection could not be disconnected because it was created by the multi-protocol router."},
"The system could not find the multi-link bundle."},
"The system cannot perform automated dial because this connection has a custom dialer specified."},
"This connection is already being dialed."},
"Remote Access Services could not be started automatically. Additional information is provided in the event log."},
"Internet Connection Sharing is already enabled on the connection."},
"An error occurred while the existing Internet Connection Sharing settings were being changed."},
"An error occurred while routing capabilities were being enabled."},
"An error occurred while Internet Connection Sharing was being enabled for the connection."},
"An error occurred while the local network was being configured for sharing."},
"Internet Connection Sharing cannot be enabled. There is more than one LAN connection other than the connection to be shared."},
"No smart card reader is installed."},
"Internet Connection Sharing cannot be enabled. A LAN connection is already configured with the IP address that is required for automatic IP addressing."},
"A certificate could not be found. Connections that use the L2TP protocol over IPSec require the installation of a machine certificate, also known as a computer certificate."},
"Internet Connection Sharing cannot be enabled. The LAN connection selected as the private network has more than one IP address configured. Please reconfigure the LAN connection with a single IP address before enabling Internet Connection Sharing."},
"The connection attempt failed because of failure to encrypt data."},
"The specified destination is not reachable."},
"The remote computer rejected the connection attempt."},
"The connection attempt failed because the network is busy."},
"The remote computer's network hardware is incompatible with the type of call requested."},
"The connection attempt failed because the destination number has changed."},
"The connection attempt failed because of a temporary failure. Try connecting again."},
"The call was blocked by the remote computer."},
"The call could not be connected because the remote computer has invoked the Do Not Disturb feature."},
"The connection attempt failed because the modem (or other connecting device) on the remote computer is out of order."},
"It was not possible to verify the identity of the server."},
"To dial out using this connection you must use a smart card."},
"An attempted function is not valid for this connection."},
"The encryption attempt failed because no valid certificate was found."},
"Connection Sharing (NAT) is currently installed as a routing protocol, and must be removed before enabling Internet Connection Sharing."},
"Internet Connection Sharing cannot be enabled. The LAN connection selected as the private network is either not present, or is disconnected from the network. Please ensure that the LAN adapter is connected before enabling Internet Connection Sharing."},
"You cannot dial using this connection at logon time, because it is configured to use a user name different than the one on the smart card. If you want to use it at logon time, you must configure it to use the user name on the smart card."},
"You cannot dial using this connection at logon time, because it is not configured to use a smart card. If you want to use it at logon time, you must edit the properties of this connection so that it uses a smart card."},
"The L2TP connection attempt failed because there is no valid machine certificate on your computer for security authentication."},
"The L2TP connection attempt failed because the security layer could not authenticate the remote computer."},
"The L2TP connection attempt failed because the security layer could not negotiate compatible parameters with the remote computer."},
"The L2TP connection attempt failed because the security layer encountered a processing error during initial negotiations with the remote computer."},
"The L2TP connection attempt failed because certificate validation on the remote computer failed."},
"The L2TP connection attempt failed because security policy for the connection was not found."},
"The L2TP connection attempt failed because security negotiation timed out."},
"The L2TP connection attempt failed because an error occurred while negotiating security."},
"The Framed Protocol RADIUS attribute for this user is not PPP."},
"The Tunnel Type RADIUS attribute for this user is not correct."},
"The Service Type RADIUS attribute for this user is neither Framed nor Callback Framed."},
"The connection failed because the modem (or other connecting device) was not found. Please make sure that the modem or other connecting device is installed."},
"A certificate could not be found that can be used with this Extensible Authentication Protocol."},
"Not available"},
*/
// 	};
// 	
// 	GetIndexTableValue(it, MAX_PAF_FAILURE_CAUSE_CNT, iIndex, szName);
}

void GetGAFFailureCause(int iIndex, char *szName)
{
	//Packet session connection failure status
#define MAX_GAF_FAILURE_STATUS_CNT 21
	T_INDEX_TABLE it[MAX_GAF_FAILURE_STATUS_CNT] = 
	{
		{TEVENT_GAF_ATTACHFAILURECAU_IMSIUNKNOWNINHLR,                              "IMSI unknown in HLR"},
		{TEVENT_GAF_ATTACHFAILURECAU_ILLEGALMS,                                     "Illegal MS"},
		{TEVENT_GAF_ATTACHFAILURECAU_ILLEGALME,                                     "Illegal ME"},
		{TEVENT_GAF_ATTACHFAILURECAU_GPRSSERVICESNOTALLOWED,                        "GPRS services not allowed"},
		{TEVENT_GAF_ATTACHFAILURECAU_GPRSSERVICESANDNON_GPRSSERVICESNOTALLOWED,     "GPRS services and non-GPRS services not allowed"},
		{TEVENT_GAF_ATTACHFAILURECAU_MSIDENTITYCANNOTBEDERIVEDBYTHENETWORK,         "MS identity cannot be derived by the network"},
		{TEVENT_GAF_ATTACHFAILURECAU_IMPLICITLYDETACHED,                            "Implicitly detached"},
		{TEVENT_GAF_ATTACHFAILURECAU_PLMNNOTALLOWED,                                "PLMN not allowed"},
		{TEVENT_GAF_ATTACHFAILURECAU_LOCATIONAREANOTALLOWED,                        "Location Area not allowed"},
		{TEVENT_GAF_ATTACHFAILURECAU_ROAMINGNOTALLOWEDINTHISLOCATIONAREA,           "Roaming not allowed in this location area"},
		{TEVENT_GAF_ATTACHFAILURECAU_MSCTEMPORARILYNOTREACHABLE,                    "MSC temporarily not reachable"},
		{TEVENT_GAF_ATTACHFAILURECAU_NETWORKFAILURE,                                "Network failure"},
		{TEVENT_GAF_ATTACHFAILURECAU_CONGESTION,                                    "Congestion"},
		{TEVENT_GAF_ATTACHFAILURECAU_SEMANTICALLYINCORRECTMESSAGE,                  "Semantically incorrect message"},
		{TEVENT_GAF_ATTACHFAILURECAU_INVALIDMANDATORYINFORMATION,                   "Invalid mandatory information"},
		{TEVENT_GAF_ATTACHFAILURECAU_MESSAGETYPENON_EXISTENTORNOTIMPLEMENTED,       "Message type non-existent or not implemented"},
		{TEVENT_GAF_ATTACHFAILURECAU_MESSAGETYPENOTCOMPATIBLEWITHTHEPROTOCOLSTATE,  "Message type not compatible with the protocol state"},
		{TEVENT_GAF_ATTACHFAILURECAU_INFORMATIONELEMENTNON_EXISTENTORNOTIMPLEMENTED,"Information element non-existent or not implemented"},
		{TEVENT_GAF_ATTACHFAILURECAU_CONDITIONALIEERROR,                            "Conditional IE error"},
		{TEVENT_GAF_ATTACHFAILURECAU_MESSAGENOTCOMPATIBLEWITHTHEPROTOCOLSTATE,      "Message not compatible with the protocol state"},
		{TEVENT_GAF_ATTACHFAILURECAU_PROTOCOLERROR_UNSPECIFIED,                     "Protocol error, unspecified"},
	};
	
	GetIndexTableValue(it, MAX_GAF_FAILURE_STATUS_CNT, iIndex, szName);
}

void GetRUAType(int iIndex, char *szName)
{
	//Routing area update type
#define MAX_RUA_TYPE_CNT 4
	T_INDEX_TABLE it[MAX_RUA_TYPE_CNT] = 
	{
		{TEVENT_RUA_ROUTINGAREAUPDAT_ROUTINGAREAUPDATE,                                     "Routing area update"},
		{TEVENT_RUA_ROUTINGAREAUPDAT_COMBINEDROUTINGAREAANDLOCATIONAREAUPDATE,              "Combined routing area and location area update"},
		{TEVENT_RUA_ROUTINGAREAUPDAT_COMBINEDROUTINGAREAANDLOCATIONAREAUPDATEWITHIMSIATTACH,"Combined routing area and location area update with IMSI attach"},
		{TEVENT_RUA_ROUTINGAREAUPDAT_PERIODICUPDATE,                                        "Periodic update"},
	};
	
	GetIndexTableValue(it, MAX_RUA_TYPE_CNT, iIndex, szName);
}

void GetSMSType(int iIndex, char *szName)
{
	//SMS message type
#define MAX_SMS_TYPE_CNT 5
	T_INDEX_TABLE it[MAX_SMS_TYPE_CNT] = 
	{
		{TEVENT_MSGA_SMSMESSAGETYPE_DELIVER,     "Deliver"},
		{TEVENT_MSGA_SMSMESSAGETYPE_SUBMIT,      "Submit"},
		{TEVENT_MSGA_SMSMESSAGETYPE_STATUSREPORT,"Status report"},
		{TEVENT_MSGA_SMSMESSAGETYPE_COMMAND,     "Command"},
		{TEVENT_MSGA_SMSMESSAGETYPE_BROADCAST,   "Broadcast"},
	};
	
	GetIndexTableValue(it, MAX_SMS_TYPE_CNT, iIndex, szName);
}

void GetMMSType(int iIndex, char *szName)
{
	//MMS message type
#define MAX_MMS_TYPE_CNT 4
	T_INDEX_TABLE it[MAX_MMS_TYPE_CNT] = 
	{
		{TEVENT_MSGA_MMSMESSAGETYPE_SEND,          "Send"},
		{TEVENT_MSGA_MMSMESSAGETYPE_RETRIEVE,      "Retrieve"},
		{TEVENT_MSGA_MMSMESSAGETYPE_NOTIFICATION,  "Notification"},
		{TEVENT_MSGA_MMSMESSAGETYPE_DELIVERYREPORT,"Delivery report"},
	};
	
	GetIndexTableValue(it, MAX_MMS_TYPE_CNT, iIndex, szName);
}

void GetMMSProtocol(int iIndex, char *szName)
{
	//MMS Protocol
#define MAX_MMS_PROTOCOL_CNT 3
	T_INDEX_TABLE it[MAX_MMS_PROTOCOL_CNT] = 
	{
		{TEVENT_MSGA_MMSTRANSPORTPROT_WAP1_0,  "WAP 1.0"},
		{TEVENT_MSGA_MMSTRANSPORTPROT_SMS,     "SMS"},
		{TEVENT_MSGA_MMSTRANSPORTPROT_WAP2_0,  "WAP 2.0"},
	};
	
	GetIndexTableValue(it, MAX_MMS_PROTOCOL_CNT, iIndex, szName);
}

void GetSMSFailedCause(int iIndex, char *szName)
{
	sprintf(szName, "Failed !");
	//SMS cause
// #define MAX_MMS_PROTOCOL_CNT 3
// 	T_INDEX_TABLE it[MAX_MMS_PROTOCOL_CNT] = 
// 	{
// 		{TEVENT_MSGA_MMSTRANSPORTPROT_WAP1_0,  "WAP 1.0"},
// 		{TEVENT_MSGA_MMSTRANSPORTPROT_SMS,     "SMS"},
// 		{TEVENT_MSGA_MMSTRANSPORTPROT_WAP2_0,  "WAP 2.0"},
// 	};
// 	
// 	GetIndexTableValue(it, MAX_MMS_PROTOCOL_CNT, iIndex, szName);
}

void GetMMSFailedCause(int iIndex, char *szName)
{
	//MMS Protocol
#define MAX_MMS_FAILED_CAUSE_CNT 8
	T_INDEX_TABLE it[MAX_MMS_FAILED_CAUSE_CNT] = 
	{
		{TEVENT_MSGF_MMSCAUSE_UNSPECIFIED,             "Unspecified"},
		{TEVENT_MSGF_MMSCAUSE_SERVICEDENIED,           "Service denied"},
		{TEVENT_MSGF_MMSCAUSE_MESSAGEFORMATCORRUPT,    "Message format corrupt"},
		{TEVENT_MSGF_MMSCAUSE_SENDINGADDRESSUNRESOLVED,"Sending address unresolved"},
		{TEVENT_MSGF_MMSCAUSE_MESSAGENOTFOUND,         "Message not found"},
		{TEVENT_MSGF_MMSCAUSE_NETWORKPROBLEM,          "Network problem"},
		{TEVENT_MSGF_MMSCAUSE_CONTENTNOTACCEPTED,      "Content not accepted"},
		{TEVENT_MSGF_MMSCAUSE_UNSUPPORTEDMESSAGE,      "Unsupported message"},
	};
	
	GetIndexTableValue(it, MAX_MMS_FAILED_CAUSE_CNT, iIndex, szName);
}

