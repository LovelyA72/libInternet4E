
#include "stdafx.h"
#include <AFXINET.H>
#include "..\mem.h"
#include "internet.h"
#include "cinet.h"

extern CInternetApp theApp;

// Constructor
CInet::CInet()
{
	m_hSession = NULL;
}

// Destructor
CInet::~CInet()
{
	Close ();
}

// Initialize a new WinInet session
// szProxyName != NULL if use proxy .
BOOL CInet::OpenSession ()
{
	if (m_hSession == NULL)
	{
		BOOL blUseProxy = !m_strProxyName.IsEmpty ();

		// Open the new session
		m_hSession = ::InternetOpen ("Mozilla/4.0 (compatible; MSIE 6.0; Windows NT 5.0)",
				(blUseProxy ? INTERNET_OPEN_TYPE_PROXY : INTERNET_OPEN_TYPE_DIRECT),
				(blUseProxy ? (LPCSTR)(LPCTSTR)m_strProxyName : NULL),
				NULL, 0);
	}

	// Did it work??
	return m_hSession != NULL;
}

// Close the WinInet session
void CInet::CloseSession ()
{
	if (m_hSession != NULL)
	{
		InternetCloseHandle (m_hSession);
		m_hSession = NULL;
	}
}

void CInet::Close ()
{
	CloseFTPConnection ();
	CloseSession ();
}

//20081125, add szUserName and szPassword args
void CInet::SetProxyName (char* szProxyName, char* szUserName, char* szPassword)
{
	SetProxy(NULL, szProxyName, szUserName, szPassword);
	return;

	//20081126: 以下是以前的代码, 暂时保留


	m_strProxyName = szProxyName == NULL ? "" : szProxyName;

	if(m_hSession == NULL) return;

	//设置代理服务器名称
	{
		INTERNET_PROXY_INFO inf;
		inf.lpszProxyBypass = NULL;
		if (m_strProxyName.IsEmpty ())
		{
			inf.dwAccessType = INTERNET_OPEN_TYPE_PRECONFIG;
			inf.lpszProxy = NULL;
		}
		else
		{
			inf.dwAccessType = INTERNET_OPEN_TYPE_PROXY;
			inf.lpszProxy = (LPCSTR)(LPCTSTR)m_strProxyName;
		}

		::InternetSetOption (m_hSession, INTERNET_OPTION_PROXY,
				&inf, sizeof (inf));
	}
}

//20081126, add
void CInet::SetProxy(HINTERNET hInternet, char* szProxyName, char* szUserName, char* szPassword)
{
	m_strProxyName = (szProxyName == NULL ? "" : szProxyName);
	m_strProxyUserName = (szUserName == NULL ? "" : szUserName);
	m_strProxyPassword = (szPassword == NULL ? "" : szPassword);

	if(hInternet)
		SetProxy(hInternet);
}

//20081126, add
void CInet::SetProxy(HINTERNET hInternet)
{
	//设置代理服务器名称

	INTERNET_PROXY_INFO inf;
	inf.lpszProxyBypass = NULL;
	if (m_strProxyName.IsEmpty ())
	{
		inf.dwAccessType = INTERNET_OPEN_TYPE_PRECONFIG;
		inf.lpszProxy = NULL;
	}
	else
	{
		inf.dwAccessType = INTERNET_OPEN_TYPE_PROXY;
		inf.lpszProxy = (LPCSTR)(LPCTSTR)m_strProxyName;
	}

	//经测试只能应用于m_hSession, 不能用于hInternet (否则GetLastError()返回20018)
	BOOL bOK = ::InternetSetOption (m_hSession, INTERNET_OPTION_PROXY, &inf, sizeof(inf));
	ASSERT(bOK);


	//设置代理服务器用户名和密码, 经测试不能应用于m_hSession (InternetOpen返回)

	bOK = ::InternetSetOption (hInternet, INTERNET_OPTION_PROXY_USERNAME,
				(LPVOID)(LPCSTR)m_strProxyUserName, m_strProxyUserName.GetLength()+1);
	ASSERT(bOK);

	bOK = ::InternetSetOption (hInternet, INTERNET_OPTION_PROXY_PASSWORD,
				(LPVOID)(LPCSTR)m_strProxyPassword, m_strProxyPassword.GetLength()+1);
	ASSERT(bOK);
}

