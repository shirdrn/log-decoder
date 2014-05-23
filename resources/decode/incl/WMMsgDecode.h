#ifndef _MSG_DECODE_H_
#define _MSG_DECODE_H_

#define DIRECTION_DOWN   2
#define DIRECTION_UP     1
#define DIRECTION_ALL    0
/************************************************************************/
#define DE_MODULE_L2   0x00
#define DE_MODULE_CC   0x03
#define DE_MODULE_MM   0x05
#define DE_MODULE_RR   0x06
#define DE_MODULE_GMM  0x08
#define DE_MODULE_SM   0x0a
#define DE_MODULE_SS   0x0b
//#define MODULE_TST  0x0f
#define DE_MODULE_SMS  0x09
#define DE_MODULE_RRC  0xef
#define DE_MODULE_GRR  0xff/* this value is not defined in protocol, and self define */
/********************channel type of msg begin**************************/
#define  CHANNEL_TYPE_UNKNOWN  0x0000
#define  CHANNEL_TYPE_FACCH        0x0001
#define  CHANNEL_TYPE_CCCH          0x0002
#define  CHANNEL_TYPE_RACH          0x0004
#define  CHANNEL_TYPE_SACCH        0x0008
#define  CHANNEL_TYPE_SDCCH        0x0010
#define  CHANNEL_TYPE_BCCH          0x0020
#define  CHANNEL_TYPE_PACCH        0x0040
#define  CHANNEL_TYPE_PBCCH        0x0080
#define  CHANNEL_TYPE_PCCCH        0x0100
#define  CHANNEL_TYPE_DCCH          0x0200
#define  CHANNEL_TYPE_PDTCH        0x0400
#define  CHANNEL_TYPE_PRACH        0x0800
#define  CHANNEL_TYPE_PCCH			0x1000
#define	 CHANNEL_TYPE_SHCCH			0x2000
/********************channel type of msg end**************************/
/************************************************************************/
#define  DECODE_GSM_OTHER_MSG                       0
#define  DECODE_GSM_CHANNEL_REQUEST                 1
#define  DECODE_RLC_MAC_MSG_WITH_HEADER             2
#define  DECODE_RLC_MAC_MSG_WITHOUT_HEADER          3
#define  DECODE_PACKET_CTRL_ACK_8BIT                4
#define  DECODE_PACKET_CHANNE_REQUEST_8BIT          5
#define  DECODE_PACKET_CHANNE_REQUEST_11BIT         6
#define  DECODE_EGPRS_PACKET_CHANNEL_REQUEST_11BIT  7
#define  DECODE_PACKET_CTRL_ACK_11BIT               8
#define  DECODE_GSM_L2_MSG							9
#define  DECODE_RRC_DL_DCCH							10
#define  DECODE_RRC_UL_DCCH							11
#define  DECODE_RRC_DL_CCCH							12
#define  DECODE_RRC_UL_CCCH							13
#define  DECODE_RRC_PCCH							14
#define  DECODE_RRC_DL_SHCCH						15
#define  DECODE_RRC_UL_SHCCH						16
#define  DECODE_RRC_BCCH_FACH						17
#define  DECODE_RRC_BCCH_BCH						18
#define  DECODE_RRC_HO_TO_CMD						19
#define  DECODE_RRC_MIB								20
#define  DECODE_RRC_SIB1							21
#define  DECODE_RRC_SIB2							22
#define  DECODE_RRC_SIB3							23
#define  DECODE_RRC_SIB4							24
#define  DECODE_RRC_SIB5							25
#define  DECODE_RRC_SIB6							26
#define  DECODE_RRC_SIB7							27
#define  DECODE_RRC_SIB11							28
#define  DECODE_RRC_SIB12							29
#define  DECODE_RRC_SIB18							30
#define  DECODE_RRC_SB1								31
#define  DECODE_RRC_SB2								32
#define  DECODE_RRC_SIB15_5							33
#define  DECODE_UNKNOW								0xff
/************************************************************************/
#define  GSM_MSG_TYPE_CHANNEL_REQUEST       0xff
#define  GSM_MSG_TYPE_CTRL_ACK_8BIT              0xef
#define  GSM_MSG_TYPE_PKT_CHANNEL_REQ_8BIT  0xdf
#define  GSM_MSG_TYPE_PKT_CHANNEL_REQ_11BIT 0xcf
#define  GSM_MSG_TYPE_EPKT_CHANNEL_REQ_11BIT 0xbf
#define  GSM_MSG_TYPE_CTRL_ACK_11BIT              0xcf
/************************************************************************/
/*                                                                      */
/************************************************************************/
/*************************************************************/
/*****************TLV format***********************************/
typedef enum {
	PRESENCE_M, PRESENCE_C, PRESENCE_O
} IE_PRESENCE;
typedef struct _L3MSG_RESULT_PARAM {
    int     nParamId;
    wchar_t *pName;
    wchar_t *pMemName;
	//    IEPARAM_TYPE nType;
    unsigned int nValue;
    wchar_t *pDesp;
    wchar_t *pMemDesp;
    struct _L3MSG_RESULT_PARAM *pNext;
#define DE_PARAM_V_CHANNEL_INVAILD  0
#define DE_PARAM_V_CHANNEL_SDCCH    1
#define DE_PARAM_V_CHANNEL_FACCH    2
    unsigned char    byChTy;
} L3MSG_RESULT_PARAM;

