/****************************************************************
Copyright (C),  
�ļ���RowTextTemplate.h RowTextTemplate.cpp
���ߣ���ƽ
�汾��1.0
���ڣ�2010-03-31
��������¼�����ӵ�����ģ��.

�޸ļ�¼��
����                ����              ����
-----------------------------------------------------------------

*****************************************************************/

#pragma once

#include <Data\Export.h>
#include <Data\Row.h>
#include <fstream>

using namespace std;


namespace Data		{

class DATA_EXPORT CRowParticleTemplate : public CRow
{
public:
	CRowParticleTemplate(void);
	~CRowParticleTemplate(void);

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
	������rst    (in)    ���ݲ�ѯ��¼��ָ�� 
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
	CString m_tempName;		//ģ������
	CString m_tempContent;	//ģ������
	CString m_tempType;     //��Ч����

	static CString m_TableName; // ��¼���ڱ������
	static CString m_ViewName;  // ��¼������ͼ������

	// �������ݱ�ʾ�����ݿ������Ӧ�ַ����ĳ���
	static int m_NameLen;      // ģ�����Ƶ��ַ���
	static int m_ContentLen;	// ģ�����ݵ��ַ���
    static int m_TypeLen;

	enum{
		RowType = 0x00000080
	};

};

}