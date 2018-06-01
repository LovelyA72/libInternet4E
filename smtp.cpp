// smtp.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "smtp.h"
#include "internet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CInternetApp theApp;

//////////////////////////////////////////////////////////////

BOOL MConnect (char* pszHostName, int nPort, char* szUserName, char* szPassword, int nTimeOut)
{
	return theApp.m_connect.Connect (pszHostName, nPort, szUserName, szPassword, nTimeOut);
}

void MDisconnect ()
{
	theApp.m_connect.Disconnect ();
}

BOOL MAttachFile (char* szFilename)
{
	CSMTPAttachment* pAttachment = new CSMTPAttachment (TRUE);
	if (pAttachment->Attach (szFilename, NULL) == FALSE ||
			theApp.m_message.AddAttachment (pAttachment) == FALSE)
	{
		delete pAttachment;
		return FALSE;
	}
	else
		return TRUE;
}

BOOL MAttachData (char* pData, int nDataSize, char* szTitle)
{
	CSMTPAttachment* pAttachment = new CSMTPAttachment (TRUE);
	if (pAttachment->Attach (pData, nDataSize, szTitle, szTitle) == FALSE ||
			theApp.m_message.AddAttachment (pAttachment) == FALSE)
	{
		delete pAttachment;
		return FALSE;
	}
	else
		return TRUE;
}

void MEmptyAttachment ()
{
	theApp.m_message.EmptyAllAttachment ();
}

char* _MSend (char* subject, char* body, char* to, char* cc, char* bcc, char* from, char* replyto)
{
	theApp.m_message.m_sSubject = subject;
	theApp.m_message.m_sBody = body;

	if (theApp.m_message.AddMultipleRecipients (CString (to), CSMTPMessage::TO) == FALSE ||
			theApp.m_message.AddMultipleRecipients (CString (cc), CSMTPMessage::CC) == FALSE ||
			theApp.m_message.AddMultipleRecipients (CString (bcc), CSMTPMessage::BCC) == FALSE)
		return "目的邮件地址格式错误!";

	CSMTPAddress adr;
	if (CSMTPMessage::ParseAdr (CString (from), adr) == FALSE)
		return "发信人邮件地址格式错误!";
	else if (adr.m_sEmailAddress.GetLength ())
		theApp.m_message.m_From = adr;

	if (CSMTPMessage::ParseAdr (CString (replyto), adr) == FALSE)
		return "回复邮件地址格式错误!";
	else if (adr.m_sEmailAddress.GetLength ())
		theApp.m_message.m_ReplyTo = adr;

	if (theApp.m_connect.SendMessage (theApp.m_message))
		return "";
	else
		return (char*)(LPCTSTR)theApp.m_connect.GetLastCommandResponse ();
}

char* MSend (char* subject, char* body, char* to, char* cc, char* bcc, char* from, char* replyto)
{
	char* szResult = _MSend (subject, body, to, cc, bcc, from, replyto);
	theApp.m_message.init ();
	return szResult;
}

