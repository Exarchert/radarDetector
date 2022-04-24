#pragma once
#include <vector>
#include <RadarCurve/RadarData.h>
#include <OpenThreads/Mutex>
#include <osg/ref_ptr>

#include "BmpDC.h"

// Radar3DWnd
#define Radar3D_CLASSNAME    _T("Radar3DWnd")  // Window class name
#define GRID_WIDTH 60
#define WM_CLICK_ON_SECTION WM_USER+1002

class Radar3DWnd : public CWnd
{
	DECLARE_DYNAMIC(Radar3DWnd)
public:
	Radar3DWnd();
	virtual ~Radar3DWnd();

	void SetSampleCount( int count );
	void SetSampleRatio( double value );
	void SetDielectric( float value );

	BOOL Create(const RECT& rect, CWnd* parent, UINT nID, LPCTSTR className = Radar3D_CLASSNAME,
		DWORD dwStyle = WS_CHILD | WS_BORDER | WS_TABSTOP | WS_VISIBLE);

	void SetName( CString const& name );
	void SetRadiaIndex( int index);

	void setCorrection( int value, int index );
	void setChannelIndex( int value );
	void setDepthIndex( int value );

	void StartDraw();
	void EndDraw();

	void addGroupData( std::vector<osg::ref_ptr<RadarData>> radarDataGroup );
	void addData( RadarData *lpData );
	void setData( RadarData *lpData );
	//void deleteData( );
	COLORREF getColor( float radarValue );

	void clearData();

protected:
	BOOL RegisterWindowClass();
	void resetXY();
	void DrawBk( CDC *lpDC );
	//void DrawScale( CDC *lpDC );
	void DrawScaleFlag( CDC *lpDC );
	void DrawCurve( CDC *lpDC );
	void DrawCurveNew( CDC *lpDC );
	virtual void DrawSection( CDC *lpDC, int newData, int channelIndex, int depthIndex );
	void drawFrontView( CDC *lpDC, int channelIndex, int depthIndex);
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

	int _curveMinY;//����������
	int _curveMaxY;
	int _curveMinX;
	int _curveMaxX;

	int _sectionMinX;//����ͼ����
	int _sectionMaxX;
	int _sectionMinY;
	int _sectionMaxY;

	int _staffMinY;//������ʾ����
	int _staffMaxY;

	//����ͼ����
	int m_dFrontViewSectionMinX;
	int m_dFrontViewSectionMaxX;
	int m_dFrontViewSectionMinY;
	int m_dFrontViewSectionMaxY;

	//����������
	int m_dVerticalSectionMinX;
	int m_dVerticalSectionMaxX;
	int m_dVerticalSectionMinY;
	int m_dVerticalSectionMaxY;

	//����������
	int m_dHorizontalSectionMinX;
	int m_dHorizontalSectionMaxX;
	int m_dHorizontalSectionMinY;
	int m_dHorizontalSectionMaxY;

	//����̶�����
	int m_dDistanceScaleMinX;
	int m_dDistanceScaleMaxX;
	int m_dDistanceScaleMinY;
	int m_dDistanceScaleMaxY;

	//��ȿ̶�����
	int m_dDepthScaleMinX;
	int m_dDepthScaleMaxX;
	int m_dDepthScaleMinY;
	int m_dDepthScaleMaxY;

	//ʱ���̶�����
	int m_dTimeWindowScaleMinX;
	int m_dTimeWindowScaleMaxX;
	int m_dTimeWindowScaleMinY;
	int m_dTimeWindowScaleMaxY;


	DWORD _maxTime;
	DWORD _minTime;


	int _startPos;
	int _scaleRatio;

	int _sampleCount;
	double _sampleRatio;
	float m_fDielectric;

	int m_dVecCorrection[12];

	int m_dChannelIndex;
	int m_dDepthIndex;

	CDC *_lpDC;

	OpenThreads::Mutex _mutex;
	typedef std::vector<osg::ref_ptr<RadarData>> RadarDataVector;
	RadarDataVector _radarDataVector;
	RadarDataVector _tmpDataVector;
	osg::ref_ptr<RadarData> _currentRadarData;

	std::vector<std::vector<osg::ref_ptr<RadarData>>> m_vecRadarGroupData;
	std::vector<std::vector<osg::ref_ptr<RadarData>>> m_vecRadarGroupDataLatest;

	bool _clickUp;
	bool _clickDown;

	CBrush m_scaleBrush;
	CPen m_scalePen;

	CBrush _staffBrush;
	CPen _staffPen;
	CPen _markPen;
	CFont _textFont;

	CBmpDC _bmpDCSource;
	CBmpDC _bmpDCTmp;

	int _newData;

	bool _needReinit;

	bool _dataAvailabled;

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