// not found in wininet.h, and not found in wininet.lib
typedef BOOL (WINAPI *PFN_FtpCommand) (
  HINTERNET hConnect,
  BOOL fExpectResponse,
  DWORD dwFlags,
  LPCTSTR lpszCommand,
  DWORD_PTR dwContext,
  HINTERNET* phFtpCommand
);

BOOL CInet::OpenFTPConnection (char* szServer, char* szUserName,
		char* szPassword, INTERNET_PORT nPort, BOOL bPassive)
{
	CloseFTPConnection ();

	if (!OpenSession ())
		return FALSE;

	if(m_strProxyName.IsEmpty())
	{
		m_hConnection = InternetConnect (m_hSession, szServer,
			nPort, szUserName, szPassword, INTERNET_SERVICE_FTP,
			(bPassive ? INTERNET_FLAG_PASSIVE : 0), 0);
	}
	else //20081126 add, 处理FTP代理
	{
		//连接代理服务器
		CString strProxyServer = m_strProxyName;
		int nProxyPort = 21;
		int nColonIndex = m_strProxyName.ReverseFind(':');
		if(nColonIndex >= 0)
		{
			strProxyServer = m_strProxyName.Left(nColonIndex);
			nProxyPort = atoi(m_strProxyName.Right(m_strProxyName.GetLength()-nColonIndex-1));
		}

		CString user; user.Format("%s@%s:%d", szUserName, szServer, nPort);
		CString pass; pass.Format("%s", szPassword);

		m_hConnection = InternetConnect (m_hSession, (LPCSTR)strProxyServer, nProxyPort,
			(LPCSTR)user, (LPCSTR)pass,
			INTERNET_SERVICE_FTP, (bPassive ? INTERNET_FLAG_PASSIVE : 0), 0);

		//也可参考以下文章, 但我一直没有测试成功（InternetConnect()时就失败）:
		//http://forums.microsoft.com/MSDN-CHT/ShowPost.aspx?PostID=1373999&SiteID=14
	}

	return m_hConnection != NULL;
}

// Close the FTP connect
void CInet::CloseFTPConnection ()
{
	if (m_hConnection != NULL)
	{
		InternetCloseHandle (m_hConnection);
		m_hConnection = NULL;
	}
}

BOOL CInet::Remove(char* pstrFileName)
{
	ASSERT(AfxIsValidString(pstrFileName));

	if (m_hConnection == NULL)
		return FALSE;
	else
		return FtpDeleteFile(m_hConnection, pstrFileName);
}

BOOL CInet::Rename(char* pstrExisting, char* pstrNew)
{
	ASSERT(AfxIsValidString(pstrExisting));
	ASSERT(AfxIsValidString(pstrNew));

	if (m_hConnection == NULL)
		return FALSE;
	else
		return FtpRenameFile(m_hConnection, pstrExisting, pstrNew);
}

BOOL CInet::CreateDirectory(char* pstrDirName)
{
	ASSERT(AfxIsValidString(pstrDirName));

	if (m_hConnection == NULL)
		return FALSE;
	else
		return FtpCreateDirectory(m_hConnection, pstrDirName);
}

BOOL CInet::RemoveDirectory(char* pstrDirName)
{
	ASSERT(AfxIsValidString(pstrDirName));

	if (m_hConnection == NULL)
		return FALSE;
	else
		return FtpRemoveDirectory(m_hConnection, pstrDirName);
}

BOOL CInet::SetCurrentDirectory(char* pstrDirName)
{
	ASSERT(AfxIsValidString(pstrDirName));

	if (m_hConnection == NULL)
		return FALSE;
	else
		return FtpSetCurrentDirectory(m_hConnection, pstrDirName);
}

