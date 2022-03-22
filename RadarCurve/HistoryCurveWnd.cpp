#include "StdAfx.h"
#include "HistoryCurveWnd.h"

#define LINE_WIDTH 10


IMPLEMENT_DYNAMIC(HistoryCurveWnd, RadarCurveWnd)

HistoryCurveWnd::HistoryCurveWnd(void)
{
	RegisterWindowClass();

}

HistoryCurveWnd::~HistoryCurveWnd(void)
{
}
BEGIN_MESSAGE_MAP(HistoryCurveWnd, RadarCurveWnd)
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()

END_MESSAGE_MAP()

BOOL HistoryCurveWnd::RegisterWindowClass()
{
	WNDCLASS wndcls;
	//HINSTANCE hInst = AfxGetInstanceHandle();
	HINSTANCE hInst = AfxGetResourceHandle();

	if (!(::GetClassInfo(hInst, History_RadarCurve_CLASSNAME, &wndcls)))
	{
		// otherwise we need to register a new class
		wndcls.style            = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
		wndcls.lpfnWndProc      = ::DefWindowProc;
		wndcls.cbClsExtra       = wndcls.cbWndExtra = 0;
		wndcls.hInstance        = hInst;
		wndcls.hIcon            = NULL;
#ifndef _WIN32_WCE_NO_CURSOR
		wndcls.hCursor          = AfxGetApp()->LoadStandardCursor(IDC_ARROW);
#else
		wndcls.hCursor          = 0;
#endif
		wndcls.hbrBackground    = (HBRUSH) (COLOR_3DFACE + 1);
		wndcls.lpszMenuName     = NULL;
		wndcls.lpszClassName    = History_RadarCurve_CLASSNAME;

		if (!AfxRegisterClass(&wndcls))
		{
			AfxThrowResourceException();
			return FALSE;
		}
	}

	return TRUE;
}

BOOL HistoryCurveWnd::Create(const RECT& rect, CWnd* parent, UINT nID, DWORD dwStyle )
{
	ASSERT(parent->GetSafeHwnd());

	if (!RadarCurveWnd::Create(rect, parent, nID, History_RadarCurve_CLASSNAME, dwStyle ))
		return FALSE;

	return TRUE;
}
void HistoryCurveWnd::EnableScrollBars(int nBar, BOOL bEnable /*=TRUE*/)
{
	if (bEnable)
	{
		if (!IsVisibleHScroll() && (nBar == SB_HORZ || nBar == SB_BOTH))
		{
			m_nBarState |= GVL_HORZ;
			CWnd::EnableScrollBarCtrl(SB_HORZ, bEnable);
		}

		if (!IsVisibleVScroll() && (nBar == SB_VERT || nBar == SB_BOTH))
		{
			m_nBarState |= GVL_VERT;
			CWnd::EnableScrollBarCtrl(SB_VERT, bEnable);
		}
	}
	else
	{
		if ( IsVisibleHScroll() && (nBar == SB_HORZ || nBar == SB_BOTH))
		{
			m_nBarState &= ~GVL_HORZ; 
			CWnd::EnableScrollBarCtrl(SB_HORZ, bEnable);
		}

		if ( IsVisibleVScroll() && (nBar == SB_VERT || nBar == SB_BOTH))
		{
			m_nBarState &= ~GVL_VERT;
			CWnd::EnableScrollBarCtrl(SB_VERT, bEnable);
		}
	}
}

