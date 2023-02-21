/************************************************************************
Copyright (C),  
文件：LightingMan.h  LightingMan.cpp
作者：黄志鹏
版本：1.0
日期：2010-01-06
描述：三维场景光照管理类.

修改记录：
作者					描述					日期
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
	目的：初始化
	参数：parent	(in)	光源的父结点
	      earth		(in)	地球结点
		  fields	(in)	厂区根结点
	      pos		(in)	光源的位置(经纬高)	
	返回：成功返回true，失败返回false.
	*/
	bool Init(osg::Group *parent, osg::Node *earth, osg::Group *fields, const osg::Vec3d &pos);

	enum ATTRI_TYPE	{
		TYPE_EARTH,
		TYPE_FIELDS
	};

	/*
	目的：设置光照参数(线程安全)
	参数：whose		(in)	光源的类别
	      ambient	(in)	环境光
	      diffuse	(in)	漫反射
	      specular	(in)	发射光
	返回：成功返回ture，失败返回false
	*/
	bool SetLightParam(ATTRI_TYPE whose, const osg::Vec4 &ambient, const osg::Vec4 &diffuse, const osg::Vec4 &specular);

	/*
	目的：获取光照参数(线程安全)
	参数：whose		(in)	光源的类别
	      ambient	(out)	环境光
	      diffuse	(out)	漫反射
	      specular	(out)	发射光
	返回：成功返回ture，失败返回false
	*/
	bool GetLightParam(ATTRI_TYPE whose, osg::Vec4 &ambient, osg::Vec4 &diffuse, osg::Vec4 &specular);

	/*
	目的：设置光源参数(线程安全)
	参数：whose		(in)	光源的类别
	       center	(in)	光源中心
	       position	(in)	光源位置
	       angle	(in)	选择角度
	返回：成功返回ture，失败返回false
	*/
	bool SetShadowParam(ATTRI_TYPE whose,osg::Vec3d center,osg::Vec3d position,float angle,bool switchb);
	/*
	目的：处理来自OSG消息队列的事件
	*/
	virtual bool handle(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa);
	
	//光照获取与位置设置
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


	//光照
	osg::ref_ptr<osg::Light> m_earthLight;
	osg::ref_ptr<osg::Light> m_fieldsLight;
	//std::list<osg::ref_ptr<osg::Light>> m_addLightList;
	//光照属性
	osg::Vec4 m_earthAmbient;
	osg::Vec4 m_earthDiffuse;
	osg::Vec4 m_earthSpecular;
	osg::Vec4 m_fieldsAmbient;
	osg::Vec4 m_fieldsDiffuse;
	osg::Vec4 m_fieldsSpecular;


	osg::ref_ptr<osg::MatrixTransform> m_fieldsLightMx;

	LightCmdList		m_cmdList;		//命令队列
	ShadowCmdList       m_shadowCmdList;
	OpenThreads::Mutex	m_cmdsMutex;	//互斥量
};

}