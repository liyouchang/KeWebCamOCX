#include "StdAfx.h"
#include "P2PCmdSocket.h"
#include "../CommonUtility/Md5A.h"
#include "../Markup.h"
#include "../Common.h"
//#include "../CommonUtility/netadapter.h"


//CMd5A Md5;

struct AddDeviceTransData
{
	int devSvrID;
	BYTE resp;
};


CP2PCmdSocket::CP2PCmdSocket(void)
{
	m_serverPort = "22616";
	m_clientID = 0;
	m_nSockType = SOCK_UDP;
	m_heartCount = 0;
	m_ResendThread = NULL;
	Init();
}

CP2PCmdSocket::~CP2PCmdSocket(void)
{
	if (m_ResendThread != NULL)
	{
		delete m_ResendThread;
	}
}


bool CP2PCmdSocket::ConnectToServer( CString severAddr )
{
	//TRACE("Connect to server, serverAddr="<<severAddr.GetString()<<";serverPort="<<serverPort.GetString());
	if ( !m_SocketClient.StartClient(NULL, severAddr, m_serverPort, AF_INET,SOCK_DGRAM) )
	{
		int errorCode = WSAGetLastError();
		//CString errorMsg;
		//errorMsg.Format("Failed to start client connection %d." , errorCode);
		//AfxMessageBox(errorMsg);
		return false;
	}
	if (m_ResendThread == NULL)
	{
		m_ResendThread = new CResendThread(this);
		m_ResendThread->Start();
	}

	this->m_serverIP  = severAddr;
	return true;
}

bool CP2PCmdSocket::ConnectToServer( CString severAddr,int svrPort )
{
	//TRACE("Connect to server, serverAddr="<<severAddr.GetString()<<";serverPort="<<serverPort.GetString());
	CString strPort;
	strPort.Format(_T("%d"),svrPort);
	if ( !m_SocketClient.StartClient(NULL, severAddr, strPort, AF_INET,SOCK_DGRAM) )
	{
		int errorCode = WSAGetLastError();
		//CString errorMsg;
		//errorMsg.Format("Failed to start client connection %d." , errorCode);
		//AfxMessageBox(errorMsg);
		return false;
	}
	if (m_ResendThread == NULL)
	{
		m_ResendThread = new CResendThread(this);
		m_ResendThread->Start();
	}

	this->m_serverIP  = severAddr;
	this->m_serverPort = strPort;
	return true;
}


bool CP2PCmdSocket::Init()
{
	m_heartCount = 0;
	const int nRecvBufSize = 0x2000;
	return __super::Init(nRecvBufSize);
}

void CP2PCmdSocket::HandleMessage( const BYTE* msgData )
{
	PKEMsgHead pHead = (PKEMsgHead)msgData;

	switch(pHead->msgType)
	{
	case KEMSG_TYPE_USERREGISTER:
		RecvUserRegister(msgData);
		break;
	case KEMSG_TYPE_USERLOGIN:
		LoginMsgResp(msgData);
		break;
	case KE_P2PMSG_TYPE_HEARTBEAT:
		RecvHeartbeat(msgData);
		break;
	case KEMSG_TYPE_DEVLIST:
		RecvDevList(msgData);
		break;
	case KEMSG_TYPE_SHAREDEVLIST:
		RecvShareDevList(msgData);
		break;
	case KEMSG_TYPE_DELETEDEV:
		RecvDeleteDev(msgData);
		break;
	case KEMSG_TYPE_DEVSTATUS:
		RecvDevStatus(msgData);
		break;
	case KEMSG_TYPE_ALARM:
		RecvAlarm(msgData);
		break;
	case KEMSG_TYPE_ADDDEVICE:
		RecvAddCamResp(msgData);
		break;
	case KEMSG_TYPE_SHARECAM:
		RecvShareDevResp(msgData);
		break;
	case KEMSG_TYPE_DEVNETINFO:
		RecvDevNetInfoResp(msgData);
		break;
	case KEMSG_TYPE_CHGDEVNAME:
		RecvChangeDevName(msgData);
		break;
	case KEMSG_TYPE_CHECKSHAREDUSER:
		RecvChkSharedUserResp(msgData);
		break;
	case KEMSG_TYPE_DEVNOTSHARED:
		RecvDevNotSharedNotice(msgData);
		break;
	case KEMSG_TYPE_CANCELSHAREDDEV:
		RecvCancelSharedDevResp(msgData);
		break;
	case KEMSG_TYPE_CHGUSERPWD:
		RecvChangeUserPwdResp(msgData);
		break;
	case KEMSG_TYPE_DEVREBOOT:
		RecvDevRebootResp(msgData);
		break;
	case KEMSG_TYPE_CHECKVERSION:
		RecvCheckVersionResp(msgData);
		break;
	case  KEMSG_TYPE_FORGETPWD:
		RecvForgetPWD(msgData);
		break;
	case KEMSG_TYPE_GetTransIP:
		RecvTransIp(msgData);
		break;
	default:
		TRACE("Receive unkown message\n");
	}
}

void CP2PCmdSocket::AskKeyMsgResp( const BYTE* msgData )
{
	// 	 LOG_DEBUG("Receive ask key response");
	// 	PMsgSecretKeyResp pMsg = (PMsgSecretKeyResp)msgData;
	// 	BYTE *secretKey= new BYTE[SECRETKEY_LEN];	//�ӷ�������ȡ����Կ
	// 	memcpy(secretKey,pMsg->keyt,SECRETKEY_LEN);
	// 	tmpData[KEMSG_EVENT_ASKKEY] = secretKey;
	// 	keEvent[KEMSG_EVENT_ASKKEY].PulseEvent();
}

void CP2PCmdSocket::LoginMsgResp( const BYTE* msgData )
{
	PKEUserLoginResp pMsg = (PKEUserLoginResp)msgData;
	m_clientID = pMsg->head.clientID;
	char * loginRet = new char;
	m_ResendThread->RemoveResendMsg(KEMSG_TYPE_USERLOGIN);
	this->respData[KEMSG_EVENT_LOGIN] = pMsg->respData;
	keEvent[KEMSG_EVENT_LOGIN].SetEvent();

}

