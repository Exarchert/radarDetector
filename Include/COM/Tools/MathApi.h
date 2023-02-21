/*************************************************
file:			MathApi.h
Author:			��ƽ    
Date:			2009-03-31
Description:    ��ѧ��ʽ��
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
	ֱ����ֱ�ߵľ���
	line0		(in)	ֱ��0
	line1		(in)	ֱ��1
	dis			(out)	����
	pQ0			(out)	ֱ��0�ϵĴ���
	pQ1			(out)	ֱ��1�ϵĴ���
	����ֵ��0����ʾ��ƽ�У�1����ʾƽ�У�ƽ��ʱ������Ч
	*/
	static int LineLineDis(osgUtil::LineSegmentIntersector& line0, osgUtil::LineSegmentIntersector& line1, double &dis, osg::Vec3d *pQ0 = NULL, osg::Vec3d *pQ1 = NULL);

	/*
	�������߶εľ���
	ray		(in)	����
	seg		(in)	�߶�
	dis		(out)	����
	����ֵ��0����ʾ�������߶ε��м�������������ϣ�
	1��ƽ��
	2����ʾ�������߶ε��м�����߷�������
	3����ʾ�������߶ε��ӳ����Ϻ�������������
	4����ʾ�������߶ε��ӳ����Ϻ����߷�������
	*/

	static int RaySegmentDis(osg::Vec3d& RayStart, osg::Vec3d& vec, osgUtil::LineSegmentIntersector& seg, double &dis, osg::Vec3d& RayCross, osg::Vec3d& SegCross);

	//static double RaySegmentDis(osgUtil::LineSegmentIntersector& ray, osgUtil::LineSegmentIntersector& seg);

	//ȥ�����������϶������ʾ�㣬��Ҫ��ȥ���������һ��ֱ���ϵ����
	static void RomoveShowPoint(std::vector<osg::Vec3d> &inPoints, std::vector<osg::Vec3d> &outPoints);

	//�Ƿ�ƽ��
	static bool IsParallel(osg::Vec3d Vec1, osg::Vec3d Vec2);

	//�Ƿ����
	static bool IsEqual(osg::Vec3d pos1, osg::Vec3d pos2);

	/*
	ֱ�ߺ�ƽ��Ľ���
	LineP		(in)	ֱ�����
	LineRay		(in)	ֱ������
	PlaneP		(in)	ƽ���
	PlaneN		(in)	ƽ������
	Q			(out)	����
	����ֵ��0����ʾƽ��
	1���ཻ
	*/
	static int LineAndPlane(osg::Vec3d LineP, osg::Vec3d LineRay, osg::Vec3d PlaneP, osg::Vec3d PlaneN, osg::Vec3d& Q);
};

}