/**************************************************************************
Copyright (C),  
文件：COM_ItemFire.h COM_ItemFire.cpp
作者：黄志鹏
版本：1.0
日期：2010-03-13
描述：卡通项-火焰类

修改记录：
作者						日期					描述
---------------------------------------------------------------------------

**************************************************************************/

#pragma once

#include <Temp\Cartoon\COM_CartoonItem.h>
#include <osg\Node>
#include <osg\MatrixTransform>


namespace TEMP		{

class TEMP_EXPORT CCOM_ItemFire : public CCOM_CartoonItem
{
public:
	CCOM_ItemFire()
	{
		m_Name = _T("未命名火焰");
		m_fireModel = _T("QuadArray.3ds");
		m_flamePic  = _T("Flame.tga");
		m_particleSystemHeight = 25.f;
		m_particleSpeed = 1.3f;
		m_particleSpread = 10.5f;
		m_particleSystemShape = 0.2f;
		m_particleShape = 1.5f;
		m_particleSize = 3.9f;
	}

	virtual ~CCOM_ItemFire();

	CCOM_ItemFire(const CCOM_ItemFire &fire);

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
	void GetInformation(CString &fireModel, CString &flamePic, float &particleSystemHeight, float &particleSpeed,
		float &particleSpread, float &particleSystemShape, float &particleShape, float &particleSize)
	{
		fireModel = m_fireModel;
		flamePic  = m_flamePic;
		particleSystemHeight = m_particleSystemHeight;
		particleSpeed = m_particleSpeed;
		particleSpread = m_particleSpread;
		particleSystemShape = m_particleSystemShape;
		particleSize = m_particleSize;
	}

	void SetInformation(const CString &fireModel, const CString &flamePic, float particleSystemHeight, float particleSpeed,
		float particleSpread, float particleSystemShape, float particleShape, float particleSize)
	{
		m_fireModel = fireModel;
		m_flamePic  = flamePic;
		m_particleSystemHeight = particleSystemHeight;
		m_particleSpeed = particleSpeed;
		m_particleSpread = particleSpread;
		m_particleSystemShape = particleSystemShape;
		m_particleSize = particleSize;
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
	DECLARE_SERIAL(CCOM_ItemFire)

protected:
	CString		m_fireModel;	//模型文件的名称
	CString		m_flamePic;		//贴图文件
	float		m_particleSystemHeight; //粒子高度
	float		m_particleSpeed;		//粒子速度
	float		m_particleSpread;		//粒子宽度
	float		m_particleSystemShape;	//火焰大小
	float		m_particleShape;		//火焰片大小
	float		m_particleSize;			//火焰片间距

	osg::ref_ptr<osg::MatrixTransform> m_mat;
};

}