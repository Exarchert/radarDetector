/*******************************************************
Copyright (C),  
�ļ���COM_EnterRow.h COM_EnterRow.cpp
���ߣ���ƽ
�汾��1.0
���ڣ�2009-11-23
������ͼ������
��ļ̳й�ϵ���£�

�޸ļ�¼��
����                ����              ����
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
��ҵ���ݼ�¼��
*/
class DATA_EXPORT CRowLayer : public CRow
{
public:
	CRowLayer(void);
	~CRowLayer(void);

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
	CString			m_Name;     // ����
	LONG64			m_Priority;	//ͼ�����ȼ�
	int				m_Visible;	//ͼ������

	static int		m_NameLen;

	static CString m_TableName;
	static CString m_ViewName;

	enum{
		RowType = 0x00000010
	};

};

}