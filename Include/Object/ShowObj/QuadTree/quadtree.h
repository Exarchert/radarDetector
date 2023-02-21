#pragma once

#include <osg/Group>
#include <osg/MatrixTransform>

#include "Util.h"
#include <Object/Export.h>

namespace EMObj
{
	class PagedLODModel;

	//ÿ���Ĳ������ĸ��ӽڵ��λ��
	enum SubBoxPos
	{
		LEFT_TOP,
		LEFT_BOTTOM,
		RIGHT_TOP,
		RIGHT_BOTTOM
	};									

	//�Ĳ����Ӻ�
	struct QuadSubBox
	{
		//�Ӻе����Ͻڵ�
		osg::Vec3 leftTop;
		//�Ӻе����½ڵ�
		osg::Vec3 rightBottom;

		//�Ӻ��Ƿ����ģ��
		bool containModel(osg::Vec3 center )
		{
			return isInBox(center);
		}

		//�ж����ĵ��Ƿ����Ӻ���
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

	////�Ĳ�����������
	//const int MAX_DEPTH = 8;
	//
	////ÿ���Ĳ������������ģ����Ŀ
	//const int MAX_CHILDREN_NUMBER = 8;
	//
	//const int lodLength = 20; //��Χ��뾶�ı���

	//�Ĳ�����
	class OBJECT_EXPORT QuadTree : public osg::PagedLOD
	{
	public:
		QuadTree(void);
	public:
		~QuadTree(void);

	public:
		//�ݹ麯��������ģ������������Ĳ��������Ͻڵ��Լ����½ڵ㽨���Ĳ���
		void build(QuadTree* quadTree, osg::Vec3 leftTop, osg::Vec3 rightBottom, int depth,int id,  std::map<LONG64,osg::ref_ptr<EMObj::PagedLODModel>> elementInfos);

		//�����Ĳ�����osg����ͼ
		void buildSceneGraph(QuadTree* quadTree);

		void setUpNormal(osg::Vec3 upNormal,float height)
		{
			m_upNormal = upNormal;
			m_upHeight = height;
		}

		void PtSelect(osg::Vec3d& startPos, osg::Vec3d& endPos, std::vector<osg::ref_ptr<EMObj::CObj>>& SelObjs);
	private:
		//�����Ĳ������Ӻ�
		QuadSubBox calculateSubBox(QuadTree* quadTree,SubBoxPos subBoxPos);
		bool QuadTree::psNode(osg::Vec3d pos,osg::Vec3d posEnd,osg::ref_ptr<osg::Node> node,osg::Vec3d& pt);

	private:
		//�Ĳ���������
		osg::Vec3 m_vCenter;

		//�Ĳ���������
		osg::Vec3 m_vLeftTop;

		//�Ĳ���������
		osg::Vec3 m_vRightBottom;

		//�Ĳ���������
		osg::Vec3 m_vLeftBottom;

		//�Ĳ���������
		osg::Vec3 m_vRightTop;

		//�Ĳ����Ƿ���Ҷ�ӽڵ�
		bool isLeaf;

		//�Ĳ��������
		int  m_nDepth;
		int  m_id;

		//���ϵķ�����
		osg::Vec3 m_upNormal;
		float m_upHeight;//���ϵĳ���

		float m_boundSphere;

		std::string m_nodeName;

	public:
		//�Ĳ������ĸ������ڵ�
		QuadTree* m_pLeftTopChild;
		QuadTree* m_pLeftBottomChild;
		QuadTree* m_pRightTopChild;
		QuadTree* m_pRightBottomChild;

	private:
		//�Ĳ���������ģ������
		std::vector<osg::ref_ptr<EMObj::PagedLODModel>> m_vecElements;

		std::vector<osg::Vec3d> m_boxVertexArray;
	};

}
