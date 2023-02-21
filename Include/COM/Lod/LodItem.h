/**************************************************************************
File:			CLodItem.h
Author:			张平    
Date:			2009-10-28
Description:    lod配置基类
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
	更新函数
	LLA				(in)	目标点经度
	Pos				(in)	目标点位置
	CameraLLA		(in)	相机经纬高
	CameraPos		(in)	相机位置
	*/
	virtual bool IsCondition(osg::Vec3d &LLA, osg::Vec3d &Pos, osg::Vec3d &CameraLLA, osg::Vec3d &CameraPos, const LONG64 ObjID = -1);

	//设置条件是正条件还是反条件
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

