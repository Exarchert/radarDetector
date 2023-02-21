#pragma once

#include <vector>

#include "Polygon2D.h"
#include <osg/Vec3d>
#include <COM/Export.h>

namespace COM
{

//二维三角形结构
struct Triangle
{
	osg::Vec2d a;
	osg::Vec2d b;
	osg::Vec2d c;
};


//三维三角形结构
struct Triangle3D
{
	osg::Vec3d a;
	osg::Vec3d b;
	osg::Vec3d c;
};

//二维多边形的三角剖分
class COM_EXPORT Triangulation2D
{
public:
	Triangulation2D(void);
	virtual ~Triangulation2D(void);

public:
	//三角剖分函数，输入时二维多边形，输出时生成的三角形列表的所有顶点
	std::vector<osg::Vec2d> triangulate(Polygon2D polygon2D);

private:
	//在三角形中寻找耳朵
	int findEar(Polygon2D polygon2D);
	
	//判断三个顶点中间的点是否是TipConvex
	bool isTipConvex(osg::Vec2d p0, osg::Vec2d p1,osg::Vec2d p2);

	//判断点是否在三角形内
	bool pointInTriangle(osg::Vec2d p,osg::Vec2d a,osg::Vec2d b,osg::Vec2d c);

	//判断前两个点是否在后两个点形成的线段的相同一侧
	bool pointOnSameSide(osg::Vec2d p1,osg::Vec2d p2,osg::Vec2d a,osg::Vec2d b);

	//计算两个二维矢量的叉积
	double crossProduct(osg::Vec2d p1,osg::Vec2d p2);

	//细分三角形
	void subdivideTriangles(std::vector<osg::Vec2d>& triangles);

	//两个向量的夹角
	double angleOfVector(osg::Vec2d v1, osg::Vec2d v2);

	//判断两个浮点数是否相同
	bool isSameValue(double x,double y);

private:
	int prevIndex,curIndex;
};

}