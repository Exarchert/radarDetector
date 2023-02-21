/********************************************************************************
Copyright (C),  
文件：ItemCamera.h  ItemCamera.cpp
作者：黄志鹏
版本：1.0
日期：2009-08-11
描述：卡通动画--相机

修改记录
作者						日期							描述
---------------------------------------------------------------------------------
黄志鹏					2009-12-09						移植到此工程

********************************************************************************/

#pragma once

#include <COM\Cartoon\CartoonItem.h>
#include <osgGA\MatrixManipulator>

namespace COM	{

class COM_EXPORT CItemCamera : public CCartoonItem
{
public:
	CItemCamera()
	{
		m_Name = _T("未命名相机");
	}

	virtual ~CItemCamera();

	CItemCamera(const CItemCamera &camera);

	CCartoonItem * Clone();

	//设置相机操作器
	void SetMatrixManipulator(osgGA::MatrixManipulator *mani);

	/*
	目的：动画单元执行行为
	参数：Time		(in)	相对于开始活动时刻的时刻
	      pos		(in)	动画单位此时的位置			
	返回：void
	*/
	virtual void DoAction(double Time, osg::Vec3d pos);

protected:
	osg::ref_ptr<osgGA::MatrixManipulator>	m_mani;
};

}