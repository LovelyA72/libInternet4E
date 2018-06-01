
#ifndef __LIBFN_H
#define	__LIBFN_H

void fnConnectSmtpServer (PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf);
void fnDisconnectSmtpServer (PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf);
void fnAttachFile (PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf);
void fnAttachData (PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf);
void fnEmptyAttachment (PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf);
void fnSendMail (PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf);

void fnSetProxyName (PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf);
void fnGetHttpFile (PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf);
void fnConnectFTPServer (PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf);
void fnDisconnectFTPServer (PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf);
void fnGetFtpFile (PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf);
void fnPutFtpFile (PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf);
void fnDeleteFtpFile (PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf);
void fnRenameFtpFile (PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf);
void fnCreateFtpDir (PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf);
void fnRemoveFtpDir (PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf);
void fnSetCurrentFtpDir (PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf);
void fnGetCurrentFtpDir (PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf);
void fnListFtpDir (PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf);

void fnGetEntriesCount (PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf);
void fnGetEntryName (PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf);
void fnGetUserName (PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf);
void fnRasDial (PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf);
void fnIsOnline (PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf);
void fnRasHangUp (PMDATA_INF pRetData, INT nArgCount, PMDATA_INF pArgInf);

#endif

