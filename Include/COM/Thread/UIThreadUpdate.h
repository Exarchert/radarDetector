/*********************************************************************
Copyright (C),  
文件：UIThreadUpdate.h UIThreadUpdate.cpp
作者：张平
版本：1.0
日期：2009-11-23
描述：UI线程更新基类

修改记录：
作者					日期				描述
----------------------------------------------------------------------
作者：谢荣耀            
日期：2009-11-23
描述：添加对话框句柄成员以及获取的方法；修改发送消息的接口

作者：谢荣耀            
日期：2009-11-24
描述：修改消息传递方式，本对象不再负责注册窗口消息回调函数，改由每个
      窗口自己通过MFC的消息映射截获消息。本对象只负责发送指定的消息
*********************************************************************/

#pragma once 

#include <Com\Export.h>
namespace COM	{

class COM_EXPORT CUIThreadUpdate
{
public:
	CUIThreadUpdate(void);
	virtual ~CUIThreadUpdate(void);

	//发送更新消息
	void SendUpdateMsg(int msg,WPARAM wParam,LPARAM lParam);

	//更新函数
	virtual void Update();

	//设置窗口句柄
	void SetWndHandle(HWND hWnd);

	//获取窗口句柄
	HWND GetWndHandle();
private:
	HWND m_hChildWnd;
};

}