#include "StdAfx.h"
#include <RadarCurve/GpsData.h>
#include <time.h>

GpsData::GpsData(void)
{
	_reciveTime = 0;
}

GpsData::~GpsData(void)
{
}


void GpsData::setReceiveData( osg::Vec3d const& value )
{
	_pos = value;
	_reciveTime = GetTickCount();

}
DWORD GpsData::getRecvTime()const
{
	return _reciveTime;

}

osg::Vec3d GpsData::getPos()const
{
	return _pos;

}
