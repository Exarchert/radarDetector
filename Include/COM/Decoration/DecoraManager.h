/*********************************************************************
Copyright (C),  
�ļ���DecoraManager.h DecoraManager.cpp
���ߣ���־��
�汾��1.0
���ڣ�2009-11-16
������װ�ζ��������.

�޸ļ�¼��
����					����				����
----------------------------------------------------------------------

*********************************************************************/

#pragma once

#include <Com\Export.h>
#include <Com\Decoration\DecoraBase.h>
#include <osgGA\GUIEventHandler>
#include <osgGA\GUIEventAdapter>
#include <osgGA\GUIActionAdapter>
#include <OpenThreads\Mutex>

#include <map>


namespace COM	{

//װ�ζ����Ԥ����ID
#define	DECORA_HINTBAR			0x0001
#define DECORA_CONTROL_PANEL	0x0002
#define DECORA_NAVIGATION_MAP	0x0003
#define DECORA_NORTH_NEEDLE		0x0004
#define DECORA_SUNSHINE			0x0005
#define DECORA_FOCUSCROSS		0x0006


//CDecoraManager��
class COM_EXPORT CDecoraManager : public osgGA::GUIEventHandler
{
public:
	CDecoraManager();
	virtual ~CDecoraManager();
	
	/*
	Ŀ�ģ���������OSG��Ϣ���е��¼�
	*/
	virtual bool handle(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa);

	/*
	Ŀ�ģ����װ�ζ���
	������id		(in)	����id
	      decora	(in)	����ָ��
	���أ��ɹ�����true�����򷵻�false; ���id�Ѿ����ڣ��򸲸�����.
	*/
	bool AddDecoration(DWORD id, CDecoraBase *decora);

	/*
	Ŀ�ģ��Ƴ�ĳ��װ�ζ���
	������id		(in)	����id
	���أ��ɹ�����true��ʧ�ܷ���false
	*/
	bool RemoveDecoration(DWORD id);

	/*
	Ŀ�ģ���ȡĳ����ָ��
	������id		(in)	����id
	���أ����ض���ָ�룬���ʧ�ܷ���NULL
	*/
	CDecoraBase *GetDecoration(DWORD id) const;
	
	/*
	Ŀ�ģ���ȡĳ���������״̬
	���أ�true��ʾ,����false
	*/
	bool IsDecorationShow(DWORD id);

	/*
	Ŀ�ģ�����ĳ�������Ƿ���ʾ
	������id		(in)	����id
	      bShow		(in)	true��ʾ,false����
	���أ�void
	*/
	void ShowDecoration(DWORD id, bool bShow);

protected:
	class CShowCmd : public osg::Referenced
	{
	public:
		DWORD	m_id;	//װ�ζ���id
		bool	m_bShow;//����
	};
	typedef std::vector<osg::ref_ptr<CShowCmd>>	ShowCmdList;
	typedef std::map<DWORD, osg::ref_ptr<CDecoraBase>>	DecorasMap;
	
	OpenThreads::Mutex	m_cmdsMutex;	//������
	ShowCmdList			m_showCmds;		//�����������
	DecorasMap			m_decoras;		//װ�ζ���
};

}