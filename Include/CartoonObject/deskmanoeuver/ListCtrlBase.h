#pragma once
/*************************************************************************
FileName£ºCTabCtrlBase.h CTabCtrlBase.cpp
Author:	Liugh
Date£º2010-01-20
Description£ºList Ctrl Base Class,Supported Drag Icon
**************************************************************************/

// CListCtrlBase

#define WM_LISTCTRL_LEAVE  WM_USER + 0x001

class CListCtrlBase : public CListCtrl
{
	DECLARE_DYNAMIC(CListCtrlBase)

public:
	CListCtrlBase();
	virtual ~CListCtrlBase();
public:
	void SetIcons(UINT nBitmapID,int cx = 16, int nGrow = 1);

protected:
	CImageList	m_ImageList;
	CImageList* m_pDragImage;
	CWnd*		m_pOtherWnd;

	int			m_iDragSrc;
	bool		m_bDraged;
	CPoint		m_LeavePt;
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLvnBegindrag(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);

public:
	BOOL SetItemText(int nItem, int nSubItem, LPCTSTR lpszText);
	int DeleteItem(int iIndex);
	void SetCurSel(int index,BOOL bSelected = TRUE);
	int GetCurSel();

private:
	int m_CurSel;
public:
	afx_msg BOOL OnNMClick ( NMHDR * pNotifyStruct, LRESULT* pResult );

};