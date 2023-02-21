/***************************************************************************
Copyright (C),  
文件：CartoonEventHandler.h
作者：谢荣耀
版本：1.0
日期：2009-11-19
描述：接受用户的输入，编辑点位置和姿态等

修改记录：
作者						日期					描述
***************************************************************************/
#pragma once
#include <COM\thread\UIThreadUpdate.h>
#include <osgGA/GUIEventHandler>
#include <osgViewer/Viewer>

namespace CartoonObject
{

class  CCartoonEventHandler: public osgGA::GUIEventHandler
{
public:
	/*
	构造函数
	root          (in)     场景根节点
    terrain       (in)     场景地形节点，可以是厂区，也可以是地球
    dlgInterface  (in)     界面的回调接口
	*/
	CCartoonEventHandler(osg::Group* root,osg::Node* terrain,COM::CUIThreadUpdate* dlgInterface);
	~CCartoonEventHandler(void);

	/*
	处理事件
	ea            (in)     当前的事件类型
	aa            (in)     当前的场景
	return: 处理结束true， 否则返回false
	*/
	virtual bool handle(const osgGA::GUIEventAdapter& ea,osgGA::GUIActionAdapter& aa);

	/*
	获取当前的控制点列表
	pointsList    (in/out)  控制点列表
	*/
	void getControlPointsList(std::vector<osg::Vec3d>& pointsList);

	/*
	更新控制点列表
    updateMap     (in)      当前更新的点的集合
	*/
	void setUpdateControlPoints(std::map<unsigned int,osg::Vec3d> updateMap);

	/*
	设置控制点位置的UI接口
	intefacePtr   (in)      UI接口指针
	*/
	void setPosInterface(COM::CUIThreadUpdate* intefacePtr);

	/*
	设置控制点片段的UI接口
	intefacePtr   (in)      UI接口指针
	*/
	void setPoseInterface(COM::CUIThreadUpdate* intefacePtr);

	/*
	删除所有数据
	*/
	void deleteGeometry();

private:
	/*
	根据当前的拾取点，取得相交得到的位置
	viewer	       (in)	     场景
	ea             (in)      当前鼠标的位置
	pos            (out)     输出的拾取点
	return: 成功返回true，失败返回false
	*/
	bool pick(osgViewer::Viewer* viewer, const osgGA::GUIEventAdapter& ea, osg::Vec3& pos);

	/*
	得到相机高度
	camera	        (in)	 当前的主相机
	subgraph        (in)     执行相交检测的子树节点
    x               (in)     鼠标位置
	y               (in)     鼠标位置
	pos             (in)     与场景相交的位置
	return: 成功返回true，失败返回false
	*/
	bool getTerrainIntersection(osg::Camera* camera, osg::Node* subgraph,float x,float y, osg::Vec3& pos);

	/*
	处理点击事件
	ea              (in)      当前拾取的位置
	*/
	void handlePush(const osgGA::GUIEventAdapter& ea,osgViewer::View* view);

	/*
	处理鼠标移动事件
	ea              (in)      当前拾取的位置
    view            (in)      场景视图
	return: 处理成功返回true，否则返回false
	*/
	bool handleMove(const osgGA::GUIEventAdapter& ea,osgViewer::View* view);

	/*
	得到相机高度
	pCamera	(in)	相机
	return: 相机高度
	*/
	double getCameraHeight(osg::Camera*pCamera);

	/*
	计算本地矩阵，及x,y,z在世界坐标系的方向
	pos     (in)	点位置
	mat     (out)   本地矩阵
	rt      (out)   x方向
	at      (out)   y方向
	up      (out)   z方向
	*/
	void computerMatrixAndXYZ(const osg::Vec3d& pos,osg::Matrix &mat,osg::Vec3 &rt,osg::Vec3 &at,osg::Vec3 &up);

	/*
	添加一个新的控制点
	*/
	void AddPoint();

