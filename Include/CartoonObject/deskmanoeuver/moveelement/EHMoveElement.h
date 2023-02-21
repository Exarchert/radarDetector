#pragma once
#include <osgGA\GUIEventHandler>
#include <CartoonObject\DeskManoeuver\MoveElement\MoveElement.h>

namespace CartoonObject
{
	class CEHMoveElement : public osgGA::GUIEventHandler
	{
	public:
		CEHMoveElement (void);
		~CEHMoveElement (void);

		virtual bool handle(const osgGA::GUIEventAdapter& ea,osgGA::GUIActionAdapter& aa);
	
		/************************************************************************/
		/* ����˵��                                                             */
		/* pos:	��ǰ������ӿ��е���Ļ����                                      */
		/* type:                                                                */
		/************************************************************************/
	
		virtual bool Start(osg::Vec2 pos,UINT type = 0);

	public:

		void SetHwnd(HWND hWnd){m_hCalledWnd = hWnd;}

		void SetActive(BOOL bActive);

		//��ά�̵߳���
		void RemovePathHandlerAddNode();

	public:
		HWND	m_hCalledWnd;
		BOOL	m_bActive;
	
		std::vector<osg::ref_ptr<CMoveElement>> m_MoveElementVector;
	private:
		//��ά�̵߳���
		void CreateScript();
	};
}
