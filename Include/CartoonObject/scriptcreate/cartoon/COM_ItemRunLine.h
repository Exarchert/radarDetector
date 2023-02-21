/********************************************************************************
Copyright (C),  
�ļ���CCOM_ItemRunLine.h  CCOM_ItemRunLine.cpp

�����				2011-1-18			����xml��ʽ�ű�
********************************************************************************/

#pragma once

#include <CartoonObject\Export.h>
#include <CartoonObject\ScriptCreate\Cartoon\COM_Cartoon.h>
#include <vector>

namespace CartoonObject		{

class CARTOONOBJECT_EXPORT CCOM_ItemRunLine : public CCOM_CartoonItem
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
	Purpose:	Parser XML	Data Format for Animation
	Author:		Liugh
	*/
	virtual bool ParserXml(MSXML2::IXMLDOMNodePtr pNode);

	/*
	Purpose:	Return XML	Node
	Author:		Liugh
	*/
	virtual MSXML2::IXMLDOMNodePtr GetXmlNode();

	/*
	Ŀ�ģ���ȡ�������˶��ߵ���Ϣ
	*/
	BOOL GetInformation(osg::Vec4 &color, int &width);
	BOOL SetInformation(osg::Vec4 color, int width);

protected:
	osg::Vec3d	m_origin;			//��ǰ����ϵ��ԭ������������ϵ�е�λ��
	osg::Vec3d	m_lastPosition;		//��һʱ�̵��λ��(�ڵ�ǰ����ϵ��)
	osg::Vec4	m_color;
	int			m_width;
};

}