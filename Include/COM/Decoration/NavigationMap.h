/*********************************************************************
Copyright (C),  
文件：NavigationMap.h NavigationMap.cpp
作者：黄志鹏
版本：1.0
日期：2009-11-16
描述：导航版对象类.

修改记录：
作者					日期				描述
----------------------------------------------------------------------

*********************************************************************/

#pragma once

#include <Com\Export.h>
#include <Com\Decoration\DecoraBase.h>
#include <osg\ref_ptr>
#include <osg\Switch>
#include <osg\Viewport>
#include <osg\Camera>
#include <osgGA\MatrixManipulator>
#include <osgAPEx\NavigationMap>

#import <msxml3.dll>

namespace COM	{

class COM_EXPORT CNavigationMap : public CDecoraBase
{
public:
	CNavigationMap();
	virtual ~CNavigationMap();

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
	osg::ref_ptr<osg::Switch>				m_root;			//挂载导航图
	osg::ref_ptr<osgAPEx::NavigationMap>	m_navMap;		//导航图
	double									m_distance;		//有效距离
	//std::vector<osgAPEx::NavigationMap::MapParam>	m_maps;	//存储地图信息

	
	/*
	刘广慧:
	修改完善导航图显示
	2010-07-06
	*/
public:
	struct MapInfo 
	{
		LONG64	_enterID;
		bool	_bActive;
		float	_dist;
		osgAPEx::NavigationMap::MapParam _mapParam;
	};

	std::vector<MapInfo>	m_maps;	//存储地图信息

	static CNavigationMap* GetInstance();
	static osg::ref_ptr<CNavigationMap> s_pNavigation;

	/*
	目的：初始化
	参数：hud		(in)		HUD
	dist		(in)		有效距离(出现导航图)
	其余参数同osgAPEx::Navigation::init(...)
	返回：成功返回true，失败返回false.
	*/
	bool init(osg::Camera *hud,std::vector<MapInfo>& maps, float mapwidth, const std::string& eyetex, 
		float eyewith, osg::Viewport* vp, osgAPEx::NavigationMap::Alignment align, const osg::Vec2& space, osgGA::MatrixManipulator* manip);

	/*
	目的: 设置唯一的导航图有效,替他场站导航图将都不再显示
	参数: EnterID	(in)	场站ID
	返回：成功返回true，失败返回false.
	*/
	bool SetSingleMapValid(LONG64 EnterID);

	/*
	目的: 设置所有导航图均有效
	参数: 无
	返回：成功返回true，失败返回false.
	*/
	bool SetAllMapValid();

	static bool GetMapInfoFromXmlNode(MSXML2::IXMLDOMNodePtr pNode,CString& mapPath,float& mapWide,
		float& enterWide,float& enterHeight,float& mapYaw,float& dist,bool& bActive);
private:
	bool bShowMap(bool bShow);
};

}