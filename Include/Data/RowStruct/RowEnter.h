/*******************************************************
Copyright (C),  
文件：COM_EnterRow.h COM_EnterRow.cpp
作者：黄志鹏
版本：1.0
日期：2009-06-02
描述：CRowEnter类对象标识了一条企业数据记录。
类的继承关系如下：
osg::reference
CCOM_DataRow
CRowEnter

修改记录：
作者                日期              描述
--------------------------------------------------------
黄志鹏             2009-06-03         添加文件读写方法

*******************************************************/

#pragma once 
#include <Data\Export.h>
#include <osg\Vec3d>
#include <osg\Vec3>
#include <Data\Row.h>
#include <fstream>
using namespace std;

namespace Data
{

/*
企业数据记录类
*/
class DATA_EXPORT CRowEnter : public CRow
{
public:
	CRowEnter(void);
	~CRowEnter(void);

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
	CString       m_Name;     // 企业名称
	osg::Vec3d    m_PosLLA;    // 坐标位置<经度、纬度、海拔>
	osg::Vec3	  m_YPR;    // 姿态<北向、俯仰, 滚转>
	CString		  m_NavigationMap;
	LONG64		  m_ParentID;				//父节点

	static CString m_TableName; // 记录所在表的名称
	static CString m_ViewName;  // 记录所在视图的名称

	// 下面数据表示在数据库表中相应字符串的长度
	static int m_NameLen;      // 企业名称的字符数
	static int m_NavigationMapLen;

	enum{
		RowType = 0x00000002
	};

};

}