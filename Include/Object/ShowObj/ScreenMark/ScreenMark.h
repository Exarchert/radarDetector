/************************************************************************
Copyright (C)  
文件：ScreenMark.h ScreenMark.cpp
作者：黄志鹏
版本：1.0
日期：2009-12-01
描述：显示对象-屏幕标注类.
					XML格式描述示例
	<屏幕标注>
		<空间位置 X="0" Y="0" Z="0">			</空间位置>
		<文字颜色 R="255" G="0" B="0" A="255">	</文字颜色>
		<文字大小 S="1">						</文字大小>
		<分辨率 WIDTH="128" HEIGHT="128">		</分辨率>
		<背景类型 T="NONE">						</背景类型>
		<背景颜色 R="255" G="0" B="0" A="255">	</背景颜色>
		<原点对齐方式 ALIGN="LEFT_TOP">			</原点对齐方式>
		<字体 FONT="SIMSUN.TTC">				</字体>
		<样式模板 NAME="">						</样式模板>
		<文本内容>
				南朗站
		</文本内容>

		<图片大小 Type="自定义" Width="200" Height="300">	</图片大小>
		<对齐方式 Type="CENTER">							</对齐方式>
		<图片名称>进气站.png								</图片名称>
	</屏幕标注>

	<样式模板>
		<文字颜色 R="255" G="0" B="0" A="255">	</文字颜色>
		<文字大小 S="1">						</文字大小>
		<分辨率 WIDTH="128" HEIGHT="128">		</分辨率>
		<背景类型 T="NONE">						</背景类型>
		<背景颜色 R="255" G="0" B="0" A="255">	</背景颜色>
		<原点对齐方式 ALIGN="LEFT_TOP">			</原点对齐方式>
		<字体 FONT="SIMSUN.TTC">				</字体>
	</样式模板>

修改记录：
作者					日期				描述
----------------------------------------------------------------------

************************************************************************/

#pragma once

#include <map>
#include <Object\Export.h>
#include <Object\Obj.h>
#include <osg\Vec3d>
#include <osg\ref_ptr>
#include <osg\Switch>
#include <osgText\Text>
#include <osg\Geode>
#include <OpenThreads\Mutex>


namespace EMObj		{

class OBJECT_EXPORT CSOScreenMark : public CObj
{
public:
	CSOScreenMark();
	virtual ~CSOScreenMark();
	//初始化
	virtual bool Init(CString &xml);

	//读取样式模板
	void ReadFromTemplate(const CString &szTempName);

	//获取属性信息
	virtual bool ReadProperty(MSXML2::IXMLDOMNodePtr pNode);
	virtual MSXML2::IXMLDOMNodePtr GetProperty();

	//获取企业ID
	virtual int GetEnterID() const;

	//弹出编辑对话框
	virtual bool BeginEditUI();

	//结束编辑
	virtual bool EndEdit3D();

	//刷新绘制
	virtual void UpdateDraw(DWORD Value = 0);

	//点选
	virtual bool PtSelect(double &x, double &y, osg::Vec3d& eye, osg::Vec3d& vec);

	//获取XML描述属性
	virtual CString GetXml();

	//重置设置
	virtual void Reset();

	//设置和获取样式模板名称
	void SetTemplateName(const CString &szTempName);
	CString GetTemplateName() const;

	//设置和获取文字内容
	void SetContent(const CString &szContent);
	CString GetContent() const;

	//设置和获取字体名
	void SetFont(const CString &szFont);
	CString GetFont() const;

	//设置和获取颜色
	void SetColor(const COLORREF &rgb, BYTE alpha);
	void GetColor(COLORREF &rgb, BYTE &alpha) const;

	//设置和获取文字大小
	void SetSize(float size);
	float GetSize() const;

	//设置和获取文字分辨率
	void SetResolution(unsigned int width, unsigned int height);
	void GetResolution(unsigned int &width, unsigned int &height) const;

	//设置和获取背景类型
	void SetBackDropType(unsigned int bkDropType);
	unsigned int GetBackDropType() const;

