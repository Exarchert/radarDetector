/*************************************************
file:			COM_DataTableManager.h
Author:			张平    
Date:			2009-03-31
Description:    表管理器
*************************************************/
#pragma once
#include <Data/Export.h>
#include <vector>
#include <map>
#include <Data/Table.h>
#include <Data/Row.h>

namespace Data
{

class DATA_EXPORT CTableManager
{
public:
	CTableManager(void);
	~CTableManager(void);

	void AddTable(DWORD Type, CTableDef* Table);
	void DelTable(DWORD Type);

	void AddRow(DWORD Type, CRow* CRow);
	void DelRow(DWORD Type, CRow* CRow);
	void DelRow(DWORD Type, LONG64 ID);

	void Clear();

	CTableDef* GetTable(DWORD Type);
	CRow* GetRow(DWORD Type, LONG64 ID);

	std::map<DWORD, osg::ref_ptr<CTableDef>>* GetTables(){return &m_mapTables;}
protected:
	std::map<DWORD, osg::ref_ptr<CTableDef>> m_mapTables;
};
}