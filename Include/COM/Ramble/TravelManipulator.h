#ifndef __TRAVEL_MANIPULATOR_H__

#include "CameraCallBack.h"
#include "CameraState.h"

#include <osgGA/GUIEventAdapter>
#include <osgViewer/ViewerEventHandlers>
#include <osgGA/matrixmanipulator>

class COM_EXPORT TravelManipulator : public osgGA::MatrixManipulator//osgGA::CameraManipulator
{
public:
	TravelManipulator(osgViewer::Viewer * viewer);
	~TravelManipulator();

public:
	virtual void setByMatrix(const osg::Matrix& matrix); //设置当前视口
	virtual  void setByInverseMatrix(const osg::Matrix& matrix);
	virtual osg::Matrixd getMatrix() const;  //得到当前矩阵
	virtual osg::Matrixd getInverseMatrix() const;

	virtual bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa); //响应事件

	void setPosition(osg::Vec3d &position);  //设置到某一点
	osg::Vec3d getPosition();

	void setRotation(osg::Vec3d &rotation);	 //设置到某一旋转角
	osg::Vec3d getRotation();

	void setStep(int step);	 //设置移动步长
	int getStep();

	void setRotationStep(float &rotationStep);	//设置旋转步长
	float getRotationStep();
private:
	void ChangePosition(osg::Vec3d &delta);	 //改变位置

private:
	osg::Vec3 m_vPosition;	//视点
	osg::Vec3 m_vRotation;	//朝向
	float m_vStep;	        //移动步长
	float m_vRotationStep;	//旋转步长

	float m_jd;//

	int m_iLeftX;	//记录坐标
	int m_iLeftY;
	bool m_bLeftDown;
	unsigned int m_type;

	osg::ref_ptr<osgViewer::Viewer> _viewer;
	osg::ref_ptr<CameraCallback> _call_back;
};

#define __TRAVEL_MANIPULATOR_H__
#endif