	//设置和获取背景颜色
	void SetDropColor(const COLORREF &rgb, BYTE alpha);
	void GetDropColor(COLORREF &rgb, BYTE &alpha) const;

	//设置和获取文字的原点对齐方式
	void SetAlignment(unsigned int align);
	unsigned int GetAlignment() const;

	//设置和获取空间位置
	void SetPosition(const osg::Vec3d &pos);
	osg::Vec3d GetPosition();

	void GetPos(osg::Vec3d &pos);

	//设置和获取文本的偏移
	void SetTextOffset(float offsetX, float offsetY);
	void GetTextOffset(float &offsetX, float &offsetY);

	//设置和获取文本的阴影偏移
	void SetDropOffset(float offsetX, float offsetY);
	void GetDropOffset(float &offsetX, float &offsetY);

	enum {
		SOType = 12
	};

	//类型
	virtual DWORD GetType() {return SOType; }

	//渲染类型
	virtual DWORD GetRenderType(){return Render_Type_Screen; }

	//辅助性静态成员函数
	//背景类型查找
	static unsigned int RemapBkType(const CString &name);
	static CString RemapBkType(unsigned int type);
	static std::map<CString, unsigned int> &GetBkTypesMap();

	//原点对齐方式查找
	static unsigned int RemapOriginType(const CString &name);
	static CString RemapOriginType(unsigned int type);
	static std::map<CString, unsigned int> &GetOriginTypesMap();

///////////////////////////图片设置//////////////////////////////////////

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

	void SetPicOffset(float offsetX, float offsetY);
	void GetPicOffset(float &offsetX, float &offsetY);

	//静态辅助成员函数
	static unsigned int RemapAlignType(const CString &name);
	static CString RemapAlignType(unsigned int type);
	static osg::Geometry* CreateImageRectangle(const osg::Image* image, const osg::Vec2* size, unsigned int mode,
							float offsetX, float offsetY);

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

	/*
	目的：生成计算的观察点的位置和姿态(子类可改写之)
	参数：EyeLLA	(out)	经纬高
	      EyeAngle	(out)	姿态
	返回：成功返回true，否则返回false
	*/
	virtual bool GenEyePosAndPose(osg::Vec3d &EyeLLA, osg::Vec3 &EyeAngle);

protected:
	//闪烁控制，可改写该行为
	virtual void FlickPulse(bool bflag);
	virtual void FlickRestore();

protected:
	osg::Vec3d		m_position;		//位置
	//文字
	CString			m_templateName;	//样式模板
	CString			m_szContent;	//文字内容
	CString			m_szFont;		//字体名
	COLORREF		m_rgb;			//颜色
	BYTE			m_alpha;		//透明度
	float			m_size;			//文字大小
	unsigned int	m_width;		//横向分辨率
	unsigned int	m_height;		//纵向分辨率
	unsigned int	m_bkDropType;	//背景类型
	COLORREF		m_bkDropColor;	//背景颜色
	BYTE			m_bkDropAlpha;	//背景透明度
	unsigned int	m_align;		//原点对齐方式
	float			m_textOffsetX;	//屏幕偏移X
	float			m_textOffsetY;	//屏幕偏移Y
	float			m_dropOffsetX;	//阴影偏移X
	float			m_dropOffsetY;	//阴影偏移Y
	
	osg::ref_ptr<osgText::Text>	m_textNode;	//文字结点
	osg::ref_ptr<osg::Switch>	m_switch;

	//图片
	CString			m_szPicture;	//图片名称
	unsigned int	m_sizeType;		//大小类型
	float			m_picWidth;		//宽度
	float			m_picHeight;	//高度
	unsigned int	m_alignType;	//对齐方式
	float			m_picOffsetX;	//屏幕偏移X
	float			m_picOffsetY;	//屏幕偏移Y

	osg::ref_ptr<osg::Geode>	m_pic;		//图片结点

	//辅助性的静态成员数据
	static std::map<CString, unsigned int> s_bkType;		//背景类型
	static std::map<CString, unsigned int> s_originType;	//原点对齐方式
	static std::map<CString, unsigned int> s_alignTypes;	//图片对齐方式

	//互斥量
	OpenThreads::Mutex	m_mutex;
};

}