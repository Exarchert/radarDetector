/*********************************************************************
Copyright (C),  
�ļ���InitHud.h InitHud.cpp
���ߣ���־��
�汾��1.0
���ڣ�2009-11-13
��������ʼ��HUD.

�޸ļ�¼��
����					����				����
----------------------------------------------------------------------
��־��					2009-11-16			����CHudResizeHandler��

*********************************************************************/

#pragma once

#include <Kernel\Export.h>
#include <Kernel\Init\InitModule.h>
#include <osg\Camera>
#include <osgGA\GUIEventHandler>
#include <osgGA\GUIEventAdapter>
#include <osgGA\GUIActionAdapter>


namespace Kernel	{

//CInitHud��
class CInitHud : public CInitModule
{
public:
	CInitHud();
	virtual ~CInitHud();

	/*
	Ŀ�ģ�ִ�г�ʼ��
	���أ��ɹ�����TRUE�����򷵻�FALSE
	*/
	virtual BOOL InitInstance(COM::CKernelContext &kContext);

	/*
	Ŀ�ģ���ʼ����������
	*/
	virtual void InitEnviroment();
};


//CHudResizeHandler��
class CHudResizeHandler : public osgGA::GUIEventHandler
{
public:
	//bAuto		(in)	�Զ�����ͶӰ����
	CHudResizeHandler(osg::Camera *hudCamera, bool bAuto=false, bool centerAligned=false);
	virtual ~CHudResizeHandler();

	virtual bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa);

protected:
	osg::Camera *m_hudCamera;
	bool		m_bAuto;
	bool		m_bCenterAligned;
};

}
