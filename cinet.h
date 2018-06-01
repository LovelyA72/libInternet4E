#if !defined(__INET_H_)
#define __INET_H_

#include "..\mem.h"

class CInet
{
public:
	CInet();
	~CInet();

private:
	HINTERNET m_hSession;
	CString m_strProxyName, m_strProxyUserName, m_strProxyPassword;

	HINTERNET m_hConnection;	// FTP Connection.

protected:
	BOOL OpenSession ();
	void CloseSession ();

public:
	void SetProxyName (char* szProxyName, char* szUserName = NULL, char* szPassword = NULL);
	void Close ();

	void CInet::SetProxy(HINTERNET hInternet, char* szProxyName, char* szUserName, char* szPassword);
private:
	void CInet::SetProxy(HINTERNET hInternet);

public:	// http
	BOOL GetHTTPFile (char* szURL, CFreqMem& mem);

public:	// ftp
	BOOL OpenFTPConnection (char* szServer, char* szUserName = NULL,
			char* szPassword = NULL,
			INTERNET_PORT nPort = INTERNET_INVALID_PORT_NUMBER,
			BOOL bPassive = FALSE);
	void CloseFTPConnection ();

	BOOL GetCurrentDirectory(CString& strDirName) const;
	BOOL SetCurrentDirectory(char* pstrDirName);

	BOOL RemoveDirectory(char* pstrDirName);
	BOOL CreateDirectory(char* pstrDirName);
	BOOL Rename(char* pstrExisting, char* pstrNew);
	BOOL Remove(char* pstrFileName);

	BOOL GetFile(char* pstrRemoteFile, char* pstrLocalFile, BOOL blTransferAscii);
	BOOL PutFile(char* pstrLocalFile, char* pstrRemoteFile, BOOL blTransferAscii);

	INT list (char* szSearchFile, DWORD dwFlags, CMyDWordArray& aryTextOffset,
			CMyDWordArray& dwaryFileAttr, CMyDWordArray& dwaryFileSize,
			CFreqMem& aryFileTime, CFreqMem& memTextData);
};

#endif
