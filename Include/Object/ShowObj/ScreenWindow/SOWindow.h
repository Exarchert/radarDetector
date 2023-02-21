/*************************************************************************
FileName：CCOM_WindowManager.h CCOM_WindowManager.cpp
Author:	Liugh
Date：2009-10-28
Description：Window manager
**************************************************************************/
#pragma once

#include <Object/Export.h>
#include <Object/Obj.h>

namespace EMObj{
	
	class CSOScreenImage;
	class CSOScreenText;

	class OBJECT_EXPORT CSOWindow : public EMObj::CObj
	{
	public:
		CSOWindow();
		~CSOWindow(void);

		enum{
			SOType = 8
		};

	public:
		float	_wndWide;			//窗口宽度
		float	_wndHeight;			//窗口高度
		UINT	_horizon;
		UINT	_xAlign;
		UINT	_virtical;
		UINT	_yAlign;
		float	_wndXOrigion;		//窗口左下起点坐标	
		float	_wndYOrigion;		
		float	_wndLeftWide;		//窗口客户区左右偏移量
		float	_wndRightWide;
		float	_wndTopWide;		//窗口客户区上下偏移量          
		float	_wndBottomWide;
		int		_wndLineNum;		//窗口拥有的总行数
		float	_wndColumnNum;		//窗口拥有的总列数
		BOOL	_bStartLC;			//行列触发器true/false
		osg::Vec4 _wndBkColor;		//窗口背景色

		std::vector<CSOScreenText*> _TextVec;		//窗口拥有的文字
		std::vector<CSOScreenImage*> _ImageVec;		//屏幕图片

		osg::ref_ptr<osg::Geode> m_wndGeode;
		//初始化一个客户自定义窗口，默认为整个客户区
		void InitUserWindow(CRect rt);

		void DrawGeometry();

		void DrawWndChildNode();

		bool RemoveChildNode();


	public:
		bool m_bDrawWnd;		//窗口对象是否已经绘制
		bool m_bCanceled;		//取消操作
	public:
		/*
		Description: Create a User Window
		Param:	(in)	xOrigin/yOrigion	Left_bottom
		(in)	width/heigth		Window wide and height
		(in)	bkColor				Window Back Ground Color
		*/
		void CreateWnd();
	public:

		//读取和获取属性信息
		virtual bool ReadProperty(MSXML2::IXMLDOMNodePtr pNode);
		
		virtual MSXML2::IXMLDOMNodePtr GetProperty();

		void UpdateDraw(DWORD Value = 0);

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
		bool UpdateBaseData();
	};
}