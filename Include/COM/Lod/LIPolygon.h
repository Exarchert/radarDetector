/**************************************************************************
File:			COM_LIPolygon.h
Author:			张平    
Date:			2009-10-28
Description:    多边形区域配置LOD
***************************************************************************/
#pragma once
#include <COM\Lod\LodItem.h>
#include <osg/Array>
#import   "msxml3.dll"

namespace COM{

class COM_EXPORT CLIPolygon : public CLodItem
{
public:
	CLIPolygon(void);
	~CLIPolygon(void);

	enum{
		ConditionType_In,
		ConditionType_Out
	};
	/*
	多边形
	ConditionType		(in)	在内还是在外条件成立
	Pointarrray			(in)	区域数组，按顺序排列
	*/
	bool Init(const osg::Vec2dArray& Pointarrray);

	/*
	通过XML数据进行数据初始化
	*/
	bool Init(MSXML2::IXMLDOMNodePtr pNode);

	bool IsCondition(osg::Vec3d &LLA, osg::Vec3d &Pos, osg::Vec3d &CameraLLA, osg::Vec3d &CameraPos, const LONG64 ObjID = -1);

	virtual DWORD GetLodType()
	{
		return LOD_Polygon;
	}
protected:
	osg::ref_ptr<osg::Vec2Array> m_Pointarrray;
};

}