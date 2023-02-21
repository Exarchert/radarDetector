/************************************************************
Copyright (C),  
文件：MsgTracer.h MsgTracer.cpp
作者：黄志鹏
版本：1.0
日期：2009-11-11
描述：日志信息跟踪对象类.

修改记录：
作者					日期				描述
-------------------------------------------------------------

************************************************************/

#pragma once

#include <COM\Export.h>


namespace COM	{

//CMsgTracer类
class COM_EXPORT CMsgTracer
{
public:
	CMsgTracer();
	virtual ~CMsgTracer();

	/*
	目的：将跟踪信息记录到该对象
	参数：lpMsg		(in)	字符串指针,以'\0'结尾
	返回：void
	*/
	virtual void Write(const TCHAR* lpMsg);

	/*
	目的：将跟踪信息记录到该对象
	参数：strMsg	(in)	字符对象
	返回：void
	*/
	virtual void Write(const CString &strMsg);
};

//CDumpToNotePad类
//打印到记事本
class COM_EXPORT CDumpToNotePad : public CMsgTracer
{
public:
	CDumpToNotePad();
	virtual ~CDumpToNotePad();

	/*
	目的：将跟踪信息记录到该对象
	参数：lpMsg		(in)	字符串指针
	返回：void
	*/
	virtual void Write(const TCHAR* lpMsg);

	/*
	目的：将跟踪信息记录到该对象
	参数：strMsg	(in)	字符对象
	返回：void
	*/
	virtual void Write(const CString &strMsg);
};

//CDumpToScreen类
//输出到开机动画
//定义消息格式如下:
//	进度:xx|错误等级:消息内容
//	
class COM_EXPORT CDumpToScreen : public CMsgTracer
{
public:
	CDumpToScreen(HWND hDest);
	virtual ~CDumpToScreen();

	/*
	目的：将跟踪信息记录到该对象
	参数：lpMsg		(in)	字符串指针
	返回：void
	*/
	virtual void Write(const TCHAR* lpMsg);

	/*
	目的：将跟踪信息记录到该对象
	参数：strMsg	(in)	字符对象
	返回：void
	*/
	virtual void Write(const CString &strMsg);

protected:
	HWND	m_hDest;	//目标窗口
};

extern COM_EXPORT bool g_bThreadExit;
}