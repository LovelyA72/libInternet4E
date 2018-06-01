
#include "stdafx.h"
#include "internet.h"
#include "..\libinf.h"
#include "..\krnl\fnshare.h"
#include "libarg.h"
#include "smtp.h"
#include "cinet.h"

extern CInternetApp theApp;

extern INT g_nLastNotifyResult;
INT WINAPI NotifySys (INT nMsg, DWORD dwParam1 = 0, DWORD dwParam2 = 0);

/////////////////////////////////////////////////////

void fnConnectSmtpServer (PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
	char* szSMTPServerAdr = pArgInf [0].m_pText;

	INT nPort = 25;
	if (pArgInf [1].m_dtDataType != _SDT_NULL)
		nPort = pArgInf [1].m_int;

	char* szUserName = NULL;
	if (pArgInf [2].m_dtDataType != _SDT_NULL)
		szUserName = pArgInf [2].m_pText;

	char* szPassword = NULL;
	if (pArgInf [3].m_dtDataType != _SDT_NULL)
		szPassword = pArgInf [3].m_pText;

	INT nTimeOut = 2000;
	if (pArgInf [4].m_dtDataType != _SDT_NULL)
		nTimeOut = max (1, pArgInf [4].m_int);

	pRetData->m_bool = MConnect (szSMTPServerAdr, nPort,
			szUserName, szPassword, nTimeOut);
}

void fnDisconnectSmtpServer (PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
	MDisconnect ();
}

void fnAttachFile (PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
	pRetData->m_bool = MAttachFile (pArgInf [0].m_pText);
}

void fnAttachData (PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
    LPBYTE pBinData = pArgInf [0].m_pBin + sizeof (INT) * 2;
    INT nBinSize = *(LPINT)(pBinData - sizeof (INT));

	pRetData->m_bool = MAttachData ((char*)pBinData, nBinSize,
            pArgInf [1].m_pText);
}

void fnEmptyAttachment (PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
	MEmptyAttachment ();
}

void fnSendMail (PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
	char* subject = pArgInf [0].m_pText;
	char* body = pArgInf [1].m_pText;
	char* to = pArgInf [2].m_pText;

	char* cc = "";
	if (pArgInf [3].m_dtDataType != _SDT_NULL)
		cc = pArgInf [3].m_pText;

	char* bcc = "";
	if (pArgInf [4].m_dtDataType != _SDT_NULL)
		bcc = pArgInf [4].m_pText;

	char* from = pArgInf [5].m_pText;

	char* replyto = from;
	if (pArgInf [6].m_dtDataType != _SDT_NULL)
		replyto = pArgInf [6].m_pText;

	pRetData->m_pText = CloneTextData (MSend (subject, body, to, cc, bcc, from, replyto));
}

///////////////////////////

void fnSetProxyName (PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
	char *szProxyName = NULL, *szUserName = NULL, *szPassword = NULL;
	if (pArgInf [0].m_dtDataType != _SDT_NULL)  szProxyName = pArgInf [0].m_pText;
	if (pArgInf [1].m_dtDataType != _SDT_NULL)  szUserName  = pArgInf [1].m_pText;
	if (pArgInf [2].m_dtDataType != _SDT_NULL)  szPassword  = pArgInf [2].m_pText;
	theApp.m_net.SetProxyName (szProxyName, szUserName, szPassword);
}

void fnGetHttpFile (PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
	CFreqMem mem;
	theApp.m_net.GetHTTPFile (pArgInf [0].m_pText, mem);
    pRetData->m_pBin = CloneBinData (mem.GetPtr (), mem.GetSize ());
}

void fnConnectFTPServer (PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
	char* szFTPServer = pArgInf [0].m_pText;

	char* szUserName = NULL;
	if (pArgInf [1].m_dtDataType != _SDT_NULL)
		szUserName = pArgInf [1].m_pText;

	char* szPassword = NULL;
	if (pArgInf [2].m_dtDataType != _SDT_NULL)
		szPassword = pArgInf [2].m_pText;

	INT nPort = 21;
	if (pArgInf [3].m_dtDataType != _SDT_NULL)
		nPort = pArgInf [3].m_int;

	BOOL blPassive = FALSE;
	if (pArgInf [4].m_dtDataType != _SDT_NULL)
		blPassive = pArgInf [4].m_bool;

	pRetData->m_bool = theApp.m_net.OpenFTPConnection (szFTPServer,
			szUserName, szPassword, nPort, blPassive);
}

