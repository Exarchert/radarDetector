/*********************************************************************
Copyright (C),  
文件：DecoraManager.h DecoraManager.cpp
作者：黄志鹏
版本：1.0
日期：2009-11-16
描述：装饰对象管理类.

修改记录：
作者					日期				描述
----------------------------------------------------------------------

*********************************************************************/

#pragma once

#include <Com\Export.h>
#include <Com\Decoration\DecoraBase.h>
#include <osgGA\GUIEventHandler>
#include <osgGA\GUIEventAdapter>
#include <osgGA\GUIActionAdapter>
#include <OpenThreads\Mutex>

#include <map>


namespace COM	{

//装饰对象的预定义ID
#define	DECORA_HINTBAR			0x0001
#define DECORA_CONTROL_PANEL	0x0002
#define DECORA_NAVIGATION_MAP	0x0003
#define DECORA_NORTH_NEEDLE		0x0004
#define DECORA_SUNSHINE			0x0005
#define DECORA_FOCUSCROSS		0x0006


//CDecoraManager类
class COM_EXPORT CDecoraManager : public osgGA::GUIEventHandler
{
public:
	CDecoraManager();
	virtual ~CDecoraManager();
	
	/*
	目的：处理来自OSG消息队列的事件
	*/
	virtual bool handle(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa);

	/*
	目的：添加装饰对象
	参数：id		(in)	对象id
	      decora	(in)	对象指针
	返回：成功返回true，否则返回false; 如果id已经存在，则覆盖已有.
	*/
	bool AddDecoration(DWORD id, CDecoraBase *decora);

	/*
	目的：移除某个装饰对象
	参数：id		(in)	对象id
	返回：成功返回true，失败返回false
	*/
	bool RemoveDecoration(DWORD id);

	/*
	目的：获取某对象指针
	参数：id		(in)	对象id
	返回：返回对象指针，如果失败返回NULL
	*/
	CDecoraBase *GetDecoration(DWORD id) const;
	
	/*
	目的：获取某对象的显隐状态
	返回：true显示,否则false
	*/
	bool IsDecorationShow(DWORD id);

	/*
	目的：设置某个对象是否显示
	参数：id		(in)	对象id
	      bShow		(in)	true显示,false隐藏
	返回：void
	*/
	void ShowDecoration(DWORD id, bool bShow);

protected:
	class CShowCmd : public osg::Referenced
	{
	public:
		DWORD	m_id;	//装饰对象id
		bool	m_bShow;//显隐
	};
	typedef std::vector<osg::ref_ptr<CShowCmd>>	ShowCmdList;
	typedef std::map<DWORD, osg::ref_ptr<CDecoraBase>>	DecorasMap;
	
	OpenThreads::Mutex	m_cmdsMutex;	//互斥量
	ShowCmdList			m_showCmds;		//显隐命令队列
	DecorasMap			m_decoras;		//装饰对象
};

}