
/******************************************
File name:			CSoundBoot.h
Author:				刘广慧    
Date:				2009-07-22
Description:		声音播放管理类
*******************************************/
#pragma once
#include <openalpp/Source>
#include <openalpp/Listener>
#include <osgGA/GUIEventHandler>
#include <osgViewer/viewer>
#include <vector>

#include <COM\Export.h>

namespace COM
{
	enum SOUND_MODEL	
	{
		SOUND_NOTHING,	//未播放任何文件
		SOUND_PLAY,		//开始播放文件
		SOUND_PAUSE,	//暂停播放
		SOUND_STOP,		//停止播放
	};

	enum SOUND_SET
	{
		SOUND_NOSET,	//不进行文件设置	
		SOUND_PITCH,	//速度调节(0 ~ 4.0)
		SOUND_VOLUM,	//音量调节(0 ~ 1.0)
		SOUND_SEEK,		//跳转到指定时刻文件
	};

	struct sourceInfo
	{
		openalpp::Source* _soundSrc;
		int src_id;
		bool bStop;
	};

	/*
	说明:目前只支持.ogg文件的跳转功能
	*/
	class COM_EXPORT CSoundBoot : public osgGA::GUIEventHandler
	{
	public:
		CSoundBoot(osgViewer::Viewer* viewer);
		~CSoundBoot(void);	

		virtual bool handle(const osgGA::GUIEventAdapter& ea,osgGA::GUIActionAdapter& aa);

		/*
		目的：初始化播放环境
		参数：
		fileName		(in)		音频文件目录
		bBindSoundSrc	(in)		false	不进行声源绑定	true	将声源与相机位置进行绑定
		返回值：		void
		*/
		bool SoundInit(const std::string& fileName,bool bBindSoundSrc = false);
		/*
		目的：退出播放
		参数：		void
		返回值：	void
		*/
		void SoundExit(void);
		/*
		目的：设置播放模式
		参数：	
		playType		(in)		播放模式枚举值
		返回值：		void
		*/
		void SetPlayModel(SOUND_MODEL playType);

		/*
		目的：设置播放条件
		参数：	
		soundEnv		(in)		播放条件枚举值
		s_f				(in)		调节数
		返回值：		void
		*/
		void SetPlayEnvir(SOUND_SET soundEnv,float s_f);

		/*
		目的：重置播放文件
		参数：		
		fineName		(in)		音频文件目录
		返回值：		void
		*/
		void ResetSrcFile(const std::string& fileName);

		/*
		目的：动态更新收听位置
		参数：			void
		返回值：		void
		*/
		void UpdateListenerPos(osgViewer::Viewer* viewer);

		/*
		目的：动态更新声源位置
		参数：			void
		返回值：		void
		*/
		void UpdateSoundSrcPos(const osg::Vec3 srcPos);
	private:
		std::string m_strMusicFineName;		//音乐文件目录	
		float	m_soundPitch;				//播放速度


	public:	
		bool	m_bPlaySrcExist;			//已存在播放文件
		bool	m_bBindSrcPos;				//是否绑定相机与资源文件
		bool	m_bSopted;					//(true)停止播放文件
		bool	m_bPlayed;					//(true)开始播放文件
		float	m_soundVolum;				//音量控制

		DWORD	m_oldTime;
		DWORD	m_curTime;

		osg::ref_ptr<openalpp::Source> m_soundSource;		//声音源对象
		osgViewer::Viewer* m_Viewer;


		osg::ref_ptr<openalpp::Listener>		m_listener;		//声音接听者变量
		osg::Vec3  m_Listenerpos;				//接受者三维坐标
		osg::Vec3  m_SoundSrcPos;				//声源三维坐标
	};
}

/*************************************************************************************
*************************************************************************************/