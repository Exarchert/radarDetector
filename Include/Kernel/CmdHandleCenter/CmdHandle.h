/*************************************************
file:			CmdHandle.h
Author:			��ƽ    
Date:			2010-02-27
Description:    �������������
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

	//��ȡ������������
	virtual CString GetName();

	//ִ������
	virtual CString ExecuteCmd(CString Cmd);
protected:
private:
};
