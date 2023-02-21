#pragma once
#include "com\osgForm\osgFormUtils.h"
#include "HotAreaDataBall.h"
namespace CartoonObject{
class MenuEventHandler : public osgForm::MenuHandler 
{
public:
	MenuEventHandler();
	~MenuEventHandler();
	virtual void itemClick(int id );
	void setHotEventList(std::vector<HotAreaDataBall::HotEvent> events){_events = events;}
private:
	std::vector<HotAreaDataBall::HotEvent>				_events;
};
}