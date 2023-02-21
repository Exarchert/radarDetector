#pragma once
#include <osgGA\GUIEventHandler>

namespace CartoonObject
{
	class CEHHelpSrc : public osgGA::GUIEventHandler
	{
	public:
		CEHHelpSrc (void);
		~CEHHelpSrc (void);

		virtual bool handle(const osgGA::GUIEventAdapter& ea,osgGA::GUIActionAdapter& aa);
	
		/************************************************************************/
		/*   ����˵��:															*/
		/*pos:	��ǰ������ӿ��е���Ļ����										*/
		/*type:                                                                 */
		/************************************************************************/
		
		virtual bool Start(osg::Vec2 pos,UINT type = 0);

	public:

		void SetHwnd(HWND hWnd){m_hCalledWnd = hWnd;}

		void SetActive(BOOL bActive);

	public:
		HWND	m_hCalledWnd;
		BOOL	m_bActive;
	};
}
