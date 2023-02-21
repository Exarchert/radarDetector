/**************************************************************************
File:			COM_LIRect.h
Author:			��ƽ    
Date:			2009-10-28
Description:    ������������LOD
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
	��ʼ��
	DeX1,DeX2	(in)	�����x������ƫ��������������
	DeY1,DeY2	(in)	�����y������ƫ��������������
	*/
	bool Init(double DeX1, double DeX2, double DeY1, double DeY2);

	/*
	ͨ��XML���ݽ������ݳ�ʼ��
	*/
	bool Init(MSXML2::IXMLDOMNodePtr pNode);

	bool IsCondition(osg::Vec3d &LLA, osg::Vec3d &Pos, osg::Vec3d &CameraLLA, osg::Vec3d &CameraPos, const LONG64 ObjID = -1);

	/*
	��ȡ����ֵ
	dex1,dex2	(in/out)	�����x������ƫ��������������
	dey1,dey2	(in/out)	�����y������ƫ��������������
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