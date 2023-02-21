/***************************************************************************
Copyright (C),  
文件：CreatePoseDlg.h
作者：谢荣耀
版本：1.0
日期：2009-11-20
描述：导出创建片段姿态调整对话框接口

修改记录：
作者						日期					描述
***************************************************************************/
#include <CartoonObject/TrackPoseDlg.h>
#include <CartoonObject/Export.h>

namespace CartoonObject
{

CARTOONOBJECT_EXPORT void*  CreateTrackPoseDlg();

CARTOONOBJECT_EXPORT void   DeleteTrackPoseDlg(void* ptr);

}