int CP2PCmdSocket::SendLoginMsg( const char * userName,const char *encryptData )
{
// 	if (!m_SocketClient.IsOpen())
// 	{
// 		return KE_SOCKET_NOTOPEN;
// 	}

// 	CNetworkAdapter *adapters = NULL;
// 	CString addrMac;
// 	ULONG	ulNeeded	= 0;
// 	DWORD dwErr = EnumNetworkAdapters( adapters, 0, &ulNeeded );
// 	int nCount	= ulNeeded / sizeof( CNetworkAdapter );
// 	if (nCount > 0)
// 	{
// 		adapters = new CNetworkAdapter[ ulNeeded / sizeof( CNetworkAdapter ) ];		
// 		dwErr		= EnumNetworkAdapters( adapters, ulNeeded, &ulNeeded );		
// 		addrMac = adapters[0].GetAdapterAddress().c_str();
// 	}
// 	
// 	
// 	CMarkup xml;
// 	xml.SetDoc("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\r\n");
// 	xml.AddElem("Login");
// 	xml.IntoElem();
// 	xml.AddElem("Info");
// 	xml.AddAttrib("U",userName);
// 	xml.AddAttrib("P",encryptData);
// 	xml.AddAttrib("M",addrMac);
// 	xml.OutOfElem();
// 	CString xmlStr= xml.GetDoc();
// 
// 	int msgLen = sizeof(KEUserLoginReq)+xmlStr.GetLength();
// 	msgSend.resize(msgLen+1);
// 
// 	KEUserLoginReq reqMsg;
// 
// 	reqMsg.head.protocal = PROTOCOL_HEAD;
// 	reqMsg.head.msgType = KEMSG_TYPE_USERLOGIN;
// 	reqMsg.head.length = msgLen;
// 	reqMsg.head.clientID = 0;
// 	memcpy(&msgSend[0],&reqMsg,sizeof(KEUserLoginReq));
// 	strcpy((char *)&msgSend[sizeof(KEUserLoginReq)],xmlStr);
// 	
// 	int ret =	this->Write(&msgSend[0],msgLen);
// 	if (ret != msgLen)
// 	{
// 		return KE_WRITE_ERROR;
// 	}
// 
// 	m_ResendThread->AddResendMsg(KEMSG_TYPE_USERLOGIN,msgSend);
// 
// 	keEvent[KEMSG_EVENT_LOGIN].ResetEvent();
// 	DWORD dw = WaitForSingleObject(keEvent[KEMSG_EVENT_LOGIN].m_hObject,LOGIN_WAIT_TIMEOUT);
// 	if (dw == WAIT_TIMEOUT)
// 	{
// 		TRACE("Wait login response time out\r\n");
// 		return KE_MSG_TIMEOUT;
// 	}
// 
// 	char loginResult = this->respData[KEMSG_EVENT_LOGIN];
// 	

//	return loginResult;
	return 0;
}

int CP2PCmdSocket::StartView( int cameraID)
{
	int devSvrID = cameraID/256;
	int ret = SendDevNetInfoReq(devSvrID);
	if (ret != KE_SUCCESS)
	{
		return ret;
	}
	CMediaSocket *media = GetMediaSocket(cameraID);
	
	ret = media->ReqestMediaData(cameraID,Media_Vedio);
	return ret;
}



void CP2PCmdSocket::RecvRealTimeMedia( const BYTE* msgData )
{

}

//��ȡ�豸�б�
void CP2PCmdSocket::CheckAllDevListAsyn()
{
	KEMsgHead msg;
	msg.clientID = m_clientID;
	msg.msgLength = sizeof(KEMsgHead);
	msg.msgType = KEMSG_TYPE_DEVLIST;
	msg.protocal = PROTOCOL_HEAD;
	vector<BYTE> msgSend;
	msgSend.resize(msg.msgLength);
	memcpy(&msgSend[0],&msg,msg.msgLength);
	this->Write((BYTE *)&msg,msg.msgLength);

	m_ResendThread->AddResendMsg(KEMSG_TYPE_DEVLIST,msgSend);

	msg.msgType = KEMSG_TYPE_SHAREDEVLIST;
	this->Write((BYTE *)&msg,msg.msgLength);
	m_ResendThread->AddResendMsg(KEMSG_TYPE_SHAREDEVLIST,msgSend);

}

void CP2PCmdSocket::RecvDevList( const BYTE* msgData )
{
	PKEMsgHead pMsg = (PKEMsgHead)msgData;
	char * data = (char *)(msgData+sizeof(KEMsgHead));
	TRACE1("RecvDevList--Data: %s\n",data);
	m_ResendThread->RemoveResendMsg(KEMSG_TYPE_DEVLIST);
	//GetViewPointer()->SetMyCamList(data);
}

void CP2PCmdSocket::RecvShareDevList( const BYTE* msgData )
{
	PKEMsgHead pMsg = (PKEMsgHead)msgData;
	char * data = (char *)(msgData+sizeof(KEMsgHead));
	TRACE1("RecvShareDevList--Data: %s\n",data);
	m_ResendThread->RemoveResendMsg(KEMSG_TYPE_SHAREDEVLIST);
	//GetViewPointer()->SetSharedCamList(data);
}

int CP2PCmdSocket::DeleteCamera( char type,int devID )
{
	if (!m_SocketClient.IsOpen())
	{
		return KE_SOCKET_NOTOPEN;
	}
	KEDeleteCameraReq msg;
	msg.head.protocal = PROTOCOL_HEAD;
	msg.head.msgType = KEMSG_TYPE_DELETEDEV;
	msg.head.msgLength = sizeof(KEDeleteCameraReq);
	msg.head.clientID = m_clientID;
	msg.devID = devID;
	msg.devType = type;
	int ret = this->Write((BYTE *)&msg,msg.head.msgLength);
	if (ret != msg.head.msgLength)
	{
		return KE_SOCKET_WRITEERROR;
	}
	DWORD dw = WaitForSingleObject(keEvent[KEMSG_EVENT_DELETEDEV].m_hObject,MSG_WAIT_TIMEOUT);
	if (dw == WAIT_TIMEOUT)
	{
		return KE_MSG_TIMEOUT;
	}
	int resp = respData[KEMSG_EVENT_DELETEDEV];

	return resp;

}

void CP2PCmdSocket::RecvDeleteDev( const BYTE * msgData )
{
	PKEDeleteCameraResp  pMsg = (PKEDeleteCameraResp)&msgData[0];
	respData[KEMSG_EVENT_DELETEDEV] = pMsg->respData;
	Sleep(0);
	keEvent[KEMSG_EVENT_DELETEDEV].PulseEvent();
}

