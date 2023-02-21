/*********************************************************************
Copyright (C),  
文件：InitModule.h InitModule.cpp
作者：黄志鹏
版本：1.0
日期：2009-11-11
描述：模块加载基类.

修改记录：
作者					日期				描述
----------------------------------------------------------------------

*********************************************************************/

#pragma once

#include <Kernel\Export.h>
#include <osg\Referenced>
#include <COM\MsgTracer\MsgTracer.h>
#include <COM\KernelContext\KernelContext.h>


namespace Kernel	{

typedef std::map<CString, CString>	EnviromentMap;

//CInitModule类
class KERNEL_EXPORT CInitModule : public osg::Referenced
{
public:
	CInitModule();
	virtual ~CInitModule();

	/*
	目的：执行初始化
	参数：kContext		(in)	内核上下文
	返回：成功返回TRUE，否则返回FALSE
	*/
	virtual BOOL InitInstance(COM::CKernelContext &kContext);

	/*
	目的：释放初始化
	*/
	virtual BOOL UnInitInstance(COM::CKernelContext &kContext);

	/*
	目的：初始化环境变量
	*/
	virtual void InitEnviroment();

	/*
	目的：设置环境变量
	参数：varName	(in)	变量名称
	      varValue	(in)	变量值
	返回：返回旧环境变量值
	*/
	CString SetEnvirVariable(const CString &varName, const CString &varValue);

	/*
	目的：获取环境变量
	参数：varName	(in)	变量名称
	返回：环境变量值
	*/
	CString GetEnvirVariable(const CString &varName) const;

	/*
	目的：设置跟踪者
	参数：lpTracer		(in)		跟踪者对象指针
	*/
	void SetTracer(COM::CMsgTracer *lpTracer);

	/*
	目的：打印字符串
	*/
	void Dump(const CString &msg);

protected:
	COM::CMsgTracer	*m_tracer;		//跟踪对象
	EnviromentMap	m_envirVars;	//环境变量Map	 
};

}