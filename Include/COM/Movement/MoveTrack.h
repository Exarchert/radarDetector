/*************************************************************************
File:			MoveTrack.h
Author:			��ƽ    
Date:			2009-03-16
Description:    �˶��������еĹ켣�࣬����һ���˶�.

ChangeLog:
����					����					����
--------------------------------------------------------------------------


*************************************************************************/

#pragma once

#include <COM\Movement\MovePath.h>
#include <COM\Movement\MoveSpeed.h>
#include <COM\Movement\MovePose.h>
#include <vector>
#include <osg/ref_ptr>
#include <COM\XML\XMLNode.h>
#include <COM\XML\XMLApi.h>

namespace COM{

//�˶�����
class COM_EXPORT CMoveTrack : public osg::Referenced, public CXMLNode
{
public:
	/*
	Ŀ�ģ����캯��
	������path		(in)	�˶�·������ָ��
		 speed		(in)	�˶��ٶȶ���ָ��
		 pose		(in)	�˶���̬����ָ��
		 timeLen	(in)	���˶�����ʱ�䳤��
	*/
	CMoveTrack(CMovePath *path, CMoveSpeed *speed, CMovePose *pose, double timeLen)
	{
		m_Path = path;
		m_Speed = speed;
		m_Pose = pose;
		m_TimeLength = timeLen;
	}

	CMoveTrack(const CMoveTrack &track);

	/*
	Ŀ�ģ���¡�Լ�
	*/
	CMoveTrack * Clone()
	{
		return new CMoveTrack(*this);
	}

	virtual ~CMoveTrack(void)
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

	CMovePath *GetPath()
	{
		return m_Path.get();
	}

	void SetPath(CMovePath *path)
	{
		m_Path = path;
	}

	CMoveSpeed *GetSpeed()
	{
		return m_Speed.get();
	}
	
	void SetSpeed(CMoveSpeed *speed)
	{
		m_Speed = speed;
	}

	CMovePose *GetPose()
	{
		return m_Pose.get();
	}
	
	void SetPose(CMovePose *pose)
	{
		m_Pose = pose;
	}

	//XML��ȡ�ͱ���ģ��
	bool Init(MSXML2::IXMLDOMNodePtr pNode);
	MSXML2::IXMLDOMNodePtr GetXMLNode();

	CMoveTrack(void)
	{
		m_TimeLength = 0.f;
	}

protected:
	osg::ref_ptr<CMovePath>		m_Path;			//�˶�·������
	osg::ref_ptr<CMoveSpeed>	m_Speed;		//�˶��ٶȶ���
	osg::ref_ptr<CMovePose>		m_Pose;			//�˶���̬����
	double							m_TimeLength;	//�˶���ʱ
};

}