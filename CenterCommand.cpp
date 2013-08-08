#include "StdAfx.h"
#include "CenterCommand.h"
#include "KeWebCamOCX.h"

CenterCommand::CenterCommand(void)
{
}

CenterCommand::~CenterCommand(void)
{

}

std::string CenterCommand::GetErrorDesA( int errorCode )
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
	case KE_FUNCTION_NOTSUPPORT:				return "不支持此功能";
	case KE_SOCKET_ConnectionDropped:	return "socket连接丢失";
	case KE_DEVOFFLINE:	return "设备不在线";
	default:
		{
			if (this->errorCode == errorCode)
			{
				return this->errorDesA;
			}
			return "其他错误";
		}
		
	}
}

void CenterCommand::SetErrorDesA( std::string des,int errorCode )
{
	this->errorCode = errorCode;
	this->errorDesA = des;
}

int CenterCommand::LoginServer( CString userName,CString pwd )
{
	return KE_FUNCTION_NOTSUPPORT;
}

int CenterCommand::StartView( int cameraID )
{
return KE_FUNCTION_NOTSUPPORT;
}

int CenterCommand::PTZControl( int cameraID, BYTE ctrlType ,BYTE speed ,BYTE data )
{
return KE_FUNCTION_NOTSUPPORT;
}

int CenterCommand::RefreshCameraList()
{
return KE_FUNCTION_NOTSUPPORT;
}

int CenterCommand::SetClientID( int clientID )
{
return KE_FUNCTION_NOTSUPPORT;
}

int CenterCommand::LogoutServer()
{
	return KE_FUNCTION_NOTSUPPORT;
}

int CenterCommand::HeartBeat()
{
	return KE_FUNCTION_NOTSUPPORT;
}
//返回clientID，无返回0
int CenterCommand::GetClientID()
{
	return 0;
}

CMediaSocket * CenterCommand::GetMediaSocket( int cameraID )
{
	//COneCamera * tmpCamera = theApp.g_PlayWnd->GetOnePlayer(cameraID);
	//CMediaSocket * media = tmpCamera->m_MediaSocket;
	CMediaSocket * media = CMediaSocket::GetVideoSvrMedia(cameraID>>8);
// 	if (media == NULL)
// 	{
// 		media = new CMediaSocket;
// 		tmpCamera->m_MediaSocket = media;
// 	}
	return media;
}

int CenterCommand::ConnectServer( CString svrIp,int svrPort )
{
	return KE_FUNCTION_NOTSUPPORT;
}

int CenterCommand::GetRecordFileList( int cameraID,int startTime,int endTime,int fileType,std::vector<RecordFileInfo> & fileInfoList )
{
return KE_FUNCTION_NOTSUPPORT;
}

int CenterCommand::PlayRemoteRecord( int cameraID,int fileNo)
{
return KE_FUNCTION_NOTSUPPORT;
}

int CenterCommand::GetDevWifiAPList( int cameraID,std::vector<KEDevAPListItem> &apList )
{
	return KE_FUNCTION_NOTSUPPORT;
}

int CenterCommand::SetDevWifi( int cameraID,int listNo,KEDevWifiStartReq wifiStart )
{
return KE_FUNCTION_NOTSUPPORT;
}

int CenterCommand::ReportCamStatus( CamStatusReport report )
{
	CamStatusReport * pReport = new CamStatusReport;
	*pReport = report;
	theApp.g_pMainWnd->PostMessage(WM_CAMSTATUSREPORT,0,(LPARAM)pReport);
	return KE_SUCCESS;
}

CMyAVPlayer * CenterCommand::GetAVIPlayer( int cameraID )
{
	COnePlayer * pCamera = theApp.g_PlayWnd->GetOnePlayer(cameraID);
	if (pCamera == NULL)
	{
		pCamera =theApp.g_PlayWnd->ReuseActivePlayer(cameraID);
	}
	return pCamera->m_AVIPlayer;
}

int CenterCommand::StopView( int cameraID )
{
return KE_FUNCTION_NOTSUPPORT;
}

int CenterCommand::QueryVersion( CString & version ,CString & url )
{
	return KE_FUNCTION_NOTSUPPORT;
}

