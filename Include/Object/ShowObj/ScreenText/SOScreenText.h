/*************************************************************************
FileName：CSOScreenText.h CSOScreenText.cpp
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
#include <Object/ShowObj/ScreenWindow/SOWindow.h>

namespace EMObj{

class OBJECT_EXPORT CSOScreenText : public EMObj::CObj
{
public:
	CSOScreenText(void);
	~CSOScreenText(void);

	enum{
		SOType = 9
	};

public:
	int			_wndId;			//窗口id
	float		_textSize;		//文字大小
	osg::Vec4	_textColor;		//颜色
	CString		_textContent;	//标注内容
	int			_fontIndex;		//字体选择索引
	CString		_textFont;		//字体
	int			_horizon;		//横向位置设置
	float		_xScreen;		//窗口左下x坐标
	float		_xPersent;		//窗口横向百分比
	UINT		_columnPos;		//列位置
	int			_xAlign;		//横向对齐方式
	int			_virtical;		//纵向位置设置
	float		_yScreen;		//窗口左下y坐标
	float		_yPersent;		//窗口纵向百分比
	UINT		_linePos;		//行位置
	int			_yAlign;		//纵向对齐方式
	osg::ref_ptr<osgText::Text> _Text;		//文字指针

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
	
	void CreateTextNode(osgText::Text* inText);
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
