#pragma once
#include <osgGA\GUIEventHandler>
#include <CartoonObject\DeskManoeuver\CharacterRole\Role.h>

namespace CartoonObject
{
	class CEHRole : public osgGA::GUIEventHandler
	{
	public:
		CEHRole (void);
		~CEHRole (void);

		virtual bool handle(const osgGA::GUIEventAdapter& ea,osgGA::GUIActionAdapter& aa);
	
		/**********************************/
		//����˵��:
		//pos:	��ǰ������ӿ��е���Ļ����
		//type:
		/*********************************/
		virtual bool Start(osg::Vec2 pos,UINT type = 0);

	public:

		void SetHwnd(HWND hWnd){m_hCalledWnd = hWnd;}

		void SetActive(BOOL bActive);

		//��ά�е���
		void RemovePathHandlerAddNode();

	public:
		HWND	m_hCalledWnd;
		BOOL	m_bActive;
		std::vector<osg::ref_ptr<CRole>> m_RoleVector;
	private:
		//��ά�е���
		void CreateScript();
	};
}
