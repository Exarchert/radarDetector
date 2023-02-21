/*************************************************************************
File:			EventDef.h
Author:			张平    
Date:			2009-0x00000010-18
Description:    事件定义头文件


*************************************************************************/
#pragma once

//控制面板
#define USER_MSG_START						(0x0400 + 100)
#define CTRL_PANEL_FORWARD					(USER_MSG_START+1)
#define CTRL_PANEL_BACK						(USER_MSG_START+2)
#define CTRL_PANEL_LEFT						(USER_MSG_START+3)
#define CTRL_PANEL_RIGHT					(USER_MSG_START+4)
#define CTRL_PANEL_ROTATELEFT				(USER_MSG_START+5)
#define CTRL_PANEL_ROTATERIGHT				(USER_MSG_START+6)
#define CTRL_PANEL_CAMERA_FORWARD			(USER_MSG_START+7)
#define CTRL_PANEL_CAMERA_BACK				(USER_MSG_START+8)


//事件类型
#define Event_Base						0x00000001		//基本事件
#define Event_KeyboardAndMouse			0x00000002		//鼠标和键盘事件
#define Event_OperatorsRefresh			0x00000004		//鼠标和键盘刷新事件
#define Event_CommonComponent			0x00000008		//常用组件事件
#define Event_SysShowObj_Base			0x00000010		//固有显示对象基本事件
#define Event_SysShowObj_Operation		0x00000020		//固有显示对象操作事件
#define Event_SysShowObj_Control 		0x00000040		//固有显示对象控制事件
#define Event_AppShowObj_Base			0x00000080		//用户显示对象基本事件
#define Event_AppShowObj_Operation		0x00000100		//用户显示对象操作事件
#define Event_AppShowObj_Control		0x00000200		//用户显示对象控制事件
#define Event_Measurement				0x00000400		//测量事件
#define Event_RegionSelect              0x00001600      //框选事件 
#define Event_XmlTabel                  0x00003200		//标牌事件
#define Event_PipleObjSelect            0x00003201		//管网选择
#define Event_ClickMessage              0x00003202      //点击事件 


//基本事件消息
#define Message_Base_InitSysModule		0x00000001		//初始化系统模块
#define Message_Base_InitAppModule		0x00000002		//初始化应用模块
#define Message_Base_StartRender		0x00000004		//开始渲染
#define Message_Base_UnInitAppModule	0x00000008		//反初始化应用模块
#define Message_Base_UnInitSysModule	0x00000010		//反初始化系统模块
#define Message_Base_StopRender			0x00000020		//停止渲染
#define Message_Base_InitFailure		0x00000040		//初始化失败

//鼠标和键盘事件消息
#define Message_KM_PUSH					0x00000001		//鼠标按下事件
#define Message_KM_RELEASE 				0x00000002		//鼠标弹起事件
#define Message_KM_DOUBLECLICK			0x00000004		//鼠标双击事件
#define Message_KM_DRAG					0x00000008		//拖动事件
#define Message_KM_MOVE					0x00000010		//鼠标移动
#define Message_KM_KEYDOWN				0x00000020		//键盘按下
#define Message_KM_KEYUP				0x00000040		//键盘抬起
#define Message_KM_RESIZE				0x00000100		//大小改变
#define Message_KM_SCROLL				0x00000200		//滚动
#define Message_KM_KSDOWN_MRIGHTRELEASE	0x00000400		//键盘Ｓ按下并且鼠标右键抬起事件

//操作器刷新事件
#define Message_OperatorsRefresh_Update 0x00000001		//操作器刷新	

//常用组件事件消息

//对象基本事件消息
#define Message_ShowObj_Base_Init		0x00000001		//初始化
#define Message_ShowObj_Base_Draw		0x00000002		//绘制
#define Message_ShowObj_Base_Refresh	0x00000004		//刷新
#define Message_ShowObj_Base_Locate		0x00000008		//定位
#define Message_ShowObj_Base_BeginEdit	0x00000010		//开始编辑
#define Message_ShowObj_Base_EndEdit	0x00000020		//结束编辑
#define Message_ShowObj_Base_AddToDB	0x00000021		//成功添加一条数据到数据库
#define Message_ShowObj_Base_UptadeToDB	0x00000022		//成功修改一条数据到数据库
#define Message_ShowObj_Base_DeleteToDB	0x00000023		//成功删除一条数据从数据库
#define Message_ShowObj_Base_ActionDef	0x00000040		//基本动作
#define Message_ShowObj_Base_SaveToDB	0x00000080		//存储到数据库
#define Message_ShowObj_Base_Selected	0x00000100		//选中




//对象操作事件消息


//对象刷新事件消息
#define Message_ShowObj_Control_IsLodAble		0x00000001		//Lod控制是否有效