void CP2PCmdSocket::RecvHeartbeat( const BYTE *msgData )
{
	PKEMsgHead pMsg = (PKEMsgHead)msgData;
	if (pMsg->clientID == m_clientID)
	{
		if (m_heartCount>0)
		{
			//TRACE1("Receive HeartBeat %d!\n",m_heartCount);
			m_heartCount = 0;
		}
	}
}

void CP2PCmdSocket::RecvDevStatus( const BYTE *msgData )
{
	PKEDeviceStatus pMsg = (PKEDeviceStatus)msgData;
	//GetViewPointer()->DevStatusChange(pMsg->devID,pMsg->status);
}

void CP2PCmdSocket::RecvAlarm( const BYTE *msgData )
{	
	PKEAlarm pMsg = (PKEAlarm)msgData;

}

void CP2PCmdSocket::RecvAddCamResp( const BYTE *msgData )
{
	PKEAddDeviceResp pMsg = (PKEAddDeviceResp)msgData;
	int respType;
	if (pMsg->type == 6)
	{
		AddDeviceTransData *transData = new AddDeviceTransData;
		transData->devSvrID = pMsg->devID;
		transData->resp = pMsg->respData;

		respType = KEMSG_EVENT_ADDCAM;
		respData[respType] =(int)transData;
		keEvent[respType].PulseEvent();
	} 
	else
	{
		TRACE("���Ӳ������ʹ���");
		return;
	}
}

int CP2PCmdSocket::ShareCamera( CString userName,int devID )
{

	if (!m_SocketClient.IsOpen())
	{
		return KE_SOCKET_NOTOPEN;
	}
	KEShareDeviceReq req;
	std::vector<BYTE> msgSend;

	CMarkup xml;
	xml.AddElem(_T("AddDeviceShare"));
	xml.IntoElem();
	xml.AddElem(_T("Info"));
	xml.AddAttrib(_T("D"),devID);
	xml.AddAttrib(_T("U"),userName);
	xml.OutOfElem();
	CString xmlStr= xml.GetDoc();

	req.head.protocal = PROTOCOL_HEAD;
	req.head.msgType = KEMSG_TYPE_SHARECAM;
	req.head.clientID = m_clientID;
	req.type = 8;
	int msgLen = sizeof(KEShareDeviceReq) + xmlStr.GetLength();
	req.head.msgLength = msgLen;
	msgSend.resize(msgLen+1);
	memcpy(&msgSend[0],&req,sizeof(KEShareDeviceReq));
//	_tcscpy((char *)&msgSend[sizeof(KEShareDeviceReq)],xmlStr.GetString());
	int ret = this->Write(&msgSend[0],msgLen);
	if (ret != msgLen)
	{
		return KE_SOCKET_WRITEERROR;
	}
	DWORD dw = WaitForSingleObject(keEvent[KEMSG_EVENT_SHARECAM].m_hObject,MSG_WAIT_TIMEOUT);
	if (dw == WAIT_TIMEOUT)
	{
		return KE_MSG_TIMEOUT;
	}
	int resp= respData[KEMSG_EVENT_SHARECAM];
	return resp;
}

int CP2PCmdSocket::AddDevice( CString mac,CString devName )
{
	if (!m_SocketClient.IsOpen())
	{
		return KE_SOCKET_NOTOPEN;
	}
	KEAddDeviceReq req;
	std::vector<BYTE> msgSend;

	CMarkup xml;
	xml.AddElem(_T("AddDevice"));
	xml.IntoElem();
	xml.AddElem(_T("Info"));
	xml.AddAttrib(_T("D"),mac);
	xml.AddAttrib(_T("N"),devName);
	xml.OutOfElem();
	CString xmlStr= xml.GetDoc();

	req.head.protocal = PROTOCOL_HEAD;
	req.head.msgType = KEMSG_TYPE_ADDDEVICE;
	req.head.clientID = m_clientID;
	req.type = 6;
	int msgLen = sizeof(KEAddDeviceReq) + xmlStr.GetLength();
	req.head.msgLength = msgLen;
	msgSend.resize(msgLen+1);

	memcpy(&msgSend[0],&req,sizeof(KEAddDeviceReq));
	//_tcscpy((char *)&msgSend[sizeof(KEAddDeviceReq)],xmlStr.GetString());

	int ret = this->Write(&msgSend[0],msgLen);
	if (ret != msgLen)
	{
		return KE_SOCKET_WRITEERROR;
	}

	DWORD dw = WaitForSingleObject(keEvent[KEMSG_EVENT_ADDCAM].m_hObject,MSG_WAIT_TIMEOUT);
	if (dw == WAIT_TIMEOUT)
	{
		return KE_MSG_TIMEOUT;
	}
	AddDeviceTransData *transData =( AddDeviceTransData *) respData[KEMSG_EVENT_ADDCAM];

	int resp = transData->resp;
	if (resp == KE_SUCCESS)
	{
		int devID = transData->devSvrID;
		TRACE3("AddDevice--Success,devID=%d,name=%s,mac=%s\n",devID,devName,mac);
		//GetViewPointer()->AddDevice(devID,devName,mac);
	}
	delete transData;
	return resp;
}

void CP2PCmdSocket::RecvUserRegister( const BYTE *msgData )
{
	PKEUserRegisterResp pMsg = (PKEUserRegisterResp)msgData;
	

	//int resendTime = m_ResendThread->GetResendTime(KEMSG_TYPE_USERREGISTER);
	//m_ResendThread->RemoveResendMsg(KEMSG_TYPE_USERREGISTER);

	
	if (pMsg->respData == 8)
	{
		m_clientID = pMsg->head.clientID;
		TRACE1("RecvUserRegister--clientID=%d\n",m_clientID);
	}
	respData[KEMSG_EVENT_USERREGISTER] = pMsg->respData;
// 	if (pMsg->respData == 5 && resendTime > 0)
// 	{
// 		respData[KEMSG_EVENT_USERREGISTER] = KE_SUCCESS;
// 	}
	keEvent[KEMSG_EVENT_USERREGISTER].SetEvent();
	

}

