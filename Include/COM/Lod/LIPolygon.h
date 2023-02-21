/**************************************************************************
File:			COM_LIPolygon.h
Author:			��ƽ    
Date:			2009-10-28
Description:    �������������LOD
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
	�����
	ConditionType		(in)	���ڻ���������������
	Pointarrray			(in)	�������飬��˳������
	*/
	bool Init(const osg::Vec2dArray& Pointarrray);

	/*
	ͨ��XML���ݽ������ݳ�ʼ��
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