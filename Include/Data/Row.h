/*************************************************
file:			COM_DataRow.h
Author:			张平    
Date:			2009-03-18
Description:    内存行记录,是所有的记录集基类
*************************************************/
#pragma once
#include <Data\Export.h>
#include <osg/Referenced>
//#import "C:\\Program Files\\Common Files\\system\\ado\\msadox.dll"
#import "C:\\Program Files\\Common Files\\system\\ado\\msado15.dll" rename("EOF","ADOEOF")
using namespace ADODB;
#include <iostream>

namespace Data
{

class DATA_EXPORT CRow : public osg::Referenced
{
public:
	CRow(void);
	virtual ~CRow(void);

	virtual CString Add(){return _T("");}
	virtual CString Update(){return _T("");}
	virtual bool Read(_RecordsetPtr rst){return true;}
	virtual CString GetTableName(){return _T("");}
	virtual CString GetViewName(){return _T("");}
	virtual bool WriteFile(std::ofstream& tableFile){return true;}
	virtual bool ReadFile(std::ifstream& tableFile){return true;}
	virtual DWORD GetType(){return NULL;}

	CString Del();

	bool ReRead(_ConnectionPtr pConn);

	LONG64 ID;

	enum{
		EM_DRSTATE_UNDEFAULT,
		EM_DRSTATE_EDITED,
		EM_DRSTATE_NEW,
		EM_DRSTATE_DELETED,
	};	
	
	void SetState(DWORD State);
	DWORD GetState();


protected:
	DWORD m_State;

};

}