void fnDisconnectFTPServer (PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
	theApp.m_net.CloseFTPConnection ();
}

void fnGetFtpFile (PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
	char* szFTPFile = pArgInf [0].m_pText;

	BOOL blTransferAscii = FALSE;
	if (pArgInf [2].m_dtDataType != _SDT_NULL && pArgInf [2].m_bool)
		blTransferAscii = TRUE;

	pRetData->m_bool = theApp.m_net.GetFile (szFTPFile,
			pArgInf [1].m_pText, blTransferAscii);
}

void fnPutFtpFile (PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
	char* szLocalFile = pArgInf [0].m_pText;

	BOOL blTransferAscii = FALSE;
	if (pArgInf [2].m_dtDataType != _SDT_NULL && pArgInf [2].m_bool)
		blTransferAscii = TRUE;

	pRetData->m_bool = theApp.m_net.PutFile (szLocalFile,
			pArgInf [1].m_pText, blTransferAscii);
}

void fnDeleteFtpFile (PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
	pRetData->m_bool = theApp.m_net.Remove (pArgInf [0].m_pText);
}

void fnRenameFtpFile (PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
	pRetData->m_bool = theApp.m_net.Rename (pArgInf [0].m_pText,
			pArgInf [1].m_pText);
}

void fnCreateFtpDir (PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
	pRetData->m_bool = theApp.m_net.CreateDirectory (pArgInf [0].m_pText);
}

void fnRemoveFtpDir (PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
	pRetData->m_bool = theApp.m_net.RemoveDirectory (pArgInf [0].m_pText);
}

void fnSetCurrentFtpDir (PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
	pRetData->m_bool = theApp.m_net.SetCurrentDirectory (pArgInf [0].m_pText);
}

void fnGetCurrentFtpDir (PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
	CString strDirName;
	theApp.m_net.GetCurrentDirectory (strDirName);
    pRetData->m_pText = CloneTextData ((char*)(LPCTSTR)strDirName);
}

void fnListFtpDir (PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
	char* szSearchFile = pArgInf [0].m_pText;

	DWORD dwFlags;
	if (pArgInf [1].m_dtDataType != _SDT_NULL)
		dwFlags = pArgInf [1].m_uint;
    else
	    dwFlags = 0xFFFFFFFF;

	CFreqMem aryFileTime, memTextData;
	CMyDWordArray aryTextOffset, dwaryFileAttr, dwaryFileSize;

	INT nCount = theApp.m_net.list (szSearchFile, dwFlags, aryTextOffset,
			dwaryFileAttr, dwaryFileSize, aryFileTime, memTextData);

	////////////////////////////

	for (INT i = 2; i < 6; i++)
	{
		if (pArgInf [i].m_dtDataType == _SDT_NULL)
            continue;

        LPBYTE p = (LPBYTE)NotifySys (NRS_MALLOC, sizeof (INT) * 2 + nCount *
                (i == 2 ? sizeof (char*) :
                i != 5 ? sizeof (INT) : sizeof (DATE)), 0);
        *(LPINT)p = 1;  // 数组维数。
        *(LPINT)(p + sizeof (INT)) = nCount;

        // 释放原变量的数组数据内容。
        NotifySys (NRS_FREE_ARY, pArgInf [i].m_dtDataType,
                (DWORD)*pArgInf [i].m_ppAryData);
        *pArgInf [i].m_ppAryData = p;   // 将新内容写回该变量。

        p += sizeof (INT) * 2;

		switch (i)
		{
        case 2:
            {
                char** ppText = (char**)p;
                char* ps = (char*)memTextData.GetPtr ();
                for (INT j = 0; j < nCount; j++)
                    ppText [j] = CloneTextData (ps + aryTextOffset [j]);
            }
            break;
		case 3:
            memcpy (p, dwaryFileAttr.GetPtr (), nCount * sizeof (INT));
			break;
		case 4:
            memcpy (p, dwaryFileSize.GetPtr (), nCount * sizeof (INT));
			break;
		case 5:
			{
				FILETIME* pTime = (FILETIME*)aryFileTime.GetPtr ();
                PDATE pDate = (PDATE)p;
				for (INT j = 0; j < nCount; j++)
				{
					//pDate [j] = COleDateTime (pTime [j]); //此转换将视参数pTime [j]为UTC时间, 但它未必是UTC时间, 取决于FTP服务器
					//以下, 直接将FTP服务器返回的文件时间原样返回, 不涉及时区转换
					SYSTEMTIME st; 
					FileTimeToSystemTime(&pTime[j], &st);
					pDate [j] = COleDateTime (st);
				}
			}
			break;
		}
	}

	pRetData->m_int = nCount;
}

