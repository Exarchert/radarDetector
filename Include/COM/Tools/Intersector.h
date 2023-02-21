/***************************************************
Copyright:       (C), 
File name:       OSGA_ToolsIntersector.h
Author:          陈俊海    Version:   1.0     Date: 2009-03-25
Description:     相交检测类
Others:          
Function List:   
History:        
*************************************************/
#pragma once
#include <COM\Export.h>
#include <osg/Vec3d>
#include <osgGA/GUIActionAdapter>
#include <osgUtil/LineSegmentIntersector>
#include <osgUtil/IntersectVisitor>
#include <osgViewer/Viewer>

namespace COM	{

class COM_EXPORT CIntersector
{
public:
	CIntersector(void);
	~CIntersector(void);

	/*
	描述：  屏幕射线求交
	参数：
	pt		(out)	射线与场景的交点
	x		(in)	屏幕横坐标位置（-1到1之间）
	y		(in)	屏幕纵坐标位置（-1到1之间）
	aa		(in)	osg事件适配器，GUIActionAdapter   
	返回值： 返回是否相交
	*/
	static bool GetPointByScreenInters(osg::Vec3d& pt,const float& x,const float& y,osgGA::GUIActionAdapter& aa);
	/*
	描述：  屏幕射线求交
	参数：
	nodePath		(out)	与射线相交的nodepath，记录了与射线相交的最底层叶子节点到场景根节点的路径（node*数组）
	x				(in)	屏幕横坐标位置（-1到1之间）
	y				(in)	屏幕纵坐标位置（-1到1之间）
	aa				(in)	osg事件适配器，GUIActionAdapter   
	返回值： 返回是否相交
	*/
	static bool GetNodePathByScreenInters(osg::NodePath& nodePath,const float& x,const float& y,osgGA::GUIActionAdapter& aa);
	/*
	描述：  屏幕射线求交
	参数：
	hits		    (out)	相交所得原始结果，具体见osgUtil::LineSegmentIntersector::Intersections说明
	x				(in)	屏幕横坐标位置（-1到1之间）
	y				(in)	屏幕纵坐标位置（-1到1之间）
	aa				(in)	osg事件适配器，GUIActionAdapter   
	返回值： 返回是否相交
	*/
	static bool GetIntersectionsByScreenInters(osgUtil::LineSegmentIntersector::Intersections& hits,const float& x,const float& y,osgGA::GUIActionAdapter& aa);



	/*
	描述：  空间线段与指点节点的相交检测
	参数：
	pt				(out)	相交交点
	startPt			(in)	线段起点
	endPt			(in)	线段终点
	intersectedNode	(in)	被检测的节点   
	返回值： 返回是否相交
	*/
	static bool GetPointByLineInters(osg::Vec3d& pt,const osg::Vec3d& startPt,const osg::Vec3d& endPt,osg::Node* intersectedNode);
	/*
	描述：  空间线段与指点节点的相交检测
	参数：
	nodePath		(out)	相交节点到场景根节点的路径
	startPt			(in)	线段起点
	endPt			(in)	线段终点
	intersectedNode	(in)	被检测的节点   
	返回值： 返回是否相交
	*/
	static bool GetNodePathByLineInters(osg::NodePath& nodePath,const osg::Vec3d& startPt,const osg::Vec3d& endPt,osg::Node* intersectedNode);
	/*
	描述：  空间线段与指点节点的相交检测
	参数：
	hitList 		(out)	相交所得原始结果，具体见osgUtil::IntersectVisitor::HitList说明
	startPt			(in)	线段起点
	endPt			(in)	线段终点
	intersectedNode	(in)	被检测的节点   
	返回值： 返回是否相交
	*/
	static bool GetHitListByLineInters(osgUtil::IntersectVisitor::HitList& hitList,const osg::Vec3d& startPt,const osg::Vec3d& endPt,osg::Node* intersectedNode);
};
}