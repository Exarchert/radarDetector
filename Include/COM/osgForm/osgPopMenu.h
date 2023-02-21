#pragma once

#include <osg/Referenced>
#include "osgMenu.h"
#include "Tree.h"
#include "osgButton.h"
#include <osgWidget/WindowManager>

namespace osgForm{
typedef list<TreeNode*> List;

class COM_EXPORT osgPopMenu  : public osg::Referenced 
{
	public:
		class mouseMoveCallBack : public ButtonCallback
		{
			public:
				mouseMoveCallBack()
				{
					_handler= NULL;
				}
				void operator()(double x,double y,std::string ID,int type)
				{
					if(!_popMenu ) return;
					int id = atoi(ID.c_str());
					if(type == 2)
					{
						if(_handler)
							_handler->itemClick(id);
					}
					else if(type == 3)
					{
						_popMenu->creatMenu(id);
					}
					else if(type == 4)
					{
						_popMenu->mouseLeave(id);
					}
				}	
				void setPopMenu(osgPopMenu* popMenu){_popMenu = popMenu;}
				void setHandler(MenuHandler* handler){_handler = handler;}
			private:
				osgPopMenu*		_popMenu;
				MenuHandler*	_handler;
		};

public:
	osgPopMenu(float itemh,const std::string& skinPath, Color color,Tree* tree,WindowManager* wm);
	~osgPopMenu(void);
	void showMenu(float x,float y);
	void creatMenu(int id);
	void mouseLeave(int id);
	void destryMenu();
	void destrySubMenu();
	void setEventHandler(MenuHandler* handler){_menuHandler = handler;}
	//void setReleaseCallBack(ButtonReleaseCallBack* callback){_releasecallback = callback;}
private:
	osgMenu::MenuItem* getItemFromID(int id);
	osgMenu* creatMenu(TreeNode* node,float x,float y);
	void deleteMenu(osgMenu* menu);
private:
	float											_itemH;
	std::string										_skinpaht;
	Color											_color;
	Tree*											_tree;
	osg::ref_ptr<WindowManager>						_wm;
	osg::ref_ptr<mouseMoveCallBack>					_movecallback;
	MenuHandler*									_menuHandler;
	osg::ref_ptr<osgMenu>							_mianMenu;
	osgMenu*										_curMenu;
	vector<osgMenu*>								_vecMenu;
	float											_X;
	float											_Y;
};
}