/*******************************************************************
Copyright (c),	
File:			PlayCtrl.h	PlayCtrl.cpp
Description:	Supports uniform interface to control cartoon play
Author:			LiuGh
Date:			2010.11.16
-------------------------------------------------------------------
History:

********************************************************************/
#pragma once
#include <CartoonObject\Export.h>
#include <osgWidget\WindowManager>
#include "..\..\..\CartoonObject\PlayerCtrl\PlayerUI\Player.h"
#include "..\..\..\CartoonObject\PlayerCtrl\PlayerWnd.h"

#include "mmsystem.h"

namespace CartoonObject
{
	class CPlayerEventHandler;
	//����������
	class CVolumeCtrl
	{
	public:
		CVolumeCtrl();
		~CVolumeCtrl();
	protected:
		CWnd*	m_pParentWnd;
		LONG	m_MinVol;
		LONG	m_MaxVol;

		HMIXER	m_hMixer;	//ָ����Ƶ�豸�ľ��

		//��Ƶ�豸��Ϣ
		CString	m_Name;
		DWORD	m_ContrlID;
		DWORD	m_ContrlNum;

		BOOL	m_bIsOpen;

	public:
		bool	CreateVolumeCtrl(CWnd* pWnd);
		void	Close();

		/*
		vl:		������
		vr��	������
		*/
		DWORD	GetVolume(DWORD& vl,DWORD& vr);
		void	SetVolume(DWORD vl,DWORD vr);

		DWORD	GetMinVolume();
		DWORD	GetMaxVolume();

		BOOL	IsOpen();
	};
	//�ű����ſ�����
	class CARTOONOBJECT_EXPORT CPlayerCtrl
	{
	private:
		CPlayerCtrl(void);
		CPlayerCtrl(osgViewer::Viewer* viewer);
		~CPlayerCtrl(void);

	public:
		static CPlayerCtrl* m_pPlayCtrl;
		static CPlayerCtrl* GetInstance();
		static CPlayerCtrl* GetInstance(osgViewer::Viewer* viewer);

		static void Release();
	public:
		//�ű����ſ���
		void PlayScript(const CString& czScript);

		void Pause(bool bPause);

		void Forward(int step);

		void Rewind(int step);

		void Seek(double time);

		void Stop();

	public:
		BOOL Create(LPCTSTR lpszWndName,POINT& ptOrigin);

		void SetProgressDraged(BOOL bDrag){_bProgressDraged = bDrag;}
		BOOL GetProgressDraged(){return _bProgressDraged;}

		void ResetTimeLegthToCtrl(double time);	

		void SetScriptIsUpdate(BOOL bUpdate);
		BOOL GetScriptIsUpdate(){return _bIsUpdate;}

		double GetScriptTimeLength(){return _timeLen;}
		void SetScriptTimeLenght(double tm){_timeLen = tm;}

		double GetCurPlayTime();

		//���û�ȡǰһ���Žű�����
		void SetPreScriptName(const CString& script){_preScriptName = script;}
		CString GetPreScriptName(){return _preScriptName;}

		//���û�ȡ����״̬ 
		DWORD GetPlayState();
		void SetPlayState(DWORD playState);

		//֪ͨ���¼��б��Ѿ�����
		void SetEventListChanged(BOOL bChanged);

		//��Ƶ����
		/*
		vl:		������
		vr��	������
		*/
		DWORD	GetVolume(DWORD& vl,DWORD& vr);
		void	SetVolume(DWORD vl,DWORD vr);
		DWORD	GetMinVolume();
		DWORD	GetMaxVolume();

	private:
		BOOL CreateWindowManager();
		BOOL ShowWindow();

	public:

		enum PLAY_STATE
		{
			PC_DEFAULT = 0,
			PC_PLAY,		
			PC_PAUSE,
			PC_STOP,
			PC_FORWARD,
			PC_REWIND,
			PC_FINISH,
		};

		DWORD _playState;
		osg::ref_ptr<CPlayerWnd>				_playerHandler;		//�¼��б��ű��б���Ӧ����
		osg::ref_ptr<Player>					_player;
		osg::ref_ptr<osgViewer::Viewer>			_viewer;
		osg::ref_ptr<osgWidget::WindowManager>	_wm;
		osg::ref_ptr<CPlayerEventHandler>		_handler;
		CVolumeCtrl		m_VolumeCtrl;
		static HANDLE	_hMutext;
	private:
		BOOL	_bProgressDraged;
		BOOL	_bIsUpdate;
		CString	_preScriptName;
		double	_timeLen;
	};

	//������
	class CWMNodeAddOrRemove : public COM::CCmdObj
	{
	public:
		enum Mode
		{
			WM_ADD,
			WM_REMOVE,
			CHILD_ADD,
			CHILD_REMOVE,
			HANDLER_REMOVE,

			ResetTime,
			NOTHING
		};
		Mode _mode;

		double _timeLen;

	public:
		CWMNodeAddOrRemove(Mode mode = NOTHING,double tmLen = 0.0f):_mode(mode),_timeLen(tmLen)
		{
		}

		virtual void DoAction();
	};

	//ע��һ��������������״̬��
	class CPlayerEventHandler : public osgGA::GUIEventHandler
	{
	public:
		CPlayerEventHandler()
		{
			_playerCtrl = CPlayerCtrl::GetInstance();
		}
		virtual bool handle(const osgGA::GUIEventAdapter& ea,osgGA::GUIActionAdapter& aa);
	public:
		CPlayerCtrl* _playerCtrl;
	};
}