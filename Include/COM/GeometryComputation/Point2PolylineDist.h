#pragma once

#include "PolyLine.h"
#include <osg/Vec3d>
#include <COM/Export.h>
namespace COM
{

class COM_EXPORT Point2PolylineDist
{
public:
	Point2PolylineDist(void);
	~Point2PolylineDist(void);

public:
	double compute(osg::Vec3d point, PolyLine polyLine);

private:
	double point2SegmentDist(osg::Vec3d point, Segment segment);
	double point2LineDist(osg::Vec3d point, Line line, float& t);
};

}