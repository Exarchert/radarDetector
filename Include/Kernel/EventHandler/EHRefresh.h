/*************************************************
file:			CEHRefresh.h
Author:			��ƽ    
Date:			2009-11-18
Description:    ˢ���࣬��������������Ҫˢ�µĶ���
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
