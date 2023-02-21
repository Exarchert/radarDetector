/*************************************************************************
File:			MovePose.h
Author:			��ƽ    
Date:			2009-03-16
Description:    �˶��������е���̬�࣬��Ҫ�������˶��������̬. ��̬�ı�ʾΪ��
		����нǡ������ǡ���ת��.

ChangeLog:
����					����					����
--------------------------------------------------------------------------
��־��				2009-07-21			�޸�CMovePose���MFC CObject�̳У�
										ʹ֮����Serialize����.
��־��				2009-09-17			�����̬��--���߶����ȱ仯
��־��				2009-09-17			�����̬��--�ӵ�����ĳ����
��־��				2009-10-06			�����̬��--�ӵ�����ĳ���߶�
��־��				2009-12-09			�����̬��--ʹ����Ԫ����ֵ����

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

//��̬����
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
	Ŀ�ģ��õ�ĳʱ�̵���̬(��������ϵ��)
	������time		(in)		ʱ��
	      timeLen	(in)		�˶�Ƭ�ܺ�ʱ
		  path		(in)		·������
		  speed		(in)		�ٶȶ���
		  pose		(out)		������ֵ̬
	���أ��ɹ�����true��pose��ֵ��Ч��������Ч
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

	//��ȡ��̬��������
	virtual CString GetTypeName();

	CMovePose(void)
	{
		m_Type = ENM_MovePoseType_UnDefault;
		m_blocal = FALSE;
	}

protected:
	DWORD m_Type;				//��̬�任����
	static CString m_sTypeName;

	//�ֲ�����ϵ����(v3.0)
	BOOL			m_blocal;	//��־λ
};


//��̬--���ȸı���̬��(��������)
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
	Ŀ�ģ����캯��
	������originPose	(in)	��ʼ��̬
		  startPose		(in)	��ʼ��̬	---\
		  endPose		(in)	������̬	----+���ڼ�����̬�仯�ٶ�
		  timeLen		(in)	��ʱ		---/
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
	Ŀ�ģ��õ�ĳʱ�̵���̬(��������ϵ��)
	������time		(in)		ʱ��
	      timeLen	(in)		�˶�Ƭ�ܺ�ʱ
		  path		(in)		·������
		  speed		(in)		�ٶȶ���
		  pose		(out)		������ֵ̬
	���أ��ɹ�����true��pose��ֵ��Ч��������Ч
	*/
	virtual bool GetPose(double time, double timeLen, CMovePath &path, CMoveSpeed &speed, 
						osg::Vec3d& pose);

	/*
	Ŀ�ģ����úͻ�ȡ��ʼ��̬����̬�仯�ٶ�
	*/
	BOOL SetOriginPoseAndChangeSpeed(osg::Vec3d &originPose, osg::Vec3d &angleSpeed);
	BOOL GetOriginPoseAndChangeSpeed(osg::Vec3d &originPose, osg::Vec3d &angleSpeed);

	//��ȡ��̬��������
	CString GetTypeName();

	//XML��ȡ�ͱ���ģ��
	bool Init(MSXML2::IXMLDOMNodePtr pNode);
	MSXML2::IXMLDOMNodePtr GetXMLNode();

	CMPSChangeAngle()
	{
		m_Type = ENM_MovePoseType_ChangeAngle;
	}

protected:
	osg::Vec3d m_originPose;	//��ʼ��̬
	osg::Vec3d m_angleSpeed;	//�仯�ٶ�
	static CString m_sTypeName;
};


//��̬��--��Զ����ǰ��
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
	Ŀ�ģ��õ�ĳʱ�̵���̬(��������ϵ��)
	������time		(in)		ʱ��
	      timeLen	(in)		�˶�Ƭ��ʱ
		  path		(in)		·������
		  speed		(in)		�ٶȶ���
		  pose		(out)		������ֵ̬
	���أ��ɹ�����true��pose��ֵ��Ч��������Ч
	*/
	virtual bool GetPose(double time, double timeLen, CMovePath &path, CMoveSpeed &speed, 
						osg::Vec3d& pose);

	//��ȡ��̬��������
	CString GetTypeName();

	//XML��ȡ�ͱ���ģ��
	bool Init(MSXML2::IXMLDOMNodePtr pNode);
	MSXML2::IXMLDOMNodePtr GetXMLNode();

private:
	static CString	m_sTypeName;
};


