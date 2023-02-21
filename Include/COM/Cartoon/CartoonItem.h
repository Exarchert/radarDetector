/***************************************************************************
File:			COM_CartoonItem.h
Author:			��ƽ    
Date:			2009-03-31
Description:    ����������Ԫ��

ChangeLog:
����					����					����
----------------------------------------------------------------------------
��־��				2009-07-21			�޸�CCartoonItem�ٴ�MFC CObject�̳�,
										ʹ֮����Serialize����.

***************************************************************************/

#pragma once
#include <COM\Export.h>
#include <Osg/Referenced>
#include <Osg/Vec3>
#include <Osg/Vec2>
#include <COM\Movement\MoveTracks.h>
#include <COM\XML\XMLNode.h>

namespace COM{

//����������Ԫ��
class COM_EXPORT CCartoonItem : public osg::Referenced
{
public:
	CCartoonItem(void);
	virtual ~CCartoonItem(void);

	virtual CCartoonItem * Clone();

	/*
	Ŀ�ģ��������캯��
	*/
	CCartoonItem(const CCartoonItem &item);


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
	void Init(CMoveTracks *Tracks, CString &name, double StartTime = 0, double TimeLength = 0);

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
	Ŀ�ģ����ò����ٶ�
	������Speed		(in)	���ٱ���
	*/
	virtual void SetSpeed(double Speed);

	/*
	Ŀ�ģ�ֹͣ����
	*/
	virtual void Stop();

	/*
	Ŀ�ģ�����Ŀǰ������Ԫ�Ĳ���״̬
	���أ�����״ֵ̬
	*/
	DWORD GetPlayState(){ return m_PlayState; }

	/*
	Ŀ�ģ���ȡ������ʱ��
	*/
	double GetPlayTime(){ return m_PlayTime; }

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
	CMoveTracks * GetTracks(){ return m_Tracks.get(); }

	/*
	Ŀ�ģ�������Ԫִ����Ϊ
	������Time		(in)	����ڿ�ʼ�ʱ�̵�ʱ��
		  pos		(in)	������λ��ʱ��λ��			
	���أ�void
	*/
	virtual void DoAction(double Time, osg::Vec3d pos);
	
	virtual void PlayFinish(double Time, osg::Vec3d pos);
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

	//XML��ȡ�ͱ���ģ��
	bool InitTracks(MSXML2::IXMLDOMNodePtr pNode);
	MSXML2::IXMLDOMNodePtr GetXMLNodeTracks();
protected:
	osg::ref_ptr<CMoveTracks> m_Tracks;		//�˶�Ƭ����
	double m_PlayTime;							//������ʱ��
	double m_StartTime;							//��ʼ�ʱ��, ��m_PlayTimeʹ��ͬһ��ʱ����
	double m_PlayScale;							//�����ٶ�(����)
	DWORD m_PlayState;							//������Ԫ��״̬
	double m_TimeLength;						//�ʱ�䳤��
	CString m_Name;								//��ͨ����
};
}