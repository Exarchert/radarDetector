/*********************************************************************
Copyright (C),  
文件：InitHud.h InitHud.cpp
作者：黄志鹏
版本：1.0
日期：2009-11-13
描述：初始化HUD.

修改记录：
作者					日期				描述
----------------------------------------------------------------------
黄志鹏					2009-11-16			增加CHudResizeHandler类

*********************************************************************/

#pragma once

#include <Kernel\Export.h>
#include <Kernel\Init\InitModule.h>
#include <osg\Camera>
#include <osgGA\GUIEventHandler>
#include <osgGA\GUIEventAdapter>
#include <osgGA\GUIActionAdapter>


namespace Kernel	{

//CInitHud类
class CInitHud : public CInitModule
{
public:
	CInitHud();
	virtual ~CInitHud();

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


//CHudResizeHandler类
class CHudResizeHandler : public osgGA::GUIEventHandler
{
public:
	//bAuto		(in)	自动调整投影矩阵
	CHudResizeHandler(osg::Camera *hudCamera, bool bAuto=false, bool centerAligned=false);
	virtual ~CHudResizeHandler();

	virtual bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa);

protected:
	osg::Camera *m_hudCamera;
	bool		m_bAuto;
	bool		m_bCenterAligned;
};

}
