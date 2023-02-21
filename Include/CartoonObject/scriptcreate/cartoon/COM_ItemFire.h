/********************************************************************************
Copyright (C),  
文件：CCOM_ItemFire.h  CCOM_ItemFire.cpp

刘广慧				2011-1-18			生成xml格式脚本
********************************************************************************/

#pragma once

#include <CartoonObject\ScriptCreate\Cartoon\COM_CartoonItem.h>

namespace CartoonObject		{

class CARTOONOBJECT_EXPORT CCOM_ItemFire : public CCOM_CartoonItem
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
	CString		m_fireModel;			//模型文件的名称
	CString		m_flamePic;				//贴图文件
	float		m_particleSystemHeight; //粒子高度
	float		m_particleSpeed;		//粒子速度
	float		m_particleSpread;		//粒子宽度
	float		m_particleSystemShape;	//火焰大小
	float		m_particleShape;		//火焰片大小
	float		m_particleSize;			//火焰片间距
};

}