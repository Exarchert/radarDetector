/**************************************************************************
File:			PathEdit.h
Author:			��ƽ    
Date:			2011-1-17
Description:    ·���༭ģ��
***************************************************************************/

#pragma once
#include <COM\Export.h>
#include <osgGA\GUIEventHandler>
//#include <vector>
//#include <osg/Group>
//#include <osgDB/ReadFile>
//#include <osg/Image>
//#include <osgViewer/Viewer>

#include <COM\DataBall\DataBall.h>
#include <Com\KernelContext\KernelContext.h>

namespace COM{

class COM_EXPORT CWaterEdit : public osgGA::GUIEventHandler
{
public:
	CWaterEdit(void);
	~CWaterEdit(void);

	//��ʼ��
	bool Init(osg::Vec3d Pos, CString strNozzle, CString strDropPoint, CKernelContext* pKC);
	bool handle(const osgGA::GUIEventAdapter& ea,osgGA::GUIActionAdapter& aa);
protected:
	//��ͷ
	osg::ref_ptr<CDataBall>				m_NozzleBall;
	osg::ref_ptr<osg::MatrixTransform>	m_NozzleTrans;	//�任����
	osg::ref_ptr<osg::Switch>			m_NozzleSwitch;	//��������

	//���
	osg::ref_ptr<CDataBall>				m_DPBall;
	osg::ref_ptr<osg::MatrixTransform>	m_DPTrans;	//�任����
	osg::ref_ptr<osg::Switch>			m_DPSwitch;	//��������

	//�豸�����Ķ���
	CKernelContext* m_pKC;

};
}