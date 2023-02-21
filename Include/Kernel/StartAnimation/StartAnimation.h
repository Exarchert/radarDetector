//#pragma once
//#include <Kernel/Export.h>
//#include "StartAnimationContext.h"
//
//namespace Kernel
//{
//
//class KERNEL_EXPORT StartAnimation
//{
//public:
//	StartAnimation();
//	~StartAnimation();
//	static StartAnimation* getSingletion();
//	void StartUp(HWND hwnde);
//	void stop();
//private:
//	HANDLE m_startAnimationThread ;	//工作线程句柄
//	osg::ref_ptr<StartAnimationContext> m_StartAnimationContext;
//	static unsigned WINAPI ThreadWorking(void *pdata);
//};
//
//}