typedef struct _L3MSG_RESULT_IE {
    int                 nID;
    wchar_t             *pDesp;
    int                 nLen;
    unsigned char       byIEI;
    IE_PRESENCE         enPre;
    unsigned char                *pValue;
    L3MSG_RESULT_PARAM  *pParam;
	
    struct _L3MSG_RESULT_IE *pNext;
} L3MSG_RESULT_IE;
typedef struct _L3MSG_RESULT_MSG {
    unsigned char            byModule;  /*  the module of msg  */
    unsigned char            byType;    /*  msg type */
    int   enDir;     /*  msg direction*/
    wchar_t         *pDesp;    /*  the name of msg */
    unsigned char            *pData;    /*  the encoded data  */
    int             nLen;      /*  the length of the encoded data */
    unsigned short  byChannelType;
    struct _L3MSG_RESULT_IE     *pIEHead;
    struct _L3MSG_RESULT_IE     *pIETail;
    struct _L3MSG_RESULT_MSG    *pNext;
} L3MSG_RESULT_MSG;

/************************************************************************/
/*************    *******************************************************/
typedef struct _MSG_FIELD_INFO_STRU{
    int          nfieldId;
    wchar_t      *pName;
    wchar_t      *pMemName;
    unsigned int nValue;
    wchar_t      *pDesp;
    wchar_t      *pMemDesp;
    IE_PRESENCE enPre;
#define DE_FIELD_TYPE_IE                 1
#define DE_FIELD_TYPE_PARAM              2
    unsigned char         byFieldType;
	unsigned char		 byNasMsgLen;
	unsigned char         *pNasMsg;
} MSG_FIELD_INFO_STRU;

typedef struct _MSG_FIELD_NODE{
    MSG_FIELD_INFO_STRU FieldInfo;
    struct _MSG_FIELD_NODE *pFirsChild;
    struct _MSG_FIELD_NODE *pLastChild;
    struct _MSG_FIELD_NODE *pNext;
    struct _MSG_FIELD_NODE *pParent;
} MSG_FIELD_NODE ;
typedef MSG_FIELD_NODE MSG_CONTENT;

typedef struct _MSG_DECDOE_RESULT_STRU{
    unsigned char        byModule;
    unsigned char        byMsgType;
    unsigned short       byChannelType;
    int         enDir;
    wchar_t     *pMsgName;
    unsigned char *pData;
    int           nDataLen;
	
    MSG_CONTENT *pMsgContentH;
} MSG_DECDOE_RESULT_STRU;

// #ifdef __cplusplus
// extern "C"
// {
// #endif
// 	extern void *DecodeMsg( unsigned char *pData, int nLen, int nDirection, int nL2PLen, unsigned char byDeType );
// 	extern void  DecodeFreeMsg( void *pResult );
// 	extern void  InitDecodeMsg(void);
// 	extern void  ClearDecodeMsg(void);
// #ifdef __cplusplus
// } 
// #endif

typedef void *(*DecodeMsgF)(unsigned char *, int, int, int, unsigned char);
typedef void (*DecodeFreeMsgF)( void *);

#endif


