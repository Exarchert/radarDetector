/*********************************************************************
Copyright (C),  
文件：DecoraBase.h DecoraBase.cpp
作者：黄志鹏
版本：1.0
日期：2009-11-16
描述：装饰对象基类.

修改记录：
作者					日期				描述
----------------------------------------------------------------------

*********************************************************************/

#pragma once 

#include <Com\Export.h>
#include <osg\Referenced>
#include <osgGA\GUIEventAdapter>
#include <osgGA\GUIActionAdapter>


namespace COM	{

class COM_EXPORT CDecoraBase : public osg::Referenced
{
public:
	CDecoraBase();
	virtual ~CDecoraBase();

	/*
	目的：处理来自OSG消息队列的事件
	*/
	virtual bool handle(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa);

	/*
	目的：控制显隐
	参数：bShow		(in)	true:显示; false:隐藏
	返回：设置前的状态
	*/
	virtual bool Show(bool bShow=true);

	/*
	目的：获取显隐状态
	返回：true表示显示, false表示隐藏
	*/
	virtual bool IsShow() const;

protected:
	bool	m_bShow;
};

}