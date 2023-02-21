/*************************************************************************
File:			MovePose.h
Author:			张平    
Date:			2009-03-16
Description:    运动控制类中的姿态类，主要是描述运动对象的姿态. 姿态的表示为：
		北向夹角、俯仰角、滚转角.

ChangeLog:
作者					日期					描述
--------------------------------------------------------------------------
黄志鹏				2009-07-21			修改CMovePose类从MFC CObject继承，
										使之具有Serialize功能.
黄志鹏				2009-09-17			添加姿态类--弧线段型匀变化
黄志鹏				2009-09-17			添加姿态类--视点锁定某个点
黄志鹏				2009-10-06			添加姿态类--视点锁定某个线段
黄志鹏				2009-12-09			添加姿态类--使用四元数插值计算

*************************************************************************/

#pragma once
#include <osg/Referenced>
#include <osg/Vec3d>
#include <osg/Vec2>
#include <osg/Quat>

#include <COM\Movement\MovePath.h>

#include <COM\XML\XMLNode.h>
#include <COM\XML\XMLApi.h>

namespace COM{

enum{
	ENM_MovePoseType_UnDefault = 0,
	ENM_MovePoseType_ConstAngle,
	ENM_MovePoseType_ChangeAngle,
	ENM_MovePoseType_Forward,
	ENM_MovePoseType_AngleSegment,
	ENM_MovePoseType_EyeLockPoint,
	ENM_MovePoseType_EyeLockSegment,
	ENM_MovePoseType_QuatSlerp,
};

//姿态基类
class COM_EXPORT CMovePose : public CXMLNode, public osg::Referenced
{
public:
	virtual ~CMovePose(void)
	{
	}

	virtual CMovePose * Clone()
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
	virtual bool GetPose(double time, double timeLen, CMovePath &path, CMoveSpeed &speed, 
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

	CMovePose(void)
	{
		m_Type = ENM_MovePoseType_UnDefault;
		m_blocal = FALSE;
	}

protected:
	DWORD m_Type;				//姿态变换类型
	static CString m_sTypeName;

	//局部坐标系功能(v3.0)
	BOOL			m_blocal;	//标志位
};


//姿态--均匀改变姿态类(弧射线型)
class COM_EXPORT CMPSChangeAngle : public CMovePose
{
public:
	virtual ~CMPSChangeAngle()
	{
	}

	CMovePose * Clone()
	{
		return new CMPSChangeAngle(m_originPose, m_angleSpeed);
	}

	/*
	目的：构造函数
	参数：originPose	(in)	初始姿态
		  startPose		(in)	起始姿态	---\
		  endPose		(in)	结束姿态	----+用于计算姿态变化速度
		  timeLen		(in)	耗时		---/
	*/
	CMPSChangeAngle(osg::Vec3d originPose, osg::Vec3d startPose, osg::Vec3d endPose,
				double timeLen)
	{
		m_Type = ENM_MovePoseType_ChangeAngle;
		m_originPose = originPose;
		if (timeLen > 0.f)
		{
			m_angleSpeed = (endPose - startPose) / timeLen;
		}
	}

	CMPSChangeAngle(osg::Vec3d originPose, osg::Vec3d angleSpeed)
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
	virtual bool GetPose(double time, double timeLen, CMovePath &path, CMoveSpeed &speed, 
						osg::Vec3d& pose);

	/*
	目的：设置和获取初始姿态和姿态变化速度
	*/
	BOOL SetOriginPoseAndChangeSpeed(osg::Vec3d &originPose, osg::Vec3d &angleSpeed);
	BOOL GetOriginPoseAndChangeSpeed(osg::Vec3d &originPose, osg::Vec3d &angleSpeed);

	//获取姿态类型名称
	CString GetTypeName();

	//XML读取和保存模块
	bool Init(MSXML2::IXMLDOMNodePtr pNode);
	MSXML2::IXMLDOMNodePtr GetXMLNode();

	CMPSChangeAngle()
	{
		m_Type = ENM_MovePoseType_ChangeAngle;
	}

protected:
	osg::Vec3d m_originPose;	//初始姿态
	osg::Vec3d m_angleSpeed;	//变化速度
	static CString m_sTypeName;
};


//姿态类--永远面向前方
class COM_EXPORT CPoseForward : public CMovePose
{
public:
	CPoseForward()
	{
		m_Type = ENM_MovePoseType_Forward;
	}

	virtual ~CPoseForward()
	{

	}

	CMovePose *Clone()
	{
		return new CPoseForward();
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
	virtual bool GetPose(double time, double timeLen, CMovePath &path, CMoveSpeed &speed, 
						osg::Vec3d& pose);

	//获取姿态类型名称
	CString GetTypeName();

	//XML读取和保存模块
	bool Init(MSXML2::IXMLDOMNodePtr pNode);
	MSXML2::IXMLDOMNodePtr GetXMLNode();

private:
	static CString	m_sTypeName;
};


//姿态类--匀变化(弧线段型)
class COM_EXPORT CPoseAngleSegment : public CMovePose
{
public:
	/*
	目的：构造函数
	参数：originPose	(in)		起始姿态
	      endPose		(in)		终止姿态
	*/
	CPoseAngleSegment(osg::Vec3d originPose, osg::Vec3d endPose)
	{
		m_Type = ENM_MovePoseType_AngleSegment;
		m_originPose = originPose;
		m_endPose = endPose;
	}

