#pragma once
#include <iostream>
#include <osg\Referenced>
namespace osgForm{
struct  COM_EXPORT ButtonCallback : public osg::Referenced 
{
	virtual ~ButtonCallback(){}
	virtual void operator()(double x,double y,std::string ID,int type) = 0;
	//type 
	// 1 = mousePush
	// 2 = mouseRelease
	// 3 = mouseEnter
	// 4 = mouseLeave
	// 5 = mouseDrag
	// 6 = mouseOver
};

struct  COM_EXPORT ResizeCallBack : public osg::Referenced 
{
	virtual ~ResizeCallBack(){}
	virtual void operator()(std::string r) = 0;
};

struct  COM_EXPORT FormHandler : public osg::Referenced
{
	
	virtual void closeButtonClick() = 0;
	virtual void minSizeButtonClick() = 0;
};

struct  COM_EXPORT MenuHandler : public osg::Referenced 
{
public:
	//virtual MenuHandler(){};
	virtual ~MenuHandler(){};
	virtual void itemClick(int id ) = 0;
};

class  COM_EXPORT ListViewHandler: public FormHandler 
{
	public:
		virtual void mousePush(int index) = 0;
		virtual void mouseRelease(int index) = 0;
		virtual void mouseEnter(int index) = 0;
		virtual void mouseLeave(int index) = 0;
		virtual void mouseDrag(int index) = 0;
		virtual void mouseOver(int index) = 0;
		virtual void closeButtonClick() = 0;
		virtual void minSizeButtonClick() = 0;
};

class COM_EXPORT osgFormUtils
{
public:
	osgFormUtils(void);
	~osgFormUtils(void);
	static std::wstring string2wstring(std::string text1);
	static std::string convertIntToTime(int x);
	static std::string convertIntToString(int x);
};
}