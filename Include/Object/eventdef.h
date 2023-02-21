/*************************************************
file:			COM_Object.h
Author:			张平    
Date:			2009-11-18
Description:    事件处理常量定义
*************************************************/
#pragma once
namespace EMObj
{
	//窗口类消息
	enum{
		Windows_Message_BeginEdit		= WM_USER,
		Windows_Message_EndEdit			= WM_USER + 1,
		Windows_Message_UpdateData		= WM_USER + 2,
		Windows_Message_ShowObjManager	= WM_USER + 3,
		Windows_Message_NewShowObj		= WM_USER + 4,		//新建显示对象
		Windows_Message_EditShowObj		= WM_USER + 5,		//修改显示对象
		Windows_Message_AREAPOINTNOTICE = WM_USER + 6,		//修改显示对象
		Windows_Message_MOUSEMOVE		= WM_USER + 7,		//修改显示对象
		Windows_Message_SetDecoration	= WM_USER + 8,		//装饰对象显隐配置
		Windows_Message_LONGPIPEDATADLG = WM_USER + 9,		//长距离管线导入工具
		Windows_Message_SetWeather		= WM_USER + 10,		//设置天气效果
		Windows_Message_SetLighting		= WM_USER + 11,		//光照配置
		Windows_Message_EnterEdit		= WM_USER + 12,		//企业信息编辑
		Windows_Message_UpdateBall		= WM_USER + 13,		//更新采集球
		Windows_Message_UpdateLod		= WM_USER + 14,		//更新LOD配置中的采集球
		Windows_Message_ToggleFullScreen= WM_USER + 15,		//窗口全屏切换
		Windows_Message_MODELDATADLG	= WM_USER + 16,		//异步加载模型数据导入工具
		Windows_Message_EnterpriseEdit	= WM_USER + 17,		//企业编辑
		Windows_Message_LAYEREDIT		= WM_USER + 18,		//物件管理编辑(图层)
		Windows_Message_PopupMenu		= WM_USER + 19,		//右键菜单消息
		Windows_Message_MenuEnable		= WM_USER + 20,		//系统菜单激活
		Windows_Message_RambleSetting	= WM_USER + 21,		//漫游设置
		Windows_Message_GUIEventAdapterPUSH=WM_USER+22,

		//添加动画播放消息
		Window_Message_Play				= WM_USER + 100,
		Window_Message_Pause			= WM_USER + 101,
		Window_Message_Continue			= WM_USER + 102,
		Window_Message_Stop				= WM_USER + 103,
		Window_Message_Forward			= WM_USER + 104,
		Window_Message_Rewind			= WM_USER + 105,
		Window_Message_Flash_Play		= WM_USER + 106,	//Flash播放窗口的创建与销毁
		Window_Message_Flash_Stop		= WM_USER + 107,
		Window_Message_Contrl			= WM_USER + 108,
		Window_Message_Seek				= WM_USER + 109,
		Window_Message_Play_Xml			= WM_USER + 110,	//xml内存数据播放动画元素：liugh
		Window_Message_Second_Player	= WM_USER + 111,	//xml内存数据播放动画元素：liugh



		//显示对象选择变化消息
		Window_Message_SelChanged		= WM_USER + 200,
		Window_Message_ClearAreaPoints	= WM_USER + 201,
		Window_Message_SurveyTool		= WM_USER + 202,	//测量
		Window_Message_ScreenTextMark	= WM_USER + 203,	//用户自定义屏幕标注
		Window_Message_GetPosByMouse	= WM_USER + 204,	//用户自定义屏幕文字标注
		Window_Message_ScreenMarkImport = WM_USER + 205, 	//屏幕标注批量导入工具
		Window_Message_ZipUnZipTool		= WM_USER + 206, 	//文件压缩与解压工具
		Window_Message_InterPpImportTool= WM_USER + 207,	//场内管线导入工具
		Window_Message_SaveDataDBToFile	= WM_USER + 208, 	//场内管线导入工具
		Window_Message_BatDelObj		= WM_USER + 209, 	//批量删除工具
		Window_Message_EnterBrowser		= WM_USER + 210,	//企业查看
		Window_Message_Embankment       = WM_USER + 211,    //挖填方
		Windows_Message_ProfileAnalysis = WM_USER + 212,     //剖面分析
		Windows_Message_UpdateArea      = WM_USER + 213,      //
		Windows_Message_Streetscape     = WM_USER + 214,	  //街景
		Windows_Message_QuickAddModel   = WM_USER + 215		  //快速加载模型
	};
}
	