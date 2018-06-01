
#include "stdafx.h"
#include "msmtp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BOOL IsAsciiText (char* ps);
char* GetCharset (char* ps);
void doEncode (char* pd, int nDataSize, int nMaxLen, BOOL blNotInsertCRLF, CString& strResult);
void encode (char* ps, CString& strResult);
char* EncodeBase64 (char* pszIn, int nInLen, int* nOutLen, BOOL blInsCRLF = TRUE);

////////////////////////////////////////////////////////

BOOL IsAsciiText (char* ps)
{
	while (*ps != '\0')
	{
		if (*ps & 0x80)
			return FALSE;
		ps++;
	}

	return TRUE;
}

char* GetCharset (char* ps)
{
	return IsAsciiText (ps) ? "us-ascii" : "gb2312";
}

void doEncode (char* pd, int nDataSize, int nMaxLen, BOOL blNotInsertCRLF, CString& strResult)
{
	if ((nDataSize + 2) / 3 * 4 > nMaxLen && nDataSize > 1)
	{
		int nHalfLen = nDataSize / 2;
		doEncode (pd, nHalfLen, nMaxLen, blNotInsertCRLF, strResult);
		doEncode (pd + nHalfLen, nDataSize - nHalfLen, nMaxLen, FALSE, strResult);
		return;
	}

	if (!blNotInsertCRLF)
		strResult += ("\r\n ");

	strResult += "=?gb2312?B?";

	int nOutLen;
	char* p = EncodeBase64 (pd, nDataSize, &nOutLen, FALSE);
	strResult += p;
	delete[] p;

	strResult += "?=";
}

//////////////////////////////

void encode (char* ps, CString& strResult)
{
	if (IsAsciiText (ps))
		strResult = ps;
	else
		doEncode (ps, strlen (ps), 68 - strlen ("gb2312"), TRUE, strResult);
}


// 返回值需要被DELETE.
char* EncodeBase64 (char* pszIn, int nInLen, int* nOutLen, BOOL blInsCRLF)
{
	#define BASE64_MAXLINE  76
	#define EOL  "\r\n"

	char base64tab [] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

	int nOutSize = (nInLen + 2) / 3 * 4;	// 3:4 conversion ratio
	nOutSize += strlen (EOL) * nOutSize / BASE64_MAXLINE + 3;  // Space for newlines and NUL

	char* pszOut = new char [nOutSize];

	//Input Parameter validation
	ASSERT (pszOut);
	ASSERT (nOutSize);

	//Set up the parameters prior to the main encoding loop
	int nInPos  = 0;
	int nOutPos = 0;
	int nLineLen = 0;
	int c1, c2, c3;
	char* cp;

	// Get three characters at a time from the input buffer and encode them
	for (int i = 0; i < nInLen / 3; ++i) 
	{
		//Get the next 2 characters
		c1 = pszIn [nInPos++] & 0xFF;
		c2 = pszIn [nInPos++] & 0xFF;
		c3 = pszIn [nInPos++] & 0xFF;

		//Encode into the 4 6 bit characters
		pszOut [nOutPos++] = base64tab [(c1 & 0xFC) >> 2];
		pszOut [nOutPos++] = base64tab [((c1 & 0x03) << 4) | ((c2 & 0xF0) >> 4)];
		pszOut [nOutPos++] = base64tab [((c2 & 0x0F) << 2) | ((c3 & 0xC0) >> 6)];
		pszOut [nOutPos++] = base64tab [c3 & 0x3F];
		nLineLen += 4;

		//Handle the case where we have gone over the max line boundary
		if (blInsCRLF && nLineLen >= BASE64_MAXLINE - 3)
		{
			cp = EOL;
			pszOut [nOutPos++] = *cp++;
			if (*cp)
				pszOut [nOutPos++] = *cp;
			nLineLen = 0;
		}
	}

	// Encode the remaining one or two characters in the input buffer
	switch (nInLen % 3) 
	{
    case 0:
		if (blInsCRLF)
		{
			cp = EOL;
			pszOut [nOutPos++] = *cp++;
			if (*cp)
				pszOut [nOutPos++] = *cp;
		}
		break;
    case 1:
		c1 = pszIn [nInPos] & 0xFF;
		pszOut [nOutPos++] = base64tab [(c1 & 0xFC) >> 2];
		pszOut [nOutPos++] = base64tab [((c1 & 0x03) << 4)];
		pszOut [nOutPos++] = '=';
		pszOut [nOutPos++] = '=';
		if (blInsCRLF)
		{
			cp = EOL;
			pszOut [nOutPos++] = *cp++;
			if (*cp) 
				pszOut [nOutPos++] = *cp;
		}
		break;
    case 2:
		c1 = pszIn [nInPos++] & 0xFF;
		c2 = pszIn [nInPos] & 0xFF;
		pszOut [nOutPos++] = base64tab [(c1 & 0xFC) >> 2];
		pszOut [nOutPos++] = base64tab [((c1 & 0x03) << 4) | ((c2 & 0xF0) >> 4)];
		pszOut [nOutPos++] = base64tab [((c2 & 0x0F) << 2)];
		pszOut [nOutPos++] = '=';
		if (blInsCRLF)
		{
			cp = EOL;
			pszOut [nOutPos++] = *cp++;
			if (*cp) 
				pszOut [nOutPos++] = *cp;
		}
		break;
    default: 
		ASSERT(FALSE); 
		break;
	}

	pszOut [nOutPos] = 0;
	*nOutLen = nOutPos;
	return pszOut;
}


//////////////// Implementation //////////////////////////////////////

CSMTPSocket::CSMTPSocket ()
{
	m_hSocket = INVALID_SOCKET;	//default to an invalid scoket descriptor
}

