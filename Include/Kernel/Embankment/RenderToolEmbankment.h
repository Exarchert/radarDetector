#pragma once
/********************************************************************
created:	2213-02-28
file base:	GUIEventHandler
file ext:	h
author:		徐宝龙
purpose:	处理挖填方过程
*********************************************************************/
/*
History:
Data: 2010.3.17
Description: liugh Move Here
*/
#include <osgViewer/ViewerEventHandlers>
#include "MDataCtrlEmbankment.h"
#include "../Kernel/Embankment/EmbankmentDlg.h"


class CRenderToolEmbankment : public osgGA::GUIEventHandler
{
public:
	CRenderToolEmbankment(osg::Group* root);
	~CRenderToolEmbankment(void);
public:

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
	//得到线段信息
	MLineDataEmbankment getLineInfo(int id);
	//得到路径信息
	void getPathInfo(int id,MPathDataEmbankment&mpd);
	//得到圆信息
	MCircleDataEmbankment getCircleInfo(int id);
	//得到多边形信息
	void getPolyInfo(int id,MPolyDataEmbankment&);
	//得到当前对象ID -1标示无操作
	int getCurrentObjId();
	//得到最近完成的对象ID -1标示无操作
	int getRecentObjId();
	//选择一个对象 唯一标示id 
	void selectOneObject(int id,bool bSelected = true);
	//得到对象标示列表
	std::vector<int> getObjectIdLst();
	//得到指定对象类别
	Embankment_TOOL_TYPE getObjectType(int id);
	//传递地球数据指针
	void setEarthNode(osg::ref_ptr<osg::Node>earth);
	
	//更新显示透视盒
	void UpdateBox();

	//
	void setEmbankmentDlg(Kernel::CEmbankmentDlg* pEmbankmentDlg)
	{
	   _pEmbankmentDlg = pEmbankmentDlg;
	}

	//初始绘制 挖填方
	void Init();
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
	目的：改变世界坐标系中点高度
	参数：point      (in/out)  点数据
		  dh	(in)	需要增加的高度
	返回：无
	*/
	void addToHeight(osg::Vec3 &point,double dH);
	/*
	目的：添加绘制到场景中
	参数：	pg	(in)	几何信息
			id  （in）  数据标示
	返回：无
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
	目的：处理鼠标右键键释放
	参数：无
	返回：无
	*/
	void mouseRightButtonRelease();
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
	目的：绘制路径
	参数：	mpd	(in)	MPathDataEmbankment
			cameraHeight(in) 相机高度
			color(in) 颜色
	返回：无
	*/
	void renderPath(MPathDataEmbankment&mpd,double cameraHeight,osg::Vec4 color);
	/*
	目的：绘制线段
	参数：	mld	(in)	MLineDataEmbankment
	cameraHeight(in) 相机高度
	color(in) 颜色
	返回：无
	*/
	void renderLine(MLineDataEmbankment&mld,double cameraHeight,osg::Vec4 color);
		/*
	目的：绘制一个盒子
	参数：	mld	(in)	MLineDataEmbankment
	cameraHeight(in) 相机高度
	返回：无
	*/
	void renderBox(MLineDataEmbankment&mld,double cameraHeight);
	/*
	目的：绘制圆
	参数：	mcd	(in)	MCircleDataEmbankment
	cameraHeight(in) 相机高度
	color(in) 颜色
	返回：无
	*/
	void renderCircle(MCircleDataEmbankment&mcd,double cameraHeight,osg::Vec4 color);
	/*
	目的：绘制多边形
	参数：	mpld	(in)	MPolyDataEmbankment
	cameraHeight(in) 相机高度
	color(in) 颜色
	返回：无
	*/
	void renderPoly(MPolyDataEmbankment&mpld,double cameraHeight,osg::Vec4 color);
	/*
	目的：绘制多边形无顶盒子
	参数：	mpld	(in)	MPolyDataEmbankment
	cameraHeight(in) 相机高度
	返回：无
	*/
	void renderPolyBox(MPolyDataEmbankment&mpld,double cameraHeight);
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

	////创建一个四边形
	osg::Geometry* createQuadrilaterals(osg::Vec3Array* coords);
	osg::Geode* createQuads(osg::Vec3Array* pyramidVertices);
	osg::Geode* createQuads(osg::Vec3Array* pyramidVertices,float precision);
		/*
	目的：创建一个无顶的盒子(计算位置有问题)
	参数：	beginDot(in)	起始点 球心坐标
			endDot(in)      结束点 球心坐标
			with(in)		盒子的快度
			height(in)      盒子的深度
			
	返回：盒子指针
	*/
	osg::Group* createBox(osg::Vec3 beginDot,osg::Vec3 endDot,float with,float height,float yuliu);
		/*
	目的：创建一个无顶的长方体盒子
	参数：	beginDot(in)	起始点 球心坐标
			endDot(in)      结束点 球心坐标
			with(in)		盒子的快度
			height(in)      盒子的深度
			
	返回：盒子指针
	*/
	osg::Group* createBox(osg::Vec3 beginDot,osg::Vec3 endDot,float with,float height);

		/*
	目的：创建一个无顶的多边形柱子（3个面）
	参数：	dotArray(in)	点集
			height(in)      柱子高度
			
	返回：柱子指针
	*/
	osg::Group* createGengonCylinder(osg::Vec3Array* dotArray,float height);

	
	/*
	目的：创建一个土地的纹理
	参数：	
			
	返回：纹理指针
	*/
	osg::StateSet* createDirtStateSet();

	
	/*
	目的：创建一个透明挡板
	参数：	
			
	返回：纹理指针
	*/
	osg::StateSet* createTransparentBaffleStateSet();
	osg::Node* createStencilStateSet(osg::Vec3Array* c,osg::Node* model);

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

	//记录键盘control
	bool m_isCtrl;

	Kernel::CEmbankmentDlg* _pEmbankmentDlg;

};
