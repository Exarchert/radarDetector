/********************************************************************************
Copyright (C),  
文件：CCOM_ItemRunLine.h  CCOM_ItemRunLine.cpp

刘广慧				2011-1-18			生成xml格式脚本
********************************************************************************/

#pragma once

#include <CartoonObject\Export.h>
#include <CartoonObject\ScriptCreate\Cartoon\COM_CartoonItem.h>
#include <osg\ref_ptr>

namespace CartoonObject		{

class CARTOONOBJECT_EXPORT CCOM_ItemWater : public CCOM_CartoonItem
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
	目的：设置和获取信息
	*/
	BOOL GetInformation(CString& imageFile, float& life, float& minNum, float& maxNum, float& minSpeed, float& maxSpeed, 
		float& phi, float& theta, int& type, float& dist)
	{
		imageFile = m_imageFile;
		life = m_lifeTime;
		minNum = m_minRate;
		maxNum = m_maxRate;
		minSpeed = m_minSpeed;
		maxSpeed = m_maxSpeed;
		phi = m_phi;
		theta = m_theta;
		type = m_Type;
		dist = m_Dist;		
		return TRUE;
	}

	BOOL SetInformation(CString imageFile, float life, float minNum, float maxNum, float minSpeed, float maxSpeed, 
		float phi, float theta, int type, float dist)
	{
		m_imageFile = imageFile;
		m_lifeTime = life;
		m_minRate = minNum;
		m_maxRate = maxNum;
		m_minSpeed = minSpeed;
		m_maxSpeed = maxSpeed;
		m_phi = phi;
		m_theta = theta;
		m_Type = type;
		m_Dist = dist;
		return TRUE;
	}

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
protected:
	CString	m_imageFile;	//贴图
	double	m_lifeTime;		//粒子生命周期
	float	m_minRate;		//最小粒子数 / 喷射口半径
	float	m_maxRate;		//最大粒子数 / 贴图率
	float	m_minSpeed;		//最小初速度 / 初始速度
	float	m_maxSpeed;		//最大初速度 / 半径增长率
	float	m_phi;			//指定水平范围 / 贴图流速(正数为正向,负数为逆向)
	float	m_theta;		//指定垂直范围 / 喷射仰角
	int		m_Type;			//0：离子水;1：圆锥抛物线
	float	m_Dist;			//水平距离
};

}