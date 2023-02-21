/***************************************************************
file:			ShowObjDef.h
Author:			��ƽ    
Date:			2009-11-13
Description:    �������ļ�
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
	
//�������ʹ�����ʾ����
OBJECT_EXPORT CObj *CreateShowObj(DWORD Type);

//�õ�ѡ�����ȼ��б�
OBJECT_EXPORT std::map<DWORD, int> *GetObjSelPriorityList();

//���ö���ѡ������ȼ�
OBJECT_EXPORT void ResetObjSelPriority();
}
