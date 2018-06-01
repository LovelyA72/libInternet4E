
#if !defined(_SMTP_H)
#define _SMTP_H

BOOL MConnect (char* pszHostName, int nPort = 25,
		char* szUserName = NULL, char* szPassword = NULL, int nTimeOut = 0);
void MDisconnect ();
BOOL MAttachFile (char* szFilename);
BOOL MAttachData (char* pData, int nDataSize, char* szTitle);
void MEmptyAttachment ();
char* MSend (char* subject, char* body, char* to, char* cc, char* bcc, char* from, char* replyto = NULL);

#endif
