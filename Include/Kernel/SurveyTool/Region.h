#pragma once
/*************************************************
file:			Region.h
Author:			��ѡ���ܹ�����
Date:			2013-03-11
Description:    ��ѡ���ܵ���������¼��ȡ�
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
	
	//��ѡ����
	void SwitchRegion(bool swich);
private:
	osg::ref_ptr<RenderToolRegion2Handler> m_pReadToolHandler;
	COM::CUserCursor* m_pCursor;
};
}