//֪ͨ���ķ�����ע�����û�
int CP2PCmdSocket::LogoutServer()
{
	KEMsgHead msg;
	msg.clientID = this->m_clientID;
	msg.msgLength = sizeof(KEMsgHead);
	msg.msgType = KEMSG_TYPE_USERLOGOUT;
	msg.protocal = PROTOCOL_HEAD;
	int ret = this->Write((BYTE *)&msg,msg.msgLength);

	//�ر�����
	m_recvBuf.CancelWaitNewData();
	this->CloseConnect();
	if (ret != msg.msgLength)
	{
		return KE_SOCKET_WRITEERROR;
	}
	return KE_SUCCESS;
}

int CP2PCmdSocket::SendDevNetInfoReq( int devID )
{
	KEDevNetInfoReq msg;
	msg.head.protocal = PROTOCOL_HEAD;
	msg.head.msgType = KEMSG_TYPE_DEVNETINFO;
	msg.head.msgLength = sizeof(KEDevNetInfoReq);
	msg.head.clientID = this->m_clientID;
	msg.devID = devID;
	std::vector<BYTE> msgSend;
	msgSend.resize(msg.head.msgLength+1);
	memcpy(&msgSend[0],&msg,msg.head.msgLength);
	int ret = this->Write((BYTE *)&msg,msg.head.msgLength);
	if (ret != msg.head.msgLength)
	{
		return KE_SOCKET_WRITEERROR;
	}
	KEDevNetInfoResp * respMsg ;
	ret  = this->WaitRecvMsg(KEMSG_TYPE_DEVNETINFO,MSG_WAIT_TIMEOUT,(void **)&respMsg);
	if (ret != KE_SUCCESS)
	{
		return ret;
	}
	
	ret = ConnectToMedia(respMsg);
	//m_ResendThread->AddResendMsg(KEMSG_TYPE_DEVNETINFO,msgSend);

	return ret;
}	

void CP2PCmdSocket::RecvDevNetInfoResp( const BYTE *msgData )
{
	PKEDevNetInfoResp pMsg = (PKEDevNetInfoResp)msgData;
	
	m_ResendThread->RemoveResendMsg(KEMSG_TYPE_DEVNETINFO);
	TRACE1("RecvDevNetInfoResp--onLine=%d\n" , pMsg->respData);
	if (pMsg->respData == 1)//�豸����
	{	
		KEDevNetInfoResp * respMsg = new KEDevNetInfoResp;
		*respMsg = *pMsg;
		this->SetRecvMsg(KEMSG_TYPE_DEVNETINFO , KE_SUCCESS , respMsg);
		return;
	} 
	if (pMsg->respData == 0)// �豸������
	{
		devSvrMap[pMsg->devID].connectSuccess = false;
		TRACE("RecvDevNetInfoResp--�豸������\n");
		this->SetRecvMsg(KEMSG_TYPE_DEVNETINFO,KE_DEVOFFLINE);
		return;
	} 
	TRACE("RecvDevNetInfoResp--�����Ӧ����Ϣ!\n");
}

int CP2PCmdSocket::ChangeDevName( int devID,CString devNewName )
{
	if (!m_SocketClient.IsOpen())
	{
		return KE_SOCKET_NOTOPEN;
	}
	KEChangeDeviceNameReq req;
	std::vector<BYTE> msgSend;
	CMarkup xml;
	xml.AddElem(_T("ModifyDevice"));
	xml.IntoElem();
	xml.AddElem(_T("Info"));
	xml.AddAttrib(_T("D"),devID);
	xml.AddAttrib(_T("U"),devNewName);
	xml.OutOfElem();
	CString xmlStr= xml.GetDoc();

	req.head.protocal = PROTOCOL_HEAD;
	req.head.msgType = KEMSG_TYPE_CHGDEVNAME;
	req.head.clientID = m_clientID;
	req.type = 15;
	int msgLen = sizeof(KEChangeDeviceNameReq) + xmlStr.GetLength();
	req.head.msgLength = msgLen;
	msgSend.resize(msgLen+1);

	memcpy(&msgSend[0],&req,sizeof(KEChangeDeviceNameReq));
//	_tcscpy((char *)&msgSend[sizeof(KEChangeDeviceNameReq)],xmlStr.GetString());

	int ret = this->Write(&msgSend[0],msgLen);
	if (ret != msgLen)
	{
		return KE_SOCKET_WRITEERROR;
	}

	DWORD dw = WaitForSingleObject(keEvent[KEMSG_EVENT_CHGDEVNAME].m_hObject,MSG_WAIT_TIMEOUT);
	if (dw == WAIT_TIMEOUT)
	{
		return KE_MSG_TIMEOUT;
	}
	int resp = respData[KEMSG_EVENT_CHGDEVNAME];
	return resp;
}

void CP2PCmdSocket::RecvChangeDevName( const BYTE *msgData )
{
	PKEChangeDeviceNameResp pMsg = (PKEChangeDeviceNameResp)msgData;
	respData[KEMSG_EVENT_CHGDEVNAME] = pMsg->respData;
	Sleep(0);
	keEvent[KEMSG_EVENT_CHGDEVNAME].PulseEvent();	
}

void CP2PCmdSocket::RecvShareDevResp( const BYTE * msgData )
{
	PKEShareDeviceResp pMsg = (PKEShareDeviceResp)msgData;
	int respType;
	if (pMsg->type == 8)
	{
		respType = KEMSG_EVENT_SHARECAM;
		respData[respType] = pMsg->respData;
		Sleep(0);
		keEvent[respType].PulseEvent();
	} 
	else{
		TRACE("RecvShareDevResp--���Ӳ������ʹ���");	
	}
}

int CP2PCmdSocket::CheckDevShareUser( int devSvrID )
{
	KECheckDevShareUserReq msg;
	int msgLen = sizeof(KECheckDevShareUserReq);
	msg.head.clientID = m_clientID;
	msg.head.msgLength = msgLen;
	msg.head.msgType = KEMSG_TYPE_CHECKSHAREDUSER;
	msg.head.protocal = PROTOCOL_HEAD;
	msg.devSvrID = devSvrID;
	this->Write((BYTE *)&msg,msgLen);
	DWORD dw = WaitForSingleObject(keEvent[KEMSG_EVENT_CHKSHAREDUSER].m_hObject,MSG_WAIT_TIMEOUT);
	if (dw == WAIT_TIMEOUT)
	{
		TRACE("CheckDevShareUser--Timeout\n");
		return KE_MSG_TIMEOUT;
	}
	int resp = respData[KEMSG_EVENT_CHKSHAREDUSER];
	return resp;
}

