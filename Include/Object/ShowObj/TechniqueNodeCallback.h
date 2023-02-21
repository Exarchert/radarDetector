#pragma once
#include <osg/NodeCallback>
#include <osgFX/Technique>

class CTechniqueNodeCallback :
	public osg::NodeCallback
{
public:
	CTechniqueNodeCallback(osgFX::Technique* _technique);
	~CTechniqueNodeCallback(void);
	virtual void operator()(osg::Node* node ,osg::NodeVisitor* nv);

private:
	osgFX::Technique* _technique;
};
