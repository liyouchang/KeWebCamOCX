#pragma once

#include <afxmt.h>
#include "../SocketThreadHandler.h"
#include "ResendThread.h"
#include "../CenterCommand.h"
#include <map>

//#include "MediaSocket.h"
//#define WM_HEARTBEATSTOP WM_USER+1029
#define  WM_ALLCAMRELOGIN WM_USER+1030


#pragma pack(1)

// typedef struct _KEMsgHead
// {
// 	BYTE protocal;
// 	BYTE msgType;
// 	int length;
// 	int clientID;
// }KEMsgHead,*PKEMsgHead;

typedef struct _KEUserRegisterReq
{
	KEMsgHead head;
	BYTE type;
	int tmpID;
	//	vector<char>  data;//<Reg><Info U=”username” P=”password” E=”email”></Reg>
}KEUserRegisterReq,*PKEUserRegisterReq;

typedef struct _KEUserRegisterResp
{
	KEMsgHead head;
	BYTE type;
	int tmpID;
	char respData;
}KEUserRegisterResp,*PKEUserRegisterResp;

typedef struct _KEUserLoginReq
{
	KEMsgHead head;
}KEUserLoginReq,*PKEUserLoginReq;

typedef struct _KEUserLoginResp
{
	KEMsgHead head;
	char respData;
}KEUserLoginResp,*PKEUserLoginResp;

//6.	删除设备节点
typedef struct _KEDeleteCameraReq
{
	KEMsgHead head;
	char devType;
	int devID;
}KEDeleteCameraReq,*PKEDeleteCameraReq;

typedef struct _KEDeleteCameraResp
{
	KEMsgHead head;
	char devType;
	int devID;
	char respData;
}KEDeleteCameraResp,*PKEDeleteCameraResp;

struct KEDeviceStatus
{
	KEMsgHead head;
	int devID;
	int status;
};

typedef struct _KEAlarm
{
	KEMsgHead head;
	int devID;
	char alarmType; //类型：0-移动侦测 1-图像遮挡 2-视频信号丢失。
	char alarmStatus; //报警标识：0停止 1开始
	char time[20];
}KEAlarm,*PKEAlarm;

typedef struct _KEAddDeviceReq
{
	KEMsgHead head;
	BYTE type;
	//	vector<char>  data;//<AddDevice><Info D=”Mac” N=”devicename” ></AddDevice>
}KEAddDeviceReq,*PKEAddDeviceReq;

typedef struct _KEAddDeviceResp
{
	KEMsgHead head;
	BYTE type;
	int devID;
	BYTE respData;//应答信息：13成功 5失败-已经被其他用户添加过 6添加数据库失败。
}KEAddDeviceResp,*PKEAddDeviceResp;

typedef struct _KEShareDeviceReq
{
	KEMsgHead head;
	BYTE type;
	int tempUserID;
	//	vector<char>  data;//<AddDeviceShare><Info D=”DeviceID” U=”OtherUserName” ></AddDeviceShare>
}KEShareDeviceReq,*PKEShareDeviceReq;

typedef struct _KEShareDeviceResp
{
	KEMsgHead head;
	BYTE type;
	int tempUserID;
	BYTE respData;//应答信息：13成功 5失败-已经被其他用户添加过 6添加数据库失败。
}KEShareDeviceResp,*PKEShareDeviceResp;

typedef struct _KEDevNetInfoReq
{
	KEMsgHead head;
	BYTE type;
	int devID;
}KEDevNetInfoReq,*PKEDevNetInfoReq;

typedef struct _KEDevNetInfoResp
{
	KEMsgHead head;
	BYTE type;
	BYTE respData;
	int devID;//这里出现的devID都是设备在中心数据库中的ID
	int devInnerIP;
	int devInnerPort;
	int devOuterIP;
	int devOuterPort;
	int devSvrIp;
	int devSvrPort;
	int mediaBackIP;
	int mediaBackPort;
}KEDevNetInfoResp,*PKEDevNetInfoResp;

typedef struct _KEChangeDeviceNameReq
{
	KEMsgHead head;
	BYTE type;
	//	vector<char>  data;//<ModifyDevice><Info D=”DeviceID” U=”DeviceName” ></ModifyDevice>
}KEChangeDeviceNameReq,*PKEChangeDeviceNameReq;

typedef struct _KEChangeDeviceNameResp
{
	KEMsgHead head;
	BYTE type;
	BYTE respData;//应答信息：13成功 5失败
}KEChangeDeviceNameResp,*PKEChangeDeviceNameResp;

//16.	查询指定设备所共享的用户信息
typedef struct _KECheckDevShareUserReq
{
	KEMsgHead head;
	int devSvrID;
}KECheckDevShareUserReq,*PKECheckDevShareUserReq;

//17.	删除共享设备给指定的用户,服务器主动下发此协议
typedef struct _KEDevNotSharedNotice
{
	KEMsgHead head;
	int devSvrID;
}KEDevNotSharedNotice,*PKEDevNotSharedNotice;

