/***********************************************************************
Copyright (C),  
文件：TEMPExports.h TEMPExports.cpp
作者：黄志鹏
版本：1.0
日期：2010-02-02
描述：动画模块的导出接口.

修改记录：
作者						日期					描述
-----------------------------------------------------------------------
liugh	add function	forward rewide seek SetScriptPath GetScriptPath
						PlayScriptFromXmlString GetScriptTimeLength
***********************************************************************/

#pragma once

#include <temp/Export.h>
#include <com/KernelContext/KernelContext.h>
#include <object/ObjManager.h>


namespace TEMP		{

/*
目的：初始化动画模块.
参数：szBinPath		(in)	程序所在目录
	  pContext		(in)	内核运行上下文指针
	  pObjMan		(in)	物件管理对象指针
*/
TEMP_EXPORT void Init(const CString &szBinPath, COM::CKernelContext *pContext, EMObj::CObjManager *pObjMan);

//清理动画模块
TEMP_EXPORT void UnInit();

/*
目的：播放脚本
参数：szScript		(in)	脚本名称

－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－－

Liugh添加参数 mode	(in)	文件加载方式，默认为(mode=0)archive文件，否则为(mode=1)xml格式文件
*/
TEMP_EXPORT void PlayScript(const CString &szScript);

TEMP_EXPORT void PlayNewScript(const CString &szScript);



/*
purpose:	Load Animation data from xml string
parameter:	xmlString	(in)	description of an action for animation item
Liugh添加 	
*/

/*
<camera name="camera" b_t="0" l_t="40">
	<tracks>
		<track timelen="16.5">
			<pos bx="-2437113.7268576822" by="5364402.4324997868" bz="2434623.2714401949"  ex="-2437029.9309432711" ey="5362686.4436971657" ez="2439277.1751346760" ></pos>
			<pose bpx="-1.9400000572204601" bpy="-7.9099993705749503" bpz="0" epx="-1.9400000572204601" epy="-7.9099993705749503" epz="0"></pose>
			<speed v0="300" a="0" ></speed>
		</track>
	</tracks>
</camera>
*/

TEMP_EXPORT void PlayScriptFromXmlString(const CString &xmlString);

/*
目的：暂停\继续
参数：bPause	(in)	true:暂停, false:继续
*/
TEMP_EXPORT void Pause(bool bPause);

//停止
TEMP_EXPORT void Stop();

//快进
TEMP_EXPORT void Forward(int step);

//慢进
TEMP_EXPORT void Rewind(int step);

//跳转
TEMP_EXPORT void Seek(double time);

//案例库目录
TEMP_EXPORT void SetScriptPath(CString ScriptPath);

TEMP_EXPORT CString GetScriptPath(CString& ScriptPath = CString(_T("")));

/*
获取或设置当前正在播放的脚本名称
*/
TEMP_EXPORT CString GetCurScriptName(CString& ScriptPath = CString(_T("")));
TEMP_EXPORT void SetCurScriptName(CString ScriptPath);

//获取播放脚本时间长度
TEMP_EXPORT double GetScriptTimeLength();

//获取播放指定脚本的脚本时间长度
TEMP_EXPORT double GetScriptTimeLength(CString scriptFullPath);

//获取当前播放时刻
TEMP_EXPORT double GetCurPlayTime();


extern TEMP_EXPORT CString g_AnimationPath;
extern TEMP_EXPORT CString g_CurPlayScriptName;
extern TEMP_EXPORT BOOL g_AnimationIsFinish;
}