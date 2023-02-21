/*************************************************
file:			MathApi.h
Author:			张平    
Date:			2009-03-31
Description:    数学公式类
*************************************************/
#pragma once
#include <COM\Export.h>
#include <osg/Vec3>
#include <osg/Vec3d>
#include <osg/Vec4>
#include <osg/Matrix>
#include <osg/Matrixf>
#include <osgUtil/LineSegmentIntersector>

namespace COM	{

class COM_EXPORT MathApi
{
public:
	MathApi(void);
	~MathApi(void);


	/*
	直线与直线的距离
	line0		(in)	直线0
	line1		(in)	直线1
	dis			(out)	距离
	pQ0			(out)	直线0上的垂足
	pQ1			(out)	直线1上的垂足
	返回值：0，表示非平行；1，表示平行，平行时候垂足无效
	*/
	static int LineLineDis(osgUtil::LineSegmentIntersector& line0, osgUtil::LineSegmentIntersector& line1, double &dis, osg::Vec3d *pQ0 = NULL, osg::Vec3d *pQ1 = NULL);

	/*
	射线与线段的距离
	ray		(in)	射线
	seg		(in)	线段
	dis		(out)	距离
	返回值：0，表示垂足在线段的中间和射线正方向上；
	1，平行
	2，表示垂足在线段的中间和射线反方向上
	3，表示垂足在线段的延长线上和射线正方向上
	4，表示垂足在线段的延长线上和射线反方向上
	*/

	static int RaySegmentDis(osg::Vec3d& RayStart, osg::Vec3d& vec, osgUtil::LineSegmentIntersector& seg, double &dis, osg::Vec3d& RayCross, osg::Vec3d& SegCross);

	//static double RaySegmentDis(osgUtil::LineSegmentIntersector& ray, osgUtil::LineSegmentIntersector& seg);

	//去除折线数组上多余的显示点，主要是去掉多个点在一条直线上的情况
	static void RomoveShowPoint(std::vector<osg::Vec3d> &inPoints, std::vector<osg::Vec3d> &outPoints);

	//是否平行
	static bool IsParallel(osg::Vec3d Vec1, osg::Vec3d Vec2);

	//是否相等
	static bool IsEqual(osg::Vec3d pos1, osg::Vec3d pos2);

	/*
	直线和平面的交集
	LineP		(in)	直线起点
	LineRay		(in)	直线向量
	PlaneP		(in)	平面点
	PlaneN		(in)	平面向量
	Q			(out)	交点
	返回值：0，表示平行
	1，相交
	*/
	static int LineAndPlane(osg::Vec3d LineP, osg::Vec3d LineRay, osg::Vec3d PlaneP, osg::Vec3d PlaneN, osg::Vec3d& Q);
};

}