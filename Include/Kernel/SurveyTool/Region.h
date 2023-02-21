#pragma once
/*************************************************
file:			Region.h
Author:			框选功能管理类
Date:			2013-03-11
Description:    框选功能的命令，处理事件等。
*************************************************/
#include <Kernel/ProfileAnglysis/RenderToolRegion2Handler.h>
#include <osg/Referenced>

namespace Kernel
{
 class  Region :public osg::Referenced
{
public:
	Region(void);
	~Region(void);
	
	//框选开关
	void SwitchRegion(bool swich);
private:
	osg::ref_ptr<RenderToolRegion2Handler> m_pReadToolHandler;
	COM::CUserCursor* m_pCursor;
};
}