/*************************************************
file:			CEHPick.h
Author:			张平    
Date:			2009-11-18
Description:    鼠标拾取类
*************************************************/
#pragma once
#include <Kernel\Export.h>
#include <osgGA\GUIEventHandler>
#include <OpenThreads\Mutex>

namespace Kernel
{

class KERNEL_EXPORT CEHPick : public osgGA::GUIEventHandler
{
public:
	CEHPick(void);
	~CEHPick(void);
	bool handle(const osgGA::GUIEventAdapter& ea,osgGA::GUIActionAdapter& aa);

protected:

	bool GetRay(float x, float y, osg::Vec3d& eye, osg::Vec3d& ray);

	HANDLE	m_hMMFile;
	LPVOID	m_pMMBuffer;
private:
	bool m_ctrl_down;
	bool m_right_down;


};
}
