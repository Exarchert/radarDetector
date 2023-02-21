/********************************************************************************
Copyright (C),  
�ļ���CCOM_ItemCommand.h  CCOM_ItemCommand.cpp

�����				2011-1-18			����xml��ʽ�ű�
********************************************************************************/

#pragma once

#include <CartoonObject/Export.h>
#include <CartoonObject/ScriptCreate/Cartoon/COM_CartoonItem.h>


namespace CartoonObject		{

class CARTOONOBJECT_EXPORT CCOM_ItemCommand : public CCOM_CartoonItem
{
public:
	CCOM_ItemCommand()
	{
		m_Name = _T("δ��������");
		m_handled = false;
	}

	virtual ~CCOM_ItemCommand();

	CCOM_ItemCommand(const CCOM_ItemCommand &cmd);

	CCOM_CartoonItem * Clone();

	/*
	Ŀ�ģ����úͻ�ȡ�����ַ���
	*/
	void GetXmlCmd(CString &xmlCmd, CString &xmlCmdClear)
	{
		xmlCmd = m_xmlCmd;
		xmlCmdClear = m_xmlCmdClear;
	}

	void SetXmlCmd(const CString &xmlCmd, const CString &xmlCmdClear)
	{
		m_xmlCmd = xmlCmd;
		m_xmlCmdClear = xmlCmdClear;
	}

	void SetInfo(CString strInfo)
	{
		m_Description = strInfo;
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
	bool	m_handled;		//�Ѵ���

	CString m_xmlCmd;		//�����ַ���
	CString m_xmlCmdClear;	//�������
	CString m_Description;
};

}