#pragma once
#include "shockwaveflash.h"

// CFlashCtrl

class CFlashCtrl : public CShockwaveFlash
{
	DECLARE_DYNAMIC(CFlashCtrl)

public:
	CFlashCtrl();
	virtual ~CFlashCtrl();

protected:
	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	static void CALLBACK OnTimer(HWND hwnd, UINT uMsg, UINT_PTR param, DWORD dwTime);

	void SetForward(int step);
	void SetRewind(int step);
	void KillCurTimer();

	int		m_nForwardID;
	int		m_nBackID;
	bool	m_bTimeIsRun;
};


