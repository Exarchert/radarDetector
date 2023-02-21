#pragma once
#include <osg/Switch>
#include <readShp/shp/Line.h>
#include <osg/MatrixTransform>
#include <PipleManager/Obj.h>

namespace PipleManager
{
class Pipe :
	public Obj
{
public:
	Pipe(void);
	~Pipe(void);
	void setPipeData(ReadSql::Line line);
private:
	ReadSql::Line _line;
	osg::MatrixTransform* _mt;
};

}