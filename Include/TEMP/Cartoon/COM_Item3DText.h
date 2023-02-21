/*********************************************************************
Copyright (C),  
�ļ���COM_Item3DText.h COM_Item3DText.cpp
���ߣ���־��
�汾��1.0
���ڣ�2009-07-30
��������ͨ��-�ռ�����(������Ļ)

�޸ļ�¼��
����						����					����
----------------------------------------------------------------------

*********************************************************************/

#pragma once

#include <temp/Export.h>
#include <osg/Vec4>
#include <osg/MatrixTransform>
#include <temp/Cartoon/COM_CartoonItem.h>


namespace TEMP		{

class TEMP_EXPORT CCOM_Item3DText : public CCOM_CartoonItem
{
public:
	CCOM_Item3DText() : m_color(1.f, 0.f, 0.f, 1.f), m_font(_T("SIMSUN.TTC")), 
				m_size(30.f), m_width(128), m_height(128), m_bScreen(FALSE), m_screenPosX(0), m_screenPosY(0)
				, m_2Dtext(NULL),m_bBackgrd(FALSE)
	{
		m_Name = _T("δ��������");
		
	}

	virtual ~CCOM_Item3DText();

	CCOM_Item3DText(const CCOM_Item3DText &text);

	virtual CCOM_CartoonItem * Clone();

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


	/*
	Ŀ�ģ���ȡ������3D������Ϣ
	*/
	BOOL GetInformation(CString &textContent, CString &fontName, osg::Vec4 &color, float &size, 
						unsigned int &width, unsigned int &height, BOOL &bScreen, UINT &screenPosX, UINT &screenPosY,
						BOOL &bBackgrd, osg::Vec4 &bkColor);

	BOOL SetInformation(CString &textContent, CString &fontName, osg::Vec4 color, float size, 
						unsigned int width, unsigned int height, BOOL bScreen, UINT screenPosX, UINT screenPosY,
						BOOL bBackgrd, osg::Vec4 bkColor);

protected:
	/*
	Ŀ�ģ�������Ļ����
	*/
	osg::Node * CreateHUD(CString &textContent, CString &fontName, osg::Vec4 color, float size, 
		unsigned int reswidth, unsigned int resheight, osg::Vec3 position, osgText::Text *&outText);

	/*
	Ŀ�ģ�����������Ļ�Ŀռ�����
	*/
	osg::Node * Create3DText(CString &textContent, CString &fontName, osg::Vec4 color, float size, 
		unsigned int reswidth, unsigned int resheight, osg::Vec3 position);

protected:
	DECLARE_SERIAL(CCOM_Item3DText)

protected:
	osg::ref_ptr<osg::MatrixTransform> m_textNode;
	CString		m_text;			//�ı�����
	CString		m_font;			//�����ļ�
	osg::Vec4	m_color;		//��ɫ
	float		m_size;			//�����С
	unsigned int	m_width, m_height;	//�ֱ���
	BOOL		m_bScreen;		//true��ʾ��Ļ����
	UINT		m_screenPosX;
	UINT		m_screenPosY;
	BOOL		m_bBackgrd;		//�Ƿ���ʾ����
	osg::Vec4	m_bkColor;
	osgText::Text *m_2Dtext;	//osgText::Text����ָ��
};

}