/************************************************************************
Copyright (C),  
文件：COM_ItemSpray.h  COM_ItemSpray.cpp
作者：黄志鹏
版本：1.0
日期：2009-08-31
描述：水幕效果

修改记录：
作者						日期						描述
-------------------------------------------------------------------------

************************************************************************/

#pragma once

#include <Temp/Export.h>
#include <osg/MatrixTransform>
#include <Temp/Cartoon/COM_CartoonItem.h>


namespace TEMP		{

class TEMP_EXPORT CCOM_ItemSpray : public CCOM_CartoonItem
{
public:
	CCOM_ItemSpray() : m_imageFile(_T("")), m_repeatRation(2), m_speed(0.0008f)
						, m_radius(0.f), m_cylinderHeight(0.f), m_sphereHeight(0.f)
	{
		m_Name = _T("未命名水幕");
	}

	virtual ~CCOM_ItemSpray();

	CCOM_ItemSpray(const CCOM_ItemSpray &spray);

	CCOM_CartoonItem * Clone();

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
	目的：设置和获取信息
	*/
	BOOL GetInformation(CString &imageFile, float &repeatRation, float &speed, float &radius, 
						float &cylinderHeight, float &sphereHeight)
	{
		imageFile = m_imageFile;
		repeatRation = m_repeatRation;
		speed = m_speed;
		radius = m_radius;
		cylinderHeight = m_cylinderHeight;
		sphereHeight = m_sphereHeight;
		return TRUE;
	}

	BOOL SetInformation(CString imageFile, float repeatRation, float speed, float radius, 
						float cylinderHeight, float sphereHeight)
	{
		m_imageFile = imageFile;
		m_repeatRation = repeatRation;
		m_speed = speed;
		m_radius = radius;
		m_cylinderHeight = cylinderHeight;
		m_sphereHeight = sphereHeight;
		return TRUE;
	}

	/*
	目的：序列化操作
	*/
	virtual void Serialize(CArchive& ar);

	/*
	Purpose:	Parser XML	Data Format for Animation
	Author:		Liugh
	*/
	virtual bool ParserXml(MSXML2::IXMLDOMNodePtr pNode);

protected:
	DECLARE_SERIAL(CCOM_ItemSpray)

protected:
	CString		m_imageFile;		//贴图文件名
	float		m_repeatRation;		//贴图重复率
	float		m_speed;			//流速(像素/帧)
	float		m_radius;			//半径
	float		m_cylinderHeight;	//圆柱高度
	float		m_sphereHeight;		//球高度

	osg::ref_ptr<osg::MatrixTransform> m_mat;
};

}