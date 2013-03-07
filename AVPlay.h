

#ifdef KAERPLAY_EXPORTS
#define AVPLAY_API __declspec(dllexport)
#else
#define AVPLAY_API __declspec(dllimport)
#endif

#define WM_FILEEND			(WM_USER + 112)		//�ļ����������Ϣ
//#define Play_Buffer_SIZE	0x200000
#define Play_Buffer_SIZE	0x300000

#pragma   pack(1) 
//�����豸�����ݽṹ����������
typedef struct tagFINDDEVICEDATA
{
	int iCount;
} FINDDEVICEDATA, *PFINDDEVICEDATA;

typedef struct tagDRAWFRAME
{
	int ileft;
	int itop;
	int ibottom;
	int iright;
}DRAWFRAM;

typedef struct tagDRAWFRAMEGROUP
{
	DRAWFRAM drawframe[4];
}DRAWFRAMGROUP;

#pragma

//13.��Ƶ���ݽ����ص�
typedef void (WINAPI *PFVideoData)(int iTerminalId, int iChlId, long hHandle, const char* pDataBuf, int iLen, BOOL bIdr, int iTimeStamp, LPVOID pContext);
//14.��Ƶ���ݽ����ص�
typedef void (WINAPI *PFAudioData)(int iTerminalId, int iChlId, long hHandle, const char* pDataBuf, int iLen, int iTimeStamp, LPVOID pContext);


extern "C" AVPLAY_API HRESULT AV_SetDllName(char *pDllName);

extern "C" AVPLAY_API HRESULT AV_Initial(HWND hWnd);

extern "C" AVPLAY_API HRESULT AV_OpenStream_Ex(long hHandle);

extern "C" AVPLAY_API HRESULT AV_CloseStream_Ex(long hHandle);

extern "C" AVPLAY_API HRESULT AV_OpenFile_Ex(long hHandle, char *sFileName,DWORD dwFileSize = 0x00);

extern "C" AVPLAY_API HRESULT AV_CloseFile_Ex(long hHandle);

extern "C" AVPLAY_API HRESULT AV_SetGPSCallback(long lFn,long lUser);


extern "C" AVPLAY_API HRESULT AV_GetPort();

extern "C" AVPLAY_API HRESULT AV_Play(long hHandle, HWND hWnd);

extern "C" AVPLAY_API HRESULT AV_OpenFile(long *phHandle, LPCTSTR sFileName , DWORD dwFileSize = 0x00 );

extern "C" AVPLAY_API HRESULT AV_OpenStream(long *phHandle,long *pFileHeadBuf,long lSize);

extern "C" AVPLAY_API HRESULT AV_SetStreamOpenMode(long hHandle,long lMode);

extern "C" AVPLAY_API HRESULT AV_GetStreamOpenMode(long hHandle);

extern "C" AVPLAY_API HRESULT AV_CloseStream(long hHandle ,bool bCallBack);

extern "C" AVPLAY_API HRESULT AV_CloseFile(long hHandle, bool bCallBack);

extern "C" AVPLAY_API HRESULT AV_RefreshSurface(long hHandle);

extern "C" AVPLAY_API HRESULT AV_RefreshByWnd(long lPlayWnd);

extern "C" AVPLAY_API HRESULT AV_Pause(long hHandle, long lPause);

extern "C" AVPLAY_API HRESULT AV_SetZoom(long hHandle, long lZoom);

extern "C" AVPLAY_API HRESULT AV_Fast(long hHandle);

extern "C" AVPLAY_API HRESULT AV_Slow(long hHandle);

extern "C" AVPLAY_API HRESULT AV_SetSpeed(long hHandle,long lSpeed);

extern "C" AVPLAY_API HRESULT AV_GetSpeed(long hHandle,long *lSpeed);

extern "C" AVPLAY_API HRESULT AV_SetPlayPos(long hHandle, long lPos);

extern "C" AVPLAY_API HRESULT AV_GetPlayPos(long hHandle, long * pPos);

extern "C" AVPLAY_API HRESULT AV_GetVersion(long * lVer);

extern "C" AVPLAY_API HRESULT AV_QueryFunction(LPCTSTR sFunctionName, long * lCanUse);

extern "C" AVPLAY_API HRESULT AV_SetFileEndMsgWnd(long hHandle, long hWnd, long lMsg);

extern "C" AVPLAY_API HRESULT AV_CapPic(long hHandle, LPCTSTR sFileName);

extern "C" AVPLAY_API HRESULT AV_SetVolume(long hHandle, long lVolume);

extern "C" AVPLAY_API HRESULT AV_InitAudioTalk();

extern "C" AVPLAY_API HRESULT AV_SoundPlay(long hHandle);

extern "C" AVPLAY_API HRESULT AV_StopSound(long hHandle);

extern "C" AVPLAY_API HRESULT AV_GetAudioData(char* pBuf);

