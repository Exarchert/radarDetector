/************************************************************************
Copyright (C)  �з�
�ļ���InitData.h InitData.cpp
���ߣ���־��
�汾��1.0
���ڣ�2009-11-11
�������������ļ�����.

�޸ļ�¼��
����					����				����
----------------------------------------------------------------------

************************************************************************/

#pragma once

#include <Kernel\Export.h>
#include <Kernel\Init\InitModule.h>


namespace Kernel	{

//CInitData��
class CInitData : public CInitModule
{
public:
	CInitData();
	virtual ~CInitData();

	/*
	Ŀ�ģ�ִ�г�ʼ��
	���أ��ɹ�����TRUE�����򷵻�FALSE
	*/
	virtual BOOL InitInstance(COM::CKernelContext &kContext);

	/*
	Ŀ�ģ��ͷų�ʼ��
	*/
	virtual BOOL UnInitInstance(COM::CKernelContext &kContext);

	/*
	Ŀ�ģ���ʼ����������
	*/
	virtual void InitEnviroment();

};

}