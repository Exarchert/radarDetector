#pragma once
#include <osg/Group>
#include <osg/Referenced>
#include <osg/Node>
#include <readShp\shp/Line.h>
#include <readShp/shp/PitchPoint.h>
#include <map>
#include <PipleManager/Obj.h>

#include <PipleManager/Export.h>
namespace PipleManager
{

class PipleManager_EXPORT LinePipe :
	public osg::Referenced
{
public:
	LinePipe(void);
	~LinePipe(void);
	void setLinePipeData( std::vector<ReadSql::Line*> lines, std::map<std::string,ReadSql::PitchPoint*> pitchPoints);
	
	bool getPipleObjList(std::map<std::string , osg::ref_ptr<Obj> >& pipleObjList);

private:
	void addLine();
	void addPitchPoint();
	void TestData();
	float randomValue( float min, float max );

	osg::Vec3 randomVector( float min, float max );
	//ReadSql::PitchPoint* getPitchPoint(LONG64 pitchPointID);
	std::map<std::string,ReadSql::PitchPoint*> _pitchPointSelect;
	std::vector<ReadSql::Line*> _lineVector;

	std::map<std::string,osg::ref_ptr<Obj>> _pipleObjList;
};
}

