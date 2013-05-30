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
	virtual int GetDevWifiAPList(int cameraID,std::vector<KEDevAPListItem> &apList);
	virtual int SetDevWifi(int cameraID,int listNo,KEDevWifiStartReq wifiStart);
	std::string GetErrorDesA(int errorCode);
	int platformType;
	CMediaSocket * GetMediaSocket(int cameraID);
protected:
	void SetErrorDesA(std::string des,int errorCode);
	
	

private:
	
	std::string errorDesA;
	int errorCode;
};
