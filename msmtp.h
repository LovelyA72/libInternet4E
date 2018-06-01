
#ifndef __SMTP_H__
#define __SMTP_H__

#ifndef __AFXTEMPL_H__
#pragma message("SMTP classes require afxtempl.h in your PCH")                                                                                
#endif

#ifndef _WINSOCKAPI_
#pragma message("SMTP classes require afxsock.h or winsock.h in your PCH")
#endif

#ifndef __AFXPRIV_H__
#pragma message("SMTP classes requires afxpriv.h in your PCH")
#endif
  

/////////////////////////////// Classes ///////////////////////////////////////


//Simple Socket wrapper class
class CSMTPSocket
{
public:
	//Constructors / Destructors
	CSMTPSocket ();
	~CSMTPSocket ();

	//methods
	BOOL Create ();
	BOOL Connect (char* pszHostAddress, int nPort);
	BOOL Send (LPCSTR pszBuf, int nBuf);
	void Close ();
	int Receive (LPSTR pszBuf, int nBuf);
	BOOL IsReadible (BOOL* pbReadible);

protected:
	BOOL Connect (const SOCKADDR* lpSockAddr, int nSockAddrLen);
	SOCKET m_hSocket;
};

//Encapsulation of an SMTP email address, used for recipients and in the From: field
class CSMTPAddress
{
public: 
	//Constructors / Destructors
	CSMTPAddress ();
	CSMTPAddress (const CSMTPAddress& address);
	CSMTPAddress (const CString& sAddress);
	CSMTPAddress (const CString& sFriendly, const CString& sAddress);
	CSMTPAddress& operator=(const CSMTPAddress& r);

	//Methods
	CString GetRegularFormat () const;

	//Data members
	CString m_sFriendlyName; //Would set it to contain something like "PJ Naughter"
	CString m_sEmailAddress; //Would set it to contains something like "pjn@indigo.ie"
};


//Encapsulation of an SMTP file attachment
class CSMTPAttachment
{
public:
	//Constructors / Destructors
	CSMTPAttachment (BOOL blMustDelete = FALSE);
	~CSMTPAttachment ();

	//methods
	BOOL Attach (char* sFilename, char* szTitle = NULL);
	BOOL Attach (char* pData, int nDataSize, char* szFileName, char* szTitle = NULL);
	const char* GetEncodedBuffer () const
	{
		return m_pszEncoded;
	}
	int GetEncodedSize () const
	{
		return m_nEncodedSize;
	}
	CString GetFilename () const
	{
		return m_sFilename;
	}
	CString GetTitle () const
	{
		return m_sTitle;
	}
	void SetTitle (const CString& sTitle)
	{
		m_sTitle = sTitle;
	}
	void Delete ()
	{
		if (m_blMustDelete)
			delete this;
	}

protected:
	CString m_sFilename;	//The filename you want to send
	CString m_sTitle;		//What it is to be known as when emailed
	char* m_pszEncoded;		//The encoded representation of the file
	int m_nEncodedSize;		//size of the encoded string

	BOOL m_blMustDelete;
};


class CSMTPConnection;

//Encapsulation of an SMTP message
class CSMTPMessage
{
public:
	// Enums
	enum RECIPIENT_TYPE { TO, CC, BCC };

	//Constructors / Destructors
	CSMTPMessage ();
	~CSMTPMessage ();

	void init ();

	//Recipient support
	int GetNumberOfRecipients (RECIPIENT_TYPE RecipientType = TO) const;
	int AddRecipient (CSMTPAddress& recipient, RECIPIENT_TYPE RecipientType = TO);
	void RemoveRecipient (int nIndex, RECIPIENT_TYPE RecipientType = TO);
	CSMTPAddress GetRecipient (int nIndex, RECIPIENT_TYPE RecipientType = TO) const;

	//Attachment support
	int GetNumberOfAttachments () const;
	BOOL AddAttachment (CSMTPAttachment* pAttachment);
	void RemoveAttachment (int nIndex);
	CSMTPAttachment* GetAttachment (int nIndex) const;
	void EmptyAllAttachment ();
	static BOOL ParseAdr (CString& str, CSMTPAddress& To);

	//Misc methods
	virtual CString GetHeader();
	BOOL AddMultipleRecipients (const CString& sRecipients, RECIPIENT_TYPE RecipientType);

	//Data Members
	CSMTPAddress m_From;
	CString m_sSubject;
	CString m_sBody;
	// CString m_sXMailer;
	CSMTPAddress m_ReplyTo;

protected:
	CArray<CSMTPAddress, CSMTPAddress&> m_ToRecipients;
	CArray<CSMTPAddress, CSMTPAddress&> m_CCRecipients;
	CArray<CSMTPAddress, CSMTPAddress&> m_BCCRecipients;
	CArray<CSMTPAttachment*, CSMTPAttachment*&> m_Attachments;

	friend class CSMTPConnection;
};


//The main class which encapsulates the SMTP connection
class CSMTPConnection
{
public:
	//Constructors / Destructors
	CSMTPConnection ();
	~CSMTPConnection ();

	//Methods
	BOOL Connect (char* pszHostName, int nPort = 25, char* szUserName = NULL, char* szPassword = NULL,
			int nTimeOut = 0);
	BOOL Disconnect ();
	CString GetLastCommandResponse () const
	{
		return m_sLastCommandResponse;
	}
	int GetLastCommandResponseCode () const
	{
		return m_nLastCommandResponseCode;
	}
	DWORD GetTimeout () const
	{
		return m_dwTimeout;
	}
	void SetTimeout (DWORD dwTimeout)
	{
		m_dwTimeout = dwTimeout;
	}
	BOOL SendMessage (CSMTPMessage& Message);

protected:
	BOOL SendRCPTForRecipient (CSMTPAddress& recipient);
	BOOL ReadResponse (int nExpectedCode = 0);
	BOOL login (char* pszHostName, char* szUserName, char* szPassword, BOOL* pblHello);
	unsigned char* supportsExtension (char* szName);
	BOOL supportsAuthentication (unsigned char* ps, char* szAuthValue);

	//////////////////////////////////////////////

	CSMTPSocket m_SMTP;
	BOOL m_bConnected;
	DWORD m_dwTimeout;

	//////////////////////////////////////////////

	CStringArray m_straryItemName, m_straryItemValue;
	int m_nLastCommandResponseCode;
	CString m_sLastCommandResponse;
};

#endif //__SMTP_H__

