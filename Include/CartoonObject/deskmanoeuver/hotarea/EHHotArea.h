#pragma once
#include <osgGA\GUIEventHandler>
#include <Object\EventDef.h>

namespace CartoonObject
{
	class CEHHotArea : public osgGA::GUIEventHandler
	{
	public:
		CEHHotArea (void);
		~CEHHotArea (void);

		virtual bool handle(const osgGA::GUIEventAdapter& ea,osgGA::GUIActionAdapter& aa);
	
		/*
		参数说明:
		pos:	当前鼠标在视口中的屏幕坐标
		type:
		*/
		virtual bool Start(osg::Vec2 pos,UINT type = 0);

	public:

		void SetHwnd(HWND hWnd){m_hCalledWnd = hWnd;}

	public:
		HWND	m_hCalledWnd;
	};
}
