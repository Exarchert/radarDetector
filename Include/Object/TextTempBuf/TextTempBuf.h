/************************************************************************
Copyright (C),   
文件：TextTempBuf.h TextTempBuf.cpp
作者：黄志鹏
版本：1.0
日期：2010-03-25
描述：文字模板名字与模板内容的映射管理.

修改记录：
作者					日期					描述
-------------------------------------------------------------------------

************************************************************************/

#pragma once

#include <Object\Export.h>
#include <Data\RowStruct\RowTextTemplate.h>
#include <map>
#include <vector>


namespace EMObj		{

class CTextTempBuf 
{
public:
	//获取对象实例
	static CTextTempBuf &GetInstance();

	/*
	目的：获取所以文字模板的名称.
	参数：nameList	(out)		模板名字列表
	返回：名字个数
	*/
	int GetNameList(std::vector<CString> &nameList) const;

	/*
	目的：通过名字获取模板内容.
	参数：szName	(in)	模板名称
	      szContent	(out)	模板内容
	返回：成功返回TRUE，失败返回FALSE
	*/
	BOOL GetValue(const CString &szName, CString &szContent) const;

	/*
	目的：添加一个模板.
	参数：szName	(in)	模板名称
	      szVlaue	(in)	模板内容
	返回：成功返回TRUE，否则返回FALSE
	备注：如果存在则添覆盖之
	*/
	BOOL AddValue(const CString &szName, const CString &szValue);

	/*
	目的：删除一组映射.
	参数：szName	(in)	模板名称
	返回：成功返回TRUE，否则返回FALSE
	*/
	BOOL RemoveValue(const CString &szName);

protected:
	CTextTempBuf();
	virtual ~CTextTempBuf() {}

protected:
	std::map<CString, LONG64> m_name2Content;
};

}
