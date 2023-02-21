/************************************************************************
Copyright (C),  
文件：SunShine.h  SunShine.cpp
作者：黄志鹏
版本：1.0
日期：2009-08-20
描述：太阳光晕类.

修改记录：
作者					日期				描述
-------------------------------------------------------------------------
黄志鹏					2010-01-04			移植到此工程

************************************************************************/

#pragma once

#include <osg\Vec3d>
#include <osg\Switch>
#include <osg\Camera>
#include <osgAPEx\Sunshine>
#include <Com\Decoration\DecoraBase.h>


namespace COM	{

class COM_EXPORT CSunShine : public CDecoraBase
{
public:
	/*
	目的：构造函数
	*/
	CSunShine();

	virtual ~CSunShine();

	/*
	目的：初始化
	参数：fieldPos	(in)	厂区位置(经纬高)
	      distance	(in)	太阳距离厂区的距离
		  texfile	(in)	贴图文件路径
	      hud		(in)	hud指针
	      camera	(in)	主相机指针
	      curTime	(in)	时刻(用于计算光晕方向, 取值6~18)
	返回：成功返回true，失败返回false
	*/
	bool Init(osg::Vec3d fieldPos, double distance, std::string texfile, osg::Camera *hud, osg::Camera *camera,
		float curTime);

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
	osg::Camera							*m_hud;				//HUD相机
	osg::ref_ptr<osg::Switch>			m_switchSunshine;	//光晕的switch
	osg::ref_ptr<osgAPEx::Sunshine>		m_sunShine;			//光晕
};

}