/********************************************************************************
Copyright (C),  
文件：CCOM_ItemScreenImage.h  CCOM_ItemScreenImage.cpp

刘广慧				2011-1-18			生成xml格式脚本
********************************************************************************/

#pragma once

#include <CartoonObject/Export.h>
#include <CartoonObject/ScriptCreate/Cartoon/COM_CartoonItem.h>


namespace CartoonObject		{

	class CARTOONOBJECT_EXPORT CCOM_ItemScreenImage : public CCOM_CartoonItem
	{
	public:
		CCOM_ItemScreenImage() : m_imageFile(_T("")), m_imageWidth(250), m_imageHeight(300), m_centerMode(1)
			, m_screenPosX(0.f), m_screenPosY(0.f), m_bEqualImage(FALSE), m_eventImage(false)
			, m_looping(TRUE)
		{
			m_Name = _T("未命名屏幕图片");
		}

		virtual ~CCOM_ItemScreenImage();

		CCOM_ItemScreenImage(const CCOM_ItemScreenImage &image);

		CCOM_CartoonItem * Clone();


		/*
		目的：设置和获取屏幕图像对象信息
		*/
		BOOL GetInformation(CString &imageFile, float &imageWidth, float &imageHeight, DWORD &centerMode, float &screenPosX,
			float &screenPosY, BOOL &bEqualImage, BOOL &looping);

		BOOL SetInformation(CString &imageFile, float imageWidth, float imageHeight, DWORD centerMode, float screenPosX,
			float screenPosY, BOOL bEqualImage, BOOL looping);


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
		CString		m_imageFile;
		float		m_imageWidth;
		float		m_imageHeight;
		DWORD		m_centerMode;
		float		m_screenPosX;
		float		m_screenPosY;
		BOOL		m_bEqualImage;
		BOOL		m_looping;

		bool		m_eventImage;
		float		m_left;
		float		m_top;
	};

}