/***********************************************************************
File:			COM_CartoonManager.h
Author:			��ƽ    
Date:			2009-03-31
Description:    ����������

ChangeLog:
����					����					����
-------------------------------------------------------------------------
��־��				2009-07-21			��飬ɾ������Ҫ�Ĵ���

************************************************************************/

#pragma once
#include <COM\Export.h>
#include <osg/ref_ptr>
#include <osg/Node>
#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>
#include <osgGA/GUIEventAdapter>
#include <osgGA/GUIActionAdapter>
#include <osgGA/GUIEventHandler>
#include <osgGA/EventQueue>
#include <COM\Cartoon\CartoonUnit.h>
#include <vector>

namespace COM{

class COM_EXPORT CCartoonManager : public osgGA::GUIEventHandler 
{
public:
	CCartoonManager(void);
	virtual ~CCartoonManager(void);

	//��Ӻ�ɾ����ͨ��Ԫ
	void AddCartoonUnit(CCartoonUnit *pCartoonUnit);
	void DelCartoonUnit(CCartoonUnit *pCartoonUnit);

	void Clear();

	//����ʱ��(delt)
	void Update(double delt);
	void SetPause();
	void SetPlay();

	//�¼�����
	bool handle(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa);
	//������������ֹͣ����
	void SetPushDone(bool done) { m_done = done; }

protected:
	std::vector<osg::ref_ptr<CCartoonUnit>> m_CartoonUnits;
	double	m_LastTime;
	bool	m_done;
};


}