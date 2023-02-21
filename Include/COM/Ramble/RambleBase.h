/************************************************************************
Copyright (C)  
文件：RambleBase.h RambleBase.cpp
作者：白冰
版本：1.0
日期：2009-06-18
描述：漫游操作器基类.

修改记录：
作者					日期				描述
-------------------------------------------------------------------------
黄志鹏					2009-11-13			移植老版本，修改部分成员函数.

************************************************************************/

#pragma once

#include <COM\Export.h>
#include <osgGA\GUIEventHandler>
#include <osgGA\GUIActionAdapter>
#include <osgAPEx\EarthManipulator>


namespace COM	{

class COM_EXPORT CRambleBase : public osg::Referenced
{
public:
	CRambleBase(osgAPEx::EarthManipulator *earthMan);
	virtual ~CRambleBase();

	/*
	目的：处理来自OSG消息队列的事件
	*/
	virtual bool handle(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa);

	/*
	目的：处理Windows发送的消息
	*/
	virtual bool handle(UINT msg, WPARAM wParam, LPARAM lParam);
	
	//向视角方向移动
	virtual void MoveAt(float fValue);

	//水平向前移动
	virtual void MoveAhead(float fValue);

	//水平向右移动
	virtual void MoveRightSide(float fValue);

	//改变偏向角
	virtual void rotateYaw(float fValue);

	//改变俯仰角
	virtual void rotatePitch(float fValue);

	//改变高度
	virtual void MoveHeight(float fValue);
protected:
	osg::ref_ptr<osgAPEx::EarthManipulator> m_earthMan;
};

}