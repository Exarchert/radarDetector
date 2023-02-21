/**********************************************************************************
File:			COM_CartoonManager.h
Author:			��ƽ    
Date:			2009-03-31
Description:    ����������

ChangeLog:
����					����					����
-----------------------------------------------------------------------------------
��־��				2009-07-21			��飬ɾ������Ҫ�Ĵ���
��־��				2009-11-08			���CCOM_PassiveCartoonPlayer
��־��				2009-12-29			��Ӵ��¼��б����й���

Liugh				2010.11.25			Add All XML Parser Function For Each Script

***********************************************************************************/

#pragma once

#include <temp/Export.h>
#include <osg/ref_ptr>
#include <osg/Node>
#include <osgViewer/Viewer>
#include <osgViewer/ViewerEventHandlers>
#include <osgGA/GUIEventAdapter>
#include <osgGA/GUIActionAdapter>
#include <osgGA/EventQueue>
#include <temp/Cartoon/COM_CartoonUnit.h>


namespace TEMP		{

class TEMP_EXPORT CCOM_CartoonManager
{
public:
	CCOM_CartoonManager(void);
	virtual ~CCOM_CartoonManager(void);

	//��Ӻ�ɾ����ͨ��Ԫ
	void AddCartoonUnit(CCOM_CartoonUnit *pCartoonUnit);
	void DelCartoonUnit(CCOM_CartoonUnit *pCartoonUnit);

	void Clear();

	//����ʱ��
	void Update(double Time);
	void SetPause();
	void SetPlay();
	void SetForward(int step);
	void SetRewind(int step);

	//����ӰƬ���Ѳ���ʱ��
	void SetPlayTime(double time);

	//��ȡ�����ܳ�
	double GetScriptTimeLength(int index = 0);

	//��ȡ��ǰ����ʱ��
	double GetCurPlayTime(int index = 0);

protected:
	std::vector<osg::ref_ptr<CCOM_CartoonUnit>> m_CartoonUnits;
	double m_LastTime;
};


//////////////////////////////////////////////////////////////////////////
//���¼��б�����
class CItemRegion
{
public:

	bool isPick(float x, float y)
	{
		return ((x>m_left) && (x<m_right) && (y>m_top) && (y<m_bottom));
	}

	//����Ϊ��������left��top��right��bottom
	float m_left;
	float m_top;		
	float m_right;
	float m_bottom;

	//��Ӧ�Ĳ�����ʼʱ��
	double m_time;
};

class CEventListRegion
{
public:
	//����Ϊ���¼�ͼƬ������ԭ��
	float m_left;
	float m_top;
	//�Ƿ���Ч
	bool m_bValid;
	//�����б�
	std::vector<CItemRegion> m_regList;
};

//�����¼�����
class CCOM_CartoonPlayEventHandler : public osgGA::GUIEventHandler
{
public:
	CCOM_CartoonPlayEventHandler() {}

	virtual ~CCOM_CartoonPlayEventHandler() {}

	//�¼�����
	bool handle(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa);

	//��ʼ��
	bool Init();

	//���ýű�
	void SetScript(const CString &szScript);
	
	//Archive Format File
	bool LoadFromArchiveFile(const CString &szFilePath);
	
	//XML Format File
	bool LoadFromXMLFile(const CString &szFilePath);

	//XML String Format
	void LoadScript(const CString& strXml);

	//��ȡ�ű�����ʱ���ܳ�
	double GetScriptTimeLength();

	//��ȡ��ǰ����ʱ��
	double GetCurPlayTime();


private:
	void InitCartoonUnit(CCOM_CartoonUnit* ptr);

protected:
	//���ؽű�
	void LoadScript();

public:
	CCOM_CartoonManager m_CartoonManager;
	CEventListRegion	m_eventList;

protected:
	CString		m_szScripts;	//�ű�����
	double		m_beginTime;	//��ʼ��ʱ�̣�����ʱ�䣩
};

}