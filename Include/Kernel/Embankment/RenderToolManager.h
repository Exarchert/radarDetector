#pragma once
/*************************************************
file:			Region.h
Author:			�������
Date:			2013-05-05
Description:    ���湦�ܵ���������¼��ȡ�
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