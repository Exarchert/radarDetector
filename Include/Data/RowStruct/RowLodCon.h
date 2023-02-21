/*******************************************************
Copyright (C),  
作者：张平
版本：1.0
日期：2010-01-27
描述：CRowLodDic加载配置条件字典表。

*******************************************************/

#pragma once 
#include <Data\Export.h>
#include <Data\Row.h>
#include <fstream>
using namespace std;

namespace Data
{

/*
企业数据记录类
*/
class DATA_EXPORT CRowLodCon : public CRow
{
public:
	CRowLodCon(void);
	~CRowLodCon(void);

	/*
	目的：生成添加该记录的SQL语句
	参数：void
	返回：CString-SQL语句
	*/
	CString Add();

	/*
	目的：生成更新该记录的SQL语句
	参数：void
	返回：CString-SQL语句
	*/
	CString Update();

	/*
	目的：初始化本对象
	参数：rst    (in)    企业数据查询记录集指针 
	返回：成功返回true，否则返回false
	*/
	bool Read(_RecordsetPtr rst);

	/*
	目的：获取该记录所在的表名称
	参数：void
	返回：CString-表名称
	*/
	CString GetTableName();

	/*
	目的：获取该记录所在的视图名称
	参数：void
	返回：CString-视图名称
	*/
	CString GetViewName();

	/*
	目的：从文件读取一条记录数据初始化本对象
	参数：tableFile    (in)   文件读对象
	返回：成功返回true，否则返回false
	*/
	bool WriteFile(std::ofstream& tableFile);

	/*
	目的：将本对象的记录数据写入文件
	参数：tableFile    (in)  文件写对象
	返回：成功返回true，否则返回false
	*/
	bool ReadFile(std::ifstream& tableFile);

public:
	CString       m_Name;		// 
	CString		  m_Content;	// lod配置内容

	static CString m_TableName; // 记录所在表的名称
	static CString m_ViewName;  // 记录所在视图的名称

	// 下面数据表示在数据库表中相应字符串的长度
	static int m_NameLen;      // 企业名称的字符数
	static int m_ContentLen;

	enum{
		RowType = 0x00000020
	};

};

}