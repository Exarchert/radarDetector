/**************************************************************************
Copyright (C),  
�ļ���CCOM_ItemVehicle.h CCOM_ItemVehicle.cpp
���ߣ���־��
�汾��1.0
���ڣ�2009-07-29
��������ͨ��-��ͨ������(�ʺ�����ģ����)

�޸ļ�¼��
����						����					����
---------------------------------------------------------------------------
��־��						2010-01-14				��Ӷ�IVE�����Ŀ���

**************************************************************************/

#pragma once

#include <temp/Export.h>
#include <osgCal/CoreModel>
#include <osgCal/Model>
#include <osg/NodeVisitor>
#include <osg/AnimationPath>
#include <osg/MatrixTransform>
#include <temp/Cartoon/COM_CartoonItem.h>


namespace TEMP		{

class TEMP_EXPORT CCOM_ItemVehicle : public CCOM_CartoonItem
{
public:
	CCOM_ItemVehicle()
	{
		m_Name = _T("δ����ģ��");
		m_isCfg = false;
		m_actions = 0;
		m_curAction = -1;
	}

	virtual ~CCOM_ItemVehicle();

	CCOM_ItemVehicle(const CCOM_ItemVehicle &vehicle);

	CCOM_CartoonItem * Clone();

	/*
	Ŀ�ģ�����
	*/
	virtual void Play();

	/*
	Ŀ�ģ���ͣ
	*/
	virtual void Pause();	

	/*
	Ŀ�ģ�������Ԫִ����Ϊ
	������Time		(in)	����ڿ�ʼ�ʱ�̵�ʱ��
		  pos		(in)	������λ��ʱ��λ��			
	���أ�void
	*/
	virtual void DoAction(double Time, osg::Vec3d pos);

	//ִ�зǻʱ�����Ķ���
	virtual void DoNoneActive();

	/*
	Ŀ�ģ����úͻ�ȡģ���ļ���
	*/
	CString GetModelFileName()
	{
		return m_modelName;
	}

	void SetModelFileName(CString &str)
	{
		m_modelName = str;
	}

	/*
	Ŀ�ģ����л�����
	*/
	virtual void Serialize(CArchive& ar);

	/*
	Purpose:	Parser XML	Data Format for Animation
	Author:		Liugh
	*/
	virtual bool ParserXml(MSXML2::IXMLDOMNodePtr pNode);


	//�������ص�����
	class CFindAnimationCallback : public osg::NodeVisitor
	{
	public:
		CFindAnimationCallback()
			: osg::NodeVisitor(osg::NodeVisitor::TRAVERSE_ALL_CHILDREN)
			{}

		virtual ~CFindAnimationCallback() { m_callbacks.clear(); }

		//����Callback���ָ��
		virtual void apply(osg::Node& node);

	public:
		std::vector<osg::ref_ptr<osg::AnimationPathCallback>> m_callbacks;
	};

protected:
	DECLARE_SERIAL(CCOM_ItemVehicle)

protected:
	CString	m_modelName;	//ģ���ļ�������
	osg::ref_ptr<osg::MatrixTransform> m_modelNode;
	osg::ref_ptr<osgCal::Model>	m_calModel;
	bool						m_isCfg;		//�Ƿ�Ϊ��������
	unsigned int				m_actions;		//��������
	int							m_curAction;	//��ǰ����

	//����������
	CFindAnimationCallback		m_nodeFinder;
};

}