#pragma once

#include <osg/Group>
#include <osg/MatrixTransform>

#include "Util.h"
#include <Object/Export.h>

namespace EMObj
{
	class PagedLODModel;

	//每个四叉树的四个子节点的位置
	enum SubBoxPos
	{
		LEFT_TOP,
		LEFT_BOTTOM,
		RIGHT_TOP,
		RIGHT_BOTTOM
	};									

	//四叉树子盒
	struct QuadSubBox
	{
		//子盒的左上节点
		osg::Vec3 leftTop;
		//子盒的右下节点
		osg::Vec3 rightBottom;

		//子盒是否包含模型
		bool containModel(osg::Vec3 center )
		{
			return isInBox(center);
		}

		//判断中心点是否在子盒内
		bool isInBox(osg::Vec3 vCenter)
		{
			float x,y,z;
			x = vCenter.x();
			y = vCenter.y();
			z = vCenter.z();

			if(!Util::isInRange(leftTop.x(), rightBottom.x(), x))
			{
				return false;		
			}

			if(!Util::isInRange(leftTop.y(), rightBottom.y(), y))
			{
				return false;		
			}

			/*if(!isInRange(leftTop.z(), rightBottom.z(), z))
			{
			return false;		
			}*/

			return true;			
		}

	};

	////四叉树的最大深度
	//const int MAX_DEPTH = 8;
	//
	////每个四叉树包含的最大模型数目
	//const int MAX_CHILDREN_NUMBER = 8;
	//
	//const int lodLength = 20; //包围球半径的倍数

	//四叉树类
	class OBJECT_EXPORT QuadTree : public osg::PagedLOD
	{
	public:
		QuadTree(void);
	public:
		~QuadTree(void);

	public:
		//递归函数，根据模型数据数组和四叉树的左上节点以及右下节点建立四叉树
		void build(QuadTree* quadTree, osg::Vec3 leftTop, osg::Vec3 rightBottom, int depth,int id,  std::map<LONG64,osg::ref_ptr<EMObj::PagedLODModel>> elementInfos);

		//建立四叉树的osg场景图
		void buildSceneGraph(QuadTree* quadTree);

		void setUpNormal(osg::Vec3 upNormal,float height)
		{
			m_upNormal = upNormal;
			m_upHeight = height;
		}

		void PtSelect(osg::Vec3d& startPos, osg::Vec3d& endPos, std::vector<osg::ref_ptr<EMObj::CObj>>& SelObjs);
	private:
		//计算四叉树的子盒
		QuadSubBox calculateSubBox(QuadTree* quadTree,SubBoxPos subBoxPos);
		bool QuadTree::psNode(osg::Vec3d pos,osg::Vec3d posEnd,osg::ref_ptr<osg::Node> node,osg::Vec3d& pt);

	private:
		//四叉树的中心
		osg::Vec3 m_vCenter;

		//四叉树的左上
		osg::Vec3 m_vLeftTop;

		//四叉树的右下
		osg::Vec3 m_vRightBottom;

		//四叉树的左下
		osg::Vec3 m_vLeftBottom;

		//四叉树的右下
		osg::Vec3 m_vRightTop;

		//四叉树是否是叶子节点
		bool isLeaf;

		//四叉树的深度
		int  m_nDepth;
		int  m_id;

		//向上的法向量
		osg::Vec3 m_upNormal;
		float m_upHeight;//向上的长度

		float m_boundSphere;

		std::string m_nodeName;

	public:
		//四叉树的四个子树节点
		QuadTree* m_pLeftTopChild;
		QuadTree* m_pLeftBottomChild;
		QuadTree* m_pRightTopChild;
		QuadTree* m_pRightBottomChild;

	private:
		//四叉树的所有模型数据
		std::vector<osg::ref_ptr<EMObj::PagedLODModel>> m_vecElements;

		std::vector<osg::Vec3d> m_boxVertexArray;
	};

}
