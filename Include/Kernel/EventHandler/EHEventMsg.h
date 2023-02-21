/*************************************************
file:			CEHEventMsg.h
Author:			张平    
Date:			2009-3-32
Description:    事件消息类，这个类放在所有监EventHandler类的最后
*************************************************/
#pragma once
#include <Kernel\Export.h>
#include <osgGA\GUIEventHandler>

namespace Kernel
{

class KERNEL_EXPORT CEHEventMsg : public osgGA::GUIEventHandler
{
public:
	CEHEventMsg(void);
	~CEHEventMsg(void);
	bool handle(const osgGA::GUIEventAdapter& ea,osgGA::GUIActionAdapter& aa);

	bool RightRelease(const osgGA::GUIEventAdapter& ea,osgGA::GUIActionAdapter& aa);
private:
	COM::CMessage m_mouseMsg;
	bool m_bIsFDown;//记录F键的状态

	bool GetRay(float x, float y, osg::Vec3d& eye, osg::Vec3d& ray);
//
};
}
