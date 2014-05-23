#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "sfl_system.h"
#include "sfl_file.h"
#include "SDir.h"
#include "EventParameterValue.h"
#include "WMMsgDecode.h"
#include "EventDecode.h"
#include "CommInterface.h"
#include "LogService.h"
#include "TransLonLat.h"

static CGisChecker*  pGisChecker   = CGisChecker::Instance();
static CTransLonLat* g_psTranslate = CTransLonLat::instance();

typedef struct _TEVENT_PARAM_2VALUE_STR_{
	unsigned int nValue;
	char *pValue;
}TEVENT_PARAM_2VALUE_STR;

#define MAX_BAND_CNT 87
TEVENT_PARAM_2VALUE_STR g_TEvent_Band2Value[MAX_BAND_CNT] = {
	{0	,	"Unknown system and band"},
	{1	,	"GSM 850"},
	{2	,	"GSM 900"},
	{3	,	"GSM 1800"},
	{4	,	"GSM 1900"},
	{5	,	"GSM 1800"},//中国的GSM就是 1800
	{6	,	"TETRA band 1"},
	{7	,	"TETRA band 2"},
	{8	,	"TETRA band 3"},
	{9	,	"TETRA band 4"},
	{10	,	"TETRA band 5"},
	{11	,	"TETRA band 6"},
	{12	,	"TETRA band 7"},
	{13	,	"TETRA band 8"},
	{14	,	"TETRA band 9"},
	{15	,	"TETRA band 10"}, 
	{16	,	"TETRA band 11"}, 
	{17	,	"TETRA band 12"}, 
	{18	,	"TETRA band 13"}, 
	{19	,	"TETRA band 14"}, 
	{20	,	"TETRA band 15"}, 
	{21	,	"TETRA"},
	{22	,	"UMTS FDD 2100"},
	{23	,	"UMTS FDD 1900"},
	{24	,	"UMTS FDD 2100-IV"},
	{25	,	"UMTS FDD 850"},
	{26	,	"UMTS FDD 900"},
	{27	,	"UMTS FDD"},
	{28	,	"UMTS TD-SCDMA 2000"},
	{29	,	"UMTS TD-SCDMA"},
	{30	,	"cdmaOne 800"},
	{31	,	"cdmaOne 1900"},
	{32	,	"cdmaOne 900 (TACS)"}, 
	{33	,	"cdmaOne 800 (JTACS)"}, 
	{34	,	"cdmaOne 1800 (Korean)"}, 
	{35	,	"cdmaOne 450 (NMT)"},
	{36	,	"cdmaOne1900-2100(IMT)"},
	{37	,	"cdmaOne700"},
	{38	,	"cdmaOne1800"},
	{39	,	"cdmaOne900"},
	{40	,	"cdmaOne900(SMR)"},
	{41	,	"cdmaOne400(PAMR)"},
	{42	,	"cdmaOne800(PAMR)"},
	{43	,	"cdmaOne"},
	{44	,	"CDMA1x800"},
	{45	,	"CDMA1x1900"},
	{46	,	"CDMA1x900(TACS)"},
	{47	,	"CDMA1x800(JTACS)"},
	{48	,	"CDMA1x1800(Korean)"},
	{49	,	"CDMA1x450(NMT)"},
	{50	,	"CDMA1x1900-2100(IMT)"},
	{51	,	"CDMA1x700"},
	{52	,	"CDMA1x1800"},
	{53	,	"CDMA1x900"},
	{54	,	"CDMA1x900(SMR)"},
	{55	,	"CDMA1x400(PAMR)"},
	{56	,	"CDMA1x800(PAMR)"},
	{57	,	"CDMA1x"},
	{58	,	"EVDO800"},
	{59	,	"EVDO1900"},
	{60	,	"EVDO900(TACS)"},
	{61	,	"EVDO800(JTACS)"},
	{62	,	"EVDO1800(Korean)"},
	{63	,	"EVDO450(NMT)"},
	{64	,	"EVDO1900-2100(IMT)"},
	{65	,	"EVDO700"},
	{66	,	"EVDO1800"},
	{67	,	"EVDO900"},
	{68	,	"EVDO900(SMR)"},
	{69	,	"EVDO400(PAMR)"},
	{70	,	"EVDO800(PAMR)"},
	{71	,	"EVDO"},
	{72	,	"GANWLAN"},
	{73	,	"WiMAX"},
	{74	,	"NMT450"},
	{75	,	"NMT900"},
	{76	,	"NMT"},
	{77	,	"AMPS800"},
	{78	,	"AMPS"},
	{79	,	"NAMPS800"},
	{80	,	"NAMPS"},
	{81	,	"DAMPS800"},
	{82	,	"DAMPS1900"},
	{83	,	"DAMPS"},
	{84	,	"ETACS"},
	{85	,	"iDEN"},
	{86	,	"DVB-H"}
};

int CombinedParam(int *pParam, int nParamNum)
{
	double flSum = 0; 
	double flY = 0; 
	for (int n = 0; n < nParamNum; n++) 
	{ 
		flY = (double)((double)pParam[n] / (double)10); 
		flSum += (double)pow((double)10,(double)flY); 
	} 
	return ((double)10 * (double)log10(flSum)); 
} 

char *GetDuration(char *szValue, unsigned int nDelay_a, unsigned int nDelay_b, unsigned int* pDelay)
{
	unsigned int ih =0, im = 0, is = 0, ims = 0;

	nDelay_a = nDelay_b - nDelay_a;
	ih = nDelay_a/3600000 ;
	if( ih > 0 )
	{
		nDelay_a = nDelay_a - ih*3600000;
	}

	im = nDelay_a/60000 ;
	if( im >0 )
	{
		nDelay_a = nDelay_a - im*60000;
	}

	is = nDelay_a/1000 ;
	if( is >0 )
	{
		nDelay_a = nDelay_a - is*1000;
	}
	ims = nDelay_a%1000;
	if (NULL != pDelay)
	{
		*pDelay = ih*3600000 +im*60000+1000*is+ims;
	}
	sprintf(szValue, "%02d:%02d:%02d.%03d", ih, im, is, ims);
	return szValue;
}

void GetRealValue(char *szValue, int iValue)
{
	if( INVALID_VALUE == iValue )	
	{
		sprintf(szValue, "");
	}
	else
	{
		sprintf(szValue, "%d", iValue);
	}
}

bool TimeData2SystemTime(unsigned short &h, unsigned short &m, unsigned short &s, unsigned short &ms, unsigned int nTime)
{
	h  = TIME_GET_HOUR(nTime);
	m  = TIME_GET_MIN(nTime);
	s  = TIME_GET_SEC(nTime);
	ms = TIME_GET_MSEC(nTime);

	return true;
}

//#include <iconv.h>
//#pragma comment(lib, "iconv.lib")

void _gConverW2M(char *szTitle, wchar_t *wcTitle, unsigned int nWidthCharCnt, unsigned int nMAXBytes)
{
	setlocale(LC_ALL, "");
	size_t size = wcslen(wcTitle)*sizeof(wchar_t);
	wcstombs(szTitle, wcTitle, size);
}


#if defined(WIN32) || defined(_WIN32)
#define LOG_SEPERATOR_DIR	"\\"
#else
#define LOG_SEPERATOR_DIR	"/"
#endif

bool _gLog_DecompZIP(UNZIP_ALL_FILE& unZipFiles, char* szAnalysisFolder, char* pCacheFolder, const char* pSourceZip)
{
	if ((NULL ==szAnalysisFolder)||(NULL ==pSourceZip))
	{
		return false;
	}
	memset(unZipFiles.szLogFile, 0, sizeof(unZipFiles.szLogFile));
	char szAnalysis[260] = {0};
	if (sfl_getcwd(szAnalysis, sizeof(szAnalysis)))
	{
		strcat(szAnalysis, LOG_SEPERATOR_DIR);
		strcat(szAnalysis, pCacheFolder);
		strcpy(szAnalysisFolder, szAnalysis);
		sfl_rmdir(szAnalysis, 1);
		if ( 0 != sfl_access(szAnalysis, MODE_EXISTENCE_ONLY) )
		{
			sfl_mkdir(szAnalysis, 0777);//Create temporary directory to store unzip files.
		}
		if ( 0 == sfl_access(szAnalysis, MODE_WRITE_PERMISSION) )
		{
			strcat(szAnalysis,  LOG_SEPERATOR_DIR);
			if (sfl_unzip(pSourceZip, szAnalysis) )
			{
				sfc::Dir dir(szAnalysis);
				const char* pfile = NULL;
				do
				{
					pfile = dir.next();
					if (pfile != NULL)
					{
						char Suffix[5] = {0};
						int nLen = strlen(pfile);
						if (nLen> 4)
						{
							strncpy(Suffix, pfile+(strlen(pfile)-4), 4);
							if ((strcmp(Suffix, ".log") == 0) || (Suffix[0] != '.'))
							{
								if (strlen(unZipFiles.szLogFile)==0)
								{
									strcat(unZipFiles.szLogFile,  szAnalysis);
									strcat(unZipFiles.szLogFile,  pfile);
								}

							}else if (strcmp(Suffix, ".xml") == 0)
							{
								strcat(unZipFiles.szXMLFile,  szAnalysis);
								strcat(unZipFiles.szXMLFile,  pfile);
							}else
							{
								string strFile;
								strFile += szAnalysis;
								strFile += pfile;
								unZipFiles.strListPicture.push_back(strFile);
							}
						}
					}
				}while (pfile);	
				return true;
			}
		}
	}
	return false;
}

const char* GetFileName(const char* szSourcFile)
{
	if (NULL == szSourcFile)
	{
		return "";
	}
	for(int i = strlen(szSourcFile); i > 0; i--) 
	{ 
		if((szSourcFile[i] == '\\')||(szSourcFile[i] == '/')) 
			return &szSourcFile[i+1]; 
	} 
	return "";
}

bool _gLog_InitInfoCfg()
{
	return pGisChecker->Init("../etc/GeoInfoCfg");
}

bool GetFileMD5(string str_path, string& str_md5, unsigned int& nFileSize)
{
	FILE* p_file = fopen(str_path.c_str(),"rb");
	char* p_buf = NULL;
	char cp_md5[32+1] = {0};
	int s32_len = 0;

	if ( !p_file )
	{
		return false;
	}

	if ( 0 != fseek(p_file,0,SEEK_END) )
	{
		return false;
	}

	s32_len = ftell(p_file);
	if ( s32_len <= 0 )
	{
		return false;
	}

	p_buf = (char*)calloc(s32_len+1,1);
	if ( !p_buf )
	{
		return false;
	}

	fseek(p_file,0,SEEK_SET);
	s32_len = fread(p_buf,1,s32_len,p_file);
	if ( s32_len <= 0 )
	{
		free(p_buf);
		fclose(p_file);
		return false;
	}

	if ( !sfl_md5str(p_buf,s32_len,str_md5) )
	{
		free(p_buf);
		fclose(p_file);
		return false;
	}

	free(p_buf);
	fclose(p_file);

	memcpy(cp_md5,str_md5.c_str(),sizeof(cp_md5));

	if ( !sfl_strupr(cp_md5) )
	{
		str_md5.clear();
		return false;
	}

	str_md5   = cp_md5;
	nFileSize = s32_len;
	return true;
}

unsigned int timeInterval(unsigned int tBeg, unsigned int tEnd)
{
	if ( tBeg == 0 || tEnd == 0 )return 0;
	int t1Hour, t2Hour, t1Min, t2Min, t1Sec, t2Sec, t1Msec, t2Msec;
	t1Hour = TIME_GET_HOUR( tBeg );
	t1Min  = TIME_GET_MIN( tBeg );
	t1Sec  = TIME_GET_SEC( tBeg );
	t1Msec = TIME_GET_MSEC( tBeg );
	t2Hour = TIME_GET_HOUR( tEnd );
	t2Min  = TIME_GET_MIN( tEnd );
	t2Sec  = TIME_GET_SEC( tEnd );
	t2Msec = TIME_GET_MSEC( tEnd );
	int Interval = 0;
	if ((t2Hour - t1Hour < 0) || (tBeg == tEnd))
	{
		//Interval += 0 - t1Msec + ( 0 - t1Sec ) * 1000 + ( 0 - t1Min) * 60 * 1000 + (24 - t1Hour) * 60 * 60 * 1000;
		//Interval += t2Msec + ( t2Sec ) * 1000 + ( t2Min ) * 60 * 1000 + (t2Hour) * 60 * 60 * 1000;
		Interval = 0;
	}
	else
	{
		Interval = t2Msec - t1Msec + ( t2Sec - t1Sec ) * 1000 + ( t2Min - t1Min) * 60 * 1000 + (t2Hour - t1Hour) * 60 * 60 * 1000;
	}
	return Interval;
}

void GetTimeString(char *szTime, unsigned int nTime)
{
	sprintf(szTime, "%02d:%02d:%02d.%03d", TIME_GET_HOUR( nTime ), TIME_GET_MIN(nTime), TIME_GET_SEC(nTime), TIME_GET_MSEC(nTime));
}

unsigned int GetMS_From_nTime( unsigned int nTime )
{
	return (TIME_GET_HOUR(nTime) * 3600000 + TIME_GET_MIN(nTime) * 60000 + TIME_GET_SEC(nTime)*1000 + TIME_GET_MSEC(nTime));
}

void TransferSize(char *szValue, int iBitsCnt)
{
	if( INVALID_VALUE == iBitsCnt )        // 1024 bit
	{
		sprintf(szValue, "%s", INVALID_STRING);
	}
	else if(  iBitsCnt < 0x400 )        // 1024 bit
	{
		sprintf(szValue, "%d bits", iBitsCnt);
	}
	else if( iBitsCnt < 0x100000)  // 1024 * 1024 = 1048576 k bit
	{
		sprintf(szValue, "%.2f kb", ((float)iBitsCnt)/1024);
	}
	else if(iBitsCnt<0x40000000)   // 1024 * 1024 * 1024 = 1073741824 M bit
	{
		sprintf(szValue, "%.2f mb", ((float)iBitsCnt)/1048576);
	}
	else if(iBitsCnt<0x10000000000)// 1024 * 1024 * 1024 * 1024 = 1099511627776 bit
	{
		sprintf(szValue, "%.2f gb", ((float)iBitsCnt)/1073741824);
	}
	else
	{
		sprintf(szValue, "It's too fast !");
	}
}

void GetThroughtput(char *szValue, int iBitsCnt)
{
	TransferSize(szValue, iBitsCnt);
	strcat(szValue, "/s");
}

int GetU16(E_UINT16 u16)
{
	unsigned short nValue = 0;
	memcpy(&nValue, u16, sizeof(E_UINT16));
	return (int)nValue;
}

int GetI16(E_INT16 i16)
{
	short i = 0;
	memcpy(&i, i16, sizeof(E_INT16));
	return (int)i;
}

int GetU32(E_UINT32 u32)
{
	unsigned int nValue = 0;
	memcpy(&nValue, u32, sizeof(E_UINT32));
	return (int)nValue;
}

int GetI32(E_INT32 i32)
{
	int i = 0;
	memcpy(&i, i32, sizeof(E_INT32));
	return i;
}

float GetFLOAT(E_FLOAT u32FLOAT)
{
	float f = 0.0f;
	memcpy(&f, u32FLOAT, sizeof(E_FLOAT));
	return f;
}

float GetDOUBLE(E_DOUBLE uDOUBLE )
{
	double d = 0.0f;
	memcpy(&d, uDOUBLE, sizeof(E_DOUBLE));
	return (float)d;
}

BOOL GetShortFilename(char *szShortFilename, const char *szFullFileName)
{
	if(NULL == szShortFilename || NULL == szFullFileName)
	{
		return FALSE;
	}

	char szBuffer[MAX_PATH] = {0};
	int iLen = strlen(szFullFileName);
	if (0 == iLen)
	{
		return FALSE;
	}
	int i    = iLen;
	for (; i>0; i--)
	{
		if (szFullFileName[i] == '\\')
		{
			break;
		}
	}

	memcpy(szBuffer,  szFullFileName+i+1, iLen - i - 1);
	sprintf(szShortFilename, szBuffer);

	return TRUE;
}

bool PreDecode(std::string str_log_path, std::string& str_service, std::string& str_scene, std::string& str_province, unsigned int& u32_p_id , unsigned int& u32_c_id, double& db_lat, double& db_lon, int& s32_start_tiem)
{
 	CEventDecode logDecode;
	int iRet = logDecode.GetLogInfo(str_log_path.c_str());
 	if (E_RESULT_OK != iRet)
 	{
		WriteLog(LOG_LEVEL_ERROR, "PreDecode()", "logDecode.GetLogInfo(%s) return (%d)", str_log_path.c_str(), iRet);
 		return false;
 	}
 	T_LOG_INFO* pLogInfo = logDecode.GetCurLogInfo();
 	if (pLogInfo->iLogServiceType == EM_LOG_SERVICE_inspection)
 	{
 		str_service = "inspection";
 	}else if (pLogInfo->iLogServiceType == EM_LOG_SERVICE_complaint)
 	{
		str_service = "complaint";
 	}else if (pLogInfo->iLogServiceType == EM_LOG_SERVICE_spotcheck)
 	{
		str_service = "spotcheck";
 	}else
 	{
 		str_service = "normal";
 	}
 	if (pLogInfo->bIsIndoor)
 	{
 		str_scene = "indoor";
 	}else
 	{
 		str_scene = "outdoor";
 	}
 	db_lat = pLogInfo->fLat;
 	db_lon = pLogInfo->fLon;
 	
 	unsigned short iMsTime = 0;
 	struct tm t; 
 	memset(&t, 0, sizeof(struct tm)); 
 	t.tm_mon  = pLogInfo->wMonth -1; 
 	t.tm_year = pLogInfo->wYear -1900; 
 	t.tm_mday = pLogInfo->wDay;
 	unsigned short usH  = 0; 
 	unsigned short usM  = 0;
 	unsigned short usS  = 0;
 	TimeData2SystemTime(usH, usM, usS, iMsTime, pLogInfo->nStartTime);
 	t.tm_hour = usH;
 	t.tm_min  = usM;
 	t.tm_sec  = usS;
 	s32_start_tiem = mktime(&t);
	if (pLogInfo->iProvinceID == 0)
	{
		LON_LAT stPt;
		stPt.dLat = pLogInfo->fLat;
		stPt.dLon = pLogInfo->fLon;
		string strValue;
		if ( pGisChecker && (stPt.dLat>0.1) && (stPt.dLon>0.1))
		{
			pGisChecker->Check(stPt, pLogInfo->iProvinceID, pLogInfo->iAreaID, strValue);
			pLogInfo->iCityID = pLogInfo->iAreaID/100;
			u32_p_id = pLogInfo->iProvinceID;
			u32_c_id = pLogInfo->iCityID;
		}else
		{
			WriteLog(LOG_LEVEL_ERROR, "PreDecode()", "Lon:%f, Lat:%f", pLogInfo->fLon, pLogInfo->fLat);
		}
	}else
	{
		u32_p_id = pLogInfo->iProvinceID;
		u32_c_id = pLogInfo->iCityID;
	}
	if (pLogInfo->iProvinceID == 0)
	{
		WriteLog(LOG_LEVEL_ERROR, "PreDecode()", "pLogInfo->iProvinceID==%d, pLogInfo->iCityID==%d", pLogInfo->iProvinceID, pLogInfo->iCityID);
		return false;
	}
	pGisChecker->GetProvinceStrByID(u32_p_id, str_province);
	return true;
}

CEventDecode::CEventDecode()
{
	_gLog_InitInfoCfg();
	if (!g_psTranslate->InitOffsetLibary("../etc/MapOffsetCfg"))
	{
		WriteLog(LOG_LEVEL_ERROR, "CEventDecode::InitOffsetLibary()", "%s(fail)", "../etc/offset.dat");
	}
	else
	{
		WriteLog(LOG_LEVEL_WARNING, "CEventDecode::InitOffsetLibary()", "%s(success)", "../etc/offset.dat");
	}
	memset(m_szMapOffset,   0, sizeof(m_szMapOffset));
	memset(&m_gLogInfo,     0, sizeof(m_gLogInfo));
	memset(&m_MsgMatrix,    0, sizeof(m_MsgMatrix));
	memset(&m_gEventMatrix, 0, sizeof(m_gEventMatrix));
	memset(&m_complaintEventMatrix, 0, sizeof(m_complaintEventMatrix));
	memset(&m_usrbhvEventMatrix, 0, sizeof(m_usrbhvEventMatrix));

    T_PARAM_LIST gParamList[E_PARAM_MAX] = {
		{0,0,"Time Line",E_PARAM_TIME_LINE,0,0,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_TIME,0, 0, NULL},
		{0,0,"Message(s)",E_PARAM_MESSAGE,0,0,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_INT,0, 0, NULL},
		{0,0,"Event(s)",E_PARAM_EVENT,0,0,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_INT,0, 0, NULL},
		{0,0,"System",E_PARAM_SYSTEM,0,65535,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_INT,0, 0, NULL},
		{0,0,"ServerState",E_PARAM_SERVER_STATE,-65535,65535,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_INT,0, 0, NULL},

		{0,0,"Longitude",E_PARAM_GPS_LON,0,0xfffffff,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_FLOAT,0, 0, NULL},
		{0,0,"Latitude", E_PARAM_GPS_LAT,0,0xfffffff,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_FLOAT,0, 0, NULL},

		{0,0,"Longitude(A)",E_PARAM_GPS_LON_A,0,0xfffffff,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_FLOAT,0, 0, NULL},
		{0,0,"Latitude(A)",E_PARAM_GPS_LAT_A,0,0xfffffff,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_FLOAT,0, 0, NULL},

		{0,0,"Distance(M)",E_PARAM_GPS_DISTANCE,0,65535,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_INT,0, 0, NULL},
		{0,0,"fixquality",E_PARAM_GPS_FIX,0,65535,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_INT,0, 0, NULL},
		{0,0,"satellites",E_PARAM_GPS_SATELLITES,0,65535,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_INT,0, 0, NULL},
		{0,0,"Velocity(Km/h)",E_PARAM_GPS_VELOCITY,0,65535,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_INT,0, 0, NULL},

		{0,0,"AppDL",E_PARAM_THROUGHT_APP_DL,0,0xffffff,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_INT,0, 0, NULL},
		{0,0,"AppUL",E_PARAM_THROUGHT_APP_UL,0,0xffffff,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_INT,0, 0, NULL},

		{0,0,"AppDL Bytes",E_PARAM_THROUGHT_APP_DL_BYTES,0,0xffffff,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_INT,0, 0, NULL},
		{0,0,"AppUL Bytes",E_PARAM_THROUGHT_APP_UL_BYTES,0,0xffffff,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_INT,0, 0, NULL},

		{0,0,"RLCDL",E_PARAM_THROUGHT_RLC_DL,0,0xffffff,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_INT,0, 0, NULL},
		{0,0,"RLCUL",E_PARAM_THROUGHT_RLC_UL,0,0xffffff,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_INT,0, 0, NULL},
		{0,0,"RLCULRetransmission",E_PARAM_THROUGHT_RLC_UL_RETRANSMISSION,0,100,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_FLOAT,0, 0, NULL},

		//GSM:[SERVER]
		{0,0,"Band",E_PARAM_GSM_BAND,0,32,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_INT,0, 0, NULL},
		{0,0,"CI",  E_PARAM_GSM_CI, RANGE_GSM_CI_MIN,RANGE_GSM_CI_MAX,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_INT,0, 0, NULL},
		{0,0,"MNC", E_PARAM_GSM_MNC,RANGE_GSM_MNC_MIN,RANGE_GSM_MNC_MAX,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_INT,0, 0, NULL},
		{0,0,"MCC", E_PARAM_GSM_MCC,RANGE_GSM_MCC_MIN,RANGE_GSM_MCC_MAX,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_INT,0, 0, NULL},
		{0,0,"LAC", E_PARAM_GSM_LAC,RANGE_GSM_LAC_MIN,RANGE_GSM_LAC_MAX,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_INT,0, 0, NULL},
		{0,0,"RAC", E_PARAM_GSM_RAC,RANGE_GSM_RAC_MIN,RANGE_GSM_RAC_MAX,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_INT,0, 0, NULL},
		{0,0,"BCCH",E_PARAM_GSM_BCCH,0,65535,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_INT,0, 0, NULL},

		{0,0,"ChannelType",      E_PARAM_GSM_CH_TYPE,1,2,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_INT,0, 0, NULL},
		{0,0,"HoppingActivities",E_PARAM_GSM_HOPPING_ACTIVITIES,1,2,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_INT,0, 0, NULL},
		{0,0,"HoppingFrequencies",E_PARAM_GSM_HOPPING_FREQUENCIES,1,2,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_INT,0, 0, NULL}, //
		{0,0,"MOS(GSM)",          E_PARAM_GSM_MOS,0,5,0,ID_GSM_MOS,E_PARAM_TYPE_FLOAT,0, 0, NULL},

		{0,0,"RxLevFull",E_PARAM_GSM_RXLEV_FULL,RANGE_GSM_RXLEV_MIN,RANGE_GSM_RXLEV_MAX,RANGE_GSM_RXLEV_MIN,ID_PARAM_UNKNOW,E_PARAM_TYPE_INT,0, 0, NULL},
		{0,0,"RxLevSub",E_PARAM_GSM_RXLEV_SUB,RANGE_GSM_RXLEV_MIN,RANGE_GSM_RXLEV_MAX,RANGE_GSM_RXLEV_MIN,ID_GSM_RXLEVEL_SUB,E_PARAM_TYPE_INT,0, 0, NULL},

		//GSMCELLS(Rxlev:dBm)
		{0,0,"Rxlev1",E_PARAM_GSM_RXLEV_N1,RANGE_GSM_RXLEV_MIN,RANGE_GSM_RXLEV_MAX,RANGE_GSM_RXLEV_MIN,ID_PARAM_UNKNOW,E_PARAM_TYPE_INT,0, 0, NULL},
		{0,0,"Rxlev2",E_PARAM_GSM_RXLEV_N2,RANGE_GSM_RXLEV_MIN,RANGE_GSM_RXLEV_MAX,RANGE_GSM_RXLEV_MIN,ID_PARAM_UNKNOW,E_PARAM_TYPE_INT,0, 0, NULL},
		{0,0,"Rxlev3",E_PARAM_GSM_RXLEV_N3,RANGE_GSM_RXLEV_MIN,RANGE_GSM_RXLEV_MAX,RANGE_GSM_RXLEV_MIN,ID_PARAM_UNKNOW,E_PARAM_TYPE_INT,0, 0, NULL},
		{0,0,"Rxlev4",E_PARAM_GSM_RXLEV_N4,RANGE_GSM_RXLEV_MIN,RANGE_GSM_RXLEV_MAX,RANGE_GSM_RXLEV_MIN,ID_PARAM_UNKNOW,E_PARAM_TYPE_INT,0, 0, NULL},
		{0,0,"Rxlev5",E_PARAM_GSM_RXLEV_N5,RANGE_GSM_RXLEV_MIN,RANGE_GSM_RXLEV_MAX,RANGE_GSM_RXLEV_MIN,ID_PARAM_UNKNOW,E_PARAM_TYPE_INT,0, 0, NULL},
		{0,0,"Rxlev6",E_PARAM_GSM_RXLEV_N6,RANGE_GSM_RXLEV_MIN,RANGE_GSM_RXLEV_MAX,RANGE_GSM_RXLEV_MIN,ID_PARAM_UNKNOW,E_PARAM_TYPE_INT,0, 0, NULL},

		{0,0,"RxQualFull",E_PARAM_GSM_RXQUAL_FULL,RANGE_GSM_RXQUAL_MIN,RANGE_GSM_RXQUAL_MAX,RANGE_GSM_RXQUAL_MIN,ID_PARAM_UNKNOW,E_PARAM_TYPE_INT,0, 0, NULL},
		{0,0,"RxQualSub",E_PARAM_GSM_RXQUAL_SUB,RANGE_GSM_RXQUAL_MIN,RANGE_GSM_RXQUAL_MAX,RANGE_GSM_RXQUAL_MIN,ID_GSM_RXQUALITY_SUB,E_PARAM_TYPE_INT,0, 0, NULL},
		{0,0,"FERFull",E_PARAM_GSM_FER_FULL,0,100,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_FLOAT,0, 0, NULL},
		{0,0,"FERSub",E_PARAM_GSM_FER_SUB,0,100,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_FLOAT,0, 0, NULL},
		{0,0,"FERTCH",E_PARAM_GSM_FER_TCH,0,100,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_FLOAT,0, 0, NULL},

		{0,0,"C/I",E_PARAM_GSM_CBI,-10,40,0,ID_GSM_CBI,E_PARAM_TYPE_FLOAT,0, 0, NULL},
		{0,0,"C1",E_PARAM_GSM_C1,0,65534,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_INT,0, 0, NULL},
		{0,0,"C2",E_PARAM_GSM_C2,0,65534,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_INT,0, 0, NULL},
		{0,0,"DTX",E_PARAM_GSM_DTX,0,1,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_INT,0, 0, NULL},
		{0,0,"Timeslot Number",E_PARAM_GSM_TN,0,7,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_INT,0, 0, NULL},
		{0,0,"MSPower",E_PARAM_GSM_TXPOWER,0,31,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_INT,0, 0, NULL},
		{0,0,"MSPower(dBm)",E_PARAM_GSM_TXPOWER_EX,-19,43,0,ID_GSM_TXPOWER,E_PARAM_TYPE_INT,0, 0, NULL},
		{0,0,"TA",E_PARAM_GSM_TA,0,63,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_INT,0, 0, NULL},
		{0,0,"MAIO",E_PARAM_GSM_MAIO,0,63,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_INT,0, 0, NULL},
		{0,0,"HSN",E_PARAM_GSM_HSN,0,63,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_INT,0, 0, NULL},
		{0,0,"Hopping",E_PARAM_GSM_HCH,0,1023,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_INT,0, 0, NULL},
		{0,0,"ChannelNumber", E_PARAM_GSM_CHI_CHANNEL,0,65535,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_INT,0, 0, NULL},
		{0,0,"Voice Code",E_PARAM_GSM_VOICE_CODEC,0,61,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_INT,0, 0, NULL},
		{0,0,"CRO",E_PARAM_GSM_CRO,0,63,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_INT,0, 0, NULL},
		{0,0,"CRH",E_PARAM_GSM_CRH,0,7,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_INT,0, 0, NULL},
		{0,0,"RLT",E_PARAM_GSM_RLT,0,64,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_INT,0, 0, NULL},
		{0,0,"Bler",E_PARAM_GSM_BLER,0,100,0,ID_GSM_BLER,E_PARAM_TYPE_FLOAT,0, 0, NULL},

		//GSM:[EDIG/GPRS]
		{0,0,"PacketTech",E_PARAM_GSM_PTECH,0,13,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_INT,0, 0, NULL},
		{0,0,"PacketRxQual",E_PARAM_GSM_PRXQUAL,0,7,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_INT,0, 0, NULL},
		{0,0,"CValue",E_PARAM_GSM_PCVALUE, -120,-48,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_INT,0, 0, NULL},
		{0,0,"TSUL",E_PARAM_GSM_PTSUP,0,65535,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_INT,0, 0, NULL},
		{0,0,"TSDL",E_PARAM_GSM_PTSDOWN,0,65535,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_INT,0, 0, NULL},
		{0,0,"CodingUL",E_PARAM_GSM_PCODINGUP,0,10,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_INT,0, 0, NULL},
		{0,0,"CodingDL",E_PARAM_GSM_PCODINGDOWN,0,10,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_INT,0, 0, NULL},
		{0,0,"WinSizeDL",E_PARAM_GSM_PWINSIZEDL,64,1024,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_INT,0, 0, NULL},
		{0,0,"WinSizeUL",E_PARAM_GSM_PWINSIZEUL,64,1024,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_INT,0, 0, NULL},
		{0,0,"RLCBLERDL(%)",E_PARAM_GSM_PBLERDOWN,0,100,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_INT,0, 0, NULL},
		{0,0,"8PSKCVBEP",E_PARAM_GSM_P8PSKCVBEP,0,7,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_INT,0, 0, NULL},
		{0,0,"GMSKCVBEP",E_PARAM_GSM_PGMSKCVBEP,0,7,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_INT,0, 0, NULL},
		{0,0,"8PSKMeanBEP",E_PARAM_GSM_P8BSKMEANBEP,0,31,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_INT,0, 0, NULL},
		{0,0,"GMSKMeanBEP",E_PARAM_GSM_PGMSKMEANBEP,0,31,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_INT,0, 0, NULL},

		//cell(s)
		{0,0,"Neighbor(s)",E_PARAM_GSM_CELLS,0,1,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_GSM_CELL,0, 0, NULL},
		{0,0,"Neighbor(s)",E_PARAM_GSM_COMPLANIT_CELLS,0,1,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_GSM_COMPLAINT_CELL,0, 0, NULL},

		//GSM : just for title
		{0,0,"Type",E_PARAM_GSM_CELL_CELLTYPE,0,1,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_JUST_FOR_TITLE,0, 0, NULL},
		{0,0,"Arfcn",E_PARAM_GSM_CELL_ARFCN,0,65535,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_JUST_FOR_TITLE,0, 0, NULL},
		{0,0,"CellID",E_PARAM_GSM_CELL_ID,0,65535,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_JUST_FOR_TITLE,0, 0, NULL},
		{0,0,"LAC",E_PARAM_GSM_CELL_LAC,1,65534,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_JUST_FOR_TITLE,0, 0, NULL},
		{0,0,"RAC",E_PARAM_GSM_CELL_RAC,0,255,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_JUST_FOR_TITLE,0, 0, NULL},
		{0,0,"BSIC",E_PARAM_GSM_CELL_BSIC,0,63,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_JUST_FOR_TITLE,0, 0, NULL},
		{0,0,"Rxlev",E_PARAM_GSM_CELL_RXLEV_FULL,RANGE_GSM_RXLEV_MIN,RANGE_GSM_RXLEV_MAX,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_JUST_FOR_TITLE,0, 0, NULL},
		{0,0,"Rxlev",E_PARAM_GSM_CELL_RXLEV_SUB,RANGE_GSM_RXLEV_MIN,RANGE_GSM_RXLEV_MAX,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_JUST_FOR_TITLE,0, 0, NULL},
		{0,0,"C1",E_PARAM_GSM_CELL_C1,0,65534,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_JUST_FOR_TITLE,0, 0, NULL},
		{0,0,"C2",E_PARAM_GSM_CELL_C2,0,65534,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_JUST_FOR_TITLE,0, 0, NULL},
		{0,0,"Srxlev",E_PARAM_GSM_CELL_SRXLEV,-107,90,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_JUST_FOR_TITLE,0, 0, NULL},
		{0,0,"Band",E_PARAM_GSM_CELL_BAND,0,65535,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_JUST_FOR_TITLE,0, 0, NULL},
		{0,0,"Cell name",E_PARAM_GSM_CELL_NAME,0,0,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_JUST_FOR_TITLE,0, 0, NULL},

		//WService(28)
		{0,0,"UARFCN",E_PARAM_W_SERV_UARFCN,0,65535,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_INT,0, 0, NULL},
		{0,0,"PSC",E_PARAM_W_SERV_SCR,0,65535,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_INT,0, 0, NULL},
		{0,0,"Rqual",E_PARAM_W_SERV_RQUAL,-200,50,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_INT,0, 0, NULL},
		{0,0,"Squal",E_PARAM_W_SERV_SQUAL,-24, 24,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_INT,0, 0, NULL},
		{0,0,"Srxlev",E_PARAM_W_SERV_SRXLEV,-107,90,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_INT,0, 0, NULL},
		{0,0,"Rrxlev",E_PARAM_W_SERV_RRXLEV,-191,25,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_INT,0, 0, NULL},
		{0,0,"Service State",E_PARAM_W_SERVICE_STATE,0,0xfffffff,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_INT,0, 0, NULL},
		{0,0,"CI",E_PARAM_W_SERV_CI,0,0xfffffff,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_INT,0, 0, NULL},
		{0,0,"Band",E_PARAM_W_SERV_BAND,0,0xfffffff,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_INT,0, 0, NULL},
		{0,0,"Treselection",E_PARAM_W_SERV_TRESELECTION,0,0xfffffff,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_INT,0, 0, NULL},
		{0,0,"Additionwindow",E_PARAM_W_SERV_ADDITIONWINDOW,-2,14,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_FLOAT,0, 0, NULL},
		{0,0,"LAC",E_PARAM_W_SERV_LAC,0,65535,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_INT,0, 0, NULL},
		{0,0,"RAC",E_PARAM_W_SERV_RAC,0,65535,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_INT,0, 0, NULL},
		{0,0,"MCC",E_PARAM_W_SERV_MCC,0,999,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_INT,0, 0, NULL},
		{0,0,"MNC",E_PARAM_W_SERV_MNC,0,99,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_INT,0, 0, NULL},
		{0,0,"NMO",E_PARAM_W_SERV_NMO,0,10,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_INT,0, 0, NULL},

		{0,0,"Ec/No",          E_PARAM_W_SERV_EC_NO,       -30,0,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_INT,0,  0, NULL},
		{0,0,"Best Ec/No",     E_PARAM_W_SERV_EC_NO_BEST,  -25,0,0,ID_WCDMA_BEST_ECIO,E_PARAM_TYPE_INT,0,  0, NULL},
		{0,0,"Combined Ec/No", E_PARAM_W_SERV_EC_NO_COMBIN,-25,0,0,ID_WCDMA_TOTAL_ECIO,E_PARAM_TYPE_INT,0,  0, NULL}, //
		{0,0,"MOS(wcdma)",     E_PARAM_W_MOS,               0, 5,0,ID_WCDMA_MOS,E_PARAM_TYPE_FLOAT,0,0,  NULL},

		{0,0,"Max Allowed TXPower",E_PARAM_W_MAX_ALLOWED_TX_POWER,-53,33,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_INT,0, 0, NULL},
		{0,0,"RRCState",E_PARAM_W_RRC_STATE,RANGE_DEFAULT_MIN,RANGE_DEFAULT_MAX,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_INT,0, 0, NULL},

		{0,0,"Combin RSCP",E_PARAM_W_SERV_RSCP_COMBIN, RANGE_W_RSCP_MIN,RANGE_W_RSCP_MAX,RANGE_W_RSCP_MIN,ID_WCDMA_TOTAL_RSCP,E_PARAM_TYPE_INT,0, 0, NULL},
		{0,0,"CPICH RSCP", E_PARAM_W_CPICH_RSCP,   RANGE_W_RSCP_MIN,RANGE_W_RSCP_MAX,RANGE_W_RSCP_MIN,ID_WCDMA_BEST_RSCP,E_PARAM_TYPE_INT,0, 0, NULL},
		{0,0,"RSCP1",      E_PARAM_W_CPICH_RSCP_N1,RANGE_W_RSCP_MIN,RANGE_W_RSCP_MAX,RANGE_W_RSCP_MIN,ID_PARAM_UNKNOW,E_PARAM_TYPE_INT,0, 0, NULL},
		{0,0,"RSCP2",      E_PARAM_W_CPICH_RSCP_N2,RANGE_W_RSCP_MIN,RANGE_W_RSCP_MAX,RANGE_W_RSCP_MIN,ID_PARAM_UNKNOW,E_PARAM_TYPE_INT,0, 0, NULL},
		{0,0,"RSCP3",      E_PARAM_W_CPICH_RSCP_N3,RANGE_W_RSCP_MIN,RANGE_W_RSCP_MAX,RANGE_W_RSCP_MIN,ID_PARAM_UNKNOW,E_PARAM_TYPE_INT,0, 0, NULL},
		{0,0,"RSCP4",      E_PARAM_W_CPICH_RSCP_N4,RANGE_W_RSCP_MIN,RANGE_W_RSCP_MAX,RANGE_W_RSCP_MIN,ID_PARAM_UNKNOW,E_PARAM_TYPE_INT,0, 0, NULL},
		{0,0,"RSCP5",      E_PARAM_W_CPICH_RSCP_N5,RANGE_W_RSCP_MIN,RANGE_W_RSCP_MAX,RANGE_W_RSCP_MIN,ID_PARAM_UNKNOW,E_PARAM_TYPE_INT,0, 0, NULL},
		{0,0,"RSCP6",      E_PARAM_W_CPICH_RSCP_N6,RANGE_W_RSCP_MIN,RANGE_W_RSCP_MAX,RANGE_W_RSCP_MIN,ID_PARAM_UNKNOW,E_PARAM_TYPE_INT,0, 0, NULL},

		{0,0,"RAC",               E_PARAM_W_RAC,                0,65535,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_INT,0, 0, NULL},
		{0,0,"Pwroffset",         E_PARAM_W_PWROFFSET,          0,65535,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_FLOAT,0, 0, NULL},
		{0,0,"PLnonmax",          E_PARAM_W_PLNONMAX,           0,1,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_FLOAT,0, 0, NULL}, 
		{0,0,"EDPCCHpoweroffset", E_PARAM_W_EDPCCH_PWROFFSET,	0,6,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_FLOAT,0, 0, NULL}, 
		{0,0,"HSDSCHSC",          E_PARAM_W_HSDSCHSC,           0,512,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_INT,0, 0, NULL}, 
		{0,0,"HRNTI",             E_PARAM_W_HRNTI,              0,65535,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_INT,0, 0, NULL}, 
		{0,0,"PrimaryERNTI",      E_PARAM_W_PRIMARY_ERNTI,      0,65535,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_INT,0, 0, NULL}, 
		{0,0,"SecondaryERNTI",    E_PARAM_W_SECONDARY_ERNTI,	0,65535,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_INT,0, 0, NULL}, 
		{0,0,"Happybitdelaycond", E_PARAM_W_HAPPYBITDELAYCOND,  2,1000,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_INT,0, 0, NULL},
		{0,0,"Packettech",        E_PARAM_W_PACKETTECH,         1,12,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_INT,0, 0, NULL},
		{0,0,"Packetstate",       E_PARAM_W_PACKETSTATE,        0,6,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_INT,0, 0, NULL},
		{0,0,"NWoperation",       E_PARAM_W_NWOPERATION,        1,3,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_INT,0, 0, NULL},
		{0,0,"HSDPAUEcateg",      E_PARAM_W_HSDPAUECATEG,       1,12,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_INT,0, 0, NULL},
		{0,0,"ACKNACKrepetitions",E_PARAM_W_ACKNACK_REPETITIONS,1,4,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_INT,0, 0, NULL},
		{0,0,"HSUPAUEcateg",      E_PARAM_W_HSUPAUE_CATEG,      1,6,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_INT,0, 0, NULL},
		{0,0,"TTI",               E_PARAM_W_TTI,                2,10,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_INT,0, 0, NULL},
		{0,0,"Ratematching",      E_PARAM_W_RATEMATCHING,       1,2,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_INT,0, 0, NULL},
		{0,0,"AGCHOVSF",          E_PARAM_W_AGCHOVSF,           0,255,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_INT,0, 0, NULL},
		{0,0,"ETFCItable",        E_PARAM_W_ETFCITABLE,         0,1,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_INT,0, 0, NULL},
		{0,0,"FER",               E_PARAM_W_FER,                0,100,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_FLOAT,0, 0, NULL},	
		{0,0,"Tx Power",          E_PARAM_W_TX_POWER,          -120,30,0,ID_WCDMA_TXPOWER,E_PARAM_TYPE_INT,0, 0, NULL},
		{0,0,"Bler",              E_PARAM_W_BLER,               0,100,0,ID_WCDMA_BLER,E_PARAM_TYPE_FLOAT,0, 0, NULL},

		{0,0,"SIR",	        E_PARAM_W_SIR,          -32,30,0,ID_WCDMA_SIR,E_PARAM_TYPE_FLOAT,0, 0, NULL},           
		{0,0,"SIR Target",	E_PARAM_W_SIR_TARGET,   -32,30,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_FLOAT,0, 0, NULL},   
		{0,0,"DL Power up",	E_PARAM_W_DLPWRUP,      0,100,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_FLOAT,0, 0, NULL},   
		{0,0,"#UL pwr up",	E_PARAM_W_DLPWRUP_CND,  0,65535,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_INT,0, 0, NULL},   
		{0,0,"#UL pwr down",E_PARAM_W_DLPWRDOWN_CND,0,65535,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_INT,0, 0, NULL}, 
		{0,0,"DPC mode",	E_PARAM_W_DPC_MODE,     0,1,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_INT,0, 0, NULL},
		{0,0,"BS div. state",E_PARAM_W_BSDIVSTATE, 0,2,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_INT,0, 0, NULL},

		{0,0,"Initial Power",  E_PARAM_W_INIT_PWR,     -99,99,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_FLOAT,0, 0, NULL},
		{0,0,"Power Ramp Step",E_PARAM_W_PWR_RAMP_STEP,0,65535,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_FLOAT,0, 0, NULL},	
		{0,0,"Preamble count", E_PARAM_W_PRE_CNT,      0,65,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_INT,0, 0, NULL},
		{0,0,"RACH TX power",  E_PARAM_W_RACH_TX_PWR,  -99,99,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_FLOAT,0, 0, NULL},
		{0,0,"Max preamble",   E_PARAM_W_MAX_PREEMBLE, 0,64,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_INT,0, 0, NULL},
		{0,0,"UL interf.",     E_PARAM_W_UL_INTERF,    -110,-70,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_FLOAT,0, 0, NULL},
		{0,0,"AICH status",    E_PARAM_W_AICH_STATUS,  0,2,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_INT,0, 0, NULL},
		{0,0,"Data gain",      E_PARAM_W_DATA_GAIN,    0,15,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_INT,0, 0, NULL},
		{0,0,"Ctrl gain",      E_PARAM_W_CTRL_GAIN,    0,15,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_INT,0, 0, NULL},
		{0,0,"Power offset",   E_PARAM_W_PWR_OFFSET,   -5,10,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_FLOAT,0, 0, NULL},
		{0,0,"Message length", E_PARAM_W_MSG_LENGTH,   5,20,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_INT,0, 0, NULL},
		{0,0,"Preamble cycles",E_PARAM_W_PRE_CYCLES,   1,32,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_INT,0, 0, NULL},

		{0,0,"Channel(s)",E_PARAM_W_CHANNELS,0,1,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_W_CHANNEL,0, 0, NULL},
		{0,0,"Neighbor(s)",E_PARAM_W_CELLS,0,1,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_W_CELL,0, 0, NULL},
		{0,0,"Neighbor(s)",E_PARAM_W_COMPLANIT_CELLS,0,1,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_W_COMPLAINT_CELL,0, 0, NULL},
		{0,0,"Transport channel info",E_PARAM_W_TRCHI,0,1,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_W_TRCHI,0, 0, NULL}, //
		{0,0,"CQI",                   E_PARAM_W_CQI,  0,30,0,ID_PARAM_UNKNOW, E_PARAM_TYPE_W_CQI,0, 0, NULL},
		{0,0,"CQI1",                  E_PARAM_W_CQI_1,0,30,0,ID_WCDMA_CQI, E_PARAM_TYPE_INT,0, 0, NULL},
		//RSSI
		{0,0,"RSSI", E_PARAM_W_RSSI,RANGE_TD_RSSI_MIN,RANGE_TD_RSSI_MAX,RANGE_TD_RSSI_MIN,ID_WCDMA_RXPOWER,E_PARAM_TYPE_FLOAT,0, 0, NULL},
		{0,0,"RSSI1",E_PARAM_W_CH_1,RANGE_TD_RSSI_MIN,RANGE_TD_RSSI_MAX,RANGE_TD_RSSI_MIN,ID_PARAM_UNKNOW,E_PARAM_TYPE_FLOAT,0, 0, NULL},
		{0,0,"RSSI2",E_PARAM_W_CH_2,RANGE_TD_RSSI_MIN,RANGE_TD_RSSI_MAX,RANGE_TD_RSSI_MIN,ID_PARAM_UNKNOW,E_PARAM_TYPE_FLOAT,0, 0, NULL},
		{0,0,"RSSI3",E_PARAM_W_CH_3,RANGE_TD_RSSI_MIN,RANGE_TD_RSSI_MAX,RANGE_TD_RSSI_MIN,ID_PARAM_UNKNOW,E_PARAM_TYPE_FLOAT,0, 0, NULL},
		{0,0,"RSSI4",E_PARAM_W_CH_4,RANGE_TD_RSSI_MIN,RANGE_TD_RSSI_MAX,RANGE_TD_RSSI_MIN,ID_PARAM_UNKNOW,E_PARAM_TYPE_FLOAT,0, 0, NULL},
		{0,0,"RSSI5",E_PARAM_W_CH_5,RANGE_TD_RSSI_MIN,RANGE_TD_RSSI_MAX,RANGE_TD_RSSI_MIN,ID_PARAM_UNKNOW,E_PARAM_TYPE_FLOAT,0, 0, NULL},
		{0,0,"RSSI6",E_PARAM_W_CH_6,RANGE_TD_RSSI_MIN,RANGE_TD_RSSI_MAX,RANGE_TD_RSSI_MIN,ID_PARAM_UNKNOW,E_PARAM_TYPE_FLOAT,0, 0, NULL},

		//W : just for titile
		{0,0,"ChannelNum",E_PARAM_W_CHANNEL_CHANNEL_NUMBER,0,65535,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_JUST_FOR_TITLE,0, 0, NULL},
		{0,0,"RSSI",E_PARAM_W_CHANNEL_RSSI,RANGE_W_RSCP_MIN,RANGE_W_RSCP_MAX,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_JUST_FOR_TITLE,0, 0, NULL},

		{0,0,"Type",   E_PARAM_W_CELL_TYPE,0,3,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_JUST_FOR_TITLE,0, 0, NULL},
		{0,0,"Band",   E_PARAM_W_CELL_BAND,0,65535,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_JUST_FOR_TITLE,0, 0, NULL},
		{0,0,"UARFCN", E_PARAM_W_CELL_CH,0,65535,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_JUST_FOR_TITLE,0, 0, NULL},
		{0,0,"PSC",  E_PARAM_W_CELL_SCR,0,65535,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_JUST_FOR_TITLE,0, 0, NULL},
		{0,0,"Ec/No",E_PARAM_W_CELL_EC_NO,-25,0,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_JUST_FOR_TITLE,0, 0, NULL},
		{0,0,"STTD", E_PARAM_W_CELL_STTD,0,1,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_JUST_FOR_TITLE,0, 0, NULL},
		{0,0,"RSCP", E_PARAM_W_CELL_RSCP,RANGE_W_RSCP_MIN,RANGE_W_RSCP_MAX,RANGE_W_RSCP_MAX,ID_PARAM_UNKNOW,E_PARAM_TYPE_JUST_FOR_TITLE,0, 0, NULL},

		{0,0,"TrChID",E_PARAM_W_TRCHI_ID,0,32,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_JUST_FOR_TITLE,0, 0, NULL},
		{0,0,"TrchBler",E_PARAM_W_TRCHI_BLER,0,5,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_JUST_FOR_TITLE,0, 0, NULL},
		{0,0,"Blocks",E_PARAM_W_TRCHI_BLOCKS_CNT,0,2,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_JUST_FOR_TITLE,0, 0, NULL},
		{0,0,"Errors",E_PARAM_W_TRCHI_ERRORS_CNT,0,8,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_JUST_FOR_TITLE,0, 0, NULL},

		//TD-SCDMA
		{0,0,"UARFCN",E_PARAM_TD_SERV_UARFCN,0,65535,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_INT,0, 0, NULL},
		{0,0,"CellParamID",E_PARAM_TD_SERV_CELL_PARAMETER_ID,0,127,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_INT,0, 0, NULL},

		{0,0,"PCCPCH RSCP",E_PARAM_TD_PCCPCH_RSCP,RANGE_TD_RSCP_MIN,RANGE_TD_RSCP_MAX,RANGE_TD_RSCP_MIN,ID_PARAM_UNKNOW,E_PARAM_TYPE_INT,0, 0, NULL},
		{0,0,"RSCP1",E_PARAM_TD_PCCPCH_RSCP_N1,RANGE_TD_RSCP_MIN,RANGE_TD_RSCP_MAX,RANGE_TD_RSCP_MIN,ID_PARAM_UNKNOW,E_PARAM_TYPE_INT,0, 0, NULL},
		{0,0,"RSCP2",E_PARAM_TD_PCCPCH_RSCP_N2,RANGE_TD_RSCP_MIN,RANGE_TD_RSCP_MAX,RANGE_TD_RSCP_MIN,ID_PARAM_UNKNOW,E_PARAM_TYPE_INT,0, 0, NULL},
		{0,0,"RSCP3",E_PARAM_TD_PCCPCH_RSCP_N3,RANGE_TD_RSCP_MIN,RANGE_TD_RSCP_MAX,RANGE_TD_RSCP_MIN,ID_PARAM_UNKNOW,E_PARAM_TYPE_INT,0, 0, NULL},
		{0,0,"RSCP4",E_PARAM_TD_PCCPCH_RSCP_N4,RANGE_TD_RSCP_MIN,RANGE_TD_RSCP_MAX,RANGE_TD_RSCP_MIN,ID_PARAM_UNKNOW,E_PARAM_TYPE_INT,0, 0, NULL},
		{0,0,"RSCP5",E_PARAM_TD_PCCPCH_RSCP_N5,RANGE_TD_RSCP_MIN,RANGE_TD_RSCP_MAX,RANGE_TD_RSCP_MIN,ID_PARAM_UNKNOW,E_PARAM_TYPE_INT,0, 0, NULL},
		{0,0,"RSCP6",E_PARAM_TD_PCCPCH_RSCP_N6,RANGE_TD_RSCP_MIN,RANGE_TD_RSCP_MAX,RANGE_TD_RSCP_MIN,ID_PARAM_UNKNOW,E_PARAM_TYPE_INT,0, 0, NULL},

		{0,0,"DPCHRSCP",E_PARAM_TD_DPCH_RSCP,RANGE_TD_RSCP_MIN,RANGE_TD_RSCP_MAX,RANGE_TD_RSCP_MIN,ID_PARAM_UNKNOW,E_PARAM_TYPE_INT,0, 0, NULL},
		{0,0,"RSCP1",E_PARAM_TD_DPCH_RSCP_N1,RANGE_TD_RSCP_MIN,RANGE_TD_RSCP_MAX,RANGE_TD_RSCP_MIN,ID_PARAM_UNKNOW,E_PARAM_TYPE_INT,0, 0, NULL},
		{0,0,"RSCP2",E_PARAM_TD_DPCH_RSCP_N2,RANGE_TD_RSCP_MIN,RANGE_TD_RSCP_MAX,RANGE_TD_RSCP_MIN,ID_PARAM_UNKNOW,E_PARAM_TYPE_INT,0, 0, NULL},
		{0,0,"RSCP3",E_PARAM_TD_DPCH_RSCP_N3,RANGE_TD_RSCP_MIN,RANGE_TD_RSCP_MAX,RANGE_TD_RSCP_MIN,ID_PARAM_UNKNOW,E_PARAM_TYPE_INT,0, 0, NULL},
		{0,0,"RSCP4",E_PARAM_TD_DPCH_RSCP_N4,RANGE_TD_RSCP_MIN,RANGE_TD_RSCP_MAX,RANGE_TD_RSCP_MIN,ID_PARAM_UNKNOW,E_PARAM_TYPE_INT,0, 0, NULL},
		{0,0,"RSCP5",E_PARAM_TD_DPCH_RSCP_N5,RANGE_TD_RSCP_MIN,RANGE_TD_RSCP_MAX,RANGE_TD_RSCP_MIN,ID_PARAM_UNKNOW,E_PARAM_TYPE_INT,0, 0, NULL},
		{0,0,"RSCP6",E_PARAM_TD_DPCH_RSCP_N6,RANGE_TD_RSCP_MIN,RANGE_TD_RSCP_MAX,RANGE_TD_RSCP_MIN,ID_PARAM_UNKNOW,E_PARAM_TYPE_INT,0, 0, NULL},

		{0,0,"PCCPCHC/I",E_PARAM_TD_PCCPCH_CBI,-30,40,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_FLOAT,0, 0, NULL},
		{0,0,"CI",E_PARAM_TD_SERV_CI,0,65535,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_INT,0, 0, NULL},
		{0,0,"LAC",E_PARAM_TD_SERV_LAC,0,65535,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_INT,0, 0, NULL},
		{0,0,"RAC",E_PARAM_TD_SERV_RAC,0,255,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_INT,0, 0, NULL},
		{0,0,"MCC",E_PARAM_TD_SERV_MCC,0,999,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_INT,0, 0, NULL},
		{0,0,"MNC",E_PARAM_TD_SERV_MNC,0,99,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_INT,0, 0, NULL},
		{0,0,"NMO",E_PARAM_TD_SERV_NMO,0,10,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_INT,0, 0, NULL},
		{0,0,"TA",E_PARAM_TD_SERV_TA,-16,240,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_INT,0, 0, NULL},
		{0,0,"FER",E_PARAM_TD_FER,0,100,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_INT,0, 0, NULL},
		{0,0,"AttachAllowed",E_PARAM_TD_ATTACH_ALLLOWED,0,1,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_INT,0, 0, NULL},
		{0,0,"SIntraSearch",E_PARAM_TD_SERV_INTRA_SEARCH,-105,91,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_INT,0, 0, NULL},
		{0,0,"SInterSearch",E_PARAM_TD_SERV_INTER_SEARCH,-105,91,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_INT,0, 0, NULL},
		{0,0,"SSearchHCS",E_PARAM_TD_SERV_SEARCH_HCS,-105,91,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_INT,0, 0, NULL},
		{0,0,"SSearchHCSRAT",E_PARAM_TD_SERV_SEARCH_HCS_RAT,-105,91,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_INT,0, 0, NULL},
		{0,0,"SHCSSupported",E_PARAM_TD_SERV_HCS_SUPPORTED,0,1,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_INT,0, 0, NULL},
		{0,0,"SQhcs",E_PARAM_TD_SERV_Q_HCS,0,99,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_INT,0, 0, NULL},
		{0,0,"RATID",E_PARAM_TD_SERV_RAT,-105,91,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_INT,0, 0, NULL},
		{0,0,"CellName",E_PARAM_TD_SERV_CELL_NAME,1,0,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_JUST_FOR_TITLE,0, 0, NULL},
		{0,0,"MaxAllowedTXPower",E_PARAM_TD_UE_MAX_ALLOWED_UL_TX_POWER,-53,33,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_FLOAT,0, 0, NULL},
		{0,0,"CellBarred",E_PARAM_TD_SERV_CELL_BARRED,0,0,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_INT,0, 0, NULL},
		{0,0,"ServURAIdCount",E_PARAM_TD_SERV_URAID_COUNT,1,8,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_INT,0, 0, NULL},
		{0,0,"T3212",E_PARAM_TD_T3212,1,255,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_INT,0, 0, NULL},
		{0,0,"DRXcycle",E_PARAM_TD_SERV_DRX_CYCLE,0,9,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_INT,0, 0, NULL},
		{0,0,"Txpower",E_PARAM_TD_TX_POWER,-99,99,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_INT,0, 0, NULL},

		//RSSI
		{0,0,"RSSI",E_PARAM_TD_RSSI,RANGE_TD_RSSI_MIN,RANGE_TD_RSSI_MAX,RANGE_TD_RSSI_MIN,ID_PARAM_UNKNOW,E_PARAM_TYPE_INT,0, 0, NULL},
		{0,0,"RSSI1",E_PARAM_TD_CH_1,RANGE_TD_RSSI_MIN,RANGE_TD_RSSI_MAX,RANGE_TD_RSSI_MIN,ID_PARAM_UNKNOW,E_PARAM_TYPE_INT,0, 0, NULL},
		{0,0,"RSSI2",E_PARAM_TD_CH_2,RANGE_TD_RSSI_MIN,RANGE_TD_RSSI_MAX,RANGE_TD_RSSI_MIN,ID_PARAM_UNKNOW,E_PARAM_TYPE_INT,0, 0, NULL},
		{0,0,"RSSI3",E_PARAM_TD_CH_3,RANGE_TD_RSSI_MIN,RANGE_TD_RSSI_MAX,RANGE_TD_RSSI_MIN,ID_PARAM_UNKNOW,E_PARAM_TYPE_INT,0, 0, NULL},
		{0,0,"RSSI4",E_PARAM_TD_CH_4,RANGE_TD_RSSI_MIN,RANGE_TD_RSSI_MAX,RANGE_TD_RSSI_MIN,ID_PARAM_UNKNOW,E_PARAM_TYPE_INT,0, 0, NULL},
		{0,0,"RSSI5",E_PARAM_TD_CH_5,RANGE_TD_RSSI_MIN,RANGE_TD_RSSI_MAX,RANGE_TD_RSSI_MIN,ID_PARAM_UNKNOW,E_PARAM_TYPE_INT,0, 0, NULL},
		{0,0,"RSSI6",E_PARAM_TD_CH_6,RANGE_TD_RSSI_MIN,RANGE_TD_RSSI_MAX,RANGE_TD_RSSI_MIN,ID_PARAM_UNKNOW,E_PARAM_TYPE_INT,0, 0, NULL},

		{0,0,"BLER",E_PARAM_TD_BLER,0,100,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_FLOAT,0, 0, NULL},
		{0,0,"SIRtarget",E_PARAM_TD_SIR_TARGET,-32,30,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_INT,0, 0, NULL},
		{0,0,"SIR",E_PARAM_TD_SIR,-32,30,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_INT,0, 0, NULL},
		{0,0,"PCCPCHPathloss",E_PARAM_TD_PCCPCH_PATHLOSS,46,148,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_FLOAT,0, 0, NULL},
		{0,0,"SRxLev",E_PARAM_TD_S_RX_LEV,-107,90,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_FLOAT,0, 0, NULL},
		{0,0,"RRxLev",E_PARAM_TD_R_RX_LEV,-191,90,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_FLOAT,0, 0, NULL},
		{0,0,"HRxLev",E_PARAM_TD_H_RX_LEV,-115,90,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_FLOAT,0, 0, NULL},
		{0,0,"Pwrctrlstep",E_PARAM_TD_TCP_STEP_SIZE,1,3,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_FLOAT,0, 0, NULL},
		{0,0,"TA(Timingadvance)",E_PARAM_TD_TA,-16,240,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_FLOAT,0, 0, NULL},

		//channel(s)&cell(s)
		{0,0,"Channel(s)",E_PARAM_TD_CHANNELS,0,1,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_TD_CHANNEL,0, 0, NULL},
		{0,0,"Neighbor(s)",E_PARAM_TD_CELLS,0,1,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_TD_CELL,0, 0, NULL},
		{0,0,"Timeslot(s)",E_PARAM_TD_TIMESLOT,0,1,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_TD_TIMESLOT,0, 0, NULL},
		{0,0,"Transportchannelinfo",E_PARAM_TD_TRCHI,0,1,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_TD_TRCHI,0, 0, NULL},

		//TD : just for titile
		{0,0,"ChannelNum",E_PARAM_TD_CHANNEL_CHANNEL_NUMBER,0,65535,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_JUST_FOR_TITLE,0, 0, NULL},
		{0,0,"RSSI",E_PARAM_TD_CHANNEL_RSSI,RANGE_TD_RSCP_MIN,RANGE_TD_RSCP_MAX,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_JUST_FOR_TITLE,0, 0, NULL},
		{0,0,"Band",E_PARAM_TD_CHANNEL_BAND,0,65535,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_JUST_FOR_TITLE,0, 0, NULL},

		{0,0,"Type",E_PARAM_TD_CELL_TYPE,0,1,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_JUST_FOR_TITLE,0, 0, NULL},
		{0,0,"UARFCN",E_PARAM_TD_CELL_CHANNEL_NUMBER,0,65535,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_JUST_FOR_TITLE,0, 0, NULL},
		{0,0,"PID",E_PARAM_TD_CELL_PARAMETERS_ID,0,65535,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_JUST_FOR_TITLE,0, 0, NULL},
		{0,0,"RSCP",E_PARAM_TD_CELL_RSCP,RANGE_TD_RSCP_MIN,RANGE_TD_RSCP_MAX,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_JUST_FOR_TITLE,0, 0, NULL},
		{0,0,"Rrxlev",E_PARAM_TD_CELL_RRXLEV,-191,25,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_JUST_FOR_TITLE,0, 0, NULL},
		{0,0,"SrxLev",E_PARAM_TD_CELL_SRXLEV,-107,90,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_JUST_FOR_TITLE,0, 0, NULL},
		{0,0,"Band",E_PARAM_TD_CELL_BAND,0,65535,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_JUST_FOR_TITLE,0, 0, NULL},

		{0,0,"TSL",E_PARAM_TD_TS_TSL,0,65535,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_JUST_FOR_TITLE,0, 0, NULL},
		{0,0,"ISCP",E_PARAM_TD_TS_ISCP,RANGE_TD_ISCP_MIN,RANGE_TD_ISCP_MAX,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_JUST_FOR_TITLE,0, 0, NULL},
		{0,0,"RSCP",E_PARAM_TD_TS_RSCP,RANGE_TD_RSCP_MIN,RANGE_TD_RSCP_MAX,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_JUST_FOR_TITLE,0, 0, NULL},

		{0,0,"TrChID",E_PARAM_TD_TRCHI_ID,0,32,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_JUST_FOR_TITLE,0, 0, NULL},
		{0,0,"TrchBler",E_PARAM_TD_TRCHI_BLER,0,5,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_JUST_FOR_TITLE,0, 0, NULL},
		{0,0,"Blocks",E_PARAM_TD_TRCHI_BLOCKS_CNT,0,2,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_JUST_FOR_TITLE,0, 0, NULL},
		{0,0,"Errors",E_PARAM_TD_TRCHI_ERRORS_CNT,0,8,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_JUST_FOR_TITLE,0, 0, NULL},

		//cdma [server]
		{0,0,"Band",E_PARAM_CDMA_SERV_BAND,0,0x7fffffff,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_INT,0, 0, NULL},
		{0,0,"Ch Number",E_PARAM_CDMA_SERV_CHANNEL_NUMBER,0,0x7fffffff,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_INT,0, 0, NULL},
		{0,0,"Pilot Number",E_PARAM_CDMA_SERV_PILOT_NUMVER,0,0x7fffffff,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_INT,0, 0, NULL},
		{0,0,"Walsh",E_PARAM_CDMA_SERV_WALSH,0,0x7fffffff,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_INT,0, 0, NULL},
		{0,0,"RSCP",E_PARAM_CDMA_SERV_RSCP,-150,-20,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_FLOAT,0, 0, NULL},
		{0,0,"Ec/Io",E_PARAM_CDMA_SERV_ECIO,-35,-3,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_FLOAT,0, 0, NULL},

		{0,0,"Channels",E_PARAM_CDMA_CHANNELS,0,0,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_CDMA_CHANNEL,0, 0, NULL},
		{0,0,"Cells",E_PARAM_CDMA_CELLS,0,0,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_CDMA_CELL,0, 0, NULL},

		{0,0,"Rxpower",E_PARAM_CDMA_RXPOWR,   -120, 30,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_FLOAT,0, 0, NULL},
		{0,0,"Rxpower1",E_PARAM_CDMA_RXPOWR_1,-120, 30,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_FLOAT,0, 0, NULL},
		{0,0,"Rxpower2",E_PARAM_CDMA_RXPOWR_2,-120, 30,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_FLOAT,0, 0, NULL},
		{0,0,"Rxpower3",E_PARAM_CDMA_RXPOWR_3,-120, 30,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_FLOAT,0, 0, NULL},
		{0,0,"Rxpower4",E_PARAM_CDMA_RXPOWR_4,-120, 30,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_FLOAT,0, 0, NULL},
		{0,0,"Rxpower5",E_PARAM_CDMA_RXPOWR_5,-120, 30,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_FLOAT,0, 0, NULL},
		{0,0,"Rxpower6",E_PARAM_CDMA_RXPOWR_6,-120, 30,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_FLOAT,0, 0, NULL},

		{0,0,"RSCP",E_PARAM_CDMA_RSCP,-150,-20,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_FLOAT,0, 0, NULL},
		{0,0,"RSCP1",E_PARAM_CDMA_RSCP,-150,-20,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_FLOAT,0, 0, NULL},
		{0,0,"RSCP2",E_PARAM_CDMA_RSCP,-150,-20,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_FLOAT,0, 0, NULL},
		{0,0,"RSCP3",E_PARAM_CDMA_RSCP,-150,-20,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_FLOAT,0, 0, NULL},
		{0,0,"RSCP4",E_PARAM_CDMA_RSCP,-150,-20,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_FLOAT,0, 0, NULL},
		{0,0,"RSCP5",E_PARAM_CDMA_RSCP,-150,-20,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_FLOAT,0, 0, NULL},
		{0,0,"RSCP6",E_PARAM_CDMA_RSCP,-150,-20,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_FLOAT,0, 0, NULL},

		//CDMA Title
		{0,0,"Band",E_PARAM_CDMA_CHANNEL_BAND,0,0x7fffffff,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_JUST_FOR_TITLE,0, 0, NULL},
		{0,0,"Channel Number",E_PARAM_CDMA_CHANNEL_NUMBER,0,0x7fffffff,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_JUST_FOR_TITLE,0, 0, NULL},
		{0,0,"Rxpower",E_PARAM_CDMA_CHANNEL_RXPOWER,-120, 30,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_JUST_FOR_TITLE,0, 0, NULL},
		{0,0,"Rx0 Power",E_PARAM_CDMA_CHANNEL_RX0POWER,-120, 30,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_JUST_FOR_TITLE,0, 0, NULL},
		{0,0,"Rx1 Power",E_PARAM_CDMA_CHANNEL_RX1POWER,-120, 30,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_JUST_FOR_TITLE,0, 0, NULL},

		{0,0,"Type",E_PARAM_CDMA_CELL_TYPE,0,0x7fffffff,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_JUST_FOR_TITLE,0, 0, NULL},
		{0,0,"Band",E_PARAM_CDMA_CELL_BAND,0,0x7fffffff,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_JUST_FOR_TITLE,0, 0, NULL},
		{0,0,"Channel Number",E_PARAM_CDMA_CELL_CHANNEL_NUMBER,0,0x7fffffff,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_JUST_FOR_TITLE,0, 0, NULL},
		{0,0,"Pilot Number",E_PARAM_CDMA_CELL_PILOT_NUMVER,0,0x7fffffff,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_JUST_FOR_TITLE,0, 0, NULL},
		{0,0,"Walsh",E_PARAM_CDMA_CELL_WALSH,0,0x7fffffff,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_JUST_FOR_TITLE,0, 0, NULL},
		{0,0,"Rscp",E_PARAM_CDMA_CELL_RSCP,-150,-20,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_JUST_FOR_TITLE,0, 0, NULL},
		{0,0,"EcIo",E_PARAM_CDMA_CELL_ECIO,-35,-3,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_JUST_FOR_TITLE,0, 0, NULL},

		//-------------------------------------------------------------------------
		{0,0,"FER",	   E_PARAM_CDMA_FER_TOTAL,0,100,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_FLOAT,0, 0, NULL},
		{0,0,"FER_FCH",E_PARAM_CDMA_FER_FCH,0,100,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_FLOAT,0, 0, NULL},
		{0,0,"FER_SCH",E_PARAM_CDMA_FER_SCH,0,100,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_FLOAT,0, 0, NULL},

		{0,0,"RLP reverse user throughput",E_PARAM_CDMA_RLPRATE_RLP_REV_RATE,0,0x7ffffff,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_INT,0, 0, NULL},
		{0,0,"RLP forward user throughput",E_PARAM_CDMA_RLPRATE_RLP_FOR_RATE,0,0x7ffffff,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_INT,0, 0, NULL},
		{0,0,"RLP reverse retransmission rate",E_PARAM_CDMA_RLPRATE_RLP_REV_RETR_RATE,0,100,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_FLOAT,0, 0, NULL},
		{0,0,"RLP forward retransmission rate",E_PARAM_CDMA_RLPRATE_RLP_FWD_RETR_RATE,0,100,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_FLOAT,0, 0, NULL},

		//TXPC
		{0,0,"TX power",E_PARAM_CDMA_TXPC_TXPOWER,-99,99,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_FLOAT,0, 0, NULL},
		{0,0,"TX adjust",E_PARAM_CDMA_TXPC_TXADJUST,0xffffffff,0x7fffffff,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_FLOAT,0, 0, NULL},
		{0,0,"TX pwr limit",E_PARAM_CDMA_TXPC_TXPWRLIMIT,0xffffffff,0x7fffffff,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_FLOAT,0, 0, NULL},
		{0,0,"R-FCH/R-PICH",E_PARAM_CDMA_TXPC_R_FCH,0xffffffff,0x7fffffff,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_FLOAT,0, 0, NULL},
		{0,0,"R-SCH0/R-PICH",E_PARAM_CDMA_TXPC_R_SCH0,0xffffffff,0x7fffffff,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_FLOAT,0, 0, NULL},
		{0,0,"R-SCH1/R-PICH",E_PARAM_CDMA_TXPC_R_SCH1,0xffffffff,0x7fffffff,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_FLOAT,0, 0, NULL},
		{0,0,"R-DCCH/R-PICH",E_PARAM_CDMA_TXPC_R_DCCH,0xffffffff,0x7fffffff,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_FLOAT,0, 0, NULL},

		//RACHI
		{0,0,"NOM_PWR",E_PARAM_CDMA_RACHI_NOM_PWR,0xffffffff,0x7fffffff,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_INT,0, 0, NULL},
		{0,0,"INIT_PWR",E_PARAM_CDMA_RACHI_INIT_PWR,0xffffffff,0x7fffffff,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_INT,0, 0, NULL},
		{0,0,"PWR_STEP",E_PARAM_CDMA_RACHI_PWR_STEP,0xffffffff,0x7fffffff,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_INT,0, 0, NULL},
		{0,0,"NUM_STEP",E_PARAM_CDMA_RACHI_NUM_STEP,0xffffffff,0x7fffffff,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_INT,0, 0, NULL},
		{0,0,"Probecount",E_PARAM_CDMA_RACHI_PROBECOUNT,0xffffffff,0x7fffffff,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_INT,0, 0, NULL},
		{0,0,"Probe seq. count",E_PARAM_CDMA_RACHI_PROBESEQCOUNT,0xffffffff,0x7fffffff,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_INT,0, 0, NULL},
		{0,0,"Access ch number",E_PARAM_CDMA_RACHI_ACCESSCHNUMBER,0xffffffff,0x7fffffff,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_INT,0, 0, NULL},
		{0,0,"Random delay",E_PARAM_CDMA_RACHI_RANDOMDELAY,0xffffffff,0x7fffffff,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_INT,0, 0, NULL},

		{0,0,"TX level",E_PARAM_CDMA_RACHI_TXLEVEL,0xffffffff,0x7fffffff,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_FLOAT,0, 0, NULL},
		{0,0,"Access RXlevel",E_PARAM_CDMA_RACHI_ACCESSRXLEVEL,0xffffffff,0x7fffffff,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_FLOAT,0, 0, NULL},
		{0,0,"Access TX adj",E_PARAM_CDMA_RACHI_ACCESSTXADJ,0xffffffff,0x7fffffff,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_FLOAT,0, 0, NULL},

		{0,0,"Psist",E_PARAM_CDMA_RACHI_PSIST,0,255,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_INT,0, 0, NULL},
		{0,0,"Seqbackoff",E_PARAM_CDMA_RACHI_SEQBACKOFF,0,255,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_INT,0, 0, NULL},
		{0,0,"Probbackoff",E_PARAM_CDMA_RACHI_PROBBACKOFF,0,255,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_INT,0, 0, NULL},
		{0,0,"Intercorr",E_PARAM_CDMA_RACHI_INTERCORR,0,255,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_INT,0, 0, NULL},

		//EVDO [SERVER]---------------------------------------------------------------------<<
		{0,0,"Band",E_PARAM_EVDO_SERV_BAND,0,0x7fffffff,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_INT,0, 0, NULL},
		{0,0,"Ch Number",E_PARAM_EVDO_SERV_CHANNEL_NUMBER,0,0x7fffffff,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_INT,0, 0, NULL},
		{0,0,"Pilot Number",E_PARAM_EVDO_SERV_PILOT_NUMVER,0,0x7fffffff,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_INT,0, 0, NULL},
		{0,0,"RSCP",E_PARAM_EVDO_SERV_RSCP,-150,-20,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_FLOAT,0, 0, NULL},
		{0,0,"Ec/Io",E_PARAM_EVDO_SERV_ECIO,-35,-3,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_FLOAT,0, 0, NULL},

		{0,0,"Channels",E_PARAM_EVDO_CHANNELS,0,0,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_EVDO_CHANNEL,0, 0, NULL},
		{0,0,"Cells",E_PARAM_EVDO_CELLS,0,0,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_EVDO_CELL,0, 0, NULL},

		{0,0,"Rxpower", E_PARAM_EVDO_RXPOWR,  -120, 30,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_FLOAT,0, 0, NULL},
		{0,0,"Rxpower1",E_PARAM_EVDO_RXPOWR_1,-120, 30,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_FLOAT,0, 0, NULL},
		{0,0,"Rxpower2",E_PARAM_EVDO_RXPOWR_2,-120, 30,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_FLOAT,0, 0, NULL},
		{0,0,"Rxpower3",E_PARAM_EVDO_RXPOWR_3,-120, 30,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_FLOAT,0, 0, NULL},
		{0,0,"Rxpower4",E_PARAM_EVDO_RXPOWR_4,-120, 30,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_FLOAT,0, 0, NULL},
		{0,0,"Rxpower5",E_PARAM_EVDO_RXPOWR_5,-120, 30,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_FLOAT,0, 0, NULL},
		{0,0,"Rxpower6",E_PARAM_EVDO_RXPOWR_6,-120, 30,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_FLOAT,0, 0, NULL},

		{0,0,"RSCP", E_PARAM_EVDO_RSCP,-150,-20,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_FLOAT,0, 0, NULL},
		{0,0,"RSCP1",E_PARAM_EVDO_RSCP,-150,-20,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_FLOAT,0, 0, NULL},
		{0,0,"RSCP2",E_PARAM_EVDO_RSCP,-150,-20,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_FLOAT,0, 0, NULL},
		{0,0,"RSCP3",E_PARAM_EVDO_RSCP,-150,-20,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_FLOAT,0, 0, NULL},
		{0,0,"RSCP4",E_PARAM_EVDO_RSCP,-150,-20,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_FLOAT,0, 0, NULL},
		{0,0,"RSCP5",E_PARAM_EVDO_RSCP,-150,-20,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_FLOAT,0, 0, NULL},
		{0,0,"RSCP6",E_PARAM_EVDO_RSCP,-150,-20,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_FLOAT,0, 0, NULL},

		//EVDO Title
		{0,0,"Band",E_PARAM_EVDO_CHANNEL_BAND,0,0x7fffffff,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_JUST_FOR_TITLE,0, 0, NULL},
		{0,0,"Channel Number",E_PARAM_EVDO_CHANNEL_NUMBER,0,0x7fffffff,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_JUST_FOR_TITLE,0, 0, NULL},
		{0,0,"Rxpower",E_PARAM_EVDO_CHANNEL_RXPOWER,-120, 30,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_JUST_FOR_TITLE,0, 0, NULL},
		{0,0,"Rx0 Power",E_PARAM_EVDO_CHANNEL_RX0POWER,-120, 30,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_JUST_FOR_TITLE,0, 0, NULL},
		{0,0,"Rx1 Power",E_PARAM_EVDO_CHANNEL_RX1POWER,-120, 30,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_JUST_FOR_TITLE,0, 0, NULL},

		{0,0,"Type",E_PARAM_EVDO_CELL_TYPE,0,0x7fffffff,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_JUST_FOR_TITLE,0, 0, NULL},
		{0,0,"Band",E_PARAM_EVDO_CELL_BAND,0,0x7fffffff,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_JUST_FOR_TITLE,0, 0, NULL},
		{0,0,"Channel Number",E_PARAM_EVDO_CELL_CHANNEL_NUMBER,0,0x7fffffff,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_JUST_FOR_TITLE,0, 0, NULL},
		{0,0,"Pilot Number",E_PARAM_EVDO_CELL_PILOT_NUMVER,0,0x7fffffff,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_JUST_FOR_TITLE,0, 0, NULL},
		{0,0,"Rscp",E_PARAM_EVDO_CELL_RSCP,-150,-20,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_JUST_FOR_TITLE,0, 0, NULL},
		{0,0,"EcIo",E_PARAM_EVDO_CELL_ECIO,-35,-3,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_JUST_FOR_TITLE,0, 0, NULL},

		//-------------------------------------------------------------------------------------------------------
		{0,0,"RLP reverse user throughput",E_PARAM_EVDO_RLPRATE_RLP_REV_RATE,0,0x7ffffff,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_INT,0, 0, NULL},
		{0,0,"RLP forward user throughput",E_PARAM_EVDO_RLPRATE_RLP_FOR_RATE,0,0x7ffffff,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_INT,0, 0, NULL},
		{0,0,"RLP reverse retransmission rate",E_PARAM_EVDO_RLPRATE_RLP_REV_RETR_RATE,0,100,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_FLOAT,0, 0, NULL},
		{0,0,"RLP forward retransmission rate",E_PARAM_EVDO_RLPRATE_RLP_FWD_RETR_RATE,0,100,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_FLOAT,0, 0, NULL},

		//TXPC
		{0,0,"TX power",E_PARAM_EVDO_TXPC_TX_POWER,-99, 99,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_FLOAT,0, 0, NULL},
		{0,0,"TX adjust",E_PARAM_EVDO_TXPC_TX_ADJUST,0xffffffff, 0x7fffffff,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_FLOAT,0, 0, NULL},
		{0,0,"TX Pilot",E_PARAM_EVDO_TXPC_TX_PILOT,-99, 99,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_FLOAT,0, 0, NULL},
		{0,0,"TX open loop power",E_PARAM_EVDO_TXPC_TX_OPEN_LOOP_POWER,-99,99,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_FLOAT,0, 0, NULL},
		{0,0,"TX max power usage",E_PARAM_EVDO_TXPC_TX_MAX_POWER_USAGE,0,100,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_FLOAT,0, 0, NULL},
		{0,0,"TX min power usage",E_PARAM_EVDO_TXPC_TX_MIN_POWER_USAGE,0,100,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_FLOAT,0, 0, NULL},

		//RACHI
		{0,0,"MaxProbeseqs",E_PARAM_EVDO_RACHI_MaxProbeseqs,0,0x7fffffff,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_INT,0, 0, NULL},
		{0,0,"Duration",E_PARAM_EVDO_RACHI_Duration,0,0x7fffffff,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_INT,0, 0, NULL},
		{0,0,"AccessPN",E_PARAM_EVDO_RACHI_AccessPN,0,0x7fffffff,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_INT,0, 0, NULL},
		{0,0,"Accesssectorid",E_PARAM_EVDO_RACHI_Accesssectorid,0,16777215,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_INT,0, 0, NULL},
		{0,0,"MaxProbes",E_PARAM_EVDO_RACHI_MaxProbes,0,0x7fffffff,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_INT,0, 0, NULL},
		{0,0,"Result",E_PARAM_EVDO_RACHI_Result,0,0x7fffffff,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_INT,0, 0, NULL},
		{0,0,"nProbes",E_PARAM_EVDO_RACHI_nProbes,0,0x7fffffff,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_INT,0, 0, NULL},
		{0,0,"nProbeseqs",E_PARAM_EVDO_RACHI_nProbeseqs,0,0x7fffffff,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_INT,0, 0, NULL},
		{0,0,"Accesscolorcode",E_PARAM_EVDO_RACHI_Accesscolorcode,0,255,0,ID_PARAM_UNKNOW,E_PARAM_TYPE_INT,0, 0, NULL},
	};
	m_pParamList = new T_PARAM_LIST[E_PARAM_MAX];
	memset(m_pParamList, 0, sizeof(T_PARAM_LIST)*E_PARAM_MAX);
	for (int i=0; i<E_PARAM_MAX; i++)
	{
		m_pParamList[i] = gParamList[i];
	}
}

CEventDecode::~CEventDecode()
{
	ReleaseParamList();
	delete []m_pParamList;
	m_pParamList = NULL;
}

void CEventDecode::ClearNode_Matrix( unsigned int nParamID, unsigned int nIndex )
{
	if( nParamID >= E_PARAM_MAX ) return;

	T_PARAM_MATRIX *p = (T_PARAM_MATRIX *)m_pParamList[nParamID].pData;
	p[nIndex].iCurTimeIndex = 0;
	p[nIndex].nCnt = 0;
	if( NULL != p[nIndex].pList ) delete[] p[nIndex].pList;
	p[nIndex].pList = NULL;
}

bool CEventDecode::IsLoadedLog(const char *pSrcFileName)
{
	if (m_gLogInfo.bLoad)
	{
		return false;
	}
	return false;
}

T_LOG_INFO* CEventDecode::GetCurLogInfo()
{
	return &m_gLogInfo;
}

T_LOG_INDOOR_INFO *CEventDecode::GetIndoorInfo()
{
	return &m_gLogInfo.indoor;
}

BOOL CEventDecode::IsIndoorLog()
{
	return m_gLogInfo.bIsIndoor;
}

int CEventDecode::LoadLogFile(const char *pSrcFileName)
{
	m_chNetFlag = 0;
	m_gLogInfo.iLockNetType = -1;
	m_gLogInfo.iLogWorkType = -1;
	m_CurSystem             = 0;

	CloseLogFile();
	//TRACE("1>> loading : %d \n", GetTickCount());
	int iResult = 0; 
	iResult = GetLogInfo(pSrcFileName);
	//TRACE("4>> get log info : %d \n", GetTickCount());
	if( E_RESULT_OK != iResult )
	{
		return iResult;
	}
	iResult = CreateParamList();
	//TRACE("5>> create buffer : %d \n", GetTickCount());
	if( E_RESULT_OK != iResult )
	{
		return iResult;
	}
	FILE *pFile = fopen(pSrcFileName,"rb");
	if (NULL == pFile)
	{
		return E_RESULT_FILE_CAN_NOT_OPEN;
	}
	iResult = PushParamList(pFile);
	//TRACE("6>> push : %d \n", GetTickCount());
	if( E_RESULT_OK != iResult )
	{
		fclose(pFile);
		pFile = NULL;
		return iResult;
	}
	m_MsgMatrix.iIndex    = 0;
	m_gEventMatrix.iIndex = 0;
	//iResult = IndexParamList(&m_gLogInfo);
	//TRACE("7>> index : %d \n", GetTickCount());
	if( E_RESULT_OK != iResult )
	{
		fclose(pFile);
		pFile = NULL;
		return iResult;
	}
	fclose(pFile);
	pFile = NULL;

	sprintf(m_gLogInfo.szLog_Filename, "%s", pSrcFileName);
	GetShortFilename(m_gLogInfo.szLog_Title, pSrcFileName);
	m_gLogInfo.bLoad         = true;
	m_gLogInfo.nCurTimePoint = 0;
	if ((m_gLogInfo.iProvinceID == 0)||(m_gLogInfo.iCityID == 0) || (m_gLogInfo.iAreaID == 0))
	{
		LON_LAT stPt;
		stPt.dLat = m_gLogInfo.fLat;
		stPt.dLon = m_gLogInfo.fLon;
		string strValue;
		if ( pGisChecker && (stPt.dLat>0.1) && (stPt.dLon>0.1))
		{
			pGisChecker->Check(stPt, m_gLogInfo.iProvinceID, m_gLogInfo.iAreaID, strValue);
		}
	}
	string strMD5Value;
	unsigned int iFileSize = 0;
	if (GetFileMD5(pSrcFileName, strMD5Value, iFileSize))
	{
		strcpy(m_gLogInfo.szLogMD5, strMD5Value.c_str());
		m_gLogInfo.iLogSize = iFileSize;
	}
	strcpy(m_gLogInfo.szLogName, GetFileName(pSrcFileName));
	if ((m_gLogInfo.fLat>0.1)&&(m_gLogInfo.fLon > 0.1))
	{
		double fLat = m_gLogInfo.fLat;
		double fLon = m_gLogInfo.fLon;
		if (g_psTranslate->TransLonLat(fLat, fLon))
		{
			m_gLogInfo.fLat = fLat;
			m_gLogInfo.fLon = fLon;
		}
	}
	return E_RESULT_OK;
}

void CEventDecode::CloseLogFile()
{
	if( !m_gLogInfo.bLoad ) return;
	ReleaseParamList();
}

int CEventDecode::GetCurrentSystem(unsigned int nTimeIndex)
{	
	return  GetParamValue_int(E_PARAM_SYSTEM,  nTimeIndex);
}

unsigned int CEventDecode::GetAppointTimeValue(unsigned int nTimeIndex)
{
	if( false == m_gLogInfo.bLoad ) return 0;
	T_TIME_LINE *p = (T_TIME_LINE *)m_pParamList[E_PARAM_TIME_LINE].pData;
	if( 0 == nTimeIndex) return m_gLogInfo.nStartTime;
	else if( nTimeIndex >= m_gLogInfo.nTimePointCnt ) return m_gLogInfo.nEndTime;
	return p[nTimeIndex].nTime;
}

void CEventDecode::GetGPS(unsigned int nTimeIndex, float &fLon, float &fLat)
{
	fLon = GetParamValue_float(E_PARAM_GPS_LON, nTimeIndex);
	fLat = GetParamValue_float(E_PARAM_GPS_LAT, nTimeIndex);
}

T_PARAM_DATA_INT* CEventDecode::GetParmList_int(unsigned int nParamID)
{
	if (m_pParamList[nParamID].nDataType != E_PARAM_TYPE_INT)
	{
		return NULL;
	}
	return (T_PARAM_DATA_INT*)m_pParamList[nParamID].pData;
}

T_PARAM_DATA_FLOAT* CEventDecode::GetParmList_float(unsigned int nParamID)
{
	if (m_pParamList[nParamID].nDataType != E_PARAM_TYPE_FLOAT)
	{
		return NULL;
	}
	return (T_PARAM_DATA_FLOAT*)m_pParamList[nParamID].pData;
}

T_PARAM_LIST* CEventDecode::GetParamList()
{
	return m_pParamList;
}

T_PARAM_LIST* CEventDecode::GetParamList(int nParamId, bool bIsParamIDentifier)
{
	if (bIsParamIDentifier)
	{
		for (int i=0; i<E_PARAM_MAX; i++)
		{
			if (m_pParamList[i].ParamIdentifier == nParamId)
			{
				return (&m_pParamList[i]);
			}
		}
	}else
	{
		if ((nParamId>E_PARAM_MAX) || (nParamId<E_PARAM_TIME_LINE))
		{
			return NULL;
		}
		return (&m_pParamList[nParamId]);
	}
	return NULL;
}

int CEventDecode::GetParamCount( unsigned int nParamID)
{
	if ((m_pParamList[nParamID].nDataType == E_PARAM_TYPE_INT)||(m_pParamList[nParamID].nDataType == E_PARAM_TYPE_FLOAT))
	{
		return m_pParamList[nParamID].nTail;
	}
	return -1;
}

int CEventDecode::GetMatrixCount(unsigned int nParamID)
{
	if( E_PARAM_TYPE_GSM_CELL   == m_pParamList[nParamID].nDataType ||
		E_PARAM_TYPE_TD_CHANNEL == m_pParamList[nParamID].nDataType ||
		E_PARAM_TYPE_TD_CELL    == m_pParamList[nParamID].nDataType ||
		E_PARAM_TYPE_TD_TIMESLOT== m_pParamList[nParamID].nDataType ||
		E_PARAM_TYPE_TD_TRCHI   == m_pParamList[nParamID].nDataType||
		E_PARAM_TYPE_W_TRCHI   == m_pParamList[nParamID].nDataType||
		E_PARAM_TYPE_W_CHANNEL == m_pParamList[nParamID].nDataType||
		E_PARAM_TYPE_W_CELL    == m_pParamList[nParamID].nDataType||
		E_PARAM_TYPE_CDMA_CHANNEL == m_pParamList[nParamID].nDataType||
		E_PARAM_TYPE_CDMA_CELL == m_pParamList[nParamID].nDataType   ||
		E_PARAM_TYPE_EVDO_CHANNEL == m_pParamList[nParamID].nDataType||
		E_PARAM_TYPE_EVDO_CELL == m_pParamList[nParamID].nDataType||
		E_PARAM_TYPE_W_CQI     == m_pParamList[nParamID].nDataType||
		E_PARAM_TYPE_GSM_COMPLAINT_CELL == m_pParamList[nParamID].nDataType||
		E_PARAM_TYPE_W_COMPLAINT_CELL == m_pParamList[nParamID].nDataType
		)
	{
		return m_pParamList[nParamID].nTail;
	}
	return -1;
}

T_PARAM_MATRIX *CEventDecode::GetParamMatrix( unsigned int nParamID)
{
	 T_PARAM_MATRIX *pMatrixList = NULL;
	if( E_PARAM_TYPE_GSM_CELL   == m_pParamList[nParamID].nDataType ||
		E_PARAM_TYPE_TD_CHANNEL == m_pParamList[nParamID].nDataType ||
		E_PARAM_TYPE_TD_CELL    == m_pParamList[nParamID].nDataType ||
		E_PARAM_TYPE_TD_TIMESLOT== m_pParamList[nParamID].nDataType ||
		E_PARAM_TYPE_TD_TRCHI   == m_pParamList[nParamID].nDataType||
		E_PARAM_TYPE_W_TRCHI   == m_pParamList[nParamID].nDataType||
		E_PARAM_TYPE_W_CHANNEL == m_pParamList[nParamID].nDataType||
		E_PARAM_TYPE_W_CELL    == m_pParamList[nParamID].nDataType||
		E_PARAM_TYPE_CDMA_CHANNEL == m_pParamList[nParamID].nDataType||
		E_PARAM_TYPE_CDMA_CELL == m_pParamList[nParamID].nDataType   ||
		E_PARAM_TYPE_EVDO_CHANNEL == m_pParamList[nParamID].nDataType||
		E_PARAM_TYPE_EVDO_CELL == m_pParamList[nParamID].nDataType||
		E_PARAM_TYPE_W_CQI     == m_pParamList[nParamID].nDataType||
		E_PARAM_TYPE_GSM_COMPLAINT_CELL == m_pParamList[nParamID].nDataType||
		E_PARAM_TYPE_W_COMPLAINT_CELL   == m_pParamList[nParamID].nDataType
		)
	{
	   pMatrixList = (T_PARAM_MATRIX*)m_pParamList[nParamID].pData;
	}
	return pMatrixList;
}

T_PARAM_MATRIX *CEventDecode::GetParamMatrix( unsigned int nParamID, int iTimePoint)
{
	if( false == m_gLogInfo.bLoad ) return NULL;

	T_PARAM_MATRIX *pMatrix = NULL;
	if( E_PARAM_TYPE_GSM_CELL   == m_pParamList[nParamID].nDataType ||
		E_PARAM_TYPE_TD_CHANNEL == m_pParamList[nParamID].nDataType ||
		E_PARAM_TYPE_TD_CELL    == m_pParamList[nParamID].nDataType ||
		E_PARAM_TYPE_TD_TIMESLOT== m_pParamList[nParamID].nDataType ||
		E_PARAM_TYPE_TD_TRCHI   == m_pParamList[nParamID].nDataType||
		E_PARAM_TYPE_W_TRCHI   == m_pParamList[nParamID].nDataType||
		E_PARAM_TYPE_W_CHANNEL == m_pParamList[nParamID].nDataType||
		E_PARAM_TYPE_W_CELL    == m_pParamList[nParamID].nDataType||
		E_PARAM_TYPE_CDMA_CHANNEL == m_pParamList[nParamID].nDataType||
		E_PARAM_TYPE_CDMA_CELL == m_pParamList[nParamID].nDataType   ||
		E_PARAM_TYPE_EVDO_CHANNEL == m_pParamList[nParamID].nDataType||
		E_PARAM_TYPE_EVDO_CELL == m_pParamList[nParamID].nDataType||
		E_PARAM_TYPE_W_CQI     == m_pParamList[nParamID].nDataType||
		E_PARAM_TYPE_GSM_COMPLAINT_CELL == m_pParamList[nParamID].nDataType||
		E_PARAM_TYPE_W_COMPLAINT_CELL ==  m_pParamList[nParamID].nDataType
		)
	{
		T_PARAM_MATRIX *pMatrixList = (T_PARAM_MATRIX*)m_pParamList[nParamID].pData;
		if (NULL == pMatrixList)
		{
			return NULL;
		}
		int iMinIndex = 0;
		int iMaxIndex = m_pParamList[nParamID].nMaxCnt - 1;
		if (pMatrixList[0].iCurTimeIndex> iTimePoint)
		{
			return &(pMatrixList[0]);
		}
		if (iTimePoint > pMatrixList[iMaxIndex].iCurTimeIndex)
		{
			return &(pMatrixList[iMaxIndex]);
		}
		while(iMinIndex <= iMaxIndex) //二分查找法
		{
			int nMid = (iMaxIndex+iMinIndex)/2; 
			if (pMatrixList[nMid].iCurTimeIndex == iTimePoint)
			{
				return &(pMatrixList[nMid]);

			}else if (pMatrixList[nMid].iCurTimeIndex > iTimePoint)
			{
				iMaxIndex = nMid - 1;
				if ((iMaxIndex - iMinIndex) <=20)
				{
					break;
				}
			}else 
			{
				iMinIndex = nMid + 1;
				if ((iMaxIndex - iMinIndex) <=20)
				{
					break;
				}
			}
		}
		if (pMatrixList[iMinIndex].iCurTimeIndex > iTimePoint)
		{
			pMatrix = &(pMatrixList[iMinIndex-1]);
		}else
		{
			for (int n=iMinIndex; n<=iMaxIndex; n++)
			{
				if (pMatrixList[n].iCurTimeIndex > iTimePoint)  ////sprintf(szValue, "Value:%d--->Time:%d\n", pParamValueList[n].iValue, pParamValueList[n].iCurTimeIndex);	//::OutputDebugStr(szValue);
				{
					break;
				}
				pMatrix = &(pMatrixList[n]);
			}
		}
	}
	return pMatrix;	
}

T_PARAM_EVENT_MATRIX * CEventDecode::GetEventMatrix(void)
{
	if( false == m_gLogInfo.bLoad ) return NULL;
	return &m_gEventMatrix;
}

unsigned int CEventDecode::GetEventCnt(void)
{
	if( false == m_gLogInfo.bLoad ) return 0;
	return m_pParamList[E_PARAM_EVENT].nMaxCnt;
}

void CEventDecode::CountParam_Value(unsigned int nParamID)
{
	m_pParamList[nParamID].nMaxCnt += 1;
}

T_PARAM_MESSAGE_MATRIX *CEventDecode::GetMessageMatrix(void)
{
	if( false == m_gLogInfo.bLoad ) return NULL;
	return &m_MsgMatrix;
}

T_COMPLAINT_EVENT_MATRIX *CEventDecode::GetComplanitEventMatrix(void)
{
	if( false == m_gLogInfo.bLoad ) return NULL;
	return &m_complaintEventMatrix;
}

unsigned int CEventDecode::GetMessageCnt(void)
{
	if( false == m_gLogInfo.bLoad ) return 0;
	return m_MsgMatrix.nRealCnt;
}

int CEventDecode::GetLogInfo(const char *pSrcFileName)
{
	unsigned char headBuf[100] = {0};
	m_chNetFlag = 0;

#define MAX_PACKET_LEN 2048
	unsigned char packetBuf[MAX_PACKET_LEN] = {0};
	unsigned int  readlen = 0, nTime = 0, nPacketHeadSize = sizeof(T_EVENT_PACKAGE_HEADER)-4;

	memset(&m_gLogInfo, 0, sizeof(T_LOG_INFO));
	m_gLogInfo.bGSM     = false;
	m_gLogInfo.bTDSCDMA = false;
	m_gLogInfo.bWCDMA   = false;
	m_gLogInfo.bCDMA    = false;
	m_gLogInfo.bEVDO    = false;
	m_gLogInfo.nEndTime = 0;
	m_gLogInfo.nStartTime    = 0;
	m_gLogInfo.nTimePointCnt = 0;
	m_gLogInfo.nCurTimePoint = 0;

	//init msg matrix
	m_MsgMatrix.nRealCnt = 0;
	m_MsgMatrix.pList    = NULL;

	//init event matrix
	m_gEventMatrix.nRealCnt = 0;
	m_gEventMatrix.pList = NULL;

	T_EVENT_PACKAGE_HEADER *pPacket = NULL;
	if(NULL == pSrcFileName)
	{
		return E_RESULT_FILE_NOT_EXISTING;
	}
	FILE *pFile = fopen(pSrcFileName,"rb");
	if (NULL == pFile)
	{
		return E_RESULT_FILE_CAN_NOT_OPEN;
	}
	do
	{
		readlen = 0;
		readlen = fread(headBuf, 1, nPacketHeadSize, pFile);
		if(readlen < nPacketHeadSize)
		{
			break;         // enf of file
		}	
		pPacket = (T_EVENT_PACKAGE_HEADER*)headBuf;
		if(	TEVENT_PACKET_HEADER != pPacket->PackageFlag )
		{
			continue;
		}
		if(	TEVENT_PACKET_HEADER == pPacket->PackageFlag )
		{
			if( MAX_PACKET_LEN < pPacket->nPacketLen )
			{
				continue;
			}

			if(pPacket->nPacketLen <= nPacketHeadSize)
			{
				return E_RESULT_FILE_NULL;
			}

			if( nTime != pPacket->time && pPacket->time != 0 )
			{
				if (0 == m_gLogInfo.nStartTime)
				{
					m_gLogInfo.nStartTime = pPacket->time;
				}
				m_gLogInfo.nEndTime = pPacket->time;				
				m_gLogInfo.nTimePointCnt++;

				nTime = pPacket->time;
			}
			memset(packetBuf, 0, sizeof(packetBuf));
			fread(packetBuf, 1, (pPacket->nPacketLen - nPacketHeadSize), pFile);
			//check system
			if (TEVENT_ID_H_EI == pPacket->EventID)
			{
				TEVENT_H_EI_STR *pEI = (TEVENT_H_EI_STR*)(packetBuf);
				strcpy(m_gLogInfo.szEI,  pEI->Deviceidentity);
			}else if (TEVENT_ID_H_START == pPacket->EventID)
			{
				TEVENT_H_START_STR *pStart = (TEVENT_H_START_STR*)(packetBuf);
				int nDay;
				int nMonth;
				int nYear;
				sscanf(pStart->Date, "%d.%d.%d", &nDay, &nMonth, &nYear);
				m_gLogInfo.wDay   = nDay;
				m_gLogInfo.wMonth = nMonth;
				m_gLogInfo.wYear  = nYear;
			}else if (TEVENT_ID_H_OUTDOOR == pPacket->EventID)
			{
				ParseLogBase((char*)packetBuf);
			}else if (TEVENT_ID_H_DN == pPacket->EventID)
			{
				std::string strDNString;
				strDNString = (char*)packetBuf;
				strcpy(m_gLogInfo.szDeviceName, (char*)packetBuf);
				if (strDNString.compare("WCDMA & GSM phone") == 0)
				{
					m_gLogInfo.emDeviceType = EM_MOBILE_DEVICE_DEC;
				}else if (strDNString.compare("iphone cooltest 3") == 0)
				{
					m_gLogInfo.emDeviceType = EM_MOBILE_DEVICE_IPHONE_3;
				}else if (strDNString.compare("iphone4 cooltest 3") == 0)
				{
					m_gLogInfo.emDeviceType = EM_MOBILE_DEVICE_IPHONE_3GS;
				}else if (strDNString.compare("W12_PROFESSIONAL") == 0)
				{
					m_gLogInfo.emDeviceType = EM_MOBILE_DEVICE_HTC_S710E;
				}else if (strDNString.compare("IPHONE4S_PROFESSIONAL") == 0 )
				{
					m_gLogInfo.emDeviceType = EM_MOBILE_DEVICE_IPHONE_4S;
				}else
				{
					m_gLogInfo.emDeviceType = EM_MOBILE_DEVICE_UNKNOW;
				}

				if (strDNString.compare("1001") == 0)
				{
					m_gLogInfo.emDeviceType = EM_MOBILE_DEVICE_IPHONE_3;
				}else if (strDNString.compare("1002") == 0)
				{
					m_gLogInfo.emDeviceType = EM_MOBILE_DEVICE_IPHONE_3GS;
				}else if (strDNString.compare("1003") == 0)
				{
					m_gLogInfo.emDeviceType = EM_MOBILE_DEVICE_IPHONE_4;
				}else if (strDNString.compare("1004") == 0)
				{
					m_gLogInfo.emDeviceType = EM_MOBILE_DEVICE_IPHONE_4S;
				}else if (strDNString.compare("1005") == 0 )
				{
					m_gLogInfo.emDeviceType = EM_MOBILE_DEVICE_IPHONE_5;
				}else if (strDNString.compare("1101") == 0 )
				{
					m_gLogInfo.emDeviceType = EM_MOBILE_DEVICE_HTC_S710E;
				}else if (strDNString.compare("1102") == 0 )
				{
					m_gLogInfo.emDeviceType = EM_MOBILE_DEVICE_HTC_Z710E;
				}

			}else if (TEVENT_ID_RELATED_TEST_SCRIPT == pPacket->EventID)
			{
				TEVENT_RELATED_TEST_SCRIPT_STR *pTestScriptSTR = (TEVENT_RELATED_TEST_SCRIPT_STR*)packetBuf;
				wchar_t szBuffer[260] = {0};
				int nIndex = 0;
				int nLen = sizeof(pTestScriptSTR->relatedNum);
				for (int n=0; n<nLen; )
				{
					memcpy(&(szBuffer[nIndex]), &(pTestScriptSTR->relatedNum[n]), 2);
					n += 2;
					nIndex++;
				}
				_gConverW2M(m_gLogInfo.szPhoneRelatedNum, szBuffer, sizeof(pTestScriptSTR->relatedNum), sizeof(m_gLogInfo.szPhoneRelatedNum));

			}else if (TEVENT_ID_WORK_TYPE == pPacket->EventID)
			{
				TEVENT_WORK_TYPE_STR *pLogWorkType = (TEVENT_WORK_TYPE_STR*)packetBuf;
				m_gLogInfo.iLogWorkType = pLogWorkType->workType;
				switch(m_gLogInfo.iLogWorkType)
				{
				case TEVENT_WORK_TYPE_TRIAL_TEST:
					{
						m_gLogInfo.iLogWorkType = 0;
					}
					break;
				case TEVENT_WORK_TYPE_WORKING:
					{
						m_gLogInfo.iLogWorkType = 1;
					}
					break;
				default:
					{
						m_gLogInfo.iLogWorkType = -1;
					}
				}
			}
			if( TEVENT_ID_H_INDOOR == pPacket->EventID )
			{
				TEVENT_H_INDOOR_STR *pIndoor = (TEVENT_H_INDOOR_STR *)packetBuf;
				//m_gLogInfo.bIsIndoor = true;
				unsigned int nOffset = 0;
				for( char i = 0 ; i<pIndoor->paramNum; ++i)
				{
					TEVENT_INDOOR_STR *pIndoor_info = (TEVENT_INDOOR_STR *)(pIndoor->Pad + nOffset);
					wchar_t szBuffer[260] = {0};
					int nIndex = 0;
					int nLen = pIndoor_info->uParamLen;
					for (int n=0; n<nLen; )
					{
						memcpy(&(szBuffer[nIndex]), &(pIndoor_info->Param[n]), 2);
						n += 2;
						nIndex++;
					}
					if( INDOOR_PARAM_PROJECT_NAME == pIndoor_info->uParamType )
					{
						_gConverW2M( m_gLogInfo.indoor.szIndoor_Project, szBuffer, pIndoor_info->uParamLen, sizeof(m_gLogInfo.indoor.szIndoor_Project));
					}
					else if( INDOOR_PARAM_DOMAIN_NAME == pIndoor_info->uParamType )
					{
						_gConverW2M( m_gLogInfo.indoor.szIndoor_Domain, szBuffer, pIndoor_info->uParamLen, sizeof(m_gLogInfo.indoor.szIndoor_Domain));
					}
					else if( INDOOR_PARAM_BUILDING_NAME == pIndoor_info->uParamType )
					{
						_gConverW2M( m_gLogInfo.indoor.szIndoor_Building, szBuffer, pIndoor_info->uParamLen, sizeof(m_gLogInfo.indoor.szIndoor_Building));
					}
					else if( INDOOR_PARAM_FLOOR_NUMBER == pIndoor_info->uParamType )
					{
						_gConverW2M( m_gLogInfo.indoor.szIndoor_Floor, szBuffer, pIndoor_info->uParamLen, sizeof(m_gLogInfo.indoor.szIndoor_Floor));
					}
					else if( INDOOR_PARAM_ROOM_NUMBER == pIndoor_info->uParamType )
					{
						_gConverW2M( m_gLogInfo.indoor.szIndoor_Room, szBuffer, pIndoor_info->uParamLen, sizeof(m_gLogInfo.indoor.szIndoor_Room));
					}
					else if( INDOOR_PARAM_TEST_PROJECT == pIndoor_info->uParamType )
					{
						_gConverW2M( m_gLogInfo.indoor.szIndoor_Project_Type, szBuffer, pIndoor_info->uParamLen, sizeof(m_gLogInfo.indoor.szIndoor_Project_Type));
					}
					else if( INDOOR_PARAM_TEST_TASK == pIndoor_info->uParamType )
					{
						_gConverW2M( m_gLogInfo.indoor.szIndoor_Project_Task, szBuffer, pIndoor_info->uParamLen, sizeof(m_gLogInfo.indoor.szIndoor_Project_Task));
					}
					else if( INDOOR_PARAM_REMARK_TEXT == pIndoor_info->uParamType )
					{
						_gConverW2M( m_gLogInfo.indoor.szIndoor_Project_Remark, szBuffer, pIndoor_info->uParamLen, sizeof(m_gLogInfo.indoor.szIndoor_Project_Remark));
					}
					nOffset += (pIndoor_info->uParamLen + 2);
				}
			}else if (TEVENT_ID_H_ST == pPacket->EventID)
			{
				std::string strSTString;
				strSTString = (char*)packetBuf;
				if (strSTString.compare("inspection") == 0)
				{
					m_gLogInfo.iLogServiceType = EM_LOG_SERVICE_inspection;

				}else if (strSTString.compare("complaint") == 0)
				{
					m_gLogInfo.iLogServiceType = EM_LOG_SERVICE_complaint;
				}else if (strSTString.compare("spotcheck") == 0)
				{
					m_gLogInfo.iLogServiceType = EM_LOG_SERVICE_spotcheck;
				}
				else
				{
					m_gLogInfo.iLogServiceType = EM_LOG_SERVICE_normal;
				}
			}else if (TEVENT_ID_H_SC == pPacket->EventID)
			{
				std::string strSCString;
				strSCString = (char*)packetBuf;
				if (strSCString.compare("indoor") == 0)
				{
					m_gLogInfo.bIsIndoor = true;
				}else
				{
					m_gLogInfo.bIsIndoor = false;
				}
			}else if (TEVENT_ID_H_PROVINCE == pPacket->EventID)
			{
				TEVENT_H_PROVINCE_STR* pProvince = (TEVENT_H_PROVINCE_STR *)packetBuf;
				m_gLogInfo.iProvinceID = GetI32(pProvince->province);

			}else if (TEVENT_ID_H_CITY == pPacket->EventID)
			{
				TEVENT_H_CITY_STR* pCity = (TEVENT_H_CITY_STR *)packetBuf;
				m_gLogInfo.iCityID = GetI32(pCity->city);
			}else if (TEVENT_ID_H_AREA == pPacket->EventID)
			{
				TEVENT_H_AREA_STR* pArea = (TEVENT_H_AREA_STR*)packetBuf;
				m_gLogInfo.iAreaID = GetI32(pArea->area);
			}else if (TEVENT_ID_H_PMD5 == pPacket->EventID)
			{
				strcpy(m_gLogInfo.szPhotoMd5, (char*)packetBuf);
			}else if (TEVENT_ID_H_WOID == pPacket->EventID)
			{
				TEVENT_H_WOID_STR* pWoirStr = (TEVENT_H_WOID_STR*)packetBuf;
				m_gLogInfo.iWorkID = GetI32(pWoirStr->woid);
			}else if (TEVENT_ID_H_PRID == pPacket->EventID)
			{
				TEVENT_H_PRID_STR* pridStr = (TEVENT_H_PRID_STR*)packetBuf;
				m_gLogInfo.iProjectID = GetI32(pridStr->prid);
			}else if (TEVENT_ID_H_TOS == pPacket->EventID)
			{
				strcpy(m_gLogInfo.szTOS, (char*)packetBuf);
			}else if (TEVENT_ID_CMP_USRID == pPacket->EventID)
			{
				TEVENT_CMP_H_USRID_STR* pCMPUSID = (TEVENT_CMP_H_USRID_STR*)packetBuf;
				strcpy(m_gLogInfo.szTsUSerID,  pCMPUSID->userID);
			}else if (TEVENT_ID_USRBHV_ONLINE == pPacket->EventID)
			{
				TEVENT_H_USRBHV_ONLINE_STR* pHOnlineStr = (TEVENT_H_USRBHV_ONLINE_STR*)packetBuf;
				m_gLogInfo.tsTimeInfo.nGSM_HOURS   = GetI32(pHOnlineStr->gsmOnlineDuration);
				m_gLogInfo.tsTimeInfo.nWCDMA_HOURS = GetI32(pHOnlineStr->wcdmaOnlineDuration);
				m_gLogInfo.tsTimeInfo.nWIFI_HOURS  = GetI32(pHOnlineStr->wifiOnlineDuration);
			}else if (TEVENT_ID_BUILDINGINFO == pPacket->EventID)
			{
				TEVENT_BUILDING_INFO_STR* pAnBuliding = (TEVENT_BUILDING_INFO_STR*)packetBuf;
				m_gLogInfo.indoor.iAnInstallType = pAnBuliding->installType;
				m_gLogInfo.indoor.iAnoperateType = pAnBuliding->operateType;
				strcpy(m_gLogInfo.indoor.szAnBuildingID, pAnBuliding->buildingAuid);
				wchar_t szBuffer[260] = {0};
				int nIndex_1 = 0;
				int nLen = pAnBuliding->uAntennaNameLen;
				for (int n=0; n<nLen; )
				{
					memcpy(&(szBuffer[nIndex_1]), &(pAnBuliding->uAntennaName[n]), 2);
					n += 2;
					nIndex_1++;
				}
				_gConverW2M(m_gLogInfo.indoor.szAntennaName,  szBuffer, nLen, sizeof(m_gLogInfo.indoor.szAntennaName));
				memset(szBuffer, 0, sizeof(szBuffer));
				char* szRemrk = (char*)(pAnBuliding->uAntennaName) + nLen; 
				nIndex_1 = 0;
				nLen     = pAnBuliding->uRemarkLen;
				for (int n=0; n<nLen; )
				{
					memcpy(&(szBuffer[nIndex_1]), &(szRemrk[n]), 2);
					n += 2;
					nIndex_1++;
				}
				_gConverW2M(m_gLogInfo.indoor.szAnRemark,  szBuffer, nLen, sizeof(m_gLogInfo.indoor.szAnRemark));
			}
			else if( TEVENT_ID_H_INDOORMAP == pPacket->EventID )
			{
				TEVENT_H_INDOORMAP_STR *pIndoor_info = (TEVENT_H_INDOORMAP_STR *)packetBuf;
				//m_gLogInfo.bIsIndoor = true;
				wchar_t szBuffer[260] = {0};
				int nIndex = 0;
				int nLen = (pPacket->nPacketLen - nPacketHeadSize - ((unsigned long)pIndoor_info->strMapName - (unsigned long)pIndoor_info));
				for (int n=0; n<nLen; )
				{
					memcpy(&(szBuffer[nIndex]), &(pIndoor_info->strMapName[n]), 2);
					n += 2;
					nIndex++;
				}
				_gConverW2M( m_gLogInfo.indoor.szIndoor_IndoorMap, szBuffer, (pPacket->nPacketLen - nPacketHeadSize - ((unsigned long)pIndoor_info->strMapName - (unsigned long)pIndoor_info)), sizeof(m_gLogInfo.indoor.szIndoor_IndoorMap));
				m_gLogInfo.indoor.nIndoor_map_width  = GetU16(pIndoor_info->nWidth);
				m_gLogInfo.indoor.nIndoor_map_height = GetU16(pIndoor_info->uLength);
			}
			else if( ( TEVENT_ID_RRCSM == pPacket->EventID )||
				( TEVENT_ID_L3SM == pPacket->EventID ) ||
				//	 ( TEVENT_ID_L2SM == pPacket->EventID ) || 					 
				//	 ( TEVENT_ID_RLCSM == pPacket->EventID ) ||
				( TEVENT_ID_MACSM == pPacket->EventID ) ||
				//	 ( TEVENT_ID_LLCSM == pPacket->EventID ) ||
				( TEVENT_ID_SNPSM == pPacket->EventID )
				//	 ( TEVENT_ID_RRLPSM == pPacket->EventID )||
				//	 ( TEVENT_ID_GANSM == pPacket->EventID ) ||
				//	 ( TEVENT_ID_SIPSM == pPacket->EventID ) ||
				//	 ( TEVENT_ID_RTPSM == pPacket->EventID ) 
				)
			{
				m_MsgMatrix.nRealCnt++;
			}
			else if(( TEVENT_ID_CAA == pPacket->EventID )||
				( TEVENT_ID_CAC_GSM == pPacket->EventID)||
				( TEVENT_ID_CAC == pPacket->EventID ) ||
				( TEVENT_ID_CAF == pPacket->EventID ) || 					 
				( TEVENT_ID_CAD == pPacket->EventID ) ||

				( TEVENT_ID_DAA == pPacket->EventID ) ||
				( TEVENT_ID_DAC == pPacket->EventID ) ||
				( TEVENT_ID_DAF == pPacket->EventID ) ||
				( TEVENT_ID_DAD == pPacket->EventID ) ||

				( TEVENT_ID_DREQ == pPacket->EventID ) ||
				( TEVENT_ID_DCOMP == pPacket->EventID )||

				( TEVENT_ID_HOA == pPacket->EventID ) ||
				( TEVENT_ID_HOF == pPacket->EventID ) ||
				( TEVENT_ID_HOS == pPacket->EventID ) ||
				( TEVENT_ID_SHO == pPacket->EventID ) ||

				( TEVENT_ID_CREL == pPacket->EventID ) ||

				( TEVENT_ID_LUA == pPacket->EventID ) ||
				( TEVENT_ID_LUS == pPacket->EventID ) ||
				( TEVENT_ID_LUF == pPacket->EventID ) ||

				( TEVENT_ID_RRA == pPacket->EventID ) ||
				( TEVENT_ID_RRC == pPacket->EventID ) ||
				( TEVENT_ID_RRF == pPacket->EventID ) ||
				( TEVENT_ID_RRD == pPacket->EventID ) ||

				( TEVENT_ID_PAA == pPacket->EventID ) ||
				( TEVENT_ID_PAC == pPacket->EventID ) ||
				( TEVENT_ID_PAF == pPacket->EventID ) ||
				( TEVENT_ID_PAD == pPacket->EventID ) ||

				( TEVENT_ID_GAA == pPacket->EventID ) ||
				( TEVENT_ID_GAC == pPacket->EventID ) ||
				( TEVENT_ID_GAF == pPacket->EventID ) ||
				( TEVENT_ID_GAD == pPacket->EventID ) ||

				( TEVENT_ID_RUA == pPacket->EventID ) ||
				( TEVENT_ID_RUF == pPacket->EventID ) ||
				( TEVENT_ID_RUS == pPacket->EventID ) ||

				( TEVENT_ID_MSGA == pPacket->EventID) ||
				( TEVENT_ID_MSGS == pPacket->EventID) ||
				( TEVENT_ID_MSGF == pPacket->EventID) ||
				( TEVENT_ID_HTTP_URL == pPacket->EventID)||
				( TEVENT_ID_APPINFO == pPacket->EventID) ||
				( TEVENT_ID_APP_OPEN_STATS == pPacket->EventID)||
				( TEVENT_ID_OFF_NET == pPacket->EventID)
				)
			{
				m_gEventMatrix.nRealCnt++;
			}
			else
			{
				CountParam(pPacket->EventID, packetBuf);
			}
		}
		else
		{
			if (NULL != pFile)
			{
				fclose(pFile);
				pFile = NULL;
			}
			return E_RESULT_FILE_INVALID;
		}

	}while(1);

	if (NULL != pFile)
	{
		fclose(pFile);
		pFile = NULL;
	}
	unsigned int iEnd   = (TIME_GET_HOUR(m_gLogInfo.nEndTime)*3600000)   + (TIME_GET_MIN(m_gLogInfo.nEndTime)* 60000)   + (TIME_GET_SEC(m_gLogInfo.nEndTime) * 1000)   + TIME_GET_MSEC(m_gLogInfo.nEndTime);
	unsigned int iStart = (TIME_GET_HOUR(m_gLogInfo.nStartTime)*3600000) + (TIME_GET_MIN(m_gLogInfo.nStartTime)* 60000) + (TIME_GET_SEC(m_gLogInfo.nStartTime) * 1000) + TIME_GET_MSEC(m_gLogInfo.nStartTime);
	m_gLogInfo.nTimeDuration = iEnd - iStart;;
	if(m_gLogInfo.nTimePointCnt<=0) return E_RESULT_FILE_INVALID;
	EM_LOG_NET_TYPE logTypeSys = GetLogNetType();
	if ((EM_GSM_NET_TYPE ==logTypeSys) || (EM_TD_GSM_NET_TYPE == logTypeSys) || (EM_WCDMA_GSM_NET_TYPE ==logTypeSys))
	{
		m_gLogInfo.bGSM = true;
	}
	if ((EM_WCDMA_NET_TYPE == logTypeSys) || (EM_WCDMA_GSM_NET_TYPE ==logTypeSys))
	{
		m_gLogInfo.bWCDMA = true;
	}
	if ((EM_CDMA_NET_TYPE == logTypeSys) || EM_EVDO_CDMA_NET_TYPE ==logTypeSys )
	{
		m_gLogInfo.bCDMA = true;
	}
	if ((EM_EVDO_NET_TYPE ==logTypeSys) || EM_EVDO_CDMA_NET_TYPE == logTypeSys)
	{
		m_gLogInfo.bEVDO = true;
	}
	if ((!m_gLogInfo.bGSM) && (!m_gLogInfo.bWCDMA) && (!m_gLogInfo.bCDMA) && (!m_gLogInfo.bEVDO))
	{
		return E_RESULT_FILE_NO_NET;
	}
	return E_RESULT_OK;
}

void CEventDecode::CountParam(unsigned short EventID, unsigned char* pBuf)
{
	switch (EventID)
	{
	case TEVENT_ID_CELLMEAS:
		{
			m_CurSystem = pBuf[0];
			SetNetTpye(pBuf[0]);
			TEVENT_CELLMEAS_STR *p = (TEVENT_CELLMEAS_STR*)pBuf;
			if( TEVENT_SYSTEM_GSM == p->System)
			{
				TEVENT_CELLMEAS_GSM_STR *pCellPacket = (TEVENT_CELLMEAS_GSM_STR *)p->Pad;
				if(pCellPacket->nCells > MAX_CELLMEAS_NUM_CELLS) return;
				m_pParamList[E_PARAM_GSM_CELLS].nMaxCnt += 1;
				for( unsigned int i = 0; i< pCellPacket->nCells; ++i )
				{
					TEVENT_CELLMEAS_GSM_PARAMSCELL_STR *pCell = (TEVENT_CELLMEAS_GSM_PARAMSCELL_STR *)( pCellPacket->PARAMSCELL + sizeof(TEVENT_CELLMEAS_GSM_PARAMSCELL_STR)*i);
					if( 1 == pCell->Celltype )
					{								
						CountParam_Value( E_PARAM_GSM_RXLEV_FULL);
						CountParam_Value( E_PARAM_GSM_CI);
						CountParam_Value( E_PARAM_GSM_RXLEV_SUB);		
						CountParam_Value( E_PARAM_GSM_BAND);
						CountParam_Value( E_PARAM_GSM_C1);
						CountParam_Value( E_PARAM_GSM_C2);
						CountParam_Value( E_PARAM_GSM_BCCH);
						CountParam_Value( E_PARAM_GSM_LAC);
						CountParam_Value( E_PARAM_GSM_RXLEV_N1);
						m_gLogInfo.iLastBand_2g = pCell->Band;
					}
					else
					{
						if ( i < MAX_CELL_CNT )
						{	
							CountParam_Value( ( E_PARAM_GSM_RXLEV_N1 + i - 1));
						}
					}
				}

			}else if (TEVENT_SYSTEM_UMTSTD_SCDMA == p->System)
			{
				TEVENT_CELLMEAS_UMTSTDSCDMA_STR *pCellPacket = (TEVENT_CELLMEAS_UMTSTDSCDMA_STR *)p->Pad;
				if(pCellPacket->nCells > MAX_CELLMEAS_NUM_CELLS) return;
				if(pCellPacket->nChs > MAX_CELLMEAS_NUM_CH) return;
				m_pParamList[E_PARAM_TD_CHANNELS].nMaxCnt += 1;
				TEVENT_CELLMEAS_UMTSTDSCDMA_PARAMSCHANNEL_STR *pCellCh = NULL;
				int iMaxCHCnt = MAX_CH_CNT;
				if (pCellPacket->nChs < MAX_CH_CNT)
				{
					iMaxCHCnt = pCellPacket->nChs;
				}
				for( unsigned int i = 0; i< iMaxCHCnt; ++i )
				{
					if( i<pCellPacket->nChs )
					{
						pCellCh = (TEVENT_CELLMEAS_UMTSTDSCDMA_PARAMSCHANNEL_STR *)( pCellPacket->PARAMSCHANNEL + sizeof(TEVENT_CELLMEAS_UMTSTDSCDMA_PARAMSCHANNEL_STR)*i );
						CountParam_Value( (E_PARAM_TD_RSSI + i ));
					}
					else
					{
						CountParam_Value( (E_PARAM_TD_RSSI + i ));
					}
				}
				unsigned char* pData = (pCellPacket->PARAMSCHANNEL + pCellPacket->nChs * sizeof(TEVENT_CELLMEAS_UMTSTDSCDMA_PARAMSCHANNEL_STR));
				TEVENT_CELLMEAS_UMTSTDSCDMA_PARAMSCELL_STR *pCell = (TEVENT_CELLMEAS_UMTSTDSCDMA_PARAMSCELL_STR *)pData;
				if(pCell->RSCP>=0)
				{
					return;
				}
				m_pParamList[E_PARAM_TD_CELLS].nMaxCnt += 1;
				int iMaxCellCnt = MAX_CELL_CNT;
				if (pCellPacket->nCells < MAX_CELL_CNT)
				{
					iMaxCellCnt = pCellPacket->nCells;
				}
				for( unsigned int i = 0; i< iMaxCellCnt; ++i )
				{
					if( i<pCellPacket->nCells )
					{
						pCell = (TEVENT_CELLMEAS_UMTSTDSCDMA_PARAMSCELL_STR *)( pData + sizeof(TEVENT_CELLMEAS_UMTSTDSCDMA_PARAMSCELL_STR)*i );
						if(pCell->RSCP<0)
						{
							CountParam_Value(( E_PARAM_TD_PCCPCH_RSCP + i ));
						}	
					}
					else
					{
						CountParam_Value(( E_PARAM_TD_PCCPCH_RSCP + i ));
					}
				}

			}else if (TEVENT_SYSTEM_UMTSFDD == p->System)
			{
				TEVENT_CELLMEAS_UMTSFDD_STR *pCellPacket = (TEVENT_CELLMEAS_UMTSFDD_STR *)p->Pad;
				if(pCellPacket->nCells > MAX_CELLMEAS_NUM_CELLS) 
				{
					return;
				}

				if(pCellPacket->nChs > MAX_CELLMEAS_NUM_CH) 
				{
					return;
				}

				m_pParamList[E_PARAM_W_CHANNELS].nMaxCnt += 1;
				TEVENT_CELLMEAS_UMTSFDD_PARAMSCHANNEL_STR *pCellCh = NULL;
				int iMaxCHCnt = MAX_CH_CNT;
				if (pCellPacket->nChs < MAX_CH_CNT)
				{
					iMaxCHCnt = pCellPacket->nChs;
				}
				for( unsigned int i = 0; i< iMaxCHCnt; ++i )
				{
					if( i<pCellPacket->nChs )
					{
						pCellCh = (TEVENT_CELLMEAS_UMTSFDD_PARAMSCHANNEL_STR *)( pCellPacket->PARAMSCHANNEL + sizeof(TEVENT_CELLMEAS_UMTSFDD_PARAMSCHANNEL_STR)*i);
						CountParam_Value( ( E_PARAM_W_RSSI + i ));
					}
					else
					{
						CountParam_Value( ( E_PARAM_W_RSSI + i ));
					}
				}
				unsigned char* pData = (pCellPacket->PARAMSCHANNEL + pCellPacket->nChs * sizeof(TEVENT_CELLMEAS_UMTSFDD_PARAMSCHANNEL_STR));
				TEVENT_CELLMEAS_UMTSFDD_PARAMSCELL_STR *pCell = (TEVENT_CELLMEAS_UMTSFDD_PARAMSCELL_STR *)pData;
				if(pCell->RSCP>=0)
				{
					return;
				}
				m_pParamList[E_PARAM_W_CELLS].nMaxCnt += 1;
				bool bIsPush          = true;
				if (pCellPacket->nCells > 100)
				{
					return;
				}
				for( unsigned int i = 0; i< pCellPacket->nCells; ++i )
				{
					pCell = (TEVENT_CELLMEAS_UMTSFDD_PARAMSCELL_STR *)( pData + sizeof(TEVENT_CELLMEAS_UMTSFDD_PARAMSCELL_STR)*i );
					if(pCell->RSCP<0)
					{
						if (0 == pCell->Celltype)//激活集
						{
							if (bIsPush)
							{
								CountParam_Value(E_PARAM_W_SERV_SCR);
								CountParam_Value(E_PARAM_W_SERV_RQUAL);
								CountParam_Value(E_PARAM_W_SERV_SQUAL);
								CountParam_Value(E_PARAM_W_SERV_SRXLEV);
								CountParam_Value(E_PARAM_W_SERV_RRXLEV);
								CountParam_Value(E_PARAM_W_SERV_UARFCN);
								CountParam_Value(E_PARAM_W_SERV_EC_NO);
								bIsPush = false;
							}
						}
						if ( i < MAX_CELL_CNT )
						{	
							CountParam_Value((E_PARAM_W_CPICH_RSCP + i ));
						}
					}	
				}
				CountParam_Value(E_PARAM_W_SERV_EC_NO_COMBIN);
				CountParam_Value(E_PARAM_W_SERV_EC_NO_BEST);
				CountParam_Value(E_PARAM_W_SERV_RSCP_COMBIN);

			}else if( TEVENT_SYSTEM_CDMA1X == p->System || TEVENT_SYSTEM_CDMAONE == p->System )
			{
				TEVENT_CELLMEAS_CDMAONEANDCDMA1X_STR *pCellPacket = (TEVENT_CELLMEAS_CDMAONEANDCDMA1X_STR *)p->Pad;
				if(pCellPacket->nCells > MAX_CELLMEAS_NUM_CELLS) return;
				if(pCellPacket->nChs > MAX_CELLMEAS_NUM_CH) return;

				m_pParamList[E_PARAM_CDMA_CHANNELS].nMaxCnt += 1;
				TEVENT_CELLMEAS_CDMAONEANDCDMA1X_PARAMSCHANNEL_STR *pCellCh = NULL;
				for ( unsigned int i = 0; i< MAX_CELL_CNT; ++i  )
				{
					if( i<pCellPacket->nChs )
					{
						pCellCh = (TEVENT_CELLMEAS_CDMAONEANDCDMA1X_PARAMSCHANNEL_STR *)( pCellPacket->PARAMSCHANNEL + sizeof(TEVENT_CELLMEAS_CDMAONEANDCDMA1X_PARAMSCHANNEL_STR)*i );
						CountParam_Value( ( E_PARAM_CDMA_RXPOWR + i ) );				
					}
					else
					{
						CountParam_Value( ( E_PARAM_CDMA_RXPOWR + i ));				
					}
				}
				unsigned char *pData = (pCellPacket->PARAMSCHANNEL + pCellPacket->nChs * sizeof( _TEVENT_CELLMEAS_CDMAONEANDCDMA1X_PARAMSCHANNEL_STR_ ));
				TEVENT_CELLMEAS_CDMAONEANDCDMA1X_PARAMSCELL_STR *pCell = (TEVENT_CELLMEAS_CDMAONEANDCDMA1X_PARAMSCELL_STR *)pData;
				bool bIsPush           = true;
				if (pCellPacket->nCells > 100)
				{
					return;
				}
				m_pParamList[E_PARAM_CDMA_CELLS].nMaxCnt += 1;
				for ( unsigned int i = 0; i < MAX_CELL_CNT; ++i )
				{
					CountParam_Value( E_PARAM_CDMA_RSCP + i);
				}
				for( unsigned int i = 0; i< pCellPacket->nCells; ++i )
				{
					pCell = (TEVENT_CELLMEAS_CDMAONEANDCDMA1X_PARAMSCELL_STR *)( pData + sizeof(TEVENT_CELLMEAS_CDMAONEANDCDMA1X_PARAMSCELL_STR)*i );
					if (0 == pCell->Set)//激活集
					{
						if (bIsPush)
						{
							CountParam_Value( E_PARAM_CDMA_SERV_BAND);
							CountParam_Value( E_PARAM_CDMA_SERV_CHANNEL_NUMBER);
							CountParam_Value( E_PARAM_CDMA_SERV_PILOT_NUMVER);
							CountParam_Value( E_PARAM_CDMA_SERV_WALSH );
							CountParam_Value( E_PARAM_CDMA_SERV_RSCP);
							CountParam_Value( E_PARAM_CDMA_SERV_ECIO);
							bIsPush = false;
						}
						if ( i < MAX_CELL_CNT )
						{
							CountParam_Value( E_PARAM_CDMA_RSCP + i);
						}
					}		
				}			
			}else if ( TEVENT_SYSTEM_EVDO == p->System)
			{
				TEVENT_CELLMEAS_EVDO_STR *pCellPacket = (TEVENT_CELLMEAS_EVDO_STR *)p->Pad;
				if(pCellPacket->nChs > MAX_CELLMEAS_NUM_CH) return;
				if(pCellPacket->nCells > MAX_CELLMEAS_NUM_CELLS) return;

				m_pParamList[E_PARAM_EVDO_CHANNELS].nMaxCnt += 1;
				TEVENT_CELLMEAS_EVDO_PARAMSCHANNEL_STR *pCellCh = NULL;
				for ( unsigned int i = 0; i< MAX_CELL_CNT; ++i  )
				{
					if( i<pCellPacket->nChs )
					{
						pCellCh = (TEVENT_CELLMEAS_EVDO_PARAMSCHANNEL_STR *)( pCellPacket->PARAMSCHANNEL + sizeof(TEVENT_CELLMEAS_EVDO_PARAMSCHANNEL_STR)*i );
						CountParam_Value( ( E_PARAM_EVDO_RXPOWR + i ));				
					}
					else
					{
						CountParam_Value( ( E_PARAM_EVDO_RXPOWR + i ));				
					}
				}
				unsigned char* pData = (pCellPacket->PARAMSCHANNEL + pCellPacket->nChs * sizeof(TEVENT_CELLMEAS_EVDO_PARAMSCHANNEL_STR));
				TEVENT_CELLMEAS_EVDO_PARAMSCELL_STR *pCell = (TEVENT_CELLMEAS_EVDO_PARAMSCELL_STR *)pData;
				bool bIsPush           = true;
				if (pCellPacket->nCells > 100)
				{
					return;
				}
				m_pParamList[E_PARAM_EVDO_CELLS].nMaxCnt += 1;
				for ( unsigned int i = 0; i < MAX_CELL_CNT; ++i )
				{
					CountParam_Value( E_PARAM_EVDO_RSCP + i);
				}
				for( unsigned int i = 0; i< pCellPacket->nCells; ++i )
				{
					pCell = (TEVENT_CELLMEAS_EVDO_PARAMSCELL_STR *)( pData + sizeof(TEVENT_CELLMEAS_EVDO_PARAMSCELL_STR)*i );
					if (0 == pCell->Set)//激活集
					{
						if (bIsPush)
						{
							CountParam_Value( E_PARAM_EVDO_SERV_BAND);
							CountParam_Value( E_PARAM_EVDO_SERV_CHANNEL_NUMBER);
							CountParam_Value( E_PARAM_EVDO_SERV_PILOT_NUMVER);
							CountParam_Value( E_PARAM_EVDO_SERV_RSCP);
							CountParam_Value( E_PARAM_EVDO_SERV_ECIO);
							bIsPush = false;
						}
						if ( i < MAX_CELL_CNT )
						{
							CountParam_Value( E_PARAM_EVDO_RSCP + i);
						}
					}		
				}	

			}

		}
		break;
	case TEVENT_ID_CQI:
		{
			TEVENT_CQI_STR *p = (TEVENT_CQI_STR*)pBuf;
			if (TEVENT_SYSTEM_UMTSFDD == p->System)
			{
				m_pParamList[E_PARAM_W_CQI].nMaxCnt   += 1;
				m_pParamList[E_PARAM_W_CQI_1].nMaxCnt += 1;
			}
		}
		break;
	case TEVENT_ID_CHI:
		{
			TEVENT_CHI_STR *p = (TEVENT_CHI_STR*)pBuf;
			m_CurSystem = p->System;
			if( TEVENT_SYSTEM_GSM == p->System )
			{
				CountParam_Value( E_PARAM_GSM_CI);
				CountParam_Value( E_PARAM_GSM_DTX);
				CountParam_Value( E_PARAM_GSM_TN);
				CountParam_Value( E_PARAM_GSM_VOICE_CODEC);
				CountParam_Value( E_PARAM_GSM_CHI_CHANNEL);
				CountParam_Value( E_PARAM_GSM_CH_TYPE);

			}else if( TEVENT_SYSTEM_UMTSTD_SCDMA == p->System )
			{
				CountParam_Value( E_PARAM_TD_SERV_UARFCN);
				CountParam_Value( E_PARAM_TD_SERV_CELL_PARAMETER_ID);
				CountParam_Value( E_PARAM_TD_SERV_CI);
				CountParam_Value( E_PARAM_TD_SERV_LAC);
				CountParam_Value( E_PARAM_TD_UE_MAX_ALLOWED_UL_TX_POWER);
			}else if( TEVENT_SYSTEM_UMTSFDD == p->System )
			{
				CountParam_Value( E_PARAM_W_SERV_CI);
				CountParam_Value( E_PARAM_W_SERV_TRESELECTION);
				CountParam_Value( E_PARAM_W_SERV_ADDITIONWINDOW);
				CountParam_Value( E_PARAM_W_SERV_LAC);
				CountParam_Value( E_PARAM_W_MAX_ALLOWED_TX_POWER);
				CountParam_Value( E_PARAM_W_RRC_STATE);
				CountParam_Value( E_PARAM_W_SERV_BAND);
				CountParam_Value( E_PARAM_W_SERV_RAC);
			}
		}
		break;
	case TEVENT_ID_SEI:
		{
			TEVENT_SEI_STR *p = (TEVENT_SEI_STR *)pBuf;
			m_CurSystem = p->System;
			if( TEVENT_SYSTEM_GSM == p->System )
			{
				CountParam_Value( E_PARAM_GSM_MNC);
				CountParam_Value( E_PARAM_GSM_MCC);
			}
			else if( TEVENT_SYSTEM_UMTSTD_SCDMA == p->System )
			{
				CountParam_Value( E_PARAM_TD_SERV_MNC);
				CountParam_Value( E_PARAM_TD_SERV_MCC);
				CountParam_Value( E_PARAM_TD_SERV_LAC);
			}
			else if( TEVENT_SYSTEM_UMTSFDD == p->System )
			{
				CountParam_Value( E_PARAM_W_SERV_MNC);
				CountParam_Value( E_PARAM_W_SERV_MCC);
				CountParam_Value( E_PARAM_W_SERV_LAC);
			}
		}
		break;
	case TEVENT_ID_PCHI:
		{
			TEVENT_PCHI_STR *p = (TEVENT_PCHI_STR *)pBuf;
			m_CurSystem = p->System;
			if( TEVENT_SYSTEM_GSM == p->System )
			{
				CountParam_Value( E_PARAM_GSM_RAC);
				CountParam_Value( E_PARAM_GSM_PTECH);
				CountParam_Value( E_PARAM_GSM_PTSUP);
				CountParam_Value( E_PARAM_GSM_PTSDOWN);
				CountParam_Value( E_PARAM_GSM_PCODINGUP);
				CountParam_Value( E_PARAM_GSM_PCODINGDOWN);
			}
			else if( TEVENT_SYSTEM_UMTSTD_SCDMA == p->System )
			{
				CountParam_Value( E_PARAM_TD_SERV_RAC);
				CountParam_Value( E_PARAM_TD_SERV_NMO);
			}
			else if( TEVENT_SYSTEM_UMTSFDD == p->System )
			{
				CountParam_Value( E_PARAM_W_RAC);
				CountParam_Value( E_PARAM_W_PWROFFSET);
				CountParam_Value( E_PARAM_W_PLNONMAX);
				CountParam_Value( E_PARAM_W_EDPCCH_PWROFFSET);		
				CountParam_Value( E_PARAM_W_HSDSCHSC);
				CountParam_Value( E_PARAM_W_HRNTI);
				CountParam_Value( E_PARAM_W_PRIMARY_ERNTI);
				CountParam_Value( E_PARAM_W_SECONDARY_ERNTI);		
				CountParam_Value( E_PARAM_W_HAPPYBITDELAYCOND);
				CountParam_Value( E_PARAM_W_PACKETTECH);
				CountParam_Value( E_PARAM_W_PACKETSTATE);
				CountParam_Value( E_PARAM_W_NWOPERATION);
				CountParam_Value( E_PARAM_W_HSDPAUECATEG);		
				CountParam_Value( E_PARAM_W_ACKNACK_REPETITIONS);
				CountParam_Value( E_PARAM_W_HSUPAUE_CATEG);
				CountParam_Value( E_PARAM_W_TTI);
				CountParam_Value( E_PARAM_W_RATEMATCHING);
				CountParam_Value( E_PARAM_W_AGCHOVSF);
				CountParam_Value( E_PARAM_W_ETFCITABLE);
			}
			else if( TEVENT_SYSTEM_CDMA1X == p->System || TEVENT_SYSTEM_CDMAONE == p->System )
			{
				//TEVENT_PCHI_CDMA1X_STR *pCHI = (TEVENT_PCHI_CDMA1X_STR *)p->pad;		
			}
			else if( TEVENT_SYSTEM_EVDO == p->System )
			{
				//TEVENT_PCHI_EVDO_STR *pCHI = (TEVENT_PCHI_EVDO_STR *)p->pad;	
			}
		}
		break;
	case TEVENT_ID_RXQ:
		{
			TEVENT_RXQ_STR *p = (TEVENT_RXQ_STR *)pBuf;
			if( TEVENT_SYSTEM_GSM == p->System )
			{
				CountParam_Value( E_PARAM_GSM_RXQUAL_FULL);
				CountParam_Value( E_PARAM_GSM_RXQUAL_SUB);		
			}
		}
		break;
	case TEVENT_ID_CI:
		{
			TEVENT_CI_STR *p = (TEVENT_CI_STR *)pBuf;
			if( TEVENT_SYSTEM_GSM == p->System )
			{
				CountParam_Value( E_PARAM_GSM_CBI);
			}
			else if( TEVENT_SYSTEM_UMTSTD_SCDMA == p->System )
			{	
				CountParam_Value( E_PARAM_TD_PCCPCH_CBI);
			}
		}
		break;
	case TEVENT_ID_MSP:
		{
			TEVENT_MSP_STR *p = (TEVENT_MSP_STR *)pBuf;
			if( TEVENT_SYSTEM_GSM == p->System )
			{
				TEVENT_MSP_GSM_STR *pPacket = (TEVENT_MSP_GSM_STR *)p->append;
				if(pPacket->MSP>32||pPacket->MSP<0) 
				{
					CountParam_Value( E_PARAM_GSM_TXPOWER_EX);
					CountParam_Value( E_PARAM_GSM_TXPOWER);	
				}else
				{
					CountParam_Value( E_PARAM_GSM_TXPOWER);	
					switch (m_gLogInfo.iLastBand_2g)
					{
					case 1:
					case 2:
						{
							CountParam_Value( E_PARAM_GSM_TXPOWER_EX);
							break;
						}
					case 3:
						{
							CountParam_Value( E_PARAM_GSM_TXPOWER_EX);
						}
						break;
					case 4:
						{
							CountParam_Value( E_PARAM_GSM_TXPOWER_EX);
						}
						break;
					default:
						{
							CountParam_Value( E_PARAM_GSM_TXPOWER_EX);
						}
					}
				}
			}
		}
		break;
	case TEVENT_ID_TAD:
		{
			TEVENT_TAD_STR *p = (TEVENT_TAD_STR *)pBuf;
			if( TEVENT_SYSTEM_GSM == p->System )
			{
				CountParam_Value( E_PARAM_GSM_TA);
			}
			else if( TEVENT_SYSTEM_UMTSTD_SCDMA == p->System )
			{
				CountParam_Value( E_PARAM_TD_SERV_TA);
			}
		}
		break;
	case TEVENT_ID_HOP:
		{
			TEVENT_HOP_STR *p = (TEVENT_HOP_STR *)pBuf;
			if( TEVENT_SYSTEM_GSM == p->System )
			{
				// take care! it's too complex
				//  >> Hopping status : 1 = On
				//                      2 = Off
				CountParam_Value( E_PARAM_GSM_HCH);
				if( 1 == p->Hopping)
				{
					CountParam_Value( E_PARAM_GSM_MAIO);
					CountParam_Value( E_PARAM_GSM_HSN);
				}
				else
				{
					CountParam_Value( E_PARAM_GSM_MAIO);
					CountParam_Value( E_PARAM_GSM_HSN);
				}
			}
		}
		break;
	case TEVENT_ID_TBFI:
		{
			TEVENT_TBFI_STR *pPacket = (TEVENT_TBFI_STR *)pBuf;
			if (pPacket->System == TEVENT_SYSTEM_GSM)
			{
				CountParam_Value( E_PARAM_GSM_PWINSIZEUL);
				CountParam_Value( E_PARAM_GSM_PWINSIZEDL);
			}
		}
		break;
	case TEVENT_ID_BEP:
		{
			TEVENT_BEP_STR *p = (TEVENT_BEP_STR *)pBuf;
			if( TEVENT_SYSTEM_GSM == p->System )
			{
				CountParam_Value( E_PARAM_GSM_P8PSKCVBEP);
				CountParam_Value( E_PARAM_GSM_PGMSKCVBEP);
				CountParam_Value( E_PARAM_GSM_P8BSKMEANBEP);
				CountParam_Value( E_PARAM_GSM_PGMSKMEANBEP);
			}	
		}
		break;
	case TEVENT_ID_PRXQ:
		{
			TEVENT_PRXQ_STR *p = (TEVENT_PRXQ_STR *)pBuf;
			if( TEVENT_SYSTEM_GSM == p->System )
			{
				CountParam_Value( E_PARAM_GSM_PCVALUE);
				CountParam_Value( E_PARAM_GSM_PRXQUAL);
			}	
		}
		break;
	case TEVENT_ID_FER:
		{
			TEVENT_FER_STR *p = (TEVENT_FER_STR *)pBuf;
			m_CurSystem = p->System;
			if( TEVENT_SYSTEM_GSM == p->System )
			{
				CountParam_Value( E_PARAM_GSM_FER_FULL);
				CountParam_Value( E_PARAM_GSM_FER_SUB);
				CountParam_Value( E_PARAM_GSM_FER_TCH);
			}
			else if( TEVENT_SYSTEM_UMTSTD_SCDMA == p->System )
			{
				CountParam_Value( E_PARAM_TD_FER);
			}
			else if( TEVENT_SYSTEM_UMTSFDD == p->System )
			{
				CountParam_Value( E_PARAM_W_FER);
			}
			else if ( TEVENT_SYSTEM_CDMA1X == p->System || TEVENT_SYSTEM_CDMAONE == p->System )
			{
				CountParam_Value( E_PARAM_CDMA_FER_TOTAL);
				CountParam_Value( E_PARAM_CDMA_FER_FCH);
				CountParam_Value( E_PARAM_CDMA_FER_SCH);
			}
		}
		break;
	case TEVENT_ID_RLT:
		{
			TEVENT_RLT_STR *p = (TEVENT_RLT_STR *)pBuf;
			if( TEVENT_SYSTEM_GSM == p->System )
			{
				CountParam_Value( E_PARAM_GSM_RLT);
			}
		}
		break;
	case TEVENT_ID_DRATE:
		{
			CountParam_Value(E_PARAM_THROUGHT_APP_DL);
			CountParam_Value(E_PARAM_THROUGHT_APP_UL);
			CountParam_Value(E_PARAM_THROUGHT_APP_DL_BYTES);
			CountParam_Value(E_PARAM_THROUGHT_APP_UL_BYTES);
		}
		break;
	case TEVENT_ID_RLCRATE:
		{
			TEVENT_RLCRATE_STR *p = (TEVENT_RLCRATE_STR *)pBuf;
			if( TEVENT_SYSTEM_GSM == p->System )
			{
				CountParam_Value(E_PARAM_THROUGHT_RLC_DL);
				CountParam_Value(E_PARAM_THROUGHT_RLC_UL);
			}
			else if( TEVENT_SYSTEM_UMTSFDD == p->System || TEVENT_SYSTEM_UMTSTD_SCDMA == p->System )
			{
				TEVENT_RLCRATE_UMTS_STR *pPacket = (TEVENT_RLCRATE_UMTS_STR *)p->append;
				CountParam_Value( E_PARAM_THROUGHT_RLC_DL);
				CountParam_Value( E_PARAM_THROUGHT_RLC_UL);
				if (GetI32(pPacket->RLCrateUL) > 0)
				{
					CountParam_Value(E_PARAM_THROUGHT_RLC_UL_RETRANSMISSION);
				}
			}
		}
		break;
	case TEVENT_ID_RLPRATE:
		{
			TEVENT_RLPRATE_STR *p = (TEVENT_RLPRATE_STR *)pBuf;
			if( TEVENT_SYSTEM_CDMA1X == p->System || TEVENT_SYSTEM_CDMAONE == p->System )
			{
				CountParam_Value(E_PARAM_CDMA_RLPRATE_RLP_REV_RATE);
				CountParam_Value(E_PARAM_CDMA_RLPRATE_RLP_FOR_RATE);
				CountParam_Value(E_PARAM_CDMA_RLPRATE_RLP_REV_RETR_RATE);
				CountParam_Value(E_PARAM_CDMA_RLPRATE_RLP_FWD_RETR_RATE);
			}
			else if( TEVENT_SYSTEM_EVDO == p->System )
			{
				CountParam_Value(E_PARAM_EVDO_RLPRATE_RLP_REV_RATE);
				CountParam_Value(E_PARAM_EVDO_RLPRATE_RLP_FOR_RATE);
				CountParam_Value(E_PARAM_EVDO_RLPRATE_RLP_REV_RETR_RATE);
				CountParam_Value(E_PARAM_EVDO_RLPRATE_RLP_FWD_RETR_RATE);
			}
		}
		break;
	case TEVENT_ID_TXPC:
		{
			TEVENT_TXPC_STR *p = (TEVENT_TXPC_STR *)pBuf;
			if( TEVENT_SYSTEM_UMTSTD_SCDMA == p->System )
			{
				CountParam_Value(E_PARAM_TD_TX_POWER);
				CountParam_Value(E_PARAM_TD_TCP_STEP_SIZE);
			}
			else if( TEVENT_SYSTEM_UMTSFDD == p->System )
			{
				CountParam_Value(E_PARAM_W_TX_POWER);
			}
			else if ( TEVENT_SYSTEM_CDMAONE == p->System || TEVENT_SYSTEM_CDMA1X == p->System )
			{
				CountParam_Value(E_PARAM_CDMA_TXPC_TXPOWER);
				CountParam_Value(E_PARAM_CDMA_TXPC_TXADJUST);
				CountParam_Value(E_PARAM_CDMA_TXPC_TXPWRLIMIT);
				CountParam_Value(E_PARAM_CDMA_TXPC_R_FCH);
				CountParam_Value(E_PARAM_CDMA_TXPC_R_SCH0);
				CountParam_Value(E_PARAM_CDMA_TXPC_R_SCH1);
				CountParam_Value(E_PARAM_CDMA_TXPC_R_DCCH);
			}
			else if ( TEVENT_SYSTEM_EVDO == p->System )
			{
				CountParam_Value(E_PARAM_EVDO_TXPC_TX_POWER);
				CountParam_Value(E_PARAM_EVDO_TXPC_TX_ADJUST);
				CountParam_Value(E_PARAM_EVDO_TXPC_TX_PILOT);
				CountParam_Value(E_PARAM_EVDO_TXPC_TX_OPEN_LOOP_POWER);
				CountParam_Value(E_PARAM_EVDO_TXPC_TX_MAX_POWER_USAGE);
				CountParam_Value(E_PARAM_EVDO_TXPC_TX_MIN_POWER_USAGE);
			}
		}
		break;
	case TEVENT_ID_RXPC:
		{
			TEVENT_RXPC_STR *p = (TEVENT_RXPC_STR *)pBuf;
			if( TEVENT_SYSTEM_UMTSTD_SCDMA == p->System )
			{
				CountParam_Value(E_PARAM_TD_SIR);
				CountParam_Value(E_PARAM_TD_SIR_TARGET);
				TEVENT_RXPC_UMTSTDSCDMA_STR *pPacket = (TEVENT_RXPC_UMTSTDSCDMA_STR *)p->Pad;
				TEVENT_RXPC_UMTSTDSCDMA_PARAMSTIMESLOT_STR *pCell = NULL;
				for( unsigned int i = 0; i< MAX_CELL_CNT; ++i )
				{
					if( i<pPacket->nTimeslots )
					{
						pCell = (TEVENT_RXPC_UMTSTDSCDMA_PARAMSTIMESLOT_STR *)( pPacket->PARAMSTIMESLOT + sizeof(TEVENT_RXPC_UMTSTDSCDMA_PARAMSTIMESLOT_STR)*i );
						CountParam_Value( ( E_PARAM_TD_DPCH_RSCP + i));				
					}
					else
					{
						CountParam_Value( ( E_PARAM_TD_DPCH_RSCP + i));				
					}
				}
			}
			else if( TEVENT_SYSTEM_UMTSFDD == p->System )
			{
				CountParam_Value(E_PARAM_W_SIR);		
				CountParam_Value(E_PARAM_W_SIR_TARGET);		
				CountParam_Value(E_PARAM_W_DLPWRUP);
				CountParam_Value(E_PARAM_W_BSDIVSTATE);
				CountParam_Value(E_PARAM_W_DLPWRUP_CND);
				CountParam_Value(E_PARAM_W_DLPWRDOWN_CND);
				CountParam_Value(E_PARAM_W_DPC_MODE);
			}
		}
		break;
	case TEVENT_ID_RLCBLER:
		{
			TEVENT_RLCBLER_STR *p = (TEVENT_RLCBLER_STR *)pBuf;
			if( TEVENT_SYSTEM_GSM == p->System )
			{
				CountParam_Value(E_PARAM_GSM_BLER);
			}	
			else if( TEVENT_SYSTEM_UMTSTD_SCDMA == p->System )
			{
				CountParam_Value(E_PARAM_TD_BLER);
				m_pParamList[E_PARAM_TD_TRCHI].nMaxCnt += 1;
			}
			else if( TEVENT_SYSTEM_UMTSFDD == p->System )
			{
				CountParam_Value(E_PARAM_W_BLER);
				m_pParamList[E_PARAM_W_TRCHI].nMaxCnt += 1;
			}
		}
		break;
	case TEVENT_ID_RACHI:
		{
			TEVENT_RACHI_STR *p = (TEVENT_RACHI_STR *)pBuf;
			if( TEVENT_SYSTEM_UMTSTD_SCDMA == p->System )
			{
				CountParam_Value(E_PARAM_TD_PCCPCH_PATHLOSS);
			}
			else if( TEVENT_SYSTEM_UMTSFDD == p->System )
			{
				CountParam_Value(E_PARAM_W_INIT_PWR);
				CountParam_Value(E_PARAM_W_PWR_RAMP_STEP);
				CountParam_Value(E_PARAM_W_RACH_TX_PWR);
				CountParam_Value(E_PARAM_W_UL_INTERF);
				CountParam_Value(E_PARAM_W_PWR_OFFSET);
				CountParam_Value(E_PARAM_W_PRE_CNT);
				CountParam_Value(E_PARAM_W_MAX_PREEMBLE);
				CountParam_Value(E_PARAM_W_AICH_STATUS);
				CountParam_Value(E_PARAM_W_DATA_GAIN);
				CountParam_Value(E_PARAM_W_CTRL_GAIN);
				CountParam_Value(E_PARAM_W_MSG_LENGTH);
				CountParam_Value(E_PARAM_W_PRE_CYCLES);
			}
			else if ( TEVENT_SYSTEM_CDMA1X == p->System || TEVENT_SYSTEM_CDMAONE == p->System )
			{
				CountParam_Value(E_PARAM_CDMA_RACHI_NOM_PWR);
				CountParam_Value(E_PARAM_CDMA_RACHI_INIT_PWR);
				CountParam_Value(E_PARAM_CDMA_RACHI_PWR_STEP);
				CountParam_Value(E_PARAM_CDMA_RACHI_NUM_STEP);
				CountParam_Value(E_PARAM_CDMA_RACHI_PROBECOUNT);
				CountParam_Value(E_PARAM_CDMA_RACHI_PROBESEQCOUNT);
				CountParam_Value(E_PARAM_CDMA_RACHI_ACCESSCHNUMBER);
				CountParam_Value(E_PARAM_CDMA_RACHI_RANDOMDELAY);
				CountParam_Value(E_PARAM_CDMA_RACHI_TXLEVEL);
				CountParam_Value(E_PARAM_CDMA_RACHI_ACCESSRXLEVEL);
				CountParam_Value(E_PARAM_CDMA_RACHI_ACCESSTXADJ);
				CountParam_Value(E_PARAM_CDMA_RACHI_PSIST);
				CountParam_Value(E_PARAM_CDMA_RACHI_SEQBACKOFF);
				CountParam_Value(E_PARAM_CDMA_RACHI_PROBBACKOFF);
				CountParam_Value(E_PARAM_CDMA_RACHI_INTERCORR);
			}
			else if ( TEVENT_SYSTEM_EVDO == p->System )
			{
				CountParam_Value(E_PARAM_EVDO_RACHI_MaxProbeseqs);
				CountParam_Value(E_PARAM_EVDO_RACHI_Duration);
				CountParam_Value(E_PARAM_EVDO_RACHI_AccessPN);
				CountParam_Value(E_PARAM_EVDO_RACHI_Accesssectorid);
				CountParam_Value(E_PARAM_EVDO_RACHI_MaxProbes);
				CountParam_Value(E_PARAM_EVDO_RACHI_Result);
				CountParam_Value(E_PARAM_EVDO_RACHI_nProbes);
				CountParam_Value(E_PARAM_EVDO_RACHI_nProbeseqs);
				CountParam_Value(E_PARAM_EVDO_RACHI_Accesscolorcode);//13683386622
			}
		}
		break;
	case TEVENT_ID_GPS:
		{
			CountParam_Value( E_PARAM_GPS_LAT);
			CountParam_Value( E_PARAM_GPS_LON);
			CountParam_Value( E_PARAM_GPS_DISTANCE);
			CountParam_Value( E_PARAM_GPS_FIX);
			CountParam_Value( E_PARAM_GPS_VELOCITY);
			CountParam_Value( E_PARAM_GPS_SATELLITES);
			if (m_gLogInfo.fLon <= 0.1 || m_gLogInfo.fLat <= 0.1)
			{
				TEVENT_GPS_STR *p = (TEVENT_GPS_STR*)pBuf;
				m_gLogInfo.fLat   =	GetDOUBLE(p->Lat);
				m_gLogInfo.fLon	  = GetDOUBLE(p->Lon);
			}
		}
		break;
	case TEVENT_ID_INDOORMARK:
		{
			CountParam_Value(E_PARAM_GPS_LAT_A);
			CountParam_Value(E_PARAM_GPS_LON_A);
			m_gLogInfo.bIsIndoor = true;
		}
		break;
	case TEVENT_ID_AQUL:
		{
			if (m_CurSystem == TEVENT_SYSTEM_GSM)
			{
				CountParam_Value(E_PARAM_GSM_MOS);
			}else if (m_CurSystem == TEVENT_SYSTEM_UMTSFDD)
			{
				CountParam_Value(E_PARAM_W_MOS);
			}
		}
		break;
	case TEVENT_ID_AQDL:
		{
			if (m_CurSystem == TEVENT_SYSTEM_GSM)
			{
				CountParam_Value(E_PARAM_GSM_MOS);
			}else if (m_CurSystem == TEVENT_SYSTEM_UMTSFDD)
			{
				CountParam_Value(E_PARAM_W_MOS);
			}
		}
		break;
	case TEVENT_ID_CMPEV:
		{
			TEVENT_CMPEV_STR* pCmpeasStr = (TEVENT_CMPEV_STR*)pBuf;
			m_CurSystem = pCmpeasStr->system;
			SetNetTpye(pCmpeasStr->system);
			if (m_complaintEventMatrix.nRealCnt < T_COMPLAINT_EVENT_MATRIX::MAX_COMPLAINT_CNT)
			{
				if( TEVENT_SYSTEM_GSM == pCmpeasStr->system)
				{
					TEVENT_CMPEV_GSM_STR *pCellPacket = (TEVENT_CMPEV_GSM_STR *)pCmpeasStr->pad;
					m_complaintEventMatrix.pCmpalintList[m_complaintEventMatrix.nRealCnt].iCellCnt = pCellPacket->nCells;
				}else if (TEVENT_SYSTEM_UMTSFDD == pCmpeasStr->system)
				{
					TEVENT_CMPEV_UTMSFDD_STR *pCellPacket = (TEVENT_CMPEV_UTMSFDD_STR*)pCmpeasStr->pad;
					m_complaintEventMatrix.pCmpalintList[m_complaintEventMatrix.nRealCnt].iCellCnt = pCellPacket->nCells;
				}
				m_complaintEventMatrix.nRealCnt++;
			}
		}
		break;
	case TEVENT_ID_CMPMEAS:
		{
			TEVENT_CMPMEAS_STR* pcmeasStr = (TEVENT_CMPMEAS_STR*)pBuf;
			CountParam_Value( E_PARAM_GPS_LAT);
			CountParam_Value( E_PARAM_GPS_LON);
			m_CurSystem = pcmeasStr->system;
			SetNetTpye(pcmeasStr->system);
			if (TEVENT_SYSTEM_GSM == pcmeasStr->system)
			{
				CountParam_Value(E_PARAM_GSM_MNC);
				CountParam_Value(E_PARAM_GSM_MCC);
				CountParam_Value(E_PARAM_GSM_LAC);
				CountParam_Value(E_PARAM_GSM_CI);
				CountParam_Value(E_PARAM_GSM_RXLEV_SUB);
				TEVENT_CMPMEAS_GSM_STR* pGSMcmeasStr = (TEVENT_CMPMEAS_GSM_STR*)pcmeasStr->pad;
				if(pGSMcmeasStr->nCells > MAX_CELLMEAS_NUM_CELLS) return;
				m_pParamList[E_PARAM_GSM_COMPLANIT_CELLS].nMaxCnt += 1;
				if ((m_gLogInfo.fLon <= 0.1) || (m_gLogInfo.fLat <= 0.1))
				{
					m_gLogInfo.fLat   =	GetFLOAT(pGSMcmeasStr->lat);
					m_gLogInfo.fLon	  = GetFLOAT(pGSMcmeasStr->lon);
				}
			}else if (TEVENT_SYSTEM_UMTSFDD == pcmeasStr->system)
			{
				CountParam_Value(E_PARAM_W_SERV_CI);
				CountParam_Value(E_PARAM_W_SERV_LAC);
				CountParam_Value(E_PARAM_W_SERV_MCC);
				CountParam_Value(E_PARAM_W_SERV_MNC);
				CountParam_Value(E_PARAM_W_SERV_SCR);
				CountParam_Value(E_PARAM_W_CPICH_RSCP);
				CountParam_Value(E_PARAM_W_SERV_EC_NO);
				TEVENT_CMPMEAS_UTMSFDD_STR* pWcdmaStr = (TEVENT_CMPMEAS_UTMSFDD_STR*)pcmeasStr->pad;
				if(pWcdmaStr->nCells > MAX_CELLMEAS_NUM_CELLS) return;
				m_pParamList[E_PARAM_W_COMPLANIT_CELLS].nMaxCnt += 1;
				if ((m_gLogInfo.fLon <= 0.1) || (m_gLogInfo.fLat <= 0.1))
				{
					m_gLogInfo.fLat   =	GetFLOAT(pWcdmaStr->lat);
					m_gLogInfo.fLon	  = GetFLOAT(pWcdmaStr->lon);
				}
			}
		}
		break;
	case TEVENT_ID_USRBHV:
		{
			TEVENT_USRBHV_STR* pHsrbhv = (TEVENT_USRBHV_STR*)pBuf;
			m_CurSystem = pHsrbhv->system;
			SetNetTpye(pHsrbhv->system);
			if (m_usrbhvEventMatrix.nRealCnt < T_USRBHV_EVENT_MATRIX::MAX_USRBHV_CNT)
			{
				m_usrbhvEventMatrix.nRealCnt++;
			}
		}
		break;
	}
}


void CEventDecode::ReleaseParamList()
{
	int i = 0;
	//release message matrix-------------------------------------------------------------------<<
	if( NULL != m_MsgMatrix.pList )
	{
		for( i=0; i<(int)m_MsgMatrix.nRealCnt; ++i )
		{
			SafeDeleteArray(m_MsgMatrix.pList[i].pName);
			SafeDeleteArray(m_MsgMatrix.pList[i].pSubChannelName);
			SafeDeleteArray(m_MsgMatrix.pList[i].pRawData);
		}
		SafeDeleteArray(m_MsgMatrix.pList);		
	}
	m_MsgMatrix.iIndex = 0;
	m_MsgMatrix.nRealCnt = 0;

	//=========================================================================================<<
	//release event matrix---------------------------------------------------------------------<<
	if( NULL != m_gEventMatrix.pList )
	{
		SafeDeleteArray(m_gEventMatrix.pList);
	}

	if (NULL != m_gEventMatrix.pUnsualEventList)
	{
		SafeDeleteArray(m_gEventMatrix.pUnsualEventList);
	}
	m_gEventMatrix.nUnsualEventCnt = 0;
	m_gEventMatrix.iIndex          = 0;
	m_gEventMatrix.nRealCnt        = 0;
	//release parameter(s) buffer--------------------------------------------------------------<<
	ReleaseBuffer(E_PARAM_TIME_LINE, E_PARAM_THROUGHT_RLC_UL_RETRANSMISSION);

	if(m_gLogInfo.bGSM)
	{
		ReleaseBuffer(E_PARAM_GSM_BAND, E_PARAM_GSM_CELL_NAME);
	}
	if(m_gLogInfo.bTDSCDMA)
	{
		ReleaseBuffer(E_PARAM_TD_SERV_UARFCN, E_PARAM_TD_TRCHI_ERRORS_CNT);
	}

	if (m_gLogInfo.bWCDMA)
	{
		ReleaseBuffer(E_PARAM_W_SERV_UARFCN, E_PARAM_W_TRCHI_ERRORS_CNT);
	}

	if (m_gLogInfo.bCDMA)
	{
		ReleaseBuffer(E_PARAM_CDMA_SERV_BAND, E_PARAM_CDMA_RACHI_INTERCORR);
	}

	if (m_gLogInfo.bEVDO)
	{
		ReleaseBuffer(E_PARAM_EVDO_SERV_BAND, E_PARAM_EVDO_RACHI_Accesscolorcode);
	}
	//=========================================================================================<<
	//reset log information--------------------------------------------------------------------<<
	memset(&m_gLogInfo, 0, sizeof(T_LOG_INFO));
	//=========================================================================================<<

	//=========================================================================================<<
	//reset complaint information--------------------------------------------------------------------<<
	memset(&m_complaintEventMatrix, 0, sizeof(m_complaintEventMatrix));
	//=========================================================================================<<
	//===========================================================================================<<
	//relase complaintEventMatrix
	m_usrbhvEventMatrix.nRealCnt = 0;
	memset(&m_usrbhvEventMatrix, 0, sizeof(m_usrbhvEventMatrix));
	//=========================================================================================<<
	
}

////////
void CEventDecode::ReleaseBuffer(unsigned int nStart, unsigned int nEnd)
{
	for (int i=nStart; i<= nEnd; i++)
	{
		if (m_pParamList[i].pData != NULL)
		{
			if ((m_pParamList[i].nDataType == E_PARAM_TYPE_INT)|| (E_PARAM_TYPE_FLOAT == m_pParamList[i].nDataType) || (E_PARAM_TYPE_TIME == m_pParamList[i].nDataType))
			{
				SafeDeleteArray(m_pParamList[i].pData);
				m_pParamList[i].nMaxCnt = 0;
				m_pParamList[i].nTail   = 0;
			}else if(( E_PARAM_TYPE_GSM_CELL    == m_pParamList[i].nDataType )||
				( E_PARAM_TYPE_TD_CHANNEL  == m_pParamList[i].nDataType )||
				( E_PARAM_TYPE_TD_CELL     == m_pParamList[i].nDataType )||
				( E_PARAM_TYPE_TD_TIMESLOT == m_pParamList[i].nDataType )||
				( E_PARAM_TYPE_TD_TRCHI    == m_pParamList[i].nDataType )||
				( E_PARAM_TYPE_W_CHANNEL   == m_pParamList[i].nDataType )||
				( E_PARAM_TYPE_W_CELL      == m_pParamList[i].nDataType )||
				( E_PARAM_TYPE_W_TRCHI     == m_pParamList[i].nDataType )||
				( E_PARAM_TYPE_CDMA_CHANNEL== m_pParamList[i].nDataType )||
				( E_PARAM_TYPE_CDMA_CELL   == m_pParamList[i].nDataType )||
				( E_PARAM_TYPE_EVDO_CHANNEL== m_pParamList[i].nDataType )||
				( E_PARAM_TYPE_EVDO_CELL   == m_pParamList[i].nDataType )||
				( E_PARAM_TYPE_W_CQI       == m_pParamList[i].nDataType )||
				( E_PARAM_TYPE_GSM_COMPLAINT_CELL == m_pParamList[i].nDataType)||
				( E_PARAM_TYPE_W_COMPLAINT_CELL == m_pParamList[i].nDataType)
				)
			{
				T_PARAM_MATRIX *p = (T_PARAM_MATRIX *)m_pParamList[i].pData;
				for (int n=0; n<m_pParamList[i].nMaxCnt; n++)
				{
					if (NULL != p[n].pList)
					{
						if (p[n].nCnt <=1)
						{
							delete p[n].pList;
							p[n].pList = NULL;
						}else
						{
							SafeDeleteArray(p[n].pList);
						}
					}
				}
				SafeDeleteArray(m_pParamList[i].pData);
				m_pParamList[i].nMaxCnt = 0;
				m_pParamList[i].nTail    = 0;
			}
		}
	}
}

//--------------------------------------------------------------------------------------------------------------------------------------------<<
//
//--------------------------------------------------------------------------------------------------------------------------------------------<<
int CEventDecode::CreateParamList()
{
	//----------------------------------------------------------------------------------------------------------------------------------------<<
	//create message matrix
	//----------------------------------------------------------------------------------------------------------------------------------------<<
	if(m_MsgMatrix.nRealCnt>0)
	{
		m_MsgMatrix.pList = new T_PARAM_MESSAGE_ITEM[m_MsgMatrix.nRealCnt];
		if( NULL == m_MsgMatrix.pList )
		{
			return E_RESULT_MEMORY_FAILURE;
		}
		memset(m_MsgMatrix.pList, 0, sizeof(T_PARAM_MESSAGE_ITEM)*m_MsgMatrix.nRealCnt);
		m_MsgMatrix.iIndex = 0;		
	}

	//----------------------------------------------------------------------------------------------------------------------------------------<<
	//create Event matrix
	//----------------------------------------------------------------------------------------------------------------------------------------<<
	if(m_gEventMatrix.nRealCnt>0)
	{
		m_gEventMatrix.pList = new T_PARAM_EVENT_ITEM[m_gEventMatrix.nRealCnt];
		if( NULL == m_gEventMatrix.pList )
		{
			return E_RESULT_MEMORY_FAILURE;
		}

		memset(m_gEventMatrix.pList, 0, sizeof(T_PARAM_EVENT_ITEM)*m_gEventMatrix.nRealCnt);
		m_gEventMatrix.iIndex = 0;
	}	
	//----------------------------------------------------------------------------------------------------------------------------------------<<
	// param list
	//----------------------------------------------------------------------------------------------------------------------------------------<<
	m_pParamList[E_PARAM_TIME_LINE].nMaxCnt = m_gLogInfo.nTimePointCnt;
	m_pParamList[E_PARAM_SYSTEM].nMaxCnt    = m_gLogInfo.nTimePointCnt;//
	m_pParamList[E_PARAM_SERVER_STATE].nMaxCnt = m_gLogInfo.nTimePointCnt;
	m_pParamList[E_PARAM_MESSAGE].nMaxCnt   = m_MsgMatrix.nRealCnt;
	m_pParamList[E_PARAM_EVENT].nMaxCnt     = m_gEventMatrix.nRealCnt;
	if (!m_gLogInfo.bIsIndoor)
	{
		m_pParamList[E_PARAM_GPS_LAT_A].nMaxCnt = m_pParamList[E_PARAM_GPS_LAT].nMaxCnt;
		m_pParamList[E_PARAM_GPS_LON_A].nMaxCnt = m_pParamList[E_PARAM_GPS_LON].nMaxCnt;
	}
	CreateSystem( E_PARAM_TIME_LINE, E_PARAM_THROUGHT_RLC_UL_RETRANSMISSION);
	CreateSystem(E_PARAM_GSM_BAND, E_PARAM_GSM_CELL_NAME);
	CreateSystem(E_PARAM_W_SERV_UARFCN,  E_PARAM_W_TRCHI_ERRORS_CNT);

	if(m_gLogInfo.bTDSCDMA)
	{
		CreateSystem(E_PARAM_TD_SERV_UARFCN,  E_PARAM_TD_TRCHI_ERRORS_CNT);
	}

	if (m_gLogInfo.bCDMA )
	{
		CreateSystem(E_PARAM_CDMA_SERV_BAND,  E_PARAM_CDMA_RACHI_INTERCORR);
	}
	if ( m_gLogInfo.bEVDO )
	{
		CreateSystem(E_PARAM_EVDO_SERV_BAND,  E_PARAM_EVDO_RACHI_Accesscolorcode);
	}
	return E_RESULT_OK;
}

//--------------------------------------------------------------------------------------------------------------------------------------------<<
//
//--------------------------------------------------------------------------------------------------------------------------------------------<<
bool CEventDecode::CreateSystem(unsigned int nStart, unsigned int nEnd)
{
	unsigned int i = 0;
	for( i=nStart; i<=nEnd; ++i)
	{
		CreateNodeBuf(i);
	}
	return true;
}

void CEventDecode::PushParam_Value_float(unsigned int nParamID, unsigned int nIndex, float fValue)
{
	T_PARAM_DATA_FLOAT *p = (T_PARAM_DATA_FLOAT *)m_pParamList[nParamID].pData;
	if ((E_PARAM_TYPE_FLOAT != m_pParamList[nParamID].nDataType)||(p== NULL))
	{
		return;
	}
	if( (fValue<m_pParamList[nParamID].iMin) || (fValue>m_pParamList[nParamID].iMax) )
	{
		return;
	}
	int nParamIndex = m_pParamList[nParamID].nTail;
	if(nParamIndex < m_pParamList[nParamID].nMaxCnt)
	{
		p[nParamIndex].fVale         = fValue;
		p[nParamIndex].iCurTimeIndex = nIndex;
		m_pParamList[nParamID].nTail += 1;
	}
}

void CEventDecode::PushParam_WithRealRange(unsigned int nParamID, unsigned int nIndex, int iValue)
{
	if( m_pParamList[nParamID].iRealMax < iValue ) m_pParamList[nParamID].iRealMax = iValue;
	PushParam_Value_int( nParamID, nIndex, iValue);
}

void CEventDecode::PushParam_Value_int(unsigned int nParamID, unsigned int nIndex, int iValue)
{
	T_PARAM_DATA_INT *p = (T_PARAM_DATA_INT *)m_pParamList[nParamID].pData;
	if ((E_PARAM_TYPE_INT != m_pParamList[nParamID].nDataType)||(NULL== p))
	{
		return;
	}
	if( (iValue<m_pParamList[nParamID].iMin) || (iValue>m_pParamList[nParamID].iMax) )
	{
		return;
	}
	int nParamIndex = m_pParamList[nParamID].nTail;
	if(nParamIndex<m_pParamList[nParamID].nMaxCnt)
	{
		p[nParamIndex].iValue        = iValue;
		p[nParamIndex].iCurTimeIndex = nIndex;
		m_pParamList[nParamID].nTail += 1;
	}
}


char *CEventDecode::PushMemory_String(char *pDec, char *pScr )
{
	if( NULL == pScr ) return NULL;

	int iLen = strlen(pScr);
	SafeDeleteArray(pDec);

	pDec = new char[iLen+1];
	memset(pDec, 0, iLen+1);

	memcpy(pDec, pScr, iLen);
	return pDec;
}

char CEventDecode::GetCharHexValue( char c )
{
	if( c >= '0' && c <='9' )
	{
		c -= '0';
	}
	else if( c >= 'a' && c <= 'z' )
	{
		c -= 'a';
		c += 10;
	}
	else if( c >= 'A' && c <= 'Z' )
	{
		c -= 'A';
		c += 10;
	}
	else
	{
		c = 0;
	}

	return c;
}

unsigned char CEventDecode::String2Hex( char *pScr, unsigned int nBitLen )
{
	return (GetCharHexValue(pScr[0])*16 + GetCharHexValue(pScr[1]));
}

unsigned char *CEventDecode::PushMemory_RawData(unsigned char *pDec, char *pScr, unsigned int &nMsgLen )
{
	if( NULL == pScr ) return NULL;

	int iLen = strlen(pScr);
	SafeDeleteArray(pDec);

	unsigned int nBitLen = 2;
	nMsgLen = (iLen/nBitLen);
	pDec = new unsigned char[nMsgLen+4];
	memset(pDec, 0, nMsgLen+4);

	for(unsigned int i=0; i<nMsgLen; ++i)
	{
		pDec[i] = String2Hex(pScr+i*nBitLen, nBitLen);
	}
	return pDec;
}

char CEventDecode::TranslateMsgDecodeType( unsigned char &eMsgType, unsigned int nEventID, unsigned int nRawDataLen, unsigned char nDirection, char *szMessageName, char *szSubChannelName )
{
	char type = -1;
	char *pMsgName   = sfl_strupr(szMessageName);
	char *pSubChName = sfl_strupr(szSubChannelName);

	if ( nEventID == TEVENT_ID_L3SM )
	{
		if( 0 == strcmp(pMsgName, "CHANNEL_REQUEST") )
		{
			type = DECODE_GSM_CHANNEL_REQUEST;
		}
		else
		{
			type = DECODE_GSM_OTHER_MSG;
		}
		eMsgType = E_MESSAGE_TYPE_L3;
		return type;
	}
	else if ( nEventID == TEVENT_ID_L2SM )
	{       
		eMsgType = E_MESSAGE_TYPE_L2;
		return DECODE_GSM_L2_MSG;
	}
	else if ( nEventID == TEVENT_ID_MACSM )
	{
		if( 0 == strcmp(pMsgName, "PACKET_CHANNE_REQUEST") )
		{
			if (2 == nRawDataLen)
				type = DECODE_PACKET_CHANNE_REQUEST_8BIT;

			if (4 == nRawDataLen)
				type = DECODE_PACKET_CHANNE_REQUEST_11BIT;            
		}
		else if( 0 == strcmp(pMsgName, "EGPRS_PACKET_CHANNEL_REQUEST") )
		{
			type = DECODE_EGPRS_PACKET_CHANNEL_REQUEST_11BIT;
		}
		else
		{
			if (2 == nRawDataLen)
				type = DECODE_PACKET_CTRL_ACK_8BIT;

			if (4 == nRawDataLen)
				type = DECODE_PACKET_CTRL_ACK_11BIT;   
		}

		if( -1 == type )
		{
			if( 0 == strcmp(pSubChName, "PACCH") )
			{
				type = DECODE_RLC_MAC_MSG_WITH_HEADER;
			}
			else if( 0 == strcmp(pSubChName, "PCCCH") )
			{
				type = DECODE_RLC_MAC_MSG_WITHOUT_HEADER;
			}
		}

		eMsgType = E_MESSAGE_TYPE_MAC;
		return type;
	}
	else if( nEventID == TEVENT_ID_RRCSM )
	{
		if( 0 == strcmp(pMsgName, "HANDOVER_TO_UTRAN_COMMAND") )
		{
			type = DECODE_RRC_HO_TO_CMD;
		}
		else if( 0 == strcmp(pMsgName, "MASTER_INFORMATION_BLOCK") )
		{
			type = DECODE_RRC_MIB;
		}
		else if( 0 == strcmp(pMsgName, "SYSTEM_INFORMATION_BLOCK_TYPE_1") )
		{
			type = DECODE_RRC_SIB1;
		}
		else if( 0 == strcmp(pMsgName, "SYSTEM_INFORMATION_BLOCK_TYPE_2") )
		{
			type = DECODE_RRC_SIB2;
		}
		else if( 0 == strcmp(pMsgName, "SYSTEM_INFORMATION_BLOCK_TYPE_3") )
		{
			type = DECODE_RRC_SIB3;
		}
		else if( 0 == strcmp(pMsgName, "SYSTEM_INFORMATION_BLOCK_TYPE_4") )
		{
			type = DECODE_RRC_SIB4;
		}
		else if( 0 == strcmp(pMsgName, "SYSTEM_INFORMATION_BLOCK_TYPE_5") )
		{
			type = DECODE_RRC_SIB5;
		}
		else if( 0 == strcmp(pMsgName, "SYSTEM_INFORMATION_BLOCK_TYPE_6") )
		{
			type = DECODE_RRC_SIB6;
		}
		else if( 0 == strcmp(pMsgName, "SYSTEM_INFORMATION_BLOCK_TYPE_7") )
		{
			type = DECODE_RRC_SIB7;
		}
		else if( 0 == strcmp(pMsgName, "SYSTEM_INFORMATION_BLOCK_TYPE_11") )
		{
			type = DECODE_RRC_SIB11;
		}
		else if( 0 == strcmp(pMsgName, "SYSTEM_INFORMATION_BLOCK_TYPE_12") )
		{
			type = DECODE_RRC_SIB12;
		}
		else if( 0 == strcmp(pMsgName, "SYSTEM_INFORMATION_BLOCK_TYPE_18") )
		{
			type = DECODE_RRC_SIB18;
		}
		else if( 0 == strcmp(pMsgName, "SCHEDULING_BLOCK_1") )
		{
			type = DECODE_RRC_SB1;
		}
		else if( 0 == strcmp(pMsgName, "SCHEDULING_BLOCK_2") )
		{
			type = DECODE_RRC_SB2;
		}
		else if( 0 == strcmp(pMsgName, "SYSTEM_INFORMATION_BLOCK_TYPE_15_5") )
		{
			type = DECODE_RRC_SIB15_5;
		}
		else
		{
			if( TEVENT_L3SM_LAYER3DIRECTION_DOWNLINK == nDirection )
			{
				if( 0 == strcmp(pSubChName, "DCCH") )
				{
					type = DECODE_RRC_DL_DCCH;
				}
				else if( 0 == strcmp(pSubChName, "CCCH") )
				{
					type = DECODE_RRC_DL_CCCH;
				}
				else if( 0 == strcmp(pSubChName, "SHCCH") )
				{
					type = DECODE_RRC_DL_SHCCH;
				}
				else if( 0 == strcmp(pSubChName, "BCCH") )
				{
					type = DECODE_RRC_BCCH_BCH;
				}
			}
			else
			{
				if( 0 == strcmp(pSubChName, "DCCH") )
				{
					type = DECODE_RRC_UL_DCCH;
				}
				else if( 0 == strcmp(pSubChName, "CCCH") )
				{
					type = DECODE_RRC_UL_CCCH;
				}
				else if( 0 == strcmp(pSubChName, "SHCCH") )
				{
					type = DECODE_RRC_UL_SHCCH;
				}
			}
		}

		if (-1 == type)
		{
			if( 0 == strcmp(pSubChName, "PCCH") )
			{
				type = DECODE_RRC_PCCH;
			}
			else if( 0 == strcmp(pSubChName, "BCCH_FACH") )
			{
				type = DECODE_RRC_BCCH_FACH;
			}
			else if( 0 == strcmp(pSubChName, "BCCH_BCH") )
			{
				type = DECODE_RRC_BCCH_BCH;
			}
		}

		eMsgType = E_MESSAGE_TYPE_RRC;
		return type;
	}
	return type;
}

void CEventDecode::PushMessageMatrix(unsigned int nEventID, unsigned int nIndex, unsigned int nTime, E_UINT8 u8Direction, char *szMsgName, char *szMsgSubChName, char *szMsgRawData)
{
	// push message list
	if( NULL == m_MsgMatrix.pList ) return ;
	if( m_MsgMatrix.iIndex >= (int)m_MsgMatrix.nRealCnt ) return ;

	m_MsgMatrix.pList[m_MsgMatrix.iIndex].Direction = u8Direction;
	m_MsgMatrix.pList[m_MsgMatrix.iIndex].iIndex    = nIndex;
	//	gMsgMatrix.pList[gMsgMatrix.iIndex].eMsgID    = TranslateMsgID(szMsgName);
	m_MsgMatrix.pList[m_MsgMatrix.iIndex].nTime     = nTime;

	m_MsgMatrix.pList[m_MsgMatrix.iIndex].pName           = PushMemory_String(m_MsgMatrix.pList[m_MsgMatrix.iIndex].pName,           szMsgName );
	m_MsgMatrix.pList[m_MsgMatrix.iIndex].pSubChannelName = PushMemory_String(m_MsgMatrix.pList[m_MsgMatrix.iIndex].pSubChannelName, szMsgSubChName );
	m_MsgMatrix.pList[m_MsgMatrix.iIndex].pRawData        = PushMemory_RawData(m_MsgMatrix.pList[m_MsgMatrix.iIndex].pRawData,       szMsgRawData, m_MsgMatrix.pList[m_MsgMatrix.iIndex].nMsgLen );

	char bMsgType = TranslateMsgDecodeType(m_MsgMatrix.pList[m_MsgMatrix.iIndex].eMsgType, nEventID, m_MsgMatrix.pList[m_MsgMatrix.iIndex].nMsgLen, u8Direction, szMsgName, szMsgSubChName);
	m_MsgMatrix.pList[m_MsgMatrix.iIndex].Type = (-1 == bMsgType)?DECODE_UNKNOW:((unsigned char)bMsgType);

	// push param list
	T_PARAM_DATA_INT *p = (T_PARAM_DATA_INT *)m_pParamList[E_PARAM_MESSAGE].pData;
	if( NULL == p ) return ;

	int nMSGIndex = m_pParamList[E_PARAM_MESSAGE].nTail;
	p[nMSGIndex].iValue        = m_MsgMatrix.iIndex;
	p[nMSGIndex].iCurTimeIndex = nIndex;
	m_pParamList[E_PARAM_MESSAGE].nTail += 1;
	//TRACE( ">> %s : iIndex = %d nIndex = %d  Time = %02d:%02d:%02d.%03d \r\n", szMsgName, gMsgMatrix.iIndex, nIndex, TIME_GET_HOUR(nTime), TIME_GET_MIN(nTime), TIME_GET_SEC(nTime), TIME_GET_MSEC(nTime));
	//point to next
	m_MsgMatrix.iIndex++;
}
////
void CEventDecode::PushParam_RRCSM(unsigned int nEventID, unsigned int nIndex, unsigned int nTime, unsigned char *pData)
{
	TEVENT_RRCSM_STR *p = (TEVENT_RRCSM_STR *)(pData);
	char *pMsgName = p->RRCmsg;
	char *pMsgSubChannelName = p->RRCmsg + (strlen(pMsgName)) + 1;
	char *pMsgRawData = p->RRCmsg + (strlen(pMsgName)) + (strlen(pMsgSubChannelName)) + 2;
	PushMessageMatrix(nEventID, nIndex, nTime, p->Direction, pMsgName, pMsgSubChannelName, pMsgRawData);
}

void CEventDecode::PushParam_L3SM(unsigned int nEventID, unsigned int nIndex, unsigned int nTime, unsigned char *pData)
{
	TEVENT_L3SM_STR *p = (TEVENT_L3SM_STR *)pData;
	if( TEVENT_SYSTEM_GSM == p->System )
	{
		TEVENT_L3SM_GSM_STR *pL3SM = (TEVENT_L3SM_GSM_STR *)p->pad;
		char *pMsgName = pL3SM->L3msg;
		char *pMsgSubChannelName = pL3SM->L3msg + (strlen(pMsgName)) + 1;
		char *pMsgRawData = pL3SM->L3msg + (strlen(pMsgName)) + (strlen(pMsgSubChannelName)) + 2;
		PushMessageMatrix(nEventID, nIndex, nTime, pL3SM->Direction, pMsgName, pMsgSubChannelName, pMsgRawData);
	}
	else if( TEVENT_SYSTEM_UMTSFDD == p->System || TEVENT_SYSTEM_UMTSTD_SCDMA == p->System )
	{
		TEVENT_L3SM_UMTS_STR *pL3SM = (TEVENT_L3SM_UMTS_STR *)p->pad;
		char *pMsgName = pL3SM->L3msg;
		char *pMsgSubChannelName = pL3SM->L3msg + (strlen(pMsgName)) + 1;
		char *pMsgRawData = pL3SM->L3msg + (strlen(pMsgName)) + (strlen(pMsgSubChannelName)) + 2;

		PushMessageMatrix(nEventID, nIndex, nTime, pL3SM->Direction, pMsgName, pMsgSubChannelName, pMsgRawData);
	}
	else if( TEVENT_SYSTEM_CDMAONE == p->System || TEVENT_SYSTEM_CDMA1X == p->System )
	{			
		TEVENT_L3SM_CDMAONEANDCDMA1X_STR *pL3SM = (TEVENT_L3SM_CDMAONEANDCDMA1X_STR *)p->pad;
		char *pMsgName = pL3SM->L3msg;
		char *pMsgSubChannelName = pL3SM->L3msg + (strlen(pMsgName)) + 1;
		char *pMsgRawData = pL3SM->L3msg + (strlen(pMsgName)) + (strlen(pMsgSubChannelName)) + 2;

		PushMessageMatrix(nEventID, nIndex, nTime, pL3SM->Direction, pMsgName, pMsgSubChannelName, pMsgRawData);
	}
}

void CEventDecode::PushParam_MACSM(unsigned int nEventID, unsigned int nIndex, unsigned int nTime, unsigned char *pData)
{
	TEVENT_MACSM_STR *p = (TEVENT_MACSM_STR *)pData;
	if( TEVENT_SYSTEM_GSM == p->System )
	{
		TEVENT_MACSM_GSM_STR *pMACSM = (TEVENT_MACSM_GSM_STR *)p->append;
		char *pMsgName = pMACSM->RLCMACmsg;
		char *pMsgSubChannelName = pMACSM->RLCMACmsg + (strlen(pMsgName)) + 1;
		char *pMsgRawData = pMACSM->RLCMACmsg + (strlen(pMsgName)) + (strlen(pMsgSubChannelName)) + 2;			
		PushMessageMatrix(nEventID, nIndex, nTime, pMACSM->Direction, pMsgName, pMsgSubChannelName, pMsgRawData);
	}
}

void CEventDecode::PushParam_SNPSM(unsigned int nEventID, unsigned int nIndex, unsigned int nTime, unsigned char *pData)
{
	TEVENT_SNPSM_EVDO_STR *p = (TEVENT_SNPSM_EVDO_STR *)pData;

	char *pMsgName = (char *)&(p->append) + 1;
	char *pMsgSubChannelName = (char *)&(p->append) + 1 + (strlen(pMsgName)) + 1;
	char *pSNPLayer= (char *)&(p->append) + 1 + (strlen(pMsgName)) + (strlen(pMsgSubChannelName)) + 2;
	char *pMsgRawData = (char *)&(p->append) + 1 + (strlen(pMsgName)) + (strlen(pMsgSubChannelName)) + (strlen(pSNPLayer)) + 3;		
	PushMessageMatrix(nEventID, nIndex, nTime, p->Direction, pMsgName, pMsgSubChannelName, pMsgRawData);
}

inline void CEventDecode::SetNetTpye(unsigned char chValue)
{
	switch (chValue)
	{
	case TEVENT_SYSTEM_GSM:
		{
			m_chNetFlag |= EM_GSM_NET_TYPE;
		}
		break;
	case TEVENT_SYSTEM_UMTSTD_SCDMA:
		{
			m_chNetFlag |= EM_TD_NET_TYPE;
		}
		break;
	case TEVENT_SYSTEM_UMTSFDD:
		{
			m_chNetFlag |= EM_WCDMA_NET_TYPE;
		}
		break;
	case TEVENT_SYSTEM_CDMA1X:
		{
			m_chNetFlag |= EM_CDMA_NET_TYPE;
		}
		break;
	case TEVENT_SYSTEM_EVDO:
		{
			m_chNetFlag |= EM_EVDO_NET_TYPE;
		}
		break;
	default:
		break;
	}
}

EM_LOG_NET_TYPE CEventDecode::GetLogNetType(void)
{
	if (m_chNetFlag & EM_GSM_NET_TYPE)
	{
		if (m_chNetFlag & EM_WCDMA_NET_TYPE)
		{
			return EM_WCDMA_GSM_NET_TYPE;

		}else if (m_chNetFlag & EM_TD_NET_TYPE)
		{
			return EM_TD_GSM_NET_TYPE;
		}
		return EM_GSM_NET_TYPE;
	}

	if (m_chNetFlag & EM_CDMA_NET_TYPE)
	{
		if (m_chNetFlag & EM_EVDO_NET_TYPE)
		{
			return EM_EVDO_CDMA_NET_TYPE;
		}
		return EM_CDMA_NET_TYPE;
	}

	if (m_chNetFlag & EM_TD_NET_TYPE)
	{
		return EM_TD_NET_TYPE;
	}

	if (m_chNetFlag & EM_WCDMA_NET_TYPE)
	{
		return EM_WCDMA_NET_TYPE;
	}

	if (m_chNetFlag & EM_EVDO_NET_TYPE)
	{
		return EM_EVDO_NET_TYPE;
	}

	return EM_NET_TYPE_Unknow;
}

void CEventDecode::ParseLogBase(char *pPacket)
{
	if (NULL == pPacket )
	{
		return;
	}
	m_gLogInfo.iLogServiceType = EM_LOG_SERVICE_inspection;
	TEVENT_OUTDOOR_STR *pOutdoor = (TEVENT_OUTDOOR_STR *)pPacket;
	TEVENT_OUTDOOR_PARAM_STR *pOutdoorParam = (TEVENT_OUTDOOR_PARAM_STR *)pOutdoor->Pad;
	for(int i=0; i<pOutdoor->paramNum; i++)
	{
		char szValue[MAX_OUT_DOOR_LEN] = {0};
		wchar_t szBuffer[260] = {0};
		int nIndex = 0;
		int nLen   = pOutdoorParam->uParamLen;
		for (int n=0; n<nLen; )
		{
			memcpy(&(szBuffer[nIndex]), &(pOutdoorParam->Param[n]), 2);
			n += 2;
			nIndex++;
		}
		_gConverW2M(szValue,  szBuffer,  pOutdoorParam->uParamLen, sizeof(szValue));
		switch((TEVENT_OUTDOOR_PARAM_EN)pOutdoorParam->uParamType)
		{
		case TEVENT_OUTDOOR_PARAM_TEST_CLASS:
			{
				strcpy(m_gLogInfo.inspectionInfo.szTestType,  szValue);
			}
			break;
		case TEVENT_OUTDOOR_PARAM_CITY:
			{
				strcpy(m_gLogInfo.inspectionInfo.szTestCity,  szValue);
			}
			break;
		case TEVENT_OUTDOOR_PARAM_PROJECT_NUM:
			{
				strcpy(m_gLogInfo.inspectionInfo.szProjectNum,  szValue);
			}
			break;
		case TEVENT_OUTDOOR_PARAM_RAT:
			{
				strcpy(m_gLogInfo.inspectionInfo.szRat,  szValue);
			}
			break;
		case TEVENT_OUTDOOR_PARAM_DEVICE_MANUFACTURER:
			{
				strcpy(m_gLogInfo.inspectionInfo.szDeviceManufcturer,  szValue);
			}
			break;
		case TEVENT_OUTDOOR_PARAM_TEST_MANUFACTURER:
			{
				strcpy(m_gLogInfo.inspectionInfo.szTestManufacturer,  szValue);
			}
			break;
		case TEVENT_OUTDOOR_PARAM_TEST_MEMBER:
			{
				strcpy(m_gLogInfo.inspectionInfo.szTestMember,  szValue);
			}
			break;
		case TEVENT_OUTDOOR_PARAM_TEST_DOMAIN:
			{
				strcpy(m_gLogInfo.inspectionInfo.szTestDoMain,  szValue);
			}
			break;
		case TEVENT_OUTDOOR_PARAM_LABEL:
			{
				strcpy(m_gLogInfo.inspectionInfo.szLabel,  szValue);
			}
			break;
		case TEVENT_OUTDOOR_PARAM_TEST_TIME:
			{
				strcpy(m_gLogInfo.inspectionInfo.szTestTime,  szValue);
			}
			break;				
		}
		pOutdoorParam = (TEVENT_OUTDOOR_PARAM_STR *)((char*)pOutdoorParam + (pOutdoorParam->uParamLen + 2));
	}
}


void CEventDecode::InitEventItem( T_PARAM_EVENT_ITEM *pEi , unsigned int nIndex, unsigned int nTime, unsigned int nEventID, unsigned int nParam1, unsigned int nParam2 )
{
	memset(pEi, 0, sizeof(T_PARAM_EVENT_ITEM));
	pEi->iIndex    = nIndex;
	pEi->nTime     = nTime;
	pEi->nEventID  = nEventID;
	pEi->nParam1 = nParam1;
	pEi->nParam2 = nParam2;
}

void CEventDecode::PushServerState(unsigned int nIndex, int iServerState)
{
	T_PARAM_DATA_INT *p = (T_PARAM_DATA_INT *)m_pParamList[E_PARAM_SERVER_STATE].pData;
	if ( E_PARAM_TYPE_INT != m_pParamList[E_PARAM_SERVER_STATE].nDataType)
	{
		return;
	}
	int nParamIndex = m_pParamList[E_PARAM_SERVER_STATE].nTail;
	p[nParamIndex].iValue        = iServerState;
	p[nParamIndex].iCurTimeIndex = nIndex;
	m_pParamList[E_PARAM_SERVER_STATE].nTail += 1;
}

void CEventDecode::PushEventMatrix(T_PARAM_EVENT_ITEM *pEventItem)
{
	// push message list
	if( NULL == m_gEventMatrix.pList ) return ;
	if( m_gEventMatrix.iIndex >= (int)m_gEventMatrix.nRealCnt ) return ;

	memcpy(&m_gEventMatrix.pList[m_gEventMatrix.iIndex], pEventItem, sizeof(T_PARAM_EVENT_ITEM));

	// push param list
	T_PARAM_DATA_INT *p = (T_PARAM_DATA_INT *)m_pParamList[E_PARAM_EVENT].pData;
	if( NULL == p ) return ;
	int nParamIndex = m_pParamList[E_PARAM_EVENT].nTail;
	p[nParamIndex].iCurTimeIndex = pEventItem->iIndex;
	p[nParamIndex].iValue        = m_gEventMatrix.iIndex;
	m_pParamList[E_PARAM_EVENT].nTail += 1;

	//TRACE( "## %s : iIndex = %d nIndex = %d  Time = %02d:%02d:%02d.%03d \r\n", pEventItem->szEventName,gEventMatrix.iIndex, pEventItem->iIndex, TIME_GET_HOUR(pEventItem->nTime), TIME_GET_MIN(pEventItem->nTime), TIME_GET_SEC(pEventItem->nTime), TIME_GET_MSEC(pEventItem->nTime));
	m_gEventMatrix.iIndex++;
}

void CEventDecode::PushParam_DAA(unsigned int nIndex, unsigned int nTime, unsigned char *pData)
{
	TEVENT_DAA_STR *p = (TEVENT_DAA_STR *)pData;	

	T_PARAM_EVENT_ITEM ei;
	InitEventItem(&ei, nIndex, nTime, TEVENT_ID_DAA, p->Applicationprotocol, 0);
	GetApplicatiionProtocol(p->Applicationprotocol, ei.szEventName);
	strcat(ei.szEventName, " Attempt");
	//ei.nSystem = p->System ;
	ei.nParam3 = p->CallcontextID;
	ei.nParam4 = GetI32(p->Hostport);
	sprintf(ei.szRemark, "%s", p->Hostaddress);
	PushEventMatrix(&ei);
}

void CEventDecode::PushParam_DAC(unsigned int nIndex, unsigned int nTime, unsigned char *pData)
{
	TEVENT_DAC_STR *p = (TEVENT_DAC_STR *)pData;

	T_PARAM_EVENT_ITEM ei;
	InitEventItem(&ei, nIndex, nTime, TEVENT_ID_DAC, p->Applicationprotocol, 0);
	GetApplicatiionProtocol(p->Applicationprotocol, ei.szEventName);
	strcat(ei.szEventName, " Connect");
	//	ei.nSystem = p->System ;
	ei.nParam3 = p->DataconnectioncontextID;
	sprintf(ei.szRemark, "connected");
	PushEventMatrix(&ei);
}

void CEventDecode::PushParam_DAF(unsigned int nIndex, unsigned int nTime, unsigned char *pData)
{
	TEVENT_DAF_STR *p = (TEVENT_DAF_STR *)pData;

	T_PARAM_EVENT_ITEM ei;
	InitEventItem(&ei, nIndex, nTime, TEVENT_ID_DAF, p->Applicationprotocol, 0);
	GetApplicatiionProtocol(p->Applicationprotocol, ei.szEventName);
	strcat(ei.szEventName, " connection failure");
	//	ei.nSystem = p->System ;
	ei.nParam3 = p->DataconnectioncontextID;
	ei.nParam4 = GetI32(p->Datafailcause);
	ei.iParam5 = p->Datafailstatus;
	// Data connection failure status
	GetDataConnectionFailureStatus(GetI32(p->Datafailcause), ei.szRemark);

	PushEventMatrix(&ei);
}

void CEventDecode::PushParam_DAD(unsigned int nIndex, unsigned int nTime, unsigned char *pData)
{
	TEVENT_DAD_STR *p = (TEVENT_DAD_STR *)pData;

	T_PARAM_EVENT_ITEM ei;
	InitEventItem(&ei, nIndex, nTime, TEVENT_ID_DAD, p->Applicationprotocol, p->Datadiscstatus);
	GetApplicatiionProtocol(p->Applicationprotocol, ei.szEventName);
	strcat(ei.szEventName, " Disconnect");
	//	ei.nSystem = p->System ;
	ei.nParam3 = p->DataconnectioncontextID;
	// CS disconnect. status
	GetDataConnectionDisconnectStatus(p->Datadiscstatus, ei.szRemark);

	PushEventMatrix(&ei);
}

void CEventDecode::PushParam_DREQ(unsigned int nIndex, unsigned int nTime, unsigned char *pData)
{
	TEVENT_DREQ_STR *p = (TEVENT_DREQ_STR *)pData;
	PushServerState(nIndex, E_SERVER_STATE_DATA);	

	T_PARAM_EVENT_ITEM ei;
	InitEventItem(&ei, nIndex, nTime, TEVENT_ID_DREQ, p->Applicationprotocol, p->Transfdir);
	//	ei.nSystem = p->System ;
	ei.nParam3 = p->DatatransfercontextID;
	GetApplicatiionProtocol(p->Applicationprotocol, ei.szEventName);

	if( 1 == p->Transfdir) strcat(ei.szEventName, " Start UL");
	else if( 2 == p->Transfdir) strcat(ei.szEventName, " Start DL");
	else strcat(ei.szEventName, "Acknowledgment");

	// Data connection failure status
	char szValue[32] = {0};
	if( TEVENT_DCOMP_APPLICATIONPROTO_FTP == p->Applicationprotocol )
	{
		TEVENT_DREQ_FTP_STR *pPacket = (TEVENT_DREQ_FTP_STR *)p->Pad;
		ei.nParam4 = GetI32(pPacket->Filesize);
		TransferSize(szValue, ei.nParam4);
		sprintf(ei.szRemark, "File size = %s  Filename = %s", szValue, pPacket->Filename);
	}
	else if( TEVENT_DCOMP_APPLICATIONPROTO_HTTP == p->Applicationprotocol )
	{
		TEVENT_DREQ_HTTP_STR *pPacket = (TEVENT_DREQ_HTTP_STR *)p->Pad;
		ei.nParam4 = GetI32(pPacket->Filesize);
		TransferSize(szValue, ei.nParam4);
		sprintf(ei.szRemark, "File size = %s  Filename = %s", szValue, pPacket->Filename);
	}
	else if( TEVENT_DCOMP_APPLICATIONPROTO_SMTP == p->Applicationprotocol )
	{
		TEVENT_DREQ_SMTP_STR *pPacket = (TEVENT_DREQ_SMTP_STR *)p->Pad;
		ei.nParam4 = GetI32(pPacket->Filesize);
		TransferSize(szValue, ei.nParam4);
		sprintf(ei.szRemark, "File size = %s  Filename = %s", szValue, pPacket->Filename);
	}
	else if( TEVENT_DCOMP_APPLICATIONPROTO_POP3 == p->Applicationprotocol )
	{
		TEVENT_DREQ_POP3_STR *pPacket = (TEVENT_DREQ_POP3_STR *)p->Pad;
		ei.nParam4 = GetI32(pPacket->Filesize);
		TransferSize(szValue, ei.nParam4);
		sprintf(ei.szRemark, "File size = %s  Filename = %s", szValue, pPacket->Filename);
	}
	else if( TEVENT_DCOMP_APPLICATIONPROTO_MMS == p->Applicationprotocol )
	{
		TEVENT_DREQ_MMS_STR *pPacket = (TEVENT_DREQ_MMS_STR *)p->Pad;
		ei.nParam4 = GetI32(pPacket->MMSfilesize);
		TransferSize(szValue, ei.nParam4);
		sprintf(ei.szRemark, "File size = %s  Filename = %s", szValue, pPacket->MMSfilename);
	}
	else if((TEVENT_DCOMP_APPLICATIONPROTO_WAP1_0 == p->Applicationprotocol)||(TEVENT_DCOMP_APPLICATIONPROTO_WAP2_0 == p->Applicationprotocol))
	{
		TEVENT_DREQ_WAP10AND20_STR *pPacket = (TEVENT_DREQ_WAP10AND20_STR *)p->Pad;
		ei.nParam4 = GetI32(pPacket->Filesize);
		TransferSize(szValue, ei.nParam4);
		sprintf(ei.szRemark, "File size = %s  Filename = %s", szValue, pPacket->Filename);
	}
	else if( TEVENT_DCOMP_APPLICATIONPROTO_STREAMING == p->Applicationprotocol )
	{
		TEVENT_DREQ_STREAMINGPROTOCOL_STR *pPacket = (TEVENT_DREQ_STREAMINGPROTOCOL_STR *)p->Pad;
		ei.nParam4 = GetI32(pPacket->Filesize);
		TransferSize(szValue, ei.nParam4);
		sprintf(ei.szRemark, "File size = %s  Filename = %s", szValue, pPacket->Filename);
	}
	else if( TEVENT_DCOMP_APPLICATIONPROTO_HTTPBROWSING == p->Applicationprotocol )
	{
		TEVENT_DREQ_HTTPBROWSING_STR *pPacket = (TEVENT_DREQ_HTTPBROWSING_STR *)p->Pad;		
		sprintf(ei.szRemark, "URL = %s", pPacket->Filename);
	}
	else if( TEVENT_DCOMP_APPLICATIONPROTO_ICMPPING == p->Applicationprotocol )
	{
		TEVENT_DREQ_ICMPPING_STR *pPacket = (TEVENT_DREQ_ICMPPING_STR *)p->Pad;
		ei.nParam4 = GetI32(pPacket->Pingsize);
		sprintf(ei.szRemark, "size = %d bytes  rate = %d ms  Timeout = %d ms", GetI32(pPacket->Pingsize), GetI32(pPacket->Pingrate), GetI32(pPacket->Pingtimeout));
	}
	else if(TEVENT_DCOMP_APPLICATIONPROTO_IPERFOVERTCP == p->Applicationprotocol)
	{
		TEVENT_DREQ_IPERFOVERTCP_STR *pPacket = (TEVENT_DREQ_IPERFOVERTCP_STR *)p->Pad;
		ei.nParam4 = GetI32(pPacket->Datasize);
		sprintf(ei.szRemark, "Data size = %d bytes", GetI32(pPacket->Datasize));
	}
	else if(TEVENT_DCOMP_APPLICATIONPROTO_IPERFOVERUDP == p->Applicationprotocol)
	{
		TEVENT_DREQ_IPERFOVERUDP_STR *pPacket = (TEVENT_DREQ_IPERFOVERUDP_STR *)p->Pad;
		ei.nParam4 = GetI32(pPacket->Datasize);
		sprintf(ei.szRemark, "Data size = %d bytes", GetI32(pPacket->Datasize));
	}

	PushEventMatrix(&ei);
}

void CEventDecode::PushParam_HOA(unsigned int nIndex, unsigned int nTime, unsigned char *pData)
{
	TEVENT_HOA_STR *p = (TEVENT_HOA_STR *)pData;	
	T_PARAM_EVENT_ITEM ei;
	unsigned int nCurrentSys = (char)p->Pad[0];
	int			 iCurChannel = 0;
	int          iCurIdentification     = 0;
	unsigned int nAttemptSys            = 0;
	int			 iAttemptChannel        = 0;
	unsigned int iAttemptIdentification = 0;

	void * pAttemtParam = NULL;

	if( TEVENT_SYSTEM_GSM == nCurrentSys )
	{
		TEVENT_HOA_SYSTEMPARAMETER_GSM_STR * pCell = (TEVENT_HOA_SYSTEMPARAMETER_GSM_STR *)p->Pad;
		iCurChannel = GetU16(pCell->Chnumber);
		iCurIdentification = pCell->TSL;

		pAttemtParam = &( p->Pad[0]) + sizeof(TEVENT_HOA_SYSTEMPARAMETER_GSM_STR);
		nAttemptSys  = (char)(p->Pad[sizeof(TEVENT_HOA_SYSTEMPARAMETER_GSM_STR)]);
	}
	else if( TEVENT_SYSTEM_UMTSFDD == nCurrentSys )
	{
		TEVENT_HOA_SYSTEMPARAMETER_UMTSFDD_STR * pCell = (TEVENT_HOA_SYSTEMPARAMETER_UMTSFDD_STR *)p->Pad;
		iCurChannel = GetI32(pCell->Chnumber);
		iCurIdentification = GetU16(pCell->SC);
		pAttemtParam = &( p->Pad[0]) + sizeof(TEVENT_HOA_SYSTEMPARAMETER_UMTSFDD_STR);
		nAttemptSys = (char)(p->Pad[sizeof(TEVENT_HOA_SYSTEMPARAMETER_UMTSFDD_STR)]);
	}
	else if( TEVENT_SYSTEM_UMTSTD_SCDMA == nCurrentSys )
	{
		TEVENT_HOA_SYSTEMPARAMETER_UMTSTDSCDMA_STR * pCell = (TEVENT_HOA_SYSTEMPARAMETER_UMTSTDSCDMA_STR *)p->Pad;
		iCurChannel = GetU16(pCell->Chnumber);
		iCurIdentification = (pCell->CellparamsID);
		pAttemtParam = &( p->Pad[0]) + sizeof(TEVENT_HOA_SYSTEMPARAMETER_UMTSTDSCDMA_STR);
		nAttemptSys = (char)(p->Pad[sizeof(TEVENT_HOA_SYSTEMPARAMETER_UMTSTDSCDMA_STR)]);
	}
	else if( TEVENT_SYSTEM_CDMA1X == nCurrentSys || TEVENT_SYSTEM_CDMAONE == nCurrentSys || TEVENT_SYSTEM_EVDO == nCurrentSys )
	{
		TEVENT_HOA_SYSTEMPARAMETER_CDMAONECDMA1XANDEVDO_STR * pCell = (TEVENT_HOA_SYSTEMPARAMETER_CDMAONECDMA1XANDEVDO_STR *)p->Pad;
		iCurChannel = GetI32(pCell->Chnumber);

		pAttemtParam = &( p->Pad[0]) + sizeof(TEVENT_HOA_SYSTEMPARAMETER_CDMAONECDMA1XANDEVDO_STR);
		nAttemptSys = (char)(p->Pad[sizeof(TEVENT_HOA_SYSTEMPARAMETER_CDMAONECDMA1XANDEVDO_STR)]);
	}
	else
	{
		nAttemptSys = INVALID_VALUE;
	}

	if( TEVENT_SYSTEM_GSM == nAttemptSys )
	{
		TEVENT_HOA_SYSTEMPARAMETER_GSM_STR * pCell = (TEVENT_HOA_SYSTEMPARAMETER_GSM_STR *)pAttemtParam;
		iAttemptChannel        = GetU16(pCell->Chnumber);
		iAttemptIdentification = pCell->TSL;
	}
	else if( TEVENT_SYSTEM_UMTSFDD == nAttemptSys )
	{
		TEVENT_HOA_SYSTEMPARAMETER_UMTSFDD_STR * pCell = (TEVENT_HOA_SYSTEMPARAMETER_UMTSFDD_STR *)pAttemtParam;
		iAttemptChannel        = GetI32(pCell->Chnumber);
		iAttemptIdentification = GetU16(pCell->SC);
	}
	else if( TEVENT_SYSTEM_UMTSTD_SCDMA == nAttemptSys )
	{
		TEVENT_HOA_SYSTEMPARAMETER_UMTSTDSCDMA_STR * pCell = (TEVENT_HOA_SYSTEMPARAMETER_UMTSTDSCDMA_STR *)pAttemtParam;
		iAttemptChannel        = GetU16(pCell->Chnumber);
		iAttemptIdentification = (pCell->CellparamsID);
	}
	else if( TEVENT_SYSTEM_CDMA1X == nAttemptSys || TEVENT_SYSTEM_CDMAONE == nAttemptSys || TEVENT_SYSTEM_EVDO == nAttemptSys )
	{
		TEVENT_HOA_SYSTEMPARAMETER_CDMAONECDMA1XANDEVDO_STR * pCell = (TEVENT_HOA_SYSTEMPARAMETER_CDMAONECDMA1XANDEVDO_STR *)pAttemtParam;
		iAttemptChannel = GetI32(pCell->Chnumber);
	}
	else
	{
		nAttemptSys = INVALID_VALUE;
	}

	InitEventItem(&ei, nIndex, nTime, TEVENT_ID_HOA, nCurrentSys, nAttemptSys);
	sprintf(ei.szEventName, "Handover attempt");
	ei.nSystem = nCurrentSys;
	ei.nParam1 = nAttemptSys;
	ei.nParam3 = p->HandovercontextID;
	ei.nParam4 = GetU16(p->HOAtype);
	ei.iParam5 = iCurChannel;
	ei.iParam6 = iCurIdentification;
	ei.iParam7 = iAttemptChannel;
	ei.iParam8 = iAttemptIdentification;
	GetHOAType(GetU16(p->HOAtype), ei.szRemark);
	PushEventMatrix(&ei);
}

void CEventDecode::PushParam_HOS(unsigned int nIndex, unsigned int nTime, unsigned char *pData)
{
	TEVENT_HOS_STR *p = (TEVENT_HOS_STR *)pData;

	T_PARAM_EVENT_ITEM ei;
	InitEventItem(&ei, nIndex, nTime, TEVENT_ID_HOS, 0, 0);
	sprintf(ei.szEventName, "Handover success");
	//	ei.nSystem = p->System ;
	ei.nParam3 = p->HandovercontextID;
	PushEventMatrix(&ei);
}

void CEventDecode::PushParam_HOF(unsigned int nIndex, unsigned int nTime, unsigned char *pData)
{
	TEVENT_HOF_STR *p = (TEVENT_HOF_STR *)pData;

	T_PARAM_EVENT_ITEM ei;
	InitEventItem(&ei, nIndex, nTime, TEVENT_ID_HOF, 0, 0);
	sprintf(ei.szEventName, "Handover fail");
	//  ei.nSystem =  ;
	ei.nParam3 = p->HandovercontextID;
	ei.nParam4 = GetI32(p->HOFcause);
	PushEventMatrix(&ei);
}

void CEventDecode::PushParam_SHO(unsigned int nIndex, unsigned int nTime, unsigned char *pData)
{
	TEVENT_SHO_STR *p  = (TEVENT_SHO_STR *)pData;
	T_PARAM_EVENT_ITEM ei;
	if(TEVENT_SYSTEM_UMTSFDD == p->System)
	{
		TEVENT_SHO_UMTSFDD_STR *pPacket = (TEVENT_SHO_UMTSFDD_STR *)(p + sizeof(TEVENT_SHO_STR));
		InitEventItem(&ei, nIndex, nTime, TEVENT_ID_SHO, 0, 0);
		//	    ei.nSystem = p->System ;
		ei.nParam1 =  pPacket->SHOstatus;
		sprintf(ei.szEventName, "Soft handover");
		PushEventMatrix(&ei);
	}
}

void CEventDecode::PushParam_CREL(unsigned int nIndex, unsigned int nTime, unsigned char *pData)
{
	TEVENT_CREL_STR *p = (TEVENT_CREL_STR *)pData;

	T_PARAM_EVENT_ITEM ei;
	InitEventItem(&ei, nIndex, nTime, TEVENT_ID_CREL, p->Oldsystem, p->Currsystem);
	sprintf(ei.szEventName, "Cell reselection");
	//	ei.nSystem = p->System ;
	ei.nParam1 = p->Oldsystem;
	ei.nParam2 = p->Currsystem;

	char szValue[100] = {0};
	GetSystem(p->Oldsystem, szValue);
	sprintf(ei.szRemark, "%s (CI:%d LAC:%d):", szValue, GetI32(p->OldCI), GetU16(p->OldLAC));

	GetSystem(p->Currsystem, szValue);
	strcat(ei.szRemark, szValue);

	sprintf(szValue, " (CI:%d LAC:%d)", GetI32(p->CI), GetU16(p->LAC));
	strcat(ei.szRemark, szValue);

	PushEventMatrix(&ei);
}

void CEventDecode::PushParam_LUA(unsigned int nIndex, unsigned int nTime, unsigned char *pData)
{
	TEVENT_LUA_STR *p = (TEVENT_LUA_STR *)pData;

	T_PARAM_EVENT_ITEM ei;
	InitEventItem(&ei, nIndex, nTime, TEVENT_ID_LUA, 0, 0);
	sprintf(ei.szEventName, "LAU attempt"); //Location update attempt

	char szValue[100] = {0};
	GetLocationAreaUpdateType(p->LAUtype, szValue);
	GetSystem(p->System, ei.szRemark);
	strcat(ei.szRemark, szValue);
	//	ei.nSystem = p->System ;
	PushEventMatrix(&ei);
}

void CEventDecode::PushParam_LUS(unsigned int nIndex, unsigned int nTime, unsigned char *pData)
{
	TEVENT_LUS_STR *p = (TEVENT_LUS_STR *)pData;

	T_PARAM_EVENT_ITEM ei;
	InitEventItem(&ei, nIndex, nTime, TEVENT_ID_LUS, 0, 0);
	sprintf(ei.szEventName, "LAU successed"); //Location update accepted

	char szValue[100] = {0};
	sprintf(szValue, "from [LAC:%d] to [LAC:%d MNC:%d MCC:%d]", GetU16(p->OldLAC), GetU16(p->LAC), GetU16(p->MNC), GetU16(p->MCC));
	GetSystem(p->System, ei.szRemark);
	strcat(ei.szRemark, szValue);
	//	ei.nSystem = p->System ;
	PushEventMatrix(&ei);
}

void CEventDecode::PushParam_LUF(unsigned int nIndex, unsigned int nTime, unsigned char *pData)
{
	TEVENT_LUF_STR *p = (TEVENT_LUF_STR *)pData;

	T_PARAM_EVENT_ITEM ei;
	InitEventItem(&ei, nIndex, nTime, TEVENT_ID_LUF, 0, 0);
	sprintf(ei.szEventName, "LAU failed"); //Location update fail

	char szValue[100] = {0};
	GetSystem(p->System, ei.szRemark);
	sprintf(szValue, " [LAC:%d] status:", GetU16(p->OldLAC));
	strcat(ei.szRemark, szValue);

	GetLUFStatus(p->LUFstatus, szValue);
	strcat(ei.szRemark, szValue);

	sprintf(szValue, " cause:");
	strcat(ei.szRemark, szValue);

	GetMMCause(p->MMcause, szValue);
	strcat(ei.szRemark, szValue);

	//	ei.nSystem = p->System ;
	PushEventMatrix(&ei);
}

void CEventDecode::PushParam_RRA(unsigned int nIndex, unsigned int nTime, unsigned char *pData)
{
	TEVENT_RRA_STR *p = (TEVENT_RRA_STR *)pData;

	T_PARAM_EVENT_ITEM ei;
	InitEventItem(&ei, nIndex, nTime, TEVENT_ID_RRA, 0, 0);
	sprintf(ei.szEventName, "RRC attempt"); //Radio resource connection attempt
	sprintf(ei.szRemark, "Cause : ");

	if(( TEVENT_SYSTEM_UMTSFDD == p->System ) || ( TEVENT_SYSTEM_UMTSTD_SCDMA == p->System ))
	{
		char szValue[100] = {0};
		TEVENT_RRA_UMTS_STR *pPacket = (TEVENT_RRA_UMTS_STR *)p->append;		
		GetRRAEstablishmentCause(pPacket->RRCestcause, szValue);

		//		ei.nSystem = p->System ;
		strcat(ei.szRemark, szValue);
		PushEventMatrix(&ei);
	}
}

void CEventDecode::PushParam_RRC(unsigned int nIndex, unsigned int nTime, unsigned char *pData)
{
	TEVENT_RRC_STR *p = (TEVENT_RRC_STR *)pData;

	T_PARAM_EVENT_ITEM ei;
	InitEventItem(&ei, nIndex, nTime, TEVENT_ID_RRC, 0, 0);

	//	ei.nSystem = p->System ;
	sprintf(ei.szEventName, "RRC success"); //Radio resource connection success
	PushEventMatrix(&ei);
}

void CEventDecode::PushParam_RRF(unsigned int nIndex, unsigned int nTime, unsigned char *pData)
{
	TEVENT_RRF_STR *p = (TEVENT_RRF_STR *)pData;

	T_PARAM_EVENT_ITEM ei;
	InitEventItem(&ei, nIndex, nTime, TEVENT_ID_RRF, 0, 0);
	//	ei.nSystem = p->System ;

	sprintf(ei.szEventName, "RRC failure"); //Radio resource connection failure
	if(( TEVENT_SYSTEM_UMTSFDD == p->System ) || ( TEVENT_SYSTEM_UMTSTD_SCDMA == p->System ))
	{
		char szValue[100] = {0};
		TEVENT_RRF_UMTS_STR *pPacket = (TEVENT_RRF_UMTS_STR *)p->append;		

		//RRC rejection status
		// 1 = Network reject
		sprintf(szValue, "RRC rejection status: %s", (1==pPacket->RRCrejstatus)?"Network reject":"unknow");
		strcat(ei.szRemark, szValue);

		//RRC rejection cause
		// 0 = Congestion
		// 1 = Unspecified
		sprintf(szValue, "cause: %s", (0==pPacket->RRCrejcause)?"Congestion":"Unspecified");
		strcat(ei.szRemark, szValue);

		PushEventMatrix(&ei);
	}
}

void CEventDecode::PushParam_RRD(unsigned int nIndex, unsigned int nTime, unsigned char *pData)
{
	TEVENT_RRD_STR *p = (TEVENT_RRD_STR *)pData;

	T_PARAM_EVENT_ITEM ei;	

	if(( TEVENT_SYSTEM_UMTSFDD == p->System ) || ( TEVENT_SYSTEM_UMTSTD_SCDMA == p->System ))
	{
		char szValue[100] = {0};
		TEVENT_RRD_UMTS_STR *pPacket = (TEVENT_RRD_UMTS_STR *)p->append;

		InitEventItem(&ei, nIndex, nTime, TEVENT_ID_RRD, pPacket->RRCrelstatus, 0);
		sprintf(ei.szEventName, "RRC release"); //Radio resource connection release

		ei.nParam1 = pPacket->RRCrelstatus;
		//RRC release status
		// 1 = Normal release
		// 2 = Dropped RRC connection
		sprintf(szValue, "RRC release status: %s", (1==pPacket->RRCrelstatus)?"Normal release":"Dropped RRC connection");
		strcat(ei.szRemark, szValue);

		//RRC release cause
		sprintf(szValue, "cause: ");
		strcat(ei.szRemark, szValue);

		GetRRDReleaseCause(GetU16(pPacket->RRCrelcause), szValue);
		strcat(ei.szRemark, szValue);

		//	ei.nSystem = p->System ;
		PushEventMatrix(&ei);
	}
}

void CEventDecode::PushParam_PAA(unsigned int nIndex, unsigned int nTime, unsigned char *pData)
{
	TEVENT_PAA_STR *p = (TEVENT_PAA_STR *)pData;

	T_PARAM_EVENT_ITEM ei;
	InitEventItem(&ei, nIndex, nTime, TEVENT_ID_PAA, 0, 0);	
	sprintf(ei.szEventName, "PaSesAct attempt"); //Packet session activation attempt

	if(( TEVENT_SYSTEM_GSM == p->System )||( TEVENT_SYSTEM_UMTSFDD == p->System ) || ( TEVENT_SYSTEM_UMTSTD_SCDMA == p->System ))
	{
		TEVENT_PAA_GSMANDUMTS_STR *pPacket = (TEVENT_PAA_GSMANDUMTS_STR *)p->append;		
		sprintf(ei.szRemark, "Packet session activation attempt (Access point name : %s)", pPacket->APN);
		//	ei.nSystem = p->System ;
		PushEventMatrix(&ei);
	}
}

void CEventDecode::PushParam_PAC(unsigned int nIndex, unsigned int nTime, unsigned char *pData)
{
	TEVENT_PAC_STR *p = (TEVENT_PAC_STR *)pData;

	T_PARAM_EVENT_ITEM ei;
	InitEventItem(&ei, nIndex, nTime, TEVENT_ID_PAC, 0, 0);

	sprintf(ei.szEventName, "PaSesActed"); //Packet session activated
	GetSystem(p->System, ei.szRemark);
	ei.nParam1 = p->Packetactstate; 
	char szValue[100] = {0};
	//Packet session activation state
	// 1 = Air interface connected (in session management layer)
	// 2 = Packet session activated
	sprintf(szValue, " %s ", (1==p->Packetactstate)?"Air interface connected":"Packet session activated");
	strcat(ei.szRemark, szValue);

	if(2==p->Packetactstate) 
	{
		sprintf(szValue, " (IP: %s)", p->IP);
		strcat(ei.szRemark, szValue);
	}

	//	ei.nSystem = p->System ;
	PushEventMatrix(&ei);
}

void CEventDecode::PushParam_PAF(unsigned int nIndex, unsigned int nTime, unsigned char *pData)
{
	TEVENT_PAF_STR *p = (TEVENT_PAF_STR *)pData;

	T_PARAM_EVENT_ITEM ei;
	InitEventItem(&ei, nIndex, nTime, TEVENT_ID_PAF, 0, 0);

	sprintf(ei.szEventName, "PaSesAct failed"); // Packet session activation failed
	GetPAFFailureStatus(p->Failstatus, ei.szRemark);

	char szValue[100] = {"Packet session activation failed (Cause : )"};
	strcat(ei.szRemark, szValue);

	GetPAFFailureCause(GetU16(p->Failcause), szValue);
	strcat(ei.szRemark, szValue);

	//	ei.nSystem = p->System ;
	PushEventMatrix(&ei);
}

void CEventDecode::PushParam_PAD(unsigned int nIndex, unsigned int nTime, unsigned char *pData)
{
	TEVENT_PAD_STR *p = (TEVENT_PAD_STR *)pData;

	T_PARAM_EVENT_ITEM ei;
	InitEventItem(&ei, nIndex, nTime, TEVENT_ID_PAD, p->Deactstatus, 0);

	sprintf(ei.szEventName, "PaSesDeacted"); //Act Packet session deactivated
	GetPAFFailureStatus(p->Deactstatus, ei.szRemark);

	char szValue[100] = {"Act Packet session deactivated >> Cause : "};
	strcat(ei.szRemark, szValue);

	GetPAFFailureCause(GetU16(p->Deactcause), szValue);
	strcat(ei.szRemark, szValue);

	ei.nParam1 = p->Deactstatus;
	//	ei.nSystem = p->System ;
	PushEventMatrix(&ei);
}

void CEventDecode::PushParam_GAA(unsigned int nIndex, unsigned int nTime, unsigned char *pData)
{
	TEVENT_GAA_STR *p = (TEVENT_GAA_STR *)pData;

	T_PARAM_EVENT_ITEM ei;
	InitEventItem(&ei, nIndex, nTime, TEVENT_ID_GAA, 0, 0);	
	sprintf(ei.szEventName, "GPRS Ata attempt"); //GPRS attach attempt
	GetSystem(p->System, ei.szRemark);
	strcat(ei.szRemark, " (GPRS attach attempt)");
	ei.nParam4 = p->AttachcontextID;
	//	ei.nSystem = p->System ;
	PushEventMatrix(&ei);
}

void CEventDecode::PushParam_GAC(unsigned int nIndex, unsigned int nTime, unsigned char *pData)
{
	TEVENT_GAC_STR *p = (TEVENT_GAC_STR *)pData;

	T_PARAM_EVENT_ITEM ei;
	InitEventItem(&ei, nIndex, nTime, TEVENT_ID_GAC, 0, 0);	
	sprintf(ei.szEventName, "GPRS Ata connected"); //GPRS attach connected
	GetSystem(p->System, ei.szRemark);
	strcat(ei.szRemark, " (GPRS attach connected)");
	ei.nParam4 = p->AttachcontextID;
	//	ei.nSystem = p->System ;
	PushEventMatrix(&ei);
}

void CEventDecode::PushParam_GAF(unsigned int nIndex, unsigned int nTime, unsigned char *pData)
{
	TEVENT_GAF_STR *p = (TEVENT_GAF_STR *)pData;

	T_PARAM_EVENT_ITEM ei;
	InitEventItem(&ei, nIndex, nTime, TEVENT_ID_GAF, 0, 0);	
	sprintf(ei.szEventName, "GPRS Ata failed"); //GPRS attach failed

	GetSystem(p->System, ei.szRemark);

	char szValue[100] = {"failure status : "};
	strcat(ei.szRemark, szValue);

	GetPAFFailureStatus(p->Attachfail, szValue);
	strcat(ei.szRemark, szValue);

	sprintf(szValue, " Cause : ");
	strcat(ei.szRemark, szValue);	

	GetGAFFailureCause(p->Attfailcause, szValue);
	strcat(ei.szRemark, szValue);
	ei.nParam3 = p->Attfailcause;
	ei.nParam4 = p->AttachcontextID;
	ei.iParam5 = p->Attachfail;
	//	ei.nSystem = p->System ;
	PushEventMatrix(&ei);
}

void CEventDecode::PushParam_GAD(unsigned int nIndex, unsigned int nTime, unsigned char *pData)
{
	TEVENT_GAD_STR *p = (TEVENT_GAD_STR *)pData;

	T_PARAM_EVENT_ITEM ei;
	InitEventItem(&ei, nIndex, nTime, TEVENT_ID_GAD, 0, 0);
	sprintf(ei.szEventName, "GPRS detach");

	GetSystem(p->System, ei.szRemark);

	char szValue[100] = {" status : "};
	strcat(ei.szRemark, szValue);

	ei.nParam3 = p->Detachstatus;
	GetPAFFailureStatus(p->Detachstatus, szValue);
	strcat(ei.szRemark, szValue);

	sprintf(szValue, " Cause : ");
	strcat(ei.szRemark, szValue);	

	GetGAFFailureCause(p->Detachcause, szValue);
	strcat(ei.szRemark, szValue);
	ei.nParam4 = p->AttachcontextID;
	//	ei.nSystem = p->System ;
	PushEventMatrix(&ei);
}

void CEventDecode::PushParam_RUA(unsigned int nIndex, unsigned int nTime, unsigned char *pData)
{
	TEVENT_RUA_STR *p = (TEVENT_RUA_STR *)pData;

	T_PARAM_EVENT_ITEM ei;
	InitEventItem(&ei, nIndex, nTime, TEVENT_ID_RUA, 0, 0);	

	sprintf(ei.szEventName, "RAU attempt");	 //Routing area update attempt
	GetRUAType(p->RAUtype, ei.szRemark);

	//	ei.nSystem = p->System ;
	PushEventMatrix(&ei);
}

void CEventDecode::PushParam_RUF(unsigned int nIndex, unsigned int nTime, unsigned char *pData)
{
	TEVENT_RUF_STR *p = (TEVENT_RUF_STR *)pData;

	T_PARAM_EVENT_ITEM ei;
	InitEventItem(&ei, nIndex, nTime, TEVENT_ID_RUF, 0, 0);	
	sprintf(ei.szEventName, "RAU fail"); //Routing area update fail

	sprintf(ei.szRemark, "[ LAC : %d  RAC : %d ] Cause : ", GetU16(p->AttLAC), p->AttRAC);

	char szValue[100] = {0};
	GetGAFFailureCause(p->RAUfailcause, szValue);
	strcat(ei.szRemark, szValue);	

	//	ei.nSystem = p->System ;
	PushEventMatrix(&ei);
}

void CEventDecode::PushParam_RUS(unsigned int nIndex, unsigned int nTime, unsigned char *pData)
{
	TEVENT_RUS_STR *p = (TEVENT_RUS_STR *)pData;

	T_PARAM_EVENT_ITEM ei;
	InitEventItem(&ei, nIndex, nTime, TEVENT_ID_RUS, 0, 0);
	sprintf(ei.szEventName, "RAU successful"); //Routing area update successful
	sprintf(ei.szRemark, "from [ LAC : %d  RAC : %d ] to  [ LAC : %d  RAC : %d ]", GetU16(p->OldLAC), p->OldRAC, GetU16(p->LAC), p->RAC);

	//	ei.nSystem = p->System ;
	PushEventMatrix(&ei);
}

void CEventDecode::PushParam_MSGA(unsigned int nIndex, unsigned int nTime, unsigned char *pData)
{
	TEVENT_MSGA_STR *p = (TEVENT_MSGA_STR *)pData;

	T_PARAM_EVENT_ITEM ei;
	InitEventItem(&ei, nIndex, nTime, TEVENT_ID_MSGA, 0, 0);

	ei.nParam1 = p->Messagetype;

	char szValue[100] = {0};
	if( TEVENT_MSGA_MESSAGETYPE_SMS == p->Messagetype )
	{
		TEVENT_MSGA_SMSMESSAGE_STR *pPacket = (TEVENT_MSGA_SMSMESSAGE_STR *)p->append;
		sprintf(ei.szEventName, "SMS ");
		GetSMSType(pPacket->SMSmsgtype, szValue);
		strcat(ei.szEventName, szValue);		
		sprintf(ei.szRemark, "%s", pPacket->SMSnumber);
		ei.nParam2 = pPacket->SMSmsgtype;
		ei.nParam3 = atoi(pPacket->SMSnumber);
		ei.nParam4 = pPacket->SMScontextID;
	}
	else if( TEVENT_MSGA_MESSAGETYPE_MMS == p->Messagetype )
	{
		TEVENT_MSGA_MMSMESSAGE_STR *pPacket = (TEVENT_MSGA_MMSMESSAGE_STR *)p->append;
		sprintf(ei.szEventName, "MMS ");
		GetMMSType(pPacket->MMSmsgtype, szValue);
		strcat(ei.szEventName, szValue);	

		sprintf(ei.szRemark, "%s", pPacket->MMSsercenter);
		//GetMMSProtocol(pPacket->MMStrprotocol, szValue);
		//strcat(ei.szRemark, szValue);

		ei.nParam2 = pPacket->MMSmsgtype;
		ei.nParam4 = pPacket->MMScontextID;
		ei.iParam5 = pPacket->MMStrprotocol;
	}	

	//	ei.nSystem = p->System ;
	PushEventMatrix(&ei);
}

void CEventDecode::PushParam_MSGS(unsigned int nIndex, unsigned int nTime, unsigned char *pData)
{
	TEVENT_MSGS_STR *p = (TEVENT_MSGS_STR *)pData;

	T_PARAM_EVENT_ITEM ei;
	InitEventItem(&ei, nIndex, nTime, TEVENT_ID_MSGS, 0, 0);
	ei.nParam1 = p->Messagetype;

	char szValue[100] = {0};
	if( TEVENT_MSGA_MESSAGETYPE_SMS == p->Messagetype )
	{
		TEVENT_MSGS_SMSMESSAGE_STR *pPacket = (TEVENT_MSGS_SMSMESSAGE_STR *)p->append;
		sprintf(ei.szEventName, "SMS ");
		GetSMSType(pPacket->SMSmsgtype, szValue);
		strcat(ei.szEventName, szValue);
		strcat(ei.szEventName, "success");

		ei.nParam2 = pPacket->SMSmsgtype;
		ei.nParam4 = pPacket->SMScontextID;
	}
	else if( TEVENT_MSGA_MESSAGETYPE_MMS == p->Messagetype )
	{
		TEVENT_MSGS_MMSMESSAGE_STR *pPacket = (TEVENT_MSGS_MMSMESSAGE_STR *)p->append;

		sprintf(ei.szEventName, "MMS ");

		GetMMSType(pPacket->MMSmsgtype, szValue);
		strcat(ei.szEventName, szValue);
		strcat(ei.szEventName, "success");

		ei.nParam2 = pPacket->MMSmsgtype;
		ei.nParam4 = pPacket->MMScontextID;
	}	

	//	ei.nSystem = p->System ;
	PushEventMatrix(&ei);
}

void CEventDecode::PushParam_MSGF(unsigned int nIndex, unsigned int nTime, unsigned char *pData)
{
	TEVENT_MSGF_STR *p = (TEVENT_MSGF_STR *)pData;

	T_PARAM_EVENT_ITEM ei;
	InitEventItem(&ei, nIndex, nTime, TEVENT_ID_MSGF, 0, 0);
	ei.nParam1 = p->Messagetype;

	char szValue[100] = {0};
	if( TEVENT_MSGF_MESSAGETYPE_SMS == p->Messagetype )
	{
		TEVENT_MSGF_SMSMESSAGE_STR *pPacket = (TEVENT_MSGF_SMSMESSAGE_STR *)p->append;

		sprintf(ei.szEventName, "SMS ");

		GetSMSType(pPacket->SMSmsgtype, szValue);
		strcat(ei.szEventName, szValue);

		strcat(ei.szEventName, "failed");
		

		//GetSMSFailedCause(pPacket->SMScause, szValue);
		//sprintf(ei.szRemark, "failed cause : %s", szValue);
		ei.nParam2 = pPacket->SMSmsgtype;
		ei.nParam3 = GetU16(pPacket->SMScause);
		ei.nParam4 = pPacket->SMScontextID;
	}
	else if( TEVENT_MSGF_MESSAGETYPE_MMS == p->Messagetype )
	{
		TEVENT_MSGF_MMSMESSAGE_STR *pPacket = (TEVENT_MSGF_MMSMESSAGE_STR *)p->append;

		sprintf(ei.szEventName, "MMS ");

		GetMMSType(pPacket->MMSmsgtype, szValue);
		strcat(ei.szEventName, szValue);

		strcat(ei.szEventName, "failed");

		GetMMSFailedCause(pPacket->MMScause, ei.szRemark);
		sprintf(ei.szRemark, "failed cause : %s", szValue);

		ei.nParam2 = pPacket->MMSmsgtype;
		ei.nParam3 = pPacket->MMScause;
		ei.nParam4 = pPacket->MMScontextID;
	}	

	//	ei.nSystem = p->System ;
	PushEventMatrix(&ei);
}

void CEventDecode::PushParam_AQUL(unsigned int nIndex, unsigned char *pData)
{
	TEVENT_AQDL_STR *p = (TEVENT_AQDL_STR*)pData;
	if (m_CurSystem == TEVENT_SYSTEM_GSM)
	{
		PushParam_Value_float(E_PARAM_GSM_MOS, nIndex, GetFLOAT(p->AQMOSDL));
	}else if (m_CurSystem == TEVENT_SYSTEM_UMTSFDD)
	{
		PushParam_Value_float(E_PARAM_W_MOS, nIndex, GetFLOAT(p->AQMOSDL));
	}
}


void CEventDecode::PushParam_AQDL(unsigned int nIndex, unsigned char *pData)
{
	TEVENT_AQDL_STR *p = (TEVENT_AQDL_STR*)pData;
	if (m_CurSystem == TEVENT_SYSTEM_GSM)
	{
		PushParam_Value_float(E_PARAM_GSM_MOS, nIndex, GetFLOAT(p->AQMOSDL));
	}else if (m_CurSystem == TEVENT_SYSTEM_UMTSFDD)
	{
		PushParam_Value_float(E_PARAM_W_MOS,   nIndex, GetFLOAT(p->AQMOSDL));
	}
}

void CEventDecode::PushParam_OffNet(unsigned int nIndex, unsigned int nTime, unsigned char *pData)
{
	TEVENT_H_OFFNET_STR *pOffsetNet = (TEVENT_H_OFFNET_STR*)pData;
	T_PARAM_EVENT_ITEM ei;
	InitEventItem(&ei, nIndex, nTime, TEVENT_ID_OFF_NET, 0, 0);
	ei.nParam1 = GetI32(pOffsetNet->nSystem);
	PushEventMatrix(&ei);
}

void CEventDecode::PushParam_APPOpenInfo(unsigned int nIndex, unsigned int nTime, unsigned char *pData)
{
	TEVENT_H_APP_OPEN_STR* pOpenAppInfo = (TEVENT_H_APP_OPEN_STR *)pData;
	T_PARAM_EVENT_ITEM ei;
	InitEventItem(&ei, nIndex, nTime, TEVENT_ID_APP_OPEN_STATS, 0, 0);
	strcpy(ei.szEventName, pOpenAppInfo->appID);
	double fLat = GetFLOAT(pOpenAppInfo->lat);
	double fLon = GetFLOAT(pOpenAppInfo->lon);
	if (g_psTranslate->TransLonLat(fLat, fLon))
	{
		ei.iParam5 = fLat*1000;
		ei.iParam6 = fLon*1000;
	}else
	{
		ei.iParam5 = GetFLOAT(pOpenAppInfo->lat)*1000;
		ei.iParam6 = GetFLOAT(pOpenAppInfo->lon)*1000;
	}
	ei.nParam1 = nTime;
	wchar_t szBuffer[260] = {0};
	int nIndex_1 = 0;
	int nLen = pOpenAppInfo->uAppNameLen;
	for (int n=0; n<nLen; )
	{
		memcpy(&(szBuffer[nIndex_1]), &(pOpenAppInfo->uAppName[n]), 2);
		n += 2;
		nIndex_1++;
	}
	_gConverW2M(ei.szRemark,  szBuffer, nLen, sizeof(ei.szRemark));
	PushEventMatrix(&ei);
}

void CEventDecode::PushParam_ComplanitAPPInfo(unsigned int nIndex, unsigned int nTime, unsigned char *pData)
{
	TEVENT_H_APP_INFO_STR *pHAppinfo = (TEVENT_H_APP_INFO_STR *)pData;
	T_PARAM_EVENT_ITEM ei;
	InitEventItem(&ei, nIndex, nTime, TEVENT_ID_APPINFO, 0, 0);
	strcpy(ei.szEventName, pHAppinfo->appID);
	ei.nParam1 = GetI32(pHAppinfo->activeDuration);
	ei.nParam2 = GetI32(pHAppinfo->onlineDuration);
	ei.nParam3 = GetI32(pHAppinfo->mobileDuration);
	ei.nParam4 = GetI32(pHAppinfo->wifiDuration);
	
	ei.iParam5 = GetI32(pHAppinfo->mobileBytes);
	ei.iParam6 = GetI32(pHAppinfo->wifiBytes);
	ei.iParam7 = GetI32(pHAppinfo->useTimes);
	wchar_t szBuffer[260] = {0};
	int nIndex_1 = 0;
	int nLen = pHAppinfo->uAppNameLen;
	for (int n=0; n<nLen; )
	{
		memcpy(&(szBuffer[nIndex_1]), &(pHAppinfo->uAppName[n]), 2);
		n += 2;
		nIndex_1++;
	}
	_gConverW2M(ei.szRemark,  szBuffer, nLen, sizeof(ei.szRemark));
	PushEventMatrix(&ei);
}

void CEventDecode::PushParam_ComplanitHTTP(unsigned int nIndex, unsigned int nTime, unsigned char *pData)
{
	TEVENT_H_HTTP_URL_STR* pHttpURL = (TEVENT_H_HTTP_URL_STR*)pData;
	T_PARAM_EVENT_ITEM ei;
	InitEventItem(&ei, nIndex, nTime, TEVENT_ID_HTTP_URL, 0, 0);
	ei.nParam1 = GetI32(pHttpURL->accessTimes);
	strcpy(ei.szEventName, pHttpURL->httpUrl);
	wchar_t szBuffer[260] = {0};
	int nIndex_1 = 0;
	int nLen = pHttpURL->uURLNameLen;
	for (int n=0; n<nLen; )
	{
		memcpy(&(szBuffer[nIndex_1]), &(pHttpURL->uURLName[n]), 2);
		n += 2;
		nIndex_1++;
	}
	_gConverW2M(ei.szRemark, szBuffer, nLen, sizeof(ei.szRemark));
	PushEventMatrix(&ei);
}

void CEventDecode::PushParam_USRBHVEV(unsigned int nIndex, unsigned char *pData)
{
	TEVENT_USRBHV_STR* pHsrbhv = (TEVENT_USRBHV_STR*)pData;
	m_CurSystem = pHsrbhv->system;
	SetNetTpye(pHsrbhv->system);
	if (m_usrbhvEventMatrix.nRealCnt < m_usrbhvEventMatrix.nRealCnt)
	{
		m_usrbhvEventMatrix.pCmpalintList[m_usrbhvEventMatrix.iIndex].iSystem  = pHsrbhv->system;
		m_usrbhvEventMatrix.pCmpalintList[m_usrbhvEventMatrix.iIndex].nEventID = TEVENT_ID_USRBHV;
		m_usrbhvEventMatrix.pCmpalintList[m_usrbhvEventMatrix.iIndex].iOperationType = GetI32(pHsrbhv->Operationtype);

		if (GetI32(pHsrbhv->Operationtype) == 1)//呼叫的
		{
			TEVENT_USRBHV_CALL_STR* pCallHsrbhv = (TEVENT_USRBHV_CALL_STR*)pHsrbhv->pad;
			m_usrbhvEventMatrix.pCmpalintList[m_usrbhvEventMatrix.iIndex].iCallType      = GetI32(pCallHsrbhv->CallType);
			m_usrbhvEventMatrix.pCmpalintList[m_usrbhvEventMatrix.iIndex].iCallDuration  = GetI32(pCallHsrbhv->Duration);
			m_usrbhvEventMatrix.pCmpalintList[m_usrbhvEventMatrix.iIndex].iCallDirection = GetI32(pCallHsrbhv->Direction);

		}else if (GetI32(pHsrbhv->Operationtype) == 2)//数据的
		{
			TEVENT_USRBHV_DATA_STR* pDataHsrnhv = (TEVENT_USRBHV_DATA_STR*)pHsrbhv->pad;
			m_usrbhvEventMatrix.pCmpalintList[m_usrbhvEventMatrix.iIndex].iDataDuration  = GetI32(pDataHsrnhv->Duration);
			m_usrbhvEventMatrix.pCmpalintList[m_usrbhvEventMatrix.iIndex].iFlow          = GetI32(pDataHsrnhv->flow);
		}
		m_usrbhvEventMatrix.iIndex++;
	}
}

void CEventDecode::PushParam_CMPEV(unsigned int nIndex, unsigned int nTime, unsigned char *pData)
{
	TEVENT_CMPEV_STR* pCmpeasStr = (TEVENT_CMPEV_STR*)pData;
	m_CurSystem = pCmpeasStr->system;
	if (m_complaintEventMatrix.iIndex < m_complaintEventMatrix.nRealCnt)
	{
		m_complaintEventMatrix.pCmpalintList[m_complaintEventMatrix.iIndex].nSystem  = pCmpeasStr->system;
		m_complaintEventMatrix.pCmpalintList[m_complaintEventMatrix.iIndex].nEventID = TEVENT_ID_CMPEV;
		if( TEVENT_SYSTEM_GSM == pCmpeasStr->system)
		{
			TEVENT_CMPEV_GSM_STR *pCellPacket = (TEVENT_CMPEV_GSM_STR *)pCmpeasStr->pad;
			m_complaintEventMatrix.pCmpalintList[m_complaintEventMatrix.iIndex].fLat  = GetFLOAT(pCellPacket->lat);
			m_complaintEventMatrix.pCmpalintList[m_complaintEventMatrix.iIndex].fLon  = GetFLOAT(pCellPacket->lon);
			m_complaintEventMatrix.pCmpalintList[m_complaintEventMatrix.iIndex].iCI   = GetI32(pCellPacket->cellid);
			m_complaintEventMatrix.pCmpalintList[m_complaintEventMatrix.iIndex].iLACI = GetI32(pCellPacket->lac);
			m_complaintEventMatrix.pCmpalintList[m_complaintEventMatrix.iIndex].iComplaintType = GetI32(pCellPacket->complainttype);
			m_complaintEventMatrix.pCmpalintList[m_complaintEventMatrix.iIndex].iMCC  = GetI32(pCellPacket->mcc);
			m_complaintEventMatrix.pCmpalintList[m_complaintEventMatrix.iIndex].iMNC  = GetI32(pCellPacket->mnc);
			m_complaintEventMatrix.pCmpalintList[m_complaintEventMatrix.iIndex].iSceneType = GetI32(pCellPacket->scenttype);
			m_complaintEventMatrix.pCmpalintList[m_complaintEventMatrix.iIndex].nTime   = nTime;
			m_complaintEventMatrix.pCmpalintList[m_complaintEventMatrix.iIndex].iIndex  = nIndex;
			m_complaintEventMatrix.pCmpalintList[m_complaintEventMatrix.iIndex].iParam1 = GetFLOAT(pCellPacket->rxlevelSub);
			TEVENT_CMPEV_GSM_PARAMCELL_STR* pCmpevParamCell = (TEVENT_CMPEV_GSM_PARAMCELL_STR*)pCellPacket->paramCells;
			for (int n=0; (n<pCellPacket->nCells)&& (n<MAX_CELLMEAS_NUM_CELLS); n++)
			{
				memcpy(&m_complaintEventMatrix.pCmpalintList[m_complaintEventMatrix.iIndex].gsmCmpev[n].cellid, &(pCmpevParamCell[n].cellid), sizeof(pCmpevParamCell[n].cellid));
				memcpy(&m_complaintEventMatrix.pCmpalintList[m_complaintEventMatrix.iIndex].gsmCmpev[n].lac, &(pCmpevParamCell[n].lac), sizeof(pCmpevParamCell[n].lac));
				memcpy(&m_complaintEventMatrix.pCmpalintList[m_complaintEventMatrix.iIndex].gsmCmpev[n].rxlevelSub, &(pCmpevParamCell[n].rxlevelSub), sizeof(pCmpevParamCell[n].rxlevelSub));
			}

		}else if (TEVENT_SYSTEM_UMTSFDD == pCmpeasStr->system)
		{
			TEVENT_CMPEV_UTMSFDD_STR *pCellPacket = (TEVENT_CMPEV_UTMSFDD_STR*)pCmpeasStr->pad;
			m_complaintEventMatrix.pCmpalintList[m_complaintEventMatrix.iIndex].fLat  = GetFLOAT(pCellPacket->lat);
			m_complaintEventMatrix.pCmpalintList[m_complaintEventMatrix.iIndex].fLon  = GetFLOAT(pCellPacket->lon);
			m_complaintEventMatrix.pCmpalintList[m_complaintEventMatrix.iIndex].iCI   = GetI32(pCellPacket->cellid);
			m_complaintEventMatrix.pCmpalintList[m_complaintEventMatrix.iIndex].iLACI = GetI32(pCellPacket->lac);
			m_complaintEventMatrix.pCmpalintList[m_complaintEventMatrix.iIndex].iComplaintType = GetI32(pCellPacket->complainttype);
			m_complaintEventMatrix.pCmpalintList[m_complaintEventMatrix.iIndex].iMCC  = GetI32(pCellPacket->mcc);
			m_complaintEventMatrix.pCmpalintList[m_complaintEventMatrix.iIndex].iMNC  = GetI32(pCellPacket->mnc);
			m_complaintEventMatrix.pCmpalintList[m_complaintEventMatrix.iIndex].iSceneType = GetI32(pCellPacket->scenttype);
			m_complaintEventMatrix.pCmpalintList[m_complaintEventMatrix.iIndex].nTime      = nTime;
			m_complaintEventMatrix.pCmpalintList[m_complaintEventMatrix.iIndex].iIndex     = nIndex;
			m_complaintEventMatrix.pCmpalintList[m_complaintEventMatrix.iIndex].iParam1    = GetFLOAT(pCellPacket->rscp);
			m_complaintEventMatrix.pCmpalintList[m_complaintEventMatrix.iIndex].iParam2    = GetFLOAT(pCellPacket->ecno);
			m_complaintEventMatrix.pCmpalintList[m_complaintEventMatrix.iIndex].iParam3    = GetI32(pCellPacket->src);
			TEVENT_CMPEV_UTMSFDD_PARAMCELL_STR* pCmpevParamCell = (TEVENT_CMPEV_UTMSFDD_PARAMCELL_STR*)pCellPacket->paramCells;
			for (int n=0; (n<pCellPacket->nCells)&&(n<MAX_CELLMEAS_NUM_CELLS); n++)
			{
				memcpy(&m_complaintEventMatrix.pCmpalintList[m_complaintEventMatrix.iIndex].wcdmaCmpeas[n].cellid, &(pCmpevParamCell[n].cellid), sizeof(pCmpevParamCell[n].cellid));
				memcpy(&m_complaintEventMatrix.pCmpalintList[m_complaintEventMatrix.iIndex].wcdmaCmpeas[n].ecno,   &(pCmpevParamCell[n].ecno), sizeof(pCmpevParamCell[n].ecno));
				memcpy(&m_complaintEventMatrix.pCmpalintList[m_complaintEventMatrix.iIndex].wcdmaCmpeas[n].lac,    &(pCmpevParamCell[n].lac),sizeof(pCmpevParamCell[n].lac));
				memcpy(&m_complaintEventMatrix.pCmpalintList[m_complaintEventMatrix.iIndex].wcdmaCmpeas[n].rscp,   &(pCmpevParamCell[n].rscp), sizeof(pCmpevParamCell[n].rscp));
				memcpy(&m_complaintEventMatrix.pCmpalintList[m_complaintEventMatrix.iIndex].wcdmaCmpeas[n].src,    &(pCmpevParamCell[n].src), sizeof(pCmpevParamCell[n].src));
			}
		}
		m_complaintEventMatrix.iIndex++;
	}
}

void CEventDecode::PushParam_INDOOR_MARK(unsigned int nIndex, unsigned char *pData)
{
	TEVENT_INDOOR_MARK_STR *p = (TEVENT_INDOOR_MARK_STR*)pData;
	PushParam_Value_float( E_PARAM_GPS_LAT_A, nIndex, GetFLOAT(p->nLat));
	PushParam_Value_float( E_PARAM_GPS_LON_A, nIndex, GetFLOAT(p->uLon));
}

//删除关键点
void CEventDecode::Delete_Indoor_Mark(unsigned char *pData)
{
	if(m_pParamList[E_PARAM_GPS_LAT_A].nTail>0) 
	{
		int iIndexLAT_A = (m_pParamList[E_PARAM_GPS_LAT_A].nTail -= 1);
		int iIndexLON_A = (m_pParamList[E_PARAM_GPS_LON_A].nTail -= 1);

		T_PARAM_DATA_FLOAT *pLat = (T_PARAM_DATA_FLOAT *)m_pParamList[E_PARAM_GPS_LAT_A].pData;
		T_PARAM_DATA_FLOAT *pLon = (T_PARAM_DATA_FLOAT *)m_pParamList[E_PARAM_GPS_LON_A].pData;

		pLat[iIndexLAT_A].fVale         = 0;
		pLat[iIndexLAT_A].iCurTimeIndex = 0;

		pLon[iIndexLON_A].fVale         = 0;
		pLon[iIndexLON_A].iCurTimeIndex = 0;
	}
}

void CEventDecode::PushParam_GPS(unsigned int nIndex, unsigned char *pData)
{
	TEVENT_GPS_STR *p = (TEVENT_GPS_STR*)pData;
	double fLat = 0;
	double fLon = 0;
	fLat = GetDOUBLE(p->Lat);
	fLon = GetDOUBLE(p->Lon);
	if (g_psTranslate->TransLonLat(fLat, fLon))
	{
		PushParam_Value_float( E_PARAM_GPS_LAT, nIndex, fLat);
		PushParam_Value_float( E_PARAM_GPS_LON, nIndex, fLon);
	}else
	{
		PushParam_Value_float( E_PARAM_GPS_LAT, nIndex, GetDOUBLE(p->Lat));
		PushParam_Value_float( E_PARAM_GPS_LON, nIndex, GetDOUBLE(p->Lon));
	}
	PushParam_Value_int( E_PARAM_GPS_DISTANCE,   nIndex, GetU32(p->Distance));
	PushParam_Value_int( E_PARAM_GPS_FIX,        nIndex, p->GPSfix);
	PushParam_Value_int( E_PARAM_GPS_VELOCITY,   nIndex, GetU32(p->Velocity));
	PushParam_Value_int( E_PARAM_GPS_SATELLITES, nIndex, GetU32(p->Satellites));
}

void CEventDecode::PushParam_DCOMP(unsigned int nIndex, unsigned int nTime, unsigned char *pData)
{
	TEVENT_DCOMP_STR *p = (TEVENT_DCOMP_STR *)pData;
	PushServerState(nIndex, E_SERVER_STATE_DATA);

	T_PARAM_EVENT_ITEM ei;
	InitEventItem(&ei, nIndex, nTime, TEVENT_ID_DCOMP, p->Applicationprotocol, p->Transfstatus);

	//	ei.nSystem = p->System ;
	ei.nParam3 = p->DatatransfercontextID;
	ei.nParam4 = GetI32(p->IPtermtime);
	ei.iParam5 = GetI32(p->BytesDL);
	ei.iParam6 = GetI32(p->BytesUL);

	GetApplicatiionProtocol(p->Applicationprotocol, ei.szEventName);
	strcat(ei.szEventName, " TransComp");

	// CS disconnect. status
	char szValue[32] = {0};
	GetDataTransferStatus(p->Transfstatus, szValue);
	sprintf(ei.szRemark, "%s DL: %d bytes UL: %d bytes", szValue, GetI32(p->BytesDL), GetI32(p->BytesUL));

	PushEventMatrix(&ei);
}

void CEventDecode::PushParam_CAA(unsigned int nIndex, unsigned int nTime, unsigned char *pData)
{
	TEVENT_CAA_STR *p = (TEVENT_CAA_STR *)pData;	
	PushSystem(nIndex, p->System);
	PushServerState(nIndex, E_SERVER_STATE_VOICE);

	T_PARAM_EVENT_ITEM ei;
	InitEventItem(&ei, nIndex, nTime, TEVENT_ID_CAA, p->Calltype, p->Direction);
	ei.nSystem = p->System ;
	ei.nParam3 = p->CallcontextID;
	GetCallType(p->Calltype, ei.szEventName);
	strcat(ei.szEventName, " Attempt");

	ei.nParam3 = p->CallcontextID;

	sprintf(ei.szRemark, "%s", p->Number);
	PushEventMatrix(&ei);
}

void CEventDecode::PushParamGSM_CAC(unsigned int nIndex, unsigned int nTime, unsigned char *pData)
{
	TEVENT_CAC_GSM_STR *p = (TEVENT_CAC_GSM_STR*)pData;
	PushSystem(nIndex,  p->System);

	T_PARAM_EVENT_ITEM ei;
	InitEventItem(&ei, nIndex, nTime, TEVENT_ID_CAC_GSM, p->Calltype, p->Callstatus);
	ei.nSystem = p->System ;
	ei.nParam3 = p->CallcontextID;
	GetCallType(p->Calltype, ei.szEventName);
	strcat(ei.szEventName, "Connect");
	GetCallConnectionStatus(p->Callstatus, ei.szRemark);
	PushEventMatrix(&ei);
}

void CEventDecode::PushParam_CAC(unsigned int nIndex, unsigned int nTime, unsigned char *pData)
{
	TEVENT_CAC_STR *p = (TEVENT_CAC_STR *)pData;
	PushSystem(nIndex, p->System);

	T_PARAM_EVENT_ITEM ei;
	InitEventItem(&ei, nIndex, nTime, TEVENT_ID_CAC, p->Calltype, p->Callstatus);
	ei.nSystem = p->System ;
	ei.nParam3 = p->CallcontextID;
	GetCallType(p->Calltype, ei.szEventName);
	strcat(ei.szEventName, " Connect");

	GetCallConnectionStatus(p->Callstatus, ei.szRemark);

	PushEventMatrix(&ei);
}

void CEventDecode::PushParam_CAF(unsigned int nIndex, unsigned int nTime, unsigned char *pData)
{
	TEVENT_CAF_STR *p = (TEVENT_CAF_STR *)pData;
	PushSystem(nIndex, p->System);
	PushServerState(nIndex, E_SERVER_STATE_IDLE);

	T_PARAM_EVENT_ITEM ei;
	InitEventItem(&ei, nIndex, nTime, TEVENT_ID_CAF, p->Calltype, p->CSfailstatus);
	ei.nSystem = p->System ;
	ei.nParam3 = p->CallcontextID;
	ei.nParam4 = (unsigned int)(GetI32(p->CSfailcause));
    GetCallType(p->Calltype, ei.szEventName);
	strcat(ei.szEventName, " faild");

	// CS fail. status
	GetCallFailureStatus(p->CSfailstatus, ei.szRemark);	

	PushEventMatrix(&ei);
}

void CEventDecode::PushParam_CAD(unsigned int nIndex, unsigned int nTime, unsigned char *pData)
{
	TEVENT_CAD_STR *p = (TEVENT_CAD_STR *)pData;
	PushSystem(nIndex, p->System);
	PushServerState(nIndex, E_SERVER_STATE_IDLE);

	T_PARAM_EVENT_ITEM ei;
	InitEventItem(&ei, nIndex, nTime, TEVENT_ID_CAD, p->Calltype, p->CSdiscstatus);
	ei.nSystem = p->System ;
	ei.nParam3 = p->CallcontextID;
	GetCallType(p->Calltype, ei.szEventName);
	strcat(ei.szEventName, " Disconnect");	

	// CS disconnect. status
	GetCallDisconnectStatus(p->CSdiscstatus, ei.szRemark);	

	PushEventMatrix(&ei);
}

void CEventDecode::PushParam_TAD(unsigned int nIndex, unsigned char *pData)
{
	TEVENT_TAD_STR *p = (TEVENT_TAD_STR *)pData;

	PushSystem(nIndex, p->System);
	if( TEVENT_SYSTEM_GSM == p->System )
	{
		TEVENT_TAD_GSM_STR *pPacket = (TEVENT_TAD_GSM_STR *)p->append;
		PushParam_Value_int( E_PARAM_GSM_TA, nIndex, pPacket->TA);
	}
	else if( TEVENT_SYSTEM_UMTSTD_SCDMA == p->System )
	{
		TEVENT_TAD_UMTSTDSCDMA_STR *pPacket = (TEVENT_TAD_UMTSTDSCDMA_STR *)p->append;
		PushParam_Value_float( E_PARAM_TD_SERV_TA, nIndex, GetFLOAT(pPacket->TA));
	}
}


void CEventDecode::PushParam_TBFI(unsigned int nIndex, unsigned char *pData)
{
	TEVENT_TBFI_STR *pPacket = (TEVENT_TBFI_STR *)pData;	
	PushParam_Value_int( E_PARAM_GSM_PWINSIZEUL, nIndex,  GetU16(pPacket->UlRLCwin));
	PushParam_Value_int( E_PARAM_GSM_PWINSIZEDL, nIndex,  GetU16(pPacket->DlRLCwin));
}

void CEventDecode::PushParam_BEP(unsigned int nIndex, unsigned char *pData)
{
	TEVENT_BEP_STR *p = (TEVENT_BEP_STR *)pData;

	PushSystem(nIndex, p->System);
	if( TEVENT_SYSTEM_GSM == p->System )
	{
		TEVENT_BEP_GSM_STR *pPacket = (TEVENT_BEP_GSM_STR *)p->Pad;
		PushParam_Value_int( E_PARAM_GSM_P8PSKCVBEP,   nIndex, pPacket->PSKCV_BEP);
		PushParam_Value_int( E_PARAM_GSM_PGMSKCVBEP,   nIndex, pPacket->GMSKCV_BEP);
		PushParam_Value_int( E_PARAM_GSM_P8BSKMEANBEP, nIndex, pPacket->PSKMEAN_BEP);
		PushParam_Value_int( E_PARAM_GSM_PGMSKMEANBEP, nIndex, pPacket->GMSKMEAN_BEP);
	}	
}

void CEventDecode::PushParam_PRXQ(unsigned int nIndex, unsigned char *pData)
{
	TEVENT_PRXQ_STR *p = (TEVENT_PRXQ_STR *)pData;

	PushSystem(nIndex, p->System);
	if( TEVENT_SYSTEM_GSM == p->System )
	{
		TEVENT_PRXQ_GSM_STR *pPacket = (TEVENT_PRXQ_GSM_STR *)p->Pad;
		PushParam_Value_int( E_PARAM_GSM_PCVALUE, nIndex, pPacket->Cvalue);
		PushParam_Value_int( E_PARAM_GSM_PRXQUAL, nIndex, pPacket->RXQ);
	}	
}

void CEventDecode::PushParam_FER(unsigned int nIndex, unsigned char *pData)
{
	TEVENT_FER_STR *p = (TEVENT_FER_STR *)pData;
	PushSystem(nIndex, p->System);
	if( TEVENT_SYSTEM_GSM == p->System )
	{
		TEVENT_FER_GSM_STR *pPacket = (TEVENT_FER_GSM_STR *)(pData + sizeof(TEVENT_FER_STR));
		PushParam_Value_float( E_PARAM_GSM_FER_FULL, nIndex, GetFLOAT(pPacket->FERfull));
		PushParam_Value_float( E_PARAM_GSM_FER_SUB,  nIndex, GetFLOAT(pPacket->FERsub));
		PushParam_Value_float( E_PARAM_GSM_FER_TCH,  nIndex, GetFLOAT(pPacket->FERTCH));
	}
	else if( TEVENT_SYSTEM_UMTSTD_SCDMA == p->System )
	{
		TEVENT_FER_UMTSTDSCDMA_STR *pPacket = (TEVENT_FER_UMTSTDSCDMA_STR *)(pData + sizeof(TEVENT_FER_STR));
		PushParam_Value_float( E_PARAM_TD_FER, nIndex, GetFLOAT(pPacket->FER));
	}
	else if( TEVENT_SYSTEM_UMTSFDD == p->System )
	{
		TEVENT_FER_UMTSFDD_STR *pPacket = (TEVENT_FER_UMTSFDD_STR *)(pData + sizeof(TEVENT_FER_STR));
		PushParam_Value_float( E_PARAM_W_FER, nIndex, GetFLOAT(pPacket->FER));
	}
	else if ( TEVENT_SYSTEM_CDMA1X == p->System || TEVENT_SYSTEM_CDMAONE == p->System )
	{
		TEVENT_FER_CDMAONEANDCDMA1X_STR *pPacket = (TEVENT_FER_CDMAONEANDCDMA1X_STR *)(pData + sizeof(TEVENT_FER_STR));
		PushParam_Value_float( E_PARAM_CDMA_FER_TOTAL, nIndex, GetFLOAT(pPacket->FER));
		PushParam_Value_float( E_PARAM_CDMA_FER_FCH, nIndex, GetFLOAT(pPacket->FERtargetFFCH));
		PushParam_Value_float( E_PARAM_CDMA_FER_SCH, nIndex, GetFLOAT(pPacket->FERtargetFSCH));
	}
}

void CEventDecode::PushParam_RLT(unsigned int nIndex, unsigned char *pData)
{
	TEVENT_RLT_STR *p = (TEVENT_RLT_STR *)pData;

	PushSystem(nIndex, p->System);
	if( TEVENT_SYSTEM_GSM == p->System )
	{
		TEVENT_RLT_GSM_STR *pPacket = (TEVENT_RLT_GSM_STR *)p->append;
		PushParam_Value_int( E_PARAM_GSM_RLT, nIndex, pPacket->RLT);
	}
}

void CEventDecode::PushParam_DRATE(unsigned int nIndex, unsigned char *pData)
{
	TEVENT_DRATE_STR *p = (TEVENT_DRATE_STR *)pData;
	PushParam_WithRealRange(E_PARAM_THROUGHT_APP_DL, nIndex, GetU32(p->ApprateDL));
	PushParam_WithRealRange(E_PARAM_THROUGHT_APP_UL, nIndex, GetU32(p->ApprateUL));
	PushParam_WithRealRange(E_PARAM_THROUGHT_APP_DL_BYTES, nIndex, GetU32(p->BytesDL));
	PushParam_WithRealRange(E_PARAM_THROUGHT_APP_UL_BYTES, nIndex, GetU32(p->BytesUL));
}

void CEventDecode::PushParam_RLCRATE(unsigned int nIndex, unsigned char *pData)
{
	TEVENT_RLCRATE_STR *p = (TEVENT_RLCRATE_STR *)pData;

	PushSystem(nIndex, p->System);
	if( TEVENT_SYSTEM_GSM == p->System )
	{
		TEVENT_RLCRATE_GSM_STR *pPacket = (TEVENT_RLCRATE_GSM_STR *)p->append;
		PushParam_WithRealRange(E_PARAM_THROUGHT_RLC_DL, nIndex, GetI32(pPacket->RLCrateDL));
		PushParam_WithRealRange(E_PARAM_THROUGHT_RLC_UL, nIndex, GetI32(pPacket->RLCrateUL));
	}
	else if( TEVENT_SYSTEM_UMTSFDD == p->System || TEVENT_SYSTEM_UMTSTD_SCDMA == p->System )
	{
		TEVENT_RLCRATE_UMTS_STR *pPacket = (TEVENT_RLCRATE_UMTS_STR *)p->append;
		PushParam_WithRealRange( E_PARAM_THROUGHT_RLC_DL, nIndex, GetI32(pPacket->RLCrateDL));
		PushParam_WithRealRange( E_PARAM_THROUGHT_RLC_UL, nIndex, GetI32(pPacket->RLCrateUL));
		if (GetI32(pPacket->RLCrateUL) > 0)
		{
			PushParam_Value_float(E_PARAM_THROUGHT_RLC_UL_RETRANSMISSION,  nIndex, GetFLOAT(pPacket->RLCretrUL));
		}
	}
}

void CEventDecode::PushParam_RLPRATE(unsigned int nIndex, unsigned char *pData)
{
	TEVENT_RLPRATE_STR *p = (TEVENT_RLPRATE_STR *)pData;

	PushSystem(nIndex, p->System);
	if( TEVENT_SYSTEM_CDMA1X == p->System || TEVENT_SYSTEM_CDMAONE == p->System )
	{
		PushParam_Value_int(E_PARAM_CDMA_RLPRATE_RLP_REV_RATE, nIndex, GetI32(p->RLPrevrate));
		PushParam_Value_int(E_PARAM_CDMA_RLPRATE_RLP_FOR_RATE, nIndex, GetI32(p->RLPforrate));
		PushParam_Value_float(E_PARAM_CDMA_RLPRATE_RLP_REV_RETR_RATE, nIndex, GetFLOAT(p->RLPrevretrrate));
		PushParam_Value_float(E_PARAM_CDMA_RLPRATE_RLP_FWD_RETR_RATE, nIndex, GetFLOAT(p->RLPfwdretrrate));
	}
	else if( TEVENT_SYSTEM_EVDO == p->System )
	{
		PushParam_Value_int(E_PARAM_EVDO_RLPRATE_RLP_REV_RATE, nIndex, GetI32(p->RLPrevrate));
		PushParam_Value_int(E_PARAM_EVDO_RLPRATE_RLP_FOR_RATE, nIndex, GetI32(p->RLPforrate));
		PushParam_Value_float(E_PARAM_EVDO_RLPRATE_RLP_REV_RETR_RATE, nIndex, GetFLOAT(p->RLPrevretrrate));
		PushParam_Value_float(E_PARAM_EVDO_RLPRATE_RLP_FWD_RETR_RATE, nIndex, GetFLOAT(p->RLPfwdretrrate));
	}
}

void CEventDecode::PushParam_TXPC(unsigned int nIndex, unsigned char *pData)
{
	TEVENT_TXPC_STR *p = (TEVENT_TXPC_STR *)pData;

	PushSystem(nIndex, p->System);
	if( TEVENT_SYSTEM_UMTSTD_SCDMA == p->System )
	{
		TEVENT_TXPC_UMTSTDSCDMA_STR *pPacket = (TEVENT_TXPC_UMTSTDSCDMA_STR *)p->Pad;
		PushParam_Value_int(E_PARAM_TD_TX_POWER, nIndex, (int)pPacket->TXpower);
		PushParam_Value_float(E_PARAM_TD_TCP_STEP_SIZE, nIndex, GetFLOAT(pPacket->Pwrctrlstep));
	}
	else if( TEVENT_SYSTEM_UMTSFDD == p->System )
	{
		TEVENT_TXPC_UMTSFDD_STR *pPacket = (TEVENT_TXPC_UMTSFDD_STR *)p->Pad;
		PushParam_Value_int(E_PARAM_W_TX_POWER, nIndex, (int)pPacket->TXpower);
	}
	else if ( TEVENT_SYSTEM_CDMAONE == p->System || TEVENT_SYSTEM_CDMA1X == p->System )
	{
		TEVENT_TXPC_CDMAONEANDCDMA1X_STR *pPacket = (TEVENT_TXPC_CDMAONEANDCDMA1X_STR *)p->Pad;

		PushParam_Value_float(E_PARAM_CDMA_TXPC_TXPOWER, nIndex, GetFLOAT(pPacket->TXpower));
		PushParam_Value_float(E_PARAM_CDMA_TXPC_TXADJUST, nIndex, GetFLOAT(pPacket->TXadjust));
		PushParam_Value_float(E_PARAM_CDMA_TXPC_TXPWRLIMIT, nIndex, GetFLOAT(pPacket->TXpwrlimit));
		PushParam_Value_float(E_PARAM_CDMA_TXPC_R_FCH, nIndex, GetFLOAT(pPacket->RFCHRPICH));
		PushParam_Value_float(E_PARAM_CDMA_TXPC_R_SCH0, nIndex, GetFLOAT(pPacket->RSCH0RPICH));
		PushParam_Value_float(E_PARAM_CDMA_TXPC_R_SCH1, nIndex, GetFLOAT(pPacket->RSCH1RPICH));
		PushParam_Value_float(E_PARAM_CDMA_TXPC_R_DCCH, nIndex, GetFLOAT(pPacket->RDCCHRPICH));
	}
	else if ( TEVENT_SYSTEM_EVDO == p->System )
	{
		TEVENT_TXPC_EVDO_STR *pPacket = (TEVENT_TXPC_EVDO_STR *)p->Pad;

		PushParam_Value_float(E_PARAM_EVDO_TXPC_TX_POWER, nIndex, GetFLOAT(pPacket->TXpower));
		PushParam_Value_float(E_PARAM_EVDO_TXPC_TX_ADJUST, nIndex, GetFLOAT(pPacket->TXadjust));
		PushParam_Value_float(E_PARAM_EVDO_TXPC_TX_PILOT, nIndex, GetFLOAT(pPacket->TXPilot));
		PushParam_Value_float(E_PARAM_EVDO_TXPC_TX_OPEN_LOOP_POWER, nIndex, GetFLOAT(pPacket->TXopenlooppower));
		PushParam_Value_float(E_PARAM_EVDO_TXPC_TX_MAX_POWER_USAGE, nIndex, GetFLOAT(pPacket->TXmaxpowerusage));
		PushParam_Value_float(E_PARAM_EVDO_TXPC_TX_MIN_POWER_USAGE, nIndex, GetFLOAT(pPacket->TXminpowerusage));
	}
}

void CEventDecode::PushParam_RXPC(unsigned int nIndex, unsigned char *pData)
{
	TEVENT_RXPC_STR *p = (TEVENT_RXPC_STR *)pData;

	PushSystem(nIndex, p->System);
	if( TEVENT_SYSTEM_UMTSTD_SCDMA == p->System )
	{
		TEVENT_RXPC_UMTSTDSCDMA_STR *pPacket = (TEVENT_RXPC_UMTSTDSCDMA_STR *)p->Pad;
		PushParam_Value_int(E_PARAM_TD_SIR,        nIndex, (int)pPacket->SIR);
		PushParam_Value_int(E_PARAM_TD_SIR_TARGET, nIndex, (int)pPacket->SIRtarget);
		PushParamMatrix(nIndex, E_PARAM_TD_TIMESLOT, pPacket->nTimeslots, (unsigned char *)pPacket->PARAMSTIMESLOT);
	}
	else if( TEVENT_SYSTEM_UMTSFDD == p->System )
	{
		TEVENT_RXPC_UMTSFDD_STR *pPacket = (TEVENT_RXPC_UMTSFDD_STR *)p->Pad;

		PushParam_Value_float(E_PARAM_W_SIR, nIndex, GetFLOAT(pPacket->SIR));		
		PushParam_Value_float(E_PARAM_W_SIR_TARGET, nIndex, GetFLOAT(pPacket->SIRtarget));		
		PushParam_Value_float(E_PARAM_W_DLPWRUP, nIndex, GetFLOAT(pPacket->DLpwrup));
		PushParam_Value_int(E_PARAM_W_BSDIVSTATE, nIndex, (pPacket->BSdivstate));
		PushParam_Value_int(E_PARAM_W_DLPWRUP_CND, nIndex, (pPacket->nDLpwrup));
		PushParam_Value_int(E_PARAM_W_DLPWRDOWN_CND, nIndex, (pPacket->nDLpwrdown));
		PushParam_Value_int(E_PARAM_W_DPC_MODE, nIndex, (pPacket->DPCmode));
	}
}

void CEventDecode::PushParam_RLCBLER(unsigned int nIndex, unsigned char *pData)
{
	TEVENT_RLCBLER_STR *p = (TEVENT_RLCBLER_STR *)pData;

	PushSystem(nIndex, p->System);
	if( TEVENT_SYSTEM_GSM == p->System )
	{
		TEVENT_RLCBLER_GSM_STR *pPacket = (TEVENT_RLCBLER_GSM_STR *)p->append;
		PushParam_Value_float(E_PARAM_GSM_BLER,      nIndex, GetFLOAT(pPacket->BLER));
	}	
	else if( TEVENT_SYSTEM_UMTSTD_SCDMA == p->System )
	{
		TEVENT_RLCBLER_UMTSTDSCDMA_STR *pPacket = (TEVENT_RLCBLER_UMTSTDSCDMA_STR *)p->append;
		PushParam_Value_float(E_PARAM_TD_BLER,      nIndex, GetFLOAT(pPacket->BLER));
		PushParamMatrix(nIndex, E_PARAM_TD_TRCHI, pPacket->Chs, (unsigned char *)pPacket->append);
	}
	else if( TEVENT_SYSTEM_UMTSFDD == p->System )
	{
		TEVENT_RLCBLER_UMTSFDD_STR *pPacket = (TEVENT_RLCBLER_UMTSFDD_STR *)p->append;
		PushParam_Value_float(E_PARAM_W_BLER,    nIndex, GetFLOAT(pPacket->BLER));
		PushParamMatrix(nIndex, E_PARAM_W_TRCHI, pPacket->Chs, (unsigned char *)pPacket->append);
	}
}

void CEventDecode::PushParam_RACHI(unsigned int nIndex, unsigned char *pData)
{
	TEVENT_RACHI_STR *p = (TEVENT_RACHI_STR *)pData;
	PushSystem(nIndex, p->System);
	if( TEVENT_SYSTEM_UMTSTD_SCDMA == p->System )
	{
		TEVENT_RACHI_UMTSTDSCDMA_STR *pPacket = (TEVENT_RACHI_UMTSTDSCDMA_STR *)(p + sizeof(TEVENT_RACHI_STR));
		PushParam_Value_float(E_PARAM_TD_PCCPCH_PATHLOSS, nIndex, GetFLOAT(pPacket->PCCPCHpathloss));
	}
	else if( TEVENT_SYSTEM_UMTSFDD == p->System )
	{
		TEVENT_RACHI_UMTSFDD_STR *pPacket = (TEVENT_RACHI_UMTSFDD_STR *)(p + sizeof(TEVENT_RACHI_STR));
		PushParam_Value_float(E_PARAM_W_INIT_PWR, nIndex, GetFLOAT(pPacket->InitTXpower));
		PushParam_Value_float(E_PARAM_W_PWR_RAMP_STEP, nIndex, GetFLOAT(pPacket->Preamblestep));
		PushParam_Value_float(E_PARAM_W_RACH_TX_PWR, nIndex, GetFLOAT(pPacket->RACHTXpower));
		PushParam_Value_float(E_PARAM_W_UL_INTERF, nIndex, GetFLOAT(pPacket->ULinterf));
		PushParam_Value_float(E_PARAM_W_PWR_OFFSET, nIndex, GetFLOAT(pPacket->Poweroffset));
		PushParam_Value_int(E_PARAM_W_PRE_CNT, nIndex, (pPacket->Preamblecount));
		PushParam_Value_int(E_PARAM_W_MAX_PREEMBLE, nIndex, (pPacket->Maxpreamble));
		PushParam_Value_int(E_PARAM_W_AICH_STATUS, nIndex, (pPacket->AICHstatus));
		PushParam_Value_int(E_PARAM_W_DATA_GAIN, nIndex, (pPacket->Datagain));
		PushParam_Value_int(E_PARAM_W_CTRL_GAIN, nIndex, (pPacket->Ctrlgain));
		PushParam_Value_int(E_PARAM_W_MSG_LENGTH, nIndex, (pPacket->Messagelength));
		PushParam_Value_int(E_PARAM_W_PRE_CYCLES, nIndex, (pPacket->Preamblecycles));
	}
	else if ( TEVENT_SYSTEM_CDMA1X == p->System || TEVENT_SYSTEM_CDMAONE == p->System )
	{
		TEVENT_RACHI_CDMAONEANDCDMA1X_STR *pPacket = (TEVENT_RACHI_CDMAONEANDCDMA1X_STR *)(p + sizeof(TEVENT_RACHI_STR));
		PushParam_Value_int(E_PARAM_CDMA_RACHI_NOM_PWR, nIndex, GetI32(pPacket->NOM_PWR));
		PushParam_Value_int(E_PARAM_CDMA_RACHI_INIT_PWR, nIndex, GetI32(pPacket->INIT_PWR));
		PushParam_Value_int(E_PARAM_CDMA_RACHI_PWR_STEP, nIndex, GetI32(pPacket->PWR_STEP));
		PushParam_Value_int(E_PARAM_CDMA_RACHI_NUM_STEP, nIndex, GetI32(pPacket->NUM_STEP));
		PushParam_Value_int(E_PARAM_CDMA_RACHI_PROBECOUNT, nIndex, GetI32(pPacket->Probecount));
		PushParam_Value_int(E_PARAM_CDMA_RACHI_PROBESEQCOUNT, nIndex, GetI32(pPacket->Probeseqcount));
		PushParam_Value_int(E_PARAM_CDMA_RACHI_ACCESSCHNUMBER, nIndex, GetI32(pPacket->Accesschnumber));
		PushParam_Value_int(E_PARAM_CDMA_RACHI_RANDOMDELAY, nIndex, GetI32(pPacket->Randomdelay));
		PushParam_Value_float(E_PARAM_CDMA_RACHI_TXLEVEL, nIndex, GetFLOAT(pPacket->TXlevel));
		PushParam_Value_float(E_PARAM_CDMA_RACHI_ACCESSRXLEVEL, nIndex, GetFLOAT(pPacket->AccessRXlevel));
		PushParam_Value_float(E_PARAM_CDMA_RACHI_ACCESSTXADJ, nIndex, GetFLOAT(pPacket->AccessTXadj));
		PushParam_Value_int(E_PARAM_CDMA_RACHI_PSIST, nIndex, (pPacket->Psist));
		PushParam_Value_int(E_PARAM_CDMA_RACHI_SEQBACKOFF, nIndex, (pPacket->Seqbackoff));
		PushParam_Value_int(E_PARAM_CDMA_RACHI_PROBBACKOFF, nIndex, (pPacket->Probbackoff));
		PushParam_Value_int(E_PARAM_CDMA_RACHI_INTERCORR, nIndex, (pPacket->Intercorr));
	}
	else if ( TEVENT_SYSTEM_EVDO == p->System )
	{
		TEVENT_RACHI_EVDO_STR *pPacket = (TEVENT_RACHI_EVDO_STR *)(p + sizeof(TEVENT_RACHI_STR));
		PushParam_Value_int(E_PARAM_EVDO_RACHI_MaxProbeseqs, nIndex, GetI32(pPacket->MaxProbeseqs));
		PushParam_Value_int(E_PARAM_EVDO_RACHI_Duration, nIndex, GetI32(pPacket->Duration));
		PushParam_Value_int(E_PARAM_EVDO_RACHI_AccessPN, nIndex, GetI32(pPacket->AccessPN));
		PushParam_Value_int(E_PARAM_EVDO_RACHI_Accesssectorid, nIndex, GetI32(pPacket->Accesssectorid));
		PushParam_Value_int(E_PARAM_EVDO_RACHI_MaxProbes, nIndex, (pPacket->MaxProbes));
		PushParam_Value_int(E_PARAM_EVDO_RACHI_Result, nIndex, (pPacket->Result));
		PushParam_Value_int(E_PARAM_EVDO_RACHI_nProbes, nIndex, (pPacket->nProbes));
		PushParam_Value_int(E_PARAM_EVDO_RACHI_nProbeseqs, nIndex, (pPacket->nProbeseqs));
		PushParam_Value_int(E_PARAM_EVDO_RACHI_Accesscolorcode, nIndex,(pPacket->Accesscolorcode));
	}
}

void CEventDecode::PushParam_HOP(unsigned int nIndex, unsigned char *pData)
{
	TEVENT_HOP_STR *p = (TEVENT_HOP_STR *)pData;
	PushSystem(nIndex, p->System);
	if( TEVENT_SYSTEM_GSM == p->System )
	{
		// take care! it's too complex
		//  >> Hopping status : 1 = On
		//                      2 = Off
		TEVENT_HOPON_GSM_STR *pPacket = (TEVENT_HOPON_GSM_STR *)(p + sizeof(TEVENT_HOP_STR));
		PushParam_Value_int( E_PARAM_GSM_HCH, nIndex,  p->Hopping);
		if( 1 == p->Hopping)
		{
			PushParam_Value_int( E_PARAM_GSM_MAIO, nIndex, GetI32(pPacket->MAIO));
			PushParam_Value_int( E_PARAM_GSM_HSN, nIndex,  GetI32(pPacket->HSN));
		}
		else
		{
			PushParam_Value_int( E_PARAM_GSM_MAIO, nIndex, INVALID_VALUE);
			PushParam_Value_int( E_PARAM_GSM_HSN,  nIndex,  INVALID_VALUE);
		}
	}
}

void CEventDecode::PushParam_CI(unsigned int nIndex, unsigned char *pData)
{
	TEVENT_CI_STR *p = (TEVENT_CI_STR *)pData;
	PushSystem(nIndex, p->System);
	if( TEVENT_SYSTEM_GSM == p->System )
	{
		TEVENT_CI_GSM_STR *pPacket = (TEVENT_CI_GSM_STR *)p->Pad;		
		PushParam_Value_float( E_PARAM_GSM_CBI, nIndex, GetFLOAT(pPacket->ChCI));
	}
	else if( TEVENT_SYSTEM_UMTSTD_SCDMA == p->System )
	{
		TEVENT_CI_UMTSTDSCDMA_STR *pPacket = (TEVENT_CI_UMTSTDSCDMA_STR *)p->Pad;		
		PushParam_Value_float( E_PARAM_TD_PCCPCH_CBI, nIndex, GetFLOAT(pPacket->CI));
	}
}

void CEventDecode::PushParam_RXQ(unsigned int nIndex, unsigned char *pData)
{
	TEVENT_RXQ_STR *p = (TEVENT_RXQ_STR *)pData;
	PushSystem(nIndex, p->System);
	if( TEVENT_SYSTEM_GSM == p->System )
	{
		TEVENT_RXQ_GSM_STR *pRXQ = (TEVENT_RXQ_GSM_STR *)p->Pad;
		PushParam_Value_int( E_PARAM_GSM_RXQUAL_FULL, nIndex, pRXQ->RXQfull);
		PushParam_Value_int( E_PARAM_GSM_RXQUAL_SUB,  nIndex, pRXQ->RXQsub);		
	}	
}

void CEventDecode::PushParam_MSP(unsigned int nIndex, unsigned char *pData)
{
	TEVENT_MSP_STR *p = (TEVENT_MSP_STR *)pData;
	PushSystem(nIndex, p->System);
	if( TEVENT_SYSTEM_GSM == p->System )
	{
		TEVENT_MSP_GSM_STR *pPacket = (TEVENT_MSP_GSM_STR *)p->append;

		char anPwrLev2Pwr_GSM900800[32] = {39,39,39,37,35,33,31,29,27,25,23,21,19,17,15,13,11,9,7,5,5,5,5,5,5,5,5,5,5,5,5,5};
		char anPwrLev2Pwr_GSM1800[32]   = {30,28,26,24,22,20,18,16,14,12,10,8,6,4,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,36,34,32};
		char anPwrLev2Pwr_GSM1900[32]   = {30,28,26,24,22,20,18,16,14,12,10,8,6,4,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

		if(pPacket->MSP>32||pPacket->MSP<0) 
		{
			PushParam_Value_int( E_PARAM_GSM_TXPOWER_EX, nIndex, INVALID_VALUE);
			PushParam_Value_int( E_PARAM_GSM_TXPOWER,    nIndex, INVALID_VALUE);	
		}else
		{
			PushParam_Value_int( E_PARAM_GSM_TXPOWER,    nIndex, pPacket->MSP);	
			switch (m_gLogInfo.iLastBand_2g)
			{
			case 1:
			case 2:
				{
					PushParam_Value_int( E_PARAM_GSM_TXPOWER_EX, nIndex, (int)anPwrLev2Pwr_GSM900800[pPacket->MSP]);
					break;
				}
			case 3:
				{
					PushParam_Value_int( E_PARAM_GSM_TXPOWER_EX, nIndex, (int)anPwrLev2Pwr_GSM1800[pPacket->MSP]);
				}
				break;
			case 4:
				{
					PushParam_Value_int( E_PARAM_GSM_TXPOWER_EX, nIndex, (int)anPwrLev2Pwr_GSM1900[pPacket->MSP]);
				}
				break;
			default:
				{
					PushParam_Value_int( E_PARAM_GSM_TXPOWER_EX, nIndex, (int)anPwrLev2Pwr_GSM1800[pPacket->MSP]);
				}
			}
		}
	}
}

void CEventDecode::PushParam_PCHI(unsigned int nIndex, unsigned char *pData)
{
	TEVENT_PCHI_STR *p = (TEVENT_PCHI_STR *)pData;
	PushSystem(nIndex, p->System);
	if( TEVENT_SYSTEM_GSM == p->System )
	{
		TEVENT_PCHI_GSM_STR *pCHI = (TEVENT_PCHI_GSM_STR *)p->pad;
		PushParam_Value_int( E_PARAM_GSM_RAC, nIndex, pCHI->RAC);
		PushParam_Value_int( E_PARAM_GSM_PTECH, nIndex, pCHI->Packettech);

		PushParam_Value_int( E_PARAM_GSM_PTSUP,   nIndex, pCHI->NumPSTSLUL);
		PushParam_Value_int( E_PARAM_GSM_PTSDOWN, nIndex, pCHI->NumPSTSLDL);

		PushParam_Value_int( E_PARAM_GSM_PCODINGUP,   nIndex, pCHI->PScodingUL);
		PushParam_Value_int( E_PARAM_GSM_PCODINGDOWN, nIndex, pCHI->PScodingDL);
	}
	else if( TEVENT_SYSTEM_UMTSTD_SCDMA == p->System )
	{
		TEVENT_PCHI_TDSCDMA_STR *pCHI = (TEVENT_PCHI_TDSCDMA_STR *)p->pad;
		PushParam_Value_int( E_PARAM_TD_SERV_RAC, nIndex, GetI32(pCHI->RAC));
		PushParam_Value_int( E_PARAM_TD_SERV_NMO, nIndex, pCHI->NWoperation);
	}
	else if( TEVENT_SYSTEM_UMTSFDD == p->System )
	{
		TEVENT_PCHI_UMTSFDD_STR *pPacket = (TEVENT_PCHI_UMTSFDD_STR *)(p->pad);

		PushParam_Value_int( E_PARAM_W_RAC, nIndex, GetI32(pPacket->RAC));
		PushParam_Value_float( E_PARAM_W_PWROFFSET, nIndex, GetFLOAT(pPacket->Pwroffset));
		PushParam_Value_float( E_PARAM_W_PLNONMAX, nIndex, GetFLOAT(pPacket->PLnonmax));
		PushParam_Value_float( E_PARAM_W_EDPCCH_PWROFFSET, nIndex, GetFLOAT(pPacket->EDPCCHpoweroffset));		
		PushParam_Value_int( E_PARAM_W_HSDSCHSC, nIndex, GetU16(pPacket->HSDSCHSC));
		PushParam_Value_int( E_PARAM_W_HRNTI, nIndex, GetU16(pPacket->HRNTI));
		PushParam_Value_int( E_PARAM_W_PRIMARY_ERNTI, nIndex, GetU16(pPacket->PrimaryERNTI));
		PushParam_Value_int( E_PARAM_W_SECONDARY_ERNTI, nIndex, GetU16(pPacket->SecondaryERNTI));		
		PushParam_Value_int( E_PARAM_W_HAPPYBITDELAYCOND, nIndex, GetU16(pPacket->Happybitdelaycond));
		PushParam_Value_int( E_PARAM_W_PACKETTECH, nIndex, (pPacket->Packettech));
		PushParam_Value_int( E_PARAM_W_PACKETSTATE, nIndex, (pPacket->Packetstate));
		PushParam_Value_int( E_PARAM_W_NWOPERATION, nIndex, (pPacket->NWoperation));
		PushParam_Value_int( E_PARAM_W_HSDPAUECATEG, nIndex, (pPacket->HSDPAUEcateg));		
		PushParam_Value_int( E_PARAM_W_ACKNACK_REPETITIONS, nIndex, (pPacket->ACKNACKrepetitions));
		PushParam_Value_int( E_PARAM_W_HSUPAUE_CATEG, nIndex, (pPacket->HSUPAUEcateg));
		PushParam_Value_int( E_PARAM_W_TTI, nIndex, (pPacket->TTI));
		PushParam_Value_int( E_PARAM_W_RATEMATCHING, nIndex, (pPacket->Ratematching));
		PushParam_Value_int( E_PARAM_W_AGCHOVSF, nIndex, (pPacket->AGCHOVSF));
		PushParam_Value_int( E_PARAM_W_ETFCITABLE, nIndex, (pPacket->ETFCItable));
	}
	else if( TEVENT_SYSTEM_CDMA1X == p->System || TEVENT_SYSTEM_CDMAONE == p->System )
	{
		//TEVENT_PCHI_CDMA1X_STR *pCHI = (TEVENT_PCHI_CDMA1X_STR *)p->pad;		
	}
	else if( TEVENT_SYSTEM_EVDO == p->System )
	{
		//TEVENT_PCHI_EVDO_STR *pCHI = (TEVENT_PCHI_EVDO_STR *)p->pad;	
	}
}

void CEventDecode::PushParam_SEI(unsigned int nIndex, unsigned char *pData)
{
	TEVENT_SEI_STR *p = (TEVENT_SEI_STR *)pData;
	PushSystem(nIndex, p->System);
	if( TEVENT_SYSTEM_GSM == p->System )
	{
		PushParam_Value_int( E_PARAM_GSM_MNC, nIndex, GetU16(p->MNC));
		PushParam_Value_int( E_PARAM_GSM_MCC, nIndex, GetU16(p->MCC));
		//PushParam_Value_int( E_PARAM_GSM_LAC, nIndex, GetU16(p->LAC));
	}
	else if( TEVENT_SYSTEM_UMTSTD_SCDMA == p->System )
	{
		PushParam_Value_int( E_PARAM_TD_SERV_MNC, nIndex, GetU16(p->MNC));
		PushParam_Value_int( E_PARAM_TD_SERV_MCC, nIndex, GetU16(p->MCC));
		PushParam_Value_int( E_PARAM_TD_SERV_LAC, nIndex, GetU16(p->LAC));
	}
	else if( TEVENT_SYSTEM_UMTSFDD == p->System )
	{
		PushParam_Value_int( E_PARAM_W_SERV_MNC, nIndex, GetU16(p->MNC));
		PushParam_Value_int( E_PARAM_W_SERV_MCC, nIndex, GetU16(p->MCC));
		PushParam_Value_int( E_PARAM_W_SERV_LAC, nIndex, GetU16(p->LAC));
	}
}


void CEventDecode::PushParam_CHI(unsigned int nIndex, unsigned char *pData)
{
	TEVENT_CHI_STR *p = (TEVENT_CHI_STR*)pData;
	PushSystem(nIndex, p->System);
	if( TEVENT_SYSTEM_GSM == p->System )
	{
		TEVENT_CHI_GSM_STR *pPacket = (TEVENT_CHI_GSM_STR *)(p->Pad);
		PushParam_Value_int( E_PARAM_GSM_CI,  nIndex, GetU16(pPacket->CellID) );
		PushParam_Value_int( E_PARAM_GSM_DTX, nIndex, pPacket->DTXUL );
		PushParam_Value_int( E_PARAM_GSM_TN,  nIndex, pPacket->TN );
		PushParam_Value_int( E_PARAM_GSM_VOICE_CODEC,  nIndex, pPacket->Extchtype );
		PushParam_Value_int( E_PARAM_GSM_CHI_CHANNEL,  nIndex, GetI16(pPacket->Ch));
		//PushParam_Value_int( E_PARAM_GSM_BCCH,  nIndex, GetU16(pPacket->BCCHch));
		PushParam_Value_int( E_PARAM_GSM_CH_TYPE,  nIndex, pPacket->Chtype );
	}
	else if( TEVENT_SYSTEM_UMTSTD_SCDMA == p->System )
	{
		TEVENT_CHI_UMTSTDSCDMA_STR *pPacket = (TEVENT_CHI_UMTSTDSCDMA_STR *)(p->Pad);

		PushParam_Value_int( E_PARAM_TD_SERV_UARFCN, nIndex, GetU16(pPacket->Ch));
		PushParam_Value_int( E_PARAM_TD_SERV_CELL_PARAMETER_ID, nIndex, pPacket->CellparamsID );
		PushParam_Value_int( E_PARAM_TD_SERV_CI, nIndex, GetU16(pPacket->CellID) );
		PushParam_Value_int( E_PARAM_TD_SERV_LAC, nIndex, GetU16(pPacket->LAC));
		PushParam_Value_float( E_PARAM_TD_UE_MAX_ALLOWED_UL_TX_POWER, nIndex, (float)(*pPacket->MaxTXpower));
	}
	else if( TEVENT_SYSTEM_UMTSFDD == p->System )
	{
		TEVENT_CHI_UMTSFDD_STR *pPacket = (TEVENT_CHI_UMTSFDD_STR *)(p->Pad);
		//PushParam_Value_int( E_PARAM_W_SERV_UARFCN, nIndex, GetU16(pPacket->Ch));
		PushParam_Value_int( E_PARAM_W_SERV_CI,     nIndex, GetU32(pPacket->CellID)&0xFFFF);
		PushParam_Value_int( E_PARAM_W_SERV_TRESELECTION, nIndex, GetU32(pPacket->Treselection));
		PushParam_Value_float( E_PARAM_W_SERV_ADDITIONWINDOW, nIndex, GetFLOAT(pPacket->Additionwindow));
		PushParam_Value_int( E_PARAM_W_SERV_LAC, nIndex, GetU16(pPacket->LAC));
		PushParam_Value_int( E_PARAM_W_MAX_ALLOWED_TX_POWER, nIndex, pPacket->MaxTXpower);
		PushParam_Value_int( E_PARAM_W_RRC_STATE, nIndex, (pPacket->RRCstate));
		PushParam_Value_int( E_PARAM_W_SERV_BAND, nIndex, (pPacket->Band));
		// 
		// 		E_FLOAT  Dropwindow;/* Drop windowDrop window for event 1B. The value is calculated using the following formula: R1 B + H1 B / 2 where R1B is reporting range constant for event 1B and H1B is hysteresis parameter for event 1B (3GPP TS 25.331). Range: 0 - 16 Unit: dB */
		// 		E_FLOAT  Replacementwindow;/* Replacement window Range: 0 - 2 Unit: dB */
		// 		E_UINT16 Timetotrigger1A;/* Time to trigger 1A Range: 0 - 5000 Unit: ms */
		// 		E_UINT16 Timetotrigger1B;/* Time to trigger 1B Range: 0 - 5000 Unit: ms */
		// 		E_UINT16 Timetotrigger1C;/* Time to trigger 1C Range: 0 - 5000 Unit: ms */
		// 		E_UINT16 DLSF;/* Spreading factor downlinkIf multiple CCTrCh channels are used the reported spreading factor is for CCTrCh with physical control channel. Range: 0 - 512 */
		// 		E_UINT16 MinULSF;/* Min spreading factor uplinkMinimum allowed uplink spreading factor. Range: 4 - 256 */
		// 		E_UINT16 DRXcycle;/* DRX cycle lengthDefines how often paging indications are monitored. Range: 0 - 512 */
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////
void CEventDecode::PushParam_CQI(unsigned int nIndex, unsigned char *pData)
{
	TEVENT_CQI_STR *p = (TEVENT_CQI_STR*)pData;
	PushSystem(nIndex, p->System);
	if (TEVENT_SYSTEM_UMTSFDD == p->System)
	{
		TEVENT_CQI_UMTSFDD_STR *pPacket = (TEVENT_CQI_UMTSFDD_STR *)(pData + sizeof(TEVENT_CQI_STR));
		PushParamMatrix(nIndex, E_PARAM_W_CQI, pPacket->nCQIvalues, (unsigned char *)pPacket->append);
	}
}

//--------------------------------------------------------------------------------------------------------------------------------------------<<
//
//--------------------------------------------------------------------------------------------------------------------------------------------<<
void CEventDecode::PushParamMatrix(unsigned int nIndex, unsigned int nParamID, unsigned int nCnt, unsigned char *pData)
{
	T_PARAM_MATRIX *p = (T_PARAM_MATRIX *)m_pParamList[nParamID].pData;
	if (p == NULL)
	{
		return;
	}
// 	if( NULL != p[nIndex].pList )
// 	{
// 		//说明在同一个时间点，记录了两个值，必须丢掉。
// 		return;
// 	}
	int nMATRIXIndex              = m_pParamList[nParamID].nTail;
	if(nMATRIXIndex>= m_pParamList[nParamID].nMaxCnt)
	{
		return;
	}
	p[nMATRIXIndex].nCnt          = nCnt;
	p[nMATRIXIndex].iCurTimeIndex = nIndex; 
	m_pParamList[nParamID].nTail += 1;
	if( E_PARAM_TYPE_GSM_CELL == m_pParamList[nParamID].nDataType )
	{
		p[nMATRIXIndex].pList = new char[sizeof(TEVENT_CELLMEAS_GSM_PARAMSCELL_STR)*nCnt];
		memcpy(p[nMATRIXIndex].pList, pData, sizeof(TEVENT_CELLMEAS_GSM_PARAMSCELL_STR)*nCnt);	

		for( unsigned int i = 0; i< nCnt; ++i )
		{
			TEVENT_CELLMEAS_GSM_PARAMSCELL_STR *pCell = (TEVENT_CELLMEAS_GSM_PARAMSCELL_STR *)( pData + sizeof(TEVENT_CELLMEAS_GSM_PARAMSCELL_STR)*i );
			if( 1 == pCell->Celltype )
			{
				PushParam_Value_int( E_PARAM_GSM_RXLEV_FULL, nIndex, pCell->RxLevfull);
				PushParam_Value_int( E_PARAM_GSM_CI,         nIndex, GetU16(pCell->CellID));
				PushParam_WithRealRange( E_PARAM_GSM_RXLEV_SUB , nIndex, pCell->RxLevsub);		

				PushParam_Value_int( E_PARAM_GSM_BAND,nIndex, pCell->Band);
				m_gLogInfo.iLastBand_2g = pCell->Band;

				PushParam_Value_int( E_PARAM_GSM_C1 ,   nIndex, pCell->C1);
				PushParam_Value_int( E_PARAM_GSM_C2 ,   nIndex, pCell->C2);
				PushParam_Value_int( E_PARAM_GSM_BCCH,  nIndex, GetU16(pCell->ARFCN));
				PushParam_Value_int( E_PARAM_GSM_LAC,   nIndex, GetU16(pCell->LAC));
				PushParam_WithRealRange( E_PARAM_GSM_RXLEV_N1, nIndex, pCell->RxLevsub);
			}
			else
			{
				if ( i < MAX_CELL_CNT )
				{	
					PushParam_WithRealRange( ( E_PARAM_GSM_RXLEV_N1 + i - 1) , nIndex, pCell->RxLevsub);
				}
			}
		}
	}else if(E_PARAM_TYPE_GSM_COMPLAINT_CELL == m_pParamList[nParamID].nDataType)
	{
		p[nMATRIXIndex].pList = new char[sizeof(TEVENT_CMPMEAS_GSM_PARAMCELL_STR)*nCnt];
		memcpy(p[nMATRIXIndex].pList, pData, sizeof(TEVENT_CMPMEAS_GSM_PARAMCELL_STR)*nCnt);
	}else if (E_PARAM_TYPE_W_COMPLAINT_CELL == m_pParamList[nParamID].nDataType)
	{
		p[nMATRIXIndex].pList = new char[sizeof(TEVENT_CMPMEAS_UTMSFDD_PARAMCELL_STR)*nCnt];
		memcpy(p[nMATRIXIndex].pList, pData, sizeof(TEVENT_CMPMEAS_UTMSFDD_PARAMCELL_STR)*nCnt);
	}
	else if( E_PARAM_TYPE_W_CHANNEL == m_pParamList[nParamID].nDataType )
	{
		p[nMATRIXIndex].pList = new char[sizeof(TEVENT_CELLMEAS_UMTSFDD_PARAMSCHANNEL_STR)*nCnt];
		memcpy(p[nMATRIXIndex].pList, pData, sizeof(TEVENT_CELLMEAS_UMTSFDD_PARAMSCHANNEL_STR)*nCnt);
		TEVENT_CELLMEAS_UMTSFDD_PARAMSCHANNEL_STR *pCellCh = NULL;
		int iMaxCHCnt = MAX_CH_CNT;
		if (nCnt < MAX_CH_CNT)
		{
			iMaxCHCnt = nCnt;
		}
		TEVENT_CELLMEAS_UMTSFDD_PARAMSCHANNEL_STR *pCell = NULL;
		for( unsigned int i = 0; i< iMaxCHCnt; ++i )
		{
			if( i<nCnt )
			{
				pCell = (TEVENT_CELLMEAS_UMTSFDD_PARAMSCHANNEL_STR *)( pData + sizeof(TEVENT_CELLMEAS_UMTSFDD_PARAMSCHANNEL_STR)*i );
				PushParam_Value_float( ( E_PARAM_W_RSSI + i ) , nIndex, GetFLOAT(pCell->RSSI));
			}
			else
			{
				PushParam_Value_float( ( E_PARAM_W_RSSI + i ) , nIndex, (float)INVALID_VALUE);
			}
		}
	}
	else if( E_PARAM_TYPE_W_CELL == m_pParamList[nParamID].nDataType )
	{
		TEVENT_CELLMEAS_UMTSFDD_PARAMSCELL_STR *pCell = (TEVENT_CELLMEAS_UMTSFDD_PARAMSCELL_STR *)pData;
		if(pCell->RSCP>=0)
		{
			return;
		}
		int  iValue = 0;
		int  iActiveRscpCnt = 0;
		int  iActiveIndex   = 0;
		int  iActiveRscp[100] = {0};
		int  iActiveEcNo[100] = {0};//激活集
		int  iBestEcNo        = -10000;
		int  iBestRscp        = -10000;
		bool bIsPush          = true;
		if (nCnt > 100)
		{
			return;
		}
		for( unsigned int i = 0; i< nCnt; ++i )
		{
			pCell = (TEVENT_CELLMEAS_UMTSFDD_PARAMSCELL_STR *)( pData + sizeof(TEVENT_CELLMEAS_UMTSFDD_PARAMSCELL_STR)*i );
			if(pCell->RSCP>=0)
			{
				ClearNode_Matrix(nParamID, nMATRIXIndex);
				break;
			}
			else
			{
				if (0 == pCell->Celltype)//激活集
				{
					if (bIsPush)
					{
						PushParam_Value_int( E_PARAM_W_SERV_SCR ,   nIndex, GetU16(pCell->Scr));
						PushParam_Value_int( E_PARAM_W_SERV_EC_NO,  nIndex, pCell->EcNo);
						PushParam_Value_int( E_PARAM_W_SERV_UARFCN, nIndex, GetU16(pCell->Ch));
						PushParam_Value_int(E_PARAM_W_SERV_RQUAL,   nIndex,  GetI16(pCell->Rqual));
						PushParam_Value_int(E_PARAM_W_SERV_SQUAL,   nIndex,  pCell->Squal);
						PushParam_Value_int(E_PARAM_W_SERV_SRXLEV,  nIndex,  pCell->Srxlev);
						PushParam_Value_int(E_PARAM_W_SERV_RRXLEV,  nIndex,  GetI16(pCell->Rrxlev));
						
						bIsPush = false;
					}
					if (pCell->EcNo > iBestEcNo)
					{
						iBestEcNo = pCell->EcNo;
					}
					iActiveEcNo[iActiveIndex] = pCell->EcNo;
					iActiveIndex++;

					if (pCell->RSCP > iBestRscp)
					{
						iBestRscp = pCell->RSCP;
					}
					iActiveRscp[iActiveRscpCnt] = pCell->RSCP;
					iActiveRscpCnt++;
				}
				if ( i < MAX_CELL_CNT )
				{	
					PushParam_WithRealRange((E_PARAM_W_CPICH_RSCP + i ), nIndex, pCell->RSCP);
				}
			}				
		}
		PushParam_Value_int(E_PARAM_W_SERV_RSCP_COMBIN, nIndex, CombinedParam(iActiveRscp,  iActiveRscpCnt));
		PushParam_Value_int(E_PARAM_W_SERV_EC_NO_COMBIN,nIndex, CombinedParam(iActiveEcNo,  iActiveIndex));
		PushParam_Value_int(E_PARAM_W_SERV_EC_NO_BEST,  nIndex, iBestEcNo);

		p[nMATRIXIndex].pList = new char[sizeof(TEVENT_CELLMEAS_UMTSFDD_PARAMSCELL_STR)*nCnt];
		memcpy(p[nMATRIXIndex].pList, pData, sizeof(TEVENT_CELLMEAS_UMTSFDD_PARAMSCELL_STR)*nCnt);
		
	}
	else if( E_PARAM_TYPE_W_TRCHI == m_pParamList[nParamID].nDataType )
	{
		if (nCnt>0)
		{
			p[nMATRIXIndex].pList = new char[sizeof(TEVENT_RLCBLER_UMTSFDD_PER_CHANNEL_STR)*nCnt];
			memcpy(p[nMATRIXIndex].pList, pData, sizeof(TEVENT_RLCBLER_UMTSFDD_PER_CHANNEL_STR)*nCnt);
		}else
		{
			p[nMATRIXIndex].pList = NULL;
		}

	}else if (E_PARAM_TYPE_W_CQI == m_pParamList[nParamID].nDataType)
	{
		if (nCnt>0)
		{
			int iCQI = 0;
			float fLastValue = 0.0f;
			p[nMATRIXIndex].pList = new char[sizeof(TEVENT_CQI_UMTSFDD_PARAMSCQIVALUE_STR)*nCnt];
			memcpy(p[nMATRIXIndex].pList, pData, sizeof(TEVENT_CQI_UMTSFDD_PARAMSCQIVALUE_STR)*nCnt);
			TEVENT_CQI_UMTSFDD_PARAMSCQIVALUE_STR *pParamCQI = (TEVENT_CQI_UMTSFDD_PARAMSCQIVALUE_STR *)p[nMATRIXIndex].pList;
			for (int c=0; c<nCnt; c++)
			{
				if (fLastValue < GetFLOAT(pParamCQI[c].Percentage))
				{
					fLastValue = GetFLOAT(pParamCQI[c].Percentage);
					iCQI       = pParamCQI[c].CQI;
				}
			}
			PushParam_Value_int(E_PARAM_W_CQI_1, nIndex, iCQI);
		}else
		{
			p[nMATRIXIndex].pList = NULL;
		}

	}else if( E_PARAM_TYPE_TD_CHANNEL == m_pParamList[nParamID].nDataType )
	{
		p[nMATRIXIndex].pList = new char[sizeof(TEVENT_CELLMEAS_UMTSTDSCDMA_PARAMSCHANNEL_STR)*nCnt];
		memcpy(p[nMATRIXIndex].pList, pData, sizeof(TEVENT_CELLMEAS_UMTSTDSCDMA_PARAMSCHANNEL_STR)*nCnt);

		TEVENT_CELLMEAS_UMTSTDSCDMA_PARAMSCHANNEL_STR *pCell = NULL;
		for( unsigned int i = 0; i< MAX_CH_CNT; ++i )
		{
			if( i<nCnt )
			{
				pCell = (TEVENT_CELLMEAS_UMTSTDSCDMA_PARAMSCHANNEL_STR *)( pData + sizeof(TEVENT_CELLMEAS_UMTSTDSCDMA_PARAMSCHANNEL_STR)*i );
				PushParam_Value_int( ( E_PARAM_TD_RSSI + i ) , nIndex, pCell->RSSI);
			}
			else
			{
				PushParam_Value_int( ( E_PARAM_TD_RSSI + i ) , nIndex, INVALID_VALUE);
			}
		}
	}
	else if( E_PARAM_TYPE_TD_CELL == m_pParamList[nParamID].nDataType )
	{
		TEVENT_CELLMEAS_UMTSTDSCDMA_PARAMSCELL_STR *pCell = (TEVENT_CELLMEAS_UMTSTDSCDMA_PARAMSCELL_STR *)pData;
		if(pCell->RSCP>=0)
		{
			return;
		}

		for( unsigned int i = 0; i< MAX_CELL_CNT; ++i )
		{
			if( i<nCnt )
			{
				pCell = (TEVENT_CELLMEAS_UMTSTDSCDMA_PARAMSCELL_STR *)( pData + sizeof(TEVENT_CELLMEAS_UMTSTDSCDMA_PARAMSCELL_STR)*i );
				if(pCell->RSCP>=0)
				{
					ClearNode_Matrix(nParamID, nIndex);
					break;
				}
				else
				{
					PushParam_WithRealRange(( E_PARAM_TD_PCCPCH_RSCP + i ), nIndex, pCell->RSCP);
				}				
			}
			else
			{
				PushParam_WithRealRange(( E_PARAM_TD_PCCPCH_RSCP + i ), nIndex, INVALID_VALUE);
			}
		}
		p[nMATRIXIndex].pList = new char[sizeof(TEVENT_CELLMEAS_UMTSTDSCDMA_PARAMSCELL_STR)*nCnt];
		memcpy(p[nMATRIXIndex].pList, pData, sizeof(TEVENT_CELLMEAS_UMTSTDSCDMA_PARAMSCELL_STR)*nCnt);
		
	}
	else if( E_PARAM_TYPE_TD_TIMESLOT == m_pParamList[nParamID].nDataType )
	{
		p[nMATRIXIndex].pList = new char[sizeof(TEVENT_RXPC_UMTSTDSCDMA_PARAMSTIMESLOT_STR)*nCnt];
		memcpy(p[nMATRIXIndex].pList, pData, sizeof(TEVENT_RXPC_UMTSTDSCDMA_PARAMSTIMESLOT_STR)*nCnt);

		TEVENT_RXPC_UMTSTDSCDMA_PARAMSTIMESLOT_STR *pCell = NULL;
		for( unsigned int i = 0; i< MAX_CELL_CNT; ++i )
		{
			if( i<nCnt )
			{
				pCell = (TEVENT_RXPC_UMTSTDSCDMA_PARAMSTIMESLOT_STR *)( pData + sizeof(TEVENT_RXPC_UMTSTDSCDMA_PARAMSTIMESLOT_STR)*i );
				PushParam_Value_int( ( E_PARAM_TD_DPCH_RSCP + i ) , nIndex, GetI16(pCell->RSCP));				
			}
			else
			{
				PushParam_Value_int( ( E_PARAM_TD_DPCH_RSCP + i ) , nIndex, INVALID_VALUE);				
			}
		}
	}
	else if( E_PARAM_TYPE_TD_TRCHI == m_pParamList[nParamID].nDataType )
	{
		p[nMATRIXIndex].pList = new char[sizeof(TEVENT_RLCBLER_UMTSTDSCDMA_PER_CHANNEL_STR)*nCnt];
		memcpy(p[nMATRIXIndex].pList, pData, sizeof(TEVENT_RLCBLER_UMTSTDSCDMA_PER_CHANNEL_STR)*nCnt);
	}
	else if ( E_PARAM_TYPE_CDMA_CHANNEL == m_pParamList[nParamID].nDataType )
	{
		p[nMATRIXIndex].pList = new char[sizeof(TEVENT_CELLMEAS_CDMAONEANDCDMA1X_PARAMSCHANNEL_STR)*nCnt];
		memcpy(p[nMATRIXIndex].pList, pData, sizeof(TEVENT_CELLMEAS_CDMAONEANDCDMA1X_PARAMSCHANNEL_STR)*nCnt);

		TEVENT_CELLMEAS_CDMAONEANDCDMA1X_PARAMSCHANNEL_STR *pCell = NULL;
		for ( unsigned int i = 0; i< MAX_CELL_CNT; ++i  )
		{
			if( i<nCnt )
			{
				pCell = (TEVENT_CELLMEAS_CDMAONEANDCDMA1X_PARAMSCHANNEL_STR *)( pData + sizeof(TEVENT_CELLMEAS_CDMAONEANDCDMA1X_PARAMSCHANNEL_STR)*i );
				PushParam_Value_float( ( E_PARAM_CDMA_RXPOWR + i ) , nIndex, GetFLOAT(pCell->RXpower));				
			}
			else
			{
				PushParam_Value_float( ( E_PARAM_CDMA_RXPOWR + i ) , nIndex, INVALID_VALUE);				
			}
		}
	}
	else if ( E_PARAM_TYPE_CDMA_CELL == m_pParamList[nParamID].nDataType )
	{
		p[nMATRIXIndex].pList = new char[sizeof(TEVENT_CELLMEAS_CDMAONEANDCDMA1X_PARAMSCELL_STR)*nCnt];
		memcpy(p[nMATRIXIndex].pList, pData, sizeof(TEVENT_CELLMEAS_CDMAONEANDCDMA1X_PARAMSCELL_STR)*nCnt);

		TEVENT_CELLMEAS_CDMAONEANDCDMA1X_PARAMSCELL_STR *pCell = (TEVENT_CELLMEAS_CDMAONEANDCDMA1X_PARAMSCELL_STR *)pData;
		bool bIsPush           = true;
		if (nCnt > 100)
		{
			return;
		}
		for ( unsigned int i = 0; i < MAX_CELL_CNT; ++i )
		{
			PushParam_Value_float( E_PARAM_CDMA_RSCP + i,  nIndex,  GetFLOAT(pCell->RSCP));
		}
		for( unsigned int i = 0; i< nCnt; ++i )
		{
			pCell = (TEVENT_CELLMEAS_CDMAONEANDCDMA1X_PARAMSCELL_STR *)( pData + sizeof(TEVENT_CELLMEAS_CDMAONEANDCDMA1X_PARAMSCELL_STR)*i );
			if (0 == pCell->Set)//激活集
			{
				if (bIsPush)
				{
					PushParam_Value_int( E_PARAM_CDMA_SERV_BAND ,   nIndex,  pCell->Band);
					PushParam_Value_int( E_PARAM_CDMA_SERV_CHANNEL_NUMBER, nIndex,   GetI32(pCell->Ch));
					PushParam_Value_int( E_PARAM_CDMA_SERV_PILOT_NUMVER,  nIndex,   GetI32(pCell->PN));
					PushParam_Value_int( E_PARAM_CDMA_SERV_WALSH ,   nIndex,   GetI32(pCell->Walsh));
					PushParam_Value_float( E_PARAM_CDMA_SERV_RSCP, nIndex,   GetFLOAT(pCell->RSCP));
					PushParam_Value_float( E_PARAM_CDMA_SERV_ECIO,  nIndex,  GetFLOAT(pCell->EcIo));
					bIsPush = false;
				}
				if ( i < MAX_CELL_CNT )
				{
					PushParam_Value_float( E_PARAM_CDMA_RSCP + i,  nIndex,  GetFLOAT(pCell->RSCP));
				}
			}		
		}	
	}
	else if ( E_PARAM_TYPE_EVDO_CHANNEL == m_pParamList[nParamID].nDataType )
	{
		p[nMATRIXIndex].pList = new char[sizeof(TEVENT_CELLMEAS_EVDO_PARAMSCHANNEL_STR)*nCnt];
		memcpy(p[nMATRIXIndex].pList, pData, sizeof(TEVENT_CELLMEAS_EVDO_PARAMSCHANNEL_STR)*nCnt);

		TEVENT_CELLMEAS_EVDO_PARAMSCHANNEL_STR *pCell = NULL;
		for ( unsigned int i = 0; i< MAX_CELL_CNT; ++i  )
		{
			if( i<nCnt )
			{
				pCell = (TEVENT_CELLMEAS_EVDO_PARAMSCHANNEL_STR *)( pData + sizeof(TEVENT_CELLMEAS_EVDO_PARAMSCHANNEL_STR)*i );
				PushParam_Value_float( ( E_PARAM_EVDO_RXPOWR + i ) , nIndex, GetFLOAT(pCell->RXpower));				
			}
			else
			{
				PushParam_Value_float( ( E_PARAM_EVDO_RXPOWR + i ) , nIndex, INVALID_VALUE);				
			}
		}
	}
	else if ( E_PARAM_TYPE_EVDO_CELL == m_pParamList[nParamID].nDataType )
	{
		p[nMATRIXIndex].pList = new char[sizeof(TEVENT_CELLMEAS_EVDO_PARAMSCELL_STR)*nCnt];
		memcpy(p[nMATRIXIndex].pList, pData, sizeof(TEVENT_CELLMEAS_EVDO_PARAMSCELL_STR)*nCnt);

		TEVENT_CELLMEAS_EVDO_PARAMSCELL_STR *pCell = (TEVENT_CELLMEAS_EVDO_PARAMSCELL_STR *)pData;
		bool bIsPush           = true;
		if (nCnt > 100)
		{
			return;
		}
		for ( unsigned int i = 0; i < MAX_CELL_CNT; ++i )
		{
			PushParam_Value_float( E_PARAM_EVDO_RSCP + i,  nIndex,  INVALID_VALUE);
		}
		for( unsigned int i = 0; i< nCnt; ++i )
		{
			pCell = (TEVENT_CELLMEAS_EVDO_PARAMSCELL_STR *)( pData + sizeof(TEVENT_CELLMEAS_EVDO_PARAMSCELL_STR)*i );

			if (0 == pCell->Set)//激活集
			{
				if (bIsPush)
				{
					PushParam_Value_int( E_PARAM_EVDO_SERV_BAND ,   nIndex,   pCell->Band);
					PushParam_Value_int( E_PARAM_EVDO_SERV_CHANNEL_NUMBER, nIndex,   GetI32(pCell->Ch));
					PushParam_Value_int( E_PARAM_EVDO_SERV_PILOT_NUMVER,  nIndex,   GetI32(pCell->PN));
					PushParam_Value_float( E_PARAM_EVDO_SERV_RSCP, nIndex,   GetFLOAT(pCell->RSCP));
					PushParam_Value_float( E_PARAM_EVDO_SERV_ECIO,  nIndex,  GetFLOAT(pCell->EcIo));
					bIsPush = false;
				}
				if ( i < MAX_CELL_CNT )
				{
					PushParam_Value_float( E_PARAM_EVDO_RSCP + i,  nIndex,  GetFLOAT(pCell->RSCP));
				}
			}		
		}	
	}
}

void CEventDecode::PushSystem(unsigned int nIndex, unsigned int nSystem)
{
	T_PARAM_DATA_INT *p = (T_PARAM_DATA_INT *)m_pParamList[E_PARAM_SYSTEM].pData;
	if ( E_PARAM_TYPE_INT != m_pParamList[E_PARAM_SYSTEM].nDataType)
	{
		return;
	}
	bool bPush   = true;
	m_CurSystem  = nSystem;
	int nTimeCnt = m_pParamList[E_PARAM_SYSTEM].nTail;
	if (nTimeCnt>0)
	{
		nTimeCnt--;
		if (p[nTimeCnt].iCurTimeIndex == nIndex)
		{
			bPush = false;
		}
	}
	if (bPush)
	{
		int nIntIndex = m_pParamList[E_PARAM_SYSTEM].nTail;
		p[nIntIndex].iValue        = nSystem;
		p[nIntIndex].iCurTimeIndex = nIndex;
		m_pParamList[E_PARAM_SYSTEM].nTail += 1;
	}

}

void CEventDecode::PushParam_CMPMEAS(unsigned int nIndex, unsigned char *pData)
{
	TEVENT_CMPMEAS_STR* pcmeasStr = (TEVENT_CMPMEAS_STR*)pData;
	m_CurSystem = pcmeasStr->system;
	SetNetTpye(pcmeasStr->system);
	double fLat = 0;
	double fLon = 0;
	PushSystem(nIndex, pcmeasStr->system);
	if (TEVENT_SYSTEM_GSM == pcmeasStr->system)
	{
		TEVENT_CMPMEAS_GSM_STR* pGSMcmeasStr = (TEVENT_CMPMEAS_GSM_STR*)pcmeasStr->pad;
		fLat = GetFLOAT(pGSMcmeasStr->lat);
		fLon = GetFLOAT(pGSMcmeasStr->lon);
		if (g_psTranslate->TransLonLat(fLat, fLon))
		{
			PushParam_Value_float(E_PARAM_GPS_LAT, nIndex, fLat);
			PushParam_Value_float(E_PARAM_GPS_LON, nIndex, fLon);
		}else
		{
			PushParam_Value_float(E_PARAM_GPS_LAT,  nIndex, GetFLOAT(pGSMcmeasStr->lat));
			PushParam_Value_float(E_PARAM_GPS_LON,  nIndex, GetFLOAT(pGSMcmeasStr->lon));
		}
		PushParam_Value_int( E_PARAM_GSM_MNC, nIndex, GetI32(pGSMcmeasStr->mnc));
		PushParam_Value_int( E_PARAM_GSM_MCC, nIndex, GetI32(pGSMcmeasStr->mcc));
		PushParam_Value_int(E_PARAM_GSM_LAC,  nIndex, GetI32(pGSMcmeasStr->lac));
		PushParam_Value_int(E_PARAM_GSM_CI,   nIndex, GetI32(pGSMcmeasStr->cellid));
		PushParam_Value_int(E_PARAM_GSM_RXLEV_SUB, nIndex, (int)(GetFLOAT(pGSMcmeasStr->rxlevelSub)));
		if(pGSMcmeasStr->nCells > MAX_CELLMEAS_NUM_CELLS) return;
		PushParamMatrix(nIndex, E_PARAM_GSM_COMPLANIT_CELLS, pGSMcmeasStr->nCells, pGSMcmeasStr->paramCells);

	}else if (TEVENT_SYSTEM_UMTSFDD == pcmeasStr->system)
	{
		TEVENT_CMPMEAS_UTMSFDD_STR* pWcdmaStr = (TEVENT_CMPMEAS_UTMSFDD_STR*)pcmeasStr->pad;
		fLat = GetFLOAT(pWcdmaStr->lat);
		fLon = GetFLOAT(pWcdmaStr->lon);
		if (g_psTranslate->TransLonLat(fLat, fLon))
		{
			PushParam_Value_float(E_PARAM_GPS_LAT, nIndex, fLat);
			PushParam_Value_float(E_PARAM_GPS_LON, nIndex, fLon);
		}else
		{
			PushParam_Value_float( E_PARAM_GPS_LAT,  nIndex, GetFLOAT(pWcdmaStr->lat));
			PushParam_Value_float( E_PARAM_GPS_LON,  nIndex, GetFLOAT(pWcdmaStr->lon));
		}
		PushParam_Value_int(E_PARAM_W_SERV_CI,   nIndex, GetI32(pWcdmaStr->cellid));
		PushParam_Value_int(E_PARAM_W_SERV_LAC, nIndex,  GetI32(pWcdmaStr->lac));
		PushParam_Value_int(E_PARAM_W_SERV_MCC, nIndex,  GetI32(pWcdmaStr->mcc));
		PushParam_Value_int(E_PARAM_W_SERV_MNC, nIndex,  GetI32(pWcdmaStr->mnc));
		PushParam_Value_int(E_PARAM_W_SERV_SCR, nIndex,  GetI32(pWcdmaStr->src));
		PushParam_Value_int(E_PARAM_W_CPICH_RSCP, nIndex, (int)(GetFLOAT(pWcdmaStr->rscp)));
		PushParam_Value_int(E_PARAM_W_SERV_EC_NO, nIndex, (int)(GetFLOAT(pWcdmaStr->ecno)));
		if(pWcdmaStr->nCells > MAX_CELLMEAS_NUM_CELLS) return;
		PushParamMatrix(nIndex, E_PARAM_W_COMPLANIT_CELLS, pWcdmaStr->nCells, pWcdmaStr->paramCells);
	}
}

void CEventDecode::PushParam_CELLMEAS(unsigned int nIndex, unsigned char *pData)
{
	TEVENT_CELLMEAS_STR *p = (TEVENT_CELLMEAS_STR*)pData;
	PushSystem(nIndex, p->System);
	if( TEVENT_SYSTEM_GSM == p->System )
	{
		TEVENT_CELLMEAS_GSM_STR *pCellPacket = (TEVENT_CELLMEAS_GSM_STR *)p->Pad;
		if(pCellPacket->nCells > MAX_CELLMEAS_NUM_CELLS) return ;
		PushParamMatrix(nIndex, E_PARAM_GSM_CELLS, pCellPacket->nCells, pCellPacket->PARAMSCELL);
	}
	else if( TEVENT_SYSTEM_UMTSTD_SCDMA == p->System)
	{
		TEVENT_CELLMEAS_UMTSTDSCDMA_STR *pCellPacket = (TEVENT_CELLMEAS_UMTSTDSCDMA_STR *)p->Pad;
		if(pCellPacket->nCells > MAX_CELLMEAS_NUM_CELLS) return ;
		if(pCellPacket->nChs > MAX_CELLMEAS_NUM_CH) return ;

		PushParamMatrix(nIndex, E_PARAM_TD_CHANNELS, pCellPacket->nChs,   pCellPacket->PARAMSCHANNEL);
		PushParamMatrix(nIndex, E_PARAM_TD_CELLS,    pCellPacket->nCells, (pCellPacket->PARAMSCHANNEL + pCellPacket->nChs * sizeof(TEVENT_CELLMEAS_UMTSTDSCDMA_PARAMSCHANNEL_STR)));

		//Get Server Param
	}
	else if( TEVENT_SYSTEM_UMTSFDD == p->System )
	{
		TEVENT_CELLMEAS_UMTSFDD_STR *pCellPacket = (TEVENT_CELLMEAS_UMTSFDD_STR *)p->Pad;
		if(pCellPacket->nCells > MAX_CELLMEAS_NUM_CELLS) return ;
		if(pCellPacket->nChs > MAX_CELLMEAS_NUM_CH) return ;

		PushParamMatrix(nIndex, E_PARAM_W_CHANNELS, pCellPacket->nChs,   pCellPacket->PARAMSCHANNEL);
		PushParamMatrix(nIndex, E_PARAM_W_CELLS,    pCellPacket->nCells, (pCellPacket->PARAMSCHANNEL + pCellPacket->nChs * sizeof(TEVENT_CELLMEAS_UMTSFDD_PARAMSCHANNEL_STR)));

		//Get Server Param
	}
	else if( TEVENT_SYSTEM_CDMA1X == p->System || TEVENT_SYSTEM_CDMAONE == p->System )
	{
		TEVENT_CELLMEAS_CDMAONEANDCDMA1X_STR *pCellPacket = (TEVENT_CELLMEAS_CDMAONEANDCDMA1X_STR *)p->Pad;
		if(pCellPacket->nCells > MAX_CELLMEAS_NUM_CELLS) return ;
		if(pCellPacket->nChs > MAX_CELLMEAS_NUM_CH) return ;

		PushParamMatrix(nIndex, E_PARAM_CDMA_CHANNELS, pCellPacket->nChs,   pCellPacket->PARAMSCHANNEL);
		PushParamMatrix(nIndex, E_PARAM_CDMA_CELLS,    pCellPacket->nCells, (pCellPacket->PARAMSCHANNEL + pCellPacket->nChs * sizeof( _TEVENT_CELLMEAS_CDMAONEANDCDMA1X_PARAMSCHANNEL_STR_ )));

	}
	else if ( TEVENT_SYSTEM_EVDO == p->System )
	{
		TEVENT_CELLMEAS_EVDO_STR *pCellPacket = (TEVENT_CELLMEAS_EVDO_STR *)p->Pad;
		if(pCellPacket->nChs > MAX_CELLMEAS_NUM_CH) return ;
		if(pCellPacket->nCells > MAX_CELLMEAS_NUM_CELLS) return ;

		PushParamMatrix(nIndex, E_PARAM_EVDO_CHANNELS, pCellPacket->nChs,   pCellPacket->PARAMSCHANNEL);
		PushParamMatrix(nIndex, E_PARAM_EVDO_CELLS,    pCellPacket->nCells, (pCellPacket->PARAMSCHANNEL + pCellPacket->nChs * sizeof(TEVENT_CELLMEAS_EVDO_PARAMSCHANNEL_STR)));
	}
}



//////////////////////////////////////////////////////////////////////////
unsigned int CEventDecode::GetTimePointCnt()
{
	if( false == m_gLogInfo.bLoad ) return 0;
	return m_gLogInfo.nTimePointCnt;
}

int CEventDecode::GetParamValue_int(unsigned int nParamID,   unsigned int nTimeIndex)
{
	if (m_pParamList[nParamID].nDataType != E_PARAM_TYPE_INT)
	{
		return INVALID_VALUE;
	}
	T_PARAM_DATA_INT *pParamValueList = (T_PARAM_DATA_INT*)m_pParamList[nParamID].pData;
	if (NULL == pParamValueList)
	{
		return INVALID_VALUE;
	}
	int iMinIndex = 0;
	int iMaxIndex = m_pParamList[nParamID].nTail - 1;
	if (pParamValueList[0].iCurTimeIndex>= nTimeIndex)
	{
		return pParamValueList[0].iValue;
	}
	if (nTimeIndex >= pParamValueList[iMaxIndex].iCurTimeIndex)
	{
		return pParamValueList[iMaxIndex].iValue;
	}
	while(iMinIndex <= iMaxIndex) //二分查找法
	{
		int nMid = (iMaxIndex+iMinIndex)/2; 
		if (pParamValueList[nMid].iCurTimeIndex == nTimeIndex)
		{
			return pParamValueList[nMid].iValue;

		}else if (pParamValueList[nMid].iCurTimeIndex > nTimeIndex)
		{
			iMaxIndex = nMid - 1;
			if ((iMaxIndex - iMinIndex) <=20)
			{
				break;
			}
		}else 
		{
			iMinIndex = nMid + 1;
			if ((iMaxIndex - iMinIndex) <=20)
			{
				break;

			}
		}
	}
	int iValue = INVALID_VALUE;
	if (pParamValueList[iMinIndex].iCurTimeIndex > nTimeIndex)
	{
		iValue = pParamValueList[iMinIndex-1].iValue;
	}else
	{
		for (int n=iMinIndex; n<=iMaxIndex; n++)
		{
			if (pParamValueList[n].iCurTimeIndex > nTimeIndex)  ////sprintf(szValue, "Value:%d--->Time:%d\n", pParamValueList[n].iValue, pParamValueList[n].iCurTimeIndex);	//::OutputDebugStr(szValue);
			{
				break;
			}
			iValue = pParamValueList[n].iValue;
		}
	}
	return iValue;
}

float CEventDecode::GetParamValue_float(unsigned int nParamID, unsigned int nTimeIndex)
{
	if (m_pParamList[nParamID].nDataType != E_PARAM_TYPE_FLOAT)
	{
		return INVALID_VALUE;
	}
	T_PARAM_DATA_FLOAT *pParamValueList = (T_PARAM_DATA_FLOAT*)m_pParamList[nParamID].pData;
	if (NULL == pParamValueList)
	{
		return INVALID_VALUE;
	}
	int iMinIndex = 0;
	int iMaxIndex = m_pParamList[nParamID].nTail - 1;
	if (pParamValueList[0].iCurTimeIndex >= nTimeIndex)
	{
		return pParamValueList[0].fVale;
	}
	if (nTimeIndex >= pParamValueList[iMaxIndex].iCurTimeIndex)
	{
		return pParamValueList[iMaxIndex].fVale;
	}
	while(iMinIndex <= iMaxIndex) //二分查找法
	{
		int nMid = (iMaxIndex+iMinIndex)/2; 
		if (pParamValueList[nMid].iCurTimeIndex == nTimeIndex)
		{
			return pParamValueList[nMid].fVale;

		}else if (pParamValueList[nMid].iCurTimeIndex > nTimeIndex)
		{
			iMaxIndex = nMid - 1;
			if ((iMaxIndex - iMinIndex) <=20)
			{
				break;
			}
		}else 
		{
			iMinIndex = nMid + 1;
			if ((iMaxIndex - iMinIndex) <=20)
			{
				break;
			}
		}
	}
	float fValue = INVALID_VALUE;
	if (pParamValueList[iMinIndex].iCurTimeIndex > nTimeIndex)
	{
		fValue = pParamValueList[iMinIndex-1].fVale;
	}else
	{
		for (int n=iMinIndex; n<=iMaxIndex; n++)
		{
			if (pParamValueList[n].iCurTimeIndex > nTimeIndex)  ////sprintf(szValue, "Value:%d--->Time:%d\n", pParamValueList[n].iValue, pParamValueList[n].iCurTimeIndex);	//::OutputDebugStr(szValue);
			{
				break;
			}
			fValue = pParamValueList[n].fVale;
		}
	}
	return fValue;
}

//--------------------------------------------------------------------------------------------------------------------------------------------<<
//
//--------------------------------------------------------------------------------------------------------------------------------------------<<
void CEventDecode::PushParam(unsigned int nIndex, unsigned int nTime, unsigned int nParamID, unsigned char *pData)
{
	//TRACE( "## Time = %02d:%02d:%02d.%03d \r\n", TIME_GET_HOUR(nTime), TIME_GET_MIN(nTime), TIME_GET_SEC(nTime), TIME_GET_MSEC(nTime));
	if( TEVENT_ID_CELLMEAS == nParamID )
	{
		PushParam_CELLMEAS(nIndex, pData);
	}else if (TEVENT_ID_CMPMEAS == nParamID)
	{
		PushParam_CMPMEAS(nIndex, pData);
	}
	else if (TEVENT_ID_CQI == nParamID)
	{
		PushParam_CQI(nIndex, pData);
	}else if( TEVENT_ID_CHI == nParamID )
	{
		PushParam_CHI(nIndex, pData);
	}
	else if( TEVENT_ID_RRCSM == nParamID )
	{
		PushParam_RRCSM(nParamID, nIndex, nTime, pData);
	}
	else if( TEVENT_ID_L3SM == nParamID )
	{
		PushParam_L3SM(nParamID, nIndex, nTime, pData);
	}
	else if( TEVENT_ID_MACSM == nParamID )
	{
		PushParam_MACSM(nParamID, nIndex, nTime, pData);
	}
	else if( TEVENT_ID_SNPSM == nParamID )
	{
		PushParam_SNPSM(nParamID, nIndex, nTime, pData);
	}
	else if( TEVENT_ID_SEI == nParamID )
	{
		PushParam_SEI(nIndex, pData);
	}
	else if( TEVENT_ID_PCHI == nParamID )
	{
		PushParam_PCHI(nIndex, pData);
	}
	else if( TEVENT_ID_RXQ == nParamID )
	{
		PushParam_RXQ(nIndex, pData);
	}
	else if( TEVENT_ID_CI == nParamID )
	{
		PushParam_CI(nIndex, pData);
	}
	else if( TEVENT_ID_MSP == nParamID )
	{
		PushParam_MSP(nIndex, pData);
	}
	else if( TEVENT_ID_TAD == nParamID )
	{
		PushParam_TAD(nIndex, pData);
	}
	else if( TEVENT_ID_HOP == nParamID )
	{
		PushParam_HOP(nIndex, pData);
	}
	else if( TEVENT_ID_TBFI == nParamID )
	{
		PushParam_TBFI(nIndex, pData);
	}
	else if( TEVENT_ID_BEP == nParamID )
	{
		PushParam_BEP(nIndex, pData);
	}
	else if( TEVENT_ID_PRXQ == nParamID )
	{
		PushParam_PRXQ(nIndex, pData);
	}
	else if( TEVENT_ID_FER == nParamID )
	{
		PushParam_FER(nIndex, pData);
	}
	else if( TEVENT_ID_RLT == nParamID )
	{
		PushParam_RLT(nIndex, pData);
	}
	else if( TEVENT_ID_DRATE == nParamID )
	{
		PushParam_DRATE(nIndex, pData);
	}
	else if( TEVENT_ID_RLCRATE == nParamID )
	{
		PushParam_RLCRATE(nIndex, pData);
	}
	else if ( TEVENT_ID_RLPRATE == nParamID )
	{
		PushParam_RLPRATE(nIndex, pData);
	}
	else if( TEVENT_ID_TXPC == nParamID )
	{
		PushParam_TXPC(nIndex, pData);
	}
	else if( TEVENT_ID_RXPC == nParamID )
	{
		PushParam_RXPC(nIndex, pData);
	}
	else if( TEVENT_ID_RLCBLER == nParamID )
	{
		PushParam_RLCBLER(nIndex, pData);
	}else if(TEVENT_ID_RACHI == nParamID)
	{
		PushParam_RACHI(nIndex, pData);
	}
	//Event : call
	else if( TEVENT_ID_CAA == nParamID ) //------>>>>>
	{
		PushParam_CAA(nIndex, nTime, pData);
	}else if (TEVENT_ID_CAC_GSM == nParamID)
	{
		PushParamGSM_CAC(nIndex, nTime, pData);
	}
	else if( TEVENT_ID_CAC == nParamID )
	{
		PushParam_CAC(nIndex, nTime, pData);
	}
	else if( TEVENT_ID_CAD == nParamID )
	{
		PushParam_CAD(nIndex, nTime, pData);
	}
	else if( TEVENT_ID_CAF == nParamID )
	{
		PushParam_CAF(nIndex, nTime, pData);
	}//------>>>
	//Event : Data serviec
	else if( TEVENT_ID_DAA == nParamID )
	{
		PushParam_DAA(nIndex, nTime, pData);
	}
	else if( TEVENT_ID_DAC == nParamID )
	{
		PushParam_DAC(nIndex, nTime, pData);
	}
	else if( TEVENT_ID_DAD == nParamID )
	{
		PushParam_DAD(nIndex, nTime, pData);
	}
	else if( TEVENT_ID_DAF == nParamID )
	{
		PushParam_DAF(nIndex, nTime, pData);
	}
	//Event : Data transfer request
	else if( TEVENT_ID_DREQ == nParamID )
	{
		PushParam_DREQ(nIndex, nTime, pData);
	}
	else if( TEVENT_ID_DCOMP == nParamID )
	{
		PushParam_DCOMP(nIndex, nTime, pData);
	}
	//Event : handover
	else if( TEVENT_ID_HOA == nParamID )
	{
		PushParam_HOA(nIndex, nTime, pData);
	}
	else if( TEVENT_ID_HOS == nParamID )
	{
		PushParam_HOS(nIndex, nTime, pData);
	}
	else if( TEVENT_ID_HOF == nParamID )
	{
		PushParam_HOF(nIndex, nTime, pData);
	}///-------------------------------------------->
	// Soft handover (SHO)
	else if( TEVENT_ID_SHO == nParamID )
	{
		PushParam_SHO(nIndex, nTime, pData);
	}
	//Event : Cell reselection
	else if( TEVENT_ID_CREL == nParamID )
	{
		PushParam_CREL(nIndex, nTime, pData);
	}
	//Event : Location update
	else if( TEVENT_ID_LUA == nParamID )
	{
		PushParam_LUA(nIndex, nTime, pData);
	}
	else if( TEVENT_ID_LUS == nParamID )
	{
		PushParam_LUS(nIndex, nTime, pData);
	}
	else if( TEVENT_ID_LUF == nParamID )
	{
		PushParam_LUF(nIndex, nTime, pData);
	}
	//Event : Radio resource connection attempt
	else if( TEVENT_ID_RRA == nParamID )
	{
		PushParam_RRA(nIndex, nTime, pData);
	}
	else if( TEVENT_ID_RRC == nParamID )
	{
		PushParam_RRC(nIndex, nTime, pData);
	}
	else if( TEVENT_ID_RRF == nParamID )
	{
		PushParam_RRF(nIndex, nTime, pData);
	}
	else if( TEVENT_ID_RRD == nParamID )
	{
		PushParam_RRD(nIndex, nTime, pData);
	}
	//Event : Packet session activation
	else if( TEVENT_ID_PAA == nParamID )
	{
		PushParam_PAA(nIndex, nTime, pData);
	}
	else if( TEVENT_ID_PAC == nParamID )
	{
		PushParam_PAC(nIndex, nTime, pData);
	}
	else if( TEVENT_ID_PAF == nParamID )
	{
		PushParam_PAF(nIndex, nTime, pData);
	}
	else if( TEVENT_ID_PAD == nParamID )
	{
		PushParam_PAD(nIndex, nTime, pData);
	}
	//Event : GPRS
	else if( TEVENT_ID_GAA == nParamID )
	{
		PushParam_GAA(nIndex, nTime, pData);
	}
	else if( TEVENT_ID_GAC == nParamID )
	{
		PushParam_GAC(nIndex, nTime, pData);
	}
	else if( TEVENT_ID_GAF == nParamID )
	{
		PushParam_GAF(nIndex, nTime, pData);
	}
	else if( TEVENT_ID_GAD == nParamID )
	{
		PushParam_GAD(nIndex, nTime, pData);
	}
	//Event : Routing area update
	else if( TEVENT_ID_RUA == nParamID )
	{
		PushParam_RUA(nIndex, nTime, pData);
	}
	else if( TEVENT_ID_RUF == nParamID )
	{
		PushParam_RUF(nIndex, nTime, pData);
	}
	else if( TEVENT_ID_RUS == nParamID )
	{
		PushParam_RUS(nIndex, nTime, pData);
	}
	//Event : Message sending/receiving
	else if( TEVENT_ID_MSGA == nParamID )
	{
		PushParam_MSGA(nIndex, nTime, pData);
	}
	else if( TEVENT_ID_MSGS == nParamID )
	{
		PushParam_MSGS(nIndex, nTime, pData);
	}
	else if( TEVENT_ID_MSGF == nParamID )
	{
		PushParam_MSGF(nIndex, nTime, pData);
	}
	else if( TEVENT_ID_GPS == nParamID )
	{
		PushParam_GPS(nIndex, pData);
	}
	else if( TEVENT_ID_INDOORMARK == nParamID )
	{
		PushParam_INDOOR_MARK(nIndex, pData);
	}else if(TEVENT_ID_INDOORMARK_DEL == nParamID)
	{
		Delete_Indoor_Mark(pData);

	}else if (TEVENT_ID_AQUL == nParamID)
	{
		PushParam_AQUL(nIndex, pData);
	}else if (TEVENT_ID_AQDL == nParamID)
	{
		PushParam_AQDL(nIndex, pData);
	}else if (TEVENT_ID_TEST_LOCK_NETWORK == nParamID)
	{
		TEVENT_TEST_LOCK_NETWORK_STR *pTestLockNetWork = (TEVENT_TEST_LOCK_NETWORK_STR*)pData;
		m_gLogInfo.iLockNetType      = pTestLockNetWork->lockNetwork;
		m_gLogInfo.iLockNetTimePonit = nIndex;
	}else if (TEVENT_ID_CMPEV == nParamID)
	{
		PushParam_CMPEV(nIndex, nTime, pData);
	}else if(TEVENT_ID_USRBHV == nParamID)
	{
		PushParam_USRBHVEV(nIndex, pData);
	}else if (TEVENT_ID_HTTP_URL == nParamID)
	{
		PushParam_ComplanitHTTP(nIndex, nTime, pData);
	}else if (TEVENT_ID_APPINFO == nParamID)
	{
		PushParam_ComplanitAPPInfo(nIndex, nTime, pData);
	}else if (TEVENT_ID_APP_OPEN_STATS == nParamID)
	{
		PushParam_APPOpenInfo(nIndex, nTime, pData);
	}else if (TEVENT_ID_OFF_NET == nParamID)
	{
		PushParam_OffNet(nIndex, nTime, pData);
	}
}

//--------------------------------------------------------------------------------------------------------------------------------------------<<
//
//--------------------------------------------------------------------------------------------------------------------------------------------<<
void CEventDecode::PushTimeLine(unsigned int nIndex, unsigned int ntime)
{
	T_TIME_LINE *p = (T_TIME_LINE *)m_pParamList[E_PARAM_TIME_LINE].pData;
	int iTimeIndex = m_pParamList[E_PARAM_TIME_LINE].nTail;
	p[iTimeIndex].nTime = ntime;
	m_pParamList[E_PARAM_TIME_LINE].nTail += 1;
	//	TRACE( ">> %d, Time = %02d:%02d:%02d.%03d \r\n", nIndex, TIME_GET_HOUR(p[nIndex].nTime), TIME_GET_MIN(p[nIndex].nTime), TIME_GET_SEC(p[nIndex].nTime), TIME_GET_MSEC(p[nIndex].nTime));
}

//--------------------------------------------------------------------------------------------------------------------------------------------<<
//
//--------------------------------------------------------------------------------------------------------------------------------------------<<
int CEventDecode::PushParamList(FILE *pFile)
{
	unsigned char headBuf[30]               = {0};
	unsigned char packetBuf[MAX_PACKET_LEN] = {0};
	int iIndex = -1;
	unsigned int readlen = 0, time = 0, nOffset = 4, nPacketHeadSize = sizeof(T_EVENT_PACKAGE_HEADER)-4;

	T_EVENT_PACKAGE_HEADER *pPacket = NULL;
	if(NULL == pFile)
	{
		return E_RESULT_FILE_NOT_EXISTING;
	}
	fseek(pFile, 0L, SEEK_SET);
	do
	{
		readlen = fread(headBuf, 1, nPacketHeadSize, pFile);
		if(readlen < nPacketHeadSize)
		{
			break;         // enf of file
		}
		pPacket = (T_EVENT_PACKAGE_HEADER*)headBuf;
		if(	TEVENT_PACKET_HEADER == pPacket->PackageFlag )
		{
			if( MAX_PACKET_LEN < pPacket->nPacketLen )
			{
				continue;
			}
			if(pPacket->nPacketLen <= nPacketHeadSize)
			{
				return E_RESULT_FILE_NULL;
			}

			if( time != pPacket->time && pPacket->time != 0 )
			{
				iIndex++;
				PushTimeLine(iIndex, pPacket->time);
				m_gLogInfo.nCurTimePoint = iIndex;
				time = pPacket->time;
			}
			fread(packetBuf, 1, (pPacket->nPacketLen - nPacketHeadSize), pFile);
			if( pPacket->time != 0)			
			{
				PushParam(iIndex, pPacket->time, pPacket->EventID, packetBuf);
			}
		}
		else
		{
			continue;
		}

	}while(1);
	return E_RESULT_OK;
}

//--------------------------------------------------------------------------------------------------------------------------------------------<<<<<
//
//--------------------------------------------------------------------------------------------------------------------------------------------<<<<<
bool CEventDecode::CreateNodeBuf(unsigned int nParamID)
{
	unsigned int nNodeSize = 0;
	int iInitValue = 0;

	if( E_PARAM_TYPE_TIME == m_pParamList[nParamID].nDataType )
	{
		nNodeSize = sizeof(unsigned int);
	}
	else if( E_PARAM_TYPE_INT == m_pParamList[nParamID].nDataType )
	{
		nNodeSize  = sizeof(T_PARAM_DATA_INT);
		iInitValue = INVALID_VALUE;
	}
	else if( E_PARAM_TYPE_FLOAT == m_pParamList[nParamID].nDataType )
	{
		nNodeSize = sizeof(T_PARAM_DATA_FLOAT);
	}
	else if(( E_PARAM_TYPE_GSM_CELL    == m_pParamList[nParamID].nDataType )||
		( E_PARAM_TYPE_TD_CHANNEL  == m_pParamList[nParamID].nDataType )||
		( E_PARAM_TYPE_TD_CELL     == m_pParamList[nParamID].nDataType )||
		( E_PARAM_TYPE_TD_TIMESLOT == m_pParamList[nParamID].nDataType )||
		( E_PARAM_TYPE_TD_TRCHI    == m_pParamList[nParamID].nDataType )||
		( E_PARAM_TYPE_W_CHANNEL   == m_pParamList[nParamID].nDataType )||
		( E_PARAM_TYPE_W_CELL      == m_pParamList[nParamID].nDataType )||
		( E_PARAM_TYPE_W_TRCHI     == m_pParamList[nParamID].nDataType )||
		( E_PARAM_TYPE_CDMA_CHANNEL== m_pParamList[nParamID].nDataType )||
		( E_PARAM_TYPE_CDMA_CELL   == m_pParamList[nParamID].nDataType )||
		( E_PARAM_TYPE_EVDO_CHANNEL== m_pParamList[nParamID].nDataType )||
		( E_PARAM_TYPE_EVDO_CELL   == m_pParamList[nParamID].nDataType )||
		( E_PARAM_TYPE_W_CQI       == m_pParamList[nParamID].nDataType )||
		( E_PARAM_TYPE_GSM_COMPLAINT_CELL == m_pParamList[nParamID].nDataType)||
		( E_PARAM_TYPE_W_COMPLAINT_CELL == m_pParamList[nParamID].nDataType)
		)
	{
		nNodeSize = sizeof(T_PARAM_MATRIX);
	}
	else
	{
		return false;
	}
	int nCnt = m_pParamList[nParamID].nMaxCnt;
	if (nCnt <=0)
	{
		return false;
	}
	m_pParamList[nParamID].pData = new char[nCnt*nNodeSize];
	m_pParamList[nParamID].nTail = 0;
	if( NULL == m_pParamList[nParamID].pData ) return false;
	memset(m_pParamList[nParamID].pData, iInitValue, nNodeSize*nCnt);

	if( ( E_PARAM_TYPE_GSM_CELL     == m_pParamList[nParamID].nDataType )||
		( E_PARAM_TYPE_TD_CHANNEL  == m_pParamList[nParamID].nDataType )||
		( E_PARAM_TYPE_TD_CELL     == m_pParamList[nParamID].nDataType )||
		( E_PARAM_TYPE_TD_TIMESLOT == m_pParamList[nParamID].nDataType )||
		( E_PARAM_TYPE_TD_TRCHI    == m_pParamList[nParamID].nDataType )||
		( E_PARAM_TYPE_W_CHANNEL   == m_pParamList[nParamID].nDataType )||
		( E_PARAM_TYPE_W_CELL      == m_pParamList[nParamID].nDataType )||
		( E_PARAM_TYPE_W_TRCHI     == m_pParamList[nParamID].nDataType )||
		( E_PARAM_TYPE_CDMA_CHANNEL== m_pParamList[nParamID].nDataType )||
		( E_PARAM_TYPE_CDMA_CELL   == m_pParamList[nParamID].nDataType )||
		( E_PARAM_TYPE_EVDO_CHANNEL== m_pParamList[nParamID].nDataType )||
		( E_PARAM_TYPE_EVDO_CELL   == m_pParamList[nParamID].nDataType )||
		( E_PARAM_TYPE_W_CQI       == m_pParamList[nParamID].nDataType )||
		( E_PARAM_TYPE_GSM_COMPLAINT_CELL == m_pParamList[nParamID].nDataType)||
	    ( E_PARAM_TYPE_W_COMPLAINT_CELL == m_pParamList[nParamID].nDataType)
		)
	{
		T_PARAM_MATRIX *p = (T_PARAM_MATRIX *)m_pParamList[nParamID].pData;
		for(unsigned int i=0; i<nCnt; ++i)
		{
			p[i].iCurTimeIndex = -1;
			p[i].nCnt          = 0;
			p[i].pList         = NULL;
		}
		m_pParamList[nParamID].nTail = 0;
	}
	return true;
}
