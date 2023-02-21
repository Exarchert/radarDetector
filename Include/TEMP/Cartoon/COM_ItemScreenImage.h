/**************************************************************************
Copyright (C),  
�ļ���CCOM_ItemScreenImage.h CCOM_ItemScreenImage.cpp
���ߣ���־��
�汾��1.0
���ڣ�2009-07-29
��������ͨ��-��ͨ������

�޸ļ�¼��
����						����					����
---------------------------------------------------------------------------

**************************************************************************/

#pragma once

#include <temp/Export.h>
#include <osg/Group>
#include <osg/ImageStream>
#include <Com/PlayAVI/PlayAVI.h>
#include <temp/Cartoon/COM_CartoonItem.h>


namespace TEMP		{

class TEMP_EXPORT CCOM_ItemScreenImage : public CCOM_CartoonItem
{
public:
	CCOM_ItemScreenImage() : m_imageFile(_T("")), m_imageWidth(250), m_imageHeight(300), m_centerMode(1)
							, m_screenPosX(0.f), m_screenPosY(0.f), m_bEqualImage(FALSE), m_eventImage(false)
							, m_looping(TRUE)
	{
		m_Name = _T("δ������ĻͼƬ");
	}

	virtual ~CCOM_ItemScreenImage();

	CCOM_ItemScreenImage(const CCOM_ItemScreenImage &image);

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
	Ŀ�ģ����úͻ�ȡ��Ļͼ�������Ϣ
	*/
	BOOL GetInformation(CString &imageFile, float &imageWidth, float &imageHeight, DWORD &centerMode, float &screenPosX,
						float &screenPosY, BOOL &bEqualImage, BOOL &looping);

	BOOL SetInformation(CString &imageFile, float imageWidth, float imageHeight, DWORD centerMode, float screenPosX,
						float screenPosY, BOOL bEqualImage, BOOL looping);

	//����AVIͼƬ�ļ���������
	static void SetAVIGeometry(COM::CPlayAVI *aviDraw, osg::Vec2 *size, DWORD mode);

	/*
	Ŀ�ģ����л�����
	*/
	virtual void Serialize(CArchive& ar);

	/*
	Purpose:	Parser XML	Data Format for Animation
	Author:		Liugh
	*/
	virtual bool ParserXml(MSXML2::IXMLDOMNodePtr pNode);

	virtual void Play();

	virtual void Pause();

	virtual void Forward(int step);

	virtual void Rewind(int step);

protected:
	DECLARE_SERIAL(CCOM_ItemScreenImage)

protected:
	CString		m_imageFile;
	CString		m_ImageFullPath;
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
	//�����ڵ�
	osg::ref_ptr<osg::Group>	m_node;
	osg::ref_ptr<COM::CPlayAVI>	m_aviDraw;
	osg::ref_ptr<osg::ImageStream> m_imageStream;

private:
	/*
	Author:			Liugh
	Description:	Adjust Flash Control Position by Center Mode
	*/
	bool AdjustFlashPosition(CRect* pRect);
};

}