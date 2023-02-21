/*********************************************************************
Copyright (C),  
�ļ���InitEventHandler.h InitEventHandler.cpp
���ߣ���־��
�汾��1.0
���ڣ�2009-11-13
��������ʼ����Ҫ�¼�����Ķ���.

�޸ļ�¼��
����					����				����
----------------------------------------------------------------------
��־��					2010-02-03			�������ݲɼ��¼�����

*********************************************************************/

#pragma once

#include <Kernel\Export.h>
#include <Kernel\Init\InitModule.h>


namespace	Kernel
{

class CInitEventHandler : public CInitModule
{
public:
	CInitEventHandler();
	virtual ~CInitEventHandler();

	/*
	Ŀ�ģ�ִ�г�ʼ��
	���أ��ɹ�����TRUE�����򷵻�FALSE
	*/
	virtual BOOL InitInstance(COM::CKernelContext &kContext);

	/*
	Ŀ�ģ���ʼ����������
	*/
	virtual void InitEnviroment();
};

}