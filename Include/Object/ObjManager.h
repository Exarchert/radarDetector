/*************************************************
file:			COM_ObjectList.h
Author:			张平    
Date:			2009-03-31
Description:    物件列表
*************************************************/
#pragma once
#include <Object/Export.h>
#include <Object/ObjList.h>
#include <osg/Referenced>
#include <Object\ShowObj\ShowObjDef.h>
#include <Object\ShowObj\PagedLODModel\PagedLODModel.h>
//#include <Object\ShowObj\QuadTree\ElementInfo.h>
#include <Object\ShowObj\QuadTree\quadtree.h>


#include <vector>

//#define OUTPUTMODELS

namespace EMObj
{
class CNCEnterAble : public osg::NodeCallback 
{
public:
	CNCEnterAble(std::map<int, osg::ref_ptr<CObjList>>* pObjList) : _lastUpdate(0) {m_pObjList = pObjList;}
	virtual void operator()(osg::Node* node, osg::NodeVisitor* nv);
	int _lastUpdate;
	std::map<int, osg::ref_ptr<CObjList>>* m_pObjList;
};

class OBJECT_EXPORT CObjManager : public osg::Referenced
{

public:
	CObjManager(void);
	~CObjManager(void);

	void Init(osg::Group *SceneNode, osg::Group *HudNode);

	bool LoadObjs(Data::CCenter * DataCenter);

	typedef std::map<LONG64,osg::ref_ptr<EMObj::PagedLODModel>> 	ElementInfoArray;

	/*
	返回对象列表
	ObjList		(out)	对象列表
	RenderType	(in)	渲染类型
	ObjType		(in)	对象类型
	EnterID		(in)	企业ID
	*/
	bool GetObjList(std::vector<osg::ref_ptr<CObj>> &ObjList, DWORD RenderType, DWORD ObjType);
	bool GetObjList(std::vector<osg::ref_ptr<CObj>> &ObjList, DWORD RenderType, DWORD ObjType, int EnterID);
	CObj *GetObj(LONG64 ID);
	void DelObj(LONG64 ID);
	//容易崩溃，待调试
	void DelScreenObj();
	void CelearScreenObj();

	//增加企业节点
	bool AddEnterNode(Data::CRowEnter *pRowEnter);
	//返回企业节点
	CObjList *GetEnterNode(LONG64 EnterID, DWORD RenderType);
	CObjList *GetEnterNode(CObj *pObj);
	CObjList *GetEnterNode();

	//添加新显示对象的接口
	CObj *AddObj(Data::CRowShowObj *pRowShowObj);
	bool AddObj(CObj *pObj);

	//点选
	int PtSelect(double &x, double &y, osg::Vec3d& eye, osg::Vec3d& vec, std::vector<osg::ref_ptr<CObj>>& SelObjs);
	//碰撞检测
	/************************************************************************/
	/* 碰撞检测
	   startArray 检测起点集合，需要和终点一一对应
	   endArray   检测终点集合，需要和起点一一对应
	   collisionDectionArray  碰撞点集合
	   返回 bool 有碰撞返回true 无返回false
	   2013-03-04 xubaolong
	*/
	/************************************************************************/
	int CollisionDetection(std::vector<osg::Vec3d> &startArray,std::vector<osg::Vec3d> &endArray,std::vector<osg::ref_ptr<CObj>>& objs,std::vector<std::vector<osg::Vec3d>> &collisionDectionArray);
	int CollisionDetection(osg::BoundingBox& collisionBox,std::vector<osg::Vec3d> &startArray,std::vector<osg::Vec3d> &endArray,std::vector<osg::ref_ptr<CObj>>& objs, std::vector<std::vector<osg::Vec3d>> &collisionDectionArray);

	/************************************************************************/
	/* 对象状态管理部分代码                                                 */
	/************************************************************************/
	void addScreenObjs(LONG64 nodeID)
	{
		bool b=true;
		std::vector<LONG64>::iterator nodeIter =m_ScreenObjs.begin();
		for (;nodeIter!=m_ScreenObjs.end();nodeIter++)
		{
			if (nodeID == *nodeIter)
			{
				b=false;
			}
		}
		if (b)
		{
			m_ScreenObjs.push_back(nodeID);
		}

	}

	void deleteScreenObjs(LONG64 nodeID)
	{
		std::vector<LONG64>::iterator nodeIter =m_ScreenObjs.begin();
		for (;nodeIter!=m_ScreenObjs.end();nodeIter++)
		{
			if (nodeID == *nodeIter)
			{
				m_ScreenObjs.erase(nodeIter);
				break;
			}
		}

	}

	std::vector<LONG64> getScreenObjs()
	{
		return m_ScreenObjs;
	}

	osg::ref_ptr<QuadTree> getModelQuadTree(int enterID)
	{
		return m_quadTreeList[enterID];
	}

	//osg::Matrixd getModelQuadTreeMx(int enterID)
	//{
	//	return m_modelQuadTreeMx;
	//}


protected:
	std::map<int, osg::ref_ptr<CObjList>> m_mapScreenObjList;
	std::map<int, osg::ref_ptr<CObjList>> m_mapSceneObjList;

	osg::ref_ptr<osg::Group> m_SceneNode, m_HudNode;

	std::vector<LONG64> m_ScreenObjs;

#if defined OUTPUTMODELS
	std::map<__int64, osg::Matrix>	_enterMatrixList;
#endif
	//四叉树数据结构定义、变量声明
private:

	//包围盒的数据结构
	struct EnterBoundBox
	{
		osg::Vec3 minObjXYZ;
		osg::Vec3 maxObjXYZ;
	};

	//根据pShowObj生成m_elementsDataList、m_enterBoundboxList
	int addElementsData(int enterId,osg::ref_ptr<EMObj::PagedLODModel> pShowObj);
	std::map<int,ElementInfoArray> m_elementsDataList;//企业ID，map<模型id,PagedLODModel>
	std::map<int,EnterBoundBox> m_enterBoundboxList;//企业ID，企业id的包围盒

	std::map<int,osg::ref_ptr<QuadTree>> m_quadTreeList;
	
	//osg::ref_ptr<QuadTree> m_modelQuadTree;
	//osg::Matrixd m_modelQuadTreeMx;

	//生成四叉树节点
	int buildQuadTree();
};

//XML读取和保存模块
OBJECT_EXPORT CObjManager *GetObjManager();
}