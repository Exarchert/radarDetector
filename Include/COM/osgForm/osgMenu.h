#pragma once
#include <osgWidget/Table>
#include <osgWidget/Label>
#include <osgWidget/Window>
#include <osgWidget/Box>
#include <osg/Referenced>
#include "osgButton.h"
#include "Tree.h"
#include "osgFormUtils.h"

//typedef list<TreeNode*> List;
using namespace osgWidget;
namespace osgForm{
class osgMenu;
class COM_EXPORT osgMenu : public Table
{
	public:
		class  MenuItem : public Window::EmbeddedWindow
		{
			public:
				MenuItem(float w,float h,std::string skinpath,Color color,std::string name);
				~MenuItem();
				void setParent(osgMenu* menu){_parent = menu;}
				osgMenu* getParent(){return _parent;}
				void setOnLabel(std::string label,std::string font,unsigned int fontSize,Color color);
				std::string getButtonName(){return _name;}
				void setIcon(std::string iconpath);
				osgButton* getLast(){return _last;}
				void setButtonMoveCallback(ButtonCallback* callback);
				void setMouseEnter();
				void setMouseLeave();
			private:
				osgMenu*						_parent;
				osg::ref_ptr<Table>			_table;
				osg::ref_ptr<osgButton>		_head;
				osg::ref_ptr<osgButton>		_center;
				osg::ref_ptr<osgButton>		_last;
				std::string					_name;
				
		};
	public:
		osgMenu(float itemh,const std::string& skinPath, Color color,List nodes,ButtonCallback* callback);//,ButtonMouseMoveCallback* movecallback = 0,ButtonReleaseCallBack* releaseCallback = 0);
		~osgMenu(void);
		osgMenu::MenuItem* getMenuButtonFromID(int id);
		void setParent(osgMenu* menu){_parent = menu;}
		void setChild(osgMenu* menu){_child = menu;}
		osgMenu* getParent(){return _parent;}
		osgMenu* getChild(){return _child;}
		void setShowPos(float x,float y){_x = x;_y = y;}
		float getShowPosX(){return _x;}
		float getShowPosY(){return _y;}
	private:
		void creatBounder(float w,float h,std::string skinPath, Color color);
	private:
		vector<MenuItem*>				_vec_menuButton;
		osgMenu*							_parent;
		osgMenu*							_child;
		float							_x,_y;
		//osg::ref_ptr<ButtonMouseMoveCallback>			_movecallback;
		
};
}