/**************************************************************************
File:			CLodItem.h
Author:			��ƽ    
Date:			2009-10-28
Description:    lod���û���
---------------------------------------------------------------------------
Modify
Author:			л��ҫ   
Date:			2009-11-30
Description:    ����LOD�������ݵĸ��½ӿڣ�UpdateData

Date:           2009-12-2
Description:    ����LOD�����е����ĵ������ֶ�
                ���ӽӿ�ReadLodCondition

Date:			2010-05-31
Author:			LiuGH
Description:	�޸�Lod���ͻ�ȡ��������Ҫ���ھ���ģʽʱ������ʾ��������
***************************************************************************/

#pragma once
#include <COM\Lod\LodItem.h>
#include <vector>
#include <osg/ref_ptr>
#import   "msxml3.dll"

namespace COM{

class COM_EXPORT CLod : public osg::Referenced
{
public:
	CLod(void);
	~CLod(void);
	/*
	��ʼ��
	LLA				(in)	��γ��
	PortfolioType	(in)	��Ϸ�ʽ
	Interval		(in)	ʱ���������Ϊ0��ʾû��ʱ����
	*/
	bool Init(osg::Vec3d &LLA, bool PortfolioType, double Interval = 0);

	/*
	ͨ��XML���ݽ������ݳ�ʼ��
	*/
	bool Init(CString strXML);
	bool Init(MSXML2::IXMLDOMNodePtr pParaNode,MSXML2::IXMLDOMNodePtr pLodConNode);


	/*
	���º���
	CameraLLA		(in)	�����γ��
	CameraPos		(in)	���λ�ã�
	����ֵ:0��ʾδ���������жϣ�ִ�е��ǿղ�����1��ʾ���������ж�����Ϊ�棻2��ʾ���������ж�����Ϊ��
	*/
	DWORD Update(double &Time, double &LastTime, osg::Vec3d &CameraLLA, osg::Vec3d &CameraPos);

	void AddItem(CLodItem* Item);
	void DelItem(CLodItem* Item);
	void Clear();

	/*

	*/
	void SetLLA(osg::Vec3d &LLA);
	osg::Vec3d GetLLA();

	//��������
	void SetPos(osg::Vec3d &Pos);
	osg::Vec3d GetPos();
	int	GetPriority(){return m_Priority;}
	void SetPriority(int Priority){m_Priority = Priority;}

	/*
    ��ȡ���ĵ�ģʽ
	return:���ĵ�ģʽ
	*/
	int GetCenterMode();

	/*
	��ȡ������Ϸ�ʽ
	return:������Ϸ�ʽ���ַ���
	*/
	CString GetConditionJudgeType();

	/*
	��ȡˢ��ʱ����
	return : ʱ����
	*/
	double GetUpdateInterval();

    /*
    ����LOD���õ�����
	strXML         (in)     LOD���õ��ַ���
	*/
	void UpdateData(CString strParaXML,CString strContentXML);

	/*
	��ȡLOD�����е����������Ϣ
	pNode          (in)     LOD������ϵĽڵ�
	*/
	void ReadLodCondition(MSXML2::IXMLDOMNodePtr pNode);

	/*
	��ȡLOD�����е����ĵ������Ϣ
	pNode          (in)     LOD���ĵ�Ľڵ�
	*/
	void ReadCenterMode(MSXML2::IXMLDOMNodePtr pNode);

	/*
	��ȡ��ǰLod���ͣ������Distance���ƣ��򷵻�Lod_Distance�����򷵻�Lod_None
	Modify History:
	Add Function Author: LiuGH
	Date:				 2010-06-01
	*/
	DWORD GetLodTypeContainDist();



protected:
	int		    m_centerPointMode;   //���ĵ�ģʽ 1 ��ʾ�������ĵ� 2 ��ʾ�ƶ����ĵ�
	int			m_Priority;			//���ȼ�

	bool		m_PortfolioType;		//������Ϸ�ʽ��true��ʾ�뷽ʽ��ϣ�false��ʾ���������
	double		m_Interval;			//ʱ��������

	osg::Vec3d	m_LLA;
	osg::Vec3d	m_Pos;

	std::vector<osg::ref_ptr<CLodItem>> m_Items;

private:
	double m_DeTime;
	float m_CamraHeight;
	float m_Dist;
};

}