// internet.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "internet.h"
#include "..\libinf.h"
#include "..\krnl\fnshare.h"
#include "libfn.h"
#include "libarg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//
//	Note!
//
//		If this DLL is dynamically linked against the MFC
//		DLLs, any functions exported from this DLL which
//		call into MFC must have the AFX_MANAGE_STATE macro
//		added at the very beginning of the function.
//
//		For example:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// normal function body here
//		}
//
//		It is very important that this macro appear in each
//		function, prior to any calls into MFC.  This means that
//		it must appear as the first statement within the 
//		function, even before any object variable declarations
//		as their constructors may generate calls into the MFC
//		DLL.
//
//		Please see MFC Technical Notes 33 and 58 for additional
//		details.
//

/////////////////////////////////////////////////////////////////////////////
// CInternetApp

BEGIN_MESSAGE_MAP(CInternetApp, CWinApp)
	//{{AFX_MSG_MAP(CInternetApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInternetApp construction

CInternetApp::CInternetApp()
{
	m_hRasConn = NULL;
	m_hStatusWnd = NULL;
}

BOOL CInternetApp::InitInstance()
{
	if (!AfxSocketInit())
		return FALSE;

	return TRUE;
}

void MHangUp (HRASCONN hRasConn);

int CInternetApp::ExitInstance() 
{
	m_message.init ();
	m_message.EmptyAllAttachment ();
	m_connect.Disconnect ();

	m_net.Close ();

	if (m_hRasConn != NULL)
	{
		m_hStatusWnd = NULL;

		RASCONNSTATUS stat;
		memset (&stat, 0, sizeof (stat));
		stat.dwSize = sizeof (RASCONNSTATUS);

		if (RasGetConnectStatus (m_hRasConn, &stat) != 0 ||
				stat.rasconnstate != RASCS_Connected)
			MHangUp (m_hRasConn);

		m_hRasConn = NULL;
	}

	return CWinApp::ExitInstance();
}


/////////////////////////////////////////////////////////////////////////////
// The one and only CInternetApp object

CInternetApp theApp;


///////////////////////////////////

extern ARG_INFO s_ArgInfo [];
extern ARG_INFO s_ArgInfo_SetProxy[];

//*** �������Ϣ:

