/******************************************
File name:			CUserCursor.h
Author:				刘广慧    
Date:				2009-09-17
Description:		光标设置类
*******************************************/
#pragma once
#include <COM\Export.h>
#include <osgViewer\Viewer>

namespace COM
{
	class COM_EXPORT CUserCursor
	{
	public:
		CUserCursor(osgViewer::Viewer* viewer);
		~CUserCursor(void);

		/**********************************************************
		函数描述：注册新的光标资源，资源全路径与光标名进行绑定
		cursorPatn			in				光标资源路径
		cursorName			in				自定义光标名
		成功返回true，否则返回false；
		***********************************************************/
		bool RegistCursor(const std::string cursorPath, const std::string cursorName);

		/*****************************************************
		函数描述：将给定的光标设为当前光标
		cursorName			in				自定义光标名
		成功返回true，否则返回false；
		******************************************************/
		bool SetCursor(const std::string cursorName);
	private:
		osgViewer::GraphicsWindow* m_GW;
	};
}
