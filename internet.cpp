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

//*** 命令定义信息:

static CMD_INFO s_CmdInfo[] =
{
//****** 连接发信服务器	** 0
	{
/*ccname*/	_WT("连接发信服务器"),
/*egname*/	_WT("ConnectSmtpServer"),
/*explain*/	_WT("连接到指定的SMTP邮件发送服务器，成功返回真，失败返回假"),
/*category*/1,		// 类别索引从1开始.
/*state*/	NULL,
/*ret*/		SDT_BOOL,
/*syscmd*/	NULL,
/*level*/	LVL_SIMPLE,
/*bmp inx*/	0,		// 图像索引从1开始.
/*bmp num*/	0,
/*ArgCount*/5,
/*arg lp*/	&s_ArgInfo [0],
	},
//****** 断开发信服务器	** 1
	{
/*ccname*/	_WT("断开发信服务器"),
/*egname*/	_WT("DisconnectSmtpServer"),
/*explain*/	_WT("断开通过“连接发信服务器”命令建立的到SMTP邮件发送服务器的连接"),
/*category*/1,		// 类别索引从1开始.
/*state*/	NULL,
/*ret*/		_SDT_NULL,
/*syscmd*/	NULL,
/*level*/	LVL_SIMPLE,
/*bmp inx*/	0,		// 图像索引从1开始.
/*bmp num*/	0,
/*ArgCount*/0,
/*arg lp*/	NULL,
	},
//****** 添加附件文件	** 2
	{
/*ccname*/	_WT("添加附件文件"),
/*egname*/	_WT("AttachFile"),
/*explain*/	_WT("添加指定的文件附件到即将发送的邮件中，成功返回真，失败返回假"),
/*category*/1,		// 类别索引从1开始.
/*state*/	NULL,
/*ret*/		SDT_BOOL,
/*syscmd*/	NULL,
/*level*/	LVL_SIMPLE,
/*bmp inx*/	0,		// 图像索引从1开始.
/*bmp num*/	0,
/*ArgCount*/1,
/*arg lp*/	&s_ArgInfo [5],
	},
//****** 添加附件数据	** 3
	{
/*ccname*/	_WT("添加附件数据"),
/*egname*/	_WT("AttachData"),
/*explain*/	_WT("添加附件数据到即将发送的邮件中，成功返回真，失败返回假"),
/*category*/1,		// 类别索引从1开始.
/*state*/	NULL,
/*ret*/		SDT_BOOL,
/*syscmd*/	NULL,
/*level*/	LVL_SIMPLE,
/*bmp inx*/	0,		// 图像索引从1开始.
/*bmp num*/	0,
/*ArgCount*/2,
/*arg lp*/	&s_ArgInfo [6],
	},
//****** 清除所有附件	** 4
	{
/*ccname*/	_WT("清除所有附件"),
/*egname*/	_WT("EmptyAttachment"),
/*explain*/	_WT("清除所有使用“添加附件文件”和“添加附件数据”"
			"命令添加到即将发送邮件中的附件数据"),
/*category*/1,		// 类别索引从1开始.
/*state*/	NULL,
/*ret*/		_SDT_NULL,
/*syscmd*/	NULL,
/*level*/	LVL_SIMPLE,
/*bmp inx*/	0,		// 图像索引从1开始.
/*bmp num*/	0,
/*ArgCount*/0,
/*arg lp*/	NULL,
	},
//****** 发送邮件	** 5
	{
/*ccname*/	_WT("发送邮件"),
/*egname*/	_WT("SendMail"),
/*explain*/	_WT("发送邮件到指定信箱，注意邮件中包含所有使用“添加附件文件”和"
			"“添加附件数据”命令添加的附件。成功返回空文本，失败返回具体错误信息文本"),
/*category*/1,		// 类别索引从1开始.
/*state*/	NULL,
/*ret*/		SDT_TEXT,
/*syscmd*/	NULL,
/*level*/	LVL_SIMPLE,
/*bmp inx*/	0,		// 图像索引从1开始.
/*bmp num*/	0,
/*ArgCount*/7,
/*arg lp*/	&s_ArgInfo [8],
	},

//****** 置代理服务器	** 6
	{
/*ccname*/	_WT("置代理服务器"),
/*egname*/	_WT("SetProxyName"),
/*explain*/	_WT("设置本库此后的HTTP和FTP新建连接所使用的代理服务器信息。"
				"对FTP操作而言，目前不支持代理服务器验证，将忽略用户名和密码参数。"),
/*category*/2,		// 类别索引从1开始.
/*state*/	NULL,
/*ret*/		_SDT_NULL,
/*syscmd*/	NULL,
/*level*/	LVL_SIMPLE,
/*bmp inx*/	0,		// 图像索引从1开始.
/*bmp num*/	0,
/*ArgCount*/3, //1,
/*arg lp*/	s_ArgInfo_SetProxy,//&s_ArgInfo [15],
	},
//****** HTTP读文件	** 7
	{
/*ccname*/	_WT("HTTP读文件"),
/*egname*/	_WT("GetHttpFile"),
/*explain*/	_WT("获取互联网上指定HTTP地址处的网页或数据文件，返回文件内容字节集"),
/*category*/2,		// 类别索引从1开始.
/*state*/	NULL,
/*ret*/		SDT_BIN,
/*syscmd*/	NULL,
/*level*/	LVL_SIMPLE,
/*bmp inx*/	0,		// 图像索引从1开始.
/*bmp num*/	0,
/*ArgCount*/1,
/*arg lp*/	&s_ArgInfo [16],
	},
//****** 连接FTP服务器	** 8
	{
/*ccname*/	_WT("连接FTP服务器"),
/*egname*/	_WT("ConnectFTPServer"),
/*explain*/	_WT("连接到互联网上指定地址处的FTP服务器。在进行所有FTP操作"
				"之前必须首先成功连接FTP服务器。成功返回真，失败返回假"),
/*category*/2,		// 类别索引从1开始.
/*state*/	NULL,
/*ret*/		SDT_BOOL,
/*syscmd*/	NULL,
/*level*/	LVL_SIMPLE,
/*bmp inx*/	0,		// 图像索引从1开始.
/*bmp num*/	0,
/*ArgCount*/5,
/*arg lp*/	&s_ArgInfo [17],
	},
//****** 断开FTP服务器	** 9
	{
/*ccname*/	_WT("断开FTP服务器"),
/*egname*/	_WT("DisconnectFTPServer"),
/*explain*/	_WT("断开通过“连接FTP服务器”命令建立的到互联网上FTP服务器的连接，"
		"本命令执行后不能再进行FTP操作"),
/*category*/2,		// 类别索引从1开始.
/*state*/	NULL,
/*ret*/		_SDT_NULL,
/*syscmd*/	NULL,
/*level*/	LVL_SIMPLE,
/*bmp inx*/	0,		// 图像索引从1开始.
/*bmp num*/	0,
/*ArgCount*/0,
/*arg lp*/	NULL,
	},
//****** FTP文件下载	** 10
	{
/*ccname*/	_WT("FTP文件下载"),
/*egname*/	_WT("GetFtpFile"),
/*explain*/	_WT("将FTP服务器上的指定文件下载到本地，成功返回真，失败返回假"),
/*category*/2,		// 类别索引从1开始.
/*state*/	NULL,
/*ret*/		SDT_BOOL,
/*syscmd*/	NULL,
/*level*/	LVL_SIMPLE,
/*bmp inx*/	0,		// 图像索引从1开始.
/*bmp num*/	0,
/*ArgCount*/3,
/*arg lp*/	&s_ArgInfo [22],
	},
//****** FTP文件上传	** 11
	{
/*ccname*/	_WT("FTP文件上传"),
/*egname*/	_WT("PutFtpFile"),
/*explain*/	_WT("将本地文件上传到FTP服务器上的指定位置，成功返回真，失败返回假"),
/*category*/2,		// 类别索引从1开始.
/*state*/	NULL,
/*ret*/		SDT_BOOL,
/*syscmd*/	NULL,
/*level*/	LVL_SIMPLE,
/*bmp inx*/	0,		// 图像索引从1开始.
/*bmp num*/	0,
/*ArgCount*/3,
/*arg lp*/	&s_ArgInfo [25],
	},
//****** FTP删除文件	** 12
	{
/*ccname*/	_WT("FTP删除文件"),
/*egname*/	_WT("DeleteFtpFile"),
/*explain*/	_WT("删除FTP服务器上的指定文件。成功返回真，失败返回假"),
/*category*/2,		// 类别索引从1开始.
/*state*/	NULL,
/*ret*/		SDT_BOOL,
/*syscmd*/	NULL,
/*level*/	LVL_SIMPLE,
/*bmp inx*/	0,		// 图像索引从1开始.
/*bmp num*/	0,
/*ArgCount*/1,
/*arg lp*/	&s_ArgInfo [28],
	},
//****** FTP文件改名	** 13
	{
/*ccname*/	_WT("FTP文件改名"),
/*egname*/	_WT("RenameFtpFile"),
/*explain*/	_WT("更改FTP服务器上指定文件的名称。成功返回真，失败返回假"),
/*category*/2,		// 类别索引从1开始.
/*state*/	NULL,
/*ret*/		SDT_BOOL,
/*syscmd*/	NULL,
/*level*/	LVL_SIMPLE,
/*bmp inx*/	0,		// 图像索引从1开始.
/*bmp num*/	0,
/*ArgCount*/2,
/*arg lp*/	&s_ArgInfo [42],
	},
//****** FTP创建目录	** 14
	{
/*ccname*/	_WT("FTP创建目录"),
/*egname*/	_WT("CreateFtpDir"),
/*explain*/	_WT("在FTP服务器上指定位置处创建新的目录。成功返回真，失败返回假"),
/*category*/2,		// 类别索引从1开始.
/*state*/	NULL,
/*ret*/		SDT_BOOL,
/*syscmd*/	NULL,
/*level*/	LVL_SIMPLE,
/*bmp inx*/	0,		// 图像索引从1开始.
/*bmp num*/	0,
/*ArgCount*/1,
/*arg lp*/	&s_ArgInfo [29],
	},
//****** FTP删除目录	** 15
	{
/*ccname*/	_WT("FTP删除目录"),
/*egname*/	_WT("RemoveFtpDir"),
/*explain*/	_WT("删除FTP服务器上的指定目录。成功返回真，失败返回假"),
/*category*/2,		// 类别索引从1开始.
/*state*/	NULL,
/*ret*/		SDT_BOOL,
/*syscmd*/	NULL,
/*level*/	LVL_SIMPLE,
/*bmp inx*/	0,		// 图像索引从1开始.
/*bmp num*/	0,
/*ArgCount*/1,
/*arg lp*/	&s_ArgInfo [29],
	},
//****** FTP置现行目录	** 16
	{
/*ccname*/	_WT("FTP置现行目录"),
/*egname*/	_WT("SetCurrentFtpDir"),
/*explain*/	_WT("设置FTP服务器上的当前目录，设置后"
		"可以在其他FTP命令中使用相对路径来指定文件。成功返回真，失败返回假"),
/*category*/2,		// 类别索引从1开始.
/*state*/	NULL,
/*ret*/		SDT_BOOL,
/*syscmd*/	NULL,
/*level*/	LVL_SIMPLE,
/*bmp inx*/	0,		// 图像索引从1开始.
/*bmp num*/	0,
/*ArgCount*/1,
/*arg lp*/	&s_ArgInfo [29],
	},
//****** FTP取现行目录	** 17
	{
/*ccname*/	_WT("FTP取现行目录"),
/*egname*/	_WT("GetCurrentFtpDir"),
/*explain*/	_WT("返回FTP服务器上的当前目录。成功返回目录路径，失败返回空文本"),
/*category*/2,		// 类别索引从1开始.
/*state*/	NULL,
/*ret*/		SDT_TEXT,
/*syscmd*/	NULL,
/*level*/	LVL_SIMPLE,
/*bmp inx*/	0,		// 图像索引从1开始.
/*bmp num*/	0,
/*ArgCount*/0,
/*arg lp*/	NULL,
	},
//****** FTP目录列表	** 18
	{
/*ccname*/	_WT("FTP目录列表"),
/*egname*/	_WT("ListFtpDir"),
/*explain*/	_WT("返回FTP服务器上指定目录内的所有匹配文件和子目录信息。"
		"成功返回被找到的文件和子目录的数目，失败返回 0 "),
/*category*/2,		// 类别索引从1开始.
/*state*/	NULL,
/*ret*/		SDT_INT,
/*syscmd*/	NULL,
/*level*/	LVL_SIMPLE,
/*bmp inx*/	0,		// 图像索引从1开始.
/*bmp num*/	0,
/*ArgCount*/6,
/*arg lp*/	&s_ArgInfo [30],
	},

//****** 取拨号连接数	** 19
	{
/*ccname*/	_WT("取拨号连接数"),
/*egname*/	_WT("GetEntriesCount"),
/*explain*/	_WT("返回用户在Windows系统中已建立的拨号连接配置数目"),
/*category*/3,		// 类别索引从1开始.
/*state*/	NULL,
/*ret*/		SDT_INT,
/*syscmd*/	NULL,
/*level*/	LVL_SIMPLE,
/*bmp inx*/	0,		// 图像索引从1开始.
/*bmp num*/	0,
/*ArgCount*/0,
/*arg lp*/	NULL,
	},
//****** 取连接名称	** 20
	{
/*ccname*/	_WT("取连接名称"),
/*egname*/	_WT("GetEntryName"),
/*explain*/	_WT("返回指定拨号连接配置的名称"),
/*category*/3,		// 类别索引从1开始.
/*state*/	NULL,
/*ret*/		SDT_TEXT,
/*syscmd*/	NULL,
/*level*/	LVL_SIMPLE,
/*bmp inx*/	0,		// 图像索引从1开始.
/*bmp num*/	0,
/*ArgCount*/1,
/*arg lp*/	&s_ArgInfo [36],
	},
//****** 取用户帐号	** 21
	{
/*ccname*/	_WT("取用户帐号"),
/*egname*/	_WT("GetUserName"),
/*explain*/	_WT("返回指定拨号连接配置所使用的用户帐号名称"),
/*category*/3,		// 类别索引从1开始.
/*state*/	NULL,
/*ret*/		SDT_TEXT,
/*syscmd*/	NULL,
/*level*/	LVL_SIMPLE,
/*bmp inx*/	0,		// 图像索引从1开始.
/*bmp num*/	0,
/*ArgCount*/1,
/*arg lp*/	&s_ArgInfo [36],
	},
//****** 拨号	** 22
	{
/*ccname*/	_WT("拨号"),
/*egname*/	_WT("RasDial"),
/*explain*/	_WT("开始拨号并试图建立网络连接。成功返回真，失败返回假。注意如果"
		"“是否等待连接完毕”参数为假，则即使本命令返回真，也并不意味着马上就可"
		"以使用该网络连接，必须等到“是否已在线”命令返回真时才可以。如果"
		"“是否等待连接完毕”参数为真，则本命令会一直等待到连接操作执行完毕后"
		"才会返回。另外，建议在执行本命令前调用“是否已在线”命令查看当前是否"
		"已经连接到网络上，如果已经连接，则不需要再执行本命令"),
/*category*/3,		// 类别索引从1开始.
/*state*/	NULL,
/*ret*/		SDT_BOOL,
/*syscmd*/	NULL,
/*level*/	LVL_SIMPLE,
/*bmp inx*/	0,		// 图像索引从1开始.
/*bmp num*/	0,
/*ArgCount*/5,
/*arg lp*/	&s_ArgInfo [37],
	},
//****** 是否已在线	** 23
	{
/*ccname*/	_WT("是否已在线"),
/*egname*/	_WT("IsOnline"),
/*explain*/	_WT("如果当前已经存在可用的网络连接(即已连接到网络上)，返回真，否则返回假"),
/*category*/3,		// 类别索引从1开始.
/*state*/	NULL,
/*ret*/		SDT_BOOL,
/*syscmd*/	NULL,
/*level*/	LVL_SIMPLE,
/*bmp inx*/	0,		// 图像索引从1开始.
/*bmp num*/	0,
/*ArgCount*/0,
/*arg lp*/	NULL,
	},
//****** 挂断	** 24
	{
/*ccname*/	_WT("挂断"),
/*egname*/	_WT("RasHangUp"),
/*explain*/	_WT("挂断当前拨号网络连接。一旦不再需要"
		"拨号网络连接，就可以使用本命令将连接断开"),
/*category*/3,		// 类别索引从1开始.
/*state*/	NULL,
/*ret*/		_SDT_NULL,
/*syscmd*/	NULL,
/*level*/	LVL_SIMPLE,
/*bmp inx*/	0,		// 图像索引从1开始.
/*bmp num*/	0,
/*ArgCount*/0,
/*arg lp*/	NULL,
	},

};

