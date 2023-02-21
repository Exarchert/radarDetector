/*************************************************
file:			CCenter.h
Author:			��ƽ    
Date:			2009-11-11
Description:    ��������
*************************************************/
#pragma once
#include <osg/Referenced>
#include <Data/Export.h>
#include <Data/TableManager.h>
#include <Data/RowStruct/RowDef.h>

namespace Data
{

class DATA_EXPORT CCenter : public osg::Referenced
{
public:
	CCenter(void);
	~CCenter(void);

	enum{
		DataSourceType_Database,			//���ݿ�
		DataSourceType_BinaryFile,			//�������ļ�
		DataSourceType_XMLFile,				//XML�ļ�
		DataSourceType_WebService,			//Web����
		DataSourceType_WebFile			//Web����

	};

	/*
	DataSourceType	(in)	������Դ��
	Para			(in)	�ַ�����
	DataSourceType_Database�������ַ���
	DataSourceType_BinaryFile���������ļ�·��
	DataSourceType_XMLFile��XML�ļ�
	DataSourceType_WebService����������
	*/
	bool Init(DWORD DataSourceType, CString Para1, CString Para2 = _T(""), DWORD DataItem = 0xffffffff);

	//�ͷż���
	bool UnInit();

	//�洢ĳһ���͵�����
	bool SaveToDatabase(DWORD DataItem = 0xffffffff);
	//�洢ĳһ������
	bool SaveToDatabase(CRow *pRow);
	//�洢�������ļ�
	bool SaveToBinaryFile(CString Path, DWORD DataItem = 0xffffffff);
	//�洢XML�ļ�
	bool SaveToXMLFile(CString Path, DWORD DataItem = 0xffffffff);

	/*
	ִ�����ݿ��������
	ErrSQL	(out)	�������
	ErrMsg	(out)	������Ϣ
	����ֵ��ȫ��ִ�гɹ�����true
	*/
	bool ExecuteSQLQueue(CString& ErrSQL, CString& ErrMsg);

	//SQL���ִ�ж���
	std::vector<CString> m_StrSQLQueue;
	CTableManager m_TM;
	_ConnectionPtr m_pConn;
	LONG64	GetNewShowObjID();
	LONG64  GetNewEnderID();
	bool CreateInstanceOpen_ConnectionPtr();
	bool Close_ConnectionPtr();
public:
	LONG64	GetNewID(CString strTableName);
	void SetMaxIDNumber();
	void SetMaxIDNumber(CString str);
	//Liugh������ݿ����������������ȡ
	DWORD	GetDataType(){return m_DBType;}
	void	SetDataType(DWORD type){m_DBType = type;}
private:
	DWORD m_DBType;
	CString m_strConnInfo;//��¼�������ݿ��������Ϣ
	OpenThreads::Mutex _lock;

};
//XML��ȡ�ͱ���ģ��
DATA_EXPORT CCenter *GetDataCenter();
}
