#ifndef ___COMMAND_H___
#define ___COMMAND_H___



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


#define MSG_WAIT_TIMEOUT 3000


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
	char keyt[8];
} MsgSecretKeyResp ,*PMsgSecretKeyResp;

struct KEMsgUserLoginReq
{
	KEMsgHead head;
	int clientID;
	int clientLevel;
	char userName[8];
	char keyt[8];
	char encreptData[16];
};

struct KEMsgUserLoginResp
{
	KEMsgHead head;
	int clientID;
	int clientLevel;
	char respData;
};
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