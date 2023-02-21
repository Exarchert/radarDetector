/*********************************************************************
Copyright (C),  
�ļ���DecoraBase.h DecoraBase.cpp
���ߣ���־��
�汾��1.0
���ڣ�2009-11-16
������װ�ζ������.

�޸ļ�¼��
����					����				����
----------------------------------------------------------------------

*********************************************************************/

#pragma once 

#include <Com\Export.h>
#include <osg\Referenced>
#include <osgGA\GUIEventAdapter>
#include <osgGA\GUIActionAdapter>


namespace COM	{

class COM_EXPORT CDecoraBase : public osg::Referenced
{
public:
	CDecoraBase();
	virtual ~CDecoraBase();

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

	/*
	Ŀ�ģ���ȡ����״̬
	���أ�true��ʾ��ʾ, false��ʾ����
	*/
	virtual bool IsShow() const;

protected:
	bool	m_bShow;
};

}