	/*
	添加一个新片段
	*/
	void addSegment();

	/*
	更新片段
	*/
	void updateSegment(unsigned int index,osg::Vec3d pos);

	/*
	使一个控制点获得焦点
	*/
	void setActivePoint();

	/*
	根据控制点的位置创建一个控制点几何体
	point          (in)   控制点的位置
	*/
	osg::Geometry* createPointGeom(osg::Vec3d point,osg::Vec4 color);

	/*
	根据两个端点创建一个线段几何体
	startPoint     (in)   起始点
	endPoint       (in)   终点
	*/
	osg::Geometry* createSegmentGeom(osg::Vec3d startPoint,osg::Vec3d endPoint);

	/*
	将几何体挂到节点下
	geode          (in)   节点指针
	geom           (in)   几何体指针
    index          (in)   几何体索引
	*/
	void addOrSetGeometry(osg::Geode* geode,osg::Geometry* geom,unsigned int index);

	/*
	检测是否拾取到了线段
	ea              (in)      当前拾取的位置
	view            (in)      场景视图
	return: 如果拾取到有线段，则返回大于0的值，否则返回-1
	*/
	int selectSegment(const osgGA::GUIEventAdapter& ea,osgViewer::View* view);

	/*
	检测当前从眼坐标开始的射线是否与指定的线段相交
    eye             (in)       视点位置，即相机在世界坐标系中的坐标
	ray			    (in)	   射线方向，已单位化
	segment         (in)       相交检测的目标线段
    dis             (out)      线段与目标线段的距离
	rayCross        (out)      射线段上的垂足
	segCross        (out)      目标线段上的垂足
	return :
	0，表示垂足在线段的中间和射线正方向上
	1，平行
	2，表示垂足在线段的中间和射线反方向上
	3，表示垂足在线段的延长线上和射线正方向上
	4，表示垂足在线段的延长线上和射线反方向上
	*/
	int RaySegmentDis(osg::Vec3d eye, osg::Vec3d ray, osgUtil::LineSegmentIntersector segment, double &dis, osg::Vec3d& rayCross, osg::Vec3d& segCross);

	/*
	直线与直线的距离
	line0		(in)	直线0
	line1		(in)	直线1
	dis			(out)	距离
	pQ0			(out)	直线0上的垂足
	pQ1			(out)	直线1上的垂足
	返回值：0，表示非平行；1，表示平行，平行时候垂足无效
	*/
	int LineLineDis(osgUtil::LineSegmentIntersector& line0, osgUtil::LineSegmentIntersector& line1, double &dis, osg::Vec3d *pQ0 = NULL, osg::Vec3d *pQ1 = NULL);


private:
	osg::ref_ptr<osg::Node>						_terrain;
	osg::ref_ptr<osg::Geode>                    _lineGeode;
	osg::ref_ptr<osg::Geode>                    _pointGeode;

	osg::Vec3                                   _prePoint;
	osg::Vec3                                   _mousePoint;
	osg::Geometry*                              _activePointGeom;
	std::vector<osg::Vec3d>                     _controlPointsList;
	std::vector<osg::ref_ptr<osg::Geometry> >   _segmentGeometryList;

	std::map<int,osg::Vec3d>                    _updateMap;//更新列表

	osg::Vec4                                   _pointColor;       //点颜色
	osg::Vec4                                   _activePointColor; //获得焦点的颜色
	osg::Vec4                                   _lineColor;        //线条颜色
	unsigned int                                _lineWidth;        //绘制线宽

	osg::ref_ptr<osg::Group>                    _group;

	bool                                        _isStop;
	bool                                        _isUpdate;
	unsigned int                                _updatePointIndex;

	bool                                        _isDirty;
	bool                                        _delete;

	double                                      _pickSegmentEplison;
	COM::CUIThreadUpdate*                       _trackPosInterface;
	COM::CUIThreadUpdate*                       _trackPoseInterface;
};

}
