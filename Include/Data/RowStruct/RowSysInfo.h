/*************************************************
file:			COM_DRSysInfo.h
Author:			张平    
Date:			2009-9-11
Description:    数据主类
*************************************************/

#pragma once
#include <Data/Export.h>
#include <Data\Export.h>
#include <osg\Vec3d>
#include <Data\Row.h>
#include <fstream>

using namespace std;

namespace Data
{

class DATA_EXPORT CRowSysInfo : public CRow
{
public:
	CRowSysInfo(void);
	virtual ~CRowSysInfo(void);

	/*
	目的：生成添加该记录的SQL语句
	参数：void
	返回：CString-SQL语句
	*/
	virtual CString Add();

	/*
	目的：生成更新该记录的SQL语句
	参数：void
	返回：CString-SQL语句
	*/
	virtual CString Update();

	/*
	目的：初始化本对象
	参数：rst    (in)    物件映射数据查询记录集指针 
	返回：成功返回true，否则返回false
	*/
	virtual bool Read(_RecordsetPtr rst);

	/*
	目的：获取该记录所在的表名称
	参数：void
	返回：CString-表名称
	*/
	virtual CString GetTableName();

	/*
	目的：获取该记录所在的视图名称
	参数：void
	返回：CString-视图名称
	*/
	virtual CString GetViewName();

	/*
	目的：从文件读取一条记录数据初始化本对象
	参数：tableFile    (in)   文件读对象
	返回：成功返回true，否则返回false
	*/
	virtual bool WriteFile(std::ofstream& tableFile);

	/*
	目的：将本对象的记录数据写入文件
	参数：tableFile    (in)  文件写对象
	返回：成功返回true，否则返回false
	*/
	virtual bool ReadFile(std::ifstream& tableFile);

public:
	CString m_Item;		
	CString m_ItemValue;	

	static CString m_TableName;
	static CString m_ViewName;

	enum{
		RowType = 0x00000001
	};

};
}