CSMTPSocket::~CSMTPSocket ()
{
	Close ();
}

BOOL CSMTPSocket::Create ()
{
	Close ();

	m_hSocket = socket (AF_INET, SOCK_STREAM, 0);
	if (m_hSocket == INVALID_SOCKET)
		return FALSE;

	INT nBufSize = 0;
	setsockopt (m_hSocket, SOL_SOCKET, SO_SNDBUF, (char*)&nBufSize, sizeof (nBufSize));

	return TRUE;
}

BOOL CSMTPSocket::Connect (char* pszHostAddress, int nPort)
{
	//must have been created first
	ASSERT (m_hSocket != INVALID_SOCKET);
  
	LPSTR lpszAscii = (LPTSTR)pszHostAddress;

	//Determine if the address is in dotted notation
	SOCKADDR_IN sockAddr;
	ZeroMemory (&sockAddr, sizeof (sockAddr));
	sockAddr.sin_family = AF_INET;
	sockAddr.sin_port = htons ((u_short)nPort);
	sockAddr.sin_addr.s_addr = inet_addr (lpszAscii);

	//If the address is not dotted notation, then do a DNS 
	//lookup of it.
	if (sockAddr.sin_addr.s_addr == INADDR_NONE)
	{
		LPHOSTENT lphost;
		lphost = gethostbyname (lpszAscii);
		if (lphost != NULL)
			sockAddr.sin_addr.s_addr = ((LPIN_ADDR)lphost->h_addr)->s_addr;
		else
		{
			WSASetLastError (WSAEINVAL); 
			return FALSE;
		}
	}

	//Call the protected version which takes an address 
	//in the form of a standard C style struct.
	return Connect ((SOCKADDR*)&sockAddr, sizeof (sockAddr));
}

BOOL CSMTPSocket::Connect (const SOCKADDR* lpSockAddr, int nSockAddrLen)
{
	return (connect (m_hSocket, lpSockAddr, nSockAddrLen) != SOCKET_ERROR);
}

BOOL CSMTPSocket::Send (LPCSTR pszBuf, int nBuf)
{
	//must have been created first
	ASSERT (m_hSocket != INVALID_SOCKET);
	return (send (m_hSocket, pszBuf, nBuf, 0) != SOCKET_ERROR);
}

int CSMTPSocket::Receive (LPSTR pszBuf, int nBuf)
{
	//must have been created first
	ASSERT (m_hSocket != INVALID_SOCKET);
	return recv (m_hSocket, pszBuf, nBuf, 0); 
}

void CSMTPSocket::Close ()
{
	if (m_hSocket != INVALID_SOCKET)
	{
		VERIFY (SOCKET_ERROR != closesocket (m_hSocket));
		m_hSocket = INVALID_SOCKET;
	}
}

BOOL CSMTPSocket::IsReadible (BOOL* pbReadible)
{
	timeval timeout = { 0, 0 };
	fd_set fds;
	FD_ZERO (&fds);
	FD_SET (m_hSocket, &fds);

	int nStatus = select (0, &fds, NULL, NULL, &timeout);
	if (nStatus == SOCKET_ERROR)
	{
		return FALSE;
	}
	else
	{
		*pbReadible = !(nStatus == 0);
		return TRUE;
	}
}

//////////////////////////////////////////////

CSMTPAddress::CSMTPAddress ()
{
}

CSMTPAddress::CSMTPAddress (const CSMTPAddress& address)
{
	*this = address;
}

CSMTPAddress::CSMTPAddress (const CString& sAddress) : m_sEmailAddress (sAddress) 
{
	ASSERT (m_sEmailAddress.GetLength ());	// An empty address is not allowed
}

CSMTPAddress::CSMTPAddress (const CString& sFriendly, const CString& sAddress) :
              m_sFriendlyName (sFriendly), m_sEmailAddress (sAddress) 
{
	ASSERT (m_sEmailAddress.GetLength ());	// An empty address is not allowed
}

CSMTPAddress& CSMTPAddress::operator= (const CSMTPAddress& r) 
{ 
	m_sFriendlyName = r.m_sFriendlyName; 
	m_sEmailAddress = r.m_sEmailAddress; 
	return *this;
}

CString CSMTPAddress::GetRegularFormat () const
{
	ASSERT(m_sEmailAddress.GetLength ()); //Email Address must be valid

	CString sAddress;
	if (m_sFriendlyName.IsEmpty ())
		sAddress = m_sEmailAddress;  //Just transfer the address across directly
	else
	{
		CString str;
		encode ((char*)(LPCTSTR)m_sFriendlyName, str);
		sAddress.Format (_T("%s <%s>"), str, m_sEmailAddress);
	}

	return sAddress;
}

///////////////////////////////////////////////////////////////////////

CSMTPAttachment::CSMTPAttachment (BOOL blMustDelete)
{
	m_pszEncoded = NULL;
	m_nEncodedSize = 0;
	m_blMustDelete = blMustDelete;
}

CSMTPAttachment::~CSMTPAttachment ()
{
	//free up any memory we allocated
	if (m_pszEncoded)
	{
		delete[] m_pszEncoded;
		m_pszEncoded = NULL;
	}
}

