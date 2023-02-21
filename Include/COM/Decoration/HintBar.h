/****************************************************************
Copyright (C),  
作者：黄志鹏
版本：1.0
日期：2009-06-11
描述：利用HUD(Heads Up Display)技术创建一个状态栏的OSG场景图.
      场景图组织如下:
                  Camera
                    /
                  Geode
                  /	   \
             Geometry   Texts

修改记录：
作者                       日期                  描述
-----------------------------------------------------------------
黄志鹏					2009-11-16				移植到该工程, 集成
												显示坐标功能

*****************************************************************/

#pragma once

#include <Com\Export.h>
#include <osg\ref_ptr>
#include <osg\Referenced>
#include <osg\Vec3d>
#include <osg\Switch>
#include <osg\Camera>
#include <osg\Geometry>
#include <osg\Viewport>
#include <osgText\Text>
#include <osgText\Font>
#include <osgViewer\Viewer>
#include <Com\Decoration\DecoraBase.h>


namespace COM	{

class COM_EXPORT CHintBar : public CDecoraBase
{
public:
	CHintBar(osg::Camera *hudCamera, osg::Node *earthNode);
	virtual ~CHintBar();

	enum BarIndex {
		LEFT = 0,
		CENTER,
		RIGHT
	};

	/*
	目的：处理来自OSG消息队列的事件
	*/
	virtual bool handle(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa);

	/*
	目的：控制显隐
	参数：bShow		(in)	true:显示; false:隐藏
	返回：设置前的状态
	*/
	virtual bool Show(bool bShow=true);

	/*
	目的：在状态栏上显示的文本
	参数：which    (in)   指定要设置哪个栏
	      margin   (in)   边距长度，左栏对应是左边距，右栏对应是右边距，中间不起作用
	      str      (str)  文本内容
	返回：成功返回true, 否则返回false
	*/
	bool setText(BarIndex which, int margin, const wchar_t *str);

	/*
	目的：设置状态栏的高度
	参数：nHeight  (in)   状态栏高度
	返回：成功返回true, 否则返回false
	*/
	bool setHeight(int nHeight);

	/*
	目的：获取屏幕鼠标与区域的交点
	参数：camera      (in)       相机指针
	      subGraph    (in)       区域的场景图
	      x, y        (in)       鼠标的视口坐标
	      pos         (out)      交点的世界坐标
		  hasParent	  (in)		 是否有父Transform结点
	返回：成功返回true,否则返回false
	*/
	static bool getTerrainIntersection(osgViewer::Viewer *viewer, osg::Node *subGraph, float x, float y, osg::Vec3d &pos,
										bool hasParent=false);

	/*
	Alter history: LiuGH add function and variable
	Purpose:	Set status bar display mode
	Parameter:	mode	(in)	if 1: degree/minute/second show, else show origin data with log/lat/alt
	Data:		2010/01/27
	*/
	void SetShowMode(int mode);

	/*
	Alter history: LiuGH add function and variable
	Purpose:	Set ini file path
	Parameter:	strIni	(in)	file name
	Data:		2010/01/27
	*/
	void SetIniFileName(CString strIni);

	int m_ShowMode;
	CString m_iniFile;	
protected:
	/*
	目的：初始化状态栏场景
	参数：void
	返回：void
	*/
	void initOsg();

	/*
	调整状态栏的大小
	*/
	void adjustSize();

private:
	osg::ref_ptr<osg::Switch>	m_refOsg;			//指向场景图根节点
	osg::ref_ptr<osgText::Text> m_refTextArray[3];	//3个文本(左中右)
	osg::ref_ptr<osg::Geometry> m_refRect;			//背景矩形
	int m_nLeftMargin;								//左边距
	int m_nRightMargin;								//右边距
	int m_nHeight;									//状态栏的高
	int m_nWidth;									//状态栏的宽

	osg::ref_ptr<osg::Node>		m_earthNode;
	osg::Vec3d					m_position;			//相机的上次位置和姿态
	osg::Vec3d					m_center;			//-------|
	osg::Vec3d					m_up;				//-------|
};

}
