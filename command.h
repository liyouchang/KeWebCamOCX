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
#define PROTOCOL_HEAD                0xFF									//Э��ͷ
#define ASK_KEY                      0xD0									//������Կ
#define CHANGE_CENTERIP              0xD1                                   //��������IP
#define MODIFY_PASSWORD              0x8E									//�޸�����
#define LOGIN_JRSERVER               0x80									//��¼������
#define REQUEST_TREESTRUCT           0x81									//�������νṹ
#define HEART_TOJRSERVER             0x82									//����
#define REQUEST_DATA                 0x83									//������Ƶ/��Ƶ/�Խ�����
#define SET_VDOPARAM                 0x84									//������Ƶ���Ȳ���
#define GET_VDOPARAM                 0x86									//��ȡ��Ƶ����
#define SET_YT                       0x85									//��̨���� 
#define SENDDATA_TO_CLIENT           0x89									//������ͻ��˷��ͱ���ͼ������
#define REQUEST_TO_SERVER            0x8F									//���������������
#define REQUEST_FILE_LIST            0x53									//�����ļ��б�
#define REQUEST_DOWNLOAD_FILE        0x54									//���������ļ�
#define SEARCH_VIDEOSTATUS           0x8b									//��Ƶͨ��״̬��ѯ
#define DELETE_NODE                  0x8c									//ɾ���ڵ�
#define REQUEST_HISTORY_DATA         0x8d									//������ʷ��
#define REQUEST_TO_VDOSERVER         0x0c									//����Ƶ��������������
#define SEND_TALKDATA_TOMEDIASERVER  0x38
#define SEND_ALERTSET                0x20					                //�������Ĳ�����
#define SETCONTROLALERTTIME          0x23									//���ò�����ʱ��
#define SETCONTROLALERTTIME1         0x6C									//���ò�����ʱ��
#define GETCONTROLALERTTIME1         0x6D									//��ȡ������ʱ��
#define SETALERTPARAM                0x40									//�������뱨������
#define GETALERTPARAM                0x65									//��ȡ���뱨������
#define GETINALERTFROMDATABASE       0x27									//��ȡ���뱨����Ϣ�����ķ�����
#define CONTROLGPS                   0x18									//����GPS����
#define CONNECTSECONDCLIENT			 0x89									//������ͻ���ͨ��
#define SERIVALPARAM                 0xC3                                   //����ȡ���ڲ���
#define GET_SERIVALPARAM             0x61                                   //��ȡ���ڲ�����Э��
#define SET_SERIVALPARAM             0x3D                                   //���ô��ڲ�����Э��
#define GET_SERIVALDATA              0x2C                                   //��ȡ��������
#define SET_DECODERRUN               0xFB                                   //���ý������Ƿ�����Ƶ
#define GET_DECODERPT_TURNARAM       0xFC                                   //��ȡ����������Ƶ����
#define SET_DECODERPT_TURON          0xFD                                   //�������Ƿ�ʼ��
#define SET_CAMERA_PARAM             0xD4                                   //���þ�ͷ����
#define GET_CAMERA_PARAM             0xD3                                  //��ȡ��ͷ����
#define SET_DEVICE_PARAM             0x9E                                   //�����ն��豸����
#define GET_DEVICE_PARAM             0x9F                                   //��ȡ�ն��豸����
#define SET_NET_PARAM                0x47                                   //�����豸�������
#define GET_NET_PARAM                0x69                                   //��ȡ�豸�������
#define SET_VIDEO_TITLE              0x42                                   //������Ƶ������ͨ���ı���
#define GET_OUTPUT_ALARM             0x66                                   //��ȡ�������
#define SET_OUTPUT_ALARM             0x41                                   //�����������

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