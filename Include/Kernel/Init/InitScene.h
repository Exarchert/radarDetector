/************************************************************************
Copyright (C)  
文件：InitScene.h InitScene.cpp
作者：黄志鹏
版本：1.0
日期：2009-11-11
描述：场景初始化类.

修改记录：
作者					日期				描述
----------------------------------------------------------------------

************************************************************************/

#pragma once 

#include <Kernel\Export.h>
#include <Kernel\Init\InitModule.h>


namespace Kernel	{

//CInitScene类
class CInitScene : public CInitModule
{
public:
	CInitScene();
	virtual ~CInitScene();

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
private:
	void DownLoadObjRes(CString strUrl,CString strCache);

};

}