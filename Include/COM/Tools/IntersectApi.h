/***************************************************************************
Copyright (C),  
文件：Intersect.h
作者：谢荣耀
版本：1.0
日期：2009-12-4
描述：导出与地形求交的接口

修改记录：
作者						日期					描述
***************************************************************************/
#pragma once
#include <COM\Export.h>
#include <osg\Camera>
namespace COM
{
	/*
	得到与地形求交的坐标点
	camera	        (in)	  当前的主相机
	subgraph        (in)      执行相交检测的子树节点
	x/y             (in)      鼠标位置  
	pos             (in/out)  与场景相交的位置
	return: 成功返回true，失败返回false
	*/
	COM_EXPORT bool GetTerrainIntersection(osg::Camera *camera, osg::Node *subGraph,float x, float y, osg::Vec3 &pos);


}
