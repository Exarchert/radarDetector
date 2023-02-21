/************************************************************************
Copyright (C)  
�ļ���RambleBase.h RambleBase.cpp
���ߣ��ױ�
�汾��1.0
���ڣ�2009-06-18
���������β���������.

�޸ļ�¼��
����					����				����
-------------------------------------------------------------------------
��־��					2009-11-13			��ֲ�ϰ汾���޸Ĳ��ֳ�Ա����.

************************************************************************/

#pragma once

#include <COM\Export.h>
#include <osgGA\GUIEventHandler>
#include <osgGA\GUIActionAdapter>
#include <osgAPEx\EarthManipulator>


namespace COM	{

class COM_EXPORT CRambleBase : public osg::Referenced
{
public:
	CRambleBase(osgAPEx::EarthManipulator *earthMan);
	virtual ~CRambleBase();

	/*
	Ŀ�ģ���������OSG��Ϣ���е��¼�
	*/
	virtual bool handle(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa);

	/*
	Ŀ�ģ�����Windows���͵���Ϣ
	*/
	virtual bool handle(UINT msg, WPARAM wParam, LPARAM lParam);
	
	//���ӽǷ����ƶ�
	virtual void MoveAt(float fValue);

	//ˮƽ��ǰ�ƶ�
	virtual void MoveAhead(float fValue);

	//ˮƽ�����ƶ�
	virtual void MoveRightSide(float fValue);

	//�ı�ƫ���
	virtual void rotateYaw(float fValue);

	//�ı丩����
	virtual void rotatePitch(float fValue);

	//�ı�߶�
	virtual void MoveHeight(float fValue);
protected:
	osg::ref_ptr<osgAPEx::EarthManipulator> m_earthMan;
};

}