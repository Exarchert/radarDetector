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
	virtual void setByMatrix(const osg::Matrix& matrix); //���õ�ǰ�ӿ�
	virtual  void setByInverseMatrix(const osg::Matrix& matrix);
	virtual osg::Matrixd getMatrix() const;  //�õ���ǰ����
	virtual osg::Matrixd getInverseMatrix() const;

	virtual bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa); //��Ӧ�¼�

	void setPosition(osg::Vec3d &position);  //���õ�ĳһ��
	osg::Vec3d getPosition();

	void setRotation(osg::Vec3d &rotation);	 //���õ�ĳһ��ת��
	osg::Vec3d getRotation();

	void setStep(int step);	 //�����ƶ�����
	int getStep();

	void setRotationStep(float &rotationStep);	//������ת����
	float getRotationStep();
private:
	void ChangePosition(osg::Vec3d &delta);	 //�ı�λ��

private:
	osg::Vec3 m_vPosition;	//�ӵ�
	osg::Vec3 m_vRotation;	//����
	float m_vStep;	        //�ƶ�����
	float m_vRotationStep;	//��ת����

	float m_jd;//

	int m_iLeftX;	//��¼����
	int m_iLeftY;
	bool m_bLeftDown;
	unsigned int m_type;

	osg::ref_ptr<osgViewer::Viewer> _viewer;
	osg::ref_ptr<CameraCallback> _call_back;
};

#define __TRAVEL_MANIPULATOR_H__
#endif