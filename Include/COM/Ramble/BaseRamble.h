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
	//	��ǰ�ƶ�
	virtual void LevelForward(bool bForward = true) = 0;
	//	�����ƶ�
	virtual void LevelLeft(bool bForward = true) = 0;

	virtual void rotateYaw(bool bPositive = true) = 0;

	virtual void rotatePitch(bool bPositive = true) = 0;

	virtual void MoveAt(bool bPositive = true) = 0;

	virtual void North() = 0;

	bool EnableRamble(bool bEnable=true);
	/*
		���������λ����̬
		pos			(in)	���λ��
		at/up		(in)	�����̬
	*/
	void setPosPose(const osg::Vec3d& pos, const osg::Vec3& at, const osg::Vec3& up);

	/*
		�������λ��
	*/
	void setPos(const osg::Vec3d& pos);

	/*
		���������λ����̬
		lon/lat/height	(in)	���λ��,��γ��
		yaw/pitch/roll	(in)	����ڱ�������ϵ����̬��
	*/
	void setPosPose(double lon, double lat, double height, float yaw, float pitch, float roll);

	/*
		�����������̬,����λ�ò���
		yaw/pitch/roll	(in)	����ڱ�������ϵ����̬��
	*/
	void setPose(float yaw, float pitch, float roll);

	/*
		��ȡ�����λ��
		lon/lat/height	(out)	���λ��,��γ��
	*/
	void getPos(double& lon, double& lat, double& height);

	/*
		��ȡ�����λ��
		pos				(out)	�������������ϵ�е�λ��
	*/
	void getPos(osg::Vec3d& pos);

	/*
		��ȡ�����̬
		at/up			(out)	�������������ϵ�е���̬����
	*/
	void getPose(osg::Vec3& at, osg::Vec3& up);

	/*
		��ȡ�����̬
		yaw/pitch/roll	(out)	����ڱ�������ϵ�е���̬��
	*/
	void getPose(float& yaw, float& pitch, float& roll);


protected:
	osg::Vec3d	_pos;
	osg::Vec3	_at;
	osg::Vec3	_up;

	bool m_bEnable;
};

}