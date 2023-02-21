/**************************************************************************
File:			COM_LIRect.h
Author:			��ƽ    
Date:			2009-10-28
Description:    ����߶�����LOD
---------------------------------------------------------------------------
Modify
Author:         л��ҫ
Date:           2009-11-27
Description:    ���ӻ�ȡ����߶����ԵĽӿ�
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
		HeightType_Absolute,	//���Ը߶ȣ�ָ���θ߶�
		HeightType_Relative		//��Ը߶ȣ�ָ�����Ŀ��ĸ߶�
	};
	/*
	��ʼ��
	HeightMin			(in)	��С����
	HeightMax			(in)	������
	*/
	bool Init(double HeightMin, double HeightMax, DWORD HeightType = HeightType_Absolute);

	/*
	ͨ��XML���ݽ������ݳ�ʼ��
	*/
	bool Init(MSXML2::IXMLDOMNodePtr pNode);

	bool IsCondition(osg::Vec3d &LLA, osg::Vec3d &Pos, osg::Vec3d &CameraLLA, osg::Vec3d &CameraPos, const LONG64 ObjID = -1);

	/*
	�������߶�����
	min                 (in)    ��С����
	max			        (in)	������
	*/
	void GetMinMaxHeight(double& min,double& max);

	/*
	�������߶�����
    type                (in)    �߶�����
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