/**************************************************************************
File:			MovePath.h
Author:			��ƽ    
Date:			2009-03-16
Description:    �˶��������е��˶�����·���࣬��Ҫ�������˶��е�·���켣��Ϣ����״��
	·�̵ȡ�������������ϵ

ChangeLog:
����					����					����
---------------------------------------------------------------------------
��־��				2009-09-17			����˶�·��--�߶�.
��־��				2009-09-17			����˶�·��--���߶�.

***************************************************************************/

#pragma once

#include <Osg/Referenced>
#include <Osg/Vec3d>
#include <Osg/Vec2>
#include <osg/ref_ptr>

#include <COM\Movement\MoveSpeed.h>

#include <COM\XML\XMLNode.h>
#include <COM\XML\XMLApi.h>

namespace COM{

//////////////////////////////////////////////////////////////////////////
//�˶�·������
class COM_EXPORT CMovePath : public CXMLNode, public osg::Referenced
{
public:
	virtual ~CMovePath(void)
	{
	}

	virtual CMovePath * Clone()
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
	Ŀ�ģ�����ĳʱ���˶������λ��
	������time		(in)	ʱ��ֵ
		  speed		(in)	�ٶȶ�������
		  position	(out)	����λ��
	���أ��ɹ�����true��λ��ֵ��Ч������λ��ֵ��Ч
	*/
	virtual bool GetPosition(double time, CMoveSpeed &speed, osg::Vec3d &position)
	{
		return false;
	}

	//����·���ĳ���
	virtual bool GetLength(double &distance)
	{
		return false;
	}

	/*
	Ŀ�ģ������˶��켣����
	*/
	DWORD GetType()
	{
		return m_Type;
	}

	/*
	Ŀ�ģ���ȡ�켣��������
	*/
	virtual CString GetTypeName();

protected:
	CMovePath(void)
	{
		m_Type = ENM_MovePathType_UnDefault;
		m_blocal = FALSE;
	}

protected:
	DWORD m_Type;
	static CString m_sTypeName;

	//�ֲ�����ϵ����(v3.0)
	BOOL			m_blocal;	//��־λ
};



//////////////////////////////////////////////////////////////////////////
//�˶�·��--ֱ��(����)��
class COM_EXPORT CMPLine : public CMovePath
{
public:
	/*
	Ŀ�ģ����캯��
	������origin		(in)		���
		  direction		(in)		����
	*/
	CMPLine(osg::Vec3d origin, osg::Vec3d direction)
	{
		m_origin = origin;
		m_direction = direction;
		m_direction.normalize();
	}

	CMovePath * Clone()
	{
		return new CMPLine(m_origin, m_direction);
	}

	virtual ~CMPLine()
	{ 
	}

	/*
	Ŀ�ģ�����ĳʱ���˶������λ��
	������time		(in)	ʱ��ֵ
		  speed		(in)	�ٶȶ�������
		  position	(out)	����λ��
	���أ��ɹ�����true��λ��ֵ��Ч������λ��ֵ��Ч
	*/
	virtual bool GetPosition(double time, CMoveSpeed &speed, osg::Vec3d &position);

	/*
	Ŀ�ģ���ȡ�켣��������
	*/
	CString GetTypeName();

	/*
	Ŀ�ģ���ȡ���ͷ���
	*/
	BOOL GetOriginPointAndDirection(osg::Vec3d &origin, osg::Vec3d &direction);
	
	/*
	Ŀ�ģ��������ͷ���
	*/
	BOOL SetOriginPointAndDirection(const osg::Vec3d &origin, const osg::Vec3d &direction);

	//XML��ȡ�ͱ���ģ��
	bool Init(MSXML2::IXMLDOMNodePtr pNode);
	MSXML2::IXMLDOMNodePtr GetXMLNode();
	CMPLine(void)
	{
		m_Type = ENM_MovePathType_Line;
	}

private:
	osg::Vec3d m_origin;	//������
	osg::Vec3d m_direction;	//����
	static CString m_sTypeName;
};


//////////////////////////////////////////////////////////////////////////
//�˶�·��---�߶���
class COM_EXPORT CMPLineSegment : public CMovePath
{
public:
	/*
	Ŀ�ģ����캯��
	������origin		(in)		���
	      end			(in)		�յ�
	*/
	CMPLineSegment(osg::Vec3d origin, osg::Vec3d end);

