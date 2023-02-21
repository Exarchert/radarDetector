/**************************************************************************
File:			COM_LIDistance.h
Author:			张平    
Date:			2009-10-28
Description:    距离配置LOD
---------------------------------------------------------------------------
Modify
Author:         谢荣耀
Data:           2009-11-27
Description:    增加获取属性数据的接口
***************************************************************************/
#pragma once
#include <COM\Lod\LodItem.h>
#import   "msxml3.dll"

namespace COM{

class COM_EXPORT CLIDistance : public CLodItem
{
public:
	CLIDistance(void);
	~CLIDistance(void);

	/*
	初始化
	DistanceMin			(in)	最小距离
	DistanceMax			(in)	最大距离
	*/
	bool Init(double DistanceMin, double DistanceMax);

	/*
	通过XML数据进行数据初始化
	*/
	bool Init(MSXML2::IXMLDOMNodePtr pNode);

	bool IsCondition(osg::Vec3d &LLA, osg::Vec3d &Pos, osg::Vec3d &CameraLLA, osg::Vec3d &CameraPos,const LONG64 ObjID = -1);

	/*
	min                 (in)   最小距离
	max			        (in)   最大距离
	*/
	void GetMinMaxDistance(double& min,double& max);

	float GetMax(){return m_DistanceMax;}
	float GetMin(){return m_DistanceMin;}

	virtual DWORD GetLodType()
	{
		return LOD_Distance;
	}

protected:
	double m_DistanceMin, m_DistanceMax;
	double m_DistanceMin2, m_DistanceMax2;

};

}