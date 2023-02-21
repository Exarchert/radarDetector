/*
Copyright (C),  
文件：COM_ItemLabel.h  COM_ItemLabel.cpp
作者：黄志鹏
版本：1.0
日期：2009-09-01
描述：空间标盘类

修改记录：
作者						日期						描述
-----------------------------------------------------------------------

*/

#pragma once 

#include <temp/Export.h>
#include <temp/Cartoon/COM_CartoonItem.h>
#include <osg\ref_ptr>
#include <osg\Vec4>
#include <osg\MatrixTransform>
#include <osgAPEx\TableLabel>


namespace TEMP		{

class TEMP_EXPORT CCOM_ItemLabel : public CCOM_CartoonItem
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
	目的：获取和设置Label信息
	*/
	BOOL GetInformation(CString &image, CString &text, CString &font, osg::Vec4 &color, float &size, float &offsetX, float &offsetY);
	BOOL SetInformation(CString image, CString text, CString font, osg::Vec4 color, float size, float offsetX, float offsetY);

protected:
	DECLARE_SERIAL(CCOM_ItemLabel)

protected:
	osg::ref_ptr<osgAPEx::TableLabel>	m_labelNode;
	CString		m_image;				//背景图片
	CString		m_text;					//文本内容
	CString		m_font;					//字体文件
	osg::Vec4	m_color;				//颜色
	float		m_size;					//字体大小
	float		m_offsetX;				//偏移X
	float		m_offsetY;				//偏移Y
};

}
