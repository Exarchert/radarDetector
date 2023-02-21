#pragma once
#include <osg/NodeCallback>

namespace EMObj
{

class ShpNodeCallback  : public osg::NodeCallback
{
public:
	ShpNodeCallback(void);
	~ShpNodeCallback(void);

private:
	virtual void operator()(osg::Node* node, osg::NodeVisitor* nv);
	void computerPosition(osg::ref_ptr<osg::Geode> geode,osg::Matrixd& mx);
	void decompositionMatrixTransformModel(osg::ref_ptr<osg::Node> model,osg::Matrix mx);
	void decompositionModel(osg::ref_ptr<osg::Node> model);
	void switchStateSet();
	void updateStateSet();
private:
	bool _first;
	bool _switchStateSet;
	std::vector<osg::ref_ptr<osg::Geode>> _geodes;
	std::vector<osg::ref_ptr<osg::StateSet>> _geodeStateSet;
	double _lastUpdate;
	float m_flickLastTime;
	
};

}