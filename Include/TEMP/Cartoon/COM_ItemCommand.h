/**************************************************************************
Copyright (C),  
�ļ���CCOM_ItemCommand.h CCOM_ItemCommand.cpp
���ߣ���־��
�汾��1.0
���ڣ�2010-03-09
��������ͨ��-�����, ÿ������ֻ������һ��, ��������ʱ����ʾ����˼�ǣ����
	  ����ʱ��δ�������账��

�޸ļ�¼��
����						����					����
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
		m_Name = _T("δ��������");
		m_handled = false;
	}

	virtual ~CCOM_ItemCommand();

	CCOM_ItemCommand(const CCOM_ItemCommand &cmd);

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
	DECLARE_SERIAL(CCOM_ItemCommand)

protected:
	CString m_xmlCmd;		//�����ַ���
	CString m_xmlCmdClear;	//�������
	bool	m_handled;		//�Ѵ���
};

}