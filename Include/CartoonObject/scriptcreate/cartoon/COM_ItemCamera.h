/********************************************************************************
Copyright (C),  
文件：COM_ItemCamera.h  COM_ItemCamera.cpp

刘广慧				2011-1-12			生成xml格式脚本
********************************************************************************/

#pragma once

#include <CartoonObject/Export.h>
#include <CartoonObject/ScriptCreate/Cartoon/COM_CartoonItem.h>


namespace CartoonObject		{

class CARTOONOBJECT_EXPORT CCOM_ItemCamera : public CCOM_CartoonItem
{
public:
	CCOM_ItemCamera()
	{
		m_Name = _T("未命名相机");
		m_bive = FALSE;
		m_bivePlayed = FALSE;
	}

	virtual ~CCOM_ItemCamera();

	CCOM_ItemCamera(const CCOM_ItemCamera &vehicle);

	CCOM_CartoonItem * Clone();

	/*
	目的：设置和获取信息
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
	BOOL m_bive;
	BOOL m_bivePlayed;
	BOOL m_bStart;
};

}