#pragma once
#include <readShp/shp/Line.h>
#include <readShp/shp/PitchPoint.h>
#include <vector>
#include <string>
#include <map>
#include "Export.h"
namespace PipleManager
{

class PipleManager_EXPORT LinePipeDataProcessing
{
public:
	LinePipeDataProcessing(void);
	~LinePipeDataProcessing(void);
	void setLinePipeData(std::vector<ReadSql::Line*>& lines,
		std::vector<ReadSql::PitchPoint*>& pitchPoints);
	std::map<std::string,ReadSql::PitchPoint*> getPitchPointProces();
private:
	void pitchPointProces(ReadSql::PitchPoint* pitchPoint,std::vector<ReadSql::Line*> startChildLines,
		std::vector<ReadSql::Line*> endChildLines);
	void pitchPointProces(ReadSql::PitchPoint* pitchPoint,std::vector<ReadSql::Line*> ChildLines);

	bool getDirection(ReadSql::Coordinate center,ReadSql::Line* line,
		ReadSql::Coordinate &direction);

	bool pipleLineCut(ReadSql::Coordinate center,std::vector<ReadSql::Line*> lines);


	std::map<std::string,ReadSql::PitchPoint*> _pitchPoints;

};

}
