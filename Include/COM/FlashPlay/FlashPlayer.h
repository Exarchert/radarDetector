/************************************************************************
CopyRight (C),  Development
File��FlashPlay.h FlashPlay.cpp
Author��Liugh
Version��1.0
Data��2010-08-18
Description: Flash File Display Controller

History:
Author					Data					Description
-------------------------------------------------------------------------

************************************************************************/
#pragma once

#include <com/Export.h>
#include "FlashCtrl.h"
#include <OpenThreads\Mutex>


namespace COM
{
 	class COM_EXPORT CFlashPlayer
 	{
 	public:
 		CFlashPlayer();
 		CFlashPlayer(CRect WindowRT,CString flashName,HWND hWnd = NULL);
 		~CFlashPlayer(void);
 
  		static CFlashPlayer* s_pFlashPlayer;
  		static CFlashPlayer* GetInstance();
  
  		void SetHwnd(HWND hWnd);
  		void SetSize(CRect WindRt);
  		void SetFile(CString flashFile);
  
  		bool InitFlashCtrl();
  
  		BOOL FlashIsPlay();
  
  	protected:
  		CString m_FlashFile;
  		CRect	m_WindRect;
  		HWND	m_hWnd;
  		CFlashCtrl* m_pFlashCtrl;

		OpenThreads::Mutex m_FlashWndMutex;
 	public:
  		void Play();
  		void Pause();
 		void Stop();
		void Forward(int step);
		void Rewind(int step);
 	};
}