BOOL CInet::GetCurrentDirectory(CString& strDirName) const
{
	if (m_hConnection == NULL)
		return FALSE;

	DWORD dwLen = INTERNET_MAX_PATH_LENGTH;
	LPTSTR pstrTarget = strDirName.GetBufferSetLength (dwLen);
	BOOL bRet = FtpGetCurrentDirectory (m_hConnection, pstrTarget, &dwLen);

	if (bRet)
		strDirName.ReleaseBuffer (dwLen);
	else
		strDirName.ReleaseBuffer (0);

	return bRet;
}

BOOL CInet::PutFile(char* pstrLocalFile, char* pstrRemoteFile, BOOL blTransferAscii)
{
	ASSERT(AfxIsValidString(pstrRemoteFile));
	ASSERT(AfxIsValidString(pstrLocalFile));

	if (m_hConnection == NULL)
		return FALSE;
	else
		return FtpPutFile(m_hConnection, pstrLocalFile, pstrRemoteFile,
                (blTransferAscii ? FTP_TRANSFER_TYPE_ASCII : FTP_TRANSFER_TYPE_BINARY), 0);
}

BOOL CInet::GetFile(char* pstrRemoteFile, char* pstrLocalFile, BOOL blTransferAscii)
{
	ASSERT(AfxIsValidString(pstrRemoteFile));
	ASSERT(AfxIsValidString(pstrLocalFile));

	if (m_hConnection == NULL)
		return FALSE;
	else
		return FtpGetFile (m_hConnection, pstrRemoteFile, pstrLocalFile,
				FALSE, FILE_ATTRIBUTE_NORMAL, (INTERNET_FLAG_RELOAD | INTERNET_FLAG_DONT_CACHE |
                (blTransferAscii ? FTP_TRANSFER_TYPE_ASCII : FTP_TRANSFER_TYPE_BINARY)), 0);
}

INT CInet::list (char* szSearchFile, DWORD dwFlags, CMyDWordArray& aryTextOffset,
				 CMyDWordArray& dwaryFileAttr, CMyDWordArray& dwaryFileSize,
				 CFreqMem& aryFileTime, CFreqMem& memTextData)
{
	if (dwFlags != 0xffffffff)
	{
		dwFlags &= FILE_ATTRIBUTE_READONLY | FILE_ATTRIBUTE_HIDDEN | FILE_ATTRIBUTE_SYSTEM |
				FILE_ATTRIBUTE_DIRECTORY | FILE_ATTRIBUTE_ARCHIVE;
	}

    aryTextOffset.Empty ();
	memTextData.Empty ();
	dwaryFileAttr.RemoveAll ();
	dwaryFileSize.RemoveAll ();
	aryFileTime.Empty ();

	if (m_hConnection == NULL)
		return 0;

	CString strFindFile = szSearchFile;
	if (strFindFile.Right (3) == _T("*.*"))
		strFindFile = strFindFile.Left (strFindFile.GetLength () - 2);	// 将"*.*"转换为"*"

	WIN32_FIND_DATA FindData;
	HINTERNET hFind = FtpFindFirstFile (m_hConnection, strFindFile, &FindData,
			(INTERNET_FLAG_RELOAD | INTERNET_FLAG_DONT_CACHE), 0);
	if (hFind == NULL)
		return 0;

	do
	{
		if (dwFlags == 0xffffffff ||
				dwFlags == 0 && FindData.dwFileAttributes == 0 ||
				(FindData.dwFileAttributes & dwFlags) != 0)
		{
            aryTextOffset.AddDWord (memTextData.GetSize ());
			memTextData.AddStr (FindData.cFileName);
			dwaryFileAttr.Add (FindData.dwFileAttributes);
			dwaryFileSize.Add (FindData.nFileSizeLow);
			aryFileTime.Append ((LPBYTE)&FindData.ftLastWriteTime, sizeof (FILETIME));
		}
	}
	while (InternetFindNextFile (hFind, &FindData));

	memTextData.AddByte (0);
	InternetCloseHandle (hFind);
	return dwaryFileAttr.GetSize ();
}