static CMD_INFO s_CmdInfo[] =
{
//****** ���ӷ��ŷ�����	** 0
	{
/*ccname*/	_WT("���ӷ��ŷ�����"),
/*egname*/	_WT("ConnectSmtpServer"),
/*explain*/	_WT("���ӵ�ָ����SMTP�ʼ����ͷ��������ɹ������棬ʧ�ܷ��ؼ�"),
/*category*/1,		// ���������1��ʼ.
/*state*/	NULL,
/*ret*/		SDT_BOOL,
/*syscmd*/	NULL,
/*level*/	LVL_SIMPLE,
/*bmp inx*/	0,		// ͼ��������1��ʼ.
/*bmp num*/	0,
/*ArgCount*/5,
/*arg lp*/	&s_ArgInfo [0],
	},
//****** �Ͽ����ŷ�����	** 1
	{
/*ccname*/	_WT("�Ͽ����ŷ�����"),
/*egname*/	_WT("DisconnectSmtpServer"),
/*explain*/	_WT("�Ͽ�ͨ�������ӷ��ŷ�������������ĵ�SMTP�ʼ����ͷ�����������"),
/*category*/1,		// ���������1��ʼ.
/*state*/	NULL,
/*ret*/		_SDT_NULL,
/*syscmd*/	NULL,
/*level*/	LVL_SIMPLE,
/*bmp inx*/	0,		// ͼ��������1��ʼ.
/*bmp num*/	0,
/*ArgCount*/0,
/*arg lp*/	NULL,
	},
//****** ��Ӹ����ļ�	** 2
	{
/*ccname*/	_WT("��Ӹ����ļ�"),
/*egname*/	_WT("AttachFile"),
/*explain*/	_WT("���ָ�����ļ��������������͵��ʼ��У��ɹ������棬ʧ�ܷ��ؼ�"),
/*category*/1,		// ���������1��ʼ.
/*state*/	NULL,
/*ret*/		SDT_BOOL,
/*syscmd*/	NULL,
/*level*/	LVL_SIMPLE,
/*bmp inx*/	0,		// ͼ��������1��ʼ.
/*bmp num*/	0,
/*ArgCount*/1,
/*arg lp*/	&s_ArgInfo [5],
	},
//****** ��Ӹ�������	** 3
	{
/*ccname*/	_WT("��Ӹ�������"),
/*egname*/	_WT("AttachData"),
/*explain*/	_WT("��Ӹ������ݵ��������͵��ʼ��У��ɹ������棬ʧ�ܷ��ؼ�"),
/*category*/1,		// ���������1��ʼ.
/*state*/	NULL,
/*ret*/		SDT_BOOL,
/*syscmd*/	NULL,
/*level*/	LVL_SIMPLE,
/*bmp inx*/	0,		// ͼ��������1��ʼ.
/*bmp num*/	0,
/*ArgCount*/2,
/*arg lp*/	&s_ArgInfo [6],
	},
//****** ������и���	** 4
	{
/*ccname*/	_WT("������и���"),
/*egname*/	_WT("EmptyAttachment"),
/*explain*/	_WT("�������ʹ�á���Ӹ����ļ����͡���Ӹ������ݡ�"
			"������ӵ����������ʼ��еĸ�������"),
/*category*/1,		// ���������1��ʼ.
/*state*/	NULL,
/*ret*/		_SDT_NULL,
/*syscmd*/	NULL,
/*level*/	LVL_SIMPLE,
/*bmp inx*/	0,		// ͼ��������1��ʼ.
/*bmp num*/	0,
/*ArgCount*/0,
/*arg lp*/	NULL,
	},
//****** �����ʼ�	** 5
	{
/*ccname*/	_WT("�����ʼ�"),
/*egname*/	_WT("SendMail"),
/*explain*/	_WT("�����ʼ���ָ�����䣬ע���ʼ��а�������ʹ�á���Ӹ����ļ�����"
			"����Ӹ������ݡ�������ӵĸ������ɹ����ؿ��ı���ʧ�ܷ��ؾ��������Ϣ�ı�"),
/*category*/1,		// ���������1��ʼ.
/*state*/	NULL,
/*ret*/		SDT_TEXT,
/*syscmd*/	NULL,
/*level*/	LVL_SIMPLE,
/*bmp inx*/	0,		// ͼ��������1��ʼ.
/*bmp num*/	0,
/*ArgCount*/7,
/*arg lp*/	&s_ArgInfo [8],
	},

//****** �ô��������	** 6
	{
/*ccname*/	_WT("�ô��������"),
/*egname*/	_WT("SetProxyName"),
/*explain*/	_WT("���ñ���˺��HTTP��FTP�½�������ʹ�õĴ����������Ϣ��"
				"��FTP�������ԣ�Ŀǰ��֧�ִ����������֤���������û��������������"),
/*category*/2,		// ���������1��ʼ.
/*state*/	NULL,
/*ret*/		_SDT_NULL,
/*syscmd*/	NULL,
/*level*/	LVL_SIMPLE,
/*bmp inx*/	0,		// ͼ��������1��ʼ.
/*bmp num*/	0,
/*ArgCount*/3, //1,
/*arg lp*/	s_ArgInfo_SetProxy,//&s_ArgInfo [15],
	},
//****** HTTP���ļ�	** 7
	{
/*ccname*/	_WT("HTTP���ļ�"),
/*egname*/	_WT("GetHttpFile"),
/*explain*/	_WT("��ȡ��������ָ��HTTP��ַ������ҳ�������ļ��������ļ������ֽڼ�"),
/*category*/2,		// ���������1��ʼ.
/*state*/	NULL,
/*ret*/		SDT_BIN,
/*syscmd*/	NULL,
/*level*/	LVL_SIMPLE,
/*bmp inx*/	0,		// ͼ��������1��ʼ.
/*bmp num*/	0,
/*ArgCount*/1,
/*arg lp*/	&s_ArgInfo [16],
	},
//****** ����FTP������	** 8
	{
/*ccname*/	_WT("����FTP������"),
/*egname*/	_WT("ConnectFTPServer"),
/*explain*/	_WT("���ӵ���������ָ����ַ����FTP���������ڽ�������FTP����"
				"֮ǰ�������ȳɹ�����FTP���������ɹ������棬ʧ�ܷ��ؼ�"),
/*category*/2,		// ���������1��ʼ.
/*state*/	NULL,
/*ret*/		SDT_BOOL,
/*syscmd*/	NULL,
/*level*/	LVL_SIMPLE,
/*bmp inx*/	0,		// ͼ��������1��ʼ.
/*bmp num*/	0,
/*ArgCount*/5,
/*arg lp*/	&s_ArgInfo [17],
	},
//****** �Ͽ�FTP������	** 9
	{
/*ccname*/	_WT("�Ͽ�FTP������"),
/*egname*/	_WT("DisconnectFTPServer"),
/*explain*/	_WT("�Ͽ�ͨ��������FTP��������������ĵ���������FTP�����������ӣ�"
		"������ִ�к����ٽ���FTP����"),
/*category*/2,		// ���������1��ʼ.
/*state*/	NULL,
/*ret*/		_SDT_NULL,
/*syscmd*/	NULL,
/*level*/	LVL_SIMPLE,
/*bmp inx*/	0,		// ͼ��������1��ʼ.
/*bmp num*/	0,
/*ArgCount*/0,
/*arg lp*/	NULL,
	},
//****** FTP�ļ�����	** 10
	{
/*ccname*/	_WT("FTP�ļ�����"),
/*egname*/	_WT("GetFtpFile"),
/*explain*/	_WT("��FTP�������ϵ�ָ���ļ����ص����أ��ɹ������棬ʧ�ܷ��ؼ�"),
/*category*/2,		// ���������1��ʼ.
/*state*/	NULL,
/*ret*/		SDT_BOOL,
/*syscmd*/	NULL,
/*level*/	LVL_SIMPLE,
/*bmp inx*/	0,		// ͼ��������1��ʼ.
/*bmp num*/	0,
/*ArgCount*/3,
/*arg lp*/	&s_ArgInfo [22],
	},
//****** FTP�ļ��ϴ�	** 11
	{
/*ccname*/	_WT("FTP�ļ��ϴ�"),
/*egname*/	_WT("PutFtpFile"),
/*explain*/	_WT("�������ļ��ϴ���FTP�������ϵ�ָ��λ�ã��ɹ������棬ʧ�ܷ��ؼ�"),
/*category*/2,		// ���������1��ʼ.
/*state*/	NULL,
/*ret*/		SDT_BOOL,
/*syscmd*/	NULL,
/*level*/	LVL_SIMPLE,
/*bmp inx*/	0,		// ͼ��������1��ʼ.
/*bmp num*/	0,
/*ArgCount*/3,
/*arg lp*/	&s_ArgInfo [25],
	},
//****** FTPɾ���ļ�	** 12
	{
/*ccname*/	_WT("FTPɾ���ļ�"),
/*egname*/	_WT("DeleteFtpFile"),
/*explain*/	_WT("ɾ��FTP�������ϵ�ָ���ļ����ɹ������棬ʧ�ܷ��ؼ�"),
/*category*/2,		// ���������1��ʼ.
/*state*/	NULL,
/*ret*/		SDT_BOOL,
/*syscmd*/	NULL,
/*level*/	LVL_SIMPLE,
/*bmp inx*/	0,		// ͼ��������1��ʼ.
/*bmp num*/	0,
/*ArgCount*/1,
/*arg lp*/	&s_ArgInfo [28],
	},
//****** FTP�ļ�����	** 13
	{
/*ccname*/	_WT("FTP�ļ�����"),
/*egname*/	_WT("RenameFtpFile"),
/*explain*/	_WT("����FTP��������ָ���ļ������ơ��ɹ������棬ʧ�ܷ��ؼ�"),
/*category*/2,		// ���������1��ʼ.
/*state*/	NULL,
/*ret*/		SDT_BOOL,
/*syscmd*/	NULL,
/*level*/	LVL_SIMPLE,
/*bmp inx*/	0,		// ͼ��������1��ʼ.
/*bmp num*/	0,
/*ArgCount*/2,
/*arg lp*/	&s_ArgInfo [42],
	},
//****** FTP����Ŀ¼	** 14
	{
/*ccname*/	_WT("FTP����Ŀ¼"),
/*egname*/	_WT("CreateFtpDir"),
/*explain*/	_WT("��FTP��������ָ��λ�ô������µ�Ŀ¼���ɹ������棬ʧ�ܷ��ؼ�"),
/*category*/2,		// ���������1��ʼ.
/*state*/	NULL,
/*ret*/		SDT_BOOL,
/*syscmd*/	NULL,
/*level*/	LVL_SIMPLE,
/*bmp inx*/	0,		// ͼ��������1��ʼ.
/*bmp num*/	0,
/*ArgCount*/1,
/*arg lp*/	&s_ArgInfo [29],
	},
//****** FTPɾ��Ŀ¼	** 15
	{
/*ccname*/	_WT("FTPɾ��Ŀ¼"),
/*egname*/	_WT("RemoveFtpDir"),
/*explain*/	_WT("ɾ��FTP�������ϵ�ָ��Ŀ¼���ɹ������棬ʧ�ܷ��ؼ�"),
/*category*/2,		// ���������1��ʼ.
/*state*/	NULL,
/*ret*/		SDT_BOOL,
/*syscmd*/	NULL,
/*level*/	LVL_SIMPLE,
/*bmp inx*/	0,		// ͼ��������1��ʼ.
/*bmp num*/	0,
/*ArgCount*/1,
/*arg lp*/	&s_ArgInfo [29],
	},
//****** FTP������Ŀ¼	** 16
	{
/*ccname*/	_WT("FTP������Ŀ¼"),
/*egname*/	_WT("SetCurrentFtpDir"),
/*explain*/	_WT("����FTP�������ϵĵ�ǰĿ¼�����ú�"
		"����������FTP������ʹ�����·����ָ���ļ����ɹ������棬ʧ�ܷ��ؼ�"),
/*category*/2,		// ���������1��ʼ.
/*state*/	NULL,
/*ret*/		SDT_BOOL,
/*syscmd*/	NULL,
/*level*/	LVL_SIMPLE,
/*bmp inx*/	0,		// ͼ��������1��ʼ.
/*bmp num*/	0,
/*ArgCount*/1,
/*arg lp*/	&s_ArgInfo [29],
	},
//****** FTPȡ����Ŀ¼	** 17
	{
/*ccname*/	_WT("FTPȡ����Ŀ¼"),
/*egname*/	_WT("GetCurrentFtpDir"),
/*explain*/	_WT("����FTP�������ϵĵ�ǰĿ¼���ɹ�����Ŀ¼·����ʧ�ܷ��ؿ��ı�"),
/*category*/2,		// ���������1��ʼ.
/*state*/	NULL,
/*ret*/		SDT_TEXT,
/*syscmd*/	NULL,
/*level*/	LVL_SIMPLE,
/*bmp inx*/	0,		// ͼ��������1��ʼ.
/*bmp num*/	0,
/*ArgCount*/0,
/*arg lp*/	NULL,
	},
//****** FTPĿ¼�б�	** 18
	{
/*ccname*/	_WT("FTPĿ¼�б�"),
/*egname*/	_WT("ListFtpDir"),
/*explain*/	_WT("����FTP��������ָ��Ŀ¼�ڵ�����ƥ���ļ�����Ŀ¼��Ϣ��"
		"�ɹ����ر��ҵ����ļ�����Ŀ¼����Ŀ��ʧ�ܷ��� 0 "),
/*category*/2,		// ���������1��ʼ.
/*state*/	NULL,
/*ret*/		SDT_INT,
/*syscmd*/	NULL,
/*level*/	LVL_SIMPLE,
/*bmp inx*/	0,		// ͼ��������1��ʼ.
/*bmp num*/	0,
/*ArgCount*/6,
/*arg lp*/	&s_ArgInfo [30],
	},

//****** ȡ����������	** 19
	{
/*ccname*/	_WT("ȡ����������"),
/*egname*/	_WT("GetEntriesCount"),
/*explain*/	_WT("�����û���Windowsϵͳ���ѽ����Ĳ�������������Ŀ"),
/*category*/3,		// ���������1��ʼ.
/*state*/	NULL,
/*ret*/		SDT_INT,
/*syscmd*/	NULL,
/*level*/	LVL_SIMPLE,
/*bmp inx*/	0,		// ͼ��������1��ʼ.
/*bmp num*/	0,
/*ArgCount*/0,
/*arg lp*/	NULL,
	},
//****** ȡ��������	** 20
	{
/*ccname*/	_WT("ȡ��������"),
/*egname*/	_WT("GetEntryName"),
/*explain*/	_WT("����ָ�������������õ�����"),
/*category*/3,		// ���������1��ʼ.
/*state*/	NULL,
/*ret*/		SDT_TEXT,
/*syscmd*/	NULL,
/*level*/	LVL_SIMPLE,
/*bmp inx*/	0,		// ͼ��������1��ʼ.
/*bmp num*/	0,
/*ArgCount*/1,
/*arg lp*/	&s_ArgInfo [36],
	},
//****** ȡ�û��ʺ�	** 21
	{
/*ccname*/	_WT("ȡ�û��ʺ�"),
/*egname*/	_WT("GetUserName"),
/*explain*/	_WT("����ָ����������������ʹ�õ��û��ʺ�����"),
/*category*/3,		// ���������1��ʼ.
/*state*/	NULL,
/*ret*/		SDT_TEXT,
/*syscmd*/	NULL,
/*level*/	LVL_SIMPLE,
/*bmp inx*/	0,		// ͼ��������1��ʼ.
/*bmp num*/	0,
/*ArgCount*/1,
/*arg lp*/	&s_ArgInfo [36],
	},
//****** ����	** 22
	{
/*ccname*/	_WT("����"),
/*egname*/	_WT("RasDial"),
/*explain*/	_WT("��ʼ���Ų���ͼ�����������ӡ��ɹ������棬ʧ�ܷ��ؼ١�ע�����"
		"���Ƿ�ȴ�������ϡ�����Ϊ�٣���ʹ��������棬Ҳ������ζ�����ϾͿ�"
		"��ʹ�ø��������ӣ�����ȵ����Ƿ������ߡ��������ʱ�ſ��ԡ����"
		"���Ƿ�ȴ�������ϡ�����Ϊ�棬�������һֱ�ȴ������Ӳ���ִ����Ϻ�"
		"�Ż᷵�ء����⣬������ִ�б�����ǰ���á��Ƿ������ߡ�����鿴��ǰ�Ƿ�"
		"�Ѿ����ӵ������ϣ�����Ѿ����ӣ�����Ҫ��ִ�б�����"),
/*category*/3,		// ���������1��ʼ.
/*state*/	NULL,
/*ret*/		SDT_BOOL,
/*syscmd*/	NULL,
/*level*/	LVL_SIMPLE,
/*bmp inx*/	0,		// ͼ��������1��ʼ.
/*bmp num*/	0,
/*ArgCount*/5,
/*arg lp*/	&s_ArgInfo [37],
	},
//****** �Ƿ�������	** 23
	{
/*ccname*/	_WT("�Ƿ�������"),
/*egname*/	_WT("IsOnline"),
/*explain*/	_WT("�����ǰ�Ѿ����ڿ��õ���������(�������ӵ�������)�������棬���򷵻ؼ�"),
/*category*/3,		// ���������1��ʼ.
/*state*/	NULL,
/*ret*/		SDT_BOOL,
/*syscmd*/	NULL,
/*level*/	LVL_SIMPLE,
/*bmp inx*/	0,		// ͼ��������1��ʼ.
/*bmp num*/	0,
/*ArgCount*/0,
/*arg lp*/	NULL,
	},
//****** �Ҷ�	** 24
	{
/*ccname*/	_WT("�Ҷ�"),
/*egname*/	_WT("RasHangUp"),
/*explain*/	_WT("�Ҷϵ�ǰ�����������ӡ�һ��������Ҫ"
		"�����������ӣ��Ϳ���ʹ�ñ�������ӶϿ�"),
/*category*/3,		// ���������1��ʼ.
/*state*/	NULL,
/*ret*/		_SDT_NULL,
/*syscmd*/	NULL,
/*level*/	LVL_SIMPLE,
/*bmp inx*/	0,		// ͼ��������1��ʼ.
/*bmp num*/	0,
/*ArgCount*/0,
/*arg lp*/	NULL,
	},

};