void CP2PCmdSocket::RecvChkSharedUserResp( const BYTE *msgData )
{
	PKEMsgHead pMsg = (PKEMsgHead)msgData;
	char * data = (char *)(msgData+sizeof(KEMsgHead));
	TRACE1("RecvChkSharedUserResp--%s\n ",data);
	//GetViewPointer()->SetShareUser(data);
	respData[KEMSG_EVENT_CHKSHAREDUSER] = KE_SUCCESS;
	Sleep(0);
	keEvent[KEMSG_EVENT_CHKSHAREDUSER].PulseEvent();	
	TRACE("RecvChkSharedUserResp--end\n");
}
void CP2PCmdSocket::RecvDevNotSharedNotice( const BYTE *msgData )
{
	PKEDevNotSharedNotice pMsg = (PKEDevNotSharedNotice)msgData;
	//GetViewPointer()->DeleteTreeNode(pMsg->devSvrID);
}

int CP2PCmdSocket::CancelDevShare( int devSvrID,int sharedUserID )
{
	KECancelSharedDevReq msg;
	int msgLen = sizeof(KECancelSharedDevReq);
	msg.head.clientID = m_clientID;
	msg.head.msgLength = msgLen;
	msg.head.msgType = KEMSG_TYPE_CANCELSHAREDDEV;
	msg.head.protocal = PROTOCOL_HEAD;
	msg.devSvrID = devSvrID;
	msg.type = 16;
	msg.sharedUserID = sharedUserID;
	this->Write((BYTE *)&msg,msgLen);
	keEvent[KEMSG_EVENT_CANCELSHAREDDEV].ResetEvent();
	DWORD dw = WaitForSingleObject(keEvent[KEMSG_EVENT_CANCELSHAREDDEV].m_hObject,MSG_WAIT_TIMEOUT);
	if (dw == WAIT_TIMEOUT)
	{
		TRACE("CancelDevShare--Timeout\n");
		return KE_MSG_TIMEOUT;
	}
	int resp = respData[KEMSG_EVENT_CANCELSHAREDDEV];
	return resp;
}

void CP2PCmdSocket::RecvCancelSharedDevResp( const BYTE * msgData )
{
	PKECancelSharedDevResp pMsg = (PKECancelSharedDevResp)msgData;
	respData[KEMSG_EVENT_CANCELSHAREDDEV] = pMsg->respData;
	Sleep(0);
	keEvent[KEMSG_EVENT_CANCELSHAREDDEV].SetEvent();	
	TRACE("RecvCancelSharedDevResp--end\n");
}


int CP2PCmdSocket::ChangeUserPwd( CString oldPwd,CString newPwd )
{

	//��֤������
// 	char szOldPwdMd5[ENCRYPTED_DATA_LEN];
// 	char szNewPwdMd5[ENCRYPTED_DATA_LEN];
// 	Md5.MDString(oldPwd, szOldPwdMd5);
// 	if (strcmp(szMD5PWD,szOldPwdMd5) != 0)
// 	{
// 		return KE_PASSWORD_ERROR;
// 	}
// 	//����������
// 	Md5.MDString(newPwd, szNewPwdMd5);
// 
// 	KEChangeUserPwdReq msg;
// 	int msgLen = sizeof(KEChangeUserPwdReq);
// 	msg.head.clientID = m_clientID;
// 	msg.head.length = msgLen;
// 	msg.head.msgType = KEMSG_TYPE_CHGUSERPWD;
// 	msg.head.protocal = PROTOCOL_HEAD;
// 	msg.type = 14;
// 	strcpy(msg.pwdData,szNewPwdMd5);
// 
// 	int ret = this->Write((BYTE *)&msg,msgLen);
// 	if (ret != msgLen)
// 	{
// 		return KE_WRITE_ERROR;
// 	}
// 	keEvent[KEMSG_EVENT_CHGUSERPWD].ResetEvent();
// 	DWORD dw = WaitForSingleObject(keEvent[KEMSG_EVENT_CHGUSERPWD].m_hObject,MSG_WAIT_TIMEOUT);
// 	if (dw == WAIT_TIMEOUT)
// 	{
// 		TRACE("ChangeUserPwd--Timeout\n");
// 		return KE_MSG_TIMEOUT;
// 	}
// 	int resp = respData[KEMSG_EVENT_CHGUSERPWD];
// 	if (resp == KE_SUCCESS)
// 	{//��ס������
// 		strcpy(szMD5PWD,szNewPwdMd5);
// 	}
// 	return resp;
	return 0;
}

void CP2PCmdSocket::RecvChangeUserPwdResp( const BYTE * msgData )
{
	PKEChangeUserPwdResp pMsg = (PKEChangeUserPwdResp)msgData;
	respData[KEMSG_EVENT_CHGUSERPWD] = pMsg->respData;
	Sleep(0);
	keEvent[KEMSG_EVENT_CHGUSERPWD].SetEvent();	
	TRACE("RecvCancelSharedDevResp--end\n");
}

void CP2PCmdSocket::CheckShareDevListAsyn()
{
	KEMsgHead msg;
	msg.clientID = m_clientID;
	msg.msgLength = sizeof(KEMsgHead);
	msg.protocal = PROTOCOL_HEAD;
	msg.msgType = KEMSG_TYPE_SHAREDEVLIST;
	this->Write((BYTE *)&msg,msg.msgLength);
}

int CP2PCmdSocket::DeviceReboot( int devID )
{
	KEDevRebootReq msg;
	int msgLen = sizeof(KEDevRebootReq);
	msg.head.clientID = m_clientID;
	msg.head.msgLength = msgLen;
	msg.head.msgType = KEMSG_TYPE_DEVREBOOT;
	msg.head.protocal = PROTOCOL_HEAD;
	msg.devID = devID;
	int ret = this->Write((BYTE *)&msg,msgLen);
	if (ret != msgLen)
	{
		return KE_SOCKET_WRITEERROR;
	}
	//return KE_SUCCESS;
	keEvent[KEMSG_EVENT_DEVREBOOT].ResetEvent();
	DWORD dw = WaitForSingleObject(keEvent[KEMSG_EVENT_DEVREBOOT].m_hObject,MSG_WAIT_TIMEOUT);
	if (dw == WAIT_TIMEOUT)
	{
		TRACE("DevReboot--Timeout\n");
		return KE_MSG_TIMEOUT;
	}
	int resp = respData[KEMSG_EVENT_DEVREBOOT];
	return resp;
}