//////////////////////

BOOL CInet::GetHTTPFile (char* szURL, CFreqMem& mem)
{
	mem.Empty ();

	if (szURL == NULL || *szURL == '\0')
		return FALSE;

	HINTERNET hConnect = NULL;
	HINTERNET hURL = NULL;
	BOOL blOK = FALSE;

	while (TRUE)
	{
		TRY
		{
			// Make sure this URL looks valid
			CString strServer, strObject, strUser, strPassword;
			INTERNET_PORT nPort;
			DWORD dwServiceType;

			// Bail if it doesn't look okay
			CString strURL = szURL;
			strURL.SpanExcluding ("#");
			if(!AfxParseURLEx (strURL, dwServiceType, strServer, strObject, nPort,
					strUser, strPassword, ICU_NO_ENCODE))
				break;

			//process HTTPS
			DWORD dwInternatFlags = INTERNET_FLAG_DONT_CACHE|INTERNET_FLAG_RELOAD;
			if(dwServiceType == 4107) //HTTPS
			{
				dwServiceType = INTERNET_SERVICE_HTTP;
				dwInternatFlags |= INTERNET_FLAG_SECURE;
			}

			if (!OpenSession ())
				break;
			ASSERT(m_hSession);

			// Establish a server connection
			//2008.7.10 modify: use port, user, password and servicetype
			hConnect = ::InternetConnect (m_hSession, strServer,
					nPort, strUser, strPassword, dwServiceType, 0, 0) ;

			if (hConnect == NULL)
				break;

			SetProxy(hConnect); //20081126 add

			// Open an HTTP protocol session
			strObject = strObject.SpanExcluding ("#");
			const char * lpszAcceptTypes [2] = { "Accept: */* \r\n", NULL };
			
			hURL = ::HttpOpenRequest (hConnect, "GET", strObject, HTTP_VERSION, NULL,
					lpszAcceptTypes, dwInternatFlags, 0);
			if (hURL == NULL)
				break;

			// Send the "get" request
			const char * lpHttpHeader = "Accept: */*\r\n\r\n"; //"Accept: image/gif, image/x-xbitmap, image/jpeg, image/pjpeg, application/x-shockwave-flash, application/vnd.ms-excel, application/vnd.ms-powerpoint, application/msword, */*\r\nAccept-Language: zh-CN\r\n";
 			if (!::HttpSendRequest (hURL, lpHttpHeader, strlen(lpHttpHeader), 0, 0))
				break;

			// Find out how much data we have
			char buf [32];
			DWORD dwLen = sizeof (buf);
			if (::HttpQueryInfo (hURL, HTTP_QUERY_CONTENT_LENGTH,
					buf, &dwLen, NULL))
			{
				dwLen = (DWORD)atol (buf);
				// Just make sure we didn't get a stupid length value back
				if ((INT)dwLen <= 0)
					dwLen = 10240;
				else
					dwLen = min (102400, dwLen + 1);
			}
			else
				dwLen = 10240;

			CFreqMem memBuf;
			LPBYTE pBuf = memBuf.Alloc ((INT)dwLen);
			if (pBuf == NULL)
				break;
			// Get the data for the file
			DWORD dwBytesRead;
			// Keep reading data until we've gotten it all
			while (TRUE)
			{
				// Add to the counts
				if (!::InternetReadFile (hURL, pBuf, dwLen, &dwBytesRead))
					break;

				if (dwBytesRead == 0)
				{
					blOK = TRUE;
					break;
				}

				mem.Append (pBuf, (INT)dwBytesRead);
			}
		}
		END_TRY

		break;
	}

	// Fatal error, just get out
	if (hURL) 
		InternetCloseHandle (hURL);
	if (hConnect)
		InternetCloseHandle (hConnect);

	return blOK;
}
