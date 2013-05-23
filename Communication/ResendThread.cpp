#include "stdafx.h"
#include "ResendThread.h"

CResendThread::CResendThread(CSocketThreadHandler * socketHandle)
{
	this->m_socketHandle = socketHandle;
}

CResendThread::~CResendThread(void)
{
	TRACE("~CResendThread start %d\n",GetTickCount());
	this->Stop(2000);
	TRACE("~CResendThread end %d\n",GetTickCount());
}

void CResendThread::AddResendMsg( int id,vector<BYTE> data )
{
	CSingleLock lock(&m_cs, TRUE);
	ResendMsg newResend(data,id);
	m_resendMsgList.push_back(newResend);
}

void CResendThread::Run()
{
	CSingleLock lock(&m_cs, FALSE);
	while (!this->toStop)
	{
		Sleep(1000);
		if (m_resendMsgList.empty())
		{
			continue;
		}
		list <ResendMsg>::iterator msgIter;
		lock.Lock();
		for (msgIter = m_resendMsgList.begin();msgIter != m_resendMsgList.end(); )
		{
			ResendMsg * pMsg = &(*msgIter);		
			if (pMsg->time >= 3)  //发送次数大于3次
			{
				TRACE1("Erase message %d after send 3 times.\n",pMsg->msgID);
				msgIter = m_resendMsgList.erase(msgIter);
				continue;
			}
			if (GetTickCount() - pMsg->lastSendTime > pMsg->eachResendTime) //3s重发一次信息
			{
				TRACE2("Resend message %d,time %d.\n",pMsg->msgID,pMsg->time);
				if (m_socketHandle->IsConnect())
				{
					m_socketHandle->Write(&pMsg->data[0],pMsg->data.size());
				}
				pMsg->lastSendTime = GetTickCount();
				pMsg->time++;
			}
			msgIter++;
		}
		lock.Unlock();
	}
	TRACE("Resend thread exit");
}

void CResendThread::RemoveResendMsg( int id )
{
	CSingleLock lock(&m_cs, TRUE);
	PredicateID<ResendMsg> rmID(id);
	TRACE1("remove message %d when responded.\n",id);

	m_resendMsgList.remove_if(rmID);
}

int CResendThread::GetResendTime( int id )
{
	CSingleLock lock(&m_cs, TRUE);
	list <ResendMsg>::iterator msgIter;
	int resendTime = 0;
	for (msgIter = m_resendMsgList.begin();msgIter != m_resendMsgList.end(); )
	{
		ResendMsg * pMsg = &(*msgIter);		
		if (pMsg->msgID == id)
		{
			resendTime = pMsg->time;
		}
	}
	return resendTime;

}
