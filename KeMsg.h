#ifndef ___KEMSG_H___
#define ___KEMSG_H___



//********************protocol type*****************************
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


#define PROTOCOL_HEAD                0xFF									//Э��ͷ
#define MSG_WAIT_TIMEOUT 3000	//��Ӧ��Ϣ��ȡ�ȴ�ʱ��



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
	int videoID;//��Ƶ������id
	int clientID;
	char channelNo;//ͨ����
	char reqType;//0:����,1:�Ͽ�
	char dataType;//0������Ƶ,1������Ƶ,2����Խ�
}KEMsgRequestDataReq,*PKEMsgRequestDataReq;

typedef struct _KEMsgRealTimeDataResp
{
	KEMsgHead head;//
	int videoID;
	char channelNo;
	int transIP;//ת��������IP
	int videoIP;//��Ƶ������IP/¼�������IP
	char iDecodeStyle;////0:�������� 1:������֡��Ƭ���
	char data;//0������Ƶ,1������Ƶ,2����Խ�
	char online;// 0=������  1=��Ƶ����������  2=ת������������ 3��������
	short port;
	short dummy;
}KEMsgRealTimeDataResp,*PKEMsgRealTimeDataResp;

typedef struct _KEMsgMediaTransReq
{
	KEMsgHead head;//0x8f
	int videoID;
	char channelNo;
	char video;//��Ƶ=0����   =1 ֹͣ
	char listen;//����=0����   =1 ֹͣ
	char talk;// �Խ�=0����   =1 ֹͣ
	char devType;// 2���ͻ���   3��ƽ̨   5��¼�������
}KEMsgMediaTransReq,*PKEMsgMediaTransReq;

typedef struct _KEMsgMediaTransResp
{
	KEMsgHead head;//0x8f
	int videoID;
	char channelNo;
	char video;//��Ƶ=0����   =1 ֹͣ
	char listen;//����=0����   =1 ֹͣ
	char talk;//, , �Խ�=0����   =1 ֹͣ
	char respType;// ��Ӧ����:0����ɹ�;1 ʧ��;2��Ƶ������������;	3ͨ��������;4�������ת����;8ÿͨ�����ɷ���8���ͻ��ˡ�һ��¼���һ��ƽ̨;6 ���ɽ���600��TCP����	
}KEMsgMediaTransResp,*PKEMsgMediaTransResp;

typedef struct _KEMalfunctionAlert
{
	KEMsgHead head;//0x8a
	int serverID;
	char channelID;
	char alertType;//�������ͣ�1�豸�澯��2���ܸ澯��3�������澯��4ͨѶ�澯��5�����澯
	char alertNo;// ���ϱ�ţ�1 CPUʹ�������ٷֱȳ�����ֵ������
						//2����ʹ�ÿռ�ٷֱȳ�����ֵ���� 
							//3�ڴ�ʹ�ðٷֱȳ�����ֵ������
						//4�������������쳣����/�¶ȹ��߱���,
						//5͸��ͨ�������쳣�澯��
						//6��������Ȩ���ɹ�������
						//7����Flashʧ�ܱ���,
						//8���̶�дʧ�ܱ���/Ӳ�̹��ϱ�����
						//9ͼ���ڵ����ܱ���,10�ƶ������ϱ�����11���̿ռ���
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

//�������νṹ������ͷ
typedef struct _KEAskTreeMsg
{
	BYTE protocal;
	BYTE msgType;//0x81
	int msgLength;
	int clientID;
	int dataSvr; //����
	short dataType;//32-�����������νڵ�
	short packageNo;
	short packageTotal;

}KEAskTreeMsg,*PKEAskTreeMsg;



#pragma pack()





#endif 