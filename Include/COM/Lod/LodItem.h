/**************************************************************************
File:			CLodItem.h
Author:			��ƽ    
Date:			2009-10-28
Description:    lod���û���
---------------------------------------------------------------------------


***************************************************************************/

#pragma once
#include <COM\Export.h>
#include <Osg/Referenced>
#include <Osg/Vec3d>
#include <COM\XML\XMLNode.h>
namespace COM{

class COM_EXPORT CLodItem : public osg::Referenced, public CXMLNode
{
public:
	CLodItem(void);
	~CLodItem(void);

	/*
	���º���
	LLA				(in)	Ŀ��㾭��
	Pos				(in)	Ŀ���λ��
	CameraLLA		(in)	�����γ��
	CameraPos		(in)	���λ��
	*/
	virtual bool IsCondition(osg::Vec3d &LLA, osg::Vec3d &Pos, osg::Vec3d &CameraLLA, osg::Vec3d &CameraPos, const LONG64 ObjID = -1);

	//�������������������Ƿ�����
	void SetConditionType(bool ConditionType)
	{
		m_ConditionType = ConditionType;
	}
	bool GetConditionType()
	{
		return m_ConditionType;
	}
	
	virtual float GetMax(){return 0;}
	virtual float GetMin(){return 0;}

	enum LODTYPE
	{
		LOD_NONE,
		LOD_Height,
		LOD_Distance,
		LOD_Circle,
		LOD_Polygon,
		LOD_Rect,
	};

	virtual DWORD GetLodType()
	{
		return LOD_NONE;
	}

protected:
	bool m_ConditionType;
};

}

