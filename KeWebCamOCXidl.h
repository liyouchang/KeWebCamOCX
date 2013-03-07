

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 7.00.0500 */
/* at Thu Mar 07 16:21:04 2013
 */
/* Compiler settings for .\KeWebCamOCX.idl:
    Oicf, W1, Zp8, env=Win32 (32b run)
    protocol : dce , ms_ext, c_ext, robust
    error checks: stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
//@@MIDL_FILE_HEADING(  )

#pragma warning( disable: 4049 )  /* more than 64k source lines */


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 475
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__


#ifndef __KeWebCamOCXidl_h__
#define __KeWebCamOCXidl_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef ___DKeWebCamOCX_FWD_DEFINED__
#define ___DKeWebCamOCX_FWD_DEFINED__
typedef interface _DKeWebCamOCX _DKeWebCamOCX;
#endif 	/* ___DKeWebCamOCX_FWD_DEFINED__ */


#ifndef ___DKeWebCamOCXEvents_FWD_DEFINED__
#define ___DKeWebCamOCXEvents_FWD_DEFINED__
typedef interface _DKeWebCamOCXEvents _DKeWebCamOCXEvents;
#endif 	/* ___DKeWebCamOCXEvents_FWD_DEFINED__ */


#ifndef __KeWebCamOCX_FWD_DEFINED__
#define __KeWebCamOCX_FWD_DEFINED__

#ifdef __cplusplus
typedef class KeWebCamOCX KeWebCamOCX;
#else
typedef struct KeWebCamOCX KeWebCamOCX;
#endif /* __cplusplus */

#endif 	/* __KeWebCamOCX_FWD_DEFINED__ */


#ifdef __cplusplus
extern "C"{
#endif 



#ifndef __KeWebCamOCXLib_LIBRARY_DEFINED__
#define __KeWebCamOCXLib_LIBRARY_DEFINED__

/* library KeWebCamOCXLib */
/* [control][helpstring][helpfile][version][uuid] */ 


EXTERN_C const IID LIBID_KeWebCamOCXLib;

#ifndef ___DKeWebCamOCX_DISPINTERFACE_DEFINED__
#define ___DKeWebCamOCX_DISPINTERFACE_DEFINED__

/* dispinterface _DKeWebCamOCX */
/* [helpstring][uuid] */ 


EXTERN_C const IID DIID__DKeWebCamOCX;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("DDC0C3D6-E09B-493D-8C6C-12A9916C60E1")
    _DKeWebCamOCX : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _DKeWebCamOCXVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            _DKeWebCamOCX * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            _DKeWebCamOCX * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            _DKeWebCamOCX * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            _DKeWebCamOCX * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            _DKeWebCamOCX * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            _DKeWebCamOCX * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            _DKeWebCamOCX * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } _DKeWebCamOCXVtbl;

    interface _DKeWebCamOCX
    {
        CONST_VTBL struct _DKeWebCamOCXVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _DKeWebCamOCX_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define _DKeWebCamOCX_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define _DKeWebCamOCX_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define _DKeWebCamOCX_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define _DKeWebCamOCX_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define _DKeWebCamOCX_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define _DKeWebCamOCX_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___DKeWebCamOCX_DISPINTERFACE_DEFINED__ */


#ifndef ___DKeWebCamOCXEvents_DISPINTERFACE_DEFINED__
#define ___DKeWebCamOCXEvents_DISPINTERFACE_DEFINED__

/* dispinterface _DKeWebCamOCXEvents */
/* [helpstring][uuid] */ 


EXTERN_C const IID DIID__DKeWebCamOCXEvents;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("F4C17E60-A7C0-44FE-98B3-D7DEACE50252")
    _DKeWebCamOCXEvents : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct _DKeWebCamOCXEventsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            _DKeWebCamOCXEvents * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            _DKeWebCamOCXEvents * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            _DKeWebCamOCXEvents * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            _DKeWebCamOCXEvents * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            _DKeWebCamOCXEvents * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            _DKeWebCamOCXEvents * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            _DKeWebCamOCXEvents * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } _DKeWebCamOCXEventsVtbl;

    interface _DKeWebCamOCXEvents
    {
        CONST_VTBL struct _DKeWebCamOCXEventsVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define _DKeWebCamOCXEvents_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define _DKeWebCamOCXEvents_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define _DKeWebCamOCXEvents_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define _DKeWebCamOCXEvents_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define _DKeWebCamOCXEvents_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define _DKeWebCamOCXEvents_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define _DKeWebCamOCXEvents_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* ___DKeWebCamOCXEvents_DISPINTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_KeWebCamOCX;

#ifdef __cplusplus

class DECLSPEC_UUID("F4418F4B-4A6B-4A93-948F-332025F395E8")
KeWebCamOCX;
#endif
#endif /* __KeWebCamOCXLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


