/***************************************************************************
File:			COM_CartoonItem.h
Author:			张平    
Date:			2010-01-21
Description:    动画动作类
***************************************************************************/

#pragma once
#include <COM\Export.h>
#include <Osg/Referenced>
#include <Osg/Vec3>
#include <Osg/Vec2>
#include <COM\XML\XMLNode.h>

namespace COM{

//动画基本单元类
class COM_EXPORT CCartoonAction : public osg::Referenced
{
public:
	CCartoonAction(void);
	virtual ~CCartoonAction(void);

	enum{
		ENM_ActionType_Continuity = 0,			//延续性动作
		ENM_ActionType_UnContinuity,			//非延续性动作
	};

	////获取动作类型
	//DWORD GetType();

	////设置和获取动作开始时刻
	//void SetStartTime(double StartTime);
	//double GetStartTime();

	////设置和获取时长
	//void SetTimeLength(double TimeLength);
	//double GetTimeLength();

	////设置和获取名称
	//void SetName(CString Name);
	//CString GetName();

	///*
	//目的：更新运行时间
	//参数：DeTime		(in)	时间的变化量
	//*/
	//virtual void Update(double DeTime);


protected:
	double m_PlayTime;							//已运行时间
	double m_StartTime;							//开始活动时刻, 与m_PlayTime使用同一个时间轴
	double m_TimeLength;						//活动时间长度
	CString m_Name;								//卡通名称
	double m_Type;								//类型
};
}