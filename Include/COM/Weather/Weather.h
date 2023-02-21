/************************************************************************
Copyright (c)  
文件：Weather.h Weather.cpp
作者：黄志鹏
版本：1.0
日期：2009-12-28
描述：借鉴原有代码移植到此. 天气的属性有：雨、雪、雾、风.
	  场景组织图如下：
					m_parent
						|
					m_swAll
						|
					m_translate
					   /   \
				   swRain  swSnow
					/		  \
				m_peRain	m_peSnow

修改记录：
作者					日期				描述
-------------------------------------------------------------------------

************************************************************************/

#pragma once

#include <Com\Export.h>
#include <vector>
#include <osg\Fog>
#include <osg\Node>
#include <osg\Switch>
#include <osg\Referenced>
#include <OpenThreads\Mutex>
#include <OpenThreads\ScopedLock>
#include <osg\MatrixTransform>
#include <osgGA\GUIEventHandler>
#include <osgGA\GUIEventAdapter>
#include <osgGA\GUIActionAdapter>
#include <osgParticle\PrecipitationEffect>


namespace COM	{

class COM_EXPORT CWeather : public osgGA::GUIEventHandler
{
public:
	/*
	目的：构造函数
	      node		(in)	雾化对象
	      parent	(in)	天气特效的父结点
	*/
	CWeather(osg::Node *node, osg::Group *parent);
	virtual ~CWeather();

	//天气属性
	enum ATTRIBUTE {
		RAIN,			//雨
		SNOW,			//雪
		FOG,			//雾
		WIND			//风
	};

	//天气强度
	enum INTENSION {
		NONE,			//无
		WEAK,			//小
		NORMAL,			//中
		STRONG			//大
	};

	//风向
	enum WINDDIR {
		EAST,			//东风
		SOUTH,			//南风
		WEST,			//西风
		NORTH			//北风
	};

	/*
	目的：初始化
	参数：rain		(in)	降雨强度
	      snow		(in)	降雪强度
	      wind		(in)	刮风强度
		  dir		(in)	风向
	*/
	bool Init(INTENSION rain, INTENSION snow, INTENSION fog, INTENSION wind, WINDDIR dir);

	/*
	目的：设置天气属性强度(线程安全)
	参数：attri		(in)	属性标记
	      inten		(in)	强度
	      dir		(in)	如果属性为WIND，则dir为风向
	*/
	bool SetIntension(ATTRIBUTE attri, INTENSION inten, DWORD dir);

	/*
	目的：获取天气属性强度(线程安全)
	参数：attri		(in)	属性标记
	      inten		(out)	强度
	      dir		(out)	如果属性为WIND，则dir为风向
	*/
	bool GetIntension(ATTRIBUTE attri, INTENSION &inten, DWORD &dir);

	/*
	目的：处理来自OSG消息队列的事件
	*/
	virtual bool handle(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa);

	void SetLocation(osg::Vec3d& LLA);

protected:
	class AttriCmd : public osg::Referenced
	{
	public:
		ATTRIBUTE	m_attri;
		INTENSION	m_inten;
		WINDDIR		m_dir;
	};
	typedef std::vector<osg::ref_ptr<AttriCmd>> AttriCmdList;

	osg::ref_ptr<osg::MatrixTransform>				m_trans;
	osg::ref_ptr<osg::Switch>						m_swAll;
	osg::ref_ptr<osg::Switch>						m_swRain;
	osg::ref_ptr<osg::Switch>						m_swSnow;
	osg::ref_ptr<osgParticle::PrecipitationEffect>	m_peRain;
	osg::ref_ptr<osgParticle::PrecipitationEffect>	m_peSnow;
	osg::ref_ptr<osg::Fog>							m_fog;

	osg::ref_ptr<osg::Node>		m_node;		//雾化结点
	osg::ref_ptr<osg::Group>	m_parent;	//父结点
	bool			m_bOk;					//相机高度满足条件
	INTENSION		m_rainInten;
	INTENSION		m_snowInten;
	INTENSION		m_fogInten;
	INTENSION		m_windInten;
	WINDDIR			m_dir;

	OpenThreads::Mutex	m_cmdsMutex;	//互斥量
	AttriCmdList		m_cmdList;		//命令队列
};

}