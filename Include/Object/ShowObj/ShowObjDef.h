/***************************************************************
file:			ShowObjDef.h
Author:			张平    
Date:			2009-11-13
Description:    对象定义文件
***************************************************************/

#pragma once
#include <Object\ShowObj\MovingStrip\SOMovingLine.h>
#include <Object\ShowObj\MovingLine\SOLine.h>
#include <Object\ShowObj\ScreenImage\SOScreenImage.h>
#include <Object\ShowObj\ScreenText\SOScreenText.h>
#include <Object\ShowObj\ScreenWindow\SOWindow.h>
#include <Object\ShowObj\Model\SOModel.h>
#include <Object\ShowObj\3DText\3DText.h>
#include <Object\ShowObj\3DImage\3DImage.h>
#include <Object\ShowObj\3DMark\3DMark.h>
#include <Object\ShowObj\ScreenMark\ScreenMark.h>
#include <Object\ShowObj\Sphere\Sphere.h>
#include <Object\ShowObj\Area\SOArea.h>
#include <Object\ShowObj\Pipe\SOPipe.h>
#include <Object\ShowObj\Pipe\SORunPipe.h>
#include <Object\ShowObj\LongPipe\SOLongPipe.h>
#include <Object\ShowObj\ScrollText\SOScrollText.h>
#include <Object\ShowObj\XmlLabel\SOXmlLabel.h>
#include <Object\ShowObj\Effect\SOEffect.h>
#include <Object\ShowObj\SphereSpray\SOSphereSpray.h>
#include <map>

namespace EMObj{
	
//根据类型创建显示对象
OBJECT_EXPORT CObj *CreateShowObj(DWORD Type);

//得到选择优先级列表
OBJECT_EXPORT std::map<DWORD, int> *GetObjSelPriorityList();

//重置对象选择的优先级
OBJECT_EXPORT void ResetObjSelPriority();
}
