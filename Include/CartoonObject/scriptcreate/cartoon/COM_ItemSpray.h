/********************************************************************************
Copyright (C),  
�ļ���CCOM_ItemRunLine.h  CCOM_ItemRunLine.cpp

�����				2011-1-18			����xml��ʽ�ű�
********************************************************************************/

#pragma once

#include <CartoonObject/Export.h>
#include <CartoonObject/ScriptCreate/Cartoon/COM_CartoonItem.h>


namespace CartoonObject		{

	class CARTOONOBJECT_EXPORT CCOM_ItemSpray : public CCOM_CartoonItem
	{
	public:
		CCOM_ItemSpray() : m_imageFile(_T("shui.tga")), m_repeatRation(5), m_speed(0.01f)
			, m_radius(2.f), m_cylinderHeight(2.f), m_sphereHeight(2.f)
		{
			m_Name = _T("δ����ˮĻ");
		}

		virtual ~CCOM_ItemSpray();

		CCOM_ItemSpray(const CCOM_ItemSpray &spray);

		CCOM_CartoonItem * Clone();

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

		BOOL SetInformation(CString imageFile, float repeatRation, float speedf, float radius, 
			float cylinderHeight, float sphereHeight)
		{
			m_imageFile = imageFile;
			m_repeatRation = repeatRation;
			m_speed = speedf;
			m_radius = radius;
			m_cylinderHeight = cylinderHeight;
			m_sphereHeight = sphereHeight;
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
		CString		m_imageFile;		//��ͼ�ļ���
		float		m_repeatRation;		//��ͼ�ظ���
		float		m_speed;			//����(����/֡)
		float		m_radius;			//�뾶
		float		m_cylinderHeight;	//Բ���߶�
		float		m_sphereHeight;		//��߶�
	};

}