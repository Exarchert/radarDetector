#pragma once
#include <vector>
#include <RadarCurve/RadarData.h>
#include <OpenThreads/Mutex>
#include <osg/ref_ptr>

#include "BmpDC.h"

// RadarCurveWnd
#define RadarCurve_CLASSNAME    _T("RadarCurveWnd")  // Window class name
#define GRID_WIDTH 60
#define WM_CLICK_ON_SECTION WM_USER+1002

class RadarCurveWnd : public CWnd
{
	DECLARE_DYNAMIC(RadarCurveWnd)
public:
	RadarCurveWnd();
	virtual ~RadarCurveWnd();

	void SetSampleCount( int count );
	void SetSampleRatio( double value );

	BOOL Create(const RECT& rect, CWnd* parent, UINT nID, LPCTSTR className = RadarCurve_CLASSNAME,
		DWORD dwStyle = WS_CHILD | WS_BORDER | WS_TABSTOP | WS_VISIBLE);

	void SetName( CString const& name );
	void SetRadiaIndex( int index);

	void setCorrection(int value);

	void StartDraw();
	void EndDraw();

	void addData( RadarData *lpData );
	void setData( RadarData *lpData );
	//void deleteData( );
	COLORREF getColor( float radarValue );

	void clearData();

protected:
	BOOL RegisterWindowClass();
	void DrawBk( CDC *lpDC );
	void DrawScaleFlag( CDC *lpDC );
	void DrawCurve( CDC *lpDC );
	void DrawCurveNew( CDC *lpDC );
	virtual void DrawSection( CDC *lpDC, int newData );
	//void DrawSection( CDC *lpDC );
	int mapRadarValueToXValue( float radarValue , float temp);
	int mapTimeToYValue( float t );
	int mapIndexToCurveYValue( int index, int dataCount );
	int mapIndexToSectionYValue( int index, int dataCount );

	CRect GetUpEllipse();
	CRect GetDownEllipse();

	
protected:
	DECLARE_MESSAGE_MAP()

protected:
	bool _drawing;
	int _curveCount;

	int _curveMinY;//≤®–Œœﬂ«¯”Ú
	int _curveMaxY;

	int _curveMinX;
	int _curveMaxX;

	int _sectionMinX;//≤®∆’Õº«¯”Ú
	int _sectionMaxX;

	int _sectionMinY;
	int _sectionMaxY;

	int _staffMinY;//æ‡¿Îœ‘ æ«¯”Ú
	int _staffMaxY;

	DWORD _maxTime;
	DWORD _minTime;


	int _startPos;
	int _scaleRatio;

	int _sampleCount;
	double _sampleRatio;

	CDC *_lpDC;

	OpenThreads::Mutex _mutex;
	typedef std::vector<osg::ref_ptr<RadarData>> RadarDataVector;
	RadarDataVector _radarDataVector;
	RadarDataVector _tmpDataVector;
	osg::ref_ptr<RadarData> _currentRadarData;

	bool _clickUp;
	bool _clickDown;


	CBrush _staffBrush;
	CPen _staffPen;
	CPen _markPen;
	CFont _textFont;

	CBmpDC _bmpDCSource;
	CBmpDC _bmpDCTmp;

	int _newData;

	bool _needReinit;

	bool _dataAvailabled;

	int _dCorrection;

public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnSize(UINT nType, int cx, int cy);
};


