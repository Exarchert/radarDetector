/********************************************************************************
Copyright (C),  
文件：CCOM_ItemFlow.h  CCOM_ItemFlow.cpp

刘广慧				2011-1-18			生成xml格式脚本
********************************************************************************/
#pragma once 

#include <CartoonObject/Export.h>
#include <CartoonObject/ScriptCreate/Cartoon/COM_CartoonItem.h>
#include <osg\ref_ptr>
#include <osg\Vec4>


namespace CartoonObject		{

	class CARTOONOBJECT_EXPORT CCOM_ItemLabel : public CCOM_CartoonItem
	{
	public:
		CCOM_ItemLabel() : m_image(_T("osg64.png")), m_text(_T("")), m_font(_T("SIMSUN.TTC"))
			, m_color(1.f, 0.f, 0.f, 1.f), m_size(30.f), m_offsetX(30.f), m_offsetY(30.f)
		{
			m_Name = _T("未命名标牌");

		}

		virtual ~CCOM_ItemLabel();

		CCOM_ItemLabel(const CCOM_ItemLabel &label);

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
		目的：获取和设置Label信息
		*/
		BOOL GetInformation(CString &image, CString &text, CString &font, osg::Vec4 &color, float &size, float &offsetX, float &offsetY);
		BOOL SetInformation(CString image, CString text, CString font, osg::Vec4 color, float size, float offsetX, float offsetY);

	protected:
		CString		m_image;				//背景图片
		CString		m_text;					//文本内容
		CString		m_font;					//字体文件
		osg::Vec4	m_color;				//颜色
		float		m_size;					//字体大小
		float		m_offsetX;				//偏移X
		float		m_offsetY;				//偏移Y
	};

}
