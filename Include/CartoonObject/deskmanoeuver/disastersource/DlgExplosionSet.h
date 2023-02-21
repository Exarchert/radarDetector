#pragma once
#include "../../../../CartoonObject/resource.h"

// CDlgExplosionSet dialog

class CDlgExplosionSet : public CDialog
{
	DECLARE_DYNAMIC(CDlgExplosionSet)

public:
	CDlgExplosionSet(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgExplosionSet();

// Dialog Data
	enum { IDD = IDD_PARTICLE_BASE_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	afx_msg void OnClose();	
	afx_msg void OnBnClickedOk();

	virtual BOOL OnInitDialog();

	void NotifyUpdate();

	virtual void PostNcDestroy();
public:
	float m_explosion_range;
	afx_msg void OnEnChangeExplosiondlgRange();
};
