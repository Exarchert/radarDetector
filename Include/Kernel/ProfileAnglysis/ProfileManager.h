#pragma once
/*************************************************
file:			Region.h
Author:			剖面分析
Date:			2013-05-05
Description:    剖面功能的命令，处理事件等。
*************************************************/
#include <Kernel/ProfileAnglysis/RenderToolProfileAnalysisHandler.h>
#include <osg/Referenced>

namespace Kernel
{
 class  ProfileManager :public osg::Referenced
{
public:
	ProfileManager(void);
	~ProfileManager(void);
	
	//框选开关
	void SwitchRegion(bool swich);
private:
	RenderToolProfileAnalysisHandler* m_pReadToolHandler;
	//COM::CUserCursor* m_pCursor;
};
}