	CMovePose * Clone()
	{
		return new CPoseAngleSegment(m_originPose, m_endPose);
	}

	virtual ~CPoseAngleSegment()
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
	virtual bool GetPose(double time, double timeLen, CMovePath &path, CMoveSpeed &speed, 
						osg::Vec3d& pose);

	/*
	目的：设置和获取初始姿态和终止姿态
	*/
	BOOL SetOriginPoseAndEndPose(osg::Vec3d &originPose, osg::Vec3d &endPose);
	BOOL GetOriginPoseAndEndPose(osg::Vec3d &originPose, osg::Vec3d &endPose);

	//获取姿态类型名称
	CString GetTypeName();

	//XML读取和保存模块
	bool Init(MSXML2::IXMLDOMNodePtr pNode);
	MSXML2::IXMLDOMNodePtr GetXMLNode();

	CPoseAngleSegment()
	{
		m_Type = ENM_MovePoseType_AngleSegment;
	}

private:
	osg::Vec3d		m_originPose;
	osg::Vec3d		m_endPose;
	static CString	m_sTypeName;
};


//姿态类--视点锁定某个点
class COM_EXPORT CPoseEyeLockPoint : public CMovePose
{
public:
	/*
	目的：构造函数
	参数：destPoint		(in)		观察目标点
	*/
	CPoseEyeLockPoint(osg::Vec3d destPoint)
	{
		m_Type = ENM_MovePoseType_EyeLockPoint;
		m_destPoint = destPoint;
	}

	CMovePose * Clone()
	{
		return new CPoseEyeLockPoint(m_destPoint);
	}

	virtual ~CPoseEyeLockPoint()
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
	virtual bool GetPose(double time, double timeLen, CMovePath &path, CMoveSpeed &speed, 
						osg::Vec3d& pose);

	/*
	目的：设置和获取目标点
	*/
	BOOL SetDestPoint(const osg::Vec3d &destPoint);
	BOOL GetDestPoint(osg::Vec3d &destPoint);

	//获取姿态类型名称
	CString GetTypeName();

	CPoseEyeLockPoint()
	{
		m_Type = ENM_MovePoseType_EyeLockPoint;
	}

	//XML读取和保存模块
	bool Init(MSXML2::IXMLDOMNodePtr pNode);
	MSXML2::IXMLDOMNodePtr GetXMLNode();
protected:
	osg::Vec3d	m_destPoint;
	static CString	m_sTypeName;
};

//姿态类--视点锁定某个线段
class COM_EXPORT CPoseEyeLockSegment : public CMovePose
{
public:
	/*
	目的：构造函数
	参数：startPoint		(in)		观察目标线段起点
	      endPoint			(in)		观察目标线段终点
	*/
	CPoseEyeLockSegment(osg::Vec3d startPoint, osg::Vec3d endPoint)
	{
		m_Type = ENM_MovePoseType_EyeLockSegment;
		m_startPoint = startPoint;
		m_endPoint = endPoint;
	}

	CMovePose * Clone()
	{
		return new CPoseEyeLockSegment(m_startPoint, m_endPoint);
	}

	virtual ~CPoseEyeLockSegment()
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
	virtual bool GetPose(double time, double timeLen, CMovePath &path, CMoveSpeed &speed, 
		osg::Vec3d& pose);

	/*
	目的：设置和获取目标线段
	*/
	BOOL SetDestLineSegment(const osg::Vec3d &startPoint, const osg::Vec3d &endPoint);
	BOOL GetDestLineSegment(osg::Vec3d &startPoint, osg::Vec3d &endPoint);

	//获取姿态类型名称
	CString GetTypeName();

	//XML读取和保存模块
	bool Init(MSXML2::IXMLDOMNodePtr pNode);
	MSXML2::IXMLDOMNodePtr GetXMLNode();

	CPoseEyeLockSegment()
	{
		m_Type = ENM_MovePoseType_EyeLockSegment;
	}

private:
	osg::Vec3d m_startPoint;
	osg::Vec3d m_endPoint;
	static CString	m_sTypeName;
};

//姿态类--使用四元数插值计算
class COM_EXPORT CPoseQuatSlerp : public CMovePose
{
public:
	CPoseQuatSlerp();
	CPoseQuatSlerp(const CPoseQuatSlerp &pose);
	CPoseQuatSlerp(osg::Vec3d originPose, osg::Vec3d endPose);
	virtual ~CPoseQuatSlerp();

	CMovePose *Clone();

	/*
	目的：得到某时刻的姿态(在球坐标系下)
	参数：time		(in)		时刻
	      timeLen	(in)		运动片耗时
	      path		(in)		路径对象
	      speed		(in)		速度对象
	      pose		(out)		返回姿态值
	返回：成功返回true，pose的值有效，否则无效
	*/
	virtual bool GetPose(double time, double timeLen, CMovePath &path, CMoveSpeed &speed, 
		osg::Vec3d& pose);

	//获取姿态类型名称
	CString GetTypeName();

	//XML读取和保存模块
	bool Init(MSXML2::IXMLDOMNodePtr pNode);
	MSXML2::IXMLDOMNodePtr GetXMLNode();

private:
	osg::Vec3d		m_originPose;
	osg::Vec3d		m_endPose;
	osg::Quat		m_from;
	osg::Quat		m_to;
	static CString	m_sTypeName;
};

CMovePose *MovePoseFactory(MSXML2::IXMLDOMNodePtr pNode);

}