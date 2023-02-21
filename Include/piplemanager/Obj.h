#pragma once
#include <osg/Switch>
#include <osg/StateSet>
#include <PipleManager/Export.h>
namespace PipleManager
{

class PipleManager_EXPORT Obj :
	public osg::Switch
{
public:
	Obj(void);
	virtual ~Obj(void);
	void setStateSet(osg::StateSet* stateset);
	void clearStateSet();
private:
	std::vector<osg::ref_ptr<osg::StateSet>> _StateSets;
};
}