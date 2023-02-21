/**************************************************************************
Copyright (C),  
文件：CCOM_ItemCommand.h CCOM_ItemCommand.cpp
作者：黄志鹏
版本：1.0
日期：2010-03-09
描述：卡通项-命令处理, 每个命令只被处理一次, 它的生命时长表示的意思是：如果
	  超过时长未处理，则不予处理。

修改记录：
作者						日期					描述
---------------------------------------------------------------------------

**************************************************************************/

#pragma once

#include <temp/Export.h>
#include <temp/Cartoon/COM_CartoonItem.h>


namespace TEMP		{

class TEMP_EXPORT CCOM_ItemCommand : public CCOM_CartoonItem
{
public:
	CCOM_ItemCommand()
	{
		m_Name = _T("未命名命令");
		m_handled = false;
	}

	virtual ~CCOM_ItemCommand();

	CCOM_ItemCommand(const CCOM_ItemCommand &cmd);

	CCOM_CartoonItem * Clone();

	/*
	目的：动画单元执行行为
	参数：Time		(in)	相对于开始活动时刻的时刻
		  pos		(in)	动画单位此时的位置			
	返回：void
	*/
	virtual void DoAction(double Time, osg::Vec3d pos);

	//执行非活动时间区的动作
	virtual void DoNoneActive();

	/*
	目的：设置和获取命令字符串
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

	/*
	目的：序列化操作
	*/
	virtual void Serialize(CArchive& ar);

	/*
	Purpose:	Parser XML	Data Format for Animation
	Author:		Liugh
	*/
	virtual bool ParserXml(MSXML2::IXMLDOMNodePtr pNode);

protected:
	DECLARE_SERIAL(CCOM_ItemCommand)

protected:
	CString m_xmlCmd;		//命令字符串
	CString m_xmlCmdClear;	//清除命令
	bool	m_handled;		//已处理
};

}