/*************************************************************************
FileName��CCOM_WindowManager.h CCOM_WindowManager.cpp
Author:	Liugh
Date��2009-10-28
Description��Window manager
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
		float	_wndWide;			//���ڿ��
		float	_wndHeight;			//���ڸ߶�
		UINT	_horizon;
		UINT	_xAlign;
		UINT	_virtical;
		UINT	_yAlign;
		float	_wndXOrigion;		//���������������	
		float	_wndYOrigion;		
		float	_wndLeftWide;		//���ڿͻ�������ƫ����
		float	_wndRightWide;
		float	_wndTopWide;		//���ڿͻ�������ƫ����          
		float	_wndBottomWide;
		int		_wndLineNum;		//����ӵ�е�������
		float	_wndColumnNum;		//����ӵ�е�������
		BOOL	_bStartLC;			//���д�����true/false
		osg::Vec4 _wndBkColor;		//���ڱ���ɫ

		std::vector<CSOScreenText*> _TextVec;		//����ӵ�е�����
		std::vector<CSOScreenImage*> _ImageVec;		//��ĻͼƬ

		osg::ref_ptr<osg::Geode> m_wndGeode;
		//��ʼ��һ���ͻ��Զ��崰�ڣ�Ĭ��Ϊ�����ͻ���
		void InitUserWindow(CRect rt);

		void DrawGeometry();

		void DrawWndChildNode();

		bool RemoveChildNode();


	public:
		bool m_bDrawWnd;		//���ڶ����Ƿ��Ѿ�����
		bool m_bCanceled;		//ȡ������
	public:
		/*
		Description: Create a User Window
		Param:	(in)	xOrigin/yOrigion	Left_bottom
		(in)	width/heigth		Window wide and height
		(in)	bkColor				Window Back Ground Color
		*/
		void CreateWnd();
	public:

		//��ȡ�ͻ�ȡ������Ϣ
		virtual bool ReadProperty(MSXML2::IXMLDOMNodePtr pNode);
		
		virtual MSXML2::IXMLDOMNodePtr GetProperty();

		void UpdateDraw(DWORD Value = 0);

		//�麯��
		DWORD GetType()
		{
			return SOType;
		}

		DWORD GetRenderType()
		{
			return Render_Type_Screen;
		}
		//��ѡ
		bool PtSelect(double &x, double &y, osg::Vec3d& eye, osg::Vec3d& vec);
		//�����༭�Ի���
		bool BeginEditUI();
		bool EndEdit3D();
		bool UpdateBaseData();
	};
}