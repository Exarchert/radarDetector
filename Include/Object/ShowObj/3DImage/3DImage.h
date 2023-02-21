/************************************************************************
Copyright (C)  
文件：3DImage.h 3DImage.cpp
作者：黄志鹏
版本：1.0
日期：2009-11-25
描述：显示对象-空间图片类.
				XML格式描述示例
	<空间图片>
			<位置 X="0.f" Y="0.f" Z="0.f">						</位置>
			<类型 T="公告板">									</类型>
			<图片大小 Type="自定义" Width="200" Height="300">	</图片大小>
			<对齐方式 Type="CENTER">							</对齐方式>
			<图片名称>进气站.png								</图片名称>
	</空间图片>

修改记录：
作者					日期				描述
----------------------------------------------------------------------

************************************************************************/

#pragma once

#include <Object\Export.h>
#include <Object\Obj.h>
#include <map>
#include <osg\Vec3>
#include <osg\Vec3d>
#include <osg\Billboard>
#include <osg\ImageStream>
#include <osgAPEx\BaseGeo>
#include <OpenThreads\Mutex>


namespace EMObj		{

class OBJECT_EXPORT CSO3DImage : virtual public CObj
{
public:
	CSO3DImage();
	virtual ~CSO3DImage();

	//初始化
	virtual bool Init(CString &xml);

	//获取属性信息
	virtual bool ReadProperty(MSXML2::IXMLDOMNodePtr pNode);
	virtual MSXML2::IXMLDOMNodePtr GetProperty();

	//弹出编辑对话框
	virtual bool BeginEditUI();

	//结束编辑
	virtual bool EndEdit3D();

	//刷新绘制
	virtual void UpdateDraw(DWORD Value = 0);

	//获取XML描述属性
	virtual CString GetXml();

	//重置设置
	virtual void Reset();

	//获取企业ID
	int GetEnterID() const;

	enum SizeType {
		EQUAL_PICTURE,
		USER_DEFINED
	};

	/*
	目的：设置和获取图片名
	参数：szName	(in)	图片名
	返回：无
	*/
	void SetPicture(const CString &szName);
	CString GetPicture() const;

	/*
	目的：设置图片大小
	参数：type		(in)	类型(自定义、等于图片)
	      width		(in)	宽(自定义有效)
	      height	(in)	高(自定义有效)
	返回：无
	*/
	void SetSize(unsigned int type, float width, float height);
	void GetSize(unsigned int &type, float &width, float &height) const;

	/*
	目的：设置对齐(原点)方式
	参数：type		(in)	对齐方式
	返回：无
	*/
	void SetAlignType(unsigned int type);
	unsigned int GetAlignType() const;
	
	//设置和获取位置&姿态
	void SetPosition(const osg::Vec3d &pos, const osg::Vec3f &pose);
	void GetPosition(osg::Vec3d &pos, osg::Vec3f &pose);

	//设置和获取图片的姿态类型
	void SetPoseType(const CString &szType);
	CString GetPoseType() const;

	enum {
		SOType = 6
	};

	//类型
	virtual DWORD GetType() {return SOType; }

	//渲染类型
	virtual DWORD GetRenderType(){return Render_Type_Scene; }

	//静态辅助成员函数
	static unsigned int RemapAlignType(const CString &name);
	static CString RemapAlignType(unsigned int type);
	static osg::Geometry* CreateImageRectangle(const osg::Image* image, const osg::Vec2* size, unsigned int mode, bool bRawSize);

	//原点对齐方式
	enum ALIGNTYPE {
		CENTER,
		BOTTOM_LEFT,
		BOTTOM_RIGHT,
		TOP_LEFT,
		TOP_RIGHT,
		LEFT_CENTER,
		RIGHT_CENTER,
		TOP_CENTER,
		BOTTOM_CENTER
	};

protected:
	osg::Vec3d		m_position;		//位置
	osg::Vec3f		m_pose;			//姿态
	CString			m_szPicture;	//图片名称
	unsigned int	m_sizeType;		//大小类型
	float			m_width;		//宽度
	float			m_height;		//高度
	unsigned int	m_alignType;	//对齐方式
	CString			m_szPoseType;	//姿态类型

	osg::ref_ptr<osg::Billboard>	m_billboard;
	osg::ref_ptr<osg::Geode>		m_geode;
	osg::ref_ptr<osg::ImageStream>	m_imageStream;
	//静态辅助成员数据
	static std::map<CString, unsigned int> s_alignTypes;

	//互斥量
	OpenThreads::Mutex	m_mutex;
};

}