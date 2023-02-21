/**********************************************************************************
File:			COM_CartoonManager.h
Author:			��ƽ    
Date:			2009-03-31
Description:    ����������

ChangeLog:
����					����					����
-----------------------------------------------------------------------------------
��־��				2009-07-21			��飬ɾ������Ҫ�Ĵ���
��־��				2009-11-08			���CCOM_PassiveCartoonPlayer
��־��				2009-12-29			��Ӵ��¼��б����й���

Liugh				2010.11.25			Add All XML Parser Function For Each Script
�����				2011-1-12			����xml��ʽ�ű�
***********************************************************************************/

#pragma once

#include <CartoonObject/Export.h>
#include <osg/ref_ptr>
#include <osg/Node>
#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>
#include <osgGA/GUIEventAdapter>
#include <osgGA/GUIActionAdapter>
#include <osgGA/EventQueue>
#include <CartoonObject/ScriptCreate/Cartoon/COM_CartoonUnit.h>


namespace CartoonObject		{

class CARTOONOBJECT_EXPORT CCOM_CartoonManager
{
public:
	CCOM_CartoonManager(void);
	virtual ~CCOM_CartoonManager(void);

	//��Ӻ�ɾ����ͨ��Ԫ
	void AddCartoonUnit(CCOM_CartoonUnit *pCartoonUnit);
	void DelCartoonUnit(CCOM_CartoonUnit *pCartoonUnit);

	void Clear();

	//����ӰƬ���Ѳ���ʱ��
	void SetPlayTime(double time);

	//��ȡ�����ܳ�
	double GetScriptTimeLength(int index = 0);

	//��ȡ��ǰ����ʱ��
	double GetCurPlayTime(int index = 0);

protected:
	std::vector<osg::ref_ptr<CCOM_CartoonUnit>> m_CartoonUnits;
	double m_LastTime;
};
}