extern "C" AVPLAY_API HRESULT AV_GetBufferFrameCount(long hHandle);

extern "C" AVPLAY_API HRESULT AV_InputData(long hHandle, long* pBuf, long lSize);

extern "C" AVPLAY_API HRESULT AV_TalkOpen(long lEncFormat = 0x50);

extern "C" AVPLAY_API HRESULT AV_TalkClose();

extern "C" AVPLAY_API HRESULT AV_Free();

extern "C" AVPLAY_API DWORD AV_GetFileTotalFrames(LONG lHandle);

extern "C" AVPLAY_API DWORD AV_GetPlayedFrames(LONG lHandle);

extern "C" AVPLAY_API DWORD AV_GetFileTotalTimes(LONG lHandle);

extern "C" AVPLAY_API HRESULT AV_SetPlayTimes(LONG lHandle,LONG lTimes);

extern "C" AVPLAY_API DWORD AV_GetPlayedTimes(LONG lHandle);

extern "C" AVPLAY_API HRESULT AV_OneByOneBack(LONG lHandle);

extern "C" AVPLAY_API	HRESULT AV_OneByOne(LONG lHandle);

extern "C" AVPLAY_API HRESULT AV_RigisterDrawFun(LONG hHandle, long lfn, long pUser);

extern "C" AVPLAY_API HRESULT AV_GetVolume(long hHandle, long* lVolume);

extern "C" AVPLAY_API HRESULT AV_ScreenClear(long hHandle);

extern "C" AVPLAY_API HRESULT AV_BackPlayNormal(long hHandle);

extern "C" AVPLAY_API HRESULT AV_GetPlayFrameRate(long hHandle);

extern "C" AVPLAY_API double AV_GetPlayFluxRate(long hHandle);

extern "C" AVPLAY_API HRESULT AV_ReInitViewHandle(long hHandle, HWND hWnd);

extern "C" AVPLAY_API HRESULT AV_SetFileEndMessage(DWORD hThreadID, long lMessageID);

extern "C" AVPLAY_API HRESULT AV_SetCallbackAfterDecode(long hHandle, int iTerminalId, int iChlId, PFVideoData pfnVideoAfterDec, PFAudioData pfnAudioAfterDec, LPVOID Context);

/*
  ���˿ͻ���ר��
*/
extern "C" AVPLAY_API HRESULT AV_SetCallbackAfterDecode1(long hHandle, long lType, long lPara1, long lPara2);

extern "C" AVPLAY_API HRESULT AV_GetCallbackAfterDecode1(long hHandle, long lType, long *pPara1, long *pPara2);

extern "C" AVPLAY_API HRESULT AV_SetCallbackForTalk(long hHandle, long callbackptr);


/*
  ��Ϊ�ͻ���ר��
*/
typedef struct
{
	long lWidth; //�������λ���ء��������Ƶ������Ϊ0��
	long lHeight; //����ߡ��������Ƶ������Ϊ0��
	long lStamp; //ʱ����Ϣ����λ���롣
	long lType; //�������ͣ�T_AUDIO16��T_RGB32�� T_YV12������궨��˵����
	long lFrameRate;//����ʱ������ͼ��֡�ʡ�
}FRAME_INFO;

//#define T_AUDIO16	101
//#define T_AUDIO8	100
//#define T_UYVY		1
//#define T_YV12		3
//#define T_RGB32		7

typedef void (__stdcall *DrawCallBack2)(long, const char*, long, FRAME_INFO*, void*, long);
/*����˵����
   lHandle:		    ���ûص��Ĳ��ž��
   pBuf:			����������Ƶ����
   lSize:			����������Ƶ����pBuf�ĳ���
   pFrameInfo:		ͼ���������Ϣ�������
   pUserData:		�û����ò���
   lReserved:		��������
����������
���ûص��������滻�������е���ʾ���֣����û��Լ�������ʾ���ú�����AV_OpenStream��AV_OpenFile֮ǰ���ã���Stopʱ�Զ�ʧЧ���´ε���Start֮ǰ��Ҫ��������
*/
extern "C" AVPLAY_API HRESULT AV_SetDecodeCallBack2(long hHandle, DrawCallBack2 Pv, void* pContext);

/*
	����������
	���ý���ص��������ͣ���SetDecCallBack��������Ч
	����˵����
		lHandle:		���ž��
		lStream:		1��Ƶ����2��Ƶ����3������

*/
extern "C" AVPLAY_API HRESULT AV_SetDecCBStream(LONG lHandle, ULONG lStream);
/*
  ��Ϊ�ͻ���ר��
*/

extern "C" AVPLAY_API HRESULT AV_GetEncodeFormat(long hHandle);

extern "C" AVPLAY_API HRESULT AV_SetDecodeStyle(long hHandle, int iDecodeStyle);