BOOL CSMTPAttachment::Attach (char* sFilename, char* szTitle)
{
	// free up any memory we previously allocated
	if (m_pszEncoded)
	{
		delete [] m_pszEncoded;
		m_pszEncoded = NULL;
	}

	if (sFilename == NULL || *sFilename == '\0')
		return FALSE;

	// determine the file size
	CFileStatus fs;
	if (!CFile::GetStatus (sFilename, fs))
	{
		TRACE (_T("Failed to get the status for file %s, probably does not exist\n"), sFilename);
		return FALSE;
	}

	// open up the file for reading in
	CFile infile;
	if (!infile.Open (sFilename, CFile::modeRead | CFile::shareDenyWrite))
	{
		TRACE(_T("Failed to open file to be attached\n"));
		return FALSE;
	}

	// read in the contents of the input file
	char* pszIn = new char [fs.m_size];
	if ((int)infile.Read (pszIn, fs.m_size) != fs.m_size)
	{
		infile.Close ();
		delete[] pszIn;
		return FALSE;
	}
	// Close the input file
	infile.Close ();

	// Do the encoding
	m_pszEncoded = EncodeBase64 (pszIn, fs.m_size, &m_nEncodedSize);

	// delete the input buffer
	delete[] pszIn;

	//Hive away the filename
	TCHAR sPath [_MAX_PATH];
	TCHAR sFname [_MAX_FNAME];
	TCHAR sExt [_MAX_EXT];
	_tsplitpath (sFilename, NULL, NULL, sFname, sExt);
	_tmakepath (sPath, NULL, NULL, sFname, sExt);
	m_sFilename = sPath;
	m_sTitle = szTitle == NULL ? sPath : szTitle;

	return TRUE;
}

BOOL CSMTPAttachment::Attach (char* pData, int nDataSize, char* szFileName, char* szTitle)
{
	if (nDataSize < 0 || szFileName == NULL || *szFileName == '\0')
		return FALSE;

	if (m_pszEncoded)
	{
		delete[] m_pszEncoded;
		m_pszEncoded = NULL;
	}

	m_pszEncoded = EncodeBase64 (pData, nDataSize, &m_nEncodedSize);
	m_sFilename = szFileName;
	m_sTitle = szTitle == NULL ? m_sFilename : szTitle;

	return TRUE;
}

CSMTPMessage::CSMTPMessage () /* : m_sXMailer (_T("EYUYAN MAIL")) */
{
}

CSMTPMessage::~CSMTPMessage ()
{
	EmptyAllAttachment ();
}

void CSMTPMessage::init ()
{
	CSMTPAddress adr;
	m_From = adr;
	m_ReplyTo = adr;

	m_sSubject.Empty ();
	m_sBody.Empty ();
	m_ToRecipients.RemoveAll ();
	m_CCRecipients.RemoveAll ();
	m_BCCRecipients.RemoveAll ();
}

void CSMTPMessage::EmptyAllAttachment ()
{
	INT nCount = GetNumberOfAttachments ();
	for (int i = 0; i < nCount; i++)
		GetAttachment (i)->Delete ();

	m_Attachments.RemoveAll ();
}

int CSMTPMessage::GetNumberOfRecipients (RECIPIENT_TYPE RecipientType) const
{
	int nSize = 0;
	switch (RecipientType)
	{
		case TO:  nSize = m_ToRecipients.GetSize ();  break;
		case CC:  nSize = m_CCRecipients.GetSize ();  break;
		case BCC: nSize = m_BCCRecipients.GetSize (); break;
		default: ASSERT (FALSE);                      break;
	}

	return nSize;
}

int CSMTPMessage::AddRecipient (CSMTPAddress& recipient, RECIPIENT_TYPE RecipientType)
{
	int nIndex = -1;
	switch (RecipientType)
	{
		case TO:  nIndex = m_ToRecipients.Add (recipient);  break;
		case CC:  nIndex = m_CCRecipients.Add (recipient);  break;
		case BCC: nIndex = m_BCCRecipients.Add (recipient); break;
		default: ASSERT (FALSE);                            break;
	}

	return nIndex;
}

void CSMTPMessage::RemoveRecipient (int nIndex, RECIPIENT_TYPE RecipientType)
{
	switch (RecipientType)
	{
		case TO:  m_ToRecipients.RemoveAt (nIndex);  break;
		case CC:  m_CCRecipients.RemoveAt (nIndex);  break;
		case BCC: m_BCCRecipients.RemoveAt (nIndex); break;
		default:  ASSERT (FALSE);                    break;
	}
}

CSMTPAddress CSMTPMessage::GetRecipient (int nIndex, RECIPIENT_TYPE RecipientType) const
{
	CSMTPAddress address;

	switch (RecipientType)
	{
		case TO:  address = m_ToRecipients.GetAt (nIndex);  break;
		case CC:  address = m_CCRecipients.GetAt (nIndex);  break;
		case BCC: address = m_BCCRecipients.GetAt (nIndex); break;
		default: ASSERT (FALSE);                            break;
	}

	return address;
}

BOOL CSMTPMessage::AddAttachment (CSMTPAttachment* pAttachment)
{
	if (pAttachment->GetEncodedBuffer () == NULL ||
			pAttachment->GetFilename ().GetLength () == 0)
		return FALSE;

	m_Attachments.Add (pAttachment);
	return TRUE;
}

void CSMTPMessage::RemoveAttachment (int nIndex)
{
	m_Attachments.RemoveAt (nIndex);
}

CSMTPAttachment* CSMTPMessage::GetAttachment (int nIndex) const
{
	return m_Attachments.GetAt (nIndex);
}

int CSMTPMessage::GetNumberOfAttachments () const
{
	return m_Attachments.GetSize ();
}

