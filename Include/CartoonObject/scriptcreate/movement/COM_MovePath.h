/**************************************************************************
File:			COM_MovePath.h
Author:			张平    
Date:			2009-03-16
Description:    运动控制类中的运动控制路径类，主要是描述运动中的路径轨迹信息：形状、
	路程等。采用世界坐标系

ChangeLog:
作者					日期					描述
---------------------------------------------------------------------------
黄志鹏				2009-07-21			修改CCOM_MovePath类从MFC CObject继承，
										使之具有Serialize功能.
黄志鹏				2009-09-17			添加运动路径--线段.
黄志鹏				2009-09-17			添加运动路径--弧线段.

***************************************************************************/

#pragma once

#include <CartoonObject/Export.h>
#include <Osg/Referenced>
#include <Osg/Vec3d>
#include <Osg/Vec2>
#include <CartoonObject/ScriptCreate/Movement/COM_MoveSpeed.h>


namespace CartoonObject		{

//////////////////////////////////////////////////////////////////////////
//运动路径基类
class CARTOONOBJECT_EXPORT CCOM_MovePath : public CObject, public osg::Referenced
{
public:
	virtual ~CCOM_MovePath(void)
	{
	}

	virtual CCOM_MovePath * Clone()
	{
		return NULL;
	}

	enum{
		ENM_MovePathType_UnDefault = 0,
		ENM_MovePathType_Line,
		ENM_MovePathType_LineSegment,
		ENM_MovePathType_Round,
		ENM_MovePathType_RoundAndLine,
		ENM_MovePathType_ArcSegment,
	};

	/*
	目的：返回某时刻运动对象的位置
	参数：time		(in)	时刻值
		  speed		(in)	速度对象引用
		  position	(out)	返回位置
	返回：成功返回true，位置值有效；否则位置值无效
	*/
	virtual bool GetPosition(double time, CCOM_MoveSpeed &speed, osg::Vec3d &position)
	{
		return false;
	}

	//返回路径的长度
	virtual bool GetLength(double &distance)
	{
		return false;
	}

	/*
	目的：返回运动轨迹类型
	*/
	DWORD GetType()
	{
		return m_Type;
	}

	/*
	目的：获取轨迹类型名称
	*/
	virtual CString GetTypeName();

	/*
	获取起始点、终点坐标
	*/
	BOOL GetOriginPointAndEndPoint(osg::Vec3d &origin, osg::Vec3d &end);

	/*
	Purpose:	Parser XML	Data Format for Animation
	Author:		Liugh
	*/
	virtual bool ParserXml(MSXML2::IXMLDOMNodePtr pNode);
	/*
	Purpose:	Return XML	Node
	Author:		Liugh
	*/
	virtual MSXML2::IXMLDOMNodePtr GetXmlNode();

protected:
	CCOM_MovePath(void)
	{
		m_Type = ENM_MovePathType_UnDefault;
	}


protected:
	DWORD m_Type;
	static CString m_sTypeName;
};


//////////////////////////////////////////////////////////////////////////
//运动路径--直线(射线)类
class CARTOONOBJECT_EXPORT CCOM_MPLine : public CCOM_MovePath
{
public:
	/*
	目的：构造函数
	参数：origin		(in)		起点
		  direction		(in)		方向
	*/
	CCOM_MPLine(osg::Vec3d origin, osg::Vec3d direction)
	{
		m_origin = origin;
		m_direction = direction;
		m_direction.normalize();
	}

	CCOM_MovePath * Clone()
	{
		return new CCOM_MPLine(m_origin, m_direction);
	}

	virtual ~CCOM_MPLine()
	{ 
	}

	/*
	目的：返回某时刻运动对象的位置
	参数：time		(in)	时刻值
		  speed		(in)	速度对象引用
		  position	(out)	返回位置
	返回：成功返回true，位置值有效；否则位置值无效
	*/
	virtual bool GetPosition(double time, CCOM_MoveSpeed &speed, osg::Vec3d &position);

	/*
	目的：获取轨迹类型名称
	*/
	CString GetTypeName();

	/*
	目的：获取起点和方向
	*/
	BOOL GetOriginPointAndDirection(osg::Vec3d &origin, osg::Vec3d &direction);
	
	/*
	目的：设置起点和方向
	*/
	BOOL SetOriginPointAndDirection(const osg::Vec3d &origin, const osg::Vec3d &direction);


	virtual bool ParserXml(MSXML2::IXMLDOMNodePtr pNode);

	MSXML2::IXMLDOMNodePtr GetXmlNode();