void RunEntries (PMDATA_INF pRetData, PMDATA_INF pArgInf, INT nWay)
{
	RASENTRYNAME aryRasEntryName [38];
	memset (aryRasEntryName, 0, sizeof (aryRasEntryName));
	aryRasEntryName [0].dwSize = sizeof (RASENTRYNAME);
	DWORD dwSize = sizeof (aryRasEntryName);
	DWORD dwEntries = 0;
	if (RasEnumEntries (NULL, NULL, aryRasEntryName, &dwSize, &dwEntries))
		dwEntries = 0;

	if (nWay == 0)
	{
        pRetData->m_int = (INT)dwEntries;
	}
	else
	{
		LPTSTR szResult = "";
		INT nIndex = pArgInf [0].m_int;

		RASDIALPARAMS param;
		if (nIndex >= 0 && nIndex < (INT)dwEntries)
		{
			if (nWay == 1)
			{
				szResult = aryRasEntryName [nIndex].szEntryName;
			}
			else
			{
				ASSERT (nWay == 2);
				memset (&param, 0, sizeof (RASDIALPARAMS));
				param.dwSize = sizeof (RASDIALPARAMS);
				strcpy (param.szEntryName, aryRasEntryName [nIndex].szEntryName);
				BOOL blHasPassword = FALSE;
				if (!RasGetEntryDialParams (NULL, &param, &blHasPassword))
					szResult = param.szUserName;
			}
		}

        pRetData->m_pText = CloneTextData (szResult);
	}
}

void fnGetEntriesCount (PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
	RunEntries (pRetData, pArgInf, 0);
}

void fnGetEntryName (PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
	RunEntries (pRetData, pArgInf, 1);
}

void fnGetUserName (PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
	RunEntries (pRetData, pArgInf, 2);
}

void fnIsOnline (PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
	pRetData->m_bool = FALSE;

	RASCONNSTATUS stat;
	memset (&stat, 0, sizeof (stat));
	stat.dwSize = sizeof (RASCONNSTATUS);

	if (theApp.m_hRasConn != NULL &&
			RasGetConnectStatus (theApp.m_hRasConn, &stat) == 0 &&
			stat.rasconnstate == RASCS_Connected)
	{
		pRetData->m_bool = TRUE;
	}
	else
	{
		RASCONN infs [38];
		memset (infs, 0, sizeof (infs));
		infs [0].dwSize = sizeof (RASCONN);

		DWORD dwSize = sizeof (infs);
		DWORD dwConnections = 0;
		if (!RasEnumConnections (infs, &dwSize, &dwConnections))
		{
			for (INT i = 0; i < (INT)dwConnections; i++)
			{
				if (RasGetConnectStatus (infs [i].hrasconn, &stat) == 0 &&
						stat.rasconnstate == RASCS_Connected)
				{
					pRetData->m_bool = TRUE;
					break;
				}
			}
		}
	}
}

void MHangUp (HRASCONN hRasConn)
{
	RasHangUp (hRasConn);

	RASCONNSTATUS stat;
	memset (&stat, 0, sizeof (stat));
	stat.dwSize = sizeof (RASCONNSTATUS);
	DWORD dwTickCount = GetTickCount ();
	while (GetTickCount () - dwTickCount < 3000)
	{
		if (RasGetConnectStatus (hRasConn, &stat) == ERROR_INVALID_HANDLE)
			break;
		Sleep (0);
	}
}

void CloseRasConnection ()
{
	if (theApp.m_hRasConn != NULL)
	{
		HRASCONN hRasConn = theApp.m_hRasConn;
		theApp.m_hRasConn = NULL;
		theApp.m_hStatusWnd = NULL;
		MHangUp (hRasConn);
	}
}

