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
	case KE_SUCCESS:											return "�ɹ�";
	case KE_FAILED:												return "ʧ��";
	case KE_RTV_VIDEOSERVEROFFLINE:			return "��Ƶ������������";
	case KE_RTV_CHANNELDISABLE:					return "ͨ��������";
	case KE_SOCKET_NOTOPEN:							return "ͨѶδ����";
	case KE_CONNECT_SERVER_ERROR:			return "���ӷ�����ʧ��";
	case KE_MSG_TIMEOUT:									return "��Ϣ��ʱ";
	case KE_RTV_BOTHOFFLINE:							return "ת������������Ƶ��������������!";
	case KE_RTV_MAXTRANSNUM:					return "�������ת����";
	case KE_RTV_MAXVIEWNUM:						return "ÿͨ�����ɷ���8���ͻ��ˡ�һ��¼���һ��ƽ̨";
	case KE_RTV_MAXTCPNUM:							return "���ɽ���600��TCP����";
	case KE_RTV_OpenStreamFailed:					return "��������ʧ��";
	case KE_ERROR_PARAM:								return "��������";
	case KE_LOGIN_AlreadyLogin:						return "�Ѿ���½";
	case KE_XML_Receiving:								return "���ڽ������νṹ";
	case KE_LOGIN_NAMEPWDERROR:				return "�û����������";
	case KE_RTV_NOPLAY:									return "δ��ʼ��Ƶ";
	case KE_SOCKET_WRITEERROR:					return "������Ϣ����";
	case KE_FUNCTION_NOTSUPPORT:				return "��֧�ִ˹���";
	case KE_DEVOFFLINE:	return "�豸������";
	default:
		{
			if (this->errorCode == errorCode)
			{
				return this->errorDesA;
			}
			return "��������";
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

int CenterCommand::GetClientID()
{
	return KE_FUNCTION_NOTSUPPORT;
}

CMediaSocket * CenterCommand::GetMediaSocket( int cameraID )
{
	COneCamera * tmpCamera = theApp.g_PlayWnd->GetOnePlayer(cameraID);
	CMediaSocket * media = tmpCamera->m_MediaSocket;//CMediaSocket::GetMediaSocket(videoID,channelNo,true);
	if (media == NULL)
	{
		media = new CMediaSocket;
		tmpCamera->m_MediaSocket = media;
	}
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