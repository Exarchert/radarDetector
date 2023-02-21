#pragma once

#include <vector>
#include <osg/Vec3d>
#include <COM/Export.h>

namespace COM	{
struct Segment
{
	osg::Vec3d p0;
	osg::Vec3d p1;
};

struct Line
{
	osg::Vec3d start;
	osg::Vec3d dir;

};

class COM_EXPORT PolyLine
{
public:
	PolyLine(void);
	PolyLine(std::vector<osg::Vec3d> vecPoints);
	~PolyLine(void);

public:
	void addPoint(osg::Vec3d point);
	std::vector<Segment> getSegmentList();

	double getLength();

private:
	std::vector<osg::Vec3d>  m_vecPoints;
};
}
