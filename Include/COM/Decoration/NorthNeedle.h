/*********************************************************************
Copyright (C),  
�ļ���NorthNeedle.h NorthNeedle.cpp
���ߣ������
�汾��1.0
���ڣ�2009-11-16
������ָ���������.

�޸ļ�¼��
����					����				����
----------------------------------------------------------------------
��־��					2009-11-16			��ֲ���˹���

*********************************************************************/

#pragma once

#include <osg\Vec2>
#include <osg\Vec3>
#include <osg\Switch>
#include <osg\Camera>
#include <osg\MatrixTransform>
#include <osgAPEx\EarthManipulator>
#include <Com\Decoration\DecoraBase.h>
#include <Com\Ramble\RambleMgr.h>


namespace COM	{

class COM_EXPORT CNorthNeedle : public CDecoraBase
{
public:
	CNorthNeedle();
	virtual ~CNorthNeedle();

	/*
	Ŀ�ģ���������OSG��Ϣ���е��¼�
	*/
	virtual bool handle(const osgGA::GUIEventAdapter& ea,osgGA::GUIActionAdapter& aa);

	/*
	Ŀ�ģ���������
	������bShow		(in)	true:��ʾ; false:����
	���أ�����ǰ��״̬
	*/
	virtual bool Show(bool bShow=true);

	/*
	Ŀ�ģ���ʼ��
	������earthMan	(in)	���������
		  hud		(in)	HUD���ָ��
	      texfile	(in)	��ͼ�ļ���
		  scale		(in)	��ԭʼͼƬ�Ŵ���
		  rmargin	(in)	�ұ߾�
		  tmargin	(in)	�ϱ߾�
	���أ��ɹ�����true��ʧ�ܷ���false.
	*/
	//bool init(osgAPEx::EarthManipulator *earthMan, osg::Camera *hud, std::string &texfile, float scale,
	//		float rmargin, float tmargin);
	bool init(COM::CRambleMgr *earthMan, osg::Camera *hud, std::string &texfile, float scale,
		float rmargin, float tmargin);

protected:
	//����һ����
	osg::Geode* createQuad(float width, float height, float z);

protected:
	osg::Camera								*m_hud;		//HUD
	osg::ref_ptr<COM::CRambleMgr>			m_earthMgr;	//���������
	osg::ref_ptr<osg::Switch>				m_root;		//ָ��������
	osg::ref_ptr<osg::MatrixTransform>		m_mat;		//װ�����
	osg::Vec2								m_margin;	//�߾�
	osg::Vec2								m_size;		//ָ����Ĵ�С
	osg::Vec3								m_pose;		//�����̬
	float									m_speed;	//��ת����
	bool									m_blaunch;	//����ת
};

}