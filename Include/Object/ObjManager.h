/*************************************************
file:			COM_ObjectList.h
Author:			��ƽ    
Date:			2009-03-31
Description:    ����б�
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
	���ض����б�
	ObjList		(out)	�����б�
	RenderType	(in)	��Ⱦ����
	ObjType		(in)	��������
	EnterID		(in)	��ҵID
	*/
	bool GetObjList(std::vector<osg::ref_ptr<CObj>> &ObjList, DWORD RenderType, DWORD ObjType);
	bool GetObjList(std::vector<osg::ref_ptr<CObj>> &ObjList, DWORD RenderType, DWORD ObjType, int EnterID);
	CObj *GetObj(LONG64 ID);
	void DelObj(LONG64 ID);
	//���ױ�����������
	void DelScreenObj();
	void CelearScreenObj();

	//������ҵ�ڵ�
	bool AddEnterNode(Data::CRowEnter *pRowEnter);
	//������ҵ�ڵ�
	CObjList *GetEnterNode(LONG64 EnterID, DWORD RenderType);
	CObjList *GetEnterNode(CObj *pObj);
	CObjList *GetEnterNode();

	//�������ʾ����Ľӿ�
	CObj *AddObj(Data::CRowShowObj *pRowShowObj);
	bool AddObj(CObj *pObj);

	//��ѡ
	int PtSelect(double &x, double &y, osg::Vec3d& eye, osg::Vec3d& vec, std::vector<osg::ref_ptr<CObj>>& SelObjs);
	//��ײ���
	/************************************************************************/
	/* ��ײ���
	   startArray �����㼯�ϣ���Ҫ���յ�һһ��Ӧ
	   endArray   ����յ㼯�ϣ���Ҫ�����һһ��Ӧ
	   collisionDectionArray  ��ײ�㼯��
	   ���� bool ����ײ����true �޷���false
	   2013-03-04 xubaolong
	*/
	/************************************************************************/
	int CollisionDetection(std::vector<osg::Vec3d> &startArray,std::vector<osg::Vec3d> &endArray,std::vector<osg::ref_ptr<CObj>>& objs,std::vector<std::vector<osg::Vec3d>> &collisionDectionArray);
	int CollisionDetection(osg::BoundingBox& collisionBox,std::vector<osg::Vec3d> &startArray,std::vector<osg::Vec3d> &endArray,std::vector<osg::ref_ptr<CObj>>& objs, std::vector<std::vector<osg::Vec3d>> &collisionDectionArray);

	/************************************************************************/
	/* ����״̬�����ִ���                                                 */
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
	//�Ĳ������ݽṹ���塢��������
private:

	//��Χ�е����ݽṹ
	struct EnterBoundBox
	{
		osg::Vec3 minObjXYZ;
		osg::Vec3 maxObjXYZ;
	};

	//����pShowObj����m_elementsDataList��m_enterBoundboxList
	int addElementsData(int enterId,osg::ref_ptr<EMObj::PagedLODModel> pShowObj);
	std::map<int,ElementInfoArray> m_elementsDataList;//��ҵID��map<ģ��id,PagedLODModel>
	std::map<int,EnterBoundBox> m_enterBoundboxList;//��ҵID����ҵid�İ�Χ��

	std::map<int,osg::ref_ptr<QuadTree>> m_quadTreeList;
	
	//osg::ref_ptr<QuadTree> m_modelQuadTree;
	//osg::Matrixd m_modelQuadTreeMx;

	//�����Ĳ����ڵ�
	int buildQuadTree();
};

//XML��ȡ�ͱ���ģ��
OBJECT_EXPORT CObjManager *GetObjManager();
}