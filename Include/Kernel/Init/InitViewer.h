/************************************************************************
Copyright (C)  
�ļ���InitViewer.h InitViewer.cpp
���ߣ���־��
�汾��1.0
���ڣ�2009-11-11
�������Ӿ��������ʼ����.

�޸ļ�¼��
����					����				����
----------------------------------------------------------------------

************************************************************************/

#pragma once

#include <Kernel\Export.h>
#include <Kernel\Init\InitModule.h>


namespace Kernel	{

//CInitViewer��
class CInitViewer : public CInitModule
{
public:
	CInitViewer();
	virtual ~CInitViewer();

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