void CP2PCmdSocket::RecvDevRebootResp( const BYTE * msgData )
{
	PKEDevRebootResp pMsg = (PKEDevRebootResp)msgData;
	respData[KEMSG_EVENT_DEVREBOOT] = pMsg->respData;
	Sleep(0);
	keEvent[KEMSG_EVENT_DEVREBOOT].SetEvent();	
	TRACE("RecvCancelSharedDevResp--end\n");
}

struct VersionInfo
{
	string version;
	string url;
};

int CP2PCmdSocket::QueryVersion( CString & version ,CString & url )
{
	KECheckVersionMsg msg;
	int msgLen = sizeof(KECheckVersionMsg);
	msg.head.clientID = m_clientID;
	msg.head.msgLength = msgLen;
	msg.head.msgType = KEMSG_TYPE_CHECKVERSION;
	msg.head.protocal = PROTOCOL_HEAD;
	msg.type = 1;
	int ret = this->Write((BYTE *)&msg,msgLen);
	if (ret != msgLen)
	{
		return KE_SOCKET_WRITEERROR;
	}
	//return KE_SUCCESS;
	keEvent[KEMSG_EVENT_CHECKVERSION].ResetEvent();
	DWORD dw = WaitForSingleObject(keEvent[KEMSG_EVENT_CHECKVERSION].m_hObject,MSG_WAIT_TIMEOUT);
	if (dw == WAIT_TIMEOUT)
	{
		TRACE("QueryVersion--Timeout\n");
		return KE_MSG_TIMEOUT;
	}
	VersionInfo *info = (VersionInfo* )respData[KEMSG_EVENT_CHECKVERSION];
	version = info->version.c_str();
	url = info->url.c_str();
	delete info;
	return KE_SUCCESS;
}

void CP2PCmdSocket::RecvCheckVersionResp( const BYTE * msgData )
{
	PKECheckVersionMsg pMsg = (PKECheckVersionMsg)msgData;
	char * data = (char *)msgData + sizeof(KECheckVersionMsg);
	short verLen = *((short *)data);
	data += 2;
	VersionInfo *info = new VersionInfo;
	info->version.insert(0,data,verLen);
	data += verLen;
	short urlLen = *((short *)data);
	data += 2;
	info->url.insert(0,data,urlLen);
	respData[KEMSG_EVENT_CHECKVERSION] = (int)info;
	Sleep(0);
	keEvent[KEMSG_EVENT_CHECKVERSION].SetEvent();	
	TRACE("RecvCheckVersionResp--end\n");
}

int CP2PCmdSocket::ResetForgetPwd( CString uname,CString email )
{
// 	if (!m_SocketClient.IsOpen())
// 	{
// 		return KE_SOCKET_NOTOPEN;
// 	}
// 	PKEForgetPWDReq pReq;
// 	CMarkup xml;
// 	xml.SetDoc("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\r\n");
// 	xml.AddElem("Reg");
// 	xml.IntoElem();
// 	xml.AddElem("Info");
// 	xml.AddAttrib("U",uname);
// 	xml.AddAttrib("E",email);
// 	xml.OutOfElem();
// 	CString xmlStr= xml.GetDoc();
// 	int msgLen = sizeof(KEForgetPWDReq) + xmlStr.GetLength();
// 	msgSend.resize(msgLen+1);
// 	pReq = (PKEForgetPWDReq)&msgSend[0];
// 	pReq->head.protocal = PROTOCOL_HEAD;
// 	pReq->head.msgType = KEMSG_TYPE_FORGETPWD;
// 	pReq->head.clientID = m_clientID;
// 	pReq->head.length = msgLen;
// 	//memcpy(&msgSend[0],&req,sizeof(KEForgetPWDReq));
// 	strcpy((char *)&msgSend[sizeof(KEForgetPWDReq)],xmlStr.GetString());
// 
// 	keEvent[KEMSG_EVENT_FORGETPWD].ResetEvent();
// 
// 	this->Write(&msgSend[0],msgLen);
// 	
// 	
// 	DWORD dw = WaitForSingleObject(keEvent[KEMSG_EVENT_FORGETPWD].m_hObject,MSG_WAIT_TIMEOUT);
// 	if (dw == WAIT_TIMEOUT)
// 	{
// 		return KE_MSG_TIMEOUT;
// 	}
// 	int resp = respData[KEMSG_EVENT_FORGETPWD];
// 	if (resp != KE_SUCCESS)
// 	{
// 		return resp;
// 	}
// 	
// 	return resp;
	return 0;
}

void CP2PCmdSocket::RecvForgetPWD( const BYTE * msgData )
{
	PKEForgetPWDResp pMsg = (PKEForgetPWDResp)msgData;
	respData[KEMSG_EVENT_FORGETPWD] = pMsg->respData;
	Sleep(0);
	keEvent[KEMSG_EVENT_FORGETPWD].SetEvent();	
	TRACE("RecvCancelSharedDevResp--end\n");
}

int CP2PCmdSocket::RequestTransIp( int devID )
{
	KEGetTransIPReq msg;
	int msgLen = sizeof(KEGetTransIPReq);
	msg.head.clientID = m_clientID;
	msg.head.msgLength = msgLen;
	msg.head.msgType = KEMSG_TYPE_GetTransIP;
	msg.head.protocal = PROTOCOL_HEAD;
	msg.devID = devID;
	int ret = this->Write((BYTE *)&msg,msgLen);
	if (ret !=msgLen)
	{
		return KE_SOCKET_WRITEERROR;
	}
	DevSvrInfo * info;
	ret = this->WaitRecvMsg(KEMSG_TYPE_GetTransIP,MSG_WAIT_TIMEOUT,(void**)&info);
	if (ret != KE_SUCCESS)//timeout
	{
		switch(ret)
		{
		case 5: this->SetErrorDesA("�豸������", ret); break;
		case 6:this->SetErrorDesA("û�к��ʵ�ý��" , ret); break;
		}
		return ret;
	}
	devSvrMap[devID].svrIp = info->svrIp;
	devSvrMap[devID].svrPort = info->svrPort;
	//delete info;
	//info = NULL;
	return ret;
}

