/*************************************************************************
FileName：CTabCtrlBase.h CTabCtrlBase.cpp
Author:	Liugh
Date：2010-01-20
Description：Tab Ctrl Base Class
**************************************************************************/
#pragma once

/////////////////////////////////////////////////////////////////////////////
// CTabCtrlBase window


/***********************************************/
//拖动图标离开窗口后发送该消息

#define WM_NOTIFY_TABCTRLCHANGED WM_USER + 200

/***********************************************/

class CTabCtrlBase : public CTabCtrl
{
public:
	CTabCtrlBase();

	virtual ~CTabCtrlBase();

	void AddItem(CWnd *pwnd,LPTSTR name);//添加选项卡

public:

	int		m_ispeed;		//运动速度
	BOOL	m_bMove;		//是否运动
	int		m_wndwidth;		//子窗体的宽度
	CPoint	m_bottomright;	//子窗体右下角的坐标
	CPoint	m_topleft;		//子窗体左上角的坐标
	CWnd *	m_pCurWnd;		//当前窗体
	CRect	m_tabrect;		//Tab的矩形区域
	CPoint	m_ptTabs;		//左上角坐标

	int m_itemcount;//Item总数

protected:

	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnSelchange(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchanging(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

	DECLARE_MESSAGE_MAP()
};