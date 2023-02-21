#ifndef __RADAR_CURVE_DB_ROW_H_FILE__
#define __RADAR_CURVE_DB_ROW_H_FILE__


#include <RadarCurve/export.h>
#include <osg/Referenced>

class RADAR_EXPORT DBRow : public osg::Referenced
{
public:
	DBRow()
	{
		_id = 0;
	}
	virtual ~DBRow()
	{

	}

	int getID()const
	{
		return _id;
	}
	void setID( int value )
	{
		_id = value;
	}

protected:
	int _id;

};

#endif //__RADAR_CURVE_DB_ROW_H_FILE__