CString CSMTPMessage::GetHeader ()
{
	// Form the Timezone info which will form part of the Date header
	TIME_ZONE_INFORMATION tzi;
	int nTZBias;
	if (GetTimeZoneInformation (&tzi) == TIME_ZONE_ID_DAYLIGHT)
		nTZBias = tzi.Bias + tzi.DaylightBias;
	else
		nTZBias = tzi.Bias;

	CString sTZBias;
	sTZBias.Format (_T("%+.2d%.2d"), -nTZBias / 60, nTZBias % 60);

	// Create the "Date:" part of the header
	CTime now (CTime::GetCurrentTime ());
	CString sDate (now.Format (_T("%a, %d %b %Y %H:%M:%S ")));
	sDate += sTZBias;

	// Create the "To:" part of the header
	CString sTo;
	CSMTPAddress recipient;
	int nCount = GetNumberOfRecipients (TO);
	for (int i = 0; i < nCount; i++)
	{
		recipient = GetRecipient (i, TO);
		if (i)
			sTo += _T(",");
		sTo += recipient.GetRegularFormat ();
	}

	// Create the "Cc:" part of the header
	CString sCc;
	nCount = GetNumberOfRecipients (CC);
	for (i = 0; i < nCount; i++)
	{
		recipient = GetRecipient (i, CC);
		if (i)
			sCc += _T(",");
		sCc += recipient.GetRegularFormat ();
	}

	// No Bcc info added in header

	CString strSubject;
	encode ((char*)(LPCTSTR)m_sSubject, strSubject);

	// Stick everything together
	CString sBuf;
	if (sCc.GetLength ())
	{
		sBuf.Format (
			_T("From: %s\r\n")\
			_T("To: %s\r\n")\
			_T("Cc: %s\r\n")\
			_T("Subject: %s\r\n")\
			_T("Date: %s\r\n")/*\
			_T("X-Mailer: %s\r\n")*/,
			m_From.GetRegularFormat(),
			sTo, 
			sCc,
			strSubject,
			sDate/*,
			m_sXMailer*/);
	}
	else
	{
		sBuf.Format (
			_T("From: %s\r\n")\
			_T("To: %s\r\n")\
			_T("Subject: %s\r\n")\
			_T("Date: %s\r\n")/*\
			_T("X-Mailer: %s\r\n")*/,
			m_From.GetRegularFormat (),
			sTo, 
			strSubject,
			sDate/*,
			m_sXMailer*/);
	}

	// Add the optional Reply-To Field
	if (m_ReplyTo.m_sEmailAddress.GetLength ())
	{
		CString sReply;
		sReply.Format (_T("Reply-To: %s\r\n"), m_ReplyTo.GetRegularFormat ());
		sBuf += sReply;
	}

	// Add the optional fields if attachments are included
	if (m_Attachments.GetSize ())
	{
		sBuf += _T("MIME-Version: 1.0\r\n"
				"Content-type: multipart/mixed; "
				"boundary=\"#BOUNDARY#\"\r\n");
	}
	else
	{
		// avoid long textual message being automatically converted by the server:
		sBuf += _T("MIME-Version: 1.0\r\n"
				"Content-type: text/plain; "
				"charset=\"");
		sBuf += GetCharset ((char*)(LPCTSTR)m_sBody);
		sBuf += _T("\"\r\n"
				"Content-Transfer-Encoding: base64\r\n");
	}

	sBuf += _T("\r\n");

	return sBuf;
}

BOOL CSMTPMessage::ParseAdr (CString& str, CSMTPAddress& To)
{
	// Now divide the substring into friendly names and e-mail addresses
	int nMark = str.Find (_T('<'));
	if (nMark >= 0)
	{
		To.m_sFriendlyName = str.Left (nMark);
		int nMark2 = str.Find (_T('>'));
		if (nMark2 < nMark)
		{
			//An invalid string was sent in, fail the call
			SetLastError (ERROR_INVALID_DATA);
			TRACE (_T("An error occurred while parsing the recipients string\n"));
			return FALSE;
		}
		// End of mark at closing bracket or end of string
		nMark2 > -1 ? nMark2 = nMark2 : nMark2 = str.GetLength () - 1;
		To.m_sEmailAddress = str.Mid (nMark + 1, nMark2 - (nMark + 1));
	}
	else
	{
		To.m_sEmailAddress = str;
		To.m_sFriendlyName = _T("");
	}

	//Finally add the new recipient to the array of recipients
	To.m_sEmailAddress.TrimRight ();
	To.m_sEmailAddress.TrimLeft ();
	To.m_sFriendlyName.TrimRight ();
	To.m_sFriendlyName.TrimLeft ();
	return TRUE;
}

BOOL CSMTPMessage::AddMultipleRecipients (const CString& sRecipients, RECIPIENT_TYPE RecipientType)
{
	if (sRecipients.GetLength () == 0)
		return TRUE;

	// Loop through the whole string, adding recipients as they are encountered
	int length = sRecipients.GetLength ();
	TCHAR* buf = new TCHAR [length + 1];	// Allocate a work area (don't touch parameter itself)
	_tcscpy (buf, sRecipients);
	CString sTemp;
	CSMTPAddress To;

	for (int pos = 0, start = 0; pos <= length; pos++)
	{
		// Valid separators between addresses are ',' or ';'
		if (buf [pos] == _T(',') || buf [pos] == _T(';') || buf [pos] == 0)
		{
			buf [pos] = 0;	// Redundant when at the end of string, but who cares.
			sTemp = &buf [start];

			if (ParseAdr (sTemp, To) == FALSE)
			{
				delete[] buf;
				return FALSE;
			}

			if (To.m_sEmailAddress.GetLength ())
				AddRecipient (To, RecipientType);

			//Move on to the next position
			start = pos + 1;
		}
	}

	delete[] buf;
	return TRUE;
}

////////////////////////////////////////////////////////////////////////////////

CSMTPConnection::CSMTPConnection ()
{
	m_bConnected = FALSE;
	m_nLastCommandResponseCode = 0;
	#ifdef _DEBUG
		m_dwTimeout = 60000; //default timeout of 60 seconds when debugging
	#else
		m_dwTimeout = 2000;  //default timeout of 2 seconds for normal release code
	#endif
}

