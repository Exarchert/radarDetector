/*************************************************
file:			CmdHandle.h
Author:			张平    
Date:			2010-02-27
Description:    命令解析器基类
*************************************************/

#pragma once
#include <osg/Referenced>
#include <Com\xml\XmlParser.h>

#include "Kernel/Export.h"//
//#define XmlHeader "<?xml version=\"1.0\" encoding=\"GBK\" ?>\n"
//
////using namespace Kernel;
//class KERNEL_EXPORT CCmdHandle : public osg::Referenced


#include "Kernel/AnimationManager/AnimationType.h"
#define XmlHeader "<?xml version=\"1.0\" encoding=\"GBK\" ?>\n"
using namespace Kernel;
class  KERNEL_EXPORT CCmdHandle : public osg::Referenced
{
public:
	CCmdHandle();
	~CCmdHandle();

	//获取解析器的名字
	virtual CString GetName();

	//执行命令
	virtual CString ExecuteCmd(CString Cmd);
protected:
private:
};
