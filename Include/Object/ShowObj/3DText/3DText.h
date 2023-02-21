/************************************************************************
Copyright (C)  
文件：3DText.h 3DText.cpp
作者：黄志鹏
版本：1.0
日期：2009-11-23
描述：显示对象-空间文字类.
						XML格式描述示例
	<空间文字>
		<位置 X="0" Y="0" Z="0">				</位置>
		<姿态 Y="0" P="0" R="0">				</姿态>
		<文字颜色 R="255" G="0" B="0" A="255">	</文字颜色>
		<文字大小 S="1">						</文字大小>
		<分辨率 WIDTH="128" HEIGHT="128">		</分辨率>
		<背景类型 T="NONE">						</背景类型>
		<背景颜色 R="255" G="0" B="0" A="255">	</背景颜色>
		<原点对齐方式 ALIGN="LEFT_TOP">			</原点对齐方式>
		<轴对齐方式 AXIS="SCREEN">				</轴对齐方式>
		<字体 FONT="SIMSUN.TTC">				</字体>
		<文本内容>
			你好, 这是个空间文字的XML描述示例.
			Hello World!
		</文本内容>
	</空间文字>

修改记录：
作者					日期				描述
----------------------------------------------------------------------
黄志鹏					2010-01-22			增加获取位置和姿态的接口(线程安全)

************************************************************************/

#pragma once

#include <Object\Export.h>
#include <Object\Obj.h>
#include <map>
#include <osg\Vec3d>
#include <osg\ref_ptr>
#include <osgText\Text>
#include <OpenThreads\Mutex>


namespace EMObj		{

class OBJECT_EXPORT CSO3DText : virtual public CObj
{
public:
	CSO3DText();
	virtual ~CSO3DText();
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

	//设置和获取轴对齐方式
	void SetAxisAlignment(unsigned int axisAlign);
	unsigned int GetAxisAlignment() const;

	//设置和获取文字位置
	void SetPosition(const osg::Vec3d &pos, const osg::Vec3f &pose);
	void GetPosition(osg::Vec3d &pos, osg::Vec3f &pose);

	//设置和获取阴影偏移
	void SetDropOffset(float offsetX, float offsetY);
	void GetDropOffset(float &offsetX, float &offsetY);

	enum {
		SOType = 5
	};

	//类型
	virtual DWORD GetType() {return SOType; }

	//渲染类型
	virtual DWORD GetRenderType(){return Render_Type_Scene; }

	//辅助性静态成员函数
	//背景类型查找
	static unsigned int RemapBkType(const CString &name);
	static CString RemapBkType(unsigned int type);
	static std::map<CString, unsigned int> &GetBkTypesMap();

	//原点对齐方式查找
	static unsigned int RemapOriginType(const CString &name);
	static CString RemapOriginType(unsigned int type);
	static std::map<CString, unsigned int> &GetOriginTypesMap();

	//轴对齐方式查找
	static unsigned int RemapAxisType(const CString &name);
	static CString RemapAxisType(unsigned int type);
	static std::map<CString, unsigned int> &GetAxisTypesMap();

protected:
	osg::Vec3d		m_position;		//位置
	osg::Vec3f		m_pose;			//姿态
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
	unsigned int	m_axisAlign;	//轴对齐方式
	float			m_dropOffsetX;	//阴影偏移X
	float			m_dropOffsetY;	//阴影偏移Y

	osg::ref_ptr<osgText::Text>	m_textNode;	//文字结点

	//辅助性的静态成员数据
	static std::map<CString, unsigned int> s_bkType;		//背景类型
	static std::map<CString, unsigned int> s_originType;	//原点对齐方式
	static std::map<CString, unsigned int> s_axisType;		//轴对齐方式

	//互斥量
	OpenThreads::Mutex	m_mutex;
};

}
