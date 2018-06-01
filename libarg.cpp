
#include "stdafx.h"
#include "..\libinf.h"
#include "..\krnl\krnllib.h"
#include "internet.h"

ARG_INFO s_ArgInfo[] =
{
//****** ���ӷ��ŷ�����	**	0
	{
/*name*/	_WT("�����ʼ���������ַ"),
/*explain*/	_WT("ָ�����������ʼ���SMTP�ʼ���������ַ"),
/*bmp inx*/	0,
/*bmp num*/	0,
/*type*/	SDT_TEXT,
/*default*/	0,
/*state*/	NULL,
	}, {
/*name*/	_WT("�˿ں�"),
/*explain*/	_WT("ָ�������ӵ�SMTP�ʼ��������ϵĶ˿ںţ�"
		"�����������ʡ�ԣ�Ĭ��ֵΪ��׼ 25 �˿�"),
/*bmp inx*/	0,
/*bmp num*/	0,
/*type*/	SDT_INT,
/*default*/	0,
/*state*/	AS_DEFAULT_VALUE_IS_EMPTY,
	}, {
/*name*/	_WT("�û���"),
/*explain*/	_WT("ָ����SMTP�ʼ��������ϵ��û��ʺ����ƣ�"
		"�����������ʡ�ԣ�Ĭ��ֵΪ���ı����������¼"),
/*bmp inx*/	0,
/*bmp num*/	0,
/*type*/	SDT_TEXT,
/*default*/	0,
/*state*/	AS_DEFAULT_VALUE_IS_EMPTY,
	}, {
/*name*/	_WT("����"),
/*explain*/	_WT("ָ����SMTP�ʼ��������ϵ��û��ʺ����룬"
		"�����������ʡ�ԣ�Ĭ��ֵΪ���ı����������¼"),
/*bmp inx*/	0,
/*bmp num*/	0,
/*type*/	SDT_TEXT,
/*default*/	0,
/*state*/	AS_DEFAULT_VALUE_IS_EMPTY,
	}, {
/*name*/	_WT("��ȴ�ʱ��"),
/*explain*/	_WT("ָ���ڷ��Ź����еȴ�SMTP�ʼ���������Ӧ�����ʱ�䣬��λΪ���룬"
		"�����������ʡ�ԣ�Ĭ��ֵΪ 2000 ����"),
/*bmp inx*/	0,
/*bmp num*/	0,
/*type*/	SDT_INT,
/*default*/	0,
/*state*/	AS_DEFAULT_VALUE_IS_EMPTY,
	},

//****** ��Ӹ����ļ�	**	5
	{
/*name*/	_WT("����ӵĸ����ļ�"),
/*explain*/	NULL,
/*bmp inx*/	0,
/*bmp num*/	0,
/*type*/	SDT_TEXT,
/*default*/	0,
/*state*/	NULL,
	},

//****** ��Ӹ�������	**	6
	{
/*name*/	_WT("����ӵĸ�������"),
/*explain*/	NULL,
/*bmp inx*/	0,
/*bmp num*/	0,
/*type*/	SDT_BIN,
/*default*/	0,
/*state*/	NULL,
	}, {
/*name*/	_WT("��������"),
/*explain*/	NULL,
/*bmp inx*/	0,
/*bmp num*/	0,
/*type*/	SDT_TEXT,
/*default*/	0,
/*state*/	NULL,
	},

//****** �����ʼ�	**	8
	{
/*name*/	_WT("����"),
/*explain*/	NULL,
/*bmp inx*/	0,
/*bmp num*/	0,
/*type*/	SDT_TEXT,
/*default*/	0,
/*state*/	NULL,
	}, {
/*name*/	_WT("����"),
/*explain*/	NULL,
/*bmp inx*/	0,
/*bmp num*/	0,
/*type*/	SDT_TEXT,
/*default*/	0,
/*state*/	NULL,
	}, {
/*name*/	_WT("�������ʼ���ַ"),
/*explain*/	_WT("����Ϊ��ַ�б������Ѻ����ƣ�ʾ���磺"
			"���ͷ���service@dywt.com.cn��;��ǰ��sale@dywt.com.cn���� ��"
			"��service@dywt.com.cn;sale@dywt.com.cn�������ԣ����ַ֮���ö��Ż�ֺŷָ���"
			"�Ѻ����ƺ���ʼ���ַ�ð�Ǽ�������ס"),
/*bmp inx*/	0,
/*bmp num*/	0,
/*type*/	SDT_TEXT,
/*default*/	0,
/*state*/	NULL,
	}, {
/*name*/	_WT("�����ʼ���ַ"),
/*explain*/	_WT("��ַ��ʽ��ͬ�ڡ��������ʼ���ַ���������������ʡ�ԣ�Ĭ��Ϊ���ı�"),
/*bmp inx*/	0,
/*bmp num*/	0,
/*type*/	SDT_TEXT,
/*default*/	0,
/*state*/	AS_DEFAULT_VALUE_IS_EMPTY,
	}, {
/*name*/	_WT("�����ʼ���ַ"),
/*explain*/	_WT("��ַ��ʽ��ͬ�ڡ��������ʼ���ַ���������������ʡ�ԣ�Ĭ��Ϊ���ı�"),
/*bmp inx*/	0,
/*bmp num*/	0,
/*type*/	SDT_TEXT,
/*default*/	0,
/*state*/	AS_DEFAULT_VALUE_IS_EMPTY,
	}, {
/*name*/	_WT("�������ʼ���ַ"),
/*explain*/	_WT("ֻ��Ϊ�����ʼ���ַ�����԰����Ѻ����ƣ�ʾ���磺"
			"���ͷ���service@dywt.com.cn������service@dywt.com.cn��"),
/*bmp inx*/	0,
/*bmp num*/	0,
/*type*/	SDT_TEXT,
/*default*/	0,
/*state*/	NULL,
	}, {
/*name*/	_WT("�ظ��ʼ���ַ"),
/*explain*/	_WT("ָ���ռ��˻ظ����ʼ��ĵ�ַ�������ʡ�ԣ�Ĭ��Ϊ�뷢�����ʼ���ַ��ͬ"),
/*bmp inx*/	0,
/*bmp num*/	0,
/*type*/	SDT_TEXT,
/*default*/	0,
/*state*/	AS_DEFAULT_VALUE_IS_EMPTY,
	},

//****** �ô��������	**	15
	{
/*name*/	_WT("�������������"),
/*explain*/	_WT("���Ҫȡ��������������ã����ṩ���ı���"
		"�����������ʡ�ԣ�Ĭ��ֵΪ���ı�"),
/*bmp inx*/	0,
/*bmp num*/	0,
/*type*/	SDT_TEXT,
/*default*/	0,
/*state*/	AS_DEFAULT_VALUE_IS_EMPTY,
	},

//****** HTTP���ļ�	**	16
	{
/*name*/	_WT("HTTP�ļ���ַ"),
/*explain*/	_WT("������ָ������ȡ�ļ��ڻ������ϵ�HTTP��ַ����URL��"),
/*bmp inx*/	0,
/*bmp num*/	0,
/*type*/	SDT_TEXT,
/*default*/	0,
/*state*/	NULL,
	},

//****** ����FTP������	**	17
	{
/*name*/	_WT("FTP��������ַ"),
/*explain*/	_WT("������ָ��FTP�������ڻ������ϵĵ�ַ"),
/*bmp inx*/	0,
/*bmp num*/	0,
/*type*/	SDT_TEXT,
/*default*/	0,
/*state*/	NULL,
	}, {
/*name*/	_WT("�û���"),
/*explain*/	_WT("�������ṩ����ͨ��FTP�������û���֤���û����ƣ������ʡ�ԣ�"
		"Ĭ��ֵΪ�����û�����anonymous��"),
/*bmp inx*/	0,
/*bmp num*/	0,
/*type*/	SDT_TEXT,
/*default*/	0,
/*state*/	AS_DEFAULT_VALUE_IS_EMPTY,
	}, {
/*name*/	_WT("����"),
/*explain*/	_WT("�������ṩ����ͨ��FTP�������û���֤�����룬�����ʡ�ԣ�"
		"Ĭ��ֵΪ���ı�"),
/*bmp inx*/	0,
/*bmp num*/	0,
/*type*/	SDT_TEXT,
/*default*/	0,
/*state*/	AS_DEFAULT_VALUE_IS_EMPTY,
	}, {
/*name*/	_WT("�˿ں�"),
/*explain*/	_WT("�������ṩ�����ӵ�FTP�������ϵĶ˿ںţ������ʡ�ԣ�"
		"Ĭ��ֵΪ��׼�˿� 21 "),
/*bmp inx*/	0,
/*bmp num*/	0,
/*type*/	SDT_INT,
/*default*/	0,
/*state*/	AS_DEFAULT_VALUE_IS_EMPTY,
	}, {
/*name*/	_WT("�Ƿ�ʹ�ñ�������"),
/*explain*/	_WT("�����������ʡ�ԣ�Ĭ��ֵΪ��"),
/*bmp inx*/	0,
/*bmp num*/	0,
/*type*/	SDT_BOOL,
/*default*/	0,
/*state*/	AS_DEFAULT_VALUE_IS_EMPTY,
	},

//****** FTP�����ļ�	** 22
	{
/*name*/	_WT("�����ص�FTP�ļ�"),
/*explain*/	_WT("������ָ����FTP���������������ļ���·��"),
/*bmp inx*/	0,
/*bmp num*/	0,
/*type*/	SDT_TEXT,
/*default*/	0,
/*state*/	NULL,
	}, {
/*name*/	_WT("�����ļ���"),
/*explain*/	_WT("������ָ���������ļ��ı��ر���λ��"),
/*bmp inx*/	0,
/*bmp num*/	0,
/*type*/	SDT_TEXT,
/*default*/	0,
/*state*/	NULL,
	}, {
/*name*/	_WT("�Ƿ�ʹ��ASCII�뷽ʽ"),
/*explain*/	_WT("ָ���Ƿ�ʹ��ASCII���뷽ʽ�����ļ����ݡ������������ʡ�ԣ�Ĭ��ֵΪ�٣���ʹ�ö����Ʒ�ʽ����"),
/*bmp inx*/	0,
/*bmp num*/	0,
/*type*/	SDT_BOOL,
/*default*/	0,
/*state*/	AS_DEFAULT_VALUE_IS_EMPTY,
	},

//****** FTP�ϴ��ļ�	** 25
	{
/*name*/	_WT("���ϴ��ı����ļ�"),
/*explain*/	_WT("������ָ�����ϴ���FTP�������ϱ����ļ���·��"),
/*bmp inx*/	0,
/*bmp num*/	0,
/*type*/	SDT_TEXT,
/*default*/	0,
/*state*/	NULL,
	}, {
/*name*/	_WT("FTP�ļ�·��"),
/*explain*/	_WT("������ָ���ϴ���FTP���������ļ��ı���λ��"),
/*bmp inx*/	0,
/*bmp num*/	0,
/*type*/	SDT_TEXT,
/*default*/	0,
/*state*/	NULL,
	}, {
/*name*/	_WT("�Ƿ�ʹ��ASCII�뷽ʽ"),
/*explain*/	_WT("ָ���Ƿ�ʹ��ASCII���뷽ʽ�����ļ����ݡ������������ʡ�ԣ�Ĭ��ֵΪ�٣���ʹ�ö����Ʒ�ʽ����"),
/*bmp inx*/	0,
/*bmp num*/	0,
/*type*/	SDT_BOOL,
/*default*/	0,
/*state*/	AS_DEFAULT_VALUE_IS_EMPTY,
	},

//****** FTPɾ���ļ�	** 28
	{
/*name*/	_WT("��ɾ����FTP�ļ�"),
/*explain*/	_WT("������ָ����FTP����������ɾ���ļ���·��"),
/*bmp inx*/	0,
/*bmp num*/	0,
/*type*/	SDT_TEXT,
/*default*/	0,
/*state*/	NULL,
	},

//****** FTP����Ŀ¼��FTPɾ��Ŀ¼��FTP������Ŀ¼	** 29
	{
/*name*/	_WT("FTPĿ¼·��"),
/*explain*/	NULL,
/*bmp inx*/	0,
/*bmp num*/	0,
/*type*/	SDT_TEXT,
/*default*/	0,
/*state*/	NULL,
	},
//****** FTPĿ¼�б�	** 30
	{
/*name*/	_WT("���г����ļ�"),
/*explain*/	_WT("���԰���·��������ʹ�ö��ַ� (*) �͵��ַ� (?) ͨ�����ָ�������ļ���Ŀ¼"),
/*bmp inx*/	0,
/*bmp num*/	0,
/*type*/	SDT_TEXT,
/*default*/	0,
/*state*/	NULL,
	}, {
/*name*/	_WT("���г��ļ�������"),
/*explain*/	_WT("����ֵ����Ϊ���³���ֵ����ͣ� 1��#ֻ���ļ��� 2��#�����ļ���"
		" 4��#ϵͳ�ļ��� 16��#��Ŀ¼�� 32��#�浵�ļ� ��"
		"ͨ������Щ����ֵ����������һ�����ö���ļ����ԡ����ʡ�Ա�������"
		"Ĭ��Ϊ��Ѱ������Ŀ¼���ļ�"),
/*bmp inx*/	0,
/*bmp num*/	0,
/*type*/	SDT_INT,
/*default*/	0,
/*state*/	AS_DEFAULT_VALUE_IS_EMPTY,
	}, {
/*name*/	_WT("����ļ������������"),
/*explain*/	_WT("������ִ����Ϻ󣬱����������ڱ�˳���������ҵ���ƥ���ļ�����Ŀ¼���ơ�"
		"����������ԭ�����ݱ�ȫ�����٣����������ά�����Զ�����Ϊ���ҵ����ļ���Ŀ��"
		"�����������ʡ�ԣ�Ĭ��ֵΪ��"),
/*bmp inx*/	0,
/*bmp num*/	0,
/*type*/	SDT_TEXT,
/*default*/	0,
/*state*/	AS_RECEIVE_VAR_ARRAY | AS_DEFAULT_VALUE_IS_EMPTY,
	}, {
/*name*/	_WT("����ļ����Ե��������"),
/*explain*/	_WT("������ִ����Ϻ󣬱����������ڱ�˳���������ҵ���ƥ���ļ������ԣ�"
		"�����ļ��������Աһһ��Ӧ��������ֵ�����³�������͹��ɣ� 1��#ֻ���ļ��� "
		"2��#�����ļ���  4��#ϵͳ�ļ��� 16��#��Ŀ¼��32��#�浵�ļ� ��Ҫ�ж��Ƿ�"
		"������ĳ�����ԣ��ڷ���ֵ����Ҫ��֪������ֵ֮��ʹ�á�λ�롱������бȽϡ�"
		"������õĽ����Ϊ�㣬���ʾ�������������ֵ������������ԭ�����ݱ�ȫ����"
		"�٣����������ά�����Զ�����Ϊ���ҵ����ļ���Ŀ�������������ʡ�ԣ�Ĭ��ֵΪ��"),
/*bmp inx*/	0,
/*bmp num*/	0,
/*type*/	SDT_INT,
/*default*/	0,
/*state*/	AS_RECEIVE_VAR_ARRAY | AS_DEFAULT_VALUE_IS_EMPTY,
	}, {
/*name*/	_WT("����ļ��ߴ���������"),
/*explain*/	_WT("������ִ����Ϻ󣬱����������ڱ�˳���������ҵ���ƥ���ļ��ĳߴ磬"
		"�����ļ��������Աһһ��Ӧ������������ԭ�����ݱ�ȫ����"
		"�٣����������ά�����Զ�����Ϊ���ҵ����ļ���Ŀ�������������ʡ�ԣ�Ĭ��ֵΪ��"),
/*bmp inx*/	0,
/*bmp num*/	0,
/*type*/	SDT_INT,
/*default*/	0,
/*state*/	AS_RECEIVE_VAR_ARRAY | AS_DEFAULT_VALUE_IS_EMPTY,
	}, {
/*name*/	_WT("����ļ�ʱ����������"),
/*explain*/	_WT("������ִ����Ϻ󣬱����������ڱ�˳���������ҵ���ƥ���ļ�������޸�ʱ�䣬"
		"�����ļ��������Աһһ��Ӧ�����ļ�ʱ�伴FTP���������ص��ļ�ʱ�䣬�����κ�ʱ��ת����"
		"����������ԭ�����ݱ�ȫ�����٣����������ά�����Զ�����Ϊ���ҵ����ļ���Ŀ�������������ʡ�ԣ�Ĭ��ֵΪ��"),
/*bmp inx*/	0,
/*bmp num*/	0,
/*type*/	SDT_DATE_TIME,
/*default*/	0,
/*state*/	AS_RECEIVE_VAR_ARRAY | AS_DEFAULT_VALUE_IS_EMPTY,
	},

//****** ȡ�������ơ�ȡ�û��ʺš�ȡ�ʺ�����	** 36
	{
/*name*/	_WT("������������"),
/*explain*/	_WT("ָ����ȡ��һ����������������Ϣ������ֵ�� 0 ��ʼ��"
		"���ֵΪ��ȡ����������������ķ���ֵ��һ"),
/*bmp inx*/	0,
/*bmp num*/	0,
/*type*/	SDT_INT,
/*default*/	0,
/*state*/	NULL,
	},

//****** ����	** 37
	{
/*name*/	_WT("�绰����"),
/*explain*/	_WT("ָ���������ĵ绰���룬�����ʹ���Ѿ����ڵ��û���������������"
			"�ĵ绰���룬��ʹ�á�@n����ʽ�ı�������nΪָ�������������õ�������ֵ��"
			"��0��ʼ���磺���� (��@0��, ...) ��ʹ�õ�һ���������������еĵ绰����"
			"����"),
/*bmp inx*/	0,
/*bmp num*/	0,
/*type*/	SDT_TEXT,
/*default*/	0,
/*state*/	NULL,
	}, {
/*name*/	_WT("�û��ʺ�"),
/*explain*/	_WT("ָ���û��ʺ�����"),
/*bmp inx*/	0,
/*bmp num*/	0,
/*type*/	SDT_TEXT,
/*default*/	0,
/*state*/	NULL,
	}, {
/*name*/	_WT("�ʺ�����"),
/*explain*/	_WT("ָ���û��ʺŵ�����"),
/*bmp inx*/	0,
/*bmp num*/	0,
/*type*/	SDT_TEXT,
/*default*/	0,
/*state*/	NULL,
	}, {
/*name*/	_WT("�Ƿ�ȴ��������"),
/*explain*/	_WT("ָ�������Ƿ�һֱ�ȵ�������ϣ��ɹ���ʧ�ܣ���ŷ��أ�"
		"���������Ϊ�棬��״̬��ǩ��������Ч�������������ʡ�ԣ�Ĭ��ֵΪ��"),
/*bmp inx*/	0,
/*bmp num*/	0,
/*type*/	SDT_BOOL,
/*default*/	0,
/*state*/	AS_DEFAULT_VALUE_IS_EMPTY,
	}, {
/*name*/	_WT("״̬��ǩ"),
/*explain*/	_WT("����ṩ��һ���Ѿ����ڵı�ǩ�ʹ���������ҡ��Ƿ�ȴ�������ϡ�"
		"����Ϊ�٣����Ź����е�״̬�ı�����ʱ��ʾ���ñ�ǩ�ϡ�"
		"�����������ʡ�ԣ�Ĭ��ֵΪ��"),
/*bmp inx*/	0,
/*bmp num*/	0,
/*type*/	DTP_LABEL,	// ��ǩ�� DataType ��
/*default*/	0,
/*state*/	AS_DEFAULT_VALUE_IS_EMPTY,
	},

//****** FTP�ļ�����	** 42
	{
/*name*/	_WT("��������FTP�ļ�"),
/*explain*/	_WT("������ָ��FTP���������������ļ���·��"),
/*bmp inx*/	0,
/*bmp num*/	0,
/*type*/	SDT_TEXT,
/*default*/	0,
/*state*/	NULL,
	}, {
/*name*/	_WT("���ļ�����"),
/*explain*/	_WT("������ָ������Ϊ���ļ�����"),
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
/*name*/	_WT("�������������"),
/*explain*/	_WT("���Ҫȡ��������������ã����ṩ���ı���"
		"�����������ʡ�ԣ�Ĭ��ֵΪ���ı�"),
/*bmp inx*/	0,
/*bmp num*/	0,
/*type*/	SDT_TEXT,
/*default*/	0,
/*state*/	AS_DEFAULT_VALUE_IS_EMPTY,
	},
	{
/*name*/	_WT("�û���"),
/*explain*/	_WT(""),
/*bmp inx*/	0,
/*bmp num*/	0,
/*type*/	SDT_TEXT,
/*default*/	0,
/*state*/	AS_DEFAULT_VALUE_IS_EMPTY,
	},
	{
/*name*/	_WT("����"),
/*explain*/	_WT(""),
/*bmp inx*/	0,
/*bmp num*/	0,
/*type*/	SDT_TEXT,
/*default*/	0,
/*state*/	AS_DEFAULT_VALUE_IS_EMPTY,
	},
};
