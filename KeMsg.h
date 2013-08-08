#ifndef ___KEMSG_H___
#define ___KEMSG_H___



//********************protocol type*****************************
#define CHANGE_CENTERIP              0xD1                                   //更改中心IP
#define MODIFY_PASSWORD              0x8E									//修改密码
#define LOGIN_JRSERVER               0x80									//登录服务器
#define REQUEST_TREESTRUCT           0x81									//请求树形结构
#define HEART_TOJRSERVER             0x82									//心跳
#define REQUEST_DATA                 0x83									//请求视频/音频/对讲数据
#define SET_VDOPARAM                 0x84									//设置视频亮度参数
#define GET_VDOPARAM                 0x86									//获取视频参数
#define SET_YT                       0x85									//云台设置 
#define SENDDATA_TO_CLIENT           0x89									//向二级客户端发送报警图像数据
#define REQUEST_FILE_LIST            0x53									//请求文件列表
#define REQUEST_DOWNLOAD_FILE        0x54									//请求下载文件
#define SEARCH_VIDEOSTATUS           0x8b									//视频通道状态查询
#define DELETE_NODE                  0x8c									//删除节点
#define REQUEST_HISTORY_DATA         0x8d									//请求历史书
#define REQUEST_TO_VDOSERVER         0x0c									//向视频服务器请求数据
#define SEND_TALKDATA_TOMEDIASERVER  0x38
#define SEND_ALERTSET                0x20					                //报警器的布撤防
#define SETCONTROLALERTTIME          0x23									//设置布撤防时间
#define SETCONTROLALERTTIME1         0x6C									//设置布撤防时间
#define GETCONTROLALERTTIME1         0x6D									//获取布撤防时间
#define SETALERTPARAM                0x40									//设置输入报警参数
#define GETALERTPARAM                0x65									//获取输入报警参数
#define GETINALERTFROMDATABASE       0x27									//获取输入报警信息从中心服务器
#define CONTROLGPS                   0x18									//接收GPS数据
#define CONNECTSECONDCLIENT			 0x89									//与二级客户端通信
#define SERIVALPARAM                 0xC3                                   //设置取串口参数
#define GET_SERIVALPARAM             0x61                                   //获取串口参数子协议
#define SET_SERIVALPARAM             0x3D                                   //设置串口参数子协议
#define GET_SERIVALDATA              0x2C                                   //获取串口数据
#define SET_DECODERRUN               0xFB                                   //设置解码器是否轮视频
#define GET_DECODERPT_TURNARAM       0xFC                                   //获取解码器轮视频参数
#define SET_DECODERPT_TURON          0xFD                                   //解码器是否开始轮
#define SET_CAMERA_PARAM             0xD4                                   //设置镜头参数
#define GET_CAMERA_PARAM             0xD3                                  //获取镜头参数
#define SET_DEVICE_PARAM             0x9E                                   //设置终端设备参数
#define GET_DEVICE_PARAM             0x9F                                   //获取终端设备参数
#define SET_NET_PARAM                0x47                                   //设置设备网络参数
#define GET_NET_PARAM                0x69                                   //获取设备网络参数
#define SET_VIDEO_TITLE              0x42                                   //设置视频服务器通道的标题
#define GET_OUTPUT_ALARM             0x66                                   //获取输出报警
#define SET_OUTPUT_ALARM             0x41                                   //设置输出报警


#define PROTOCOL_HEAD                0xFF									//协议头
#define MSG_WAIT_TIMEOUT 3000	//相应消息读取等待时间



enum KEMSG_EVENT
{
	KEMSG_EVENT_ASKKEY = 0,
	KEMSG_EVENT_LOGIN,
	KEMSG_EVENT_REALTIMEDATA,
	KEMSG_EVENT_VideoSvrOnline,
	KEMSG_EVENT_MAX
};


#define SECRETKEY_LEN 8 
#define USERNAME_LEN 8
#define PASSWORD_LEN 8
#define  ENCRYPTED_DATA_LEN 16

inline int MakeCameraID(int videoID,int channelNo)
{
	return (videoID<<8) + channelNo;
}


#pragma pack(1)

typedef struct _KEMsgHead
{
	BYTE protocal;
	BYTE msgType;
	unsigned int msgLength;
	int clientID;
}KEMsgHead,*PKEMsgHead;

typedef struct _KEMsgSecretKeyReq
{
	KEMsgHead head;

} MsgSecretKeyReq,*PMsgSecretKeyReq;
typedef struct _KEMsgSecretKeyResp
{
	KEMsgHead head;
	char keyt[SECRETKEY_LEN];
} MsgSecretKeyResp ,*PMsgSecretKeyResp;

typedef struct _KEMsgUserLoginReq
{
	KEMsgHead head;
	int clientLevel;
	char userName[USERNAME_LEN];
	char keyt[SECRETKEY_LEN];
	char encreptData[ENCRYPTED_DATA_LEN];
}KEMsgUserLoginReq ,*PKEMsgUserLoginReq;