extern "C" AVPLAY_API HRESULT AV_AudioEncode(char *pInBuf, int iInlen, char *pOutBuf, int *pOutlen, int iType);



/**
  ������ɫ������

  @param hHandle �����������
  @param nBrightness ���ȣ�Ĭ��128����Χ0-255��
  @param nContrast �Աȶȣ�Ĭ��128����Χ0-255��
  @param nSaturation ���Ͷȣ�Ĭ��128����Χ0-255��
  @param nHue ɫ�ȣ�Ĭ��128����Χ0-255��
  @return ִ�гɹ�����0������ʱ���ش����롣
*/
extern "C" AVPLAY_API HRESULT AV_SetColor(long hHandle, long nBrightness, long nContrast, long nSaturation, long nHue);

/**
  ��ȡ��ǰ���õ���ɫ������

  @param hHandle �����������
  @param pBrightness �������������ȣ�Ĭ��128����Χ0-255��
  @param pContrast �����������Աȶȣ�Ĭ��128����Χ0-255��
  @param pSaturation �������������Ͷȣ�Ĭ��128����Χ0-255��
  @param pHue ����������ɫ�ȣ�Ĭ��128����Χ0-255��
  @return ִ�гɹ�����0������ʱ���ش����롣
*/
extern "C" AVPLAY_API HRESULT AV_GetColor(long hHandle, long* pBrightness, long* pContrast, long* pSaturation, long* pHue);

/**
  ������Ƶͼ��ת���ԣ�����������ת�Ƕ�Ϊ180�ȣ���

  @param hHandle �����������
  @param lReverseImage �Ƿ����÷�ת���ԣ�Ĭ�ϲ����ã�0 - �����ã�1 - ���á�
*/
extern "C" AVPLAY_API HRESULT AV_SetReverseImage(long hHandle, long lReverseImage);

extern "C" AVPLAY_API HRESULT AV_SetDrawframeArea(long hHandle, DRAWFRAMGROUP &DrawframeGroup);

extern "C" AVPLAY_API HRESULT AV_GetDrawframeArea(long hHandle, DRAWFRAMGROUP &PDrawframeGroup);

extern "C" AVPLAY_API HRESULT AV_SetWireFrames(long lFrames);

extern "C" AVPLAY_API HRESULT AV_SetWirelessFrames(long lFrames);

extern "C" AVPLAY_API HRESULT AV_GetWireFrames();

extern "C" AVPLAY_API HRESULT AV_GetWirelessFrames();

extern "C" AVPLAY_API HRESULT AV_GetLeftDataSize(long hHandle);

extern "C" AVPLAY_API HRESULT AV_ResetSourceBuffer(long hHandle);

extern "C" AVPLAY_API HRESULT AV_SetBackPlaySel(long hHandle, long lSel);

extern "C" AVPLAY_API HRESULT AV_GetBackPlaySel(long hHandle, long *lSel);

extern "C" AVPLAY_API HRESULT AV_SetRealPlaySel(long hHandle, long lSel);

extern "C" AVPLAY_API HRESULT AV_GetRealPlaySel(long hHandle, long *lSel);

extern "C" AVPLAY_API HRESULT AV_SetGDI(long lVlaue);

extern "C" AVPLAY_API HRESULT AV_GetGDI();

extern "C" AVPLAY_API HRESULT AV_SetLostFrame(long lVlaue);

extern "C" AVPLAY_API HRESULT AV_GetLostFrame();

extern "C" AVPLAY_API HRESULT AV_SetVideoSort(long lVlaue);

extern "C" AVPLAY_API HRESULT AV_GetVideoSort();

extern "C" AVPLAY_API HRESULT AV_SetEncoderFormatget(long lVlaue);

extern "C" AVPLAY_API HRESULT AV_GetEncoderFormatget();
 
extern "C" AVPLAY_API HRESULT AV_SetHigh(long lVlaue);

extern "C" AVPLAY_API HRESULT AV_SetAudioCallBack(long lCallback);

extern "C" AVPLAY_API HRESULT AV_GetHigh();

extern "C" AVPLAY_API HRESULT AV_SetAudioBroadcast(long lValue);

extern "C" AVPLAY_API HRESULT AV_InputPCMData(char* pPCMData, long nBufSize);

extern "C" AVPLAY_API HRESULT AV_SetPCMEncodeCallBack(long fPCMEncode, long lUser);

extern "C" AVPLAY_API HRESULT AV_SetFileSize(long hHandle, long lFileSize);

extern "C" AVPLAY_API HRESULT AV_SetPlayVideoInfo(long hHandle, long lVid, long lChlid);

extern "C" AVPLAY_API HRESULT AV_PlayWindowSelect(long hHandle, long lSelect);

extern "C" AVPLAY_API HRESULT AV_SetPowerView(long hHandle, long lPower, long lOpen);

