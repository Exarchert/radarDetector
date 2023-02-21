/*************************************************
file:			Main.h
Author:			张平    
Date:			2009-11-18
Description:    程序主类
*************************************************/
#pragma once
#include <Kernel\Export.h>
#include <Com\KernelContext\KernelContext.h>
#include <vector>
#include <Object\Obj.h>
#include <osg\ref_ptr>
#include <kernel\init\InitModule.h>

namespace Kernel
{

class KERNEL_EXPORT CMain
{
public:
	CMain(void);
	~CMain(void);

	//打开配置文件
	void LoadSysConfigFile(CString strFile);

	//内核上下文
	osg::ref_ptr<COM::CKernelContext>	m_kContext;

	//应用程序路径
	CString								m_appPath;//执行程序exe所在位置

	//配置文件路径
	CString								m_SysConfigFilePath;//配置文件全路径

	//项目路径
	CString								m_ProPath;//记录配置文件sysconfig.ini 所在位置

	//数据库连接密码
	CString								m_DBPassWord;

	std::vector<bool>                   m_isLoadEnter;
	////选中对象列表
	//std::vector<osg::ref_ptr<EMObj::CObj>> m_SelObjs;


	std::vector<osg::ref_ptr<CInitModule>>	m_AppInitModules;	//应用工程初始化对象

};

}