//��̬��--�ȱ仯(���߶���)
class COM_EXPORT CPoseAngleSegment : public CMovePose
{
public:
	/*
	Ŀ�ģ����캯��
	������originPose	(in)		��ʼ��̬
	      endPose		(in)		��ֹ��̬
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
	Ŀ�ģ��õ�ĳʱ�̵���̬(��������ϵ��)
	������time		(in)		ʱ��
	      timeLen	(in)		�˶�Ƭ��ʱ
	      path		(in)		·������
	      speed		(in)		�ٶȶ���
	      pose		(out)		������ֵ̬
	���أ��ɹ�����true��pose��ֵ��Ч��������Ч
	*/
	virtual bool GetPose(double time, double timeLen, CMovePath &path, CMoveSpeed &speed, 
						osg::Vec3d& pose);

	/*
	Ŀ�ģ����úͻ�ȡ��ʼ��̬����ֹ��̬
	*/
	BOOL SetOriginPoseAndEndPose(osg::Vec3d &originPose, osg::Vec3d &endPose);
	BOOL GetOriginPoseAndEndPose(osg::Vec3d &originPose, osg::Vec3d &endPose);

	//��ȡ��̬��������
	CString GetTypeName();

	//XML��ȡ�ͱ���ģ��
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


//��̬��--�ӵ�����ĳ����
class COM_EXPORT CPoseEyeLockPoint : public CMovePose
{
public:
	/*
	Ŀ�ģ����캯��
	������destPoint		(in)		�۲�Ŀ���
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
	Ŀ�ģ��õ�ĳʱ�̵���̬(��������ϵ��)
	������time		(in)		ʱ��
	      timeLen	(in)		�˶�Ƭ��ʱ
	      path		(in)		·������
	      speed		(in)		�ٶȶ���
	      pose		(out)		������ֵ̬
	���أ��ɹ�����true��pose��ֵ��Ч��������Ч
	*/
	virtual bool GetPose(double time, double timeLen, CMovePath &path, CMoveSpeed &speed, 
						osg::Vec3d& pose);

	/*
	Ŀ�ģ����úͻ�ȡĿ���
	*/
	BOOL SetDestPoint(const osg::Vec3d &destPoint);
	BOOL GetDestPoint(osg::Vec3d &destPoint);

	//��ȡ��̬��������
	CString GetTypeName();

	CPoseEyeLockPoint()
	{
		m_Type = ENM_MovePoseType_EyeLockPoint;
	}

	//XML��ȡ�ͱ���ģ��
	bool Init(MSXML2::IXMLDOMNodePtr pNode);
	MSXML2::IXMLDOMNodePtr GetXMLNode();
protected:
	osg::Vec3d	m_destPoint;
	static CString	m_sTypeName;
};

//��̬��--�ӵ�����ĳ���߶�
class COM_EXPORT CPoseEyeLockSegment : public CMovePose
{
public:
	/*
	Ŀ�ģ����캯��
	������startPoint		(in)		�۲�Ŀ���߶����
	      endPoint			(in)		�۲�Ŀ���߶��յ�
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
	Ŀ�ģ��õ�ĳʱ�̵���̬(��������ϵ��)
	������time		(in)		ʱ��
	      timeLen	(in)		�˶�Ƭ��ʱ
	      path		(in)		·������
	      speed		(in)		�ٶȶ���
	      pose		(out)		������ֵ̬
	���أ��ɹ�����true��pose��ֵ��Ч��������Ч
	*/
	virtual bool GetPose(double time, double timeLen, CMovePath &path, CMoveSpeed &speed, 
		osg::Vec3d& pose);

	/*
	Ŀ�ģ����úͻ�ȡĿ���߶�
	*/
	BOOL SetDestLineSegment(const osg::Vec3d &startPoint, const osg::Vec3d &endPoint);
	BOOL GetDestLineSegment(osg::Vec3d &startPoint, osg::Vec3d &endPoint);

	//��ȡ��̬��������
	CString GetTypeName();

	//XML��ȡ�ͱ���ģ��
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

//��̬��--ʹ����Ԫ����ֵ����
class COM_EXPORT CPoseQuatSlerp : public CMovePose
{
public:
	CPoseQuatSlerp();
	CPoseQuatSlerp(const CPoseQuatSlerp &pose);
	CPoseQuatSlerp(osg::Vec3d originPose, osg::Vec3d endPose);
	virtual ~CPoseQuatSlerp();

	CMovePose *Clone();

	/*
	Ŀ�ģ��õ�ĳʱ�̵���̬(��������ϵ��)
	������time		(in)		ʱ��
	      timeLen	(in)		�˶�Ƭ��ʱ
	      path		(in)		·������
	      speed		(in)		�ٶȶ���
	      pose		(out)		������ֵ̬
	���أ��ɹ�����true��pose��ֵ��Ч��������Ч
	*/
	virtual bool GetPose(double time, double timeLen, CMovePath &path, CMoveSpeed &speed, 
		osg::Vec3d& pose);

	//��ȡ��̬��������
	CString GetTypeName();

	//XML��ȡ�ͱ���ģ��
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