#pragma once
#include "../../../../CartoonObject/resource.h"

class CDlgSprayFire : public CDialog
{
	DECLARE_DYNAMIC(CDlgSprayFire)

public:
	CDlgSprayFire(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgSprayFire();

// Dialog Data
	enum { IDD = IDD_PARTICLE_BASE_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	DECLARE_MESSAGE_MAP()

	void NotifyUpdate();
	afx_msg void OnClose();	
public:
	float m_editeIndensity;
	afx_msg void OnEnChangeEditIndensity();
	afx_msg void OnBnClickedOk();
	virtual void PostNcDestroy();
};
