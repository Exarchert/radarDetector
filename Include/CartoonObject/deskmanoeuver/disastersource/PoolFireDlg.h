#pragma once
#include "..\..\..\..\CartoonObject\resource.h"

// CDlgPoolFire dialog

class CDlgPoolFire : public CDialog
{
	DECLARE_DYNAMIC(CDlgPoolFire)

public:
	CDlgPoolFire(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgPoolFire();

// Dialog Data
	enum { IDD = IDD_DIALOG_POOLEFFECT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnEnChangePoolwidth();
	afx_msg void OnEnChangePoolhight();
	afx_msg void OnEnChangeFire();
	afx_msg void OnClose();	

	virtual void PostNcDestroy();

	void NotifyUpdate();
protected:
	float m_poolWidth;
	float m_poolHight;
	float m_firlIndensity;
public:
	afx_msg void OnBnClickedOk();
};
