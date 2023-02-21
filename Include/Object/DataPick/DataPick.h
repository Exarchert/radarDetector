/****************************************************************************
Copyright (C),  
文件：DataPick.h  DataPick.cpp
作者：黄志鹏
版本：1.0
日期：2010-02-03
描述：数据采集球，用于可视化地采集空间数据.

修改记录：
作者						日期						描述
-----------------------------------------------------------------------------

****************************************************************************/
#pragma once
#include <Object\Export.h>
#include <osg\Matrixd>
#include <osg\Switch>
#include <osg\MatrixTransform>
#include <osgGA\GUIEventHandler>
#include <Com\DataBall\DataBall.h>

namespace EMObj{

class OBJECT_EXPORT CDataPick : public osgGA::GUIEventHandler
{
public:
	CDataPick(void);
	virtual ~CDataPick(void);

	//处理OSG输入
	virtual bool handle(const osgGA::GUIEventAdapter& ea,osgGA::GUIActionAdapter& aa);

	//初始化
	void Init();

	/*
	目的：设置提示球(或其它模型)，表示当前的位置和姿态
	参数：szHint		(in)		资源名称
	*/
	void SetHintBall(LPCTSTR szHint);

	//获取提示球的结点
	osg::Node * GetHintBall() const;

	//更新提示球位置和姿态
	void UpdateHintBall(const osg::Matrixd &mat);

	//添加提示球
	void AddHintBall();

	//移除提示球
	void RemoveHintBall();

	//显隐提示球
	void ShowHintBall(bool bShow);

public:
	COM::CDataBall	m_dataBall;						//根据用户输入计算位置

protected:
	osg::ref_ptr<osg::MatrixTransform>	m_trans;	//变换矩阵
	osg::ref_ptr<osg::Switch>			m_switch;	//显隐控制
	bool			m_bWork;						//是否启用
};

}