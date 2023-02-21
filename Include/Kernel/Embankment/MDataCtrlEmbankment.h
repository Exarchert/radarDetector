#pragma once
/********************************************************************
created:	2009/07/01
file base:	GUIEventHandler
file ext:	h
author:		张攀
purpose:	处理绘制过程类提供数据支持
*********************************************************************/
/*
History:
Data: 2010.3.17
Description: liugh Move Here
*/
#include <osg/Node>
#include <vector>
#include <osg/Geode>
class CRenderToolEmbankment;

class MLineDataEmbankment//线数据类
{
public:
	osg::Vec3 m_begin;
	osg::Vec3 m_end;
	//
	int m_id;
};
class MCircleDataEmbankment//圆数据类
{
public:
	double m_r;
	osg::Vec3 m_Center;
	//
	int m_id;
};
class MPathDataEmbankment//路径数据
{
public:
	MPathDataEmbankment(){_vertex = new osg::Vec3Array;}
	osg::ref_ptr<osg::Vec3Array> _vertex;
	MPathDataEmbankment&operator=(const MPathDataEmbankment&rh)
	{
		*_vertex = *rh._vertex;
		m_id = rh.m_id;
		return (*this);
	}
	//
	int m_id;
};
class MPolyDataEmbankment//多边形数据
{
public:
	MPolyDataEmbankment(){_vertex = new osg::Vec3Array;}
	osg::ref_ptr<osg::Vec3Array> _vertex;
	MPolyDataEmbankment&operator=(const MPolyDataEmbankment&rh)
	{
		*_vertex = *rh._vertex;
		m_id = rh.m_id;
		return (*this);
	}
	//
	int m_id;
};
class MNodeDataEmbankment
{
public:
	osg::ref_ptr<osg::Node> node;
	int m_id;
};
enum Embankment_TOOL_TYPE{//绘制种类
	LINE_TOOL2 = 1,//线
	CIRCLE_TOOL2,//圆
	POLY_TOOL2,//多边形
	PATH_TOOL2,//路径
	UNUSE_TOOL2,//未使用
};
class CMDataCtrlEmbankment
{
public:
	CMDataCtrlEmbankment(void);
	~CMDataCtrlEmbankment(void);
	friend class CRenderToolEmbankment;

	//添加线数据
	void addLineData(const MLineDataEmbankment&data);
	//删除线数据
	void delLineData(const MLineDataEmbankment&data);
	//查询线数据
	bool findLineData(MLineDataEmbankment&data);
	//添加圆数据
	void addCircleData(const MCircleDataEmbankment&data);
	//删除圆数据
	void delCircleData(const MCircleDataEmbankment&data);
	//查找圆数据
	bool findCircleData(MCircleDataEmbankment&data);
	//添加路径数据
	void addPathData(const MPathDataEmbankment&data);
	//删除路径数据
	void delPathData(const MPathDataEmbankment&data);
	//查找并返回路径数据
	bool findPathData(MPathDataEmbankment&data);
	//添加多边形数据
	void addPolyData(const MPolyDataEmbankment&data);
	//删除多边形数据
	void delPolyData(const MPolyDataEmbankment&data);
	//查找并返回路径数据
	bool findPolyData(MPolyDataEmbankment&data);
	//新建唯一标示
	int createNewId();
	//添加osg数据
	void addOsgData(int id,osg::ref_ptr<osg::Node>gnode);
	//删除osg数据
	void delOsgData(int id);
	//查找并返回osg数据
	osg::ref_ptr<osg::Node> findOsgData(int id);
	//删除指定ID的数据
	void delOneIdData(int id);
	//删除指定类型的数据
	void delOneToolTypeData(Embankment_TOOL_TYPE type);
	//删除所有数据
	void delAllData();
	//得到指定ID的类型
	Embankment_TOOL_TYPE getObjectType(int id);
	////得到ID的列表
	std::vector<int> getAllId();
	////得到指定类型的ID列表
	std::vector<int> getTypeIdLst(Embankment_TOOL_TYPE type);
private:
	std::vector<MLineDataEmbankment>		m_Line;//线数据
	std::vector<MCircleDataEmbankment>	m_Circle;//圆数据
	std::vector<MPathDataEmbankment>		m_Path;//路径数据
	std::vector<MPolyDataEmbankment>		m_Poly;	//多边形，至少三个点
	//场景节点存储
	std::vector<MNodeDataEmbankment>		m_osgData;//场景节点存储
};
