/************************************************************************
Copyright (C),  
�ļ���SunShine.h  SunShine.cpp
���ߣ���־��
�汾��1.0
���ڣ�2009-08-20
������̫��������.

�޸ļ�¼��
����					����				����
-------------------------------------------------------------------------
��־��					2010-01-04			��ֲ���˹���

************************************************************************/

#pragma once

#include <osg\Vec3d>
#include <osg\Switch>
#include <osg\Camera>
#include <osgAPEx\Sunshine>
#include <Com\Decoration\DecoraBase.h>


namespace COM	{

class COM_EXPORT CSunShine : public CDecoraBase
{
public:
	/*
	Ŀ�ģ����캯��
	*/
	CSunShine();

	virtual ~CSunShine();

	/*
	Ŀ�ģ���ʼ��
	������fieldPos	(in)	����λ��(��γ��)
	      distance	(in)	̫�����볧���ľ���
		  texfile	(in)	��ͼ�ļ�·��
	      hud		(in)	hudָ��
	      camera	(in)	�����ָ��
	      curTime	(in)	ʱ��(���ڼ�����η���, ȡֵ6~18)
	���أ��ɹ�����true��ʧ�ܷ���false
	*/
	bool Init(osg::Vec3d fieldPos, double distance, std::string texfile, osg::Camera *hud, osg::Camera *camera,
		float curTime);

	/*
	Ŀ�ģ���������OSG��Ϣ���е��¼�
	*/
	virtual bool handle(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa);

	/*
	Ŀ�ģ���������
	������bShow		(in)	true:��ʾ; false:����
	���أ�����ǰ��״̬
	*/
	virtual bool Show(bool bShow=true);

protected:
	osg::Camera							*m_hud;				//HUD���
	osg::ref_ptr<osg::Switch>			m_switchSunshine;	//���ε�switch
	osg::ref_ptr<osgAPEx::Sunshine>		m_sunShine;			//����
};

}