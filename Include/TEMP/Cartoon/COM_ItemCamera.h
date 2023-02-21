/********************************************************************************
Copyright (C),  
文件：COM_ItemCamera.h  COM_ItemCamera.cpp
作者：黄志鹏
版本：1.0
日期：2009-08-11
描述：卡通动画--相机

修改记录
作者						日期							描述
---------------------------------------------------------------------------------

********************************************************************************/

#pragma once

#include <temp/Export.h>
#include <temp/Cartoon/COM_CartoonItem.h>


namespace TEMP		{

class TEMP_EXPORT CCOM_ItemCamera : public CCOM_CartoonItem
{
public:
	CCOM_ItemCamera()
	{
		m_Name = _T("未命名相机");
		m_bive = FALSE;
		m_bivePlayed = FALSE;
	}

	virtual ~CCOM_ItemCamera();

	CCOM_ItemCamera(const CCOM_ItemCamera &vehicle);

	CCOM_CartoonItem * Clone();

	/*
	目的：播放
	*/
	virtual void Play();

	/*
	目的：暂停
	*/
	virtual void Pause();

	/*
	目的：停止播放
	*/
	virtual void Stop();

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
	void SetInformation(BOOL bive) 
	{
		m_bive = bive;
	}

	BOOL GetInformation()
	{
		return m_bive;
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
	DECLARE_SERIAL(CCOM_ItemCamera)

protected:
	BOOL m_bive;
	BOOL m_bivePlayed;
	BOOL m_bStart;
};

}