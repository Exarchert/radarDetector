/****************************************************************************
Copyright (C),  
文件：LocateAgent.h LocateAgent.cpp
作者：黄志鹏
版本：1.0
日期：2010-03-01
描述：代理操控相机的定位，目前支持的方式有：直接定位、飞行定位.具有线程安全性.

修改记录：
作者					日期					描述
-----------------------------------------------------------------------------

****************************************************************************/

#pragma once

#include <Com\Export.h>
#include <osg\Vec3d>
#include <osgGA\GUIEventHandler>
#include <osgGA\GUIEventAdapter>
#include <osgGA\GUIActionAdapter>
#include <OpenThreads\Mutex>
#include <Com\Cartoon\CartoonManager.h>

#include <map>


namespace COM	{

//CLocateAgent类
class COM_EXPORT CLocateAgent : public osgGA::GUIEventHandler
{
public:
	CLocateAgent();
	virtual ~CLocateAgent();

	/*
	目的：处理来自OSG消息队列的事件
	*/
	virtual bool handle(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa);

	/*
	目的：初始化
	参数：player	(in)	卡通播放器指针
	*/
	void Init(CCartoonManager *player);

	/*
	目的：飞行定位(线程安全)
	参数：pos	(in)	经纬高
	      pose	(in)	姿态角
	*/
	void Fly(osg::Vec3d pos, osg::Vec3d pose);

	/*
	目的：直接定位(线程安全)
	参数：pos	(in)	经纬高
	      pose	(in)	姿态角
	*/
	void Direct(osg::Vec3d pos, osg::Vec3d pose);

protected:
	enum LOCATE_TYPE {
		DIRECT,		//直接定位
		FLY			//飞行定位
	};

	class CLocateCmd : public osg::Referenced
	{
	public:
		LOCATE_TYPE m_type;
		osg::Vec3d	m_destPos;
		osg::Vec3d	m_destPose;
	};
	typedef std::vector<CLocateCmd>	LocateCmdList;

	OpenThreads::Mutex	m_cmdsMutex;	//互斥量
	LocateCmdList		m_locateCmds;	//定位命令队列
	osg::ref_ptr<CCartoonManager>	m_player;
};

}