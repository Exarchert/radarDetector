/*********************************************************************
Copyright (C),  
�ļ���InitModule.h InitModule.cpp
���ߣ���־��
�汾��1.0
���ڣ�2009-11-11
������ģ����ػ���.

�޸ļ�¼��
����					����				����
----------------------------------------------------------------------

*********************************************************************/

#pragma once

#include <Kernel\Export.h>
#include <osg\Referenced>
#include <COM\MsgTracer\MsgTracer.h>
#include <COM\KernelContext\KernelContext.h>


namespace Kernel	{

typedef std::map<CString, CString>	EnviromentMap;

//CInitModule��
class KERNEL_EXPORT CInitModule : public osg::Referenced
{
public:
	CInitModule();
	virtual ~CInitModule();

	/*
	Ŀ�ģ�ִ�г�ʼ��
	������kContext		(in)	�ں�������
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

	/*
	Ŀ�ģ����û�������
	������varName	(in)	��������
	      varValue	(in)	����ֵ
	���أ����ؾɻ�������ֵ
	*/
	CString SetEnvirVariable(const CString &varName, const CString &varValue);

	/*
	Ŀ�ģ���ȡ��������
	������varName	(in)	��������
	���أ���������ֵ
	*/
	CString GetEnvirVariable(const CString &varName) const;

	/*
	Ŀ�ģ����ø�����
	������lpTracer		(in)		�����߶���ָ��
	*/
	void SetTracer(COM::CMsgTracer *lpTracer);

	/*
	Ŀ�ģ���ӡ�ַ���
	*/
	void Dump(const CString &msg);

protected:
	COM::CMsgTracer	*m_tracer;		//���ٶ���
	EnviromentMap	m_envirVars;	//��������Map	 
};

}