CSMTPConnection::~CSMTPConnection ()
{
	if (m_bConnected)
		Disconnect ();
}

BOOL CSMTPConnection::Connect (char* pszHostName, int nPort, char* szUserName, char* szPassword, int nTimeOut)
{
	if (m_bConnected)
		Disconnect ();

	if (pszHostName == NULL || *pszHostName == '\0')
		return FALSE;

	m_dwTimeout = nTimeOut <= 0 ? 2000 : (DWORD)nTimeOut;

	// Create the socket
	if (!m_SMTP.Create ())
	{
		m_sLastCommandResponse = _T("创建SOCKET网络组件失败!");
		return FALSE;
	}

	// Connect to the SMTP Host
	if (!m_SMTP.Connect (pszHostName, nPort))
	{
		m_SMTP.Close ();
		m_sLastCommandResponse.Format (_T("连接指定SMTP邮件服务器“%s”失败!"), pszHostName);
		return FALSE;
	}
	else
	{
		// We're now connected !!
		m_bConnected = TRUE;

		// check the response to the login
		if (!ReadResponse (220))
		{
			TRACE (_T("An unexpected SMTP login response was received\n"));
			Disconnect ();
			return FALSE;
		}

		// retreive the localhost name
		char sHostName [100];
		gethostname (sHostName, sizeof (sHostName));

		BOOL blHello = TRUE;
		if (!login (sHostName, szUserName, szPassword, &blHello))
		{
			m_sLastCommandResponse.Format (_T("登录指定SMTP邮件服务器“%s”失败!"), pszHostName);
			Disconnect ();
			return FALSE;
		}

		if (blHello)
		{
			CString sBuf;
			// Send the HELO command
			sBuf.Format(_T("HELO %s\r\n"), sHostName);
			LPCSTR pszData = (LPTSTR)(LPCTSTR)sBuf;
			int nCmdLength = strlen (pszData);
			if (!m_SMTP.Send (pszData, nCmdLength))
			{
				m_sLastCommandResponse = _T("发送数据失败!");
				Disconnect ();
				TRACE (_T("An unexpected error occurred while sending the HELO command\n"));
				return FALSE;
			}

			// check the response to the HELO command
			if (!ReadResponse (250))
			{
				Disconnect();
				TRACE(_T("An unexpected HELO response was received\n"));
				return FALSE;
			}
		}

		return TRUE;
	}
}

BOOL CSMTPConnection::login (char* pszHostName, char* szUserName, char* szPassword, BOOL* pblHello)
{
	if (szUserName == NULL || szPassword == NULL || *szUserName == '\0' || *szPassword == '\0')
		return TRUE;

	CString sBuf;
	// Send the HELO command
	sBuf.Format (_T("EHLO %s\r\n"), pszHostName);
	LPCSTR pszData = (LPTSTR)(LPCTSTR)sBuf;
	int nCmdLength = strlen (pszData);
	if (!m_SMTP.Send (pszData, nCmdLength))
	{
		TRACE (_T("An unexpected error occurred while sending the HELO command\n"));
		return FALSE;
	}

	// check the response to the HELO command
	if (!ReadResponse (250))
		return TRUE;

	*pblHello = FALSE;

	BOOL blLogin;
	unsigned char* pAuth = supportsExtension ("AUTH");
	if (pAuth == NULL)
	{
		pAuth = supportsExtension ("AUTH=LOGIN");
		blLogin = (pAuth != NULL);
	}
	else
		blLogin = FALSE;

	if (pAuth == NULL)
		return TRUE;

    if (blLogin || supportsAuthentication (pAuth, "LOGIN"))
	{
		char* szCmd = "AUTH LOGIN\r\n";
		if (!m_SMTP.Send (szCmd, strlen (szCmd)))
			return FALSE;

		if (ReadResponse (334))
		{
			int nSize;
			char* p = EncodeBase64 (szUserName, strlen (szUserName), &nSize);
			if (!m_SMTP.Send (p, nSize))
			{
				delete[] p;
				return FALSE;
			}
			delete[] p;

			if (ReadResponse (334))
			{
				p = EncodeBase64 (szPassword, strlen (szPassword), &nSize);
				if (!m_SMTP.Send (p, nSize))
				{
					delete[] p;
					return FALSE;
				}
				delete[] p;

				ReadResponse ();
			}
		}

		if (m_nLastCommandResponseCode != 235)
			return FALSE;
	}
	else if (supportsAuthentication (pAuth, "PLAIN"))
	{
		char* szCmd = "AUTH PLAIN\r\n";
		if (!m_SMTP.Send (szCmd, strlen (szCmd)))
			return FALSE;

		if (ReadResponse (334))
		{
			int nUserNameSize = strlen (szUserName);
			int nPassWordSize = strlen (szPassword);
			char* pb = new char [2 + nUserNameSize + nPassWordSize];
			*pb = 0;
			strcpy (pb + 1, szUserName);
			memcpy (pb + 1 + nUserNameSize + 1, szPassword, nPassWordSize);

			int nSize;
			char* p = EncodeBase64 (pb, 2 + nUserNameSize + nPassWordSize, &nSize);
			if (!m_SMTP.Send (p, nSize))
			{
				delete[] p;
				delete[] pb;
				return FALSE;
			}
			delete[] p;
			delete[] pb;

			ReadResponse ();
		}

		if (m_nLastCommandResponseCode != 235)
			return FALSE;
	}

	return TRUE;
}

