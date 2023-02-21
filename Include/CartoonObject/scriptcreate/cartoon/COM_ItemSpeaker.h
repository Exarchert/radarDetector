/********************************************************************************
Copyright (C),  
文件：CCOM_ItemSpeaker.h  CCOM_ItemSpeaker.cpp

刘广慧				2011-1-18			生成xml格式脚本
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
		m_Name = _T("未命名扬声器");
	}

	virtual ~CCOM_ItemSpeaker();

	CCOM_ItemSpeaker(const CCOM_ItemSpeaker &speaker);

	CCOM_CartoonItem * Clone();

	/*
	目的：设置和获取扬声器信息
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
	CString			m_soundFile;	//音频文件的名称
	BOOL			m_bBindCamera;	//伴随相机标识
	float			m_volum;		//音量(0~1.f)
	BOOL			m_bPlay;		//是否已开始播放标识
};

}