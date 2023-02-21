#pragma once
#include <osg/Referenced>
#include <osgGA/GUIEventHandler>
#include <Com/Command/Cmd.h>

namespace Kernel
{
	class ClickMessageHandler :public osgGA::GUIEventHandler
	{
	public:
		ClickMessageHandler();
		~ClickMessageHandler();
	public:
		virtual bool handle(const osgGA::GUIEventAdapter&,osgGA::GUIActionAdapter&, osg::Object*, osg::NodeVisitor*);
	private:
		bool GetRay(float x, float y, osg::Vec3d& eye, osg::Vec3d& ray);
		bool _isDoubleClick;
		COM::CMessage m_mouseMsg;
	};

	class ClickMessageCmd :public COM::CCmdObj
	{
	public:
		ClickMessageCmd(ClickMessageHandler* pRenderTool,bool bOK);
		void DoAction();
		
	private:
		ClickMessageHandler* _pRenderTool;
		bool _bSurveyOK;
	};
	class ClickMessage :public osg::Referenced
	{
	public:
		ClickMessage(void);
		~ClickMessage(void);

		//¿ª¹Ø
		void switchClick(bool swich);
	private:
		osg::ref_ptr<ClickMessageHandler> _clickMessageHandler;
	};

}
