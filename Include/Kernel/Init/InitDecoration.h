/*********************************************************************
Copyright (C),  研发
文件：InitDecoration.h InitDecoration.cpp
作者：黄志鹏
版本：1.0
日期：2009-11-13
描述：初始化装饰物：天空、大气层、星空、导航板、指北针、状态栏等.

修改记录：
作者					日期				描述
----------------------------------------------------------------------

*********************************************************************/

#pragma once

#include <Kernel\Export.h>
#include <Kernel\Init\InitModule.h>


namespace Kernel	{

//CInitDecoration类
class CInitDecoration : public CInitModule
{
public:
	CInitDecoration();
	virtual ~CInitDecoration();

	/*
	目的：执行初始化
	返回：成功返回TRUE，否则返回FALSE
	*/
	virtual BOOL InitInstance(COM::CKernelContext &kContext);
	
	/*
	目的：初始化环境变量
	*/
	virtual void InitEnviroment();

	/*
	目的：释放初始化
	*/
	virtual BOOL UnInitInstance(COM::CKernelContext &kContext);
};

}