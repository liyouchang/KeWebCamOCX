#pragma once

#include <string>
#include "CommonUtility/tstdlibs.h"
#include "Common.h"
#include "MediaSocket.h"
#include "MyAVPlayer.h"
#include <vector>


struct CamStatusReport
{
	int reportType;//1-选择镜头；2-视频停止; 3-设备上线；4-设备离线
	int cameraID;
	int devID;
	int errorCode;
	bool isRecording;//0-无录像，1-正在本地录像
	int playType;//1-只有视频播放，3-视频和音频播放，7-视频、音频和对讲
};




class CenterCommand
{
public:
	CenterCommand(void);
	~CenterCommand(void);
	virtual int ConnectServer(CString svrIp,int svrPort);
	
	virtual int LoginServer(CString userName,CString pwd);
	virtual int StartView(int cameraID);
	virtual int StopView(int cameraID);
	virtual int PTZControl(int cameraID, BYTE ctrlType ,BYTE speed ,BYTE data);
	virtual int RefreshCameraList();
	virtual int LogoutServer();
	virtual int HeartBeat();
	virtual int SetClientID(int clientID);
	virtual int GetClientID();
	virtual int GetRecordFileList(int cameraID,int startTime,int endTime,int fileType,int targetType,std::vector<RecordFileInfo> & fileInfoList);
	virtual int PlayRemoteRecord(int cameraID,int fileNo);
	virtual int GetDevWifiAPList(int cameraID,std::vector<KEDevAPListItem> &apList);
	virtual int SetDevWifi(int cameraID,int listNo,KEDevWifiStartReq wifiStart);
	virtual int QueryVersion(CString & version ,CString & url);
	virtual int GetVideoParam(int cameraID,KEVedioParam & param);
	virtual int SetVideoParam(int cameraID,const KEVedioParam & param);
	virtual int GetOldVideoParam(int cameraID,KEVedioParam & param);
	std::string GetErrorDesA(int errorCode);
	int platformType;
	CMediaSocket * GetMediaSocket(int cameraID);
	CMyAVPlayer * GetAVIPlayer(int cameraID);
	int ReportCamStatus(const CamStatusReport& report);
	void SetErrorDesA(std::string des,int errorCode);
protected:
private:
	
	std::string errorDesA;
	int errorCode;
};
