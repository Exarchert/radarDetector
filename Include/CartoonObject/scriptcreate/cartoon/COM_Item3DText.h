/********************************************************************************
Copyright (C),  
文件：CCOM_Item3DText.h  CCOM_Item3DText.cpp

刘广慧				2011-1-18			生成xml格式脚本
********************************************************************************/

#pragma once

#include <CartoonObject/Export.h>
#include <osg/Vec4>
#include <CartoonObject/ScriptCreate/Cartoon/COM_CartoonItem.h>


namespace CartoonObject		{

class CARTOONOBJECT_EXPORT CCOM_Item3DText : public CCOM_CartoonItem
{
public:
	CCOM_Item3DText() : m_color(1.f, 0.f, 0.f, 1.f), m_font(_T("SIMSUN.TTC")), 
				m_size(30.f), m_width(128), m_height(128), m_bScreen(FALSE), m_screenPosX(0), m_screenPosY(0)
				,m_bBackgrd(FALSE)
	{
		m_Name = _T("未命名文字");
		
	}

	virtual ~CCOM_Item3DText();

	CCOM_Item3DText(const CCOM_Item3DText &text);

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
	目的：获取和设置3D文字信息
	*/
	BOOL GetInformation(CString &textContent, CString &fontName, osg::Vec4 &color, float &size, 
						 BOOL &bScreen, UINT &screenPosX, UINT &screenPosY, BOOL &bBackgrd,
						 osg::Vec4 &bkColor,unsigned int &width, unsigned int &height);

	BOOL SetInformation(const CString &textContent, const CString &fontName, osg::Vec4 color, float size, 
						BOOL bScreen, UINT screenPosX, UINT screenPosY,	BOOL bBackgrd, 
						osg::Vec4 bkColor,unsigned int width = 128, unsigned int height = 128);
protected:
	CString		m_text;			//文本内容
	CString		m_font;			//字体文件
	osg::Vec4	m_color;		//颜色
	float		m_size;			//字体大小
	unsigned int	m_width, m_height;	//分辨率
	BOOL		m_bScreen;		//true表示屏幕文字
	UINT		m_screenPosX;
	UINT		m_screenPosY;
	BOOL		m_bBackgrd;		//是否显示背景
	osg::Vec4	m_bkColor;
};

}