void CP2PCmdSocket::RecvTransIp( const BYTE * msgData )
{
	PKEGetTransIPResp pMsg = (PKEGetTransIPResp)msgData;
	if (pMsg->respData == 13)//�ɹ�
	{
		DevSvrInfo * info = new DevSvrInfo;
		info->svrIp = pMsg->transIp;
		info->svrPort = pMsg->transPort;

		this->SetRecvMsg(KEMSG_TYPE_GetTransIP,pMsg->respData,info);
		TRACE2("RecvTransIp-- Ip %d,port %d\n",pMsg->transIp,pMsg->transPort);
	}
	else
		this->SetRecvMsg(KEMSG_TYPE_GetTransIP,pMsg->respData);

}

int CP2PCmdSocket::SetClientID( int clientID )
{
	this->m_clientID = clientID;
	return KE_SUCCESS;
}

int CP2PCmdSocket::RefreshCameraList()
{
	return KE_FUNCTION_NOTSUPPORT;
}

int CP2PCmdSocket::PTZControl( int cameraID, BYTE ctrlType ,BYTE speed ,BYTE data )
{
	return KE_FUNCTION_NOTSUPPORT;
}

int CP2PCmdSocket::HeartBeat()
{
	this->m_heartCount++;

	KEMsgHead msg;
	msg.clientID = this->m_clientID;
	msg.msgLength = sizeof(KEMsgHead);
	msg.msgType = KE_P2PMSG_TYPE_HEARTBEAT;
	msg.protocal = PROTOCOL_HEAD;
	int ret = this->Write((BYTE *)&msg,msg.msgLength);
	if (ret != msg.msgLength)
	{
		return KE_SOCKET_WRITEERROR;
	}
	return ret;
}

int CP2PCmdSocket::ConnectToMedia( PKEDevNetInfoResp pMsg )
{
	CString tmpIP; 
	struct in_addr in;
	int camID = pMsg->devID<<8+1;
	CMediaSocket * media = GetMediaSocket(camID);

	if (media->IsConnect())
	{
		return KE_SUCCESS;
	}
	//ʹ������IP��½
	in.s_addr =pMsg->devOuterIP ;
	tmpIP = inet_ntoa(in);
	TRACE2("RecvDevNetInfoResp--������½,�豸IP:%s;�˿�%d\n",tmpIP,pMsg->devOuterPort);
	if (media->ConnectToServer(pMsg->devOuterIP,pMsg->devOuterPort))
	{
		devSvrMap[pMsg->devID].svrIp = pMsg->devOuterIP;
		devSvrMap[pMsg->devID].svrPort = pMsg->devOuterPort;
		devSvrMap[pMsg->devID].connectSuccess = true;
		return KE_SUCCESS;
	}
	//ֱ��ip��½��ʹ�������˿�
	in.s_addr =pMsg->devSvrIp ;
	tmpIP = inet_ntoa(in);
	TRACE2("RecvDevNetInfoResp--����IP��½,�豸IP:%s;�˿�%d\n",tmpIP,pMsg->devOuterPort);
	if ((pMsg->devSvrIp != pMsg->devOuterIP) && media->ConnectToServer(pMsg->devSvrIp,pMsg->devOuterPort))
	{
		devSvrMap[pMsg->devID].svrIp = pMsg->devSvrIp;
		devSvrMap[pMsg->devID].svrPort = pMsg->devOuterPort;
		devSvrMap[pMsg->devID].connectSuccess = true;
		return KE_SUCCESS;
	}
	//ʹ������IP��½
	in.s_addr =pMsg->devInnerIP ;
	tmpIP = inet_ntoa(in);
	TRACE2("RecvDevNetInfoResp--������½,�豸IP:%s;�˿�%d\n",tmpIP,pMsg->devInnerPort);
	if ((pMsg->devInnerIP != pMsg->devOuterIP || pMsg->devOuterPort != pMsg->devInnerPort)  && 
		media->ConnectToServer(pMsg->devInnerIP,pMsg->devInnerPort))
	{
		TRACE("RecvDevNetInfoResp--�豸������½�ɹ�!\n");
		devSvrMap[pMsg->devID].svrIp = pMsg->devInnerIP;
		devSvrMap[pMsg->devID].svrPort = pMsg->devInnerPort;
		devSvrMap[pMsg->devID].connectSuccess = true;
		return KE_SUCCESS;
	}

	//PPPOE��ʹ�������˿�
	in.s_addr =pMsg->devSvrIp ;
	tmpIP = inet_ntoa(in);
	TRACE2("RecvDevNetInfoResp--PPPOE��½,�豸IP:%s;�˿�%d\n",tmpIP,pMsg->devInnerPort);
	if ((pMsg->devSvrIp != pMsg->devInnerIP) && media->ConnectToServer(pMsg->devSvrIp,pMsg->devInnerPort))
	{
		TRACE("RecvDevNetInfoResp--ֱ��ip��½�ɹ�!\n");
		devSvrMap[pMsg->devID].svrIp = pMsg->devSvrIp;
		devSvrMap[pMsg->devID].svrPort = pMsg->devInnerPort;
		devSvrMap[pMsg->devID].connectSuccess = true;
		return KE_SUCCESS;
	}

	//ʹ�ñ���ý��ip��½
	if (pMsg->mediaBackIP == 0)
	{
		int ret = 	RequestTransIp(pMsg->devID);
		if (ret != KE_SUCCESS)
		{
			return ret;
		}
	}
	else
	{
		in.s_addr =pMsg->mediaBackIP ;
		tmpIP = inet_ntoa(in);
		TRACE2("RecvDevNetInfoResp--ý���½,�豸IP:%s;�˿�%d\n",tmpIP,pMsg->mediaBackPort);
		devSvrMap[pMsg->devID].svrIp = pMsg->mediaBackIP;
		devSvrMap[pMsg->devID].svrPort = pMsg->mediaBackPort;
		devSvrMap[pMsg->devID].connectSuccess = false;
	}
	if (media->ConnectToServer(devSvrMap[pMsg->devID].svrIp,devSvrMap[pMsg->devID].svrPort,2,m_clientID))
	{
		TRACE("RecvDevNetInfoResp--ý���½�ɹ�!\n");
		return KE_SUCCESS;
	}
	TRACE("RecvDevNetInfoResp--�豸��½ʧ��!\n");
	return KE_RTV_BOTHOFFLINE;
}

