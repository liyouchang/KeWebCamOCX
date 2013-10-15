#pragma once

// KeWebCamOCXCtrl.h : CKeWebCamOCXCtrl ActiveX �ؼ����������

#include "WebCamPannel.h"

// CKeWebCamOCXCtrl : �й�ʵ�ֵ���Ϣ������� KeWebCamOCXCtrl.cpp��

class CKeWebCamOCXCtrl : public COleControl
{
	DECLARE_DYNCREATE(CKeWebCamOCXCtrl)
// ���캯��
public:
	CKeWebCamOCXCtrl();
	CWebCamPannel m_pannel;
	//CCmdSocket m_socketSvr;
	tstd::tstring iniFile;
protected:
	UINT_PTR m_HeartBeatTimer;
// ��д
public:
	virtual void OnDraw(CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid);
	virtual void DoPropExchange(CPropExchange* pPX);
	virtual void OnResetState();

// ʵ��
protected:
	~CKeWebCamOCXCtrl();

	BEGIN_OLEFACTORY(CKeWebCamOCXCtrl)        // �๤���� guid
		virtual BOOL VerifyUserLicense();
		virtual BOOL GetLicenseKey(DWORD, BSTR FAR*);
	END_OLEFACTORY(CKeWebCamOCXCtrl)

	DECLARE_OLETYPELIB(CKeWebCamOCXCtrl)      // GetTypeInfo
	DECLARE_PROPPAGEIDS(CKeWebCamOCXCtrl)     // ����ҳ ID
	DECLARE_OLECTLTYPE(CKeWebCamOCXCtrl)		// �������ƺ�����״̬

// ��Ϣӳ��
	DECLARE_MESSAGE_MAP()

// ����ӳ��
	DECLARE_DISPATCH_MAP()

// �¼�ӳ��
	DECLARE_EVENT_MAP()

// ���Ⱥ��¼� ID
public:

	enum {
		dispidGetLocalPath = 27L,
		dispidQueryRecordFileList2 = 26L,
		dispidGetLocalMac = 25L,
		dispidCheckVersion = 24L,
		dispidSetDevWifiAP = 23L,
		dispidGetDevWifiAP = 22L,
		dispidPlayRemoteRecord = 21L,
		dispidQueryRecordFileList = 20L,
		dispidConnectServer = 19L,
		dispidInitailCtrl = 18L,
		dispidStopRecord = 17L,
		dispidStartRecord = 16L,
		dispidRecordFilePath = 15,
		dispidStopAudioTalk = 14L,
		dispidStartAudioTalk = 13L,
		dispidStopRealTimeAudio = 12L,
		dispidStartRealTimeAudio = 11L,
		eventidTreeStructNotify = 4L,
		dispidTakeSnapshot = 10L,
		eventidReportCameraStatus = 3L,
		dispidSnapFilePath = 9,
		dispidLoginServer = 8L,
		dispidControlPTZ = 7L,
		eventidHeartBeatStop = 2L,
		dispidStopRealTimeVideo = 6L,
		dispidStartRealTimeVideo = 5L,
		dispidQueryUserCamera = 3L,
		dispidSetDivision = 1L
	};

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	
protected:
	void SetDivision(LONG nDivision);
	BSTR QueryUserCamera(void);
	BSTR StartRealTimeVideo(LONG nCameraID);
	BSTR StopRealTimeVideo(LONG nCameraID);
	
public:
	afx_msg void OnDestroy();
	virtual void OnSetClientSite();

protected:
	LRESULT OnRTVideoStop(WPARAM wParam, LPARAM lParam);
	LRESULT OnHeartbeatStop(WPARAM wParam, LPARAM lParam);
	LRESULT OnCamStatusReport(WPARAM wParam, LPARAM lParam);
	LRESULT OnTreeStructNotify(WPARAM wParam, LPARAM lParam);
	
	void HeartBeatStop(LPCTSTR reason)
	{
		FireEvent(eventidHeartBeatStop, EVENT_PARAM(VTS_BSTR), reason);
	}
	BSTR ControlPTZ(LONG cameraID, BYTE PTZCmd, BYTE iSpeed, BYTE iData);
	BSTR LoginServer(LPCTSTR userName, LPCTSTR password);
	void OnSnapFilePathChanged(void);
	CString m_SnapFilePath;

protected:
	void ReportCameraStatus(LPCTSTR info)
	{
		FireEvent(eventidReportCameraStatus, EVENT_PARAM(VTS_BSTR), info);
	}
	BSTR TakeSnapshot(LONG nCameraID);
	void TreeStructNotify(LPCTSTR jsonInfo)
	{
		FireEvent(eventidTreeStructNotify, EVENT_PARAM(VTS_BSTR), jsonInfo);
	}
	BSTR StartRealTimeAudio(LONG cameraID);
	BSTR StopRealTimeAudio(LONG cameraID);
	BSTR StartAudioTalk(LONG cameraID);
	BSTR StopAudioTalk(LONG cameraID);
	void OnRecordFilePathChanged(void);
	CString m_RecordFilePath;
	BSTR StartRecord(LONG cameraID);
	BSTR StopRecord(LONG cameraID);
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
protected:
	BSTR InitailCtrl(LONG platform);
	BSTR ConnectServer(LPCTSTR svrAddr, LONG svrPort, LONG clientID);
	BSTR QueryRecordFileList(LONG cameraID, LONG startTime, LONG endTime, LONG fileType);

	BSTR PlayRemoteRecord(LONG cameraID, LONG fileNo);
	BSTR GetDevWifiAP(LONG cameraID);
	BSTR SetDevWifiAP(LONG cameraID, LPCTSTR jsonParam);
	BSTR CheckVersion(void);
	BSTR GetLocalMac(void);
	BSTR QueryRecordFileList2(LONG cameraID, LPCTSTR jsonParam);

	BSTR GetLocalPath(void);

	BSTR SetVideoParam(LONG cameraID, LPCTSTR jsonParam);

	enum 
	{
		dispidGetVideoParam = 29L,
		dispidSetVideoParam = 28L
	};
	BSTR GetVideoParam(LONG cameraID);
};

