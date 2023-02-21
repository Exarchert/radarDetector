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

----------------------------------------------------------------------------
�����				2011-1-12			����xml��ʽ�ű�

***************************************************************************/
#pragma once

#include <CartoonObject/Export.h>
#include <Osg/Referenced>
#include <Osg/Vec3>
#include <Osg/Vec2>
#include <CartoonObject/ScriptCreate/Movement/COM_MoveTracks.h>


namespace CartoonObject		{

//����������Ԫ��
class CARTOONOBJECT_EXPORT CCOM_CartoonItem : public osg::Referenced, public CObject
{
protected:
	DECLARE_SERIAL(CCOM_CartoonItem)
	/*
	Ŀ�ģ����л�����
	*/
	virtual void Serialize(CArchive& ar);
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
	osg::ref_ptr<CCOM_MoveTracks> m_Tracks;		//�˶�Ƭ����
	double m_PlayTime;							//������ʱ��
	double m_StartTime;							//��ʼ�ʱ��, ��m_PlayTimeʹ��ͬһ��ʱ����
	double m_PlayScale;							//�����ٶ�(����)
	DWORD m_PlayState;							//������Ԫ��״̬
	double m_TimeLength;						//�ʱ�䳤��
	CString m_Name;								//��ͨ����
	CString m_GroupName;
	bool	m_hasPlayed;						//�Ѿ����ʶ
};

}