BOOL CSMTPConnection::Disconnect ()
{
	BOOL bSuccess = FALSE;

	//disconnect from the SMTP server if connected 
	if (m_bConnected)
	{
		char sBuf [10];
		strcpy (sBuf, "QUIT\r\n");
		int nCmdLength = strlen (sBuf);
		if (!m_SMTP.Send (sBuf, nCmdLength))
			TRACE (_T("Failed in call to send QUIT command, GetLastError returns: %d\n"), GetLastError ());

		// Check the reponse
		bSuccess = ReadResponse (221);
		if (!bSuccess)
		{
			SetLastError (ERROR_BAD_COMMAND);
			TRACE (_T("An unexpected QUIT response was received\n"));
		}

		//Reset all the state variables
		m_bConnected = FALSE;
	}
	else
		TRACE(_T("Already disconnected from SMTP server, doing nothing\n"));
 
	//free up our socket
	m_SMTP.Close ();

	return bSuccess;
}

BOOL CSMTPConnection::SendMessage(CSMTPMessage& Message)
{
	if (!m_bConnected)
	{
		m_sLastCommandResponse = _T("未连接到SMTP服务器!");
		return FALSE;
	}

	//Must be sending to someone
	if (Message.GetNumberOfRecipients (CSMTPMessage::TO) + 
		 Message.GetNumberOfRecipients (CSMTPMessage::CC) + 
		 Message.GetNumberOfRecipients (CSMTPMessage::BCC) == 0)
	{
		m_sLastCommandResponse = _T("未指定发送地址!");
		return FALSE;
	}

	//Send the MAIL command
	CString sBuf;
	sBuf.Format (_T("MAIL FROM:<%s>\r\n"), Message.m_From.m_sEmailAddress);
	LPCSTR pszMailFrom = (LPTSTR)(LPCTSTR)sBuf;
	int nCmdLength = strlen (pszMailFrom);
	if (!m_SMTP.Send (pszMailFrom, nCmdLength))
	{
		m_sLastCommandResponse = _T("发送数据失败!");
		TRACE(_T("Failed in call to send MAIL command, GetLastError returns: %d\n"), GetLastError());
		return FALSE;
	}

	BOOL blOK = TRUE;

	while (TRUE)
	{
		//check the response to the MAIL command
		if (!ReadResponse (250))
		{
			SetLastError (ERROR_BAD_COMMAND);
			TRACE(_T("An unexpected MAIL response was received\n"));
			break;
		} 

		//Send the RCPT command, one for each recipient (includes the TO, CC & BCC recipients)

		CSMTPAddress recipient;
		//First the "To" recipients
		for (int i = 0; i < Message.GetNumberOfRecipients (CSMTPMessage::TO); i++)
		{
			recipient = Message.GetRecipient (i, CSMTPMessage::TO);
			if (!SendRCPTForRecipient (recipient))
			{
				blOK = FALSE;
				break;
			}
		}
		if (blOK == FALSE)  break;

		//Then the "CC" recipients
		for (i = 0; i < Message.GetNumberOfRecipients (CSMTPMessage::CC); i++)
		{
			recipient = Message.GetRecipient (i, CSMTPMessage::CC);
			if (!SendRCPTForRecipient (recipient))
			{
				blOK = FALSE;
				break;
			}
		}
		if (blOK == FALSE)  break;

		//Then the "BCC" recipients
		for (i = 0; i < Message.GetNumberOfRecipients (CSMTPMessage::BCC); i++)
		{
			recipient = Message.GetRecipient (i, CSMTPMessage::BCC);
			if (!SendRCPTForRecipient (recipient))
			{
				blOK = FALSE;
				break;
			}
		}
		if (blOK == FALSE)  break;

		//Send the DATA command
		char* pszDataCommand = "DATA\r\n";
		nCmdLength = strlen (pszDataCommand);
		if (!m_SMTP.Send (pszDataCommand, nCmdLength))
		{
			m_sLastCommandResponse = _T("发送数据失败!");
			TRACE(_T("Failed in call to send MAIL command, GetLastError returns: %d\n"), GetLastError());
			break;
		}

		//check the response to the DATA command
		if (!ReadResponse (354))
		{
			SetLastError (ERROR_BAD_COMMAND);
			TRACE(_T("An unexpected DATA response was received\n"));
			break;
		} 

		//Send the Header
		CString sHeader = Message.GetHeader ();
		char* pszHeader = (LPTSTR)(LPCTSTR)sHeader;
		nCmdLength = strlen (pszHeader);
		if (!m_SMTP.Send (pszHeader, nCmdLength))
		{
			m_sLastCommandResponse = _T("发送邮件头失败!");
			TRACE(_T("Failed in call to send the header, GetLastError returns: %d\n"), GetLastError());
			break;
		}

		//Send the Mime Header for the body
		if (Message.m_Attachments.GetSize())
		{
			CString sBodyHeader = _T("\r\n--#BOUNDARY#\r\n")\
				_T("Content-Type: text/plain; charset=\"");
			sBodyHeader += GetCharset ((char*)(LPCTSTR)Message.m_sBody);
			sBodyHeader += _T("\"\r\n")\
				_T("Content-Transfer-Encoding: base64\r\n\r\n");

			char* ps = (char*)(LPCTSTR)sBodyHeader;
			nCmdLength = strlen (ps);
			if (!m_SMTP.Send (ps, nCmdLength))
			{
				m_sLastCommandResponse = _T("发送数据失败!");
				TRACE(_T("Failed in call to send the body header, GetLastError returns: %d\n"), GetLastError());
				break;
			}
		}

		//Send the body
		char* pszBody = (LPTSTR)(LPCTSTR)Message.m_sBody;
		nCmdLength = strlen (pszBody);
		int nSize;
		char* p = EncodeBase64 (pszBody, nCmdLength, &nSize);
		if (!m_SMTP.Send (p, nSize))
		{
			m_sLastCommandResponse = _T("发送邮件内容失败!");
			TRACE(_T("Failed in call to send the body, GetLastError returns: %d\n"), GetLastError());
			delete[] p;
			break;
		}
		delete[] p;

		//Send all the attachments
		CSMTPAttachment* pAttachment;
		CString sContent;
		char* pszContent;

		int nCount = Message.m_Attachments.GetSize();
		for (i = 0; i < nCount; i++)
		{
			pAttachment = Message.m_Attachments.GetAt (i);

			//First send the Mime header for each attachment
			sContent.Format (_T("\r\n\r\n--#BOUNDARY#\r\n")\
					_T("Content-Type: application/octet-stream; name=%s\r\n")\
					_T("Content-Transfer-Encoding: base64\r\n")\
					_T("Content-Disposition: attachment; filename=%s\r\n\r\n"),
					pAttachment->GetFilename (), pAttachment->GetTitle ());

			pszContent = (LPTSTR)(LPCTSTR)sContent;
			nCmdLength = strlen (pszContent);
			if (!m_SMTP.Send (pszContent, nCmdLength))
			{
				m_sLastCommandResponse = _T("发送数据失败!");
				TRACE(_T("Failed in call to send Mime attachment header, GetLastError returns: %d\n"), GetLastError());
				blOK = FALSE;
				break;
			}

			//Then send the encoded attachment
			if (!m_SMTP.Send (pAttachment->GetEncodedBuffer (), pAttachment->GetEncodedSize ()))
			{
				m_sLastCommandResponse = _T("发送附件数据失败!");
				TRACE(_T("Failed in call to send the attachment, GetLastError returns: %d\n"), GetLastError());
				blOK = FALSE;
				break;
			}
		}
		if (blOK == FALSE)  break;

		//Send the final mime boundary
		if (Message.m_Attachments.GetSize())
		{
			char* pszFinalBoundary = "\r\n--#BOUNDARY#--";
			nCmdLength = strlen (pszFinalBoundary);
			if (!m_SMTP.Send (pszFinalBoundary, nCmdLength))
			{
				m_sLastCommandResponse = _T("发送数据失败!");
				TRACE(_T("Failed in call to send Mime attachment header, GetLastError returns: %d\n"), GetLastError());
				break;
			}
		}

		//Send the end of message indicator
		char* pszEOM = "\r\n.\r\n";
		nCmdLength = strlen (pszEOM);
		if (!m_SMTP.Send (pszEOM, nCmdLength))
		{
			m_sLastCommandResponse = _T("发送数据失败!");
			TRACE(_T("Failed in call to send end of message indicator, GetLastError returns: %d\n"), GetLastError());
			break;
		}

		//check the response to the End of Message command
		if (!ReadResponse (250))
		{
			SetLastError (ERROR_BAD_COMMAND);
			TRACE(_T("An unexpected end of message response was received\n"));
			break;
		} 

		return TRUE;
	}

	///////////////////////////////////////

	int nLastCommandResponseCode = m_nLastCommandResponseCode;
	CString sLastCommandResponse = m_sLastCommandResponse;

	//Send the reset of message indicator
	char* pszReset = "RSET\r\n";
	nCmdLength = strlen (pszReset);
	if (m_SMTP.Send (pszReset, nCmdLength))
		ReadResponse ();

	m_nLastCommandResponseCode = nLastCommandResponseCode;
	m_sLastCommandResponse = sLastCommandResponse;

	return FALSE;
}

