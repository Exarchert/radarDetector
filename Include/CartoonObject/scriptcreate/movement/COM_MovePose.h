/*************************************************************************
File:			COM_MovePose.h
Author:			��ƽ    
Date:			2009-03-16
Description:    �˶��������е���̬�࣬��Ҫ�������˶��������̬. ��̬�ı�ʾΪ��
		����нǡ������ǡ���ת��.

ChangeLog:
����					����					����
--------------------------------------------------------------------------
��־��				2009-07-21			�޸�CCOM_MovePose���MFC CObject�̳У�
										ʹ֮����Serialize����.
��־��				2009-09-17			�����̬��--���߶����ȱ仯
��־��				2009-09-17			�����̬��--�ӵ�����ĳ����
��־��				2009-10-06			�����̬��--�ӵ�����ĳ���߶�

*************************************************************************/

#pragma once

#include <CartoonObject/Export.h>
#include <osg/Referenced>
#include <osg/Vec3d>
#include <osg/Vec2>
#include <CartoonObject/ScriptCreate/Movement/COM_MovePath.h>


namespace CartoonObject		{

enum{
	ENM_MovePoseType_UnDefault = 0,
	ENM_MovePoseType_ConstAngle,
	ENM_MovePoseType_ChangeAngle,
	ENM_MovePoseType_Forward,
	ENM_MovePoseType_AngleSegment,
	ENM_MovePoseType_EyeLockPoint,
	ENM_MovePoseType_EyeLockSegment,
};

//��̬����
class CARTOONOBJECT_EXPORT CCOM_MovePose : public osg::Referenced, public CObject
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
	Ŀ�ģ��õ�ĳʱ�̵���̬(��������ϵ��)
	������time		(in)		ʱ��
	      timeLen	(in)		�˶�Ƭ�ܺ�ʱ
		  path		(in)		·������
		  speed		(in)		�ٶȶ���
		  pose		(out)		������ֵ̬
	���أ��ɹ�����true��pose��ֵ��Ч��������Ч
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

	//��ȡ��̬��������
	virtual CString GetTypeName();

	//��ȡʼĩ����̬
	virtual BOOL GetOriginPoseAndEndPose(osg::Vec3d &originPose, osg::Vec3d &endPose);

	virtual bool ParserXml(MSXML2::IXMLDOMNodePtr pNode);

	/*
	Purpose:	Return XML	Node
	Author:		Liugh
	*/
	virtual MSXML2::IXMLDOMNodePtr GetXmlNode();

protected:
	CCOM_MovePose(void)
	{
		m_Type = ENM_MovePoseType_UnDefault;
	}

protected:
	DWORD m_Type;				//��̬�任����
	static CString m_sTypeName;
};


//��̬--���ȸı���̬��(��������)
class CARTOONOBJECT_EXPORT CCOM_MPSChangeAngle : public CCOM_MovePose
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
	Ŀ�ģ����캯��
	������originPose	(in)	��ʼ��̬
		  startPose		(in)	��ʼ��̬	---\
		  endPose		(in)	������̬	----+���ڼ�����̬�仯�ٶ�
		  timeLen		(in)	��ʱ		---/
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
	Ŀ�ģ��õ�ĳʱ�̵���̬(��������ϵ��)
	������time		(in)		ʱ��
	      timeLen	(in)		�˶�Ƭ�ܺ�ʱ
		  path		(in)		·������
		  speed		(in)		�ٶȶ���
		  pose		(out)		������ֵ̬
	���أ��ɹ�����true��pose��ֵ��Ч��������Ч
	*/
	virtual bool GetPose(double time, double timeLen, CCOM_MovePath &path, CCOM_MoveSpeed &speed, 
						osg::Vec3d& pose);

	/*
	Ŀ�ģ����úͻ�ȡ��ʼ��̬����̬�仯�ٶ�
	*/
	BOOL SetOriginPoseAndChangeSpeed(osg::Vec3d &originPose, osg::Vec3d &angleSpeed);
	BOOL GetOriginPoseAndChangeSpeed(osg::Vec3d &originPose, osg::Vec3d &angleSpeed);

	//��ȡ��̬��������
	CString GetTypeName();


	virtual bool ParserXml(MSXML2::IXMLDOMNodePtr pNode);

	CCOM_MPSChangeAngle()
	{
		m_Type = ENM_MovePoseType_ChangeAngle;
	}


private:
	osg::Vec3d m_originPose;	//��ʼ��̬
	osg::Vec3d m_angleSpeed;	//�仯�ٶ�
	static CString m_sTypeName;
};


//��̬��--��Զ����ǰ��
class CARTOONOBJECT_EXPORT CCOM_PoseForward : public CCOM_MovePose
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
	Ŀ�ģ��õ�ĳʱ�̵���̬(��������ϵ��)
	������time		(in)		ʱ��
	      timeLen	(in)		�˶�Ƭ��ʱ
		  path		(in)		·������
		  speed		(in)		�ٶȶ���
		  pose		(out)		������ֵ̬
	���أ��ɹ�����true��pose��ֵ��Ч��������Ч
	*/
	virtual bool GetPose(double time, double timeLen, CCOM_MovePath &path, CCOM_MoveSpeed &speed, 
						osg::Vec3d& pose);

	//��ȡ��̬��������
	CString GetTypeName();


	virtual bool ParserXml(MSXML2::IXMLDOMNodePtr pNode);

