/************************************************************************
Copyright (c)  
�ļ���Weather.h Weather.cpp
���ߣ���־��
�汾��1.0
���ڣ�2009-12-28
���������ԭ�д�����ֲ����. �����������У��ꡢѩ������.
	  ������֯ͼ���£�
					m_parent
						|
					m_swAll
						|
					m_translate
					   /   \
				   swRain  swSnow
					/		  \
				m_peRain	m_peSnow

�޸ļ�¼��
����					����				����
-------------------------------------------------------------------------

************************************************************************/

#pragma once

#include <Com\Export.h>
#include <vector>
#include <osg\Fog>
#include <osg\Node>
#include <osg\Switch>
#include <osg\Referenced>
#include <OpenThreads\Mutex>
#include <OpenThreads\ScopedLock>
#include <osg\MatrixTransform>
#include <osgGA\GUIEventHandler>
#include <osgGA\GUIEventAdapter>
#include <osgGA\GUIActionAdapter>
#include <osgParticle\PrecipitationEffect>


namespace COM	{

class COM_EXPORT CWeather : public osgGA::GUIEventHandler
{
public:
	/*
	Ŀ�ģ����캯��
	      node		(in)	������
	      parent	(in)	������Ч�ĸ����
	*/
	CWeather(osg::Node *node, osg::Group *parent);
	virtual ~CWeather();

	//��������
	enum ATTRIBUTE {
		RAIN,			//��
		SNOW,			//ѩ
		FOG,			//��
		WIND			//��
	};

	//����ǿ��
	enum INTENSION {
		NONE,			//��
		WEAK,			//С
		NORMAL,			//��
		STRONG			//��
	};

	//����
	enum WINDDIR {
		EAST,			//����
		SOUTH,			//�Ϸ�
		WEST,			//����
		NORTH			//����
	};

	/*
	Ŀ�ģ���ʼ��
	������rain		(in)	����ǿ��
	      snow		(in)	��ѩǿ��
	      wind		(in)	�η�ǿ��
		  dir		(in)	����
	*/
	bool Init(INTENSION rain, INTENSION snow, INTENSION fog, INTENSION wind, WINDDIR dir);

	/*
	Ŀ�ģ�������������ǿ��(�̰߳�ȫ)
	������attri		(in)	���Ա��
	      inten		(in)	ǿ��
	      dir		(in)	�������ΪWIND����dirΪ����
	*/
	bool SetIntension(ATTRIBUTE attri, INTENSION inten, DWORD dir);

	/*
	Ŀ�ģ���ȡ��������ǿ��(�̰߳�ȫ)
	������attri		(in)	���Ա��
	      inten		(out)	ǿ��
	      dir		(out)	�������ΪWIND����dirΪ����
	*/
	bool GetIntension(ATTRIBUTE attri, INTENSION &inten, DWORD &dir);

	/*
	Ŀ�ģ���������OSG��Ϣ���е��¼�
	*/
	virtual bool handle(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa);

	void SetLocation(osg::Vec3d& LLA);

protected:
	class AttriCmd : public osg::Referenced
	{
	public:
		ATTRIBUTE	m_attri;
		INTENSION	m_inten;
		WINDDIR		m_dir;
	};
	typedef std::vector<osg::ref_ptr<AttriCmd>> AttriCmdList;

	osg::ref_ptr<osg::MatrixTransform>				m_trans;
	osg::ref_ptr<osg::Switch>						m_swAll;
	osg::ref_ptr<osg::Switch>						m_swRain;
	osg::ref_ptr<osg::Switch>						m_swSnow;
	osg::ref_ptr<osgParticle::PrecipitationEffect>	m_peRain;
	osg::ref_ptr<osgParticle::PrecipitationEffect>	m_peSnow;
	osg::ref_ptr<osg::Fog>							m_fog;

	osg::ref_ptr<osg::Node>		m_node;		//�����
	osg::ref_ptr<osg::Group>	m_parent;	//�����
	bool			m_bOk;					//����߶���������
	INTENSION		m_rainInten;
	INTENSION		m_snowInten;
	INTENSION		m_fogInten;
	INTENSION		m_windInten;
	WINDDIR			m_dir;

	OpenThreads::Mutex	m_cmdsMutex;	//������
	AttriCmdList		m_cmdList;		//�������
};

}