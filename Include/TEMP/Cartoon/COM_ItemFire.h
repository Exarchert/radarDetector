/**************************************************************************
Copyright (C),  
�ļ���COM_ItemFire.h COM_ItemFire.cpp
���ߣ���־��
�汾��1.0
���ڣ�2010-03-13
��������ͨ��-������

�޸ļ�¼��
����						����					����
---------------------------------------------------------------------------

**************************************************************************/

#pragma once

#include <Temp\Cartoon\COM_CartoonItem.h>
#include <osg\Node>
#include <osg\MatrixTransform>


namespace TEMP		{

class TEMP_EXPORT CCOM_ItemFire : public CCOM_CartoonItem
{
public:
	CCOM_ItemFire()
	{
		m_Name = _T("δ��������");
		m_fireModel = _T("QuadArray.3ds");
		m_flamePic  = _T("Flame.tga");
		m_particleSystemHeight = 25.f;
		m_particleSpeed = 1.3f;
		m_particleSpread = 10.5f;
		m_particleSystemShape = 0.2f;
		m_particleShape = 1.5f;
		m_particleSize = 3.9f;
	}

	virtual ~CCOM_ItemFire();

	CCOM_ItemFire(const CCOM_ItemFire &fire);

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
	Ŀ�ģ����úͻ�ȡ��Ϣ
	*/
	void GetInformation(CString &fireModel, CString &flamePic, float &particleSystemHeight, float &particleSpeed,
		float &particleSpread, float &particleSystemShape, float &particleShape, float &particleSize)
	{
		fireModel = m_fireModel;
		flamePic  = m_flamePic;
		particleSystemHeight = m_particleSystemHeight;
		particleSpeed = m_particleSpeed;
		particleSpread = m_particleSpread;
		particleSystemShape = m_particleSystemShape;
		particleSize = m_particleSize;
	}

	void SetInformation(const CString &fireModel, const CString &flamePic, float particleSystemHeight, float particleSpeed,
		float particleSpread, float particleSystemShape, float particleShape, float particleSize)
	{
		m_fireModel = fireModel;
		m_flamePic  = flamePic;
		m_particleSystemHeight = particleSystemHeight;
		m_particleSpeed = particleSpeed;
		m_particleSpread = particleSpread;
		m_particleSystemShape = particleSystemShape;
		m_particleSize = particleSize;
	}

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
	DECLARE_SERIAL(CCOM_ItemFire)

protected:
	CString		m_fireModel;	//ģ���ļ�������
	CString		m_flamePic;		//��ͼ�ļ�
	float		m_particleSystemHeight; //���Ӹ߶�
	float		m_particleSpeed;		//�����ٶ�
	float		m_particleSpread;		//���ӿ��
	float		m_particleSystemShape;	//�����С
	float		m_particleShape;		//����Ƭ��С
	float		m_particleSize;			//����Ƭ���

	osg::ref_ptr<osg::MatrixTransform> m_mat;
};

}