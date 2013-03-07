#ifndef ___COMMAND_H___
#define ___COMMAND_H___

/* major type definition */
#define	dvr_cmd_msg			0x01
#define	dvr_pwd_msg			0x02
#define	download_msg		0x03
#define	sock_close_msg		0x09


/* dvr msg minor type definition */

#define	dvrsub_cmd_msg			0x01
#define	dvrsub_pwd_msg			0x02
#define	dvrsub_recordlist_msg	0x03
#define	dvrsub_eventlist_msg	0x04
#define dvrsub_playbacklist_msg	0x05
#define dvrsub_timesearch_msg	0x06

/* download msg minor type definition */
#define downloadsub_msg		0x01

/* Full */
#define full_screen_request	0x80
#define	full_screen_on		0x00
#define	full_screen_off		0x01

/* dvrsub_cmd_msg command */
#define status_request		0x01
#define	status_reply		0x02
#define	status_ack			0x03
#define	status_nack			0x04

#define	video_play_request	0x05
#define	video_play_reply	0x06
#define	video_play_ack		0x07
#define	video_play_nack		0x08

#define	video_stop_request	0x09
#define	video_stop_reply	0x0a
#define	video_stop_ack		0x0b
#define	video_stop_nack		0x0c

#define	live_request		0x0d
#define	live_reply			0x0e
#define	live_ack			0x0f
#define	live_nack			0x10

#define	record_start_request	0x11
#define	record_start_reply		0x12
#define	record_start_ack		0x13
#define	record_star_nack		0x14

#define	record_stop_request		0x15
#define	record_stop_reply		0x16
#define	record_stop_ack			0x17
#define	record_stop_nack		0x18

/* if application close then, this message send*/
#define	socket_close_request	0x19
#define	socket_close_reply		0x1a
#define	socket_close_ack		0x1b
#define	socket_close_nack		0x1c

#define playback_request		0x1d
#define playback_reply			0x1e
#define playback_ack			0x1f
#define playback_nack			0x20

/*	dvrsub_pwd_msg commnd	*/
#define	connect_pass_request	0x01
#define connect_pass_reply		0x02
#define connect_pass_ack		0x03
#define connect_pass_nack		0x04

#define	change_pass_request		0x05
#define change_pass_reply		0x06
#define change_pass_ack			0x07
#define	change_pass_nack		0x08

/*	dvrsub_recordlist_msg	*/
#define	record_list_request		0x01
#define	record_list_reply		0x02
#define	record_list_ack			0x03
#define	record_list_nack		0x04

#define record_list_index_request	0x05
#define record_list_index_reply		0x06
#define record_list_index_ack		0x07
#define record_list_index_nack		0x08

/*	dvrsub_eventlist_msg	*/
#define	event_list_request		0x01
#define	event_list_reply		0x02
#define	event_list_ack			0x03
#define	event_list_nack			0x04

#define	event_list_index_request	0x05
#define	event_list_index_reply		0x06
#define	event_list_index_ack		0x07
#define	event_list_index_nack		0x08

/* dvrsub_playbacklist_msg */
#define list_pb_request			0x01
#define list_pb_reply			0x02
#define list_pb_ack				0x03
#define list_pb_nack			0x04

#define time_search_reqeuset    0x01
#define time_search_reply	    0x02
#define time_search_ack			0x03
#define time_search_nack	    0x04

#define firmware_download_request	0x01
#define firmware_download_replay	0x02
#define firmware_download_ack		0x03
#define	firmware_download_nack		0x04

/* event name of event list  */
#define power_on				0x00
#define motion					0x01
#define alarm					0x02
#define schedule				0x03
#define network					0x04



//********************protocol type*****************************
#define PROTOCOL_HEAD                0xFF									//协议头
#define ASK_KEY                      0xD0									//申请密钥
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

#pragma pack(1)

struct MsgHead
{
	BYTE protocal;
	BYTE type;
	int length;
};
struct MsgSecretKeyReq
{
	MsgHead head;
	int clientID;
} ;
struct MsgSecretKeyResp
{
	MsgHead head;
	int clientID;
	char keyt[8];
} ;

struct MsgUserLoginReq
{
	MsgHead head;
	int clientID;
	int clientLevel;
	char userName[8];
	char keyt[8];
	char encreptData[16];
};

struct MsgUserLoginResp
{
	MsgHead head;
	int clientID;
	int clientLevel;
	char respData;
};
struct MsgXMLInfoReq
{
	MsgHead head;
	int clientID;
	int dataServerID;

};

struct MsgHeartbeatReq
{
	MsgHead head;
	int clientID;
	char status;
};
struct MsgHeartbeatResp
{
	MsgHead head;
	int clientID;
	char status;
};


#pragma pack()





#endif 