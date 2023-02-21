/*******************************************************
Copyright (C),  
文件：COM_EnterRow.h COM_EnterRow.cpp
作者：张平
版本：1.0
日期：2009-11-23
描述：图层对象表
类的继承关系如下：

修改记录：
作者                日期              描述
--------------------------------------------------------


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
class DATA_EXPORT CRowLayer : public CRow
{
public:
	CRowLayer(void);
	~CRowLayer(void);

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
	CString			m_Name;     // 名称
	LONG64			m_Priority;	//图层优先级
	int				m_Visible;	//图层显隐

	static int		m_NameLen;

	static CString m_TableName;
	static CString m_ViewName;

	enum{
		RowType = 0x00000010
	};

};

}