void HistoryCurveWnd::ResetScrollBars( int virWidth, int virHeight )
{
	if (!::IsWindow(GetSafeHwnd())) 
		return;

	CRect rect;

	GetClientRect(rect);



	if (rect.left == rect.right || rect.top == rect.bottom)
		return;

	if (IsVisibleVScroll())
		rect.right += GetSystemMetrics(SM_CXVSCROLL) + GetSystemMetrics(SM_CXBORDER);

	if (IsVisibleHScroll())
		rect.bottom += GetSystemMetrics(SM_CYHSCROLL) + GetSystemMetrics(SM_CYBORDER);

	if (rect.left >= rect.right || rect.top >= rect.bottom)
	{
		EnableScrollBarCtrl(SB_BOTH, FALSE);
		return;
	}

	CRect VisibleRect(0, 0, 
		rect.right - GRID_WIDTH, rect.bottom);
	CRect VirtualRect(0, 0,	virWidth, virHeight);

	if (VisibleRect.Height() < VirtualRect.Height())
		VisibleRect.right -= ::GetSystemMetrics(SM_CXVSCROLL);
	// If horz scroll bar, vert space is reduced
	if (VisibleRect.Width() < VirtualRect.Width())
		VisibleRect.bottom -= ::GetSystemMetrics(SM_CYHSCROLL);



	if (VisibleRect.Height() < VirtualRect.Height())
	{
		EnableScrollBars(SB_VERT, TRUE); 
		_maxVirtualHeight = VirtualRect.Height() - 1;
	}
	else
	{
		EnableScrollBars(SB_VERT, FALSE); 
		_maxVirtualHeight = 0;
	}

	if (VisibleRect.Width() < VirtualRect.Width())
	{
		EnableScrollBars(SB_HORZ, TRUE); 
		_maxVirtualWidth = VirtualRect.Width() - 1;
	}
	else
	{
		EnableScrollBars(SB_HORZ, FALSE); 
		_maxVirtualWidth = 0;
	}

	ASSERT( _maxVirtualHeight< INT_MAX/* && m_nHScrollMax < INT_MAX*/); // This should be fine


	// New code - Paul Runstedler 
	SCROLLINFO si;
	si.cbSize = sizeof(SCROLLINFO);
	si.fMask = SIF_PAGE | SIF_RANGE;
	si.nPage = (_maxVirtualWidth>0)? VisibleRect.Width() : 0;
	si.nMin = 0;
	si.nMax = _maxVirtualWidth;
	SetScrollInfo(SB_HORZ, &si, TRUE);

	si.fMask |= SIF_DISABLENOSCROLL;
	si.nPage = (_maxVirtualHeight>0)? VisibleRect.Height() : 0;
	si.nMin = 0;
	si.nMax = _maxVirtualHeight;
	SetScrollInfo(SB_VERT, &si, TRUE);
}


void HistoryCurveWnd::OnHScroll(UINT nSBCode, UINT /*nPos*/, CScrollBar* /*pScrollBar*/)
{
	int scrollPos = GetScrollPos32(SB_HORZ);

	CRect rect;
	GetClientRect(rect);

	switch (nSBCode)
	{
	case SB_LINERIGHT:
		if (scrollPos < _maxVirtualWidth )
		{
			// may have contiguous hidden columns.  Blow by them
			int xScroll = LINE_WIDTH;
			SetScrollPos32(SB_HORZ, scrollPos + xScroll);
			if (GetScrollPos32(SB_HORZ) == scrollPos)
				break;          // didn't work

			rect.left = 0;
			InvalidateRect(rect);
		}
		break;

	case SB_LINELEFT:
		if ( scrollPos > 0 )
		{
			int xScroll = LINE_WIDTH;
			SetScrollPos32(SB_HORZ, __max(0, scrollPos - xScroll));
			rect.left = 0;
			InvalidateRect(rect);
		}
		break;

	case SB_PAGERIGHT:
		if (scrollPos < _maxVirtualWidth)
		{
			rect.left = 0;
			int offset = rect.Width() - 10;
			int pos = min( _maxVirtualWidth, scrollPos + offset);
			SetScrollPos32(SB_HORZ, pos);
			rect.left = 0;
			InvalidateRect(rect);
		}
		break;

	case SB_PAGELEFT:
		if (scrollPos > 0)
		{
			rect.left = 0;
			int offset = -rect.Width() + 10;
			int pos = __max(0, scrollPos + offset);
			SetScrollPos32(SB_HORZ, pos);
			rect.left = 0;
			InvalidateRect(rect);
		}
		break;

	case SB_THUMBPOSITION:
	case SB_THUMBTRACK:
		{
			SetScrollPos32(SB_HORZ, GetScrollPos32(SB_HORZ, TRUE));
			InvalidateRect(rect);
		}
		break;

	case SB_LEFT:
		if (scrollPos > 0)
		{
			SetScrollPos32(SB_HORZ, 0);
			Invalidate();
		}
		break;

	case SB_RIGHT:
		if (scrollPos < _maxVirtualWidth)
		{
			SetScrollPos32(SB_HORZ, _maxVirtualWidth);
			Invalidate();
		}
		break;


	default: 
		break;
	}
}

