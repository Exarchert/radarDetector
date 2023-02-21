#pragma once
#include <osgWidget/Table>
#include "osgFormUtils.h"
#include "osgButton.h"
namespace osgForm{
class ScrollBar : public Table
{
	public:
		ScrollBar(float w,float h,std::string skinpath,Color color);
		~ScrollBar();
		void setButtonCallback(ButtonCallback* callback);
		void destory();
	private:
		osg::ref_ptr<osgButton>				_up;
		osg::ref_ptr<osgButton>				_down;
};	
}