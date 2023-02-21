/*************************************************************************
File:			COM_MoveTracks.h
Author:			��ƽ    
Date:			2009-03-16
Description:    �˶��������е��˶������࣬����һ���������˶�.

ChangeLog:
����					����					����
--------------------------------------------------------------------------
��־��				2009-07-21			�޸�CCOM_MoveTracks���MFC CObject
										�̳У�ʹ֮����Serialize����.

*************************************************************************/

#pragma once

#include <temp/Export.h>
#include <temp/Movement/COM_MoveTrack.h>


namespace TEMP		{

//�˶�������
class TEMP_EXPORT CCOM_MoveTracks : public osg::Referenced, public CObject
{
public:
	CCOM_MoveTracks(void)
	{
		m_timeLength = 0.f;
	}

	virtual ~CCOM_MoveTracks(void)
	{
	}

	CCOM_MoveTracks(const CCOM_MoveTracks &tracks);

	//��¡����
	virtual CCOM_MoveTracks * Clone();

	/*
	Ŀ�ģ���ȡCCOM_MoveTrack����ĸ���
	*/
	int GetTrackCount()
	{
		return m_Tracks.size();
	}

	/*
	Ŀ�ģ���ȡָ����ŵ�CCOM_MoveTrack����
	������index		(in)	����ֵ
	���أ��ɹ�����CCOM_MoveTrack����ָ�룬���򷵻�NULL
	*/
	CCOM_MoveTrack *GetTrackByIndex(unsigned int index)
	{
		if (index >= 0 && index < m_Tracks.size())
		{
			return m_Tracks[index].get();
		}
		return NULL;
	}

	//��Ӻ�ɾ��ĳ��CCOM_MoveTrack����
	void AddTrack(CCOM_MoveTrack* Track);
	void DelTrack(CCOM_MoveTrack* Track);
	void Clear();

	/*
	Ŀ�ģ���ָ������ǰ����ĳ��CCOM_MoveTrack����
	������where		(in)	����λ��
		  track		(in)	����Ķ���
	���أ��ɹ�����TRUE,���򷵻�FALSE
	��ע�����whereΪNULL�����ڶ���������
	*/
	bool InsertTrack(CCOM_MoveTrack* where, CCOM_MoveTrack* track);

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

	bool GetSpeed(double time, double &speed);

	/*
	Ŀ�ģ����¼�������ʱ��
	*/
	void RecalTimeLength();

	/*
	Ŀ�ģ����л�����
	*/
	virtual void Serialize(CArchive& ar);

	bool ParserXml(MSXML2::IXMLDOMNodePtr pNode);

protected:
	/*
	Ŀ�ģ���ȡʱ��time�����ڵ�CCOM_MoveTrack����
	������Time		(in)		ʱ��
		  OutTime	(out)		���������Track�����ڵ�ʱ��
	���أ�NULL��ʾʧ�ܣ�OutTime��Ч������OutTime��Ч
	*/
	CCOM_MoveTrack* GetTrack(double Time, double& OutTime);

	DECLARE_SERIAL(CCOM_MoveTracks)

protected:
	double m_timeLength;	//����ʱ��
	std::vector<osg::ref_ptr<CCOM_MoveTrack>> m_Tracks;
};

typedef	std::vector<osg::ref_ptr<CCOM_MoveTrack>>::iterator	VecTrackItr;
typedef	std::vector<osg::ref_ptr<CCOM_MoveTrack>>::const_iterator VecTrackConstItr;

}