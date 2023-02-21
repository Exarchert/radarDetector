/********************************************************************************
Copyright (C),  
�ļ���COM_ItemFlow.h  COM_ItemFlow.cpp
���ߣ���ƽ
�汾��1.0
���ڣ�2010-6-25
��������ͨ����--����

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
		m_Name = _T("����");
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
	Ŀ�ģ�������Ԫִ����Ϊ
	������Time		(in)	����ڿ�ʼ�ʱ�̵�ʱ��
	      pos		(in)	������λ��ʱ��λ��			
	���أ�void
	*/
	virtual void DoAction(double Time, osg::Vec3d pos);

	/*
	Ŀ�ģ����úͻ�ȡ��Ϣ
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
	Ŀ�ģ����л�����
	*/
	virtual void Serialize(CArchive& ar);

	/*
	Purpose:	Parser XML	Data Format for Animation
	Author:		Liugh
	*/
	virtual bool ParserXml(MSXML2::IXMLDOMNodePtr pNode);

	//���������������
	void ConstructPipeObject();

protected:
	DECLARE_SERIAL(CCOM_ItemFlow)

protected:
	double	m_FlowTimeLength;
	BOOL	m_bPlayed;
	LONG64	m_PipeID;			//����ID
	BOOL	m_bClear;			//�������������ʶ
	float	m_Speed;			//�����ٶ�
	float	m_r;				//�������߰뾶
	osg::Vec4 m_Color;			//�������ߵ���ɫ
	BOOL	m_bReverse;			//��������
	float	m_PipeLen;			//���߳���
	float	m_Piper;			//���߰뾶

	osg::ref_ptr<osg::MatrixTransform> m_MT;


	//��̬ƫ�ƾ���
	osg::ref_ptr<osg::MatrixTransform> m_MoveMT;
	osg::ref_ptr<EMObj::CSOLongPipe> m_pSOLongPipe;

	osg::ref_ptr<osgAPEx::FlowPipe> m_FlowPipe;


	double getTime(void *pData);
};
}