BOOL CSMTPConnection::SendRCPTForRecipient (CSMTPAddress& recipient)
{
	ASSERT (recipient.m_sEmailAddress.GetLength ()); //must have an email address for this recipient

	CString sBuf;
	sBuf.Format (_T("RCPT TO:<%s>\r\n"), recipient.m_sEmailAddress);
	LPTSTR pszRCPT = (LPTSTR)(LPCTSTR)sBuf;

	int nCmdLength = strlen (pszRCPT);
	if (!m_SMTP.Send (pszRCPT, nCmdLength))
	{
		m_sLastCommandResponse = _T("发送数据失败!");
		TRACE(_T("Failed in call to send RCPT command, GetLastError returns: %d\n"), GetLastError());
		return FALSE;
	}

	//check the response to the RCPT command
	if (!ReadResponse (250))
	{
		SetLastError (ERROR_BAD_COMMAND);
		TRACE(_T("An unexpected RCPT response was received\n"));
		return FALSE;
	} 

	return TRUE;
}

BOOL CSMTPConnection::ReadResponse (int nExpectedCode)
{
	//must have been created first
	ASSERT (m_bConnected);

	m_nLastCommandResponseCode = -1;
	m_straryItemName.RemoveAll ();
	m_straryItemValue.RemoveAll ();
	m_sLastCommandResponse.Empty ();

	char buf [1024];

	//The local variables which will receive the data
	LPSTR pszRecvBuffer = buf;
	*pszRecvBuffer = '\0';
	int nBufSize = sizeof (buf);
	char* pszOverFlowBuffer = NULL;

	//retrieve the reponse using until we
	//get the terminator or a timeout occurs
	BOOL bReadible;
	int nReceived = 0;
	DWORD dwStartTicks = ::GetTickCount ();
	while (TRUE)
	{
		//Has the timeout occured
		if (::GetTickCount () - dwStartTicks > m_dwTimeout)
		{
			m_sLastCommandResponse = _T("接收数据超时!");
			pszRecvBuffer [nReceived] = '\0';
			SetLastError (WSAETIMEDOUT);
			break;
		}

		//check the socket for readability
		if (!m_SMTP.IsReadible (&bReadible))
		{
			m_sLastCommandResponse = _T("网络出错!");
			pszRecvBuffer [nReceived] = '\0';
			break;
		}
		else if (!bReadible) //no data to receive, just loop around
		{
			Sleep (250); //Sleep for a while before we loop around again
			continue;
		}

		//receive the data from the socket
		int nBufRemaining = nBufSize - nReceived - 1; //Allows allow one space for the NULL terminator
		if (nBufRemaining < 0)
			nBufRemaining = 0;

		int nData = m_SMTP.Receive (pszRecvBuffer + nReceived, nBufRemaining);

		//If an error occurred receiving the data
		if (nData == SOCKET_ERROR)
		{
			m_sLastCommandResponse = _T("网络出错!");
			//NULL terminate the data received
			pszRecvBuffer [nReceived] = '\0';
			break;
		}

		//Reset the idle timeout if data was received
		if (nData)
		{
			dwStartTicks = ::GetTickCount ();
			//Increment the count of data received
			nReceived += nData;
		}

		//NULL terminate the data received
		pszRecvBuffer [nReceived] = '\0';

		if (nBufRemaining == nData) //No space left in the current buffer
		{
			//Allocate the new receive buffer
			nBufSize += 4096; //Grow the buffer by the specified amount
			LPSTR pszNewBuf = new char [nBufSize];

			//copy the old contents over to the new buffer and assign 
			//the new buffer to the local variable used for retreiving 
			//from the socket
			strcpy (pszNewBuf, pszRecvBuffer);
			pszRecvBuffer = pszNewBuf;

			//delete the old buffer if it was allocated
			if (pszOverFlowBuffer)
				delete[] pszOverFlowBuffer;

			//Remember the overflow buffer for the next time around
			pszOverFlowBuffer = pszNewBuf;        
		}

		//Check to see if the terminator character(s) have been found
		char* p = strrchr (pszRecvBuffer, '\r');
		char* pEnd = pszRecvBuffer + nReceived;
		if (p == NULL)
			p = pEnd;
		p--;

		while (p >= pszRecvBuffer && *p != '\n')
			p--;
		p += 4;

		if (p < pEnd && *p == ' ') 
		{
			*p = '\0';
			m_nLastCommandResponseCode = atoi (p - 3);
			*p = ' ';
			break;
		}
	}

	if (m_nLastCommandResponseCode != -1)
	{
		char *ps = pszRecvBuffer, *p, *pn, *pb, *pNextLine;
		char* pEnd = pszRecvBuffer + nReceived;

		while (ps < pEnd)
		{
			p = strchr (ps, '\r');
			if (p == NULL)  break;

			pNextLine = p;

			if (p - ps > 4)
			{
				while (p > ps + 4 && (unsigned char)*(p - 1) <= 32)
					p--;
				*p = '\0';

				ps += 4;
				while (*ps != '\0' && (unsigned char)*ps <= 32)
					ps++;

				pn = strchr (ps, ' ');
				if (pn == NULL)
				{
					if (*ps != '\0')
					{
						m_straryItemName.Add (ps);
						m_straryItemValue.Add ("");
						// TRACE2("\r\n%s - %s", ps, "");
					}
				}
				else
				{
					pb = pn;
					while ((unsigned char)*(pb - 1) <= 32)
						pb--;
					*pb = '\0';
					m_straryItemName.Add (ps);

					pn++;
					while ((unsigned char)*pn <= 32)
						pn++;
					m_straryItemValue.Add (pn);

					// TRACE2("\r\n%s - %s", ps, pn);
				}
			}

			ps = pNextLine + 1;
			if (ps < pEnd && *ps == '\n')
				ps++;
		}
	}

	int nMaxIndex = m_straryItemName.GetUpperBound ();
	if (nMaxIndex >= 0)
	{
		m_sLastCommandResponse = m_straryItemName.GetAt (nMaxIndex);
		if (m_straryItemValue.GetAt (nMaxIndex).GetLength () > 0)
		{
			m_sLastCommandResponse += " ";
			m_sLastCommandResponse += m_straryItemValue.GetAt (nMaxIndex);
		}
	}
	else if (*pszRecvBuffer != '\0')
		m_sLastCommandResponse = pszRecvBuffer;

	//delete the old buffer if it was allocated
	if (pszOverFlowBuffer)
		delete [] pszOverFlowBuffer;

	if (m_nLastCommandResponseCode == -1)
	{
		if (m_sLastCommandResponse.IsEmpty ())
			m_sLastCommandResponse = _T("等待SMTP邮件服务器响应失败!");
		return FALSE;
	}

	if (m_nLastCommandResponseCode != nExpectedCode)
	{
		if (m_sLastCommandResponse.IsEmpty ())
			m_sLastCommandResponse = _T("SMTP邮件服务器返回失败信息!");
		return FALSE;
	}
	else
		return TRUE;
}

unsigned char* CSMTPConnection::supportsExtension (char* szName)
{
	int nCount = m_straryItemName.GetSize ();
	for (int i = 0; i < nCount; i++)
	{
		if (m_straryItemName.GetAt (i).CompareNoCase (szName) == 0)
			return (unsigned char*)(LPCTSTR)m_straryItemValue.GetAt (i);
	}

	return NULL;
}

BOOL CSMTPConnection::supportsAuthentication (unsigned char* ps, char* szAuthValue)
{
	unsigned char* p, ch;
	while (*ps != '\0')
	{
		p = ps;
		while (*p > ' ' && *p != '\0')
			p++;

		ch = *p;
		if (ch != '\0')
			*p = '\0';

		if (stricmp ((char*)ps, szAuthValue) == 0)
			return TRUE;

		if (ch == '\0')
			break;

		*p++ = ch;
		while (*p <= ' ' && *p != '\0')
			p++;
		ps = p;
	}

    return FALSE;
}

