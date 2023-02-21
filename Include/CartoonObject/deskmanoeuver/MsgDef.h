/*************************************************************************
FileName：MsgDef.h
Author:	Liugh
Date：2010-01-22
Description：Manoeuver Message define
**************************************************************************/
#pragma once

namespace CartoonObject
{
	enum
	{
		//UI->三维
		WM_MSG_BEGINE		= 	 WM_USER + 0x00500,
		WM_MSG_DISASTER		=	 WM_MSG_BEGINE + 1,		//灾害
		WM_MSG_FIREEQUIP	=	 WM_MSG_BEGINE + 2,		//消防设备
		WM_MSG_MOVEMENT		=	 WM_MSG_BEGINE + 3,		//车
		WM_MSG_ROLE			=	 WM_MSG_BEGINE + 4,		//人物


		//桌面推演
		Window_Message_StartDeskTest	=	WM_MSG_BEGINE + 10,	//桌面推演
		Window_Message_ScriptRegister	=	WM_MSG_BEGINE + 11,	//桌面推演
		Window_Message_DeskFolder_Create=	WM_MSG_BEGINE + 12,	//推演库新建


		//三维->UI
		WM_WND_MSG_BEGINE		= 	WM_USER + 0x00600,
		WM_WND_MSG_DISASTER		=	WM_WND_MSG_BEGINE + 1,	//灾害
		WM_WND_MSG_FIREEQUIP	=	WM_WND_MSG_BEGINE + 2,	//消防设备
		WM_WND_MSG_MOVEMENT		=	WM_WND_MSG_BEGINE + 3,	//车
		WM_WND_MSG_ROLE			=	WM_WND_MSG_BEGINE + 4,	//人
		WM_WND_MSG_HOTEREA		=	WM_WND_MSG_BEGINE + 5,	//热区
		WM_WND_MSG_HELP_SRC		=	WM_WND_MSG_BEGINE + 6,	//物资
		WM_WND_MSG_TEXT_MARK	=	WM_WND_MSG_BEGINE + 7,	//标注
		WM_WND_MSG_ELEMENT		=	WM_WND_MSG_BEGINE + 8,	//通用
		
		//begin fire
		WM_WND_MSG_BEGING_FOAM_FIRE = WM_WND_MSG_BEGINE +9,
		WM_WND_MSG_END_FOAM_FIRE    = WM_WND_MSG_BEGINE+10,
		WM_WND_MSG_FOAM_DATABALLMOVE = WM_WND_MSG_BEGINE + 11,
		WM_WND_MSG_BEGING_WARTER_FIRE = WM_WND_MSG_BEGINE +12,

		WM_WND_MSG_BEGINGPOOL_FIRE      =   WM_WND_MSG_BEGINE + 13,		//池火
		WM_WND_MSG_BEGINGSPRAY_FIRE     =   WM_WND_MSG_BEGINE + 14,		//喷射火
		WM_WND_MSG_BEGINGEXPLOSION      =   WM_WND_MSG_BEGINE + 15,		//爆炸
		
		//热区
		Window_Message_BeginHotAreaEdit = WM_WND_MSG_BEGINE + 16,	//开始热区编辑
		Window_Message_DoubleHotMark	= WM_WND_MSG_BEGINE + 17,	//双击热区图标				
		Window_Message_ModifiyHotArea   = WM_WND_MSG_BEGINE + 18,    //定义热区
		Window_Message_EndModifiyHotArea = WM_WND_MSG_BEGINE + 19,	//热区定义结束	
		Window_Message_CancelModifiyHotArea = WM_WND_MSG_BEGINE + 20, //取消热区编辑
		Window_Message_EndingHotAreaEdit = WM_WND_MSG_BEGINE + 21,	//结束热区编辑
		Window_Message_DeleteHotArea	= WM_WND_MSG_BEGINE + 22 ,	//删除热区
		Window_Message_BeginDetectionHotArea = WM_WND_MSG_BEGINE + 23,	//检测热区
		Window_Message_DestroyPopMenu   =   WM_WND_MSG_BEGINE+ 24,		//销毁热区右键菜单
		Window_Message_ClickPopMenu   =   WM_WND_MSG_BEGINE+ 25,			//点击菜单
		Window_Message_WritedToDisk    = WM_WND_MSG_BEGINE+ 26,         //已经写到硬盘上

		//begin gas leak
		WM_WND_MSG_BEGINGASLEAK      =   WM_WND_MSG_BEGINE + 30,
		WM_WND_MSG_GASLEAK_DATABALLMOVE = WM_WND_MSG_BEGINE + 31,
		WM_WND_MSG_ENDGASLEAK		=  WM_WND_MSG_BEGINE + 32,
	
		//mark
		WM_WND_MSG_BEGINMARK		= WM_WND_MSG_BEGINE + 33,		//开始标绘
		WM_WND_MSG_ENDMARK			= WM_WND_MSG_BEGINE + 34,			//结束标绘
		WM_WND_MSG_ENDMARKLINE		= WM_WND_MSG_BEGINE + 35
	};
}

/***********************************************/
//拖动图标离开窗口后发送该消息
