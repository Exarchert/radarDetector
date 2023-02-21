#pragma once

#include <vector>
#include <osg/Vec2d>
#include <COM/Export.h>

namespace COM
{

enum WindingOrder
{
	CCW,
	CW
};

//二维多边形
class COM_EXPORT Polygon2D
{
public:
	//构造函数
	Polygon2D(void);
	//根据边界点的链表构造多边形
	Polygon2D(std::vector<osg::Vec2d> lstPoint);
	~Polygon2D(void);

public:
	//获取所有闭合的点的数目
	int count();

	//获取所有open点的数目
	int countOpen();	

	//获取指定索引处的点
	osg::Vec2d getPoint(int index);

public:
	//获取闭合点的链表
	std::vector<osg::Vec2d> getPointList();
	
	//获取open点的链表
	std::vector<osg::Vec2d> getOpenPointList();

private:
	//整理多边形数据
	void cleanUpPolygon(std::vector<osg::Vec2d> lstPoint);

	//删除所有重复的点
	void removeDuplicatePoints();
	
	//检查点数据时逆时针还是顺时针
	void checkWindingOrder();
	
	//把点链表逆序
	void reversePointList();
	
	//计算点链表表示的多边形的面积
	double computeArea(std::vector<osg::Vec2d> lstPoint);
	

private:
	//所有闭合的多边形的点的链表
	std::vector<osg::Vec2d>  m_lstPoint;
	
	//获取所有open的多边形的点的链表
	std::vector<osg::Vec2d>  m_lstOpenPoint;
	
	//获取初始点的链表
	std::vector<osg::Vec2d> m_lstOriginPoint;

};

}