///////////////////////////////////

//*** ����ʵ����Ϣ:

PFN_EXECUTE_CMD s_RunFunc [] =	// ����Ӧ��s_CmdInfo�е������˳���Ӧ
{
	fnConnectSmtpServer,
	fnDisconnectSmtpServer,
	fnAttachFile,
	fnAttachData,
	fnEmptyAttachment,
	fnSendMail,

	fnSetProxyName,
	fnGetHttpFile,
	fnConnectFTPServer,
	fnDisconnectFTPServer,
	fnGetFtpFile,
	fnPutFtpFile,
	fnDeleteFtpFile,
	fnRenameFtpFile,
	fnCreateFtpDir,
	fnRemoveFtpDir,
	fnSetCurrentFtpDir,
	fnGetCurrentFtpDir,
	fnListFtpDir,

	fnGetEntriesCount,
	fnGetEntryName,
	fnGetUserName,
	fnRasDial,
	fnIsOnline,
	fnRasHangUp,
};

/////////////////////////////////////////////////////////////////////////////

PFN_NOTIFY_SYS g_fnNotifySys = NULL;

/////////////////////////////////

INT WINAPI ProcessNotifyLib (INT nMsg, DWORD dwParam1, DWORD dwParam2)
{
	ENTRY_LIB_FUNCTION

	INT nRet = NR_OK;

	switch (nMsg)
	{
	case NL_SYS_NOTIFY_FUNCTION:
		g_fnNotifySys = (PFN_NOTIFY_SYS)dwParam1;
		break;
	default:
		nRet = NR_ERR;
		break;
	}

	return nRet;
}

