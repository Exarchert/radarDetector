#pragma once
/********************************************************************
created:	2013-03-6
file base:	GUIEventHandler
file ext:	h
author:		徐宝龙
purpose:	框选
*********************************************************************/

#include <osgViewer/ViewerEventHandlers>
#include <Kernel/Embankment/MDataCtrlEmbankment.h>
#include "ProfileAnalysisDlg.h"


namespace Kernel
{


class RenderToolRegion2Handler : public osgGA::GUIEventHandler
{
public:
	RenderToolRegion2Handler(osg::Group* root);
	~RenderToolRegion2Handler(void);

public:
	/** 重载处理鼠标点击、释放、拖动*/
	bool handle(const osgGA::GUIEventAdapter& ea,osgGA::GUIActionAdapter& aa, osg::Object*, osg::NodeVisitor*);
	//检查是否处于绘制状态
	bool isMeasure();
	//设置测绘开关
	void setMeasureEnable(bool bEnable);
	//清除全部测绘
	void clearAll();
	//清除指定对象
	void delObject(int id);
	//查找并返回osg数据
	osg::ref_ptr<osg::Node> getOsgData(int id);
	//清除指定指定类型对象
	void delObject(Embankment_TOOL_TYPE type);
	//得到当前测绘类型
	Embankment_TOOL_TYPE getToolType();
	//设置当前测绘类型
	void setToolType(Embankment_TOOL_TYPE type);
	//新建绘制 绘制类别toolType 返回标示
	void createNewMeasure(Embankment_TOOL_TYPE toolType);
	//结束当前绘制
	void closeCurrentMeasure();
	//得到多边形信息
	void getPolyInfo(int id,MPolyDataEmbankment&);
	//得到当前对象ID -1标示无操作
	int getCurrentObjId();
	//得到最近完成的对象ID -1标示无操作
	int getRecentObjId();
	//选择一个对象 唯一标示id 
	//void selectOneObject(int id,bool bSelected = true);
	//得到对象标示列表
	std::vector<int> getObjectIdLst();
	//得到指定对象类别
	Embankment_TOOL_TYPE getObjectType(int id);
	//传递地球数据指针
	void setEarthNode(osg::ref_ptr<osg::Node>earth);
private:
	/*
	目的：获取屏幕鼠标与区域的交点
	参数：camera      (in)       相机指针
	subGraph    (in)       区域的场景图
	x, y        (in)       鼠标的视口坐标
	pos         (out)      交点的世界坐标
	返回：成功返回true,否则返回false
	*/
	bool getTerrainIntersection(osg::Camera *camera, osg::Node *subGraph,float x, float y, osg::Vec3 &pos);
	
	/*
	目的：添加绘制到场景中
	参数：	pg	(in)	几何信息
			id  （in）  数据标示
	返回：无	long
	*/
	void addGeometryToOsg(osg::Node* pN,int id);
	/*
	目的：处理鼠标按下事件
	参数：	ea	(in)	鼠标事件
	返回：无
	*/
	void mousePush(const osgGA::GUIEventAdapter& ea);
	/*
	目的：处理鼠标释放事件
	参数：	ea	(in)	鼠标事件
			view(in)    场景信息
	返回：无
	*/
	void mouseRelease(const osgGA::GUIEventAdapter& ea,osgViewer::View *view);
	/*
	目的：处理鼠标左键释放
	参数：	ea	(in)	鼠标事件
	view(in)    场景信息
	返回：无
	*/
	void mouseLeftButtonRelease(const osgGA::GUIEventAdapter& ea,osgViewer::View *view);

	/*
	目的：处理鼠标拖动事件
	参数：	ea	(in)	鼠标事件
			view(in)    场景信息
	返回：无
	*/
	void mouseDrag(const osgGA::GUIEventAdapter& ea,osgViewer::View *view);
	/*
	目的：处理鼠标移动事件
	参数：	ea	(in)	鼠标事件
			view(in)    场景信息
	返回：无
	*/
	void mouseMove(const osgGA::GUIEventAdapter& ea,osgViewer::View *view);
	/*
	求空间一点在指定空间坐标系中的坐标
	pos			(in/out)	输入空间点在世界坐标系中的坐标,输出该点在指定空间坐标系中的坐标
	origin		(in)		指定空间坐标系原点在世界坐标系中的坐标
	x/y/z		(in)		指定坐标系的X/Y/Z轴在世界坐标系的方向
	*/
	void GetPos(osg::Vec3& pos, const osg::Vec3& origin, const osg::Vec3& x, const osg::Vec3& y, const osg::Vec3& z);
	/*
	目的：得到相机高度
	参数：	pCamera	(in)	相机指针
	返回：相机高度
	*/
	double getCameraHeight(osg::Camera*pCamera);
	/*
	目的：绘制多边形
	参数：	mpld	(in)	MPolyDataEmbankment
	cameraHeight(in) 相机高度
	color(in) 颜色
	返回：无			 long
	*/
	void renderPoly(MPolyDataEmbankment&mpld);
	void renderPolyEnd(MPolyDataEmbankment&mpld);
	void renderPolyMove(MPolyDataEmbankment&mpld);
	void readerPolyLineMove(MPolyDataEmbankment&mpld,osg::Vec3 newPoint);
	
	/*
	目的：计算本地矩阵，及x,y,z在世界坐标系的方向
	参数：	pos(in)	点位置
			mat(out) 本地矩阵
			rt(out) x方向
			at(out) y方向
			up(out) z方向
	返回：无
	*/
	void computerMatrixAndXYZ(const osg::Vec3d& pos,osg::Matrix &mat,osg::Vec3 &rt,osg::Vec3 &at,osg::Vec3 &up);

	
	/*
	目的：创建一个透明挡板
	参数：	
			
	返回：纹理指针
	*/
	osg::StateSet* createTransparentBaffleStateSet(osg::Vec4 color);

	bool pointToFace(std::vector<osg::Vec3>& face4,osg::Vec3& point);


	inline osg::Vec3 project2Vector(osg::Vec3& vec2 , osg::Vec3 ori_vec2 ,osg::Vec3 p)
	{
		osg::Vec3f dir = p - ori_vec2;
		double t = (dir*vec2)/(vec2*vec2);
		return ori_vec2 + vec2 *t ;
	}
private://数据
	Embankment_TOOL_TYPE m_toolType;
	CMDataCtrlEmbankment m_data;
	osg::ref_ptr<osg::Node> m_earth;
private://控制
	float _mx;
	float _my;
	//绘制开关
	bool m_bMeasured;
	//选点计数
	int m_iMeasurCount;
	//当前绘制控制标示
	int m_id;
	//记录最近编辑的ID
	int m_ReadlyEdit;
	//相机高度，用于切换绘制方式
	double m_CameraHeight;
	//相机当前高度
	double m_currentCameraHeight;
	//添加的高度
	double m_addHeight;
	//
	osg::Vec3 m_pathVertexMove;
	//绘制颜色
	osg::Vec4 m_renderColor;
	//绘制完成颜色
	osg::Vec4 m_renderdColor;
	//绘制线宽
	int m_lineWith;
	//场景根节点
	osg::Group* m_Root;

};
}