
/******************************************
File name:			CSoundBoot.h
Author:				�����    
Date:				2009-07-22
Description:		�������Ź�����
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
		SOUND_NOTHING,	//δ�����κ��ļ�
		SOUND_PLAY,		//��ʼ�����ļ�
		SOUND_PAUSE,	//��ͣ����
		SOUND_STOP,		//ֹͣ����
	};

	enum SOUND_SET
	{
		SOUND_NOSET,	//�������ļ�����	
		SOUND_PITCH,	//�ٶȵ���(0 ~ 4.0)
		SOUND_VOLUM,	//��������(0 ~ 1.0)
		SOUND_SEEK,		//��ת��ָ��ʱ���ļ�
	};

	struct sourceInfo
	{
		openalpp::Source* _soundSrc;
		int src_id;
		bool bStop;
	};

	/*
	˵��:Ŀǰֻ֧��.ogg�ļ�����ת����
	*/
	class COM_EXPORT CSoundBoot : public osgGA::GUIEventHandler
	{
	public:
		CSoundBoot(osgViewer::Viewer* viewer);
		~CSoundBoot(void);	

		virtual bool handle(const osgGA::GUIEventAdapter& ea,osgGA::GUIActionAdapter& aa);

		/*
		Ŀ�ģ���ʼ�����Ż���
		������
		fileName		(in)		��Ƶ�ļ�Ŀ¼
		bBindSoundSrc	(in)		false	��������Դ��	true	����Դ�����λ�ý��а�
		����ֵ��		void
		*/
		bool SoundInit(const std::string& fileName,bool bBindSoundSrc = false);
		/*
		Ŀ�ģ��˳�����
		������		void
		����ֵ��	void
		*/
		void SoundExit(void);
		/*
		Ŀ�ģ����ò���ģʽ
		������	
		playType		(in)		����ģʽö��ֵ
		����ֵ��		void
		*/
		void SetPlayModel(SOUND_MODEL playType);

		/*
		Ŀ�ģ����ò�������
		������	
		soundEnv		(in)		��������ö��ֵ
		s_f				(in)		������
		����ֵ��		void
		*/
		void SetPlayEnvir(SOUND_SET soundEnv,float s_f);

		/*
		Ŀ�ģ����ò����ļ�
		������		
		fineName		(in)		��Ƶ�ļ�Ŀ¼
		����ֵ��		void
		*/
		void ResetSrcFile(const std::string& fileName);

		/*
		Ŀ�ģ���̬��������λ��
		������			void
		����ֵ��		void
		*/
		void UpdateListenerPos(osgViewer::Viewer* viewer);

		/*
		Ŀ�ģ���̬������Դλ��
		������			void
		����ֵ��		void
		*/
		void UpdateSoundSrcPos(const osg::Vec3 srcPos);
	private:
		std::string m_strMusicFineName;		//�����ļ�Ŀ¼	
		float	m_soundPitch;				//�����ٶ�


	public:	
		bool	m_bPlaySrcExist;			//�Ѵ��ڲ����ļ�
		bool	m_bBindSrcPos;				//�Ƿ���������Դ�ļ�
		bool	m_bSopted;					//(true)ֹͣ�����ļ�
		bool	m_bPlayed;					//(true)��ʼ�����ļ�
		float	m_soundVolum;				//��������

		DWORD	m_oldTime;
		DWORD	m_curTime;

		osg::ref_ptr<openalpp::Source> m_soundSource;		//����Դ����
		osgViewer::Viewer* m_Viewer;


		osg::ref_ptr<openalpp::Listener>		m_listener;		//���������߱���
		osg::Vec3  m_Listenerpos;				//��������ά����
		osg::Vec3  m_SoundSrcPos;				//��Դ��ά����
	};
}

/*************************************************************************************
*************************************************************************************/