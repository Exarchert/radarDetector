/*************************************************************************
File:			MoveTracks.h
Author:			��ƽ    
Date:			2009-03-16
Description:    �˶��������е��˶������࣬����һ���������˶�.

ChangeLog:
����					����					����
--------------------------------------------------------------------------

*************************************************************************/

#pragma once
#include <COM\Export.h>
#include <COM\Movement\MoveTrack.h>

#include <COM\XML\XMLNode.h>
#include <COM\XML\XMLApi.h>

namespace COM{

//�˶�������
class COM_EXPORT CMoveTracks : public osg::Referenced, public CXMLNode
{
public:
	CMoveTracks(void)
	{
		m_timeLength = 0.f;
	}

	virtual ~CMoveTracks(void)
	{
	}

	CMoveTracks(const CMoveTracks &tracks);

	//��¡����
	virtual CMoveTracks * Clone();

	/*
	Ŀ�ģ���ȡCMoveTrack����ĸ���
	*/
	int GetTrackCount()
	{
		return m_Tracks.size();
	}

	/*
	Ŀ�ģ���ȡָ����ŵ�CMoveTrack����
	������index		(in)	����ֵ
	���أ��ɹ�����CMoveTrack����ָ�룬���򷵻�NULL
	*/
	CMoveTrack *GetTrackByIndex(unsigned int index)
	{
		if (index >= 0 && index < m_Tracks.size())
		{
			return m_Tracks[index].get();
		}
		return NULL;
	}

	//��Ӻ�ɾ��ĳ��CMoveTrack����
	void AddTrack(CMoveTrack* Track);
	void DelTrack(CMoveTrack* Track);
	void Clear();

	/*
	Ŀ�ģ���ָ������ǰ����ĳ��CMoveTrack����
	������where		(in)	����λ��
		  track		(in)	����Ķ���
	���أ��ɹ�����TRUE,���򷵻�FALSE
	��ע�����whereΪNULL�����ڶ���������
	*/
	bool InsertTrack(CMoveTrack* where, CMoveTrack* track);

	/*
	Ŀ�ģ���ȡ���˶����еĺ�ʱ
	*/
	double GetTimeLength()
	{
		return m_timeLength;
	}

	bool GetPosition(double time, osg::Vec3d &position);
	bool GetStartPosition(osg::Vec3d &position);
	bool GetEndPosition(osg::Vec3d &position);

	bool GetPose(double time, osg::Vec3d& pose);
	bool GetStartPose(osg::Vec3d& pose);
	bool GetEndPose(osg::Vec3d& pose);

	/*
	Ŀ�ģ����¼�������ʱ��
	*/
	void RecalTimeLength();

	//XML��ȡ�ͱ���ģ��
	bool Init(MSXML2::IXMLDOMNodePtr pNode);
	MSXML2::IXMLDOMNodePtr GetXMLNode();

protected:
	/*
	Ŀ�ģ���ȡʱ��time�����ڵ�CMoveTrack����
	������Time		(in)		ʱ��
		  OutTime	(out)		���������Track�����ڵ�ʱ��
	���أ�NULL��ʾʧ�ܣ�OutTime��Ч������OutTime��Ч
	*/
	CMoveTrack* GetTrack(double Time, double& OutTime);

protected:
	double m_timeLength;	//����ʱ��
	std::vector<osg::ref_ptr<CMoveTrack>> m_Tracks;
};

typedef	std::vector<osg::ref_ptr<CMoveTrack>>::iterator	VecTrackItr;
typedef	std::vector<osg::ref_ptr<CMoveTrack>>::const_iterator VecTrackConstItr;

}