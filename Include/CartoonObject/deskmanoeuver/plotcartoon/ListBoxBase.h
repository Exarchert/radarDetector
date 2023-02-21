#pragma once


// CListBoxBase

class CListBoxBase : public CListBox
{
	DECLARE_DYNAMIC(CListBoxBase)

public:
	CListBoxBase();
	virtual ~CListBoxBase();

protected:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnAddPlot();
	afx_msg void OnDelPlot();
	afx_msg void OnBrowserPlot();
	afx_msg void OnInsertElement();
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
};


