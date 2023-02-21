/*******************************************************
Copyright (C),  
�ļ���COM_EnterRow.h COM_EnterRow.cpp
���ߣ���־��
�汾��1.0
���ڣ�2009-06-02
������CRowEnter������ʶ��һ����ҵ���ݼ�¼��
��ļ̳й�ϵ���£�
osg::reference
CCOM_DataRow
CRowEnter

�޸ļ�¼��
����                ����              ����
--------------------------------------------------------
��־��             2009-06-03         ����ļ���д����

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
��ҵ���ݼ�¼��
*/
class DATA_EXPORT CRowEnter : public CRow
{
public:
	CRowEnter(void);
	~CRowEnter(void);

	/*
	Ŀ�ģ�������Ӹü�¼��SQL���
	������void
	���أ�CString-SQL���
	*/
	CString Add();

	/*
	Ŀ�ģ����ɸ��¸ü�¼��SQL���
	������void
	���أ�CString-SQL���
	*/
	CString Update();

	/*
	Ŀ�ģ���ʼ��������
	������rst    (in)    ��ҵ���ݲ�ѯ��¼��ָ�� 
	���أ��ɹ�����true�����򷵻�false
	*/
	bool Read(_RecordsetPtr rst);

	/*
	Ŀ�ģ���ȡ�ü�¼���ڵı�����
	������void
	���أ�CString-������
	*/
	CString GetTableName();

	/*
	Ŀ�ģ���ȡ�ü�¼���ڵ���ͼ����
	������void
	���أ�CString-��ͼ����
	*/
	CString GetViewName();

	/*
	Ŀ�ģ����ļ���ȡһ����¼���ݳ�ʼ��������
	������tableFile    (in)   �ļ�������
	���أ��ɹ�����true�����򷵻�false
	*/
	bool WriteFile(std::ofstream& tableFile);

	/*
	Ŀ�ģ���������ļ�¼����д���ļ�
	������tableFile    (in)  �ļ�д����
	���أ��ɹ�����true�����򷵻�false
	*/
	bool ReadFile(std::ifstream& tableFile);

public:
	CString       m_Name;     // ��ҵ����
	osg::Vec3d    m_PosLLA;    // ����λ��<���ȡ�γ�ȡ�����>
	osg::Vec3	  m_YPR;    // ��̬<���򡢸���, ��ת>
	CString		  m_NavigationMap;
	LONG64		  m_ParentID;				//���ڵ�

	static CString m_TableName; // ��¼���ڱ������
	static CString m_ViewName;  // ��¼������ͼ������

	// �������ݱ�ʾ�����ݿ������Ӧ�ַ����ĳ���
	static int m_NameLen;      // ��ҵ���Ƶ��ַ���
	static int m_NavigationMapLen;

	enum{
		RowType = 0x00000002
	};

};

}