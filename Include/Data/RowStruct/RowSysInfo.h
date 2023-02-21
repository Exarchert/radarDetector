/*************************************************
file:			COM_DRSysInfo.h
Author:			��ƽ    
Date:			2009-9-11
Description:    ��������
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
	Ŀ�ģ�������Ӹü�¼��SQL���
	������void
	���أ�CString-SQL���
	*/
	virtual CString Add();

	/*
	Ŀ�ģ����ɸ��¸ü�¼��SQL���
	������void
	���أ�CString-SQL���
	*/
	virtual CString Update();

	/*
	Ŀ�ģ���ʼ��������
	������rst    (in)    ���ӳ�����ݲ�ѯ��¼��ָ�� 
	���أ��ɹ�����true�����򷵻�false
	*/
	virtual bool Read(_RecordsetPtr rst);

	/*
	Ŀ�ģ���ȡ�ü�¼���ڵı�����
	������void
	���أ�CString-������
	*/
	virtual CString GetTableName();

	/*
	Ŀ�ģ���ȡ�ü�¼���ڵ���ͼ����
	������void
	���أ�CString-��ͼ����
	*/
	virtual CString GetViewName();

	/*
	Ŀ�ģ����ļ���ȡһ����¼���ݳ�ʼ��������
	������tableFile    (in)   �ļ�������
	���أ��ɹ�����true�����򷵻�false
	*/
	virtual bool WriteFile(std::ofstream& tableFile);

	/*
	Ŀ�ģ���������ļ�¼����д���ļ�
	������tableFile    (in)  �ļ�д����
	���أ��ɹ�����true�����򷵻�false
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