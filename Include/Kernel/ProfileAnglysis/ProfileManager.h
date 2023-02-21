#pragma once
/*************************************************
file:			Region.h
Author:			�������
Date:			2013-05-05
Description:    ���湦�ܵ���������¼��ȡ�
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
	
	//��ѡ����
	void SwitchRegion(bool swich);
private:
	RenderToolProfileAnalysisHandler* m_pReadToolHandler;
	//COM::CUserCursor* m_pCursor;
};
}