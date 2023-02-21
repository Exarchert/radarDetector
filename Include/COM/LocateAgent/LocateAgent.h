/****************************************************************************
Copyright (C),  
�ļ���LocateAgent.h LocateAgent.cpp
���ߣ���־��
�汾��1.0
���ڣ�2010-03-01
����������ٿ�����Ķ�λ��Ŀǰ֧�ֵķ�ʽ�У�ֱ�Ӷ�λ�����ж�λ.�����̰߳�ȫ��.

�޸ļ�¼��
����					����					����
-----------------------------------------------------------------------------

****************************************************************************/

#pragma once

#include <Com\Export.h>
#include <osg\Vec3d>
#include <osgGA\GUIEventHandler>
#include <osgGA\GUIEventAdapter>
#include <osgGA\GUIActionAdapter>
#include <OpenThreads\Mutex>
#include <Com\Cartoon\CartoonManager.h>

#include <map>


namespace COM	{

//CLocateAgent��
class COM_EXPORT CLocateAgent : public osgGA::GUIEventHandler
{
public:
	CLocateAgent();
	virtual ~CLocateAgent();

	/*
	Ŀ�ģ���������OSG��Ϣ���е��¼�
	*/
	virtual bool handle(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa);

	/*
	Ŀ�ģ���ʼ��
	������player	(in)	��ͨ������ָ��
	*/
	void Init(CCartoonManager *player);

	/*
	Ŀ�ģ����ж�λ(�̰߳�ȫ)
	������pos	(in)	��γ��
	      pose	(in)	��̬��
	*/
	void Fly(osg::Vec3d pos, osg::Vec3d pose);

	/*
	Ŀ�ģ�ֱ�Ӷ�λ(�̰߳�ȫ)
	������pos	(in)	��γ��
	      pose	(in)	��̬��
	*/
	void Direct(osg::Vec3d pos, osg::Vec3d pose);

protected:
	enum LOCATE_TYPE {
		DIRECT,		//ֱ�Ӷ�λ
		FLY			//���ж�λ
	};

	class CLocateCmd : public osg::Referenced
	{
	public:
		LOCATE_TYPE m_type;
		osg::Vec3d	m_destPos;
		osg::Vec3d	m_destPose;
	};
	typedef std::vector<CLocateCmd>	LocateCmdList;

	OpenThreads::Mutex	m_cmdsMutex;	//������
	LocateCmdList		m_locateCmds;	//��λ�������
	osg::ref_ptr<CCartoonManager>	m_player;
};

}