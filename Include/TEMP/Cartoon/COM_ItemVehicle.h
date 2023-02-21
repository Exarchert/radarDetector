/**************************************************************************
Copyright (C),  
文件：CCOM_ItemVehicle.h CCOM_ItemVehicle.cpp
作者：黄志鹏
版本：1.0
日期：2009-07-29
描述：卡通项-交通工具类(适合所有模型类)

修改记录：
作者						日期					描述
---------------------------------------------------------------------------
黄志鹏						2010-01-14				添加对IVE动画的控制

**************************************************************************/

#pragma once

#include <temp/Export.h>
#include <osgCal/CoreModel>
#include <osgCal/Model>
#include <osg/NodeVisitor>
#include <osg/AnimationPath>
#include <osg/MatrixTransform>
#include <temp/Cartoon/COM_CartoonItem.h>


namespace TEMP		{

class TEMP_EXPORT CCOM_ItemVehicle : public CCOM_CartoonItem
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
	目的：播放
	*/
	virtual void Play();

	/*
	目的：暂停
	*/
	virtual void Pause();	

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
	目的：序列化操作
	*/
	virtual void Serialize(CArchive& ar);

	/*
	Purpose:	Parser XML	Data Format for Animation
	Author:		Liugh
	*/
	virtual bool ParserXml(MSXML2::IXMLDOMNodePtr pNode);


	//动画结点回调查找
	class CFindAnimationCallback : public osg::NodeVisitor
	{
	public:
		CFindAnimationCallback()
			: osg::NodeVisitor(osg::NodeVisitor::TRAVERSE_ALL_CHILDREN)
			{}

		virtual ~CFindAnimationCallback() { m_callbacks.clear(); }

		//保存Callback结点指针
		virtual void apply(osg::Node& node);

	public:
		std::vector<osg::ref_ptr<osg::AnimationPathCallback>> m_callbacks;
	};

protected:
	DECLARE_SERIAL(CCOM_ItemVehicle)

protected:
	CString	m_modelName;	//模型文件的名称
	osg::ref_ptr<osg::MatrixTransform> m_modelNode;
	osg::ref_ptr<osgCal::Model>	m_calModel;
	bool						m_isCfg;		//是否为骨骼动画
	unsigned int				m_actions;		//动作数量
	int							m_curAction;	//当前动作

	//动画结点查找
	CFindAnimationCallback		m_nodeFinder;
};

}