/************************************************************************
文件:			COM_MoveSpeed.h
作者:			张平    
创建日期:		2009-03-16
描述:			运动控制类中的速度类，主要是描述运动中的速度(标量)

修改记录：
作者					日期					描述
-------------------------------------------------------------------------
黄志鹏				2009-07-21			修改CCOM_MoveSpeed类从MFC CObject类
										继承，使之具有Serialize功能.

************************************************************************/

#pragma once

#include <temp/Export.h>
#include <Osg/Referenced>
#include <Osg/Vec3>
#include <Osg/Vec2>


namespace TEMP	{

enum{
	ENM_MoveSpeedType_UnDefault = 0,
	ENM_MoveSpeedType_Uniform,
	ENM_MoveSpeedType_Uniform_Angle,
	ENM_MoveSpeedType_Uniform_Angle_Both,
	ENM_MoveSpeedType_Uniform_Angle_Three,
};

//运动速度基类
class TEMP_EXPORT CCOM_MoveSpeed : public osg::Referenced, public CObject
{
public:
	virtual ~CCOM_MoveSpeed(void)
	{
	}

	virtual CCOM_MoveSpeed * Clone()
	{
		return NULL;
	}

	/*
	目的：返回某一时刻的速度
	参数：time		(in)		时刻
		  speed		(out)		返回速度值
	返回：成功返回true，speed有效，否则无效
	*/
	virtual bool GetSpeed(double time, double &speed)
	{
		return false;
	}

	/*
	目的：返回从0到某一时刻已运动的路程
	参数：time		(in)		时刻（从0开始）
		 distance	(out)		返回路程值
	返回：成功返回true，distance有效，否则distance无效
	*/
	virtual bool GetDistance(double time, double &distance)
	{
		return false;
	}

	DWORD GetType()
	{
		return m_Type;
	}

	virtual CString GetTypeName();

	/*
	目的：序列化操作
	*/
	virtual void Serialize(CArchive& ar);

	virtual bool ParserXml(MSXML2::IXMLDOMNodePtr pNode);

protected:
	CCOM_MoveSpeed(void)
	{
		m_Type = ENM_MoveSpeedType_UnDefault;
	}

	DECLARE_SERIAL(CCOM_MoveSpeed)

protected:
	DWORD	m_Type;
	static CString m_sTypeName;
};


//匀加速运动
class TEMP_EXPORT CCOM_MSUniform : public CCOM_MoveSpeed
{
public:
	virtual ~CCOM_MSUniform()
	{
	}

	CCOM_MoveSpeed * Clone()
	{
		return new CCOM_MSUniform(m_v, m_a);
	}

	/*
	目的：构造函数
	参数：v		(in)	初速度
		  a		(in)	加速度
	*/
	CCOM_MSUniform(double v, double a = 0)
	{
		m_Type = ENM_MoveSpeedType_Uniform;
		m_v = v;
		m_a = a;
	}

	/*
	目的：获取某个时刻的速度值
	参数：time		(in)	时刻
		  speed		(out)	速度
	返回：成功返回true，speed值有效，否则无效
	*/
	bool GetSpeed(double time, double &speed)
	{
		speed = m_v + m_a * time;
		return true;
	}

	/*
	目的：返回从0到某一时刻已运动的路程
	参数：time		(in)		时刻（从0开始）
	      distance	(out)		返回路程值
	返回：成功返回true，distance有效，否则distance无效
	*/
	bool GetDistance(double time, double &distance)
	{
		distance = (m_v + m_v + m_a * time) * time * 0.5f;
		return true;
	}

	CString GetTypeName();

	/*
	目的：设置起始速率和加速率
	*/
	BOOL SetOriginSpeedAndAcceleration(double v, double a);
	BOOL GetOriginSpeedAndAcceleration(double &v, double &a);

	/*
	目的：序列化操作
	*/
	virtual void Serialize(CArchive& ar);

	virtual bool ParserXml(MSXML2::IXMLDOMNodePtr pNode);

	CCOM_MSUniform()
	{
		m_Type = ENM_MoveSpeedType_Uniform;
		m_v = 0.f;
		m_a = 0.f;
	}

protected:
	DECLARE_SERIAL(CCOM_MSUniform)

protected:
	double m_v;		//初速度
	double m_a;		//加速率
	static CString m_sTypeName;
};

}