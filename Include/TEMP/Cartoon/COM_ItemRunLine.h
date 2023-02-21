/**************************************************************************
Copyright (C),  
文件：COM_ItemRunLine.h COM_ItemRunLine.cpp
作者：黄志鹏
版本：1.0
日期：2009-07-29
描述：卡通项-动态画的线

修改记录：
作者						日期						描述
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
		m_Name = _T("未命名运动线");

	}

	virtual ~CCOM_ItemRunLine();

	CCOM_ItemRunLine(const CCOM_ItemRunLine &line);

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
	目的：获取和设置运动线的信息
	*/
	BOOL GetInformation(osg::Vec4 &color, int &width);
	BOOL SetInformation(osg::Vec4 color, int width);

protected:
	DECLARE_SERIAL(CCOM_ItemRunLine)

protected:
	osg::ref_ptr<osg::MatrixTransform> m_arrowNode;
	osg::ref_ptr<osg::MatrixTransform> m_matrix;
	osg::ref_ptr<osg::Geode>		m_geode;
	osg::Vec3d	m_origin;			//当前坐标系的原点在世界坐标系中的位置
	osg::Vec3d	m_lastPosition;		//上一时刻点的位置(在当前坐标系中)
	osg::Vec4	m_color;
	int			m_width;
};

}