void fnRasHangUp (PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
	if (theApp.m_hRasConn != NULL)
	{
		HWND hStatusWnd = theApp.m_hStatusWnd;
		CloseRasConnection ();

		if (hStatusWnd != NULL && ::IsWindow (hStatusWnd))
		{
			::SetWindowText (hStatusWnd, "连接被挂断");
			::InvalidateRect (hStatusWnd, NULL, TRUE);
		}
	}

	RASCONN infs [38];
	memset (infs, 0, sizeof (infs));
	infs [0].dwSize = sizeof (RASCONN);

	DWORD dwSize = sizeof (infs);
	DWORD dwConnections = 0;
	if (!RasEnumConnections (infs, &dwSize, &dwConnections))
	{
		for (INT i = 0; i < (INT)dwConnections; i++)
			MHangUp (infs [i].hrasconn);
	}
}

VOID WINAPI RasDialFunc (UINT unMsg, RASCONNSTATE rasconnstate, DWORD dwError)
{
	if (theApp.m_hRasConn != NULL && theApp.m_hStatusWnd != NULL &&
			::IsWindow (theApp.m_hStatusWnd))
	{
		char* szCaption;
		if (dwError != 0)
		{
			szCaption = rasconnstate == RASCS_AuthNotify ? "用户验证失败" :
					"有错误发生";
		}
		else
		{
			szCaption =
				rasconnstate == RASCS_OpenPort ? "正在打开端口" :
				rasconnstate == RASCS_PortOpened ? "端口被打开" :
				rasconnstate == RASCS_ConnectDevice ? "正在连接设备" :
				rasconnstate == RASCS_DeviceConnected ? "设备被连接" :
				rasconnstate == RASCS_Authenticate ? "正在验证用户" :
				rasconnstate == RASCS_Authenticated ? "用户验证通过" :
				rasconnstate == RASCS_Connected ? "连接成功" :
				rasconnstate == RASCS_Disconnected ? "连接被断开" :
				NULL;
		}

		if (szCaption != NULL)
		{
			::SetWindowText (theApp.m_hStatusWnd, szCaption);
			::InvalidateRect (theApp.m_hStatusWnd, NULL, TRUE);
		}
	}
}

void fnRasDial (PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf)
{
	CloseRasConnection ();

	pRetData->m_bool = FALSE;

	while (TRUE)
	{
		RASDIALPARAMS param;
		memset (&param, 0, sizeof (param));
		param.dwSize = sizeof (param);

		char* szPhoneNumber = pArgInf [0].m_pText;
		if (*szPhoneNumber == '@')
		{
			RASENTRYNAME aryRasEntryName [38];
			memset (aryRasEntryName, 0, sizeof (aryRasEntryName));
			aryRasEntryName [0].dwSize = sizeof (RASENTRYNAME);
			DWORD dwSize = sizeof (aryRasEntryName);
			DWORD dwEntries = 0;
			if (RasEnumEntries (NULL, NULL, aryRasEntryName, &dwSize, &dwEntries))
				dwEntries = 0;

			INT nIndex = atoi (szPhoneNumber + 1);
			if (nIndex >= 0 && nIndex < (INT)dwEntries)
				strcpy (param.szEntryName, aryRasEntryName [nIndex].szEntryName);
			else
				break;
		}
		else
			strcpy (param.szPhoneNumber, szPhoneNumber);

		strcpy (param.szUserName, pArgInf [1].m_pText);
		strcpy (param.szPassword, pArgInf [2].m_pText);

		LPVOID lpvNotifier = RasDialFunc;
		if (pArgInf [3].m_dtDataType != _SDT_NULL && pArgInf [3].m_bool)
			lpvNotifier = NULL;

		if (RasDial (NULL, NULL, &param, 0, lpvNotifier, &theApp.m_hRasConn) != 0)
		{
			CloseRasConnection ();
			break;
		}

		if (lpvNotifier != NULL && pArgInf [4].m_dtDataType != _SDT_NULL)
		{
			CWnd* pWnd = (CWnd*)NotifySys (NRS_GET_UNIT_PTR,
					pArgInf [4].m_unit.m_dwFormID,
					pArgInf [4].m_unit.m_dwUnitID);
			theApp.m_hStatusWnd = pWnd->GetSafeHwnd ();
		}
		else
			theApp.m_hStatusWnd = NULL;

		pRetData->m_bool = TRUE;
		break;
	}
}

