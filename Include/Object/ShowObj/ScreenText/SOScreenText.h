/*************************************************************************
FileName��CSOScreenText.h CSOScreenText.cpp
Author:	Liugh
Date��2009-10-30
Description��Screen Text
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
	int			_wndId;			//����id
	float		_textSize;		//���ִ�С
	osg::Vec4	_textColor;		//��ɫ
	CString		_textContent;	//��ע����
	int			_fontIndex;		//����ѡ������
	CString		_textFont;		//����
	int			_horizon;		//����λ������
	float		_xScreen;		//��������x����
	float		_xPersent;		//���ں���ٷֱ�
	UINT		_columnPos;		//��λ��
	int			_xAlign;		//������뷽ʽ
	int			_virtical;		//����λ������
	float		_yScreen;		//��������y����
	float		_yPersent;		//��������ٷֱ�
	UINT		_linePos;		//��λ��
	int			_yAlign;		//������뷽ʽ
	osg::ref_ptr<osgText::Text> _Text;		//����ָ��

	float _wndLeft,_wndRight,_wndTop,_wndBottom,_offX,_offY,_columnNum,_lineNum;
	osg::Vec3 _textPos;
	float _textWide,_textHeight;

	CSOWindow* m_pWnd;
	bool m_bCanceled;

	/*
	Description��Create Text Pointer
	When xAlignOpen/yAlignOpen = 2 Open Align Mode
	xAlignMode: 0 - left; 1 - middle; 2 - right
	yAlignMode: 0 - top; 1 - middle; 2 - bottom	
	*/
	void UpdateDraw(DWORD Value = 0);


	/*
	Description��Get Window Information
	*/
	bool GetWndInfo();
	
	void CreateTextNode(osgText::Text* inText);
public:
	//��ȡ�ͻ�ȡ������Ϣ
	virtual bool ReadProperty(MSXML2::IXMLDOMNodePtr pNode);
	virtual MSXML2::IXMLDOMNodePtr GetProperty();
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
};
}
