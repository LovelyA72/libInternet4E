
#include "stdafx.h"
#include "..\libinf.h"
#include "..\krnl\krnllib.h"
#include "internet.h"

ARG_INFO s_ArgInfo[] =
{
//****** 连接发信服务器	**	0
	{
/*name*/	_WT("发信邮件服务器地址"),
/*explain*/	_WT("指定用作发送邮件的SMTP邮件服务器地址"),
/*bmp inx*/	0,
/*bmp num*/	0,
/*type*/	SDT_TEXT,
/*default*/	0,
/*state*/	NULL,
	}, {
/*name*/	_WT("端口号"),
/*explain*/	_WT("指定欲连接到SMTP邮件服务器上的端口号，"
		"如果本参数被省略，默认值为标准 25 端口"),
/*bmp inx*/	0,
/*bmp num*/	0,
/*type*/	SDT_INT,
/*default*/	0,
/*state*/	AS_DEFAULT_VALUE_IS_EMPTY,
	}, {
/*name*/	_WT("用户名"),
/*explain*/	_WT("指定在SMTP邮件服务器上的用户帐号名称，"
		"如果本参数被省略，默认值为空文本，即无需登录"),
/*bmp inx*/	0,
/*bmp num*/	0,
/*type*/	SDT_TEXT,
/*default*/	0,
/*state*/	AS_DEFAULT_VALUE_IS_EMPTY,
	}, {
/*name*/	_WT("密码"),
/*explain*/	_WT("指定在SMTP邮件服务器上的用户帐号密码，"
		"如果本参数被省略，默认值为空文本，即无需登录"),
/*bmp inx*/	0,
/*bmp num*/	0,
/*type*/	SDT_TEXT,
/*default*/	0,
/*state*/	AS_DEFAULT_VALUE_IS_EMPTY,
	}, {
/*name*/	_WT("最长等待时间"),
/*explain*/	_WT("指定在发信过程中等待SMTP邮件服务器响应的最大时间，单位为毫秒，"
		"如果本参数被省略，默认值为 2000 毫秒"),
/*bmp inx*/	0,
/*bmp num*/	0,
/*type*/	SDT_INT,
/*default*/	0,
/*state*/	AS_DEFAULT_VALUE_IS_EMPTY,
	},

//****** 添加附件文件	**	5
	{
/*name*/	_WT("欲添加的附件文件"),
/*explain*/	NULL,
/*bmp inx*/	0,
/*bmp num*/	0,
/*type*/	SDT_TEXT,
/*default*/	0,
/*state*/	NULL,
	},

//****** 添加附件数据	**	6
	{
/*name*/	_WT("欲添加的附件数据"),
/*explain*/	NULL,
/*bmp inx*/	0,
/*bmp num*/	0,
/*type*/	SDT_BIN,
/*default*/	0,
/*state*/	NULL,
	}, {
/*name*/	_WT("附件标题"),
/*explain*/	NULL,
/*bmp inx*/	0,
/*bmp num*/	0,
/*type*/	SDT_TEXT,
/*default*/	0,
/*state*/	NULL,
	},

//****** 发送邮件	**	8
	{
/*name*/	_WT("主题"),
/*explain*/	NULL,
/*bmp inx*/	0,
/*bmp num*/	0,
/*type*/	SDT_TEXT,
/*default*/	0,
/*state*/	NULL,
	}, {
/*name*/	_WT("正文"),
/*explain*/	NULL,
/*bmp inx*/	0,
/*bmp num*/	0,
/*type*/	SDT_TEXT,
/*default*/	0,
/*state*/	NULL,
	}, {
/*name*/	_WT("收信人邮件地址"),
/*explain*/	_WT("可以为地址列表并包括友好名称，示例如："
			"“客服〈service@dywt.com.cn〉;售前〈sale@dywt.com.cn〉” 或："
			"“service@dywt.com.cn;sale@dywt.com.cn”都可以，多地址之间用逗号或分号分隔，"
			"友好名称后的邮件地址用半角尖括号括住"),
/*bmp inx*/	0,
/*bmp num*/	0,
/*type*/	SDT_TEXT,
/*default*/	0,
/*state*/	NULL,
	}, {
/*name*/	_WT("抄送邮件地址"),
/*explain*/	_WT("地址格式等同于“收信人邮件地址”，如果本参数被省略，默认为空文本"),
/*bmp inx*/	0,
/*bmp num*/	0,
/*type*/	SDT_TEXT,
/*default*/	0,
/*state*/	AS_DEFAULT_VALUE_IS_EMPTY,
	}, {
/*name*/	_WT("暗送邮件地址"),
/*explain*/	_WT("地址格式等同于“收信人邮件地址”，如果本参数被省略，默认为空文本"),
/*bmp inx*/	0,
/*bmp num*/	0,
/*type*/	SDT_TEXT,
/*default*/	0,
/*state*/	AS_DEFAULT_VALUE_IS_EMPTY,
	}, {
/*name*/	_WT("发信人邮件地址"),
/*explain*/	_WT("只能为单个邮件地址，可以包括友好名称，示例如："
			"“客服〈service@dywt.com.cn〉”或“service@dywt.com.cn”"),
/*bmp inx*/	0,
/*bmp num*/	0,
/*type*/	SDT_TEXT,
/*default*/	0,
/*state*/	NULL,
	}, {
/*name*/	_WT("回复邮件地址"),
/*explain*/	_WT("指定收件人回复此邮件的地址。如果被省略，默认为与发信人邮件地址相同"),
/*bmp inx*/	0,
/*bmp num*/	0,
/*type*/	SDT_TEXT,
/*default*/	0,
/*state*/	AS_DEFAULT_VALUE_IS_EMPTY,
	},

//****** 置代理服务器	**	15
	{
/*name*/	_WT("代理服务器名称"),
/*explain*/	_WT("如果要取消代理服务器设置，请提供空文本。"
		"如果本参数被省略，默认值为空文本"),
/*bmp inx*/	0,
/*bmp num*/	0,
/*type*/	SDT_TEXT,
/*default*/	0,
/*state*/	AS_DEFAULT_VALUE_IS_EMPTY,
	},

//****** HTTP读文件	**	16
	{
/*name*/	_WT("HTTP文件地址"),
/*explain*/	_WT("本参数指定欲读取文件在互联网上的HTTP地址（即URL）"),
/*bmp inx*/	0,
/*bmp num*/	0,
/*type*/	SDT_TEXT,
/*default*/	0,
/*state*/	NULL,
	},

//****** 连接FTP服务器	**	17
	{
/*name*/	_WT("FTP服务器地址"),
/*explain*/	_WT("本参数指定FTP服务器在互联网上的地址"),
/*bmp inx*/	0,
/*bmp num*/	0,
/*type*/	SDT_TEXT,
/*default*/	0,
/*state*/	NULL,
	}, {
/*name*/	_WT("用户名"),
/*explain*/	_WT("本参数提供用作通过FTP服务器用户验证的用户名称，如果被省略，"
		"默认值为匿名用户名“anonymous”"),
/*bmp inx*/	0,
/*bmp num*/	0,
/*type*/	SDT_TEXT,
/*default*/	0,
/*state*/	AS_DEFAULT_VALUE_IS_EMPTY,
	}, {
/*name*/	_WT("密码"),
/*explain*/	_WT("本参数提供用作通过FTP服务器用户验证的密码，如果被省略，"
		"默认值为空文本"),
/*bmp inx*/	0,
/*bmp num*/	0,
/*type*/	SDT_TEXT,
/*default*/	0,
/*state*/	AS_DEFAULT_VALUE_IS_EMPTY,
	}, {
/*name*/	_WT("端口号"),
/*explain*/	_WT("本参数提供欲连接到FTP服务器上的端口号，如果被省略，"
		"默认值为标准端口 21 "),
/*bmp inx*/	0,
/*bmp num*/	0,
/*type*/	SDT_INT,
/*default*/	0,
/*state*/	AS_DEFAULT_VALUE_IS_EMPTY,
	}, {
/*name*/	_WT("是否使用被动语义"),
/*explain*/	_WT("如果本参数被省略，默认值为假"),
/*bmp inx*/	0,
/*bmp num*/	0,
/*type*/	SDT_BOOL,
/*default*/	0,
/*state*/	AS_DEFAULT_VALUE_IS_EMPTY,
	},

//****** FTP下载文件	** 22
	{
/*name*/	_WT("欲下载的FTP文件"),
/*explain*/	_WT("本参数指定在FTP服务器上欲下载文件的路径"),
/*bmp inx*/	0,
/*bmp num*/	0,
/*type*/	SDT_TEXT,
/*default*/	0,
/*state*/	NULL,
	}, {
/*name*/	_WT("本地文件名"),
/*explain*/	_WT("本参数指定被下载文件的本地保存位置"),
/*bmp inx*/	0,
/*bmp num*/	0,
/*type*/	SDT_TEXT,
/*default*/	0,
/*state*/	NULL,
	}, {
/*name*/	_WT("是否使用ASCII码方式"),
/*explain*/	_WT("指定是否使用ASCII编码方式传递文件数据。如果本参数被省略，默认值为假，即使用二进制方式传送"),
/*bmp inx*/	0,
/*bmp num*/	0,
/*type*/	SDT_BOOL,
/*default*/	0,
/*state*/	AS_DEFAULT_VALUE_IS_EMPTY,
	},

//****** FTP上传文件	** 25
	{
/*name*/	_WT("欲上传的本地文件"),
/*explain*/	_WT("本参数指定欲上传到FTP服务器上本地文件的路径"),
/*bmp inx*/	0,
/*bmp num*/	0,
/*type*/	SDT_TEXT,
/*default*/	0,
/*state*/	NULL,
	}, {
/*name*/	_WT("FTP文件路径"),
/*explain*/	_WT("本参数指定上传到FTP服务器上文件的保存位置"),
/*bmp inx*/	0,
/*bmp num*/	0,
/*type*/	SDT_TEXT,
/*default*/	0,
/*state*/	NULL,
	}, {
/*name*/	_WT("是否使用ASCII码方式"),
/*explain*/	_WT("指定是否使用ASCII编码方式传递文件数据。如果本参数被省略，默认值为假，即使用二进制方式传送"),
/*bmp inx*/	0,
/*bmp num*/	0,
/*type*/	SDT_BOOL,
/*default*/	0,
/*state*/	AS_DEFAULT_VALUE_IS_EMPTY,
	},

//****** FTP删除文件	** 28
	{
/*name*/	_WT("欲删除的FTP文件"),
/*explain*/	_WT("本参数指定在FTP服务器上欲删除文件的路径"),
/*bmp inx*/	0,
/*bmp num*/	0,
/*type*/	SDT_TEXT,
/*default*/	0,
/*state*/	NULL,
	},

//****** FTP创建目录、FTP删除目录、FTP置现行目录	** 29
	{
/*name*/	_WT("FTP目录路径"),
/*explain*/	NULL,
/*bmp inx*/	0,
/*bmp num*/	0,
/*type*/	SDT_TEXT,
/*default*/	0,
/*state*/	NULL,
	},
//****** FTP目录列表	** 30
	{
/*name*/	_WT("欲列出的文件"),
/*explain*/	_WT("可以包含路径，可以使用多字符 (*) 和单字符 (?) 通配符来指定多重文件及目录"),
/*bmp inx*/	0,
/*bmp num*/	0,
/*type*/	SDT_TEXT,
/*default*/	0,
/*state*/	NULL,
	}, {
/*name*/	_WT("欲列出文件的属性"),
/*explain*/	_WT("参数值可以为以下常量值或其和： 1、#只读文件； 2、#隐藏文件；"
		" 4、#系统文件； 16、#子目录； 32、#存档文件 。"
		"通过将这些常量值加起来可以一次设置多个文件属性。如果省略本参数，"
		"默认为搜寻所有子目录和文件"),
/*bmp inx*/	0,
/*bmp num*/	0,
/*type*/	SDT_INT,
/*default*/	0,
/*state*/	AS_DEFAULT_VALUE_IS_EMPTY,
	}, {
/*name*/	_WT("存放文件名的数组变量"),
/*explain*/	_WT("在命令执行完毕后，本变量数组内被顺序填入所找到的匹配文件和子目录名称。"
		"变量数组内原有数据被全部销毁，变量数组的维数被自动调整为所找到的文件数目。"
		"如果本参数被省略，默认值为空"),
/*bmp inx*/	0,
/*bmp num*/	0,
/*type*/	SDT_TEXT,
/*default*/	0,
/*state*/	AS_RECEIVE_VAR_ARRAY | AS_DEFAULT_VALUE_IS_EMPTY,
	}, {
/*name*/	_WT("存放文件属性的数组变量"),
/*explain*/	_WT("在命令执行完毕后，本变量数组内被顺序填入所找到的匹配文件的属性，"
		"并与文件名数组成员一一对应。此属性值由以下常量或其和构成： 1、#只读文件； "
		"2、#隐藏文件；  4、#系统文件； 16、#子目录；32、#存档文件 。要判断是否"
		"设置了某个属性，在返回值与想要得知的属性值之间使用“位与”命令进行比较。"
		"如果所得的结果不为零，则表示设置了这个属性值。变量数组内原有数据被全部销"
		"毁，变量数组的维数被自动调整为所找到的文件数目。如果本参数被省略，默认值为空"),
/*bmp inx*/	0,
/*bmp num*/	0,
/*type*/	SDT_INT,
/*default*/	0,
/*state*/	AS_RECEIVE_VAR_ARRAY | AS_DEFAULT_VALUE_IS_EMPTY,
	}, {
/*name*/	_WT("存放文件尺寸的数组变量"),
/*explain*/	_WT("在命令执行完毕后，本变量数组内被顺序填入所找到的匹配文件的尺寸，"
		"并与文件名数组成员一一对应。变量数组内原有数据被全部销"
		"毁，变量数组的维数被自动调整为所找到的文件数目。如果本参数被省略，默认值为空"),
/*bmp inx*/	0,
/*bmp num*/	0,
/*type*/	SDT_INT,
/*default*/	0,
/*state*/	AS_RECEIVE_VAR_ARRAY | AS_DEFAULT_VALUE_IS_EMPTY,
	}, {
/*name*/	_WT("存放文件时间的数组变量"),
/*explain*/	_WT("在命令执行完毕后，本变量数组内被顺序填入所找到的匹配文件的最后修改时间，"
		"并与文件名数组成员一一对应。此文件时间即FTP服务器返回的文件时间，不做任何时区转换。"
		"变量数组内原有数据被全部销毁，变量数组的维数被自动调整为所找到的文件数目。如果本参数被省略，默认值为空"),
/*bmp inx*/	0,
/*bmp num*/	0,
/*type*/	SDT_DATE_TIME,
/*default*/	0,
/*state*/	AS_RECEIVE_VAR_ARRAY | AS_DEFAULT_VALUE_IS_EMPTY,
	},

//****** 取连接名称、取用户帐号、取帐号密码	** 36
	{
/*name*/	_WT("拨号网络索引"),
/*explain*/	_WT("指定获取哪一个拨号网络配置信息，索引值从 0 开始，"
		"最大值为“取拨号连接数”命令的返回值减一"),
/*bmp inx*/	0,
/*bmp num*/	0,
/*type*/	SDT_INT,
/*default*/	0,
/*state*/	NULL,
	},

//****** 拨号	** 37
	{
/*name*/	_WT("电话号码"),
/*explain*/	_WT("指定欲拨出的电话号码，如果欲使用已经存在的用户拨号网络配置中"
			"的电话号码，请使用“@n”格式文本，其中n为指定拨号网络配置的索引数值，"
			"从0开始。如：拨号 (“@0”, ...) 即使用第一个拨号网络配置中的电话号码"
			"拨号"),
/*bmp inx*/	0,
/*bmp num*/	0,
/*type*/	SDT_TEXT,
/*default*/	0,
/*state*/	NULL,
	}, {
/*name*/	_WT("用户帐号"),
/*explain*/	_WT("指定用户帐号名称"),
/*bmp inx*/	0,
/*bmp num*/	0,
/*type*/	SDT_TEXT,
/*default*/	0,
/*state*/	NULL,
	}, {
/*name*/	_WT("帐号密码"),
/*explain*/	_WT("指定用户帐号的密码"),
/*bmp inx*/	0,
/*bmp num*/	0,
/*type*/	SDT_TEXT,
/*default*/	0,
/*state*/	NULL,
	}, {
/*name*/	_WT("是否等待连接完毕"),
/*explain*/	_WT("指定命令是否一直等到连接完毕（成功或失败）后才返回，"
		"如果本参数为真，则“状态标签”参数无效。如果本参数被省略，默认值为假"),
/*bmp inx*/	0,
/*bmp num*/	0,
/*type*/	SDT_BOOL,
/*default*/	0,
/*state*/	AS_DEFAULT_VALUE_IS_EMPTY,
	}, {
/*name*/	_WT("状态标签"),
/*explain*/	_WT("如果提供了一个已经存在的标签型窗口组件并且“是否等待连接完毕”"
		"参数为假，拨号过程中的状态文本将即时显示到该标签上。"
		"如果本参数被省略，默认值为空"),
/*bmp inx*/	0,
/*bmp num*/	0,
/*type*/	DTP_LABEL,	// 标签的 DataType 。
/*default*/	0,
/*state*/	AS_DEFAULT_VALUE_IS_EMPTY,
	},

//****** FTP文件改名	** 42
	{
/*name*/	_WT("欲改名的FTP文件"),
/*explain*/	_WT("本参数指定FTP服务器上欲改名文件的路径"),
/*bmp inx*/	0,
/*bmp num*/	0,
/*type*/	SDT_TEXT,
/*default*/	0,
/*state*/	NULL,
	}, {
/*name*/	_WT("新文件名称"),
/*explain*/	_WT("本参数指定欲改为的文件名称"),
/*bmp inx*/	0,
/*bmp num*/	0,
/*type*/	SDT_TEXT,
/*default*/	0,
/*state*/	NULL,
	},

};

ARG_INFO s_ArgInfo_SetProxy[] = 
{
	{
/*name*/	_WT("代理服务器名称"),
/*explain*/	_WT("如果要取消代理服务器设置，请提供空文本。"
		"如果本参数被省略，默认值为空文本"),
/*bmp inx*/	0,
/*bmp num*/	0,
/*type*/	SDT_TEXT,
/*default*/	0,
/*state*/	AS_DEFAULT_VALUE_IS_EMPTY,
	},
	{
/*name*/	_WT("用户名"),
/*explain*/	_WT(""),
/*bmp inx*/	0,
/*bmp num*/	0,
/*type*/	SDT_TEXT,
/*default*/	0,
/*state*/	AS_DEFAULT_VALUE_IS_EMPTY,
	},
	{
/*name*/	_WT("密码"),
/*explain*/	_WT(""),
/*bmp inx*/	0,
/*bmp num*/	0,
/*type*/	SDT_TEXT,
/*default*/	0,
/*state*/	AS_DEFAULT_VALUE_IS_EMPTY,
	},
};
