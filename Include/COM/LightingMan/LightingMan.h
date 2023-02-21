/************************************************************************
Copyright (C),  
�ļ���LightingMan.h  LightingMan.cpp
���ߣ���־��
�汾��1.0
���ڣ�2010-01-06
��������ά�������չ�����.

�޸ļ�¼��
����					����					����
-------------------------------------------------------------------------
liugh	get a specified light pointer and reset light position  2010.5.19
************************************************************************/

#pragma once

#include <Com\Export.h>
#include <osg/ref_ptr>
#include <osg/Referenced>
#include <osg/Vec4>
#include <osg/Vec3d>
#include <osg/Node>
#include <osg/Group>
#include <osg/Matrix>
#include <osg/MatrixTransform>
#include <osg/Light>
#include <osg/LightSource>
#include <osg/LightModel>
#include <OpenThreads\Mutex>
#include <OpenThreads\ScopedLock>
#include <osgGA\GUIEventHandler>
#include <osgGA\GUIEventAdapter>
#include <osgGA\GUIActionAdapter>


namespace COM	{

class COM_EXPORT CLightingMan : public osgGA::GUIEventHandler
{
public:
	CLightingMan();
	virtual ~CLightingMan();

	/*
	Ŀ�ģ���ʼ��
	������parent	(in)	��Դ�ĸ����
	      earth		(in)	������
		  fields	(in)	���������
	      pos		(in)	��Դ��λ��(��γ��)	
	���أ��ɹ�����true��ʧ�ܷ���false.
	*/
	bool Init(osg::Group *parent, osg::Node *earth, osg::Group *fields, const osg::Vec3d &pos);

	enum ATTRI_TYPE	{
		TYPE_EARTH,
		TYPE_FIELDS
	};

	/*
	Ŀ�ģ����ù��ղ���(�̰߳�ȫ)
	������whose		(in)	��Դ�����
	      ambient	(in)	������
	      diffuse	(in)	������
	      specular	(in)	�����
	���أ��ɹ�����ture��ʧ�ܷ���false
	*/
	bool SetLightParam(ATTRI_TYPE whose, const osg::Vec4 &ambient, const osg::Vec4 &diffuse, const osg::Vec4 &specular);

	/*
	Ŀ�ģ���ȡ���ղ���(�̰߳�ȫ)
	������whose		(in)	��Դ�����
	      ambient	(out)	������
	      diffuse	(out)	������
	      specular	(out)	�����
	���أ��ɹ�����ture��ʧ�ܷ���false
	*/
	bool GetLightParam(ATTRI_TYPE whose, osg::Vec4 &ambient, osg::Vec4 &diffuse, osg::Vec4 &specular);

	/*
	Ŀ�ģ����ù�Դ����(�̰߳�ȫ)
	������whose		(in)	��Դ�����
	       center	(in)	��Դ����
	       position	(in)	��Դλ��
	       angle	(in)	ѡ��Ƕ�
	���أ��ɹ�����ture��ʧ�ܷ���false
	*/
	bool SetShadowParam(ATTRI_TYPE whose,osg::Vec3d center,osg::Vec3d position,float angle,bool switchb);
	/*
	Ŀ�ģ���������OSG��Ϣ���е��¼�
	*/
	virtual bool handle(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa);
	
	//���ջ�ȡ��λ������
	osg::Light* GetEarthLight(){return m_earthLight;}
	osg::Light* GetFieldLight(){return m_fieldsLight;}

	void SetEarthLightPos(const osg::Vec3d pos);
	void SetFieldLightPos(const osg::Vec3d pos);

	void SetShadowSwitch(bool switchb);

protected:
	void SetSceneShadow();

	osg::Group* m_sceneShadow;
	osg::Group* m_parent;
	osg::Switch* m_sceneShadowSwitch;

	class LightCmd : public osg::Referenced
	{
	public:
		ATTRI_TYPE	m_type;
		osg::Vec4	m_ambient;
		osg::Vec4	m_diffuse;
		osg::Vec4	m_specular;
	};
	typedef std::vector<osg::ref_ptr<LightCmd>> LightCmdList;

	class ShadowCmd:public osg::Referenced
	{
	public:
		ATTRI_TYPE m_type;
		osg::Vec3d m_center;
		osg::Vec3d m_position;
		float m_angle;
		bool m_switch;
	};
	typedef std::vector<osg::ref_ptr<ShadowCmd>> ShadowCmdList;


	//����
	osg::ref_ptr<osg::Light> m_earthLight;
	osg::ref_ptr<osg::Light> m_fieldsLight;
	//std::list<osg::ref_ptr<osg::Light>> m_addLightList;
	//��������
	osg::Vec4 m_earthAmbient;
	osg::Vec4 m_earthDiffuse;
	osg::Vec4 m_earthSpecular;
	osg::Vec4 m_fieldsAmbient;
	osg::Vec4 m_fieldsDiffuse;
	osg::Vec4 m_fieldsSpecular;


	osg::ref_ptr<osg::MatrixTransform> m_fieldsLightMx;

	LightCmdList		m_cmdList;		//�������
	ShadowCmdList       m_shadowCmdList;
	OpenThreads::Mutex	m_cmdsMutex;	//������
};

}