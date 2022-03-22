#include "StdAfx.h"
#include "MeasureLine.h"
#include <osg/Geode>
#include <osg/Geometry>
#include <osgDB/WriteFile>
#include <osgDB/ReadFile>
#include <osg/LineWidth>
#include <osg/Point>
#include <Object\CmdCenter\CmdCenter.h>

#include <osgAPEx/CoordinateTool>
MeasureLine::MeasureLine(std::vector<osg::Vec3d> pTrackPoints, float pLineWidth, osg::Vec4f pColor)
{

	CreateLine(pTrackPoints, pLineWidth, pColor);
}

MeasureLine::~MeasureLine(void)
{

}

void MeasureLine::CreateLine(std::vector<osg::Vec3d> pTrackPoints, float pLineWidth, osg::Vec4f pColor)
{
	//计算道路的局部矩阵
	osg::Matrixd toOriginMat;
	osg::Vec3 LLA = pTrackPoints[0];
	osg::MatrixTransform* MT = new osg::MatrixTransform;

	osg::Matrixd localToWorld;
	osg::Matrixd worldToLocal;
	osgAPEx::ComputeLocalToWorldTransformFromLongLatHeight(LLA.x(),LLA.y(),LLA.z(), localToWorld );
	MT->setMatrix(localToWorld);//ok
	//bool aaa = MT->addChild(osgDB::readNodeFile("D:\\GDJW_Data\\osgData\\cow.osg"));
	worldToLocal = osg::Matrixd::inverse(localToWorld);
	EMObj::CmdObjLocate(LLA, EMObj::CObj::LocateStyle_Normal);
	osg::Vec3Array* linePoints = new osg::Vec3Array;
	//以该条道路的第一个点为原点（0，0，0）计算其他点相对该点的局部坐标
	for(int i = 0; i != pTrackPoints.size(); ++i)
	{
		osg::Vec3d xyz;
		osgAPEx::ConvertLongLatHeightToXYZ( pTrackPoints[i].x(), pTrackPoints[i].y(), pTrackPoints[i].z(), xyz );
		osg::Vec3d localPos = xyz * worldToLocal;
		linePoints->push_back(localPos);
	}

	osg::Geode* pGeode = new osg::Geode;
	osg::Geometry* geom = new osg::Geometry;

	geom->setVertexArray(linePoints);
 	osg::Vec4Array* pColorArray = new osg::Vec4Array;
	pColorArray->push_back(pColor);
 	geom->setColorArray(pColorArray);
	geom->setColorBinding(osg::Geometry::BIND_OVERALL);

	osg::ref_ptr<osg::Point> pSize = new osg::Point;
	pSize->setSize(12.0);
	geom->getOrCreateStateSet()->setAttributeAndModes(pSize.get(),osg::StateAttribute::ON);


	osg::ref_ptr<osg::LineWidth> pLineWidthmode = new osg::LineWidth;
	pLineWidthmode->setWidth(pLineWidth);
	geom->getOrCreateStateSet()->setAttributeAndModes(pLineWidthmode.get(),osg::StateAttribute::ON);

	geom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINE_STRIP,0,linePoints->size()));
	geom->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);
	pGeode->addDrawable(geom);
	MT->addChild(pGeode);
	addChild(MT);
	
}