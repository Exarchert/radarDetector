/**********************************************************************************
File:			COM_CartoonManager.h
Author:			张平    
Date:			2009-03-31
Description:    动画管理类

ChangeLog:
作者					日期					描述
-----------------------------------------------------------------------------------
黄志鹏				2009-07-21			审查，删除不必要的代码
黄志鹏				2009-11-08			添加CCOM_PassiveCartoonPlayer
黄志鹏				2009-12-29			添加大事件列表命中功能

Liugh				2010.11.25			Add All XML Parser Function For Each Script

***********************************************************************************/

#pragma once

#include <temp/Export.h>
#include <osg/ref_ptr>
#include <osg/Node>
#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>
#include <osgGA/GUIEventAdapter>
#include <osgGA/GUIActionAdapter>
#include <osgGA/EventQueue>
#include <temp/Cartoon/COM_CartoonUnit.h>


namespace TEMP		{

class TEMP_EXPORT CCOM_CartoonManager
{
public:
	CCOM_CartoonManager(void);
	virtual ~CCOM_CartoonManager(void);

	//添加和删除卡通单元
	void AddCartoonUnit(CCOM_CartoonUnit *pCartoonUnit);
	void DelCartoonUnit(CCOM_CartoonUnit *pCartoonUnit);

	void Clear();

	//更新时间
	void Update(double Time);
	void SetPause();
	void SetPlay();
	void SetForward(int step);
	void SetRewind(int step);

	//设置影片的已播放时间
	void SetPlayTime(double time);

	//获取播放总长
	double GetScriptTimeLength(int index = 0);

	//获取当前播放时刻
	double GetCurPlayTime(int index = 0);

protected:
	std::vector<osg::ref_ptr<CCOM_CartoonUnit>> m_CartoonUnits;
	double m_LastTime;
};


//////////////////////////////////////////////////////////////////////////
//大事件列表热区
class CItemRegion
{
public:

	bool isPick(float x, float y)
	{
		return ((x>m_left) && (x<m_right) && (y>m_top) && (y<m_bottom));
	}

	//以下为热区矩形left、top、right、bottom
	float m_left;
	float m_top;		
	float m_right;
	float m_bottom;

	//相应的播放起始时间
	double m_time;
};

class CEventListRegion
{
public:
	//以下为大事件图片的区域原点
	float m_left;
	float m_top;
	//是否有效
	bool m_bValid;
	//热区列表
	std::vector<CItemRegion> m_regList;
};

//播放事件处理
class CCOM_CartoonPlayEventHandler : public osgGA::GUIEventHandler
{
public:
	CCOM_CartoonPlayEventHandler() {}

	virtual ~CCOM_CartoonPlayEventHandler() {}

	//事件处理
	bool handle(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa);

	//初始化
	bool Init();

	//设置脚本
	void SetScript(const CString &szScript);
	
	//Archive Format File
	bool LoadFromArchiveFile(const CString &szFilePath);
	
	//XML Format File
	bool LoadFromXMLFile(const CString &szFilePath);

	//XML String Format
	void LoadScript(const CString& strXml);

	//获取脚本播放时间总长
	double GetScriptTimeLength();

	//获取当前播放时刻
	double GetCurPlayTime();


private:
	void InitCartoonUnit(CCOM_CartoonUnit* ptr);

protected:
	//加载脚本
	void LoadScript();

public:
	CCOM_CartoonManager m_CartoonManager;
	CEventListRegion	m_eventList;

protected:
	CString		m_szScripts;	//脚本名称
	double		m_beginTime;	//开始点时刻（世界时间）
};

}