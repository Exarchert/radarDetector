/************************************************************************
Copyright (C)  
文件：KernelExport.h KernelExport.cpp
作者：黄志鹏
版本：1.0
日期：2009-11-11
描述：内核导出函数

修改记录：
作者					日期				描述
----------------------------------------------------------------------
黄志鹏					2009-12-23			加入开机画面渲染类

************************************************************************/

#pragma once 

#include <Kernel\Export.h>
#include <Kernel\Init\InitModule.h>
#include <COM\Event\Event.h>
#include <COM\KernelContext\KernelContext.h>
#include <Object\Obj.h>
#include <Kernel\CmdHandleCenter\CmdManager.h>
#include <Kernel\AnimationManager\AnimationType.h>
#include <Kernel\PipleLineAnimation\PipleAnimationDB.h>
#include <iostream>
#include <vector>

namespace Kernel	{


KERNEL_EXPORT BOOL AddModuleInit(CInitModule *pModuleInit);

KERNEL_EXPORT void SetConfigFile(CString strFile);
KERNEL_EXPORT void WriteConfigValue(CString,CString,CString);
KERNEL_EXPORT bool ShowSelProDlg(CWnd* pParent = NULL);
KERNEL_EXPORT bool ShowSelProDlg(CWnd* pParent ,BOOL bIsShowDialog );
KERNEL_EXPORT bool ShowUrlSelProDlg(CWnd* pParent = NULL);
KERNEL_EXPORT bool SelectProDlg(CWnd* pParent = NULL);

KERNEL_EXPORT void KernelStartUp(HWND hwnd, COM::CMessageQueue *pMsgQueue = NULL, UINT MsgValue = 0, DWORD style = 1);

KERNEL_EXPORT void KernelStop();
KERNEL_EXPORT void GetDisplayCardInfo();

//检测硬盘ID
//KERNEL_EXPORT BOOL CheckHardDiskID();

//检测加密狗,mode = 0为编辑工具 1为动画播放	2为圣天狗
//KERNEL_EXPORT BOOL CheckDog(UINT mode = 0);

KERNEL_EXPORT BOOL CheckKey(HWND hwd);

KERNEL_EXPORT COM::CKernelContext * GetpKC();

KERNEL_EXPORT void AddStreetscape();

KERNEL_EXPORT void DirectTo(double lon, double lat, double height, float yaw, float pitch, float roll);

KERNEL_EXPORT void FlyTo(double lon, double lat, double height, float yaw, float pitch, float roll);

KERNEL_EXPORT void GetCameraPosPose(double &lon, double &lat, double &height, float &yaw, float &pitch, float &roll);

KERNEL_EXPORT void ShowDecorasWindow(bool bShow);

KERNEL_EXPORT void ShowWeatherWindow(bool bShow);

KERNEL_EXPORT void ShowLightingWindow(bool bShow);

KERNEL_EXPORT void ShowSurveyToolWindow(bool bShow);	

KERNEL_EXPORT void ToggleFullScreen();

KERNEL_EXPORT void EnableWeather(BOOL bEnable);

KERNEL_EXPORT void SetWeather(COM::CWeather::ATTRIBUTE attribute,COM::CWeather::INTENSION intension,DWORD d);

KERNEL_EXPORT void EnableRamble(bool bEnable);

KERNEL_EXPORT void ChangeManip();

//获取当前选中的首个ID
KERNEL_EXPORT LONG64 GetSelObjID();
//获取当前选中的ID组
KERNEL_EXPORT std::vector<LONG64> GetSelObjIDs();

//设置渲染模式
KERNEL_EXPORT void SetStereo(osg::DisplaySettings::StereoMode stereoMode, bool ONOFF);


//设置框选开关
KERNEL_EXPORT void SetRegionSwitch(bool switchs);
//点击获取位置事件
KERNEL_EXPORT void SetClickMessageSwitch(bool switchs);
//设置添加模型开关
KERNEL_EXPORT void SetAddModelSwitch(bool switchs);
KERNEL_EXPORT void setAddModelFile(std::string modelFile);
KERNEL_EXPORT void drawPiple();
KERNEL_EXPORT void deleteManual();

//添加粒子系统
KERNEL_EXPORT void AddParticle(CString id,CString modeID,CString particleStype);
KERNEL_EXPORT void AddParticleCoordinate(CString id,osg::Vec3 coordinate,CString particleStype,CString strParam);
//删除粒子系统
KERNEL_EXPORT void DeleteParticle(CString id);

//设置剖面开关
KERNEL_EXPORT void SetProfileManagerSwitch(bool switchs);

//设置地下开关
KERNEL_EXPORT void SetEmbankmentManagerSwitch(bool switchs);

//开始管道动画 animationState 1 表示动画开始 2 表示动画结束 
KERNEL_EXPORT void PipleLineAnimationSwitch(AnimationPipleLineModeDB aplm,AnimationMode animationState);


KERNEL_EXPORT EMObj::CObj* PtSelectTest(double x, double y);

KERNEL_EXPORT  bool GetRay(float x, float y, osg::Vec3d& eye, osg::Vec3d& ray);

//获取相机对应区域宽度
KERNEL_EXPORT double GetCameraScope(void);

//抓图
KERNEL_EXPORT bool Screenshot(int w,int h,std::string imagePathName);

//根据宽度设置相机位置
KERNEL_EXPORT void SetCameraScope(DOUBLE verticalDist);

KERNEL_EXPORT void SetDecorasShowOrNot(CString strDecorate,bool bShow = true);

KERNEL_EXPORT void BootOrPause3DRender(bool bPause = true);

KERNEL_EXPORT void ImportShpLineManager(std::string sqlLine);

KERNEL_EXPORT void ClearPiple();
//============zhangqiang============
//加个参数，用于初始化窗口标题
KERNEL_EXPORT BOOL ShowPlayCtrl(CString title);

//导出一个存放所有有权限运行程序的IP列表
extern KERNEL_EXPORT std::vector<std::string> HardDiskIDList;

extern KERNEL_EXPORT CCmdManager g_CmdManager;


KERNEL_EXPORT void ShowRambleSettingDlg(bool bShow);

//
KERNEL_EXPORT void CameraAnimation();

class KERNEL_EXPORT CBootScreen
{
public:
	CBootScreen();
	virtual ~CBootScreen();

	/*
	目的：初始化
	参数：文件名称
	ResType			(in)		资源类型，1表示本地，2表示网络
	Para1			(in)		本地资源时表示路径，网络资源是表示Url
	Para2			(in)		本地资源时无效，网络资源是表示本地缓存目录
	*/
	void Init(CDC *pDc, DWORD ResType, CString Para1 = _T(""), CString Para2 = _T(""));

	/*
	目的：处理字符消息
	返回：返回true. 表示初始化完成
	*/
	bool HandleMsg(const CString &szMsg, CRect &rt);

	/*
	目的：绘制画面
	*/
	void OnDraw(CDC *pDc, CRect rt);

 	enum ERR_LEVEL {
 		LEVEL_NORMAL,
 		LEVEL_WARNING,
 		LEVEL_ERROR
 	};

protected:
	//WM_COPYDATA
	//
	CDC		m_backGroundDC;		//背景图片
	CDC		m_initDC;			//已初始化进度图片
	CDC		m_uninitDC;			//未初始化进度图片
	CSize	m_backGroundSize;	//背景图片大小
	CSize	m_initSize;			//已初始化进度图片大小
	CSize	m_uninitSize;		//未初始化进度图片大小
	int		m_process;			//进度值
	CString	m_szMsg;			//消息
	ERR_LEVEL m_errLevel;		//出错等级
};

}