	CCOM_MPLine(void)
	{
		m_Type = ENM_MovePathType_Line;
	}


private:
	osg::Vec3d m_origin;	//出发点
	osg::Vec3d m_direction;	//方向
	static CString m_sTypeName;
};


//////////////////////////////////////////////////////////////////////////
//运动路径---线段类
class CARTOONOBJECT_EXPORT CCOM_LineSegment : public CCOM_MovePath
{
public:
	/*
	目的：构造函数
	参数：origin		(in)		起点
	      end			(in)		终点
	*/
	CCOM_LineSegment(osg::Vec3d origin, osg::Vec3d end)
	{
		m_origin = origin;
		m_end = end;
		m_length = (m_end - m_origin).length();
	}

	CCOM_MovePath * Clone()
	{
		return new CCOM_LineSegment(m_origin, m_end);
	}

	virtual ~CCOM_LineSegment()
	{
	}

	/*
	目的：返回某时刻运动对象的位置
	参数：time		(in)	时刻值
	      speed		(in)	速度对象引用
	      position	(out)	返回位置
	返回：成功返回true，位置值有效；否则位置值无效
	*/
	virtual bool GetPosition(double time, CCOM_MoveSpeed &speed, osg::Vec3d &position);

	virtual bool GetLength(double &distance)
	{
		distance = (m_end - m_origin).length();
		return true;
	}

	/*
	目的：获取轨迹类型名称
	*/
	CString GetTypeName();

	/*
	目的：获取起点和终点
	*/
	BOOL GetOriginPointAndEndPoint(osg::Vec3d &origin, osg::Vec3d &end);

	/*
	目的：设置起点和终点
	*/
	BOOL SetOriginPointAndEndPoint(const osg::Vec3d &origin, const osg::Vec3d &end);


	virtual bool ParserXml(MSXML2::IXMLDOMNodePtr pNode);
	/*
	Purpose:	Return XML	Node
	Author:		Liugh
	*/
	virtual MSXML2::IXMLDOMNodePtr GetXmlNode();

	CCOM_LineSegment(void)
	{
		m_Type = ENM_MovePathType_LineSegment;
	}
private:
	osg::Vec3d	m_origin;	//起点
	osg::Vec3d	m_end;		//终点
	double		m_length;	//长度
	static CString m_sTypeName;
};


//////////////////////////////////////////////////////////////////////////
//运动路径--弧线段类
class CARTOONOBJECT_EXPORT CCOM_PathArcSegment : public CCOM_MovePath
{
public:
	/*
	目的：构造函数
	参数：origin		(in)		起点
	      end			(in)		终点
	      center		(in)		弧心
	*/
	CCOM_PathArcSegment(osg::Vec3d origin, osg::Vec3d end, osg::Vec3d center)
	{
		m_origin = origin;
		m_end = end;
		m_center = center;
		m_length = (m_end - m_origin).length();
		m_Type = ENM_MovePathType_ArcSegment;
	}

	CCOM_MovePath * Clone()
	{
		return new CCOM_PathArcSegment(m_origin, m_end, m_center);
	}

	virtual ~CCOM_PathArcSegment()
	{
	}

	/*
	目的：返回某时刻运动对象的位置
	参数：time		(in)	时刻值
	      speed		(in)	速度对象引用
	      position	(out)	返回位置
	返回：成功返回true，位置值有效；否则位置值无效
	*/
	virtual bool GetPosition(double time, CCOM_MoveSpeed &speed, osg::Vec3d &position);

	virtual bool GetLength(double &distance)
	{
		distance = (m_end - m_origin).length();
		return true;
	}

	/*
	目的：获取轨迹类型名称
	*/
	CString GetTypeName();

	/*
	目的：获取起点、终点和弧心
	*/
	BOOL GetOriginPointEndPointCenterPoint(osg::Vec3d &origin, osg::Vec3d &end, osg::Vec3d &center);

	/*
	目的：设置起点、终点和弧心
	*/
	BOOL SetOriginPointEndPointCenterPoint(const osg::Vec3d &origin, const osg::Vec3d &end, const osg::Vec3 &center);

	virtual bool ParserXml(MSXML2::IXMLDOMNodePtr pNode);

	CCOM_PathArcSegment(void)
	{
		m_Type = ENM_MovePathType_ArcSegment;
	}


private:
	osg::Vec3d	m_origin;	//起点
	osg::Vec3d	m_end;		//终点
	osg::Vec3d	m_center;	//弧心
	double		m_length;	//长度(起点到终点的直线距离)
	static CString m_sTypeName;
};

}