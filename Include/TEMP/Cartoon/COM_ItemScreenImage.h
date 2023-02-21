/**************************************************************************
Copyright (C),  
文件：CCOM_ItemScreenImage.h CCOM_ItemScreenImage.cpp
作者：黄志鹏
版本：1.0
日期：2009-07-29
描述：卡通项-交通工具类

修改记录：
作者						日期					描述
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
		m_Name = _T("未命名屏幕图片");
	}

	virtual ~CCOM_ItemScreenImage();

	CCOM_ItemScreenImage(const CCOM_ItemScreenImage &image);

	CCOM_CartoonItem * Clone();

	/*
	目的：动画单元执行行为
	参数：Time		(in)	相对于开始活动时刻的时刻
		  pos		(in)	动画单位此时的位置			
	返回：void
	*/
	virtual void DoAction(double Time, osg::Vec3d pos);

	//执行非活动时间区的动作
	virtual void DoNoneActive();

	/*
	目的：设置和获取屏幕图像对象信息
	*/
	BOOL GetInformation(CString &imageFile, float &imageWidth, float &imageHeight, DWORD &centerMode, float &screenPosX,
						float &screenPosY, BOOL &bEqualImage, BOOL &looping);

	BOOL SetInformation(CString &imageFile, float imageWidth, float imageHeight, DWORD centerMode, float screenPosX,
						float screenPosY, BOOL bEqualImage, BOOL looping);

	//设置AVI图片的几何体坐标
	static void SetAVIGeometry(COM::CPlayAVI *aviDraw, osg::Vec2 *size, DWORD mode);

	/*
	目的：序列化操作
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
	//场景节点
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