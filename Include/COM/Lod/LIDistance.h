/**************************************************************************
File:			COM_LIDistance.h
Author:			��ƽ    
Date:			2009-10-28
Description:    ��������LOD
---------------------------------------------------------------------------
Modify
Author:         л��ҫ
Data:           2009-11-27
Description:    ���ӻ�ȡ�������ݵĽӿ�
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
	��ʼ��
	DistanceMin			(in)	��С����
	DistanceMax			(in)	������
	*/
	bool Init(double DistanceMin, double DistanceMax);

	/*
	ͨ��XML���ݽ������ݳ�ʼ��
	*/
	bool Init(MSXML2::IXMLDOMNodePtr pNode);

	bool IsCondition(osg::Vec3d &LLA, osg::Vec3d &Pos, osg::Vec3d &CameraLLA, osg::Vec3d &CameraPos,const LONG64 ObjID = -1);

	/*
	min                 (in)   ��С����
	max			        (in)   ������
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