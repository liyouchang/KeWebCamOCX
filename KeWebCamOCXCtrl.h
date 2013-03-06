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
		dispidSetDivision = 1L
	};
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
protected:
	void SetDivision(LONG nDivision);
};

