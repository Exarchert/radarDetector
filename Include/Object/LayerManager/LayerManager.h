/*************************************
FileName：	CLayerManager.h CLayerManager.cpp
Author:		Liugh
Date：		2009-12-03
Description：Layer Edit
*************************************/

#pragma once
#include <Object\Export.h>
#include <Data\RowStruct\RowEnter.h>
#include <Data\Center.h>

namespace EMObj
{
	class CLayer;
	class CLayerItem;

	class OBJECT_EXPORT CLayerManager : public osg::Referenced
	{
	public:
		enum OperateType
		{
			UNDO,
			EDITED,
			NEW,
			DELETED
		};
		enum ElementType
		{
			LY_LAYER = 0,
			LY_GROUP = 2,
			LY_OBJECT = 4
		};

		BOOL	bSelect;
		BOOL	m_bLoaded;
		LONG64	ID;
		LONG64	LayerID;
		LONG64	ParentID;
		LONG64	Priority;
		LONG64	LodConID;
		CString	m_NodeName;	

		OperateType DbType;
		ElementType Type;




		struct ObjType
		{
			LONG64			TypeID;
			CString			TypeName;     // 名称
		};

		struct Object3D
		{
			BOOL			bVisible;
			LONG64			ObjectID;
			LONG64			ObjTypeID;
			LONG64			EnterID;
			BOOL			bLoaded;
			int				Count;
			CString			ObjectName;     // 名称
			std::vector<LONG64> ParentVec;	//用于存储该三维对象已加载父节点记录
		};
	public:
		CLayerManager(void);
		~CLayerManager(void);

		bool InitLayer(Data::CCenter * DataCenter);
		bool InitShowObjType(Data::CCenter* DataCenter);
		bool InitShowObject(Data::CCenter* DataCenter);

		//需要扩展的接口
		//控制图层显隐
		void SwitchLayer(LONG64 LayerID, bool isShow);

		//添加图层
		void AddLayer(CLayer* pLayer);

		//获取图层列表
		std::vector<CLayer*> GetLayerList();

		//获取图层个数
		int GetLayerCount();

		//获取图层元素列表
		std::vector<CLayerItem*> GetLayerItemList();

		//添加图层元素列表
		void AddLayerItem(CLayerItem* pLayerItem);

	

		std::vector<ObjType>	ObjTypeVec;
		std::map<LONG64,Object3D>		ObjectMap;

		private:
		//图层
		std::vector<CLayer*>		m_LayerVec;
		std::vector<CLayerItem*>	m_LayerItemVec;
	};


	class CLayer : public CLayerManager
	{
	public:
		CLayer(void);
		~CLayer(void);
	};

	class CLayerItem : public CLayerManager
	{
	public:
		CLayerItem(void);
		~CLayerItem(void);

		LONG64	ObjID;
	};

	//XML读取和保存模块
	OBJECT_EXPORT CLayerManager *GetLayerManager();
}

