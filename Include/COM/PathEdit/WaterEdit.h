/**************************************************************************
File:			PathEdit.h
Author:			张平    
Date:			2011-1-17
Description:    路径编辑模块
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

	//初始化
	bool Init(osg::Vec3d Pos, CString strNozzle, CString strDropPoint, CKernelContext* pKC);
	bool handle(const osgGA::GUIEventAdapter& ea,osgGA::GUIActionAdapter& aa);
protected:
	//喷头
	osg::ref_ptr<CDataBall>				m_NozzleBall;
	osg::ref_ptr<osg::MatrixTransform>	m_NozzleTrans;	//变换矩阵
	osg::ref_ptr<osg::Switch>			m_NozzleSwitch;	//显隐控制

	//落点
	osg::ref_ptr<CDataBall>				m_DPBall;
	osg::ref_ptr<osg::MatrixTransform>	m_DPTrans;	//变换矩阵
	osg::ref_ptr<osg::Switch>			m_DPSwitch;	//显隐控制

	//设备上下文对象
	CKernelContext* m_pKC;

};
}