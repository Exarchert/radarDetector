#pragma once
#include "osgFormBorder.h"
#include "osgListView.h"
#include "osgFormUtils.h"
namespace osgForm{
class COM_EXPORT osgListViewForm : public osgFormBorder
{
public:
	osgListViewForm(const std::string& title, float w, float h,float itemH,const std::string& skinPath, Color color,unsigned int flags =0,bool haveCtrlBox = true);
	~osgListViewForm(void);
	void fillData(std::vector<osgListView::Item> data);
	void setEventHandler(ListViewHandler* handler);
private:
	void dragBottom();
private:
	osg::ref_ptr<osgListView>					_listView;
	float										_itemH,_h,_w;	
};
}