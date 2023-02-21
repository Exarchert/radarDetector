/************************************************************************
Copyright (C)  研发
文件：InitData.h InitData.cpp
作者：黄志鹏
版本：1.0
日期：2009-11-11
描述：数据中心加载类.

修改记录：
作者					日期				描述
----------------------------------------------------------------------

************************************************************************/

#pragma once

#include <Kernel\Export.h>
#include <Kernel\Init\InitModule.h>


namespace Kernel	{

//CInitData类
class CInitData : public CInitModule
{
public:
	CInitData();
	virtual ~CInitData();

	/*
	目的：执行初始化
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

};

}