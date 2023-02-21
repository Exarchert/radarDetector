/*************************************************************************
File:			COM_MoveTrack.h
Author:			张平    
Date:			2009-03-16
Description:    运动控制类中的轨迹类，描述一个运动.

ChangeLog:
作者					日期					描述
--------------------------------------------------------------------------
黄志鹏				2009-07-21			修改CCOM_MoveTrack类从MFC CObject类
										继承，使之具有Serialize功能.

*************************************************************************/

#pragma once

#include <temp/Export.h>
#include <vector>
#include <osg/ref_ptr>
#include <temp/Movement/COM_MovePath.h>
#include <temp/Movement/COM_MovePose.h>
#include <temp/Movement/COM_MoveSpeed.h>


namespace TEMP		{

//运动基类
class TEMP_EXPORT CCOM_MoveTrack : public osg::Referenced, public CObject
{
public:
	/*
	目的：构造函数
	参数：path		(in)	运动路径对象指针
		 speed		(in)	运动速度对象指针
		 pose		(in)	运动姿态对象指针
		 timeLen	(in)	该运动消耗时间长度
	*/
	CCOM_MoveTrack(CCOM_MovePath *path, CCOM_MoveSpeed *speed, CCOM_MovePose *pose, double timeLen)
	{
		m_Path = path;
		m_Speed = speed;
		m_Pose = pose;
		m_TimeLength = timeLen;
	}

	CCOM_MoveTrack(const CCOM_MoveTrack &track);

	/*
	目的：克隆自己
	*/
	CCOM_MoveTrack * Clone()
	{
		return new CCOM_MoveTrack(*this);
	}

	virtual ~CCOM_MoveTrack(void)
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

	//返回某时刻的速率
	bool GetSpeed(double time, double &speed);

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

	CCOM_MovePath *GetPath()
	{
		return m_Path.get();
	}

	void SetPath(CCOM_MovePath *path)
	{
		m_Path = path;
	}

	CCOM_MoveSpeed *GetSpeed()
	{
		return m_Speed.get();
	}
	
	void SetSpeed(CCOM_MoveSpeed *speed)
	{
		m_Speed = speed;
	}

	CCOM_MovePose *GetPose()
	{
		return m_Pose.get();
	}
	
	void SetPose(CCOM_MovePose *pose)
	{
		m_Pose = pose;
	}

	/*
	目的：序列化操作
	*/
	virtual void Serialize(CArchive& ar);

	bool ParserXml(MSXML2::IXMLDOMNodePtr pNode);

	void SetPoseType(CString strType){m_strType = strType;}


public:
	CCOM_MoveTrack(void)
	{
		m_TimeLength = 0.f;
	}

	DECLARE_SERIAL(CCOM_MoveTrack)

protected:
	osg::ref_ptr<CCOM_MovePath>		m_Path;			//运动路径对象
	osg::ref_ptr<CCOM_MoveSpeed>	m_Speed;		//运动速度对象
	osg::ref_ptr<CCOM_MovePose>		m_Pose;			//运动姿态对象
	double							m_TimeLength;	//运动耗时
	CString	m_strType;
};

}