/*************************************************
file:			CEHRefresh.h
Author:			张平    
Date:			2009-11-18
Description:    刷新类，用来出来所有需要刷新的对象
*************************************************/
#pragma once
#include <Kernel\Export.h>
#include <osgGA\GUIEventHandler>

namespace Kernel
{

class KERNEL_EXPORT CEHRefresh : public osgGA::GUIEventHandler
{
public:
	CEHRefresh(void);
	~CEHRefresh(void);
	bool handle(const osgGA::GUIEventAdapter& ea,osgGA::GUIActionAdapter& aa);

};
}
