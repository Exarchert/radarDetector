/*********************************************************************
Copyright (C),  
�ļ���COM_ItemScrollText.h COM_ItemScrollText.cpp
���ߣ���־��
�汾��1.0
���ڣ�2009-07-30
��������ͨ��-������Ļ

�޸ļ�¼��
����						����					����
----------------------------------------------------------------------
��־��						2010-02-25				��ֲ���˹���

*********************************************************************/

#pragma once

#include <osg/Vec4>
#include <osg/Group>
#include <osgText/Font>
#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>
#include <osgViewer/api/Win32/GraphicsWindowWin32>
#include <osgAPEx/ScrollText>
#include <temp/Cartoon/COM_CartoonItem.h>


namespace TEMP	{

class TEMP_EXPORT CCOM_ItemScrollText : public CCOM_CartoonItem
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
	Ŀ�ģ���ȡ�����ù�����Ļ��Ϣ
	*/
	BOOL GetInformation(CString &textContent, CString &fontName, osg::Vec4 &color, float &textHeight, float &fieldHeight,
						float &lineSpace, float &scrollSpeed, UINT &maxLines, DWORD &alignType, float &offsetX, float &offsetY,
						osg::Vec4 &backColor, BOOL &hasBack);

	BOOL SetInformation(CString &textContent, CString &fontName, osg::Vec4 &color, float textHeight, float fieldHeight,
						float lineSpace, float scrollSpeed, UINT maxLines, DWORD alignType, float offsetX, float offsetY,
						osg::Vec4 backColor, BOOL hasBack);

protected:
	DECLARE_SERIAL(CCOM_ItemScrollText)

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
	
	//�����ڵ�
	osg::ref_ptr<osg::Group>	m_scene;
};

}