#pragma once
#include <osg/Switch>
#include <Object/Export.h>
class OBJECT_EXPORT Steetscape :
	public osg::Switch
{
public:
	Steetscape(void);
	~Steetscape(void);
private:
	void init();
	osg::ref_ptr<osg::Geometry> createPlane( float length, float width );

};
