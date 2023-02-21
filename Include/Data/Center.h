/*************************************************
file:			CCenter.h
Author:			张平    
Date:			2009-11-11
Description:    数据中心
*************************************************/
#pragma once
#include <osg/Referenced>
#include <Data/Export.h>
#include <Data/TableManager.h>
#include <Data/RowStruct/RowDef.h>

namespace Data
{

class DATA_EXPORT CCenter : public osg::Referenced
{
public:
	CCenter(void);
	~CCenter(void);

	enum{
		DataSourceType_Database,			//数据库
		DataSourceType_BinaryFile,			//二进制文件
		DataSourceType_XMLFile,				//XML文件
		DataSourceType_WebService,			//Web服务
		DataSourceType_WebFile			//Web服务

	};

	/*
	DataSourceType	(in)	数据来源，
	Para			(in)	字符参数
	DataSourceType_Database：连接字符串
	DataSourceType_BinaryFile：二进制文件路径
	DataSourceType_XMLFile：XML文件
	DataSourceType_WebService：服务名称
	*/
	bool Init(DWORD DataSourceType, CString Para1, CString Para2 = _T(""), DWORD DataItem = 0xffffffff);

	//释放加载
	bool UnInit();

	//存储某一类型的数据
	bool SaveToDatabase(DWORD DataItem = 0xffffffff);
	//存储某一条数据
	bool SaveToDatabase(CRow *pRow);
	//存储二进制文件
	bool SaveToBinaryFile(CString Path, DWORD DataItem = 0xffffffff);
	//存储XML文件
	bool SaveToXMLFile(CString Path, DWORD DataItem = 0xffffffff);

	/*
	执行数据库命令队列
	ErrSQL	(out)	错误语句
	ErrMsg	(out)	错误信息
	返回值：全部执行成功返回true
	*/
	bool ExecuteSQLQueue(CString& ErrSQL, CString& ErrMsg);

	//SQL语句执行队列
	std::vector<CString> m_StrSQLQueue;
	CTableManager m_TM;
	_ConnectionPtr m_pConn;
	LONG64	GetNewShowObjID();
	LONG64  GetNewEnderID();
	bool CreateInstanceOpen_ConnectionPtr();
	bool Close_ConnectionPtr();
public:
	LONG64	GetNewID(CString strTableName);
	void SetMaxIDNumber();
	void SetMaxIDNumber(CString str);
	//Liugh添加数据库链接类型设置与获取
	DWORD	GetDataType(){return m_DBType;}
	void	SetDataType(DWORD type){m_DBType = type;}
private:
	DWORD m_DBType;
	CString m_strConnInfo;//记录创建数据库的配置信息
	OpenThreads::Mutex _lock;

};
//XML读取和保存模块
DATA_EXPORT CCenter *GetDataCenter();
}
