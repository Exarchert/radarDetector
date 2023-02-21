/********************************************************************************
Copyright (C),  
�ļ���CCOM_ItemRunLine.h  CCOM_ItemRunLine.cpp

�����				2011-1-18			����xml��ʽ�ű�
********************************************************************************/

#pragma once

#include <CartoonObject\Export.h>
#include <CartoonObject\ScriptCreate\Cartoon\COM_CartoonItem.h>
#include <osg\ref_ptr>

namespace CartoonObject		{

class CARTOONOBJECT_EXPORT CCOM_ItemWater : public CCOM_CartoonItem
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
	Ŀ�ģ����úͻ�ȡ��Ϣ
	*/
	BOOL GetInformation(CString& imageFile, float& life, float& minNum, float& maxNum, float& minSpeed, float& maxSpeed, 
		float& phi, float& theta, int& type, float& dist)
	{
		imageFile = m_imageFile;
		life = m_lifeTime;
		minNum = m_minRate;
		maxNum = m_maxRate;
		minSpeed = m_minSpeed;
		maxSpeed = m_maxSpeed;
		phi = m_phi;
		theta = m_theta;
		type = m_Type;
		dist = m_Dist;		
		return TRUE;
	}

	BOOL SetInformation(CString imageFile, float life, float minNum, float maxNum, float minSpeed, float maxSpeed, 
		float phi, float theta, int type, float dist)
	{
		m_imageFile = imageFile;
		m_lifeTime = life;
		m_minRate = minNum;
		m_maxRate = maxNum;
		m_minSpeed = minSpeed;
		m_maxSpeed = maxSpeed;
		m_phi = phi;
		m_theta = theta;
		m_Type = type;
		m_Dist = dist;
		return TRUE;
	}

	/*
	Purpose:	Parser XML	Data Format for Animation
	Author:		Liugh
	*/
	virtual bool ParserXml(MSXML2::IXMLDOMNodePtr pNode);

	/*
	Purpose:	Return XML	Node
	Author:		Liugh
	*/
	virtual MSXML2::IXMLDOMNodePtr GetXmlNode();
protected:
	CString	m_imageFile;	//��ͼ
	double	m_lifeTime;		//������������
	float	m_minRate;		//��С������ / ����ڰ뾶
	float	m_maxRate;		//��������� / ��ͼ��
	float	m_minSpeed;		//��С���ٶ� / ��ʼ�ٶ�
	float	m_maxSpeed;		//�����ٶ� / �뾶������
	float	m_phi;			//ָ��ˮƽ��Χ / ��ͼ����(����Ϊ����,����Ϊ����)
	float	m_theta;		//ָ����ֱ��Χ / ��������
	int		m_Type;			//0������ˮ;1��Բ׶������
	float	m_Dist;			//ˮƽ����
};

}