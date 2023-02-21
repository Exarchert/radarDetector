#pragma once
#include <osg/StateSet>
#include <osg/TextureCubeMap>
#define PIPLEMANAGER_LIBRARY
#include <PipleManager/Export.h>

class PipleManager_EXPORT Water: public osg::StateSet
{
public:
	Water(void);
	~Water(void);

	void createWateState();
	osg::TextureCubeMap* readCubeMap();
};
