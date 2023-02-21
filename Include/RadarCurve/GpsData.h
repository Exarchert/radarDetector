#pragma once
#include <RadarCurve/export.h>
#include <osg/Vec3d>

class RADAR_EXPORT GpsData
{
public:
	GpsData(void);
	~GpsData(void);

	void setReceiveData( osg::Vec3d const& value );

	DWORD getRecvTime()const;
	osg::Vec3d getPos()const;

protected:
	DWORD _reciveTime;
	osg::Vec3d _pos;
};
