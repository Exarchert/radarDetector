/**************************************************************************
Copyright (C),  
�ļ���COM_ItemRunLine.h COM_ItemRunLine.cpp
���ߣ���־��
�汾��1.0
���ڣ�2009-07-29
��������ͨ��-��̬������

�޸ļ�¼��
����						����						����
---------------------------------------------------------------------------

**************************************************************************/

#pragma once

#include <temp\Export.h>
#include <vector>
#include <osg\Array>
#include <osg\Geode>
#include <osg\MatrixTransform>
#include <temp\Cartoon\COM_CartoonItem.h>


namespace TEMP		{

class TEMP_EXPORT CCOM_ItemRunLine : public CCOM_CartoonItem
{
public:
	CCOM_ItemRunLine() : m_color(1.f, 0.f, 0.f, 1.f), m_width(5)
	{
		m_Name = _T("δ�����˶���");

	}

	virtual ~CCOM_ItemRunLine();

	CCOM_ItemRunLine(const CCOM_ItemRunLine &line);

	virtual CCOM_CartoonItem * Clone();


	/*
	Ŀ�ģ�������Ԫִ����Ϊ
	������Time		(in)	����ڿ�ʼ�ʱ�̵�ʱ��
	      pos		(in)	������λ��ʱ��λ��			
	���أ�void
	*/
	virtual void DoAction(double Time, osg::Vec3d pos);

	//ִ�зǻʱ�����Ķ���
	virtual void DoNoneActive();

	/*
	Ŀ�ģ����л�����
	*/
	virtual void Serialize(CArchive& ar);

	/*
	Purpose:	Parser XML	Data Format for Animation
	Author:		Liugh
	*/
	virtual bool ParserXml(MSXML2::IXMLDOMNodePtr pNode);

	/*
	Ŀ�ģ���ȡ�������˶��ߵ���Ϣ
	*/
	BOOL GetInformation(osg::Vec4 &color, int &width);
	BOOL SetInformation(osg::Vec4 color, int width);

protected:
	DECLARE_SERIAL(CCOM_ItemRunLine)

protected:
	osg::ref_ptr<osg::MatrixTransform> m_arrowNode;
	osg::ref_ptr<osg::MatrixTransform> m_matrix;
	osg::ref_ptr<osg::Geode>		m_geode;
	osg::Vec3d	m_origin;			//��ǰ����ϵ��ԭ������������ϵ�е�λ��
	osg::Vec3d	m_lastPosition;		//��һʱ�̵��λ��(�ڵ�ǰ����ϵ��)
	osg::Vec4	m_color;
	int			m_width;
};

}