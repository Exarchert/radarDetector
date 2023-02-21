/*************************************************************************
File:			COM_MovePose.h
Author:			张平    
Date:			2009-03-16
Description:    运动控制类中的姿态类，主要是描述运动对象的姿态. 姿态的表示为：
		北向夹角、俯仰角、滚转角.

ChangeLog:
作者					日期					描述
--------------------------------------------------------------------------
黄志鹏				2009-07-21			修改CCOM_MovePose类从MFC CObject继承，
										使之具有Serialize功能.
黄志鹏				2009-09-17			添加姿态类--弧线段型匀变化
黄志鹏				2009-09-17			添加姿态类--视点锁定某个点
黄志鹏				2009-10-06			添加姿态类--视点锁定某个线段

*************************************************************************/

#pragma once

#include <temp/Export.h>
#include <osg/Referenced>
#include <osg/Vec3d>
#include <osg/Vec2>
#include <temp/Movement/COM_MovePath.h>


namespace TEMP		{

enum{
	ENM_MovePoseType_UnDefault = 0,
	ENM_MovePoseType_ConstAngle,
	ENM_MovePoseType_ChangeAngle,
	ENM_MovePoseType_Forward,
	ENM_MovePoseType_AngleSegment,
	ENM_MovePoseType_EyeLockPoint,
	ENM_MovePoseType_EyeLockSegment,
};

//姿态基类
class TEMP_EXPORT CCOM_MovePose : public osg::Referenced, public CObject
{
public:
	virtual ~CCOM_MovePose(void)
	{
	}

	virtual CCOM_MovePose * Clone()
	{
		return NULL;
	}

	/*
	目的：得到某时刻的姿态(在球坐标系下)
	参数：time		(in)		时刻
	      timeLen	(in)		运动片总耗时
		  path		(in)		路径对象
		  speed		(in)		速度对象
		  pose		(out)		返回姿态值
	返回：成功返回true，pose的值有效，否则无效
	*/
	virtual bool GetPose(double time, double timeLen, CCOM_MovePath &path, CCOM_MoveSpeed &speed, 
						osg::Vec3d& pose)
	{
		return false;
	}

	DWORD GetType()
	{
		return m_Type;
	}

	//获取姿态类型名称
	virtual CString GetTypeName();

	/*
	目的：序列化操作
	*/
	virtual void Serialize(CArchive& ar);

	virtual bool ParserXml(MSXML2::IXMLDOMNodePtr pNode);

protected:
	CCOM_MovePose(void)
	{
		m_Type = ENM_MovePoseType_UnDefault;
	}

	DECLARE_SERIAL(CCOM_MovePose)

protected:
	DWORD m_Type;				//姿态变换类型
	static CString m_sTypeName;
};


//姿态--均匀改变姿态类(弧射线型)
class TEMP_EXPORT CCOM_MPSChangeAngle : public CCOM_MovePose
{
public:
	virtual ~CCOM_MPSChangeAngle()
	{
	}

	CCOM_MovePose * Clone()
	{
		return new CCOM_MPSChangeAngle(m_originPose, m_angleSpeed);
	}

	/*
	目的：构造函数
	参数：originPose	(in)	初始姿态
		  startPose		(in)	起始姿态	---\
		  endPose		(in)	结束姿态	----+用于计算姿态变化速度
		  timeLen		(in)	耗时		---/
	*/
	CCOM_MPSChangeAngle(osg::Vec3d originPose, osg::Vec3d startPose, osg::Vec3d endPose,
				double timeLen)
	{
		m_Type = ENM_MovePoseType_ChangeAngle;
		m_originPose = originPose;
		if (timeLen > 0.f)
		{
			m_angleSpeed = (endPose - startPose) / timeLen;
		}
	}

	CCOM_MPSChangeAngle(osg::Vec3d originPose, osg::Vec3d angleSpeed)
	{
		m_Type = ENM_MovePoseType_ChangeAngle;
		m_originPose = originPose;
		m_angleSpeed = angleSpeed;
	}

	/*
	目的：得到某时刻的姿态(在球坐标系下)
	参数：time		(in)		时刻
	      timeLen	(in)		运动片总耗时
		  path		(in)		路径对象
		  speed		(in)		速度对象
		  pose		(out)		返回姿态值
	返回：成功返回true，pose的值有效，否则无效
	*/
	virtual bool GetPose(double time, double timeLen, CCOM_MovePath &path, CCOM_MoveSpeed &speed, 
						osg::Vec3d& pose);

