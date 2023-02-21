/**************************************************************************
File:			COM_LIRect.h
Author:			张平    
Date:			2009-10-28
Description:    矩形区域配置LOD
***************************************************************************/

#pragma once
#include <COM\Lod\LodItem.h>
#import   "msxml3.dll"

namespace COM{

class COM_EXPORT CLIRect : public CLodItem
{
public:
	CLIRect(void);
	~CLIRect(void);

	/*
	初始化
	DeX1,DeX2	(in)	相对于x的左右偏移量，都是正数
	DeY1,DeY2	(in)	相对于y的下上偏移量，都是正数
	*/
	bool Init(double DeX1, double DeX2, double DeY1, double DeY2);

	/*
	通过XML数据进行数据初始化
	*/
	bool Init(MSXML2::IXMLDOMNodePtr pNode);

	bool IsCondition(osg::Vec3d &LLA, osg::Vec3d &Pos, osg::Vec3d &CameraLLA, osg::Vec3d &CameraPos, const LONG64 ObjID = -1);

	/*
	获取坐标值
	dex1,dex2	(in/out)	相对于x的左右偏移量，都是正数
	dey1,dey2	(in/out)	相对于y的下上偏移量，都是正数
	*/
	void GetCoordinate(double& dex1,double& dex2,double& dey1,double& dey2);

	virtual DWORD GetLodType()
	{
		return LOD_Rect;
	}

protected:
	double m_DeX1, m_DeX2, m_DeY1, m_DeY2;
};

}