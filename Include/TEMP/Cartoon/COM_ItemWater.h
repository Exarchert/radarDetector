/**************************************************************************
Copyright (C),  
�ļ���COM_ItemWater.h COM_ItemWater.cpp
���ߣ���־��
�汾��1.0
���ڣ�2009-07-29
��������ͨ��-��ˮЧ��

�޸ļ�¼��
����						����					����
---------------------------------------------------------------------------

**************************************************************************/

#pragma once

#include <Temp\Export.h>
#include <Temp\Cartoon\COM_CartoonItem.h>
#include <osg\ref_ptr>
#include <osg\MatrixTransform>


namespace TEMP		{

class TEMP_EXPORT CCOM_ItemWater : public CCOM_CartoonItem
{
public:
	CCOM_ItemWater() : m_imageFile(_T("shui.tga")), m_lifeTime(3.5f),
		m_minRate(80.f), m_maxRate(81.f), m_minSpeed(80.f), m_maxSpeed(81.f),
		m_phi(2), m_theta(0.01f),m_Type(0), m_Dist(0.0f)
	{
		m_Name = _T("δ������ˮ");
	}

	virtual ~CCOM_ItemWater();

	CCOM_ItemWater(const CCOM_ItemWater &water);

	CCOM_CartoonItem * Clone();

	/*
	Ŀ�ģ�������Ԫִ����Ϊ
	������Time		(in)	����ڿ�ʼ�ʱ�̵�ʱ��
		  pos		(in)	������λ��ʱ��λ��			
	���أ�void
	*/
	virtual void DoAction(double Time, osg::Vec3d pos);

	//ִ�зǻʱ�����Ķ���
	virtual void DoNoneActive();

	/*
	Ŀ�ģ����л�����
	*/
	virtual void Serialize(CArchive& ar);

	/*
	Purpose:	Parser XML	Data Format for Animation
	Author:		Liugh
	*/
	virtual bool ParserXml(MSXML2::IXMLDOMNodePtr pNode);

protected:
	DECLARE_SERIAL(CCOM_ItemWater)

protected:
	CString	m_imageFile;	//��ͼ
	double	m_lifeTime;		//������������
	float	m_minRate;		//��С������
	float	m_maxRate;		//���������
	float	m_minSpeed;		//��С���ٶ�
	float	m_maxSpeed;		//�����ٶ�
	float	m_phi;			//ָ��ˮƽ��Χ
	float	m_theta;		//ָ����ֱ��Χ
	int		m_Type;
	float	m_Dist;			//ˮƽ����

	osg::ref_ptr<osg::MatrixTransform> m_modelNode;
};

}