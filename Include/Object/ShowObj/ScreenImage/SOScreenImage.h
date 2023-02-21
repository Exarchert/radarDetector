/*************************************************************************
FileName：CSOScreenImage.h CSOScreenImage.cpp
Author:	Liugh
Date：2009-11-3
Description：Screen Image
**************************************************************************/
#pragma once
#include <Object/Export.h>
#include <Object/Obj.h>
#include <osg/Node>
#include <Object/ObjList.h>
#include <Object\ObjManager.h>
#include <Object\ShowObj\ScreenWindow\SOWindow.h>
#include <object\Edit\EditPage.h>


namespace EMObj{

class OBJECT_EXPORT CSOScreenImage : public EMObj::CObj
{
public:
	CSOScreenImage(void);
	~CSOScreenImage(void);

	enum CenterMode
	{
		CENTER,
		BOTTOM_LEFT,
		BOTTOM_RIGHT,
		TOP_LEFT,
		TOP_RIGHT,
		LEFT_CENTER,
		RIGHT_CENTER
	};
 
	//读取和获取属性信息
	virtual bool ReadProperty(MSXML2::IXMLDOMNodePtr pNode);

	bool CreateImageRectangle(const osg::Image* image, const osg::Vec2* size,osg::Geometry* geometry, CenterMode mode=CENTER);

	bool SetImageToBkGround();	


 public:
 	int			_wndId;			//窗口id
	float		_imageWide;		//图片宽度
	float		_imageHeigth;	//图片高度
	CString		_imagePath;		//资源路径
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
	BOOL		_bSetBkImage;	//是否设为背景图片
	float		_imageAlpha;	//图片透明度
	osg::MatrixTransform* _imageTrans;	//图片变换矩阵

 	osg::Node* m_ImageNode;

	CSOWindow* m_pWnd;

	float _wndLeft,_wndRight,_wndTop,_wndBottom,_offX,_offY,_columnNum,_lineNum;
	osg::Vec3 _imagePos;
	bool m_bCanceled;		//取消操作

	/*
	Description：Create Screen Image
	When xAlignOpen/yAlignOpen = 2 Open Align Mode
	xAlignMode: 0 - left; 1 - middle; 2 - right
	yAlignMode: 0 - top; 1 - middle; 2 - bottom
	*/
	void UpdateDraw(DWORD Value = 0);
	bool CreateImageNode(osg::MatrixTransform*& imageTrans);

	bool GetWndInfo();
 
	enum{
		SOType = 10
	};
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

	MSXML2::IXMLDOMNodePtr GetProperty();

};
}
