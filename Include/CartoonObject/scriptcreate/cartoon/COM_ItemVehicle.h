/********************************************************************************
Copyright (C),  
文件：COM_ItemVehicle.h  COM_ItemVehicle.cpp

刘广慧				2011-1-17			生成xml格式脚本
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
		m_Name = _T("未命名模型");
		m_isCfg = false;
		m_actions = 0;
		m_curAction = -1;
	}

	virtual ~CCOM_ItemVehicle();

	CCOM_ItemVehicle(const CCOM_ItemVehicle &vehicle);

	CCOM_CartoonItem * Clone();

	/*
	目的：设置和获取模型文件名
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
	CString	m_modelName;	//模型文件的名称
	bool						m_isCfg;		//是否为骨骼动画
	unsigned int				m_actions;		//动作数量
	int							m_curAction;	//当前动作
};

}