/*************************************************************************
File:			COM_MoveTrack.h
Author:			��ƽ    
Date:			2009-03-16
Description:    �˶��������еĹ켣�࣬����һ���˶�.

ChangeLog:
����					����					����
--------------------------------------------------------------------------
��־��				2009-07-21			�޸�CCOM_MoveTrack���MFC CObject��
										�̳У�ʹ֮����Serialize����.

*************************************************************************/

#pragma once

#include <temp/Export.h>
#include <vector>
#include <osg/ref_ptr>
#include <temp/Movement/COM_MovePath.h>
#include <temp/Movement/COM_MovePose.h>
#include <temp/Movement/COM_MoveSpeed.h>


namespace TEMP		{

//�˶�����
class TEMP_EXPORT CCOM_MoveTrack : public osg::Referenced, public CObject
{
public:
	/*
	Ŀ�ģ����캯��
	������path		(in)	�˶�·������ָ��
		 speed		(in)	�˶��ٶȶ���ָ��
		 pose		(in)	�˶���̬����ָ��
		 timeLen	(in)	���˶�����ʱ�䳤��
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
	Ŀ�ģ���¡�Լ�
	*/
	CCOM_MoveTrack * Clone()
	{
		return new CCOM_MoveTrack(*this);
	}

	virtual ~CCOM_MoveTrack(void)
	{
	}

	/*
	Ŀ�ģ�����ĳʱ�̵�λ��
	������time		(in)	ʱ�̣���0���㣩
		  position	(out)	����	λ��
	���أ��ɹ�����true��position��Ч��������Ч
	*/
	bool GetPosition(double time, osg::Vec3d &position);
	bool GetStartPosition(osg::Vec3d &position);
	bool GetEndPosition(osg::Vec3d &position);

	/*
	Ŀ�ģ�����ĳʱ�̵���̬
	������time		(in)	ʱ�̣���0���㣩
		  pose		(out)	������̬
	���أ��ɹ�����true��pose��Ч��������Ч
	*/
	bool GetPose(double time, osg::Vec3d &pose);
	bool GetStartPose(osg::Vec3d &pose);
	bool GetEndPose(osg::Vec3d &pose);

	//����ĳʱ�̵�����
	bool GetSpeed(double time, double &speed);

	/*
	Ŀ�ģ���ȡ���˶����ĵ�ʱ��
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
	Ŀ�ģ����л�����
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
	osg::ref_ptr<CCOM_MovePath>		m_Path;			//�˶�·������
	osg::ref_ptr<CCOM_MoveSpeed>	m_Speed;		//�˶��ٶȶ���
	osg::ref_ptr<CCOM_MovePose>		m_Pose;			//�˶���̬����
	double							m_TimeLength;	//�˶���ʱ
	CString	m_strType;
};

}