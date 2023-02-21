/*********************************************************************
Copyright (C),  
文件：COM_Item3DText.h COM_Item3DText.cpp
作者：黄志鹏
版本：1.0
日期：2009-07-30
描述：卡通项-空间文字(面向屏幕)

修改记录：
作者						日期					描述
----------------------------------------------------------------------

*********************************************************************/

#pragma once

#include <temp/Export.h>
#include <osg/Vec4>
#include <osg/MatrixTransform>
#include <temp/Cartoon/COM_CartoonItem.h>


namespace TEMP		{

class TEMP_EXPORT CCOM_Item3DText : public CCOM_CartoonItem
{
public:
	CCOM_Item3DText() : m_color(1.f, 0.f, 0.f, 1.f), m_font(_T("SIMSUN.TTC")), 
				m_size(30.f), m_width(128), m_height(128), m_bScreen(FALSE), m_screenPosX(0), m_screenPosY(0)
				, m_2Dtext(NULL),m_bBackgrd(FALSE)
	{
		m_Name = _T("未命名文字");
		
	}

	virtual ~CCOM_Item3DText();

	CCOM_Item3DText(const CCOM_Item3DText &text);

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
	目的：获取和设置3D文字信息
	*/
	BOOL GetInformation(CString &textContent, CString &fontName, osg::Vec4 &color, float &size, 
						unsigned int &width, unsigned int &height, BOOL &bScreen, UINT &screenPosX, UINT &screenPosY,
						BOOL &bBackgrd, osg::Vec4 &bkColor);

	BOOL SetInformation(CString &textContent, CString &fontName, osg::Vec4 color, float size, 
						unsigned int width, unsigned int height, BOOL bScreen, UINT screenPosX, UINT screenPosY,
						BOOL bBackgrd, osg::Vec4 bkColor);

protected:
	/*
	目的：创建屏幕文字
	*/
	osg::Node * CreateHUD(CString &textContent, CString &fontName, osg::Vec4 color, float size, 
		unsigned int reswidth, unsigned int resheight, osg::Vec3 position, osgText::Text *&outText);

	/*
	目的：创建面向屏幕的空间文字
	*/
	osg::Node * Create3DText(CString &textContent, CString &fontName, osg::Vec4 color, float size, 
		unsigned int reswidth, unsigned int resheight, osg::Vec3 position);

protected:
	DECLARE_SERIAL(CCOM_Item3DText)

protected:
	osg::ref_ptr<osg::MatrixTransform> m_textNode;
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
	osgText::Text *m_2Dtext;	//osgText::Text对象指针
};

}