/************************************************************************
Copyright (C)  
文件：RambleManager.h RambleManager.cpp
作者：白冰
版本：1.0
日期：2009-06-18
描述：漫游操作器管理类，用于和用户输入接口，封装具体的漫游器.

修改记录：
作者					日期				描述
-------------------------------------------------------------------------
黄志鹏					2009-11-13			移植老版本，修改部分成员函数.

************************************************************************/

#pragma once 

#include <COM\Export.h>
#include <COM\Ramble\RambleBase.h>
#include <osgGA\GUIEventHandler>
#include <osgGA\GUIEventAdapter>
#include <osgGA\GUIActionAdapter>


namespace COM	{

class COM_EXPORT CRambleManager : public osgGA::GUIEventHandler
{
public:
	CRambleManager();
	virtual ~CRambleManager();

	//事件处理
	bool handle(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa);

	//处理Windows发送的消息
	virtual bool handle(UINT msg, WPARAM wParam, LPARAM lParam);

	//设置和获取当前的漫游者
	void SetRambler(CRambleBase *rambler);
	CRambleBase *GetRambler() const;

	//设置漫游是否工作
	bool EnableRamble(bool bEnable=true);

protected:
	osg::ref_ptr<CRambleBase>	m_rambler;
	bool						m_bEnable;
};

}
