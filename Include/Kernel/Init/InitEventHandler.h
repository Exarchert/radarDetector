/*********************************************************************
Copyright (C),  
文件：InitEventHandler.h InitEventHandler.cpp
作者：黄志鹏
版本：1.0
日期：2009-11-13
描述：初始化需要事件处理的对象.

修改记录：
作者					日期				描述
----------------------------------------------------------------------
黄志鹏					2010-02-03			加入数据采集事件处理

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