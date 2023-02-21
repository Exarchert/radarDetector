/*********************************************************************
Copyright (C),  
文件：COM_ItemScrollText.h COM_ItemScrollText.cpp
作者：黄志鹏
版本：1.0
日期：2009-07-30
描述：卡通项-滚动字幕

修改记录：
作者						日期					描述
----------------------------------------------------------------------
黄志鹏						2010-02-25				移植到此工程

*********************************************************************/

#pragma once

#include <osg/Vec4>
#include <osg/Group>
#include <osgText/Font>
#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>
#include <osgViewer/api/Win32/GraphicsWindowWin32>
#include <osgAPEx/ScrollText>
#include <temp/Cartoon/COM_CartoonItem.h>


namespace TEMP	{

class TEMP_EXPORT CCOM_ItemScrollText : public CCOM_CartoonItem
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
	目的：动画单元执行行为
	参数：Time		(in)	相对于开始活动时刻的时刻
		  pos		(in)	动画单位此时的位置			
	返回：void
	*/
	virtual void DoAction(double Time, osg::Vec3d pos);

	//执行非活动时间区的动作
	virtual void DoNoneActive();

	/*
	目的：序列化操作
	*/
	virtual void Serialize(CArchive& ar);

	/*
	Purpose:	Parser XML	Data Format for Animation
	Author:		Liugh
	*/
	virtual bool ParserXml(MSXML2::IXMLDOMNodePtr pNode);

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
	DECLARE_SERIAL(CCOM_ItemScrollText)

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
	
	//场景节点
	osg::ref_ptr<osg::Group>	m_scene;
};

}