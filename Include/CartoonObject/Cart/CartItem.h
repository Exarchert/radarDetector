/***************************************************************************
File:			CartItem.h
Author:			张平    
Date:			2010-03-03
Description:    动画基本单元类

***************************************************************************/

#pragma once
#include <Osg/Referenced>
#include <Osg/Vec3>
#include <Osg/Vec2>
#include <Osg/ref_ptr>
#include <CartoonObject/Export.h>
#include <CartoonObject/Cart/CartAction.h>
#include <vector>

namespace CartoonObject{

//动画基本单元类
class CARTOONOBJECT_EXPORT CCartItem : public osg::Referenced
{
public:
	CCartItem(void);
	virtual ~CCartItem(void);

	/*
	目的：初始化动画对象
	参数：StartTime		(in)	动画对象的开始活动时刻
		  TimeLenght	(in)	动画对象的活动时间长度
		  name			(in)	动画对象名称
	返回：void
	*/
	void Init(CString &Name, double StartTime = 0, double TimeLength = 0);


	/*
	目的：获取动画单元的活动时间长度
	*/
	double GetTimeLength(){ return m_TimeLength; }

	void SetTimeLength(double timeLength) 
	{ 
		m_TimeLength = timeLength; 
	}


	/*
	目的：获取名称
	*/
	CString GetName()
	{
		return m_Name;
	}

	void SetName(CString &name)
	{
		m_Name = name;
	}

	/*
	目的：设置开始活动时刻
	*/
	void SetStartTime(double startTime)
	{
		m_StartTime = startTime;
	}

	double GetStartTime()
	{
		return m_StartTime;
	}

	/*
	目的：动画单元执行行为
	参数：Time		(in)	相对于开始活动时刻的时刻
	返回：void
	*/
	virtual void DoAction(double Time);

	/*
	目的：播放完成执行的行为
	参数：Time		(in)	相对于开始活动时刻的时刻
	返回：void
	*/
	virtual void PlayFinish(double Time);

	/*
	目的：更新运行时间
	参数：DeTime		(in)	时间的变化量
	*/
	virtual void Update(double DeTime);

	/*
	目的：播放准备
	*/
	virtual void ReadyPlay();

	/*
	获得和设置对象ID
	*/
	void SetObjID(LONG64 ObjID){m_ObjID = ObjID;}
	LONG64 GetObjID(){return m_ObjID;}

protected:
	double m_PlayTime;							//已运行时间
	double m_StartTime;							//开始活动时刻, 与m_PlayTime使用同一个时间轴
	double m_PlayScale;							//播放速度(倍率)
	DWORD m_PlayState;							//动画单元的状态
	double m_TimeLength;						//活动时间长度
	CString m_Name;								//卡通名称

	LONG64 m_ObjID;								//对象ID

	//动作列表
	std::vector<osg::ref_ptr<CCartAction>> m_CartActions;

};
}