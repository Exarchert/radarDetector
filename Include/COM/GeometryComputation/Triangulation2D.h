#pragma once

#include <vector>

#include "Polygon2D.h"
#include <osg/Vec3d>
#include <COM/Export.h>

namespace COM
{

//��ά�����νṹ
struct Triangle
{
	osg::Vec2d a;
	osg::Vec2d b;
	osg::Vec2d c;
};


//��ά�����νṹ
struct Triangle3D
{
	osg::Vec3d a;
	osg::Vec3d b;
	osg::Vec3d c;
};

//��ά����ε������ʷ�
class COM_EXPORT Triangulation2D
{
public:
	Triangulation2D(void);
	virtual ~Triangulation2D(void);

public:
	//�����ʷֺ���������ʱ��ά����Σ����ʱ���ɵ��������б�����ж���
	std::vector<osg::Vec2d> triangulate(Polygon2D polygon2D);

private:
	//����������Ѱ�Ҷ���
	int findEar(Polygon2D polygon2D);
	
	//�ж����������м�ĵ��Ƿ���TipConvex
	bool isTipConvex(osg::Vec2d p0, osg::Vec2d p1,osg::Vec2d p2);

	//�жϵ��Ƿ�����������
	bool pointInTriangle(osg::Vec2d p,osg::Vec2d a,osg::Vec2d b,osg::Vec2d c);

	//�ж�ǰ�������Ƿ��ں��������γɵ��߶ε���ͬһ��
	bool pointOnSameSide(osg::Vec2d p1,osg::Vec2d p2,osg::Vec2d a,osg::Vec2d b);

	//����������άʸ���Ĳ��
	double crossProduct(osg::Vec2d p1,osg::Vec2d p2);

	//ϸ��������
	void subdivideTriangles(std::vector<osg::Vec2d>& triangles);

	//���������ļн�
	double angleOfVector(osg::Vec2d v1, osg::Vec2d v2);

	//�ж������������Ƿ���ͬ
	bool isSameValue(double x,double y);

private:
	int prevIndex,curIndex;
};

}