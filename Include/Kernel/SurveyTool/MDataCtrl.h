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
class CrenderTool;

class MLineData//线数据类
{
public:
	osg::Vec3 m_begin;
	osg::Vec3 m_end;
	//
	int m_id;
};
class MCircleData//圆数据类
{
public:
	double m_r;
	osg::Vec3 m_Center;
	//
	int m_id;
};
class MPathData//路径数据
{
public:
	MPathData(){_vertex = new osg::Vec3Array;}
	osg::ref_ptr<osg::Vec3Array> _vertex;
	MPathData&operator=(const MPathData&rh)
	{
		*_vertex = *rh._vertex;
		m_id = rh.m_id;
		return (*this);
	}
	//
	int m_id;
};
class MPolyData//多边形数据
{
public:
	MPolyData(){_vertex = new osg::Vec3Array;}
	osg::ref_ptr<osg::Vec3Array> _vertex;
	MPolyData&operator=(const MPolyData&rh)
	{
		*_vertex = *rh._vertex;
		m_id = rh.m_id;
		return (*this);
	}
	//
	int m_id;
};
class MNodeData
{
public:
	osg::ref_ptr<osg::Node> node;
	int m_id;
};
enum TOOL_TYPE{//绘制种类
	LINE_TOOL = 1,//线
	CIRCLE_TOOL,//圆
	POLY_TOOL,//多边形
	PATH_TOOL,//路径
	UNUSE_TOOL,//未使用
};
class CMDataCtrl
{
public:
	CMDataCtrl(void);
	~CMDataCtrl(void);
	friend class CRenderTool;
private:
	//添加线数据
	void addLineData(const MLineData&data);
	//删除线数据
	void delLineData(const MLineData&data);
	//查询线数据
	bool findLineData(MLineData&data);
	//添加圆数据
	void addCircleData(const MCircleData&data);
	//删除圆数据
	void delCircleData(const MCircleData&data);
	//查找圆数据
	bool findCircleData(MCircleData&data);
	//添加路径数据
	void addPathData(const MPathData&data);
	//删除路径数据
	void delPathData(const MPathData&data);
	//查找并返回路径数据
	bool findPathData(MPathData&data);
	//添加多边形数据
	void addPolyData(const MPolyData&data);
	//删除多边形数据
	void delPolyData(const MPolyData&data);
	//查找并返回路径数据
	bool findPolyData(MPolyData&data);
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
	void delOneToolTypeData(TOOL_TYPE type);
	//删除所有数据
	void delAllData();
	//得到指定ID的类型
	TOOL_TYPE getObjectType(int id);
	////得到ID的列表
	std::vector<int> getAllId();
	////得到指定类型的ID列表
	std::vector<int> getTypeIdLst(TOOL_TYPE type);
private:
	std::vector<MLineData>		m_Line;//线数据
	std::vector<MCircleData>	m_Circle;//圆数据
	std::vector<MPathData>		m_Path;//路径数据
	std::vector<MPolyData>		m_Poly;	//多边形，至少三个点
	//场景节点存储
	std::vector<MNodeData>		m_osgData;//场景节点存储
};
