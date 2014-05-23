#ifndef _MESSAGE_DEFINE_H_
#define _MESSAGE_DEFINE_H_
#include "WMMsgDecode.h"

#define MSG_DIR_MAX_CNT       2
#define MSG_TAG_LIST_MAX_CNT  304
#define MSG_LAYER_MAX_CNT     6
#define MSG_CHA_MAX_CNT       15

#define MSG_MODULE_MAX_CNT 9
#define PARAM_NAME_MAXLEN  150

typedef enum E_MESSAGE_TYPE_ID
{
	E_MESSAGE_TYPE_ALL = 0,
	E_MESSAGE_TYPE_L2,
	E_MESSAGE_TYPE_L3,
	E_MESSAGE_TYPE_RRC,
	E_MESSAGE_TYPE_MAC,
	E_MESSAGE_TYPE_MAX
}_E_MESSAGE_TYPE_ID;

typedef struct _MSG_TAG_HEADER_{
	char pMsgName[PARAM_NAME_MAXLEN];
	unsigned char nDir;
}MSG_TAG_HEADER;

typedef struct _MSG_CHA_TAG_{
	char pName[PARAM_NAME_MAXLEN];
	unsigned char nCHA_ID;
}MSG_CHA_TAG;

typedef struct _MSG_LAYER_TAG_{
	char pName[PARAM_NAME_MAXLEN];
	unsigned char nLayerID;
}MSG_LAYER_TAG;

typedef struct _MSG_TAG_STR_{
	char pName[PARAM_NAME_MAXLEN];
	int ndir;
	int nMsgType;
	int nIsDisable;
}MSG_TAG_STR;

typedef struct _MSG_DIR_TAG_STR_{
	char pMsgName[PARAM_NAME_MAXLEN];
	int ndir;
}MSG_DIR_TAG_STR;

typedef struct _MSG_MODULE_TAG_STR_{
	char pName[PARAM_NAME_MAXLEN];
	int nModType;
	int nIndexStart;
	int nIndexEnd;
	int nIsDisable;
}MSG_MODULE_TAG_STR;

MSG_DIR_TAG_STR    *get_MsgDirList();
MSG_MODULE_TAG_STR *get_MsgModuleList();
MSG_TAG_STR        *get_MsgList();
MSG_LAYER_TAG      *get_MsgLayerList();
MSG_CHA_TAG        *get_MsgChannelList();

#endif
