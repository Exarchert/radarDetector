/*************************************************************************
FileName：CSOScrollText.h CSOScrollText.cpp
Author:	Liugh
Date：2009-10-30
Description：Screen Text
**************************************************************************/
#pragma once
#include <Object/Export.h>
#include <Object/Obj.h>
#include <osg/Node>
#include <osgText/Text>
#include <Object/ObjList.h>
#include <Object/ObjManager.h>
#include <osgAPEx/ScrollText>
#include <Object/ShowObj/ScreenWindow/SOWindow.h>

namespace EMObj{

class OBJECT_EXPORT CSOScrollText : public EMObj::CObj
{
public:
	CSOScrollText(void);
	~CSOScrollText(void);

	enum{
		SOType = 16
	};

public:
	int			_wndId;			//窗口id
	float		_textSize;	//文字高度
	osg::Vec4	_textColor;		//颜色
	CString		_textContent;	//标注内容
	int			_fontIndex;		//字体选择索引
	CString		_textFont;		//字体
	int			_Align;			//对齐方式
	float		_lineSpace;		//行间距
	float		_speed;			//滚动速度

	osg::ref_ptr<osgText::Text> _Text;		//文字指针

	osg::ref_ptr<osgAPEx::ScrollText> m_ScrollText;

	float _wndLeft,_wndRight,_wndTop,_wndBottom,_offX,_offY,_columnNum,_lineNum;
	osg::Vec3 _textPos;
	float _textWide,_textHeight;

	CSOWindow* m_pWnd;
	bool m_bCanceled;

	/*
	Description：Create Text Pointer
	When xAlignOpen/yAlignOpen = 2 Open Align Mode
	xAlignMode: 0 - left; 1 - middle; 2 - right
	yAlignMode: 0 - top; 1 - middle; 2 - bottom	
	*/
	void UpdateDraw(DWORD Value = 0);


	/*
	Description：Get Window Information
	*/
	bool GetWndInfo();
	
public:
	//读取和获取属性信息
	virtual bool ReadProperty(MSXML2::IXMLDOMNodePtr pNode);
	virtual MSXML2::IXMLDOMNodePtr GetProperty();
	//虚函数
	DWORD GetType()
	{
		return SOType;
	}

	DWORD GetRenderType()
	{
		return Render_Type_Screen;	
	}


	//点选
	bool PtSelect(double &x, double &y, osg::Vec3d& eye, osg::Vec3d& vec);

	//弹出编辑对话框
	bool BeginEditUI();
	bool EndEdit3D();
};
}
