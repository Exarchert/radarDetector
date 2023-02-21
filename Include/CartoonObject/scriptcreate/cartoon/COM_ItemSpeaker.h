/********************************************************************************
Copyright (C),  
�ļ���CCOM_ItemSpeaker.h  CCOM_ItemSpeaker.cpp

�����				2011-1-18			����xml��ʽ�ű�
********************************************************************************/

#pragma once 

#include <temp/Export.h>
#include <CartoonObject/ScriptCreate/Cartoon/COM_CartoonItem.h>


namespace CartoonObject		{

class CARTOONOBJECT_EXPORT CCOM_ItemSpeaker : public CCOM_CartoonItem
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
	CString			m_soundFile;	//��Ƶ�ļ�������
	BOOL			m_bBindCamera;	//���������ʶ
	float			m_volum;		//����(0~1.f)
	BOOL			m_bPlay;		//�Ƿ��ѿ�ʼ���ű�ʶ
};

}