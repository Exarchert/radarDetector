/********************************************************************************
Copyright (C),  
�ļ���CCOM_ItemFlow.h  CCOM_ItemFlow.cpp

�����				2011-1-18			����xml��ʽ�ű�
********************************************************************************/
#pragma once 

#include <CartoonObject/Export.h>
#include <CartoonObject/ScriptCreate/Cartoon/COM_CartoonItem.h>
#include <osg\ref_ptr>
#include <osg\Vec4>


namespace CartoonObject		{

	class CARTOONOBJECT_EXPORT CCOM_ItemLabel : public CCOM_CartoonItem
	{
	public:
		CCOM_ItemLabel() : m_image(_T("osg64.png")), m_text(_T("")), m_font(_T("SIMSUN.TTC"))
			, m_color(1.f, 0.f, 0.f, 1.f), m_size(30.f), m_offsetX(30.f), m_offsetY(30.f)
		{
			m_Name = _T("δ��������");

		}

		virtual ~CCOM_ItemLabel();

		CCOM_ItemLabel(const CCOM_ItemLabel &label);

		virtual CCOM_CartoonItem * Clone();

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

		/*
		Ŀ�ģ���ȡ������Label��Ϣ
		*/
		BOOL GetInformation(CString &image, CString &text, CString &font, osg::Vec4 &color, float &size, float &offsetX, float &offsetY);
		BOOL SetInformation(CString image, CString text, CString font, osg::Vec4 color, float size, float offsetX, float offsetY);

	protected:
		CString		m_image;				//����ͼƬ
		CString		m_text;					//�ı�����
		CString		m_font;					//�����ļ�
		osg::Vec4	m_color;				//��ɫ
		float		m_size;					//�����С
		float		m_offsetX;				//ƫ��X
		float		m_offsetY;				//ƫ��Y
	};

}
