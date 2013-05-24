#pragma once

#include <string>
#include "CommonUtility/tstdlibs.h"
#include "Common.h"
#include "MediaSocket.h"
#include <vector>




class CenterCommand
{
public:
	CenterCommand(void);
	~CenterCommand(void);
	virtual int ConnectServer(CString svrIp,int svrPort);
	
	virtual int LoginServer(CString userName,CString pwd);
	virtual int StartView(int cameraID);
	virtual int PTZControl(int cameraID, BYTE ctrlType ,BYTE speed ,BYTE data);
	virtual int RefreshCameraList();
	virtual int LogoutServer();
	virtual int HeartBeat();
	virtual int SetClientID(int clientID);
	virtual int GetClientID();
	virtual int GetRecordFileList(int cameraID,int startTime,int endTime,int fileType,std::vector<RecordFileInfo> & fileInfoList);
	virtual int PlayRemoteRecord(int cameraID,int fileNo);
	std::string GetErrorDesA(int errorCode);
	int platformType;
protected:
	void SetErrorDesA(std::string des,int errorCode);
	
	CMediaSocket * GetMediaSocket(int cameraID);

private:
	
	std::string errorDesA;
	int errorCode;
};
