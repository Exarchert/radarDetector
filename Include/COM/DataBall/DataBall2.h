#pragma once 

#include <Com\Export.h>
#include <osg\Vec3>
#include <osg\Vec3d>
#include <osg\Matrixd>
#include <osg\Node>
#include <osgViewer\Viewer>
#include <osgGA\GUIEventAdapter>
#include <osgGA\GUIActionAdapter>
//#include <Com\Ramble\RambleManager.h>
#include <COM\Ramble\RambleMgr.h>


namespace COM	{

class COM_EXPORT CDataBall2 : public osg::Referenced
{
public:
	CDataBall2();
	virtual ~CDataBall2();

	
	void Init(osg::Node *node, const osg::Vec3d &pos,osg::Node *field);
													  //用于拖放计算位置
	void SetRambleManager(COM::CRambleMgr *rambleMan){_rambleMan = rambleMan;}

	bool handle(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa);

	void GetPosAndPose(osg::Vec3d &pos, osg::Vec3d &at, osg::Vec3d &up) const;
	void GetPosAndPose(osg::Vec3d &pos, float &yaw, float &pitch, float &roll);
	void GetLocalToWorld(osg::Matrixd &localToWorld) const;

	bool IsChanged(){return _change;} 
private:
	float getAngleStep(){return _angleStep;}
	float getDistanceStep(){return _distanceStep;}
	
	//osg::Node* getDataBall(){return _dataBall;}
	//osg::Node* getFteld(){return _field;}

private:
	float								_angleStep;
	float								_distanceStep;
	float								_scale;
	bool								_change;
	osg::Matrixd						_localToWorld;	
	osg::ref_ptr<COM::CRambleMgr>		_rambleMan;
	osg::Node*							_dataBall;
	osg::Node*							_field;
	osg::Vec3d							_r;
};

}