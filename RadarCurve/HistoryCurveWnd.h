#pragma once
#include "RadarCurveWnd.h"

#define History_RadarCurve_CLASSNAME _T("HistoryCurveWnd")

#define GVL_NONE                0L      // Neither
#define GVL_HORZ                1L      // Horizontal line or scrollbar
#define GVL_VERT                2L      // Vertical line or scrollbar
#define GVL_BOTH                3L      // Both

class HistoryCurveWnd : public RadarCurveWnd
{
	DECLARE_DYNAMIC(HistoryCurveWnd)
public:
	HistoryCurveWnd(void);
	~HistoryCurveWnd(void);
	BOOL Create(const RECT& rect, CWnd* parent, UINT nID,
		DWORD dwStyle = WS_CHILD | WS_BORDER | WS_TABSTOP | WS_VISIBLE);

	void setRadarData( unsigned int *buff, int width, int height );
	void ResetScrollBars( int virWidth, int virHeight );

	unsigned int GetHitPos( CPoint const& p );

protected:
	virtual void DrawSection( CDC *lpDC, int newData );
	BOOL RegisterWindowClass();
	void EnableScrollBars(int nBar, BOOL bEnable =TRUE);
	BOOL IsVisibleVScroll() { return ( (m_nBarState & GVL_VERT) > 0); } 
	BOOL IsVisibleHScroll() { return ( (m_nBarState & GVL_HORZ) > 0); }
	int  GetScrollPos32(int nBar, BOOL bGetTrackPos = FALSE);
	BOOL SetScrollPos32(int nBar, int nPos, BOOL bRedraw = TRUE);
protected:
	int		m_nBarState;	// 是不是显示滚动条

	int		_maxVirtualWidth;
	int		_maxVirtualHeight;

	CBitmap _dataBmp;
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar *pScrollBar);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
/*	afx_msg void OnPaint();*/
};
