/*********************************************************************
Copyright (C),  
�ļ���NavigationMap.h NavigationMap.cpp
���ߣ���־��
�汾��1.0
���ڣ�2009-11-16
�����������������.

�޸ļ�¼��
����					����				����
----------------------------------------------------------------------

*********************************************************************/

#pragma once

#include <Com\Export.h>
#include <Com\Decoration\DecoraBase.h>
#include <osg\ref_ptr>
#include <osg\Switch>
#include <osg\Viewport>
#include <osg\Camera>
#include <osgGA\MatrixManipulator>
#include <osgAPEx\NavigationMap>

#import <msxml3.dll>

namespace COM	{

class COM_EXPORT CNavigationMap : public CDecoraBase
{
public:
	CNavigationMap();
	virtual ~CNavigationMap();

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

protected:
	osg::ref_ptr<osg::Switch>				m_root;			//���ص���ͼ
	osg::ref_ptr<osgAPEx::NavigationMap>	m_navMap;		//����ͼ
	double									m_distance;		//��Ч����
	//std::vector<osgAPEx::NavigationMap::MapParam>	m_maps;	//�洢��ͼ��Ϣ

	
	/*
	�����:
	�޸����Ƶ���ͼ��ʾ
	2010-07-06
	*/
public:
	struct MapInfo 
	{
		LONG64	_enterID;
		bool	_bActive;
		float	_dist;
		osgAPEx::NavigationMap::MapParam _mapParam;
	};

	std::vector<MapInfo>	m_maps;	//�洢��ͼ��Ϣ

	static CNavigationMap* GetInstance();
	static osg::ref_ptr<CNavigationMap> s_pNavigation;

	/*
	Ŀ�ģ���ʼ��
	������hud		(in)		HUD
	dist		(in)		��Ч����(���ֵ���ͼ)
	�������ͬosgAPEx::Navigation::init(...)
	���أ��ɹ�����true��ʧ�ܷ���false.
	*/
	bool init(osg::Camera *hud,std::vector<MapInfo>& maps, float mapwidth, const std::string& eyetex, 
		float eyewith, osg::Viewport* vp, osgAPEx::NavigationMap::Alignment align, const osg::Vec2& space, osgGA::MatrixManipulator* manip);

	/*
	Ŀ��: ����Ψһ�ĵ���ͼ��Ч,������վ����ͼ����������ʾ
	����: EnterID	(in)	��վID
	���أ��ɹ�����true��ʧ�ܷ���false.
	*/
	bool SetSingleMapValid(LONG64 EnterID);

	/*
	Ŀ��: �������е���ͼ����Ч
	����: ��
	���أ��ɹ�����true��ʧ�ܷ���false.
	*/
	bool SetAllMapValid();

	static bool GetMapInfoFromXmlNode(MSXML2::IXMLDOMNodePtr pNode,CString& mapPath,float& mapWide,
		float& enterWide,float& enterHeight,float& mapYaw,float& dist,bool& bActive);
private:
	bool bShowMap(bool bShow);
};

}