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
#define REQUEST_TO_SERVER            0x8F									//向服务器请求数据
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
#define MSG_WAIT_TIMEOUT 10000	//相应消息读取等待时间


enum KEMSG_TYPE
{
	KEMSG_TYPE_ASKKEY = 0xD0,
	KEMSG_TYPE_LOGIN = 0x80,
};

enum KEMSG_EVENT
{
	KEMSG_EVENT_ASKKEY = 0,
	KEMSG_EVENT_LOGIN,
	KEMSG_EVENT_MAX
};

#define SECRETKEY_LEN 8 
#define USERNAME_LEN 8
#define PASSWORD_LEN 8
#define  ENCRYPTED_DATA_LEN 16

#pragma pack(1)

typedef struct _KEMsgHead
{
	BYTE protocal;
	BYTE type;
	int length;
}KEMsgHead,*PKEMsgHead;


typedef struct _KEMsgSecretKeyReq
{
	KEMsgHead head;
	int clientID;
} MsgSecretKeyReq,*PMsgSecretKeyReq;
typedef struct _KEMsgSecretKeyResp
{
	KEMsgHead head;
	int clientID;
	char keyt[SECRETKEY_LEN];
} MsgSecretKeyResp ,*PMsgSecretKeyResp;

typedef struct _KEMsgUserLoginReq
{
	KEMsgHead head;
	int clientID;
	int clientLevel;
	char userName[USERNAME_LEN];
	char keyt[SECRETKEY_LEN];
	char encreptData[ENCRYPTED_DATA_LEN];
}KEMsgUserLoginReq ,*PKEMsgUserLoginReq;


typedef struct _KEMsgUserLoginResp
{
	KEMsgHead head;
	int clientID;
	int clientLevel;
	char respData;
}KEMsgUserLoginResp,*PKEMsgUserLoginResp;


struct KEMsgXMLInfoReq
{
	KEMsgHead head;
	int clientID;
	int dataServerID;

};

struct KEMsgHeartbeatReq
{
	KEMsgHead head;
	int clientID;
	char status;
};
struct KEMsgHeartbeatResp
{
	KEMsgHead head;
	int clientID;
	char status;
};

#pragma pack()





#endif 