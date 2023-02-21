/*
Copyright (C),  
�ļ���COM_ItemXmlLabel.h  COM_ItemXmlLabel.cpp
���ߣ���־��
�汾��1.0
���ڣ�2009-09-01
�������ռ������--֧��XML��ʽ����

�޸ļ�¼��
����						����						����
-----------------------------------------------------------------------

*/

#pragma once 
#include <Temp\Export.h>
#include <Temp\Cartoon\COM_CartoonItem.h>
#include <osg\ref_ptr>
#include <osg\MatrixTransform>
#include <osg\Vec4>
#include <osgAPEx\TableLabel>

namespace TEMP		{

class TEMP_EXPORT CCOM_ItemXmlLabel : public CCOM_CartoonItem
{
public:
	CCOM_ItemXmlLabel()
	{
		 m_Name = _T("δ����Xml����");
	}

	virtual ~CCOM_ItemXmlLabel();

	CCOM_ItemXmlLabel(const CCOM_ItemXmlLabel &label);

	virtual CCOM_CartoonItem * Clone();

	/*
	Ŀ�ģ�������Ԫִ����Ϊ
	������Time		(in)	����ڿ�ʼ�ʱ�̵�ʱ��
		  pos		(in)	������λ��ʱ��λ��			
	���أ�void
	*/
	virtual void DoAction(double Time, osg::Vec3d pos);

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
	BOOL GetInformation(CString &devName);
	BOOL SetInformation(const CString &devName);

protected:
	DECLARE_SERIAL(CCOM_ItemXmlLabel)

protected:
	CString		m_devName;				//�豸����
};
}