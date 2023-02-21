/************************************************************************
Copyright (C),   
�ļ���TextTempBuf.h TextTempBuf.cpp
���ߣ���־��
�汾��1.0
���ڣ�2010-03-25
����������ģ��������ģ�����ݵ�ӳ�����.

�޸ļ�¼��
����					����					����
-------------------------------------------------------------------------

************************************************************************/

#pragma once

#include <Object\Export.h>
#include <Data\RowStruct\RowTextTemplate.h>
#include <map>
#include <vector>


namespace EMObj		{

class CTextTempBuf 
{
public:
	//��ȡ����ʵ��
	static CTextTempBuf &GetInstance();

	/*
	Ŀ�ģ���ȡ��������ģ�������.
	������nameList	(out)		ģ�������б�
	���أ����ָ���
	*/
	int GetNameList(std::vector<CString> &nameList) const;

	/*
	Ŀ�ģ�ͨ�����ֻ�ȡģ������.
	������szName	(in)	ģ������
	      szContent	(out)	ģ������
	���أ��ɹ�����TRUE��ʧ�ܷ���FALSE
	*/
	BOOL GetValue(const CString &szName, CString &szContent) const;

	/*
	Ŀ�ģ����һ��ģ��.
	������szName	(in)	ģ������
	      szVlaue	(in)	ģ������
	���أ��ɹ�����TRUE�����򷵻�FALSE
	��ע���������������֮
	*/
	BOOL AddValue(const CString &szName, const CString &szValue);

	/*
	Ŀ�ģ�ɾ��һ��ӳ��.
	������szName	(in)	ģ������
	���أ��ɹ�����TRUE�����򷵻�FALSE
	*/
	BOOL RemoveValue(const CString &szName);

protected:
	CTextTempBuf();
	virtual ~CTextTempBuf() {}

protected:
	std::map<CString, LONG64> m_name2Content;
};

}
