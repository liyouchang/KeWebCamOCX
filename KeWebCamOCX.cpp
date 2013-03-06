// KeWebCamOCX.cpp : CKeWebCamOCXApp 和 DLL 注册的实现。

#include "stdafx.h"
#include "KeWebCamOCX.h"

#include "comcat.h"
#include "strsafe.h"
#include "objsafe.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CKeWebCamOCXApp theApp;

const GUID CDECL BASED_CODE _tlid =
		{ 0xFF2B2757, 0xE552, 0x4DA1, { 0x9E, 0x3C, 0x93, 0xBC, 0x40, 0xEF, 0x7E, 0xC3 } };
const WORD _wVerMajor = 1;
const WORD _wVerMinor = 0;

// CLSID_SafeItem - Necessary for safe ActiveX control
// Id taken from IMPLEMENT_OLECREATE_EX function in xxxCtrl.cpp

const CATID CLSID_SafeItem =
{ 0xf4418f4b, 0x4a6b, 0x4a93, {0x94, 0x8f, 0x33, 0x20, 0x25, 0xf3, 0x95, 0xe8}};

// HRESULT CreateComponentCategory - Used to register ActiveX control as safe

HRESULT CreateComponentCategory(CATID catid, WCHAR *catDescription)
{
	ICatRegister *pcr = NULL ;
	HRESULT hr = S_OK ;

	hr = CoCreateInstance(CLSID_StdComponentCategoriesMgr, 
		NULL, CLSCTX_INPROC_SERVER, IID_ICatRegister, (void**)&pcr);
	if (FAILED(hr))
		return hr;

	// Make sure the HKCR\Component Categories\{..catid...}
	// key is registered.
	CATEGORYINFO catinfo;
	catinfo.catid = catid;
	catinfo.lcid = 0x0409 ; // english
	size_t len;
	// Make sure the provided description is not too long.
	// Only copy the first 127 characters if it is.
	// The second parameter of StringCchLength is the maximum
	// number of characters that may be read into catDescription.
	// There must be room for a NULL-terminator. The third parameter
	// contains the number of characters excluding the NULL-terminator.
	hr = StringCchLength(catDescription, STRSAFE_MAX_CCH, &len);
	if (SUCCEEDED(hr))
	{
		if (len>127)
		{
			len = 127;
		}
	}   
	else
	{
		// TODO: Write an error handler;
	}
	// The second parameter of StringCchCopy is 128 because you need 
	// room for a NULL-terminator.
	hr = StringCchCopy(catinfo.szDescription, len + 1, catDescription);
	// Make sure the description is null terminated.
	catinfo.szDescription[len + 1] = '\0';

	hr = pcr->RegisterCategories(1, &catinfo);
	pcr->Release();

	return hr;
}

// HRESULT RegisterCLSIDInCategory -
//      Register your component categories information

HRESULT RegisterCLSIDInCategory(REFCLSID clsid, CATID catid)
{
	// Register your component categories information.
	ICatRegister *pcr = NULL ;
	HRESULT hr = S_OK ;
	hr = CoCreateInstance(CLSID_StdComponentCategoriesMgr, 
		NULL, CLSCTX_INPROC_SERVER, IID_ICatRegister, (void**)&pcr);
	if (SUCCEEDED(hr))
	{
		// Register this category as being "implemented" by the class.
		CATID rgcatid[1] ;
		rgcatid[0] = catid;
		hr = pcr->RegisterClassImplCategories(clsid, 1, rgcatid);
	}

	if (pcr != NULL)
		pcr->Release();

	return hr;
}

// HRESULT UnRegisterCLSIDInCategory - Remove entries from the registry

HRESULT UnRegisterCLSIDInCategory(REFCLSID clsid, CATID catid)
{
	ICatRegister *pcr = NULL ;
	HRESULT hr = S_OK ;

	hr = CoCreateInstance(CLSID_StdComponentCategoriesMgr, 
		NULL, CLSCTX_INPROC_SERVER, IID_ICatRegister, (void**)&pcr);
	if (SUCCEEDED(hr))
	{
		// Unregister this category as being "implemented" by the class.
		CATID rgcatid[1] ;
		rgcatid[0] = catid;
		hr = pcr->UnRegisterClassImplCategories(clsid, 1, rgcatid);
	}

	if (pcr != NULL)
		pcr->Release();

	return hr;
}





// CKeWebCamOCXApp::InitInstance - DLL 初始化


BOOL CKeWebCamOCXApp::InitInstance()
{
	BOOL bInit = COleControlModule::InitInstance();

	if (bInit)
	{
		// TODO: 在此添加您自己的模块初始化代码。
		m_nMAXCHANNEL	= CAM_MAX;
		m_bConnectOK	= FALSE;
		m_nCurrentMappingCamera = 0;

	}

	return bInit;
}



// CKeWebCamOCXApp::ExitInstance - DLL 终止

int CKeWebCamOCXApp::ExitInstance()
{
	// TODO: 在此添加您自己的模块终止代码。

	return COleControlModule::ExitInstance();
}



// DllRegisterServer - 将项添加到系统注册表

STDAPI DllRegisterServer(void)
{
	AFX_MANAGE_STATE(_afxModuleAddrThis);

	if (!AfxOleRegisterTypeLib(AfxGetInstanceHandle(), _tlid))
		return ResultFromScode(SELFREG_E_TYPELIB);

	if (!COleObjectFactoryEx::UpdateRegistryAll(TRUE))
		return ResultFromScode(SELFREG_E_CLASS);

	return NOERROR;
}



// DllUnregisterServer - 将项从系统注册表中移除

STDAPI DllUnregisterServer(void)
{
	AFX_MANAGE_STATE(_afxModuleAddrThis);

	if (!AfxOleUnregisterTypeLib(_tlid, _wVerMajor, _wVerMinor))
		return ResultFromScode(SELFREG_E_TYPELIB);

	if (!COleObjectFactoryEx::UpdateRegistryAll(FALSE))
		return ResultFromScode(SELFREG_E_CLASS);

	return NOERROR;
}