int CP2PCmdSocket::ConnectServer( CString svrIp,int svrPort )
{
	if (ConnectToServer(svrIp,svrPort))
	{
		return KE_SUCCESS;
	}
	else
	{
		return KE_CONNECT_SERVER_ERROR;
	}
		
}

int CP2PCmdSocket::GetRecordFileList( int cameraID,int startTime,int endTime,int fileType,vector<RecordFileInfo> & fileInfoList )
{
	int ret = SetCameraMedia(cameraID);
	if (ret != KE_SUCCESS)
	{
		return ret;
	}
	CMediaSocket *media = GetMediaSocket(cameraID);
	if (media->GetMediaSvrType() == 1)
	{
		ret = media->GetRecordFileList(cameraID,startTime,endTime,fileType,fileInfoList);
	}
	else
	{
		ret = this->RequestRecordFileList(cameraID,startTime,endTime,fileType);
		if (ret == KE_SUCCESS)
		{
			fileInfoList = media->recordFileList;
		}
	}
	return ret;
}

int CP2PCmdSocket::PlayRemoteRecord( int cameraID,int fileNo )
{
	int devSvrID = cameraID/256;
	int ret = KE_SUCCESS;
	ret = SetCameraMedia(cameraID) ;
	if (ret != KE_SUCCESS)
	{
		return ret;
	}
	CMediaSocket *media = GetMediaSocket(cameraID);
	ret = media->RemoteRecordPlay(cameraID,fileNo);

	return ret;
}

int CP2PCmdSocket::SetCameraMedia( int cameraID )
{
	CMediaSocket *media = GetMediaSocket(cameraID);
	if (media->IsConnect())
	{
		return KE_SUCCESS;
	}
	int devSvrID = cameraID/256;
	if (devSvrMap[devSvrID].connectSuccess)
	{
		if (media->ConnectToServer(devSvrMap[devSvrID].svrIp,devSvrMap[devSvrID].svrPort))
		{
			return KE_SUCCESS;
		}
	}
	return SendDevNetInfoReq(devSvrID);
}

int CP2PCmdSocket::RequestRecordFileList( int cameraID,int startTime,int endTime,int fileType )
{
	if (!m_SocketClient.IsOpen())
	{
		return KE_SOCKET_NOTOPEN;
	}
	int devID = cameraID/256;
	int channelNo = cameraID%256;
	std::vector<BYTE> msgSend;
	int msgLen = sizeof(KEQueryRecordFileListReq);
	msgSend.resize(msgLen,0);
	PKEQueryRecordFileListReq pReqMsg;
	pReqMsg = (PKEQueryRecordFileListReq)&msgSend[0];
	pReqMsg->head.protocal = PROTOCOL_HEAD;
	pReqMsg->head.msgType = KEMSG_UDP_QueryRecordFileList;
	pReqMsg->head.msgLength = msgLen;
	pReqMsg->head.clientID = m_clientID;
	pReqMsg->channelNo = channelNo;
	pReqMsg->devID = devID;
	pReqMsg->data.protocal = PROTOCOL_HEAD;
	pReqMsg->data.msgType = KEMSG_RecordFileList;
	pReqMsg->data.msgLength = msgLen;
	pReqMsg->data.clientID = m_clientID;
	pReqMsg->data.channelNo = channelNo;
	pReqMsg->data.videoID = devID;
	CTime st(startTime);
	CTime et(endTime);
	pReqMsg->data.startTime[0] = st.GetYear()%2000;
	pReqMsg->data.startTime[1] = st.GetMonth();
	pReqMsg->data.startTime[2] = st.GetDay();
	pReqMsg->data.startTime[3] = st.GetHour();
	pReqMsg->data.startTime[4] = st.GetMinute();
	pReqMsg->data.startTime[5] = st.GetSecond();
	pReqMsg->data.endTime[0] = et.GetYear()%2000;
	pReqMsg->data.endTime[1] = et.GetMonth();
	pReqMsg->data.endTime[2] = et.GetDay();
	pReqMsg->data.endTime[3] = et.GetHour();
	pReqMsg->data.endTime[4] = et.GetMinute();
	pReqMsg->data.endTime[5] = et.GetSecond();

	pReqMsg->data.fileType = fileType;
	pReqMsg->data.alarmNo = 0;

	CMediaSocket *media = GetMediaSocket(cameraID);
	media->recordFileList.clear();

	int ret = this->Write(&msgSend[0],msgLen);
	if (ret != msgLen)
	{
		return KE_SOCKET_WRITEERROR;
	}

	ret = this->WaitRecvMsg(KEMSG_UDP_QueryRecordFileList);

	//fileInfoList = media->recordFileList;

	return ret;
}

void CP2PCmdSocket::RecvRecordFileList( const BYTE * msgData )
{
	PKEQueryRecordFileListResp pMsg = (PKEQueryRecordFileListResp)msgData;
	PKERecordFileListResp pAckMsg = &pMsg->ackData;
	if (pAckMsg->msgLength == 16)
	{
		this->SetRecvMsg(KEMSG_UDP_QueryRecordFileList,KE_SUCCESS);
		return;
	}
	RecordFileInfo fileInfo;
	fileInfo.fileNo = pAckMsg->fileNo;
	CTime st(pAckMsg->startTime[0]+2000,pAckMsg->startTime[1],pAckMsg->startTime[2],
		pAckMsg->startTime[3],pAckMsg->startTime[4],pAckMsg->startTime[5]);
	CTime et(pAckMsg->endTime[0]+2000,pAckMsg->endTime[1],pAckMsg->endTime[2],
		pAckMsg->endTime[3],pAckMsg->endTime[4],pAckMsg->endTime[5]);

	fileInfo.startTime = st.GetTime();
	fileInfo.endTime = et.GetTime();
	fileInfo.fileSize = pAckMsg->fileSize;
	memcpy(fileInfo.fileData,pAckMsg->data,80);

	int cameraID = pAckMsg->videoID<<8+pAckMsg->channelNo;
	CMediaSocket *media = GetMediaSocket(cameraID);
	media->recordFileList.push_back(fileInfo);

	if (pAckMsg->resp == 6)
	{
		this->SetRecvMsg(KEMSG_UDP_QueryRecordFileList,KE_SUCCESS);
	}
}