private:
	static CString	m_sTypeName;
};


//��̬��--�ȱ仯(���߶���)
class CARTOONOBJECT_EXPORT CCOM_PoseAngleSegment : public CCOM_MovePose
{
public:
	/*
	Ŀ�ģ����캯��
	������originPose	(in)		��ʼ��̬
	      endPose		(in)		��ֹ��̬
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
	Ŀ�ģ��õ�ĳʱ�̵���̬(��������ϵ��)
	������time		(in)		ʱ��
	      timeLen	(in)		�˶�Ƭ��ʱ
	      path		(in)		·������
	      speed		(in)		�ٶȶ���
	      pose		(out)		������ֵ̬
	���أ��ɹ�����true��pose��ֵ��Ч��������Ч
	*/
	virtual bool GetPose(double time, double timeLen, CCOM_MovePath &path, CCOM_MoveSpeed &speed, 
						osg::Vec3d& pose);

	/*
	Ŀ�ģ����úͻ�ȡ��ʼ��̬����ֹ��̬
	*/
	BOOL SetOriginPoseAndEndPose(osg::Vec3d &originPose, osg::Vec3d &endPose);
	BOOL GetOriginPoseAndEndPose(osg::Vec3d &originPose, osg::Vec3d &endPose);

	//��ȡ��̬��������
	CString GetTypeName();


	virtual bool ParserXml(MSXML2::IXMLDOMNodePtr pNode);

	/*
	Purpose:	Return XML	Node
	Author:		Liugh
	*/
	virtual MSXML2::IXMLDOMNodePtr GetXmlNode();

	CCOM_PoseAngleSegment()
	{
		m_Type = ENM_MovePoseType_AngleSegment;
	}


private:
	osg::Vec3d		m_originPose;
	osg::Vec3d		m_endPose;
	static CString	m_sTypeName;
};


//��̬��--�ӵ�����ĳ����
class CARTOONOBJECT_EXPORT CCOM_PoseEyeLockPoint : public CCOM_MovePose
{
public:
	/*
	Ŀ�ģ����캯��
	������destPoint		(in)		�۲�Ŀ���
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
	Ŀ�ģ��õ�ĳʱ�̵���̬(��������ϵ��)
	������time		(in)		ʱ��
	      timeLen	(in)		�˶�Ƭ��ʱ
	      path		(in)		·������
	      speed		(in)		�ٶȶ���
	      pose		(out)		������ֵ̬
	���أ��ɹ�����true��pose��ֵ��Ч��������Ч
	*/
	virtual bool GetPose(double time, double timeLen, CCOM_MovePath &path, CCOM_MoveSpeed &speed, 
						osg::Vec3d& pose);

	/*
	Ŀ�ģ����úͻ�ȡĿ���
	*/
	BOOL SetDestPoint(const osg::Vec3d &destPoint);
	BOOL GetDestPoint(osg::Vec3d &destPoint);

	//��ȡ��̬��������
	CString GetTypeName();

	virtual bool ParserXml(MSXML2::IXMLDOMNodePtr pNode);


	CCOM_PoseEyeLockPoint()
	{
		m_Type = ENM_MovePoseType_EyeLockPoint;
	}

private:
	osg::Vec3d	m_destPoint;
	static CString	m_sTypeName;
};

//��̬��--�ӵ�����ĳ����
class CARTOONOBJECT_EXPORT CCOM_PoseEyeLockSegment : public CCOM_MovePose
{
public:
	/*
	Ŀ�ģ����캯��
	������startPoint		(in)		�۲�Ŀ���߶����
	      endPoint			(in)		�۲�Ŀ���߶��յ�
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
	Ŀ�ģ��õ�ĳʱ�̵���̬(��������ϵ��)
	������time		(in)		ʱ��
	      timeLen	(in)		�˶�Ƭ��ʱ
	      path		(in)		·������
	      speed		(in)		�ٶȶ���
	      pose		(out)		������ֵ̬
	���أ��ɹ�����true��pose��ֵ��Ч��������Ч
	*/
	virtual bool GetPose(double time, double timeLen, CCOM_MovePath &path, CCOM_MoveSpeed &speed, 
		osg::Vec3d& pose);

	/*
	Ŀ�ģ����úͻ�ȡĿ���߶�
	*/
	BOOL SetDestLineSegment(const osg::Vec3d &startPoint, const osg::Vec3d &endPoint);
	BOOL GetDestLineSegment(osg::Vec3d &startPoint, osg::Vec3d &endPoint);

	//��ȡ��̬��������
	CString GetTypeName();


	virtual bool ParserXml(MSXML2::IXMLDOMNodePtr pNode);

	CCOM_PoseEyeLockSegment()
	{
		m_Type = ENM_MovePoseType_EyeLockSegment;
	}

private:
	osg::Vec3d m_startPoint;
	osg::Vec3d m_endPoint;
	static CString	m_sTypeName;
};

}