	/*
	目的：设置和获取初始姿态和姿态变化速度
	*/
	BOOL SetOriginPoseAndChangeSpeed(osg::Vec3d &originPose, osg::Vec3d &angleSpeed);
	BOOL GetOriginPoseAndChangeSpeed(osg::Vec3d &originPose, osg::Vec3d &angleSpeed);

	//获取姿态类型名称
	CString GetTypeName();

	/*
	目的：序列化操作
	*/
	virtual void Serialize(CArchive& ar);

	virtual bool ParserXml(MSXML2::IXMLDOMNodePtr pNode);

	CCOM_MPSChangeAngle()
	{
		m_Type = ENM_MovePoseType_ChangeAngle;
	}

protected:
	DECLARE_SERIAL(CCOM_MPSChangeAngle)

private:
	osg::Vec3d m_originPose;	//初始姿态
	osg::Vec3d m_angleSpeed;	//变化速度
	static CString m_sTypeName;
};


//姿态类--永远面向前方
class TEMP_EXPORT CCOM_PoseForward : public CCOM_MovePose
{
public:
	CCOM_PoseForward()
	{
		m_Type = ENM_MovePoseType_Forward;
	}

	virtual ~CCOM_PoseForward()
	{

	}

	CCOM_MovePose *Clone()
	{
		return new CCOM_PoseForward();
	}

	/*
	目的：得到某时刻的姿态(在球坐标系下)
	参数：time		(in)		时刻
	      timeLen	(in)		运动片耗时
		  path		(in)		路径对象
		  speed		(in)		速度对象
		  pose		(out)		返回姿态值
	返回：成功返回true，pose的值有效，否则无效
	*/
	virtual bool GetPose(double time, double timeLen, CCOM_MovePath &path, CCOM_MoveSpeed &speed, 
						osg::Vec3d& pose);

	//获取姿态类型名称
	CString GetTypeName();

	/*
	目的：序列化操作
	*/
	virtual void Serialize(CArchive& ar);

	virtual bool ParserXml(MSXML2::IXMLDOMNodePtr pNode);

protected:
	DECLARE_SERIAL(CCOM_PoseForward)

private:
	static CString	m_sTypeName;
};


//姿态类--匀变化(弧线段型)
class TEMP_EXPORT CCOM_PoseAngleSegment : public CCOM_MovePose
{
public:
	/*
	目的：构造函数
	参数：originPose	(in)		起始姿态
	      endPose		(in)		终止姿态
	*/
	CCOM_PoseAngleSegment(osg::Vec3d originPose, osg::Vec3d endPose)
	{
		m_Type = ENM_MovePoseType_AngleSegment;
		m_originPose = originPose;
		m_endPose = endPose;
	}

	CCOM_MovePose * Clone()
	{
		return new CCOM_PoseAngleSegment(m_originPose, m_endPose);
	}

	virtual ~CCOM_PoseAngleSegment()
	{

	}

	/*
	目的：得到某时刻的姿态(在球坐标系下)
	参数：time		(in)		时刻
	      timeLen	(in)		运动片耗时
	      path		(in)		路径对象
	      speed		(in)		速度对象
	      pose		(out)		返回姿态值
	返回：成功返回true，pose的值有效，否则无效
	*/
	virtual bool GetPose(double time, double timeLen, CCOM_MovePath &path, CCOM_MoveSpeed &speed, 
						osg::Vec3d& pose);

	/*
	目的：设置和获取初始姿态和终止姿态
	*/
	BOOL SetOriginPoseAndEndPose(osg::Vec3d &originPose, osg::Vec3d &endPose);
	BOOL GetOriginPoseAndEndPose(osg::Vec3d &originPose, osg::Vec3d &endPose);

	//获取姿态类型名称
	CString GetTypeName();

	/*
	目的：序列化操作
	*/
	virtual void Serialize(CArchive& ar);

	virtual bool ParserXml(MSXML2::IXMLDOMNodePtr pNode);

