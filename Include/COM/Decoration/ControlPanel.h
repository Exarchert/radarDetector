/*********************************************************************
Copyright (C),  
文件：ControlPanel.h ControlPanel.cpp
作者：刘广慧
版本：1.0
日期：2009-07-25
描述：相机控制面板类.

修改记录：
作者					日期				描述
----------------------------------------------------------------------
黄志鹏					2009-11-16			移植到此工程

*********************************************************************/

#pragma once

#include <Com\Export.h>
#include <Com\Decoration\DecoraBase.h>
#include <Com\Ramble\RambleMgr.h>
#include <osg\Vec2>
#include <osg\Camera>
#include <osgAPEx\ControlPanel>
#include <osgWidget\WindowManager>


namespace COM	{

class COSGControlPanel;

//CControlPanel类
class COM_EXPORT CControlPanel : public CDecoraBase
{
public:
	CControlPanel();
	virtual ~CControlPanel();

	bool init(CRambleMgr *rambleMan, osgWidget::WindowManager *wm, osg::Camera *hud, const std::string& texfilepath,
			float rmargin, float tmargin);

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

protected:
	osg::ref_ptr<osgWidget::WindowManager>	m_wm;
	osg::ref_ptr<osg::Camera>				m_hud;
	osg::ref_ptr<COSGControlPanel>			m_crtPanel;
	osg::ref_ptr<CRambleMgr>			m_rambleMan; //漫游管理器
	osg::Vec2								m_margin;
	bool									m_picked;
};


//继承自引擎的控制面板基类
class COSGControlPanel : public osgAPEx::ControlPanel
{
public:
	COSGControlPanel();
	virtual ~COSGControlPanel();

	bool init(CRambleMgr *rambleMan, const std::string& texfilepath);

protected:
	virtual void moveAhead(double dt);

	virtual void moveBack(double dt);

	virtual void moveLeft(double dt);

	virtual void moveRight(double dt);

	virtual void moveUp(double dt);

	virtual void moveDown(double dt);

	virtual void rotateLeft(double dt);

	virtual void rotateRight(double dt);

	virtual void toNorth();

	virtual void toVertical();

protected:
	osg::ref_ptr<CRambleMgr>	m_rambleMan; //漫游管理器

	float	_scale;
};

}