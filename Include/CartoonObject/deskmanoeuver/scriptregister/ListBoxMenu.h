#pragma once

// CListBoxBase

class CListBoxMenu : public CListBox
{
	DECLARE_DYNAMIC(CListBoxMenu)

public:
	CListBoxMenu();
	virtual ~CListBoxMenu();

protected:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnRegister();
	afx_msg void OnBrowser();
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
};


