/*************************************************
file:			CEHPick.h
Author:			��ƽ    
Date:			2009-11-18
Description:    �ȼ�������
*************************************************/
#pragma once
#include <Kernel\Export.h>
#include <osgGA\GUIEventHandler>

namespace Kernel
{

class KERNEL_EXPORT CEHHotKey : public osgGA::GUIEventHandler
{
public:
	CEHHotKey(void);
	~CEHHotKey(void);
	bool handle(const osgGA::GUIEventAdapter& ea,osgGA::GUIActionAdapter& aa);
	
	///*
	//
	//*/
	//bool Init(osg::Vec3d Pos)

};
}
