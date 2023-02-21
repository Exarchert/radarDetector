/********************************************************************************
Copyright (C),  
�ļ���CCOM_ItemRunLine.h  CCOM_ItemRunLine.cpp

�����				2011-1-18			����xml��ʽ�ű�
********************************************************************************/

#pragma once

#include <CartoonObject\Export.h>
#include <CartoonObject\ScriptCreate\Cartoon\COM_Cartoon.h>
#include <osg/Vec4>

namespace CartoonObject	{

class CARTOONOBJECT_EXPORT CCOM_ItemScrollText : public CCOM_CartoonItem
{
public:
	CCOM_ItemScrollText() : m_color(1.f, 0.f, 0.f, 1.f), m_fontName(_T("SIMSUN.TTC")), 
				m_textHeight(25), m_fieldHeight(200), m_lineSpace(5), m_scrollSpeed(1.5)
				, m_maxLines(80), m_alignType(0), m_offsetX(0), m_offsetY(0)
				, m_backColor(0.5,0.5,0.5,0.5), m_hasBack(FALSE)
	{
		m_Name = _T("δ����������Ļ");
	}

	virtual ~CCOM_ItemScrollText();

	CCOM_ItemScrollText(const CCOM_ItemScrollText &text);

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
	Ŀ�ģ���ȡ�����ù�����Ļ��Ϣ
	*/
	BOOL GetInformation(CString &textContent, CString &fontName, osg::Vec4 &color, float &textHeight, float &fieldHeight,
						float &lineSpace, float &scrollSpeed, UINT &maxLines, DWORD &alignType, float &offsetX, float &offsetY,
						osg::Vec4 &backColor, BOOL &hasBack);

	BOOL SetInformation(CString &textContent, CString &fontName, osg::Vec4 &color, float textHeight, float fieldHeight,
						float lineSpace, float scrollSpeed, UINT maxLines, DWORD alignType, float offsetX, float offsetY,
						osg::Vec4 backColor, BOOL hasBack);

protected:
	osg::Vec4	m_color;		//��ɫ
	CString		m_fontName;		//��������
	float		m_textHeight;	//�ı��߶�
	float		m_fieldHeight;	//��������߶�
	float		m_lineSpace;	//�о�
	float		m_scrollSpeed;	//�����ٶ�
	UINT		m_maxLines;		//����������
	DWORD		m_alignType;	//���뷽ʽ	
	float		m_offsetX;		//���뷽ʽ�µ�ƫ��(x,y)
	float		m_offsetY;
	osg::Vec4	m_backColor;	//������ɫ
	BOOL		m_hasBack;		//ӵ�б�����ʶ
	CString		m_textContent;	//�ı�����
};

}