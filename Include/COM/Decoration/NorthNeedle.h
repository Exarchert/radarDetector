/*********************************************************************
Copyright (C),  
文件：NorthNeedle.h NorthNeedle.cpp
作者：刘广慧
版本：1.0
日期：2009-11-16
描述：指北针对象类.

修改记录：
作者					日期				描述
----------------------------------------------------------------------
黄志鹏					2009-11-16			移植到此工程

*********************************************************************/

#pragma once

#include <osg\Vec2>
#include <osg\Vec3>
#include <osg\Switch>
#include <osg\Camera>
#include <osg\MatrixTransform>
#include <osgAPEx\EarthManipulator>
#include <Com\Decoration\DecoraBase.h>
#include <Com\Ramble\RambleMgr.h>


namespace COM	{

class COM_EXPORT CNorthNeedle : public CDecoraBase
{
public:
	CNorthNeedle();
	virtual ~CNorthNeedle();

	/*
	目的：处理来自OSG消息队列的事件
	*/
	virtual bool handle(const osgGA::GUIEventAdapter& ea,osgGA::GUIActionAdapter& aa);

	/*
	目的：控制显隐
	参数：bShow		(in)	true:显示; false:隐藏
	返回：设置前的状态
	*/
	virtual bool Show(bool bShow=true);

	/*
	目的：初始化
	参数：earthMan	(in)	地球操作器
		  hud		(in)	HUD结点指针
	      texfile	(in)	贴图文件名
		  scale		(in)	对原始图片放大倍数
		  rmargin	(in)	右边距
		  tmargin	(in)	上边距
	返回：成功返回true，失败返回false.
	*/
	//bool init(osgAPEx::EarthManipulator *earthMan, osg::Camera *hud, std::string &texfile, float scale,
	//		float rmargin, float tmargin);
	bool init(COM::CRambleMgr *earthMan, osg::Camera *hud, std::string &texfile, float scale,
		float rmargin, float tmargin);

protected:
	//创建一矩形
	osg::Geode* createQuad(float width, float height, float z);

protected:
	osg::Camera								*m_hud;		//HUD
	osg::ref_ptr<COM::CRambleMgr>			m_earthMgr;	//地球操作器
	osg::ref_ptr<osg::Switch>				m_root;		//指北针根结点
	osg::ref_ptr<osg::MatrixTransform>		m_mat;		//装换结点
	osg::Vec2								m_margin;	//边距
	osg::Vec2								m_size;		//指北针的大小
	osg::Vec3								m_pose;		//相机姿态
	float									m_speed;	//旋转速率
	bool									m_blaunch;	//起动旋转
};

}