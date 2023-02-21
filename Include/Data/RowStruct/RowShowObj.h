/***************************************************************
修改记录：
作者                      日期                 描述
----------------------------------------------------------------
黄志鹏                 2009-06-04            编写成员函数Add()
谢荣耀                 2009-11-03            添加m_Lod成员，Read方法添加读取Lod字段的内容
***************************************************************/

#pragma once
#include <Data/Export.h>
#include <osg\Vec3d>
#include <Data\Row.h>
#include <fstream>

namespace Data{

class DATA_EXPORT CRowShowObj : public CRow
{
public:
	CRowShowObj(void);
	~CRowShowObj(void);

	int		m_EnterID;				//企业ID
	CString m_Name;					//名称
	LONG64  m_TypeID;				//类型ID
	LONG64	m_LodConID;				//LOD条件ID
	CString m_EyePosAndPose;		//观察点位置和姿态
	int		m_Visable;				//显隐
	int		m_ShowPriority;			//显示优先级
	CString	m_Property;				//XML属性
	LONG64  m_ParentID;				//父节点
	CString	m_LodPara;				//Lod参数

	CString Add();
	CString Update();
	bool Read(_RecordsetPtr rst);
	bool ReadFile(std::ifstream& tableFile);
	bool WriteFile(std::ofstream& tableFile);
	CString GetTableName();
	CString GetViewName();

	static CString m_TableName;
	static CString m_ViewName;

public:
	static int m_NameLen;      // 企业名称的字符数
	static int m_LodParaLen;	//lod参数长度

	enum{
		RowType = 0x00000004
	};

	virtual DWORD GetType(){return RowType;}
};
}