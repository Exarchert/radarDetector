/********************************************************************************
Copyright (C),  
文件：CCOM_ItemRunLine.h  CCOM_ItemRunLine.cpp

刘广慧				2011-1-18			生成xml格式脚本
********************************************************************************/

#pragma once

#include <CartoonObject\Export.h>
#include <CartoonObject\ScriptCreate\Cartoon\COM_Cartoon.h>
#include <osg/Vec4>

namespace CartoonObject	{

class CARTOONOBJECT_EXPORT CCOM_ItemScrollText : public CCOM_CartoonItem
{
public:
	CCOM_ItemScrollText() : m_color(1.f, 0.f, 0.f, 1.f), m_fontName(_T("SIMSUN.TTC")), 
				m_textHeight(25), m_fieldHeight(200), m_lineSpace(5), m_scrollSpeed(1.5)
				, m_maxLines(80), m_alignType(0), m_offsetX(0), m_offsetY(0)
				, m_backColor(0.5,0.5,0.5,0.5), m_hasBack(FALSE)
	{
		m_Name = _T("未命名滚动字幕");
	}

	virtual ~CCOM_ItemScrollText();

	CCOM_ItemScrollText(const CCOM_ItemScrollText &text);

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
	目的：获取和设置滚动字幕信息
	*/
	BOOL GetInformation(CString &textContent, CString &fontName, osg::Vec4 &color, float &textHeight, float &fieldHeight,
						float &lineSpace, float &scrollSpeed, UINT &maxLines, DWORD &alignType, float &offsetX, float &offsetY,
						osg::Vec4 &backColor, BOOL &hasBack);

	BOOL SetInformation(CString &textContent, CString &fontName, osg::Vec4 &color, float textHeight, float fieldHeight,
						float lineSpace, float scrollSpeed, UINT maxLines, DWORD alignType, float offsetX, float offsetY,
						osg::Vec4 backColor, BOOL hasBack);

protected:
	osg::Vec4	m_color;		//颜色
	CString		m_fontName;		//字体名称
	float		m_textHeight;	//文本高度
	float		m_fieldHeight;	//滚动区域高度
	float		m_lineSpace;	//行距
	float		m_scrollSpeed;	//滚动速度
	UINT		m_maxLines;		//最大滚动行数
	DWORD		m_alignType;	//对齐方式	
	float		m_offsetX;		//对齐方式下的偏移(x,y)
	float		m_offsetY;
	osg::Vec4	m_backColor;	//背景颜色
	BOOL		m_hasBack;		//拥有背景标识
	CString		m_textContent;	//文本内容
};

}