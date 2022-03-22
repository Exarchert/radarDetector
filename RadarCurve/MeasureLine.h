#pragma once
#include <osg/MatrixTransform>


class MeasureLine : public osg::MatrixTransform
{
public:
	MeasureLine(std::vector<osg::Vec3d> pTrackPoints, float pLineWidth, osg::Vec4f pColor);


	~MeasureLine(void);

private:
	void CreateLine(std::vector<osg::Vec3d> pTrackPoints, float pLineWidth, osg::Vec4f pColor);

};
