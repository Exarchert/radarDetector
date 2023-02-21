/*
Copyright (C),  
�ļ���COM_ItemLabel.h  COM_ItemLabel.cpp
���ߣ���־��
�汾��1.0
���ڣ�2009-09-01
�������ռ������

�޸ļ�¼��
����						����						����
-----------------------------------------------------------------------

*/

#pragma once 

#include <temp/Export.h>
#include <temp/Cartoon/COM_CartoonItem.h>
#include <osg\ref_ptr>
#include <osg\Vec4>
#include <osg\MatrixTransform>
#include <osgAPEx\TableLabel>


namespace TEMP		{

class TEMP_EXPORT CCOM_ItemLabel : public CCOM_CartoonItem
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
	Ŀ�ģ���ȡ������Label��Ϣ
	*/
	BOOL GetInformation(CString &image, CString &text, CString &font, osg::Vec4 &color, float &size, float &offsetX, float &offsetY);
	BOOL SetInformation(CString image, CString text, CString font, osg::Vec4 color, float size, float offsetX, float offsetY);

protected:
	DECLARE_SERIAL(CCOM_ItemLabel)

protected:
	osg::ref_ptr<osgAPEx::TableLabel>	m_labelNode;
	CString		m_image;				//����ͼƬ
	CString		m_text;					//�ı�����
	CString		m_font;					//�����ļ�
	osg::Vec4	m_color;				//��ɫ
	float		m_size;					//�����С
	float		m_offsetX;				//ƫ��X
	float		m_offsetY;				//ƫ��Y
};

}