	CCOM_PoseAngleSegment()
	{
		m_Type = ENM_MovePoseType_AngleSegment;
	}

protected:
		DECLARE_SERIAL(CCOM_PoseAngleSegment)

private:
	osg::Vec3d		m_originPose;
	osg::Vec3d		m_endPose;
	static CString	m_sTypeName;
};


//姿态类--视点锁定某个点
class TEMP_EXPORT CCOM_PoseEyeLockPoint : public CCOM_MovePose
{
public:
	/*
	目的：构造函数
	参数：destPoint		(in)		观察目标点
	*/
	CCOM_PoseEyeLockPoint(osg::Vec3d destPoint)
	{
		m_Type = ENM_MovePoseType_EyeLockPoint;
		m_destPoint = destPoint;
	}

	CCOM_MovePose * Clone()
	{
		return new CCOM_PoseEyeLockPoint(m_destPoint);
	}

	virtual ~CCOM_PoseEyeLockPoint()
	{

	}

	/*
	目的：得到某时刻的姿态(在球坐标系下)
	参数：time		(in)		时刻
	      timeLen	(in)		运动片耗时
	      path		(in)		路径对象
	      speed		(in)		速度对象
	      pose		(out)		返回姿态值
	返回：成功返回true，pose的值有效，否则无效
	*/
	virtual bool GetPose(double time, double timeLen, CCOM_MovePath &path, CCOM_MoveSpeed &speed, 
						osg::Vec3d& pose);

	/*
	目的：设置和获取目标点
	*/
	BOOL SetDestPoint(const osg::Vec3d &destPoint);
	BOOL GetDestPoint(osg::Vec3d &destPoint);

	//获取姿态类型名称
	CString GetTypeName();

	/*
	目的：序列化操作
	*/
	virtual void Serialize(CArchive& ar);

	virtual bool ParserXml(MSXML2::IXMLDOMNodePtr pNode);


	CCOM_PoseEyeLockPoint()
	{
		m_Type = ENM_MovePoseType_EyeLockPoint;
	}

protected:
	DECLARE_SERIAL(CCOM_PoseEyeLockPoint)

private:
	osg::Vec3d	m_destPoint;
	static CString	m_sTypeName;
};

//姿态类--视点锁定某个点
class TEMP_EXPORT CCOM_PoseEyeLockSegment : public CCOM_MovePose
{
public:
	/*
	目的：构造函数
	参数：startPoint		(in)		观察目标线段起点
	      endPoint			(in)		观察目标线段终点
	*/
	CCOM_PoseEyeLockSegment(osg::Vec3d startPoint, osg::Vec3d endPoint)
	{
		m_Type = ENM_MovePoseType_EyeLockSegment;
		m_startPoint = startPoint;
		m_endPoint = endPoint;
	}

	CCOM_MovePose * Clone()
	{
		return new CCOM_PoseEyeLockSegment(m_startPoint, m_endPoint);
	}

	virtual ~CCOM_PoseEyeLockSegment()
	{

	}

	/*
	目的：得到某时刻的姿态(在球坐标系下)
	参数：time		(in)		时刻
	      timeLen	(in)		运动片耗时
	      path		(in)		路径对象
	      speed		(in)		速度对象
	      pose		(out)		返回姿态值
	返回：成功返回true，pose的值有效，否则无效
	*/
	virtual bool GetPose(double time, double timeLen, CCOM_MovePath &path, CCOM_MoveSpeed &speed, 
		osg::Vec3d& pose);

	/*
	目的：设置和获取目标线段
	*/
	BOOL SetDestLineSegment(const osg::Vec3d &startPoint, const osg::Vec3d &endPoint);
	BOOL GetDestLineSegment(osg::Vec3d &startPoint, osg::Vec3d &endPoint);

	//获取姿态类型名称
	CString GetTypeName();

	/*
	目的：序列化操作
	*/
	virtual void Serialize(CArchive& ar);

	virtual bool ParserXml(MSXML2::IXMLDOMNodePtr pNode);

	CCOM_PoseEyeLockSegment()
	{
		m_Type = ENM_MovePoseType_EyeLockSegment;
	}

protected:
	DECLARE_SERIAL(CCOM_PoseEyeLockSegment)

private:
	osg::Vec3d m_startPoint;
	osg::Vec3d m_endPoint;
	static CString	m_sTypeName;
};

}