/********************************************************************************
Copyright (C),  
文件：CCOM_ItemRunLine.h  CCOM_ItemRunLine.cpp

刘广慧				2011-1-18			生成xml格式脚本
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
		m_Name = _T("未命名运动线");

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
	目的：获取和设置运动线的信息
	*/
	BOOL GetInformation(osg::Vec4 &color, int &width);
	BOOL SetInformation(osg::Vec4 color, int width);

protected:
	osg::Vec3d	m_origin;			//当前坐标系的原点在世界坐标系中的位置
	osg::Vec3d	m_lastPosition;		//上一时刻点的位置(在当前坐标系中)
	osg::Vec4	m_color;
	int			m_width;
};

}