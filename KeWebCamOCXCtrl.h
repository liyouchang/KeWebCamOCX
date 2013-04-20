#pragma once

// KeWebCamOCXCtrl.h : CKeWebCamOCXCtrl ActiveX 控件类的声明。

#include "WebCamPannel.h"

// CKeWebCamOCXCtrl : 有关实现的信息，请参阅 KeWebCamOCXCtrl.cpp。

class CKeWebCamOCXCtrl : public COleControl
{
	DECLARE_DYNCREATE(CKeWebCamOCXCtrl)
// 构造函数
public:
	CKeWebCamOCXCtrl();
	CWebCamPannel m_pannel;
// 重写
public:
	virtual void OnDraw(CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid);
	virtual void DoPropExchange(CPropExchange* pPX);
	virtual void OnResetState();

// 实现
protected:
	~CKeWebCamOCXCtrl();

	BEGIN_OLEFACTORY(CKeWebCamOCXCtrl)        // 类工厂和 guid
		virtual BOOL VerifyUserLicense();
		virtual BOOL GetLicenseKey(DWORD, BSTR FAR*);
	END_OLEFACTORY(CKeWebCamOCXCtrl)

	DECLARE_OLETYPELIB(CKeWebCamOCXCtrl)      // GetTypeInfo
	DECLARE_PROPPAGEIDS(CKeWebCamOCXCtrl)     // 属性页 ID
	DECLARE_OLECTLTYPE(CKeWebCamOCXCtrl)		// 类型名称和杂项状态

// 消息映射
	DECLARE_MESSAGE_MAP()

// 调度映射
	DECLARE_DISPATCH_MAP()

// 事件映射
	DECLARE_EVENT_MAP()

// 调度和事件 ID
public:
	enum {
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
	BSTR StartRealTimeVideo(LONG videoID, LONG channelNo);
	BSTR StopRealTimeVideo(LONG videoID, LONG channelNo);
public:
	afx_msg void OnDestroy();
public:
	void ShowCamPannel(BOOL bFull = FALSE);
	BOOL m_bFullScreen;
	
	HWND m_OldWndParent;
	CRect m_rcRect;
	WINDOWPLACEMENT m_OldWndPlacement;
	virtual void OnSetClientSite();
protected:
	LRESULT OnRTVideoStop(WPARAM wParam, LPARAM lParam);
	LRESULT OnHeartbeatStop(WPARAM wParam, LPARAM lParam);
	LRESULT OnCamStatusReport(WPARAM wParam, LPARAM lParam);
	void HeartBeatStop(LPCTSTR reason)
	{
		FireEvent(eventidHeartBeatStop, EVENT_PARAM(VTS_BSTR), reason);
	}
	BSTR ControlPTZ(LONG cameraID, BYTE PTZCmd, BYTE iSpeed, BYTE iData);
	BSTR LoginServer(LPCTSTR userName, LPCTSTR password, LPCTSTR svrIpAddr);
	void OnSnapFilePathChanged(void);
	CString m_SnapFilePath;
protected:
	void ReportCameraStatus(LPCTSTR info)
	{
		FireEvent(eventidReportCameraStatus, EVENT_PARAM(VTS_BSTR), info);
	}
	BSTR TakeSnapshot(LONG nCameraID);
	
};

