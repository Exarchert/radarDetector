/************************************************************************
Copyright (C)  
�ļ���InitScene.h InitScene.cpp
���ߣ���־��
�汾��1.0
���ڣ�2009-11-11
������������ʼ����.

�޸ļ�¼��
����					����				����
----------------------------------------------------------------------

************************************************************************/

#pragma once 

#include <Kernel\Export.h>
#include <Kernel\Init\InitModule.h>


namespace Kernel	{

//CInitScene��
class CInitScene : public CInitModule
{
public:
	CInitScene();
	virtual ~CInitScene();

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
private:
	void DownLoadObjRes(CString strUrl,CString strCache);

};

}