#pragma once
#include "afxwin.h"

// KeWebCamOCXPropPage.h : CKeWebCamOCXPropPage ����ҳ���������


// CKeWebCamOCXPropPage : �й�ʵ�ֵ���Ϣ������� KeWebCamOCXPropPage.cpp��

class CKeWebCamOCXPropPage : public COlePropertyPage
{
	DECLARE_DYNCREATE(CKeWebCamOCXPropPage)
	DECLARE_OLECREATE_EX(CKeWebCamOCXPropPage)

// ���캯��
public:
	CKeWebCamOCXPropPage();

// �Ի�������
	enum { IDD = IDD_PROPPAGE_KEWEBCAMOCX };

// ʵ��
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ��Ϣӳ��
protected:
	DECLARE_MESSAGE_MAP()
public:
	
	
};

