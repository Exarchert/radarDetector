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

		//添加和删除卡通单元
		void AddCartoonUnit(CCOM_CartoonUnit *pCartoonUnit);
		void DelCartoonUnit(CCOM_CartoonUnit *pCartoonUnit);

		void Clear();

		//更新时间
		void Update(double Time);
		void SetPause();
		void SetPlay();
		void SetForward(int step);
		void SetRewind(int step);

		//设置影片的已播放时间
		void SetPlayTime(double time);

		//获取播放总长
		double GetScriptTimeLength(int index = 0);

		//获取当前播放时刻
		double GetCurPlayTime(int index = 0);

	protected:
		std::vector<osg::ref_ptr<CCOM_CartoonUnit>> m_CartoonUnits;
		double m_LastTime;
	};

	//播放事件处理
	class CCOM_CartoonSecondPlayerHandler : public osgGA::GUIEventHandler
	{
	public:
		CCOM_CartoonSecondPlayerHandler() {}

		virtual ~CCOM_CartoonSecondPlayerHandler() {}

		//事件处理
		bool handle(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa);


		//设置脚本
		void SetScript(const CString &szScript);

		//Archive Format File
		bool LoadFromArchiveFile(const CString &szFilePath);

		//XML Format File
		bool LoadFromXMLFile(const CString &szFilePath);

		//XML String Format
		void LoadScript(const CString& strXml);


		//获取脚本播放时间总长
		double GetScriptTimeLength();

		//获取当前播放时刻
		double GetCurPlayTime();

	private:
		void InitCartoonUnit(CCOM_CartoonUnit* ptr);

	protected:
		//加载脚本
		void LoadScript();

	public:
		CCOM_CartoonSecondManager m_CartoonManager;
		CEventListRegion	m_eventList;

	protected:
		CString		m_szScripts;	//脚本名称
		double		m_beginTime;	//开始点时刻（世界时间）
	};

}