typedef struct _KEMsgUserLoginResp
{
	KEMsgHead head;
	int clientLevel;
	char respData;
}KEMsgUserLoginResp,*PKEMsgUserLoginResp;

typedef struct _KEMsgXMLInfo
{
	KEMsgHead head;
	int dataServerID;
}KEMsgXMLInfo,*PKEMsgXMLInfo;

typedef struct _KEMsgHeartBeat
{
	KEMsgHead head;//0x82
	char status;
}KEMsgHeartBeat,*PKEMsgHeartBeat;

typedef struct _KEMsgRealTimeDataReq
{
	BYTE protocal;//0x83
	BYTE msgType;
	int msgLength;
	int videoID;//视频服务器id
	int clientID;
	char channelNo;//通道号
	char reqType;//0:请求,1:断开
	char dataType;//0请求视频,1请求音频,2请求对讲
}KEMsgRequestDataReq,*PKEMsgRequestDataReq;

typedef struct _KEMsgRealTimeDataResp
{
	KEMsgHead head;//
	int videoID;
	char channelNo;
	int transIP;//转发服务器IP
	int videoIP;//视频服务器IP/录像服务器IP
	char iDecodeStyle;////0:正常解码 1:不处理丢帧丢片情况
	char data;//0请求视频,1请求音频,2请求对讲
	char online;// 0=都在线  1=视频服务器在线  2=转发服务器在线 3都不在线
	short port;
	short dummy;
}KEMsgRealTimeDataResp,*PKEMsgRealTimeDataResp;

typedef struct _KEMsgMediaTransReq
{
	KEMsgHead head;//0x8f
	int videoID;
	char channelNo;
	char video;//视频=0请求   =1 停止
	char listen;//监听=0请求   =1 停止
	char talk;// 对讲=0请求   =1 停止
	char devType;// 2：客户端   3：平台   5：录像服务器
}KEMsgMediaTransReq,*PKEMsgMediaTransReq;

typedef struct _KEMsgMediaTransResp
{
	KEMsgHead head;//0x8f
	int videoID;
	char channelNo;
	char video;//视频=0请求   =1 停止
	char listen;//监听=0请求   =1 停止
	char talk;//, , 对讲=0请求   =1 停止
	char respType;// 响应类型:0请求成功;1 失败;2视频服务器不在线;	3通道被禁用;4超出最大转发数;8每通道最多可发给8个客户端、一个录像和一个平台;6 最多可建立600个TCP连接	
}KEMsgMediaTransResp,*PKEMsgMediaTransResp;

typedef struct _KEMalfunctionAlert
{
	KEMsgHead head;//0x8a
	int serverID;
	char channelID;
	char alertType;//故障类型：1设备告警，2性能告警，3处理出错告警，4通讯告警，5环境告警
	char alertNo;// 故障编号：1 CPU使用能力百分比超过阈值报警，
						//2磁盘使用空间百分比超过阈值报警 
							//3内存使用百分比超过阈值报警，
						//4外接输入检测故障异常报警/温度过高报警,
						//5透明通道故障异常告警，
						//6编码器鉴权不成功报警，
						//7操作Flash失败报警,
						//8磁盘读写失败报警/硬盘故障报警，
						//9图像遮挡功能报警,10移动侦测故障报警，11磁盘空间满
	char status;
	char timeSpan[5];
}KEMalfunctionAlert,*PKEMalfunctionAlert;

typedef struct _KERTStreamHead
{
	BYTE protocal;
	BYTE msgType;//0x39 0x38
	int msgLength;
	int videoID;
	char channelNo;
}KERTStreamHead,*PKERTStreamHead;

typedef struct _KEPTZControlReq
{
	BYTE protocal;
	BYTE msgType;//0x85
	int msgLength;
	int videoID;
	int clientID;
	char channelNo;
	BYTE ctrlType;
	BYTE speed;
	BYTE data;
}KEPTZControlReq,*PKEPTZControlReq;

typedef struct _KEPTZControlResp
{
	BYTE protocal;
	BYTE msgType;//0x85
	int msgLength;
	int videoID;
	int clientID;
	char respType;	
}KEPTZControlResp,*PKEPTZControlResp;


enum KEMSG_ASKTREE_DATATYPE
{
	KEMSG_ASKTREE_DATATYPE_AllRootNodes = 32,
	KEMSG_ASKTREE_DATATYPE_ErrorOccured
};

//请求树形结构，数据头
typedef struct _KEAskTreeMsg
{
	BYTE protocal;
	BYTE msgType;//0x81
	int msgLength;
	int clientID;
	int dataSvr; //无用
	short dataType;//32-请求所有树形节点
	short packageNo;
	short packageTotal;

}KEAskTreeMsg,*PKEAskTreeMsg;



#pragma pack()





#endif 