/*******************************************************************************
Copyright (C),  
�ļ���COM_ItemSpeaker.h  COM_ItemSpeaker.cpp
���ߣ���־��
�汾��1.0
���ڣ�2009-08-12
�������˶���--�����������ڷ���������

�޸ļ�¼��
����						����						����
---------------------------------------------------------------------------------

********************************************************************************/

#pragma once 

#include <temp/Export.h>
#include <temp/Cartoon/COM_CartoonItem.h>
#include <Com/soundboot/SoundBoot.h>


namespace TEMP		{

class TEMP_EXPORT CCOM_ItemSpeaker : public CCOM_CartoonItem
{
public:
	CCOM_ItemSpeaker() : m_bBindCamera(FALSE), m_volum(0.5f), m_bPlay(false)
	{
		m_Name = _T("δ����������");
	}

	virtual ~CCOM_ItemSpeaker();

	CCOM_ItemSpeaker(const CCOM_ItemSpeaker &speaker);

	CCOM_CartoonItem * Clone();

	/*
	Ŀ�ģ�������Ԫִ����Ϊ
	������Time		(in)	����ڿ�ʼ�ʱ�̵�ʱ��
		  pos		(in)	������λ��ʱ��λ��			
	���أ�void
	*/
	virtual void DoAction(double Time, osg::Vec3d pos);

	//ִ�зǻʱ�����Ķ���
	virtual void DoNoneActive();

	/*
	Ŀ�ģ����úͻ�ȡ��������Ϣ
	*/
	BOOL GetInformation(CString &soundFile, BOOL &bBindCamera, float &volum)
	{
		soundFile = m_soundFile;
		bBindCamera = m_bBindCamera;
		volum = m_volum;
		return TRUE;
	}

	BOOL SetInformation(const CString &soundFile, BOOL bBindCamera, float volum)
	{
		m_soundFile = soundFile;
		m_bBindCamera = bBindCamera;
		m_volum = volum;
		return TRUE;
	}

	/*
	Ŀ�ģ�����
	*/
	virtual void Play();

	/*
	Ŀ�ģ���ͣ
	*/
	virtual void Pause();	

	/*
	Ŀ��: �������
	*/
	virtual void Forward(int step);

	/*
	Ŀ��: ����
	*/
	virtual void Rewind(int step);

	/*
	Ŀ��: ���ò���ʱ��,��Ҫʵ����ת����
	*/
	virtual void SetPlayTime(double Time);


	/*	
	Ŀ�ģ����л�����
	*/
	virtual void Serialize(CArchive& ar);

	/*
	Purpose:	Parser XML	Data Format for Animation
	Author:		Liugh
	*/
	virtual bool ParserXml(MSXML2::IXMLDOMNodePtr pNode);

protected:
	DECLARE_SERIAL(CCOM_ItemSpeaker)

protected:
	CString			m_soundFile;	//��Ƶ�ļ�������
	BOOL			m_bBindCamera;	//���������ʶ
	float			m_volum;		//����(0~1.f)
	BOOL			m_bPlay;		//�Ƿ��ѿ�ʼ���ű�ʶ
	osg::ref_ptr<COM::CSoundBoot> m_soundBoot;	//��Ƶ���Ŷ���
};

}