/*************************************************************************
File:			MoveTrack.h
Author:			张平    
Date:			2009-03-16
Description:    运动控制类中的轨迹类，描述一个运动.

ChangeLog:
作者					日期					描述
--------------------------------------------------------------------------


*************************************************************************/

#pragma once

#include <COM\Movement\MovePath.h>
#include <COM\Movement\MoveSpeed.h>
#include <COM\Movement\MovePose.h>
#include <vector>
#include <osg/ref_ptr>
#include <COM\XML\XMLNode.h>
#include <COM\XML\XMLApi.h>

namespace COM{

//运动基类
class COM_EXPORT CMoveTrack : public osg::Referenced, public CXMLNode
{
public:
	/*
	目的：构造函数
	参数：path		(in)	运动路径对象指针
		 speed		(in)	运动速度对象指针
		 pose		(in)	运动姿态对象指针
		 timeLen	(in)	该运动消耗时间长度
	*/
	CMoveTrack(CMovePath *path, CMoveSpeed *speed, CMovePose *pose, double timeLen)
	{
		m_Path = path;
		m_Speed = speed;
		m_Pose = pose;
		m_TimeLength = timeLen;
	}

	CMoveTrack(const CMoveTrack &track);

	/*
	目的：克隆自己
	*/
	CMoveTrack * Clone()
	{
		return new CMoveTrack(*this);
	}

	virtual ~CMoveTrack(void)
	{
	}

	/*
	目的：返回某时刻的位置
	参数：time		(in)	时刻（从0计算）
		  position	(out)	返回	位置
	返回：成功返回true，position有效，否则无效
	*/
	bool GetPosition(double time, osg::Vec3d &position);
	bool GetStartPosition(osg::Vec3d &position);
	bool GetEndPosition(osg::Vec3d &position);

	/*
	目的：返回某时刻的姿态
	参数：time		(in)	时刻（从0计算）
		  pose		(out)	返回姿态
	返回：成功返回true，pose有效，否则无效
	*/
	bool GetPose(double time, osg::Vec3d &pose);
	bool GetStartPose(osg::Vec3d &pose);
	bool GetEndPose(osg::Vec3d &pose);

	/*
	目的：获取该运动消耗的时间
	*/
	double GetTimeLength()
	{
		return m_TimeLength;
	}

	void SetTimeLength(double timeLength)
	{
		m_TimeLength = timeLength;
	}

	CMovePath *GetPath()
	{
		return m_Path.get();
	}

	void SetPath(CMovePath *path)
	{
		m_Path = path;
	}

	CMoveSpeed *GetSpeed()
	{
		return m_Speed.get();
	}
	
	void SetSpeed(CMoveSpeed *speed)
	{
		m_Speed = speed;
	}

	CMovePose *GetPose()
	{
		return m_Pose.get();
	}
	
	void SetPose(CMovePose *pose)
	{
		m_Pose = pose;
	}

	//XML读取和保存模块
	bool Init(MSXML2::IXMLDOMNodePtr pNode);
	MSXML2::IXMLDOMNodePtr GetXMLNode();

	CMoveTrack(void)
	{
		m_TimeLength = 0.f;
	}

protected:
	osg::ref_ptr<CMovePath>		m_Path;			//运动路径对象
	osg::ref_ptr<CMoveSpeed>	m_Speed;		//运动速度对象
	osg::ref_ptr<CMovePose>		m_Pose;			//运动姿态对象
	double							m_TimeLength;	//运动耗时
};

}