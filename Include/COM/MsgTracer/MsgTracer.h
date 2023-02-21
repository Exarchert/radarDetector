/************************************************************
Copyright (C),  
�ļ���MsgTracer.h MsgTracer.cpp
���ߣ���־��
�汾��1.0
���ڣ�2009-11-11
��������־��Ϣ���ٶ�����.

�޸ļ�¼��
����					����				����
-------------------------------------------------------------

************************************************************/

#pragma once

#include <COM\Export.h>


namespace COM	{

//CMsgTracer��
class COM_EXPORT CMsgTracer
{
public:
	CMsgTracer();
	virtual ~CMsgTracer();

	/*
	Ŀ�ģ���������Ϣ��¼���ö���
	������lpMsg		(in)	�ַ���ָ��,��'\0'��β
	���أ�void
	*/
	virtual void Write(const TCHAR* lpMsg);

	/*
	Ŀ�ģ���������Ϣ��¼���ö���
	������strMsg	(in)	�ַ�����
	���أ�void
	*/
	virtual void Write(const CString &strMsg);
};

//CDumpToNotePad��
//��ӡ�����±�
class COM_EXPORT CDumpToNotePad : public CMsgTracer
{
public:
	CDumpToNotePad();
	virtual ~CDumpToNotePad();

	/*
	Ŀ�ģ���������Ϣ��¼���ö���
	������lpMsg		(in)	�ַ���ָ��
	���أ�void
	*/
	virtual void Write(const TCHAR* lpMsg);

	/*
	Ŀ�ģ���������Ϣ��¼���ö���
	������strMsg	(in)	�ַ�����
	���أ�void
	*/
	virtual void Write(const CString &strMsg);
};

//CDumpToScreen��
//�������������
//������Ϣ��ʽ����:
//	����:xx|����ȼ�:��Ϣ����
//	
class COM_EXPORT CDumpToScreen : public CMsgTracer
{
public:
	CDumpToScreen(HWND hDest);
	virtual ~CDumpToScreen();

	/*
	Ŀ�ģ���������Ϣ��¼���ö���
	������lpMsg		(in)	�ַ���ָ��
	���أ�void
	*/
	virtual void Write(const TCHAR* lpMsg);

	/*
	Ŀ�ģ���������Ϣ��¼���ö���
	������strMsg	(in)	�ַ�����
	���أ�void
	*/
	virtual void Write(const CString &strMsg);

protected:
	HWND	m_hDest;	//Ŀ�괰��
};

extern COM_EXPORT bool g_bThreadExit;
}