//18.	取消设备共享
typedef struct _KECancelSharedDevReq
{
	KEMsgHead head;
	BYTE type;//类型：16
	int devSvrID;
	int sharedUserID;
}KECancelSharedDevReq,*PKECancelSharedDevReq;

typedef struct _KECancelSharedDevResp
{
	KEMsgHead head;
	BYTE type;//类型：16
	int devSvrID;
	int sharedUserID;
	char respData;//13-成功  5-共享用户ID不存在 6-操作数据库失败
}KECancelSharedDevResp,*PKECancelSharedDevResp;

//12.	修改客户端密码
typedef struct _KEChangeUserPwdReq
{
	KEMsgHead head;
	BYTE type;//类型：14
	char pwdData[40];
}KEChangeUserPwdReq,*PKEChangeUserPwdReq;

typedef struct _KEChangeUserPwdResp
{
	KEMsgHead head;
	BYTE type;//类型：14
	char respData;//13成功 5失败。
}KEChangeUserPwdResp,*PKEChangeUserPwdResp;

//设备重启
typedef struct _KEDevRebootReq
{
	KEMsgHead head;
	BYTE type;//类型：none
	int devID;
}KEDevRebootReq,*PKEDevRebootReq;

typedef struct _KEDevRebootResp
{
	KEMsgHead head;
	BYTE type;//类型：none
	char respData;//13成功 5失败。
}KEDevRebootResp,*PKEDevRebootResp;

typedef struct _KECheckVersonMsg
{
	KEMsgHead head;//0xf9
	char type;//：0-mobile 1-web
}KECheckVersionMsg,*PKECheckVersionMsg;

typedef struct _KEForgetPWDReq
{
	KEMsgHead head;//0x20
	char type;//不用填
	//<Reg>	<Info U=”username” E=”email”></Reg>
}KEForgetPWDReq,*PKEForgetPWDReq;

typedef struct _KEForgetPWDResp
{
	KEMsgHead head;//0x20
	char type;//不用填
	char respData;//13用户名和邮箱匹配 5-用户名和邮箱不匹配 6-用户名不存在 7-数据库修改失败
}KEForgetPWDResp,*PKEForgetPWDResp;

typedef struct _KEGetTransIPReq
{
	KEMsgHead head;//0x22
	int devID;
}KEGetTransIPReq,*PKEGetTransIPReq;

typedef struct _KEGetTransIPResp
{
	KEMsgHead head;//0x22
	char respData;
	int transIp;
	int transPort;
}KEGetTransIPResp,*PKEGetTransIPResp;

typedef struct _KEQueryRecordFileListReq
{
	KEMsgHead head;//0x23
	int devID;
	char channelNo;
	KERecordFileListReq data;
}KEQueryRecordFileListReq,*PKEQueryRecordFileListReq;

typedef struct _KEQueryRecordFileListResp
{
	KEMsgHead head;//0x23
	KERecordFileListResp ackData;
}KEQueryRecordFileListResp,*PKEQueryRecordFileListResp;

typedef struct _KEPTZCtrlUDPReq
{
	KEMsgHead head;//0x21
	int devID;
	KEDevGetSerialDataHead data;
}PTZCtrlUDPReq,*PPTZCtrlUDPReq;

#pragma pack()



