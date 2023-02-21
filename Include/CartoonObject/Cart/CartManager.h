/***********************************************************************
File:			CartManager.h
Author:			张平    
Date:			2009-03-3
Description:    动画管理类
************************************************************************/

#pragma once
#include <CartoonObject\Export.h>
#include <osgGA\GUIEventHandler>
#include <vector>
#include <CartoonObject\Cart\CartUnit.h>
namespace CartoonObject{

class CARTOONOBJECT_EXPORT CCartManager : public osgGA::GUIEventHandler 
{
public:
	CCartManager(void);
	virtual ~CCartManager(void);

	//添加和删除卡通单元
	void AddCartUnit(CCartUnit *pCartUnit);
	void DelCartUnit(CCartUnit *pCartUnit);

	//清楚动画单元
	void Clear();

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
	目的：播放
	*/
	void Play();

	/*
	目的：暂停
	*/
	void Pause();

	/*
	目的：重新播放
	*/
	void Replay();

	/*
	目的：设置播放时间
	参数：Time		(in)	播放时间
	*/
	void CCartManager::SetPlayTime(double Time);

	/*
	目的：设置播放速度
	参数：Speed		(in)	加速倍数
	*/
	void SetSpeed(double Speed);

	/*
	目的：停止播放
	*/
	void Stop();

	/*
	目的：返回目前动画单元的播放状态
	返回：播放状态值
	*/
	DWORD GetPlayState(){ return m_PlayState; }



	//事件处理
	bool handle(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa);


	/*
	从文件打开
	*/
	bool Load(CString strFile);


protected:
	std::vector<osg::ref_ptr<CCartUnit>> m_CartUnits;	//动画单元数组
	double m_PlayTime;							//已运行时间
	double m_PlayScale;							//播放速度(倍率)
	DWORD m_PlayState;							//动画单元的状态

private:
	double m_LastTime;
};


}