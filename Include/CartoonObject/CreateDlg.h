/***************************************************************************
Copyright (C),  
文件：CreateDlg.h
作者：谢荣耀
版本：1.0
日期：2009-11-19
描述：导出动画路径控制点对话框创建接口。 

修改记录：
作者						日期					描述
***************************************************************************/
#pragma once
#include <CartoonObject/TrackPosDlg.h>
#include <CartoonObject/Export.h>

namespace CartoonObject
{

CARTOONOBJECT_EXPORT void*  CreateTrackPosDlg();

CARTOONOBJECT_EXPORT void   DeleteTrackPosDlg(void* ptr);

}
