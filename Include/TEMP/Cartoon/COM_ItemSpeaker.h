/*******************************************************************************
Copyright (C),  
文件：COM_ItemSpeaker.h  COM_ItemSpeaker.cpp
作者：黄志鹏
版本：1.0
日期：2009-08-12
描述：运动项--扬声器，用于发出声音。

修改记录：
作者						日期						描述
---------------------------------------------------------------------------------

********************************************************************************/

#pragma once 

#include <temp/Export.h>
#include <temp/Cartoon/COM_CartoonItem.h>
#include <Com/soundboot/SoundBoot.h>


namespace TEMP		{

class TEMP_EXPORT CCOM_ItemSpeaker : public CCOM_CartoonItem
{
public:
	CCOM_ItemSpeaker() : m_bBindCamera(FALSE), m_volum(0.5f), m_bPlay(false)
	{
		m_Name = _T("未命名扬声器");
	}

	virtual ~CCOM_ItemSpeaker();

	CCOM_ItemSpeaker(const CCOM_ItemSpeaker &speaker);

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
	目的：设置和获取扬声器信息
	*/
	BOOL GetInformation(CString &soundFile, BOOL &bBindCamera, float &volum)
	{
		soundFile = m_soundFile;
		bBindCamera = m_bBindCamera;
		volum = m_volum;
		return TRUE;
	}

	BOOL SetInformation(const CString &soundFile, BOOL bBindCamera, float volum)
	{
		m_soundFile = soundFile;
		m_bBindCamera = bBindCamera;
		m_volum = volum;
		return TRUE;
	}

	/*
	目的：播放
	*/
	virtual void Play();

	/*
	目的：暂停
	*/
	virtual void Pause();	

	/*
	目的: 快进播放
	*/
	virtual void Forward(int step);

	/*
	目的: 慢放
	*/
	virtual void Rewind(int step);

	/*
	目的: 设置播放时刻,主要实现跳转功能
	*/
	virtual void SetPlayTime(double Time);


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
	DECLARE_SERIAL(CCOM_ItemSpeaker)

protected:
	CString			m_soundFile;	//音频文件的名称
	BOOL			m_bBindCamera;	//伴随相机标识
	float			m_volum;		//音量(0~1.f)
	BOOL			m_bPlay;		//是否已开始播放标识
	osg::ref_ptr<COM::CSoundBoot> m_soundBoot;	//音频播放对象
};

}