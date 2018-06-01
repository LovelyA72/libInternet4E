// internet.h : main header file for the INTERNET DLL
//

#if !defined(AFX_INTERNET_H__DAFE237B_24F7_4D84_AC66_0281A62D974E__INCLUDED_)
#define AFX_INTERNET_H__DAFE237B_24F7_4D84_AC66_0281A62D974E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"
#include "msmtp.h"
#include <afxinet.h>
#include <ras.h>
#include "cinet.h"

/////////////////////////////////////////////////////////////////////////////
// CInternetApp
// See internet.cpp for the implementation of this class
//

class CInternetApp : public CWinApp
{
public:
	CInternetApp();

	////////////////////////////////////

	CSMTPConnection m_connect;
	CSMTPMessage m_message;

	CInet m_net;

	HRASCONN m_hRasConn;
	HWND m_hStatusWnd;

	////////////////////////////////////

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInternetApp)
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CInternetApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INTERNET_H__DAFE237B_24F7_4D84_AC66_0281A62D974E__INCLUDED_)
