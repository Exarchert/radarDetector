#pragma once
#include <osgWidget/Label>
#include <osg\Texture2D>
#include "osgFormUtils.h"

using namespace osgWidget;
namespace osgForm{
class COM_EXPORT osgButton : public Label
{
	public:
		osgButton(float w,float h,std::string skinpath,Color color,std::string ID);
		~osgButton(void);
		virtual bool mousePush (double, double, WindowManager*);
		virtual bool mouseRelease(double, double, WindowManager*);	
		virtual bool mouseEnter(double, double, WindowManager*);
		virtual bool mouseLeave(double, double, WindowManager*);
		virtual bool mouseDrag(double , double , WindowManager*);
		virtual bool mouseOver(double , double , WindowManager*);

		void setOnLabel(std::string ,std::string,unsigned int,Color);
		bool changeImage(int index);
		void insertImage(std::string path);
		void setTips(std::string tip);
		void setButtonCallback(ButtonCallback* callback){_callback = callback;}
	private:
		std::string getNextImagePath(std::string path);
		virtual void positioned ();
	protected:
		float									_w,_h;
		Color									_color;
		std::string								_ID;
		std::vector<osg::ref_ptr<osg::Image>>	_imageList;
		osg::ref_ptr<ButtonCallback>			_callback;
};
}