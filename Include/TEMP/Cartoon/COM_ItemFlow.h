/********************************************************************************
Copyright (C),  
文件：COM_ItemFlow.h  COM_ItemFlow.cpp
作者：张平
版本：1.0
日期：2010-6-25
描述：卡通动画--流程

********************************************************************************/

#pragma once

#include <temp/Export.h>
#include <osg/Vec4>
#include <temp/Cartoon/COM_CartoonItem.h>

#include <osgAPEx\FlowPipe>
#include <Object\ShowObj\LongPipe\SOLongPipe.h>
#include <Object\ShowObj\Pipe\SOPipe.h>
namespace TEMP		{


class TEMP_EXPORT CCOM_ItemFlow : public CCOM_CartoonItem
{
public:
	CCOM_ItemFlow()
	{
		m_FlowTimeLength = 0;
		m_Name = _T("流程");
		m_bPlayed = FALSE;
		m_PipeID = 0L;
		m_bClear = TRUE;
		m_Speed = 0;
		m_r = 0;
		m_bReverse = FALSE;
		m_PipeLen = 0;
		m_Piper = 0;
	}

	virtual ~CCOM_ItemFlow();

	CCOM_ItemFlow(const CCOM_ItemFlow &vehicle);

	CCOM_CartoonItem * Clone();

	/*
	目的：动画单元执行行为
	参数：Time		(in)	相对于开始活动时刻的时刻
	      pos		(in)	动画单位此时的位置			
	返回：void
	*/
	virtual void DoAction(double Time, osg::Vec3d pos);

	/*
	目的：设置和获取信息
	*/
	BOOL SetInformation(double FlowTimeLength, LONG64 PipeID, BOOL bClear, float Speed, float r, osg::Vec4 Color, BOOL bDirection, float PipeLen, float Piper) 
	{
		m_FlowTimeLength = FlowTimeLength;
		m_PipeID = PipeID;
		m_bClear = bClear;
		m_Speed = Speed;
		m_r = r;
		m_Color = Color;
		m_bReverse = bDirection;
		m_PipeLen = PipeLen;
		m_Piper = Piper;
		return TRUE;
	}

	BOOL GetInformation(double &FlowTimeLength, LONG64 &PipeID, BOOL &bClear, float &Speed, float &r, osg::Vec4 &Color, BOOL &bDirection, float &PipeLen, float &Piper)
	{
		m_FlowTimeLength = FlowTimeLength;
		PipeID = m_PipeID;
		bClear = m_bClear;
		Speed = m_Speed;
		r = m_r;
		Color = m_Color;
		bDirection = m_bReverse;
		PipeLen = m_PipeLen;
		Piper = m_Piper;
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

	//构造管线流动对象
	void ConstructPipeObject();

protected:
	DECLARE_SERIAL(CCOM_ItemFlow)

protected:
	double	m_FlowTimeLength;
	BOOL	m_bPlayed;
	LONG64	m_PipeID;			//管线ID
	BOOL	m_bClear;			//流动结束清除标识
	float	m_Speed;			//流动速度
	float	m_r;				//流动管线半径
	osg::Vec4 m_Color;			//流动管线的颜色
	BOOL	m_bReverse;			//反向流动
	float	m_PipeLen;			//管线长度
	float	m_Piper;			//管线半径

	osg::ref_ptr<osg::MatrixTransform> m_MT;


	//动态偏移矩阵
	osg::ref_ptr<osg::MatrixTransform> m_MoveMT;
	osg::ref_ptr<EMObj::CSOLongPipe> m_pSOLongPipe;

	osg::ref_ptr<osgAPEx::FlowPipe> m_FlowPipe;


	double getTime(void *pData);
};
}