///////////////////////////////////

//*** 命令实现信息:

PFN_EXECUTE_CMD s_RunFunc [] =	// 索引应与s_CmdInfo中的命令定义顺序对应
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
//////////// 库定义开始
static LIB_INFO s_LibInfo =
{
/*Lib Format Ver*/		LIB_FORMAT_VER,		// 保留未用。

// 本支持库的GUID串：
// guid: {707CA373-2247-4F6C-A841-F0E224F4B620}
#define		LI_LIB_GUID_STR	"707ca37322474f6ca841f0e224f4b620"
/*guid str*/			_T (LI_LIB_GUID_STR),

/*m_nMajorVersion*/		1,
/*m_nMinorVersion*/		1,

/*m_nBuildNumber*/		0,  // 1: 2.5;  2: 3.0;  50: 3.39;  51: 4.11
		// 构建版本号，无需对此版本号作任何处理。
		//   本版本号仅用作区分相同正式版本号的库（譬如仅仅修改了几个 BUG，
		// 不值得升级正式版本的）。任何公布过给用户使用的版本其此版本
		// 号都应该不一样。
		//   赋值时应该顺序递增。

/*m_nRqSysMajorVer*/		3,
/*m_nRqSysMinorVer*/		0,
/*m_nRqSysKrnlLibMajorVer*/	3,
/*m_nRqSysKrnlLibMinorVer*/	0,

/*name*/				_T ("互联网支持库"),
#ifndef __COMPILE_LANG_VER
    #error *** lang.h must be imported!
#endif
/*lang*/				__COMPILE_LANG_VER,
/*explain*/				_WT("本支持库用作实现对常用互联网际协议的支持，"
							"如SMTP、HTTP、FTP等"),  //***
/*dwState*/				NULL,

/*szAuthor*/	_WT("大有吴涛易语言软件公司"),
/*szZipCode*/	_WT("116001"),
/*szAddress*/	_WT("辽宁省大连市中山区人民路55号亚太国际金融中心"),
/*szPhoto*/		_WT("+86(0411)88995831"),
/*szFax*/		_WT("+86(0411)88995834"),
/*szEmail*/		_WT("service@dywt.com.cn"),
/*szHomePage*/	_WT("http://dywt.com.cn"),
/*szOther*/		_WT("祝您一帆风顺，心想事成！"),

/*type count*/			0,
/*PLIB_DATA_TYPE_INFO*/	NULL,

/*CategoryCount*/ 3,	// 加了类别需加此值。
/*category*/_WT("0000邮件发送\0"	// 类别说明表每项为一字符串,前四位数字表示图象索引号(从1开始,0无).
				"0000HTTP及FTP操作\0"
				"0000拨号上网\0"
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

