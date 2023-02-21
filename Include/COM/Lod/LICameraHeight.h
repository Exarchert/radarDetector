/**************************************************************************
File:			COM_LIRect.h
Author:			张平    
Date:			2009-10-28
Description:    相机高度配置LOD
---------------------------------------------------------------------------
Modify
Author:         谢荣耀
Date:           2009-11-27
Description:    增加获取相机高度属性的接口
***************************************************************************/

#pragma once
#include <COM\Lod\LodItem.h>
#import   "msxml3.dll"

namespace COM{

class COM_EXPORT CLICameraHeight : public CLodItem
{
public:
	CLICameraHeight(void);
	~CLICameraHeight(void);

	enum{
		HeightType_Absolute,	//绝对高度，指海拔高度
		HeightType_Relative		//相对高度，指相对于目标的高度
	};
	/*
	初始化
	HeightMin			(in)	最小距离
	HeightMax			(in)	最大距离
	*/
	bool Init(double HeightMin, double HeightMax, DWORD HeightType = HeightType_Absolute);

	/*
	通过XML数据进行数据初始化
	*/
	bool Init(MSXML2::IXMLDOMNodePtr pNode);

	bool IsCondition(osg::Vec3d &LLA, osg::Vec3d &Pos, osg::Vec3d &CameraLLA, osg::Vec3d &CameraPos, const LONG64 ObjID = -1);

	/*
	获得相机高度属性
	min                 (in)    最小距离
	max			        (in)	最大距离
	*/
	void GetMinMaxHeight(double& min,double& max);

	/*
	获得相机高度类型
    type                (in)    高度类型
	*/
	void GetHeightType(unsigned int& type);

	float GetMax(){return m_HeightMax;}
	float GetMin(){return m_HeightMin;}

	virtual DWORD GetLodType()
	{
		return LOD_Height;
	}

protected:
	double m_HeightMin, m_HeightMax;
	DWORD m_HeightType;
};

}