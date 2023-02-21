/*************************************************************************
FileName��CSOScreenImage.h CSOScreenImage.cpp
Author:	Liugh
Date��2009-11-3
Description��Screen Image
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
 
	//��ȡ�ͻ�ȡ������Ϣ
	virtual bool ReadProperty(MSXML2::IXMLDOMNodePtr pNode);

	bool CreateImageRectangle(const osg::Image* image, const osg::Vec2* size,osg::Geometry* geometry, CenterMode mode=CENTER);

	bool SetImageToBkGround();	


 public:
 	int			_wndId;			//����id
	float		_imageWide;		//ͼƬ���
	float		_imageHeigth;	//ͼƬ�߶�
	CString		_imagePath;		//��Դ·��
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
	BOOL		_bSetBkImage;	//�Ƿ���Ϊ����ͼƬ
	float		_imageAlpha;	//ͼƬ͸����
	osg::MatrixTransform* _imageTrans;	//ͼƬ�任����

 	osg::Node* m_ImageNode;

	CSOWindow* m_pWnd;

	float _wndLeft,_wndRight,_wndTop,_wndBottom,_offX,_offY,_columnNum,_lineNum;
	osg::Vec3 _imagePos;
	bool m_bCanceled;		//ȡ������

	/*
	Description��Create Screen Image
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

	//��ѡ
	bool PtSelect(double &x, double &y, osg::Vec3d& eye, osg::Vec3d& vec);

	//�����༭�Ի���
	bool BeginEditUI();
	bool EndEdit3D();

	MSXML2::IXMLDOMNodePtr GetProperty();

};
}
