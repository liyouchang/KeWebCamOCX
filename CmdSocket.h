#pragma once

#include <afxmt.h>
#include "SocketThreadHandler.h"



 


class CCmdSocket :public CSocketThreadHandler
{
	#define  RECV_BUF_SIZE  0x40000
public:
	CCmdSocket(void);
	virtual ~CCmdSocket(void);

	virtual bool Init();

	
	bool ConnectToServer(CString severAddr);
	int LoginServer(CString userName,CString pwd);

	//************************************
	// Method:    AskKeyt ������Կ
	// Access:    public 
	// Returns:   int
	// Qualifier:
	//************************************
	
	
protected:
	virtual void HandleMessage(const BYTE* msgData);
protected:
	int AskKeyt(char * keyt);
	int SendLoginMsg(const char * userName,const char *encryptData);
	void AskKeyMsgResp(const BYTE* msgData);
	void LoginMsgResp(const BYTE* msgData);

private:
	
	
	CString serverPort;//������port 22616
	
	int m_clientID;

	CEvent keEvent[KEMSG_EVENT_MAX];		//����key
	void * tmpData[KEMSG_EVENT_MAX];	//������Ϣ��ȡ������,resp��������,ʹ�����ͷ�


private:

	void EncryptData(const char * userName,const char * pwd,const char * keyt,char * encryptedData);
};
