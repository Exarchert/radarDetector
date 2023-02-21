/*********************************************************************
Copyright (C),  
文件：InitManipulator.h InitManipulator.cpp
作者：黄志鹏
版本：1.0
日期：2009-11-13
描述：初始化相机操作器.

修改记录：
作者					日期				描述
----------------------------------------------------------------------

*********************************************************************/

#pragma once

#include <Kernel\Export.h>
#include <Kernel\Init\InitModule.h>


namespace	Kernel
{

class CCameraPlacement 
{
public:
	CCameraPlacement();
	CCameraPlacement(int key, double lon, double lat, double height, float yaw, float pitch, float roll);

public:
	int		m_key;
	double	m_lon;
	double	m_lat;
	double	m_height;
	float	m_yaw;
	float	m_pitch;
	float	m_roll;
};

class CInitManipulator : public CInitModule
{
public:
	CInitManipulator();
	virtual ~CInitManipulator();

	/*
	目的：执行初始化
	返回：成功返回TRUE，否则返回FALSE
	*/
	virtual BOOL InitInstance(COM::CKernelContext &kContext);

	/*
	目的：释放初始化
	*/
	virtual BOOL UnInitInstance(COM::CKernelContext &kContext);

	/*
	目的：初始化环境变量
	*/
	virtual void InitEnviroment();

protected:
	CCameraPlacement				m_default;
	std::vector<CCameraPlacement>	m_places;
};

}