void HistoryCurveWnd::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar *pScrollBar)
{
	int scrollPos = GetScrollPos32(SB_VERT);

	CRect rect;
	GetClientRect(rect);

	switch (nSBCode)
	{
	case SB_LINEDOWN:
		if (scrollPos < _maxVirtualWidth )
		{
			// may have contiguous hidden columns.  Blow by them
			int yScroll = LINE_WIDTH;
			SetScrollPos32(SB_VERT, scrollPos + yScroll);
			if (GetScrollPos32(SB_VERT) == scrollPos)
				break;          // didn't work

			rect.left = 0;
			InvalidateRect(rect);
		}
		break;

	case SB_LINEUP:
		if ( scrollPos > 0 )
		{
			int yScroll = LINE_WIDTH;
			SetScrollPos32(SB_VERT, __max(0, scrollPos - yScroll));
			rect.left = 0;
			InvalidateRect(rect);
		}
		break;

	case SB_PAGEDOWN:
		if (scrollPos < _maxVirtualWidth)
		{
			rect.top = 0;
			int offset = rect.Height() - 10;
			int pos = min( _maxVirtualWidth, scrollPos + offset);
			SetScrollPos32(SB_VERT, pos);
			rect.top = 0;
			InvalidateRect(rect);
		}
		break;

	case SB_PAGEUP:
		if (scrollPos > 0)
		{
			rect.top = 0;
			int offset = -rect.Height() + 10;
			int pos = __max(0, scrollPos + offset);
			SetScrollPos32(SB_VERT, pos);
			rect.top = 0;
			InvalidateRect(rect);
		}
		break;

	case SB_THUMBPOSITION:
	case SB_THUMBTRACK:
		{
			SetScrollPos32(SB_VERT, GetScrollPos32(SB_VERT, TRUE));
			InvalidateRect(rect);
		}
		break;

	case SB_TOP:
		if (scrollPos > 0)
		{
			SetScrollPos32(SB_VERT, 0);
			Invalidate();
		}
		break;

	case SB_BOTTOM:
		if (scrollPos < _maxVirtualWidth)
		{
			SetScrollPos32(SB_VERT, _maxVirtualWidth);
			Invalidate();
		}
		break;


	default: 
		break;
	}
}

int HistoryCurveWnd::GetScrollPos32(int nBar, BOOL bGetTrackPos /* = FALSE */)
{
	SCROLLINFO si;
	si.cbSize = sizeof(SCROLLINFO);

	if (bGetTrackPos)
	{
		if (GetScrollInfo(nBar, &si, SIF_TRACKPOS))
			return si.nTrackPos;
	}
	else
	{
		if (GetScrollInfo(nBar, &si, SIF_POS))
			return si.nPos;
	}

	return 0;
}

BOOL HistoryCurveWnd::SetScrollPos32(int nBar, int nPos, BOOL bRedraw /* = TRUE */)
{
	SCROLLINFO si;
	si.cbSize = sizeof(SCROLLINFO);
	si.fMask  = SIF_POS;
	si.nPos   = nPos;
	return SetScrollInfo(nBar, &si, bRedraw);
}

BOOL HistoryCurveWnd::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	return TRUE;
	return CWnd::OnEraseBkgnd(pDC);
}
// void HistoryCurveWnd::OnPaint()
// {
// 	RadarCurveWnd::OnPaint();
// }
void HistoryCurveWnd::setRadarData( unsigned int *buff, int width, int height )
{
	_dataBmp.DeleteObject();
	_dataBmp.CreateBitmap( width, height, 1, 32, buff );

	ResetScrollBars( width, height );

	_drawing = true;
}
void HistoryCurveWnd::DrawSection( CDC *lpDC , int newData)
{
	CRect rc;
	CDC dcMem;
	GetClientRect(&rc);
	CBitmap bmp; 

	dcMem.CreateCompatibleDC(lpDC);

	bmp.CreateCompatibleBitmap(lpDC,rc.Width(),rc.Height());
	CBitmap *pOldBit=dcMem.SelectObject(&_dataBmp);



	int scrollPos = GetScrollPos32(SB_HORZ);
	int ypos = GetScrollPos32( SB_VERT );
	lpDC->StretchBlt( 0, 0, rc.Width() - GRID_WIDTH, rc.Height(), &dcMem, scrollPos, ypos, rc.Width() - GRID_WIDTH, rc.Height(), SRCCOPY );

}
unsigned int HistoryCurveWnd::GetHitPos( CPoint const& p )
{
	CPoint cp = p;
	ScreenToClient( &cp );
	int scrollPos = GetScrollPos32(SB_HORZ);
	return cp.x + scrollPos;
}