INT g_nLastNotifyResult;

INT WINAPI NotifySys (INT nMsg, DWORD dwParam1, DWORD dwParam2)
{
	ASSERT (g_fnNotifySys != NULL);
	if (g_fnNotifySys != NULL)
		return g_nLastNotifyResult = g_fnNotifySys (nMsg, dwParam1, dwParam2);
	else
		return g_nLastNotifyResult = 0;
}

#include "..\krnl\fnshare.cpp"

/////////////////////////////////////////////////////////////////////////////

static char* s_szInfo = "This is a runtime library file for EPL applications. The EPL is a software development environment. For details please visit www.dywt.com.cn/info";
//////////// �ⶨ�忪ʼ
static LIB_INFO s_LibInfo =
{
/*Lib Format Ver*/		LIB_FORMAT_VER,		// ����δ�á�

// ��֧�ֿ��GUID����
// guid: {707CA373-2247-4F6C-A841-F0E224F4B620}
#define		LI_LIB_GUID_STR	"707ca37322474f6ca841f0e224f4b620"
/*guid str*/			_T (LI_LIB_GUID_STR),

/*m_nMajorVersion*/		1,
/*m_nMinorVersion*/		1,

/*m_nBuildNumber*/		0,  // 1: 2.5;  2: 3.0;  50: 3.39;  51: 4.11
		// �����汾�ţ�����Դ˰汾�����κδ���
		//   ���汾�Ž�����������ͬ��ʽ�汾�ŵĿ⣨Ʃ������޸��˼��� BUG��
		// ��ֵ��������ʽ�汾�ģ����κι��������û�ʹ�õİ汾��˰汾
		// �Ŷ�Ӧ�ò�һ����
		//   ��ֵʱӦ��˳�������

/*m_nRqSysMajorVer*/		3,
/*m_nRqSysMinorVer*/		0,
/*m_nRqSysKrnlLibMajorVer*/	3,
/*m_nRqSysKrnlLibMinorVer*/	0,

/*name*/				_T ("������֧�ֿ�"),
#ifndef __COMPILE_LANG_VER
    #error *** lang.h must be imported!
#endif
/*lang*/				__COMPILE_LANG_VER,
/*explain*/				_WT("��֧�ֿ�����ʵ�ֶԳ��û�������Э���֧�֣�"
							"��SMTP��HTTP��FTP��"),  //***
/*dwState*/				NULL,

/*szAuthor*/	_WT("�������������������˾"),
/*szZipCode*/	_WT("116001"),
/*szAddress*/	_WT("����ʡ��������ɽ������·55����̫���ʽ�������"),
/*szPhoto*/		_WT("+86(0411)88995831"),
/*szFax*/		_WT("+86(0411)88995834"),
/*szEmail*/		_WT("service@dywt.com.cn"),
/*szHomePage*/	_WT("http://dywt.com.cn"),
/*szOther*/		_WT("ף��һ����˳�������³ɣ�"),

/*type count*/			0,
/*PLIB_DATA_TYPE_INFO*/	NULL,

/*CategoryCount*/ 3,	// ���������Ӵ�ֵ��
/*category*/_WT("0000�ʼ�����\0"	// ���˵����ÿ��Ϊһ�ַ���,ǰ��λ���ֱ�ʾͼ��������(��1��ʼ,0��).
				"0000HTTP��FTP����\0"
				"0000��������\0"
				"\0"),
/*CmdCount*/				sizeof (s_CmdInfo) / sizeof (s_CmdInfo [0]),
/*BeginCmd*/				s_CmdInfo,
/*pCmdsFunc*/               s_RunFunc,
/*pfnRunAddInFn*/			NULL,
/*szzAddInFnInfo*/			NULL,

/*pfnNotify*/				ProcessNotifyLib,

/*pfnRunSuperTemplate*/		NULL,
/*szzSuperTemplateInfo*/	NULL,

/*nLibConstCount*/			0,
/*pLibConst*/				NULL,

/*szzDependFiles*/			NULL,
};

PLIB_INFO WINAPI GetNewInf ()
{
	return &s_LibInfo;
}

