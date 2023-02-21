#pragma once
#include <Com\Export.h>
#include <osgGA/MatrixManipulator>

namespace COM
{

class COM_EXPORT CBaseRamble : public osgGA::MatrixManipulator
{
public:
	CBaseRamble(void);
	~CBaseRamble(void);

	virtual const char* className() const { return "base"; }

	virtual void setByMatrix(const osg::Matrixd& matrix) = 0;

	virtual void setByInverseMatrix(const osg::Matrixd& matrix) = 0;

	virtual osg::Matrixd getMatrix() const = 0;

	virtual osg::Matrixd getInverseMatrix() const = 0;

	virtual bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa) = 0;

	virtual void SetFreeSetting(CString strSetting) = 0;
	//	向前移动
	virtual void LevelForward(bool bForward = true) = 0;
	//	向左移动
	virtual void LevelLeft(bool bForward = true) = 0;

	virtual void rotateYaw(bool bPositive = true) = 0;

	virtual void rotatePitch(bool bPositive = true) = 0;

	virtual void MoveAt(bool bPositive = true) = 0;

	virtual void North() = 0;

	bool EnableRamble(bool bEnable=true);
	/*
		设置相机的位置姿态
		pos			(in)	相机位置
		at/up		(in)	相机姿态
	*/
	void setPosPose(const osg::Vec3d& pos, const osg::Vec3& at, const osg::Vec3& up);

	/*
		设置相机位置
	*/
	void setPos(const osg::Vec3d& pos);

	/*
		设置相机的位置姿态
		lon/lat/height	(in)	相机位置,经纬高
		yaw/pitch/roll	(in)	相机在本地坐标系的姿态角
	*/
	void setPosPose(double lon, double lat, double height, float yaw, float pitch, float roll);

	/*
		设置相机的姿态,保持位置不变
		yaw/pitch/roll	(in)	相机在本地坐标系的姿态角
	*/
	void setPose(float yaw, float pitch, float roll);

	/*
		获取相机的位置
		lon/lat/height	(out)	相机位置,经纬高
	*/
	void getPos(double& lon, double& lat, double& height);

	/*
		获取相机的位置
		pos				(out)	相机在世界作标系中的位置
	*/
	void getPos(osg::Vec3d& pos);

	/*
		获取相机姿态
		at/up			(out)	相机在世界坐标系中的姿态向量
	*/
	void getPose(osg::Vec3& at, osg::Vec3& up);

	/*
		获取相机姿态
		yaw/pitch/roll	(out)	相机在本地坐标系中的姿态角
	*/
	void getPose(float& yaw, float& pitch, float& roll);


protected:
	osg::Vec3d	_pos;
	osg::Vec3	_at;
	osg::Vec3	_up;

	bool m_bEnable;
};

}