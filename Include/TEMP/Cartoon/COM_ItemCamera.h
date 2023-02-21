/********************************************************************************
Copyright (C),  
�ļ���COM_ItemCamera.h  COM_ItemCamera.cpp
���ߣ���־��
�汾��1.0
���ڣ�2009-08-11
��������ͨ����--���

�޸ļ�¼
����						����							����
---------------------------------------------------------------------------------

********************************************************************************/

#pragma once

#include <temp/Export.h>
#include <temp/Cartoon/COM_CartoonItem.h>


namespace TEMP		{

class TEMP_EXPORT CCOM_ItemCamera : public CCOM_CartoonItem
{
public:
	CCOM_ItemCamera()
	{
		m_Name = _T("δ�������");
		m_bive = FALSE;
		m_bivePlayed = FALSE;
	}

	virtual ~CCOM_ItemCamera();

	CCOM_ItemCamera(const CCOM_ItemCamera &vehicle);

	CCOM_CartoonItem * Clone();

	/*
	Ŀ�ģ�����
	*/
	virtual void Play();

	/*
	Ŀ�ģ���ͣ
	*/
	virtual void Pause();

	/*
	Ŀ�ģ�ֹͣ����
	*/
	virtual void Stop();

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
	Ŀ�ģ����úͻ�ȡ��Ϣ
	*/
	void SetInformation(BOOL bive) 
	{
		m_bive = bive;
	}

	BOOL GetInformation()
	{
		return m_bive;
	}

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
	DECLARE_SERIAL(CCOM_ItemCamera)

protected:
	BOOL m_bive;
	BOOL m_bivePlayed;
	BOOL m_bStart;
};

}