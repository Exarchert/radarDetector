/***************************************************************************
File:			COM_CartoonItem.h
Author:			��ƽ    
Date:			2009-03-31
Description:    ����������Ԫ��

ChangeLog:
����					����					����
----------------------------------------------------------------------------
��־��				2009-07-21			�޸�CCOM_CartoonItem�ٴ�MFC CObject�̳�,
										ʹ֮����Serialize����.
��־��				2009-12-29			���DoNoneActive()

***************************************************************************/

#pragma once

#include <temp/Export.h>
#include <Osg/Referenced>
#include <Osg/Vec3>
#include <Osg/Vec2>
#include <temp/Movement/COM_MoveTracks.h>


namespace TEMP		{

//����������Ԫ��
class TEMP_EXPORT CCOM_CartoonItem : public osg::Referenced, public CObject
{
public:
	CCOM_CartoonItem(void);
	virtual ~CCOM_CartoonItem(void);

	virtual CCOM_CartoonItem * Clone();

	/*
	Ŀ�ģ��������캯��
	*/
	CCOM_CartoonItem(const CCOM_CartoonItem &item);


	enum{
		ENM_PlayState_UnDefault = 0,
		ENM_PlayState_Play,
		ENM_PlayState_Pause,
		ENM_PlayState_Stop,
		ENM_PlayState_Finish,
		ENM_PlayState_QuickSpeedPlay,
		ENM_PlayState_SlowSpeedPlay,
	};

	/*
	Ŀ�ģ���ʼ����������
	������Tracks			(in)	�˶�Ƭ����
		  StartTime		(in)	��������Ŀ�ʼ�ʱ��
		  TimeLenght	(in)	��������Ļʱ�䳤��
		  name			(in)	������������
	���أ�void
	*/
	void Init(CCOM_MoveTracks *Tracks, CString &name, double StartTime = 0, double TimeLength = 0);

	/*
	Ŀ�ģ�����
	*/
	virtual void Play();

	/*
	Ŀ�ģ���ͣ
	*/
	virtual void Pause();

	/*
	Ŀ�ģ����²���
	*/
	virtual void Replay();

	/*
	Ŀ�ģ�����������ʱ��, ��ʱ���������һ��ʱ����
	������Time		(in)	������������ʱ��
	*/
	virtual void SetPlayTime(double Time);

	/*
	Ŀ�ģ���ȡ������ʱ��
	*/
	double GetPlayTime(){ return m_PlayTime; }

	/*
	Ŀ�ģ����ò����ٶ�
	������Speed		(in)	���ٱ���
	*/
	virtual void SetSpeed(double Speed);

	/*
	Purpose:	Get Play Speed Of Animation
	*/
	virtual double GetSpeed(){return m_PlayScale;}

	/*
	Ŀ�ģ�ֹͣ����
	*/
	virtual void Stop();

	/*
	Ŀ�ģ��������(flash/avi/gif��ӰƬ)
	*/
	virtual void Forward(int step);

	/*
	Ŀ�ģ���������(flash/avi/gif��ӰƬ)
	*/
	virtual void Rewind(int step);

	/*
	Ŀ�ģ�����Ŀǰ������Ԫ�Ĳ���״̬
	���أ�����״ֵ̬
	*/
	DWORD GetPlayState(){ return m_PlayState; }

	/*
	Ŀ�ģ���ȡ������Ԫ�Ļʱ�䳤��
	*/
	double GetTimeLength(){ return m_TimeLength; }

	void SetTimeLength(double timeLength) 
	{ 
		m_TimeLength = timeLength; 
	}

	/*
	Ŀ�ģ���ȡ������Ԫ���˶�Ƭ����
	���أ�NULL��ʾʧ�ܣ�����ɹ�
	*/
	CCOM_MoveTracks * GetTracks(){ return m_Tracks.get(); }

	/*
	Ŀ�ģ�������Ԫִ����Ϊ
	������Time		(in)	����ڿ�ʼ�ʱ�̵�ʱ��
		  pos		(in)	������λ��ʱ��λ��			
	���أ�void
	*/
	virtual void DoAction(double Time, osg::Vec3d pos);
	
	virtual void PlayFinish(double Time, osg::Vec3d pos);

	//ִ�зǻʱ�����Ķ���
	virtual void DoNoneActive() {}

	/*
	Ŀ�ģ���������ʱ��
	������DeTime		(in)	ʱ��ı仯��
	*/
	virtual void Update(double DeTime);

	/*
	Ŀ�ģ���ȡ����
	*/
	CString GetName()
	{
		return m_Name;
	}

	void SetName(CString &name)
	{
		m_Name = name;
	}

	void SetGroupName(const CString& groupName)
	{
		m_GroupName = groupName;
	}

	CString GetGroupName()
	{
		return m_GroupName;
	}


	/*
	Ŀ�ģ����ÿ�ʼ�ʱ��
	*/
	void SetStartTime(double startTime)
	{
		m_StartTime = startTime;
	}

	double GetStartTime()
	{
		return m_StartTime;
	}

	void SetCurScriptPath(CString scriptPath){m_ScriptPath = scriptPath;}
	CString GetScriptPath(){return m_ScriptPath;}

	/*
	Ŀ�ģ����л�����
	*/
	virtual void Serialize(CArchive& ar);

	virtual bool ParserXml(MSXML2::IXMLDOMNodePtr pNode);

protected:
	DECLARE_SERIAL(CCOM_CartoonItem)

protected:
	osg::ref_ptr<CCOM_MoveTracks> m_Tracks;		//�˶�Ƭ����
	double m_PlayTime;							//������ʱ��
	double m_StartTime;							//��ʼ�ʱ��, ��m_PlayTimeʹ��ͬһ��ʱ����
	double m_PlayScale;							//�����ٶ�(����)
	DWORD m_PlayState;							//������Ԫ��״̬
	double m_TimeLength;						//�ʱ�䳤��
	CString m_Name;								//��ͨ����
	CString m_GroupName;
	bool	m_hasPlayed;						//�Ѿ����ʶ
	CString m_ScriptPath;
};

}