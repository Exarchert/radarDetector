/************************************************************************
Copyright (C)  
文件：InitViewer.h InitViewer.cpp
作者：黄志鹏
版本：1.0
日期：2009-11-11
描述：视景浏览器初始化类.

修改记录：
作者					日期				描述
----------------------------------------------------------------------

************************************************************************/

#pragma once

#include <Kernel\Export.h>
#include <Kernel\Init\InitModule.h>


namespace Kernel	{

//CInitViewer类
class CInitViewer : public CInitModule
{
public:
	CInitViewer();
	virtual ~CInitViewer();

	/*
	目的：执行初始化
	返回：成功返回TRUE，否则返回FALSE
	*/
	virtual BOOL InitInstance(COM::CKernelContext &kContext);

	/*
	目的：初始化环境变量
	*/
	virtual void InitEnviroment();
};

}
