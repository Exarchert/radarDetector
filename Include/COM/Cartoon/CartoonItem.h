/***************************************************************************
File:			COM_CartoonItem.h
Author:			张平    
Date:			2009-03-31
Description:    动画基本单元类

ChangeLog:
作者					日期					描述
----------------------------------------------------------------------------
黄志鹏				2009-07-21			修改CCartoonItem再从MFC CObject继承,
										使之具有Serialize特性.

***************************************************************************/

#pragma once
#include <COM\Export.h>
#include <Osg/Referenced>
#include <Osg/Vec3>
#include <Osg/Vec2>
#include <COM\Movement\MoveTracks.h>
#include <COM\XML\XMLNode.h>

namespace COM{

//动画基本单元类
class COM_EXPORT CCartoonItem : public osg::Referenced
{
public:
	CCartoonItem(void);
	virtual ~CCartoonItem(void);

	virtual CCartoonItem * Clone();

	/*
	目的：拷贝构造函数
	*/
	CCartoonItem(const CCartoonItem &item);


	enum{
		ENM_PlayState_UnDefault = 0,
		ENM_PlayState_Play,
		ENM_PlayState_Pause,
		ENM_PlayState_Stop,
		ENM_PlayState_Finish,
		ENM_PlayState_QuickSpeedPlay,
		ENM_PlayState_SlowSpeedPlay,
	};

	/*
	目的：初始化动画对象
	参数：Tracks			(in)	运动片序列
		  StartTime		(in)	动画对象的开始活动时刻
		  TimeLenght	(in)	动画对象的活动时间长度
		  name			(in)	动画对象名称
	返回：void
	*/
	void Init(CMoveTracks *Tracks, CString &name, double StartTime = 0, double TimeLength = 0);

	/*
	目的：播放
	*/
	virtual void Play();

	/*
	目的：暂停
	*/
	virtual void Pause();

	/*
	目的：重新播放
	*/
	virtual void Replay();

	/*
	目的：设置已运行时间, 该时间是相对于一个时间轴
	参数：Time		(in)	播放器已运行时间
	*/
	virtual void SetPlayTime(double Time);

	/*
	目的：设置播放速度
	参数：Speed		(in)	加速倍数
	*/
	virtual void SetSpeed(double Speed);

	/*
	目的：停止播放
	*/
	virtual void Stop();

	/*
	目的：返回目前动画单元的播放状态
	返回：播放状态值
	*/
	DWORD GetPlayState(){ return m_PlayState; }

	/*
	目的：获取已运行时间
	*/
	double GetPlayTime(){ return m_PlayTime; }

	/*
	目的：获取动画单元的活动时间长度
	*/
	double GetTimeLength(){ return m_TimeLength; }

	void SetTimeLength(double timeLength) 
	{ 
		m_TimeLength = timeLength; 
	}

	/*
	目的：获取动画单元的运动片序列
	返回：NULL表示失败，否则成功
	*/
	CMoveTracks * GetTracks(){ return m_Tracks.get(); }

	/*
	目的：动画单元执行行为
	参数：Time		(in)	相对于开始活动时刻的时刻
		  pos		(in)	动画单位此时的位置			
	返回：void
	*/
	virtual void DoAction(double Time, osg::Vec3d pos);
	
	virtual void PlayFinish(double Time, osg::Vec3d pos);
	/*
	目的：更新运行时间
	参数：DeTime		(in)	时间的变化量
	*/
	virtual void Update(double DeTime);

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

	//XML读取和保存模块
	bool InitTracks(MSXML2::IXMLDOMNodePtr pNode);
	MSXML2::IXMLDOMNodePtr GetXMLNodeTracks();
protected:
	osg::ref_ptr<CMoveTracks> m_Tracks;		//运动片序列
	double m_PlayTime;							//已运行时间
	double m_StartTime;							//开始活动时刻, 与m_PlayTime使用同一个时间轴
	double m_PlayScale;							//播放速度(倍率)
	DWORD m_PlayState;							//动画单元的状态
	double m_TimeLength;						//活动时间长度
	CString m_Name;								//卡通名称
};
}