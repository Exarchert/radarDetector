#pragma once
/*************************************************
file:			AddModelCmd.h
Author:			ƽ̨���ģ��
Date:			2013-09-01
Description:   -=��
*************************************************/
#include <Kernel/SurveyTool/AddModelEvent.h>
#include <COM/Cursor/UserCursor.h>
#include <osg/Referenced>

namespace Kernel
{
 class  AddModelCmd :public osg::Referenced
{
public:
	AddModelCmd(void);
	~AddModelCmd(void);
	
	//�¼�����
	void SwitchEvent(bool swich);
	void SetAddModelFile(std::string modelFile);
	void setDrawPiple();
	void setDelete();
private:
	static AddModelEvent* GetAddModelEvent();
	AddModelEvent* m_addModelHandler;
	COM::CUserCursor* m_pCursor;
};
}