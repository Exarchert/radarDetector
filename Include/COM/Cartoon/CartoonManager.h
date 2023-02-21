/***********************************************************************
File:			COM_CartoonManager.h
Author:			张平    
Date:			2009-03-31
Description:    动画管理类

ChangeLog:
作者					日期					描述
-------------------------------------------------------------------------
黄志鹏				2009-07-21			审查，删除不必要的代码

************************************************************************/

#pragma once
#include <COM\Export.h>
#include <osg/ref_ptr>
#include <osg/Node>
#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>
#include <osgGA/GUIEventAdapter>
#include <osgGA/GUIActionAdapter>
#include <osgGA/GUIEventHandler>
#include <osgGA/EventQueue>
#include <COM\Cartoon\CartoonUnit.h>
#include <vector>

namespace COM{

class COM_EXPORT CCartoonManager : public osgGA::GUIEventHandler 
{
public:
	CCartoonManager(void);
	virtual ~CCartoonManager(void);

	//添加和删除卡通单元
	void AddCartoonUnit(CCartoonUnit *pCartoonUnit);
	void DelCartoonUnit(CCartoonUnit *pCartoonUnit);

	void Clear();

	//更新时间(delt)
	void Update(double delt);
	void SetPause();
	void SetPlay();

	//事件处理
	bool handle(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa);
	//按下鼠标左键则停止播放
	void SetPushDone(bool done) { m_done = done; }

protected:
	std::vector<osg::ref_ptr<CCartoonUnit>> m_CartoonUnits;
	double	m_LastTime;
	bool	m_done;
};


}