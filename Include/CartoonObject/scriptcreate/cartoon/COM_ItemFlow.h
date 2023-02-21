/********************************************************************************
Copyright (C),  
文件：CCOM_ItemFlow.h  CCOM_ItemFlow.cpp

刘广慧				2011-1-18			生成xml格式脚本
********************************************************************************/

#pragma once

#include <CartoonObject/Export.h>
#include <osg/Vec4>
#include <CartoonObject/ScriptCreate/Cartoon/COM_CartoonItem.h>

namespace CartoonObject		{


class CARTOONOBJECT_EXPORT CCOM_ItemFlow : public CCOM_CartoonItem
{
public:
	CCOM_ItemFlow()
	{
		m_Name = _T("流程");
		m_bPlayed = FALSE;
		m_PipeID = 0L;
		m_bClear = TRUE;
		m_Speed = 0;
		m_r = 0;
		m_bReverse = FALSE;
	}

	virtual ~CCOM_ItemFlow();

	CCOM_ItemFlow(const CCOM_ItemFlow &vehicle);

	CCOM_CartoonItem * Clone();

	/*
	目的：设置和获取信息
	*/
	BOOL SetInformation(LONG64 PipeID, BOOL bClear, float Speed, float r, osg::Vec4 Color, BOOL bDirection) 
	{
		m_PipeID = PipeID;
		m_bClear = bClear;
		m_r = r;
		m_Color = Color;
		m_bReverse = bDirection;
		m_Speed = Speed;
		return TRUE;
	}

	BOOL GetInformation(LONG64 &PipeID, BOOL &bClear, float &Speed, float &r, osg::Vec4 &Color, BOOL &bDirection)
	{
		PipeID = m_PipeID;
		bClear = m_bClear;
		Speed = m_Speed;
		r = m_r;
		Color = m_Color;
		bDirection = m_bReverse;
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
	BOOL	m_bPlayed;
	LONG64	m_PipeID;			//管线ID
	BOOL	m_bClear;			//流动结束清除标识
	BOOL	m_bTimeFirst;		//以时长流动管线
	float	m_Speed;			//流动速度
	float	m_r;				//流动管线半径
	osg::Vec4 m_Color;			//流动管线的颜色
	BOOL	m_bReverse;			//反向流动
};
}