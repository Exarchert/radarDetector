#pragma once
/*************************************************
file:			Region.h
Author:			剖面分析
Date:			2013-05-05
Description:    剖面功能的命令，处理事件等。
*************************************************/
#include <Kernel/Embankment/RenderToolEmbankment.h>
#include <COM/Cursor/UserCursor.h>
#include <osg/Referenced>

namespace Kernel
{
 class  RenderToolManager :public osg::Referenced
{
public:
	RenderToolManager(void);
	~RenderToolManager(void);
	
	//
	void SwitchRegion(bool swich);
private:
	CRenderToolEmbankment* m_pReadToolHandler;
	COM::CUserCursor* m_pCursor;
};
}