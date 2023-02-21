#pragma once
#include <osg/Switch>
#include <readShp/shp/PitchPoint.h>
#include <osg/MatrixTransform>
#include <PipleManager/Obj.h>
namespace PipleManager
{

class PipeFitting :
	public Obj
{
public:
	PipeFitting(void);
	~PipeFitting(void);
	void setPipeFitting(ReadSql::PitchPoint* pitchPoint);
private:
	osg::Node* GetPipeFlitting();

	osg::MatrixTransform* _mt;
};

}
