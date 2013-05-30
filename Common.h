#ifndef ___COMMON_H___
#define ___COMMON_H___


#include "CommonUtility/tstdlibs.h"
#include "CommonUtility/CommonFunctions.h"

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
#define  DIV_PLAYER 202

#define DIV_DEFAULT_CH16		DIV_CH16
#define DIV_DEFAULT_CH9			DIV_CH9
#define DIV_DEFAULT_CH8			DIV_CH8
#define DIV_DEFAULT_CH4			DIV_CH4
#define DIV_DEFAULT_CH1			DIV_CH1

#define INDEX_MAX				64000   

#define CAM_PER_DVR				16
#define DVR_MAX					1


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

//lht useful
#define CAM_MAX					16


#define _NEW_SWAP(a, b)			 { (a)^=(b)^=(a)^=(b); }
#define _SAFE_DELETE(p)			{ if(p != NULL)	delete p;  p = NULL; }
#define _SAFE_FCLOSE(p)			{ if(p != NULL)	fclose(p); p = NULL; }
#define _SAFE_CLOSEHANDLE(p)	{ if(p != NULL)	CloseHandle(p); p = NULL; }


#define FLOAT_CAM_FONT_COLOR     RGB(0, 0, 0)//clWhite
#define FLOAT_CAM_FRAME_COLOR    RGB(128, 128, 128)//clGray
#define TOGGLED_FrameColor RGB(200, 200, 200)//clSilver
#define clBlue	RGB(0, 0, 255)
#define clBlack	RGB(20,20, 20)

enum CamStatusReportType
{
	CamStatusReportType_SelectCam = 1,
	CamStatusReportType_StopVideo = 2,
};

typedef struct tagCHNODE
{
	int NodeType;							//0：工程；1:组；2：有线终端；3：通道；4：输入报警；5：解码器
	tstd::tstring NodeName;							//节点名称
	tstd::tstring HardVer;                     //终端节点类型
	tstd::tstring strMac;                             //设备标识码
	tstd::tstring pInstallPlace;
	int ParentNodeID;						//父结点ID
	int NodeID;								//节点ID
	int onLine;
}CHNODE;



struct CamStatusReport
{
	int reportType;//1-选择镜头；2-视频停止
	int cameraID;
	int errorCode;

};



enum KEERRORCODE
{
	KE_FAILED = 5,
	KE_SUCCESS = 13,
	KE_ERROR_PARAM,
	KE_SOCKET_NOTOPEN,
	KE_SOCKET_WRITEERROR,
	KE_CONNECT_SERVER_ERROR,
	KE_MSG_TIMEOUT,
	KE_DEVOFFLINE,
	KE_LOGIN_NAMEPWDERROR,
	KE_LOGIN_AlreadyLogin,
	KE_LOGIN_DBServerOff,
	KE_RTV_BOTHOFFLINE,
	KE_RTV_DVSOFFLINE ,
	KE_RTV_CHANNELDISABLE,
	KE_RTV_MAXTRANSNUM,
	KE_RTV_MAXVIEWNUM,
	KE_RTV_MAXTCPNUM,
	KE_RTV_OpenStreamFailed,
	KE_RTV_OpenSoundFailed,
	KE_RTV_NOPLAY,
	KE_XML_Receiving,
	KE_FUNCTION_NOTSUPPORT,
	KE_OTHERS
};

inline char * GetKEErrorDescriptA(int errorCode)
{
	switch(errorCode)
	{
	case KE_SUCCESS:											return "成功";
	case KE_FAILED:												return "失败";
	case KE_RTV_DVSOFFLINE:			return "视频服务器不在线";
	case KE_RTV_CHANNELDISABLE:					return "通道被禁用";
	case KE_SOCKET_NOTOPEN:							return "通讯未建立";
	case KE_CONNECT_SERVER_ERROR:			return "连接服务器失败";
	case KE_MSG_TIMEOUT:									return "消息超时";
	case KE_RTV_BOTHOFFLINE:							return "转发服务器和视频服务器都不在线!";
	case KE_RTV_MAXTRANSNUM:					return "超出最大转发数";
	case KE_RTV_MAXVIEWNUM:						return "每通道最多可发给8个客户端、一个录像和一个平台";
	case KE_RTV_MAXTCPNUM:							return "最多可建立600个TCP连接";
	case KE_RTV_OpenStreamFailed:					return "播放器打开失败";
	case KE_ERROR_PARAM:								return "参数错误";
	case KE_LOGIN_AlreadyLogin:						return "已经登陆";
	case KE_XML_Receiving:								return "正在接收树形结构";
	case KE_LOGIN_NAMEPWDERROR:				return "用户名密码错误";
	case KE_RTV_NOPLAY:									return "未开始视频";
	case KE_SOCKET_WRITEERROR:					return "发送消息错误";

	default:
		return "其他错误";
	}
}
inline CString GetKEErrorDescript(int errorCode)
{
	switch(errorCode)
	{
	case KE_SUCCESS: return _T("成功");
	case KE_SOCKET_NOTOPEN: return _T("通讯未建立");
	case KE_CONNECT_SERVER_ERROR: return _T("连接服务器失败");
	case KE_MSG_TIMEOUT: return _T("消息超时");
	case KE_RTV_BOTHOFFLINE: return _T("转发服务器和视频服务器都不在线！");
	default:
		return _T("其他错误");
	}
}

inline CString GetCamTimeFileName(int cameraID,CString extName,CString path)
{
	CString fullPath;
	CString fileName ;
	CString filePath;

	SYSTEMTIME sys;
	GetLocalTime( &sys );
	fileName.Format(_T("%d_%04d%02d%02d%02d%02d%02d%03d.%s"), cameraID,
		sys.wYear,sys.wMonth,sys.wDay,sys.wHour,sys.wMinute,sys.wSecond,sys.wMilliseconds,extName);
	
	if (path.GetAt(path.GetLength()-1)!=_T('\\'))
	{
		path.AppendChar(_T('\\'));
	}
	filePath.Format(_T("%s%d\\"),path,cameraID);

	if (!FolderExist(filePath))
	{
		CreateFolderEx(filePath);
	}
	fullPath = filePath + fileName;
	return fullPath;
}




#endif 