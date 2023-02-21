/**********************************************************************************
File:			COM_CartoonSecondPlayerHandler.h
Description:	Second Cartoon Player
Liugh			2011.01.24
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
#include <temp/Cartoon/COM_CartoonManager.h>


namespace TEMP		
{

	class TEMP_EXPORT CCOM_CartoonSecondManager
	{
	public:
		CCOM_CartoonSecondManager(void);
		virtual ~CCOM_CartoonSecondManager(void);

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

	//�����¼�����
	class CCOM_CartoonSecondPlayerHandler : public osgGA::GUIEventHandler
	{
	public:
		CCOM_CartoonSecondPlayerHandler() {}

		virtual ~CCOM_CartoonSecondPlayerHandler() {}

		//�¼�����
		bool handle(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa);


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
		CCOM_CartoonSecondManager m_CartoonManager;
		CEventListRegion	m_eventList;

	protected:
		CString		m_szScripts;	//�ű�����
		double		m_beginTime;	//��ʼ��ʱ�̣�����ʱ�䣩
	};

}