	CMovePath * Clone();

	virtual ~CMPLineSegment();

	/*
	Ŀ�ģ�����ĳʱ���˶������λ��
	������time		(in)	ʱ��ֵ
	      speed		(in)	�ٶȶ�������
	      position	(out)	����λ��
	���أ��ɹ�����true��λ��ֵ��Ч������λ��ֵ��Ч
	*/
	virtual bool GetPosition(double time, CMoveSpeed &speed, osg::Vec3d &position);

	bool GetLength(double &distance);

	/*
	Ŀ�ģ���ȡ�켣��������
	*/
	CString GetTypeName();

	/*
	Ŀ�ģ���ȡ�����յ�
	*/
	BOOL GetOriginPointAndEndPoint(osg::Vec3d &origin, osg::Vec3d &end);

	/*
	Ŀ�ģ����������յ�
	*/
	BOOL SetOriginPointAndEndPoint(const osg::Vec3d &origin, const osg::Vec3d &end);

	//XML��ȡ�ͱ���ģ��
	bool Init(MSXML2::IXMLDOMNodePtr pNode);
	MSXML2::IXMLDOMNodePtr GetXMLNode();
	CMPLineSegment(void)
	{
		m_Type = ENM_MovePathType_LineSegment;
	}

private:
	osg::Vec3d	m_origin;	//���
	osg::Vec3d	m_end;		//�յ�
	double		m_length;	//����
	static CString m_sTypeName;
};


//////////////////////////////////////////////////////////////////////////
//�˶�·��--���߶���
class COM_EXPORT CMPArcSegment : public CMovePath
{
public:
	/*
	Ŀ�ģ����캯��
	������origin		(in)		���
	      end			(in)		�յ�
	      center		(in)		����
	*/
	CMPArcSegment(osg::Vec3d origin, osg::Vec3d end, osg::Vec3d center);

	CMovePath * Clone();

	virtual ~CMPArcSegment();
	/*
	Ŀ�ģ�����ĳʱ���˶������λ��
	������time		(in)	ʱ��ֵ
	      speed		(in)	�ٶȶ�������
	      position	(out)	����λ��
	���أ��ɹ�����true��λ��ֵ��Ч������λ��ֵ��Ч
	*/
	virtual bool GetPosition(double time, CMoveSpeed &speed, osg::Vec3d &position);

	bool GetLength(double &distance);

	/*
	Ŀ�ģ���ȡ�켣��������
	*/
	CString GetTypeName();

	/*
	Ŀ�ģ���ȡ��㡢�յ�ͻ���
	*/
	BOOL GetOriginPointEndPointCenterPoint(osg::Vec3d &origin, osg::Vec3d &end, osg::Vec3d &center);

	/*
	Ŀ�ģ�������㡢�յ�ͻ���
	*/
	BOOL SetOriginPointEndPointCenterPoint(const osg::Vec3d &origin, const osg::Vec3d &end, const osg::Vec3 &center);

	//XML��ȡ�ͱ���ģ��
	bool Init(MSXML2::IXMLDOMNodePtr pNode);
	MSXML2::IXMLDOMNodePtr GetXMLNode();

	CMPArcSegment(void)
	{
		m_Type = ENM_MovePathType_ArcSegment;
	}

private:
	osg::Vec3d	m_origin;	//���
	osg::Vec3d	m_end;		//�յ�
	osg::Vec3d	m_center;	//����
	double		m_length;	//����(��㵽�յ��ֱ�߾���)
	static CString m_sTypeName;
};

//XML��ȡ�ͱ���ģ��
COM_EXPORT CMovePath *MovePathFactory(MSXML2::IXMLDOMNodePtr pNode);

}
