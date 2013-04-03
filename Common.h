#ifndef ___COMMON_H___
#define ___COMMON_H___

const int new_MappiingID[16] = { 0,  8,  1,  9,  2, 10,  3, 11,  4, 12,  5, 13,  6, 14,  7, 15 };
const int new_ChannelID[16]  = { 0,  2,  4,  6,  8, 10, 12, 14,  1,  3,  5,  7,  9, 11, 13, 15 };
const int new_CIF_ID_08[16]  = { 0,  8,  0,  8,  2, 10,  2, 10,  4, 12,  4, 12,  6, 14,  6, 14 };
const int new_CIF_ID_16[16]  = { 0,  0,  2,  2,  4,  4,  6,  6,  8,  8, 10, 10, 12, 12, 14, 14 };

#define MAX_SESSION_LIST		25

//Kind of Division
#define DIV_CH1					1
#define DIV_CH4					4
#define DIV_CH6					6
#define DIV_CH8					8
#define DIV_CH9					9
#define DIV_CH10				10
#define DIV_CH13				13
#define DIV_CH16				16

#define DIV_TOGGLED				201

#define DIV_DEFAULT_CH16		DIV_CH16
#define DIV_DEFAULT_CH9			DIV_CH9
#define DIV_DEFAULT_CH8			DIV_CH8
#define DIV_DEFAULT_CH4			DIV_CH4
#define DIV_DEFAULT_CH1			DIV_CH1

#define INDEX_MAX				64000   //Local Play 矫 阿 庆歹 File Pointer    : 傈眉弥措650M/弥家 捞固瘤 荤捞令10K

#define CAM_PER_DVR				16
#define DVR_MAX					1
#define CAM_MAX					16

#define	DF						40
#define HEADER_SIZE				16
#define PACKET_SIZE				512

#define BLOCK_SIZE				40//800

#define TCP_PREAMBLE	        0xff
#define TCP_PREAMBLE_COUNT      16
#define JPG_PREAMBLE	        0xaa
#define JPG_PREAMBLE_COUNT      2
#define AUDIO_PREAMBLE	        0x55
#define AUDIO_PREAMBLE_COUNT	2


//边距
#define BOTTOM_MARGIN			10
#define RIGHT_MARGIN			10
#define LEFT_MARGIN 10
#define TOP_MARGIN 10
#define  SPLIT_LINE 1

#define MAX_AVI_SIZE            600000000L
#define MAX_DVR_SIZE            600000000L
#define MAX_IMAGE_BUF			128000

#define NETWORK_VIDEO_FRAME		0x01
#define NETWORK_AUDIO_FRAME		0x02

#define VIDEO_PORT				5000
#define COMMAND_PORT			5001

#define	RECORD_PATH				"C:\\DVR\\"
#define DIVLOGO					"res\\DivLogo.bmp"

// Windows Version
enum {
	UNKNOWN_WIN_VERSION	= 0,
	WIN32S,
	WIN95,
	WIN95OSR2,
	WIN98,
	WIN98SE,
	WINNT,
	WINME,
	WIN2000,
	WINXP,
	WIN2003
};

struct TCamStatus
{
    BOOL	bMotion;
    BOOL	bAlarm;
    BOOL	bVideoLoss;
};


enum KEERRORCODE
{
	KE_SUCCESS = 0,
	KE_SOCKET_NOTOPEN,
	KE_SOCKET_WRITEERROR,
	KE_CONNECT_SERVER_ERROR,
	KE_MSG_TIMEOUT,
	KE_LOGIN_NAMEPWDERROR,
	KE_LOGIN_OTHERERROR,
};

inline CString GetKEErrorDescript(int errorCode)
{
	switch(errorCode)
	{
	//case KE_MSG_FAILED:return _T("服务器操作失败");
	case KE_SUCCESS: return _T("成功");
	case KE_SOCKET_NOTOPEN: return _T("通讯未建立");
	case KE_CONNECT_SERVER_ERROR: return _T("连接服务器失败");
	case KE_MSG_TIMEOUT: return _T("消息超时");
	//case KE_READ_DATAERROR: return _T("读取消息错误");
	//case KE_WRITE_ERROR: return _T("发送消息错误");
	//case KE_PASSWORD_ERROR:return _T("密码不匹配");
	default:
		return _T("其他错误");
	}
}

/*
#ifdef _DEBUG
    inline void ASSERT(bool ok) {
        if (!ok)
            ShowMessage("Error!");
    }
#else
    #define ASSERT
#endif
*/
#endif 