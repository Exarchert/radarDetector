/*********************************************************************
Copyright (C),  
�ļ���ControlPanel.h ControlPanel.cpp
���ߣ������
�汾��1.0
���ڣ�2009-07-25
������������������.

�޸ļ�¼��
����					����				����
----------------------------------------------------------------------
��־��					2009-11-16			��ֲ���˹���

*********************************************************************/

#pragma once

#include <Com\Export.h>
#include <Com\Decoration\DecoraBase.h>
#include <Com\Ramble\RambleMgr.h>
#include <osg\Vec2>
#include <osg\Camera>
#include <osgAPEx\ControlPanel>
#include <osgWidget\WindowManager>


namespace COM	{

class COSGControlPanel;

//CControlPanel��
class COM_EXPORT CControlPanel : public CDecoraBase
{
public:
	CControlPanel();
	virtual ~CControlPanel();

	bool init(CRambleMgr *rambleMan, osgWidget::WindowManager *wm, osg::Camera *hud, const std::string& texfilepath,
			float rmargin, float tmargin);

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
	osg::ref_ptr<osgWidget::WindowManager>	m_wm;
	osg::ref_ptr<osg::Camera>				m_hud;
	osg::ref_ptr<COSGControlPanel>			m_crtPanel;
	osg::ref_ptr<CRambleMgr>			m_rambleMan; //���ι�����
	osg::Vec2								m_margin;
	bool									m_picked;
};


//�̳�������Ŀ���������
class COSGControlPanel : public osgAPEx::ControlPanel
{
public:
	COSGControlPanel();
	virtual ~COSGControlPanel();

	bool init(CRambleMgr *rambleMan, const std::string& texfilepath);

protected:
	virtual void moveAhead(double dt);

	virtual void moveBack(double dt);

	virtual void moveLeft(double dt);

	virtual void moveRight(double dt);

	virtual void moveUp(double dt);

	virtual void moveDown(double dt);

	virtual void rotateLeft(double dt);

	virtual void rotateRight(double dt);

	virtual void toNorth();

	virtual void toVertical();

protected:
	osg::ref_ptr<CRambleMgr>	m_rambleMan; //���ι�����

	float	_scale;
};

}