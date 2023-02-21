/****************************************************************
Copyright (C),  
���ߣ���־��
�汾��1.0
���ڣ�2009-06-11
����������HUD(Heads Up Display)��������һ��״̬����OSG����ͼ.
      ����ͼ��֯����:
                  Camera
                    /
                  Geode
                  /	   \
             Geometry   Texts

�޸ļ�¼��
����                       ����                  ����
-----------------------------------------------------------------
��־��					2009-11-16				��ֲ���ù���, ����
												��ʾ���깦��

*****************************************************************/

#pragma once

#include <Com\Export.h>
#include <osg\ref_ptr>
#include <osg\Referenced>
#include <osg\Vec3d>
#include <osg\Switch>
#include <osg\Camera>
#include <osg\Geometry>
#include <osg\Viewport>
#include <osgText\Text>
#include <osgText\Font>
#include <osgViewer\Viewer>
#include <Com\Decoration\DecoraBase.h>


namespace COM	{

class COM_EXPORT CHintBar : public CDecoraBase
{
public:
	CHintBar(osg::Camera *hudCamera, osg::Node *earthNode);
	virtual ~CHintBar();

	enum BarIndex {
		LEFT = 0,
		CENTER,
		RIGHT
	};

	/*
	Ŀ�ģ���������OSG��Ϣ���е��¼�
	*/
	virtual bool handle(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa);

	/*
	Ŀ�ģ���������
	������bShow		(in)	true:��ʾ; false:����
	���أ�����ǰ��״̬
	*/
	virtual bool Show(bool bShow=true);

	/*
	Ŀ�ģ���״̬������ʾ���ı�
	������which    (in)   ָ��Ҫ�����ĸ���
	      margin   (in)   �߾೤�ȣ�������Ӧ����߾࣬������Ӧ���ұ߾࣬�м䲻������
	      str      (str)  �ı�����
	���أ��ɹ�����true, ���򷵻�false
	*/
	bool setText(BarIndex which, int margin, const wchar_t *str);

	/*
	Ŀ�ģ�����״̬���ĸ߶�
	������nHeight  (in)   ״̬���߶�
	���أ��ɹ�����true, ���򷵻�false
	*/
	bool setHeight(int nHeight);

	/*
	Ŀ�ģ���ȡ��Ļ���������Ľ���
	������camera      (in)       ���ָ��
	      subGraph    (in)       ����ĳ���ͼ
	      x, y        (in)       �����ӿ�����
	      pos         (out)      �������������
		  hasParent	  (in)		 �Ƿ��и�Transform���
	���أ��ɹ�����true,���򷵻�false
	*/
	static bool getTerrainIntersection(osgViewer::Viewer *viewer, osg::Node *subGraph, float x, float y, osg::Vec3d &pos,
										bool hasParent=false);

	/*
	Alter history: LiuGH add function and variable
	Purpose:	Set status bar display mode
	Parameter:	mode	(in)	if 1: degree/minute/second show, else show origin data with log/lat/alt
	Data:		2010/01/27
	*/
	void SetShowMode(int mode);

	/*
	Alter history: LiuGH add function and variable
	Purpose:	Set ini file path
	Parameter:	strIni	(in)	file name
	Data:		2010/01/27
	*/
	void SetIniFileName(CString strIni);

	int m_ShowMode;
	CString m_iniFile;	
protected:
	/*
	Ŀ�ģ���ʼ��״̬������
	������void
	���أ�void
	*/
	void initOsg();

	/*
	����״̬���Ĵ�С
	*/
	void adjustSize();

private:
	osg::ref_ptr<osg::Switch>	m_refOsg;			//ָ�򳡾�ͼ���ڵ�
	osg::ref_ptr<osgText::Text> m_refTextArray[3];	//3���ı�(������)
	osg::ref_ptr<osg::Geometry> m_refRect;			//��������
	int m_nLeftMargin;								//��߾�
	int m_nRightMargin;								//�ұ߾�
	int m_nHeight;									//״̬���ĸ�
	int m_nWidth;									//״̬���Ŀ�

	osg::ref_ptr<osg::Node>		m_earthNode;
	osg::Vec3d					m_position;			//������ϴ�λ�ú���̬
	osg::Vec3d					m_center;			//-------|
	osg::Vec3d					m_up;				//-------|
};

}
