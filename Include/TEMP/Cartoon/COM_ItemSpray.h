/************************************************************************
Copyright (C),  
�ļ���COM_ItemSpray.h  COM_ItemSpray.cpp
���ߣ���־��
�汾��1.0
���ڣ�2009-08-31
������ˮĻЧ��

�޸ļ�¼��
����						����						����
-------------------------------------------------------------------------

************************************************************************/

#pragma once

#include <Temp/Export.h>
#include <osg/MatrixTransform>
#include <Temp/Cartoon/COM_CartoonItem.h>


namespace TEMP		{

class TEMP_EXPORT CCOM_ItemSpray : public CCOM_CartoonItem
{
public:
	CCOM_ItemSpray() : m_imageFile(_T("")), m_repeatRation(2), m_speed(0.0008f)
						, m_radius(0.f), m_cylinderHeight(0.f), m_sphereHeight(0.f)
	{
		m_Name = _T("δ����ˮĻ");
	}

	virtual ~CCOM_ItemSpray();

	CCOM_ItemSpray(const CCOM_ItemSpray &spray);

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
	BOOL GetInformation(CString &imageFile, float &repeatRation, float &speed, float &radius, 
						float &cylinderHeight, float &sphereHeight)
	{
		imageFile = m_imageFile;
		repeatRation = m_repeatRation;
		speed = m_speed;
		radius = m_radius;
		cylinderHeight = m_cylinderHeight;
		sphereHeight = m_sphereHeight;
		return TRUE;
	}

	BOOL SetInformation(CString imageFile, float repeatRation, float speed, float radius, 
						float cylinderHeight, float sphereHeight)
	{
		m_imageFile = imageFile;
		m_repeatRation = repeatRation;
		m_speed = speed;
		m_radius = radius;
		m_cylinderHeight = cylinderHeight;
		m_sphereHeight = sphereHeight;
		return TRUE;
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
	DECLARE_SERIAL(CCOM_ItemSpray)

protected:
	CString		m_imageFile;		//��ͼ�ļ���
	float		m_repeatRation;		//��ͼ�ظ���
	float		m_speed;			//����(����/֡)
	float		m_radius;			//�뾶
	float		m_cylinderHeight;	//Բ���߶�
	float		m_sphereHeight;		//��߶�

	osg::ref_ptr<osg::MatrixTransform> m_mat;
};

}