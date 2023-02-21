/**********************************************************************************
File:			COM_CartoonManager.h
Author:			张平    
Date:			2009-03-31
Description:    动画管理类

ChangeLog:
作者					日期					描述
-----------------------------------------------------------------------------------
黄志鹏				2009-07-21			审查，删除不必要的代码
黄志鹏				2009-11-08			添加CCOM_PassiveCartoonPlayer
黄志鹏				2009-12-29			添加大事件列表命中功能

Liugh				2010.11.25			Add All XML Parser Function For Each Script
刘广慧				2011-1-12			生成xml格式脚本
***********************************************************************************/

#pragma once

#include <CartoonObject/Export.h>
#include <osg/ref_ptr>
#include <osg/Node>
#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>
#include <osgGA/GUIEventAdapter>
#include <osgGA/GUIActionAdapter>
#include <osgGA/EventQueue>
#include <CartoonObject/ScriptCreate/Cartoon/COM_CartoonUnit.h>


namespace CartoonObject		{

class CARTOONOBJECT_EXPORT CCOM_CartoonManager
{
public:
	CCOM_CartoonManager(void);
	virtual ~CCOM_CartoonManager(void);

	//添加和删除卡通单元
	void AddCartoonUnit(CCOM_CartoonUnit *pCartoonUnit);
	void DelCartoonUnit(CCOM_CartoonUnit *pCartoonUnit);

	void Clear();

	//设置影片的已播放时间
	void SetPlayTime(double time);

	//获取播放总长
	double GetScriptTimeLength(int index = 0);

	//获取当前播放时刻
	double GetCurPlayTime(int index = 0);

protected:
	std::vector<osg::ref_ptr<CCOM_CartoonUnit>> m_CartoonUnits;
	double m_LastTime;
};
}