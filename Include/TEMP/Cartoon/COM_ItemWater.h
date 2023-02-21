/**************************************************************************
Copyright (C),  
文件：COM_ItemWater.h COM_ItemWater.cpp
作者：黄志鹏
版本：1.0
日期：2009-07-29
描述：卡通项-喷水效果

修改记录：
作者						日期					描述
---------------------------------------------------------------------------

**************************************************************************/

#pragma once

#include <Temp\Export.h>
#include <Temp\Cartoon\COM_CartoonItem.h>
#include <osg\ref_ptr>
#include <osg\MatrixTransform>


namespace TEMP		{

class TEMP_EXPORT CCOM_ItemWater : public CCOM_CartoonItem
{
public:
	CCOM_ItemWater() : m_imageFile(_T("shui.tga")), m_lifeTime(3.5f),
		m_minRate(80.f), m_maxRate(81.f), m_minSpeed(80.f), m_maxSpeed(81.f),
		m_phi(2), m_theta(0.01f),m_Type(0), m_Dist(0.0f)
	{
		m_Name = _T("未命名喷水");
	}

	virtual ~CCOM_ItemWater();

	CCOM_ItemWater(const CCOM_ItemWater &water);

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
	目的：序列化操作
	*/
	virtual void Serialize(CArchive& ar);

	/*
	Purpose:	Parser XML	Data Format for Animation
	Author:		Liugh
	*/
	virtual bool ParserXml(MSXML2::IXMLDOMNodePtr pNode);

protected:
	DECLARE_SERIAL(CCOM_ItemWater)

protected:
	CString	m_imageFile;	//贴图
	double	m_lifeTime;		//粒子生命周期
	float	m_minRate;		//最小粒子数
	float	m_maxRate;		//最大粒子数
	float	m_minSpeed;		//最小初速度
	float	m_maxSpeed;		//最大初速度
	float	m_phi;			//指定水平范围
	float	m_theta;		//指定垂直范围
	int		m_Type;
	float	m_Dist;			//水平距离

	osg::ref_ptr<osg::MatrixTransform> m_modelNode;
};

}