class CP2PCmdSocket :
	public CSocketThreadHandler,public CenterCommand
{
	//#define  RECV_BUF_SIZE  0x40000

	struct DevSvrInfo
	{
		DevSvrInfo(){
			connectSuccess=false;
		}
		int svrIp;
		int svrPort;
		bool connectSuccess;
	};
public:
	CP2PCmdSocket(void);
	virtual ~CP2PCmdSocket(void);
	
	virtual bool Init();
	
	bool ConnectToServer(CString severAddr);
	bool ConnectToServer(CString severAddr,int svrPort);
	virtual int ConnectServer(CString svrIp,int svrPort);
	virtual int StartView(int cameraID);
	virtual int StopView(int cameraID);
	virtual int PTZControl(int cameraID, BYTE ctrlType ,BYTE speed ,BYTE data);
	virtual int RefreshCameraList();
	virtual int SetClientID(int clientID);
	virtual int HeartBeat();
	virtual int GetRecordFileList(int cameraID,int startTime,int endTime,int fileType,vector<RecordFileInfo> & fileInfoList);
	virtual int PlayRemoteRecord(int cameraID,int fileNo);
	virtual int GetDevWifiAPList(int cameraID,std::vector<KEDevAPListItem> &apList);
	virtual int SetDevWifi(int cameraID,int listNo,KEDevWifiStartReq wifiStart);
	virtual int QueryVersion(CString & version ,CString & url);
	
	int LogoutServer();
	

	void CheckAllDevListAsyn();
	void CheckShareDevListAsyn();
	int DeleteCamera(char type,int devID);
	int AddDevice(CString mac,CString devName);
	int ShareCamera(CString userName,int devID);

	int SendHeartBeat();
	int ChangeDevName(int devID,CString devNewName);
	int CheckDevShareUser(int devSvrID);
	int CancelDevShare(int devSvrID,int sharedUserID); 
	int ChangeUserPwd(CString oldPwd, CString newPwd);
	int DeviceReboot(int devID);
	
	int ResetForgetPwd(CString uname,CString email);
	int RequestTransIp(int devID);
	int RequestRecordFileList(int cameraID,int startTime,int endTime,int fileType);
protected:
	virtual void HandleMessage(const BYTE* msgData);
	std::map<int ,DevSvrInfo> devSvrMap;
	//连接媒体
	int ConnectToMedia(PKEDevNetInfoResp pMsg);
	int SetCameraMedia(int cameraID);
protected:
	int SendLoginMsg(const char * userName,const char *encryptData);
	void AskKeyMsgResp(const BYTE * msgData);
	void LoginMsgResp(const BYTE * msgData);
	int RequestDevStatus(int devID);
	int RequestDevNetInfo(int devID);
	int SendPTZControl(int cameraID, BYTE ctrlType ,BYTE speed );
protected://received message functions 
	void RecvRealTimeMedia(const BYTE * msgData);
	void RecvUserRegister(const BYTE * msgData);
	void RecvDevList(const BYTE* msgData);
	void RecvShareDevList(const BYTE* msgData);
	void RecvDeleteDev(const BYTE * msgData);
	void RecvHeartbeat(const BYTE *msgData);
	void RecvDevStatus(const BYTE *msgData);
	void RecvAlarm(const BYTE *msgData);
	void RecvAddCamResp(const BYTE *msgData);
	void RecvDevNetInfoResp(const BYTE *msgData);
	void RecvChangeDevName(const BYTE *msgData);
	void RecvShareDevResp(const BYTE * msgData);
	void RecvChkSharedUserResp(const BYTE *msgData);
	void RecvDevNotSharedNotice(const BYTE *msgData);
	void RecvCancelSharedDevResp(const BYTE * msgData);
	void RecvChangeUserPwdResp(const BYTE * msgData);
	void RecvDevRebootResp(const BYTE * msgData);
	void RecvCheckVersionResp(const BYTE * msgData);
	void RecvForgetPWD(const BYTE * msgData);
	void RecvTransIp(const BYTE * msgData);
	void RecvRecordFileList(const BYTE * msgData);
private:
	CResendThread *m_ResendThread;
	int m_clientID;
	CEvent keEvent[KEMSG_EVENT_MAX];		//申请key
	int respData[KEMSG_EVENT_MAX];	//resp数据
	volatile   int m_heartCount;
private:

};


enum KE_P2PMSG_TYPE
{
	KEMSG_TYPE_USERREGISTER = 0x00,
	KEMSG_TYPE_USERLOGIN = 0x01,
	KE_P2PMSG_TYPE_HEARTBEAT = 0x02,
	KEMSG_TYPE_DEVLIST = 0x03,
	KEMSG_TYPE_DELETEDEV = 0x05,
	KEMSG_TYPE_DEVSTATUS = 0x07,
	KEMSG_TYPE_ALARM = 0x08,
	KEMSG_TYPE_ADDDEVICE = 0x09,
	KEMSG_TYPE_SHARECAM = 0x10,
	KEMSG_TYPE_CHGUSERPWD = 0x11,
	KEMSG_TYPE_CHGDEVNAME = 0x12,
	KEMSG_TYPE_DEVNETINFO = 0x13,
	KEMSG_TYPE_USERLOGOUT =0x14,
	KEMSG_TYPE_CHECKSHAREDUSER = 0x15,
	KEMSG_TYPE_DEVNOTSHARED = 0x16,
	KEMSG_TYPE_CANCELSHAREDDEV = 0x17,
	KEMSG_TYPE_SHAREDEVLIST = 0x18,
	KEMSG_TYPE_DEVREBOOT = 0x19,
	KEMSG_TYPE_FORGETPWD = 0x20,
	KEMSG_UDP_PTZControl=0x21,
	KEMSG_TYPE_GetTransIP = 0x22,
	KEMSG_UDP_QueryRecordFileList= 0x23,
	KEMSG_TYPE_CHECKVERSION = 0xf9,
};

enum KE_P2PMSG_EVENT
{
	//KEMSG_EVENT_ASKKEY = 0,
//	KEMSG_EVENT_LOGIN,
	KEMSG_EVENT_USERREGISTER,
	KEMSG_EVENT_DELETEDEV,
	KEMSG_EVENT_ADDCAM,
	KEMSG_EVENT_SHARECAM,
	KEMSG_EVENT_CHGDEVNAME,
	KEMSG_EVENT_CHKSHAREDUSER,
	KEMSG_EVENT_CANCELSHAREDDEV,
	KEMSG_EVENT_CHGUSERPWD,
	KEMSG_EVENT_DEVREBOOT,
	KEMSG_EVENT_FORGETPWD,
//	KEMSG_EVENT_MAX
};

//#define SECRETKEY_LEN 10
#define USERNAME_LEN 8
#define PASSWORD_LEN 8
//#define  ENCRYPTED_DATA_LEN 33
