/*********************************************************************
Copyright (C),  �з�
�ļ���InitDecoration.h InitDecoration.cpp
���ߣ���־��
�汾��1.0
���ڣ�2009-11-13
��������ʼ��װ�����ա������㡢�ǿա������塢ָ���롢״̬����.

�޸ļ�¼��
����					����				����
----------------------------------------------------------------------

*********************************************************************/

#pragma once

#include <Kernel\Export.h>
#include <Kernel\Init\InitModule.h>


namespace Kernel	{

//CInitDecoration��
class CInitDecoration : public CInitModule
{
public:
	CInitDecoration();
	virtual ~CInitDecoration();

	/*
	Ŀ�ģ�ִ�г�ʼ��
	���أ��ɹ�����TRUE�����򷵻�FALSE
	*/
	virtual BOOL InitInstance(COM::CKernelContext &kContext);
	
	/*
	Ŀ�ģ���ʼ����������
	*/
	virtual void InitEnviroment();

	/*
	Ŀ�ģ��ͷų�ʼ��
	*/
	virtual BOOL UnInitInstance(COM::CKernelContext &kContext);
};

}