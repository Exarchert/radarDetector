/**************************************************************************
File:			COM_MovePath.h
Author:			��ƽ    
Date:			2009-03-16
Description:    �˶��������е��˶�����·���࣬��Ҫ�������˶��е�·���켣��Ϣ����״��
	·�̵ȡ�������������ϵ

ChangeLog:
����					����					����
---------------------------------------------------------------------------
��־��				2009-07-21			�޸�CCOM_MovePath���MFC CObject�̳У�
										ʹ֮����Serialize����.
��־��				2009-09-17			����˶�·��--�߶�.
��־��				2009-09-17			����˶�·��--���߶�.

***************************************************************************/

#pragma once

#include <CartoonObject/Export.h>
#include <Osg/Referenced>
#include <Osg/Vec3d>
#include <Osg/Vec2>
#include <CartoonObject/ScriptCreate/Movement/COM_MoveSpeed.h>


namespace CartoonObject		{

//////////////////////////////////////////////////////////////////////////
//�˶�·������
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
	Ŀ�ģ�����ĳʱ���˶������λ��
	������time		(in)	ʱ��ֵ
		  speed		(in)	�ٶȶ�������
		  position	(out)	����λ��
	���أ��ɹ�����true��λ��ֵ��Ч������λ��ֵ��Ч
	*/
	virtual bool GetPosition(double time, CCOM_MoveSpeed &speed, osg::Vec3d &position)
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

	/*
	��ȡ��ʼ�㡢�յ�����
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
//�˶�·��--ֱ��(����)��
class CARTOONOBJECT_EXPORT CCOM_MPLine : public CCOM_MovePath
{
public:
	/*
	Ŀ�ģ����캯��
	������origin		(in)		���
		  direction		(in)		����
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
	Ŀ�ģ�����ĳʱ���˶������λ��
	������time		(in)	ʱ��ֵ
		  speed		(in)	�ٶȶ�������
		  position	(out)	����λ��
	���أ��ɹ�����true��λ��ֵ��Ч������λ��ֵ��Ч
	*/
	virtual bool GetPosition(double time, CCOM_MoveSpeed &speed, osg::Vec3d &position);

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


	virtual bool ParserXml(MSXML2::IXMLDOMNodePtr pNode);

	MSXML2::IXMLDOMNodePtr GetXmlNode();

	CCOM_MPLine(void)
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
class CARTOONOBJECT_EXPORT CCOM_LineSegment : public CCOM_MovePath
{
public:
	/*
	Ŀ�ģ����캯��
	������origin		(in)		���
	      end			(in)		�յ�
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
	Ŀ�ģ�����ĳʱ���˶������λ��
	������time		(in)	ʱ��ֵ
	      speed		(in)	�ٶȶ�������
	      position	(out)	����λ��
	���أ��ɹ�����true��λ��ֵ��Ч������λ��ֵ��Ч
	*/
	virtual bool GetPosition(double time, CCOM_MoveSpeed &speed, osg::Vec3d &position);

	virtual bool GetLength(double &distance)
	{
		distance = (m_end - m_origin).length();
		return true;
	}

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
	osg::Vec3d	m_origin;	//���
	osg::Vec3d	m_end;		//�յ�
	double		m_length;	//����
	static CString m_sTypeName;
};


//////////////////////////////////////////////////////////////////////////
//�˶�·��--���߶���
class CARTOONOBJECT_EXPORT CCOM_PathArcSegment : public CCOM_MovePath
{
public:
	/*
	Ŀ�ģ����캯��
	������origin		(in)		���
	      end			(in)		�յ�
	      center		(in)		����
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
	Ŀ�ģ�����ĳʱ���˶������λ��
	������time		(in)	ʱ��ֵ
	      speed		(in)	�ٶȶ�������
	      position	(out)	����λ��
	���أ��ɹ�����true��λ��ֵ��Ч������λ��ֵ��Ч
	*/
	virtual bool GetPosition(double time, CCOM_MoveSpeed &speed, osg::Vec3d &position);

	virtual bool GetLength(double &distance)
	{
		distance = (m_end - m_origin).length();
		return true;
	}

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

	virtual bool ParserXml(MSXML2::IXMLDOMNodePtr pNode);

	CCOM_PathArcSegment(void)
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

}