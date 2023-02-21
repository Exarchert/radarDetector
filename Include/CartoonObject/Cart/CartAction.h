/***************************************************************************
File:			CartAction.h
Author:			张平    
Date:			2010-01-21
Description:    动画动作类
***************************************************************************/

#pragma once
#include <CartoonObject/Export.h>
#include <Osg/Referenced>
#include <Osg/Vec3>
#include <Osg/Vec2>
#include <object/Obj.h>

namespace CartoonObject{

//动画基本单元类
class CARTOONOBJECT_EXPORT CCartAction : public osg::Referenced
{
public:
	CCartAction(void);
	virtual ~CCartAction(void);

	enum{
		ENM_ActionType_Continuity = 0,			//延续性动作
		ENM_ActionType_UnContinuity,			//非延续性动作
	};

	//获取动作类型
	DWORD GetType();

	//设置和获取动作开始时刻
	void SetStartTime(double StartTime);
	double GetStartTime();

	//设置和获取时长
	void SetTimeLength(double TimeLength);
	double GetTimeLength();

	//设置和获取名称
	void SetName(CString Name);
	CString GetName();

	/*
	目的：设置作用对象
	*/
	void SetObj(EMObj::CObj *pObj);

	/*
	目的：更新运行时间
	参数：Time		(in)	CartItem的播放时间
	*/
	virtual void Update(double Time);

	/*
	目的：执行命令
	参数：Time		(in)	CartItem的播放时间
	*/
	virtual void Update();

protected:
	double m_PlayTime;							//已运行时间
	double m_StartTime;							//开始活动时刻, 与m_PlayTime使用同一个时间轴
	double m_TimeLength;						//活动时间长度
	CString m_Name;								//名称
	DWORD m_TimeType;							//类型
	osg::ref_ptr<EMObj::CObj> m_Obj;			//动作对象


};
}