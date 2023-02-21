/*************************************************
file:			CEHEventMsg.h
Author:			��ƽ    
Date:			2009-3-32
Description:    �¼���Ϣ�࣬�����������м�EventHandler������
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
	bool m_bIsFDown;//��¼F����״̬

	bool GetRay(float x, float y, osg::Vec3d& eye, osg::Vec3d& ray);
//
};
}
