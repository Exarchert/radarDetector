#pragma once
#include <osgWidget/Box>
#include <osgWidget/Table>
#include "osgButton.h"
using namespace osgWidget;
using namespace std;

namespace osgForm{
class COM_EXPORT osgListView : public Table
{
	public:
		struct Item
		{
			std::string label;
			int ID;
		};
		
		class ItemClickCallback : public ButtonCallback
		{
			public:
				ItemClickCallback()
				{
					_parent = NULL;
				}
				void operator()(double x,double y,std::string ID,int type)
				{
					if(!_parent) return;
					int index = atoi(ID.c_str());
					if(type ==1)
					{
						if(_handler)
							_handler->mousePush(index);
					}
					else if(type == 2)
					{
						_parent->hightLight(index);
						if(_handler)
							_handler->mouseRelease(index);
					}
					else if(type ==3)
					{
						if(_handler)
							_handler->mouseEnter(index);
					}
					else if(type ==4)
					{
						if(_handler)
							_handler->mouseLeave(index);
					}
					else if(type ==5)
					{
						if(_handler)
							_handler->mouseDrag(index);
					}
					else if(type ==6)
					{
						if(_handler)
							_handler->mouseOver(index);
					}

				}
				void setParent(osgListView* parent){_parent = parent;}
				void setEventHandler(ListViewHandler* handler){_handler = handler;}
			private:
				osgListView*				_parent;
				ListViewHandler*			_handler;
		};
		
		class ScrollBarCallback : public ButtonCallback
		{
			public:
				ScrollBarCallback(){_parent = NULL;}
				void operator()(double x,double y,std::string ID,int type)
				{
					if(!_parent) return;
					if(type == 2)
					{
						if(ID == "ScrollButtonUp")
							_parent->StratIndexUp();
						else if(ID == "ScrollButtonDown")
							_parent->StratIndexDown();
					}
				}
				void setParent(osgListView* parent){_parent = parent;}
			private:
				osgListView*			_parent;
		};

		class ListItem : public osgButton
		{
			public:
				ListItem(float w,float h,std::string skinpath,Color color,std::string name);
				~ListItem();
				void setIndex(int index){_index = index;}
				int getIndex(){return _index;}
			private:
				int											_index;
		};

	public:
		osgListView(float w,float h,float itemh,std::string skinpath,Color color);
		~osgListView();
		int fillData(vector<Item> data);
		void hightLight(int id);
        void unHightLight(int id	);
        void clearHightLight();
        void setHightLightColor(Color color);
        void setEventHandler(ListViewHandler* handler);
		void reSetItem(int index);
		void StratIndexUp();
        void StratIndexDown();
        void removeAllData();	
        void reFillData(float hh);
        int	 getNumWidgets(){return _Item.size();}
	private:
		void creatWorkSpace(bool);	
	private:
		vector<Item>							_data;
		vector<Widget*>							_Item;
		osg::ref_ptr<EmbeddedWindow>			_workspace,_scrollbar; 
		osg::ref_ptr<ScrollBarCallback>			_scrollbarCallback;
		osg::ref_ptr<Window>					_box;
		float									_itemW;
		float									_itemH;
		float									_curH;
		float									_w,_h;
		string									_skinpath;
		Color									_color;
		Color									_fontColor;
		Color									_itemColor;
		Color									_highLightColor;
		osg::ref_ptr<ItemClickCallback>			_itemClickCallback;
		int										_hightLightIndex;
		int										_startIndex;
};
}