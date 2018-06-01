// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__EA53B6A3_660E_45AD_9B7A_35DB9EB237BF__INCLUDED_)
#define AFX_STDAFX_H__EA53B6A3_660E_45AD_9B7A_35DB9EB237BF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../lang.h"   // 加入编译语言定义宏。

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxtempl.h>			  // MFC template classes
#include <afxsock.h>        // MFC Sockets
#include <afxpriv.h>        // to get access to the T2A macro

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxole.h>         // MFC OLE classes
#include <afxodlgs.h>       // MFC OLE dialog classes
#include <afxdisp.h>        // MFC Automation classes
#endif // _AFX_NO_OLE_SUPPORT


#ifndef _AFX_NO_DB_SUPPORT
#include <afxdb.h>			// MFC ODBC database classes
#endif // _AFX_NO_DB_SUPPORT

#ifndef _AFX_NO_DAO_SUPPORT
#include <afxdao.h>			// MFC DAO database classes
#endif // _AFX_NO_DAO_SUPPORT

#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT


//!!! 编译 .fnr 时设置此宏，否则为编译 .fne 。
// #define	__COMPILE_FNR

#ifdef __COMPILE_FNR
	#define	_WT(text)	_T("")
#else
	#define	_WT(text)	_T(text)
#endif


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__EA53B6A3_660E_45AD_9B7A_35DB9EB237BF__INCLUDED_)
