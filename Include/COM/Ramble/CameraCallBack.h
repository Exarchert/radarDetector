#ifndef __CAMERA_CALL_BACK_H__
#define __CAMERA_CALL_BACK_H__

#include "CameraState.h"
#include <osg/NodeCallback>
#include <osg/Node>
#include <osg/NodeVisitor>
#include <osgViewer/Viewer>

class CameraCallback : public osg::NodeCallback
{
public:
	CameraCallback(osgViewer::Viewer * viewer, float step, osg::Vec3 * pos);
	void operator()(osg::Node* node, osg::NodeVisitor* nv);
	void setState(int state);
	void setmvStep(float mvStep);
private:
	float m_vStep; //ÒÆ¶¯²½³¤
	osg::Vec3 * m_vPosition;
	int _state;
	osg::ref_ptr<osgViewer::Viewer> _viewer;
};

#endif