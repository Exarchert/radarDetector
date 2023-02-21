/*************************************
FileName��	CLayerManager.h CLayerManager.cpp
Author:		Liugh
Date��		2009-12-03
Description��Layer Edit
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
			CString			TypeName;     // ����
		};

		struct Object3D
		{
			BOOL			bVisible;
			LONG64			ObjectID;
			LONG64			ObjTypeID;
			LONG64			EnterID;
			BOOL			bLoaded;
			int				Count;
			CString			ObjectName;     // ����
			std::vector<LONG64> ParentVec;	//���ڴ洢����ά�����Ѽ��ظ��ڵ��¼
		};
	public:
		CLayerManager(void);
		~CLayerManager(void);

		bool InitLayer(Data::CCenter * DataCenter);
		bool InitShowObjType(Data::CCenter* DataCenter);
		bool InitShowObject(Data::CCenter* DataCenter);

		//��Ҫ��չ�Ľӿ�
		//����ͼ������
		void SwitchLayer(LONG64 LayerID, bool isShow);

		//���ͼ��
		void AddLayer(CLayer* pLayer);

		//��ȡͼ���б�
		std::vector<CLayer*> GetLayerList();

		//��ȡͼ�����
		int GetLayerCount();

		//��ȡͼ��Ԫ���б�
		std::vector<CLayerItem*> GetLayerItemList();

		//���ͼ��Ԫ���б�
		void AddLayerItem(CLayerItem* pLayerItem);

	

		std::vector<ObjType>	ObjTypeVec;
		std::map<LONG64,Object3D>		ObjectMap;

		private:
		//ͼ��
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

	//XML��ȡ�ͱ���ģ��
	OBJECT_EXPORT CLayerManager *GetLayerManager();
}

