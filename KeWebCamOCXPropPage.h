#pragma once
#include "afxwin.h"

// KeWebCamOCXPropPage.h : CKeWebCamOCXPropPage 属性页类的声明。


// CKeWebCamOCXPropPage : 有关实现的信息，请参阅 KeWebCamOCXPropPage.cpp。

class CKeWebCamOCXPropPage : public COlePropertyPage
{
	DECLARE_DYNCREATE(CKeWebCamOCXPropPage)
	DECLARE_OLECREATE_EX(CKeWebCamOCXPropPage)

// 构造函数
public:
	CKeWebCamOCXPropPage();

// 对话框数据
	enum { IDD = IDD_PROPPAGE_KEWEBCAMOCX };

// 实现
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 消息映射
protected:
	DECLARE_MESSAGE_MAP()
public:
	
	
};

