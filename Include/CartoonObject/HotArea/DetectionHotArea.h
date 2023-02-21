#pragma once
#include <CartoonObject\Export.h>
#include <tinyxml.h>

#include <osgGA\GUIEventHandler>
#include "HotAreaDataBall.h"
#include "com\osgForm\osgPopMenu.h"
#include "com\osgForm\tree.h"
#include "MenuEnevtHandler.h"
namespace CartoonObject{

	class CARTOONOBJECT_EXPORT DetectionHotArea : public osgGA::GUIEventHandler
	{
	public:
		//static DetectionHotArea* GetInstance(void) // 获得类的实例
		//{
		//	if( !m_DetecHotArea )
		//		m_DetecHotArea = new DetectionHotArea();
		//	return m_DetecHotArea;
		//}
		//static osg::ref_ptr<DetectionHotArea> m_DetecHotArea;

		DetectionHotArea(CString string);
		~DetectionHotArea(void);

		void Init();

		virtual bool handle(const osgGA::GUIEventAdapter& ea,osgGA::GUIActionAdapter& aa);
		void end();
	protected:
		void addHotMark(osg::Vec3d pos,osg::Vec3d pose,int id,std::string& name);
		void hideHotMark();
		void removeHotMark();
		void hideHotMark(int id);
		void showHotMark(int id);
		void detection(osg::Vec3d cameraPos);
	private:
		bool											m_bWork;
		CString											m_path;
		std::vector<HotAreaDataBall::HotArea>			_hotAreas;
		Color											_color;
		osg::ref_ptr<osgForm::osgPopMenu>				_menu;
		osg::ref_ptr<MenuEventHandler>					_menuEventHandler;
	};
}