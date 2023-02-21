/********************************************************************************
Copyright (C),  
�ļ���COM_ItemVehicle.h  COM_ItemVehicle.cpp

�����				2011-1-17			����xml��ʽ�ű�
********************************************************************************/

#pragma once

#include <CartoonObject/Export.h>
#include <CartoonObject/ScriptCreate/Cartoon/COM_CartoonItem.h>


namespace CartoonObject		{

class CARTOONOBJECT_EXPORT CCOM_ItemVehicle : public CCOM_CartoonItem
{
public:
	CCOM_ItemVehicle()
	{
		m_Name = _T("δ����ģ��");
		m_isCfg = false;
		m_actions = 0;
		m_curAction = -1;
	}

	virtual ~CCOM_ItemVehicle();

	CCOM_ItemVehicle(const CCOM_ItemVehicle &vehicle);

	CCOM_CartoonItem * Clone();

	/*
	Ŀ�ģ����úͻ�ȡģ���ļ���
	*/
	CString GetModelFileName()
	{
		return m_modelName;
	}

	void SetModelFileName(CString &str)
	{
		m_modelName = str;
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
	DECLARE_SERIAL(CCOM_ItemVehicle)

protected:
	CString	m_modelName;	//ģ���ļ�������
	bool						m_isCfg;		//�Ƿ�Ϊ��������
	unsigned int				m_actions;		//��������
	int							m_curAction;	//��ǰ����
};

}