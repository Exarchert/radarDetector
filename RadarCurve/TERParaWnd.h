#pragma once
#include <vector>
#include <RadarCurve/RadarData.h>
#include <OpenThreads/Mutex>
#include <osg/ref_ptr>

#include "BmpDC.h"

// TERParaWnd
#define TERPARA_CLASSNAME    _T("TERParaWnd")  // Window class name
#define WM_CLICK_ON_SECTION WM_USER+1002

class TERParaWnd : public CWnd
{
	DECLARE_DYNAMIC(TERParaWnd)
public:
	TERParaWnd();
	virtual ~TERParaWnd();

	BOOL Create(const RECT& rect, CWnd* parent, UINT nID, LPCTSTR className = TERPARA_CLASSNAME, DWORD dwStyle = WS_CHILD | WS_BORDER | WS_TABSTOP | WS_VISIBLE);

	void SetSenderFrequency( float fFrequency );
	void SetSenderDutyRatio( int nDutyRatio );
	void SetReceiverSampleCount( int nSampleCount );
	void SetReceiverSampleRatio( int nSampleRatio );
	void SetReceiverDelay( int nDelay );
	
	void StartDraw();
	void EndDraw();

	void SetSampleCount( int count );
	void SetSampleRatio( double value );
	
	//void StartDraw();
	//void EndDraw();

	void SetName( CString const& name );
	void SetRadiaIndex( int index);

	void SetCorrection(int value);

	void addData( RadarData *lpData );
	COLORREF getColor( float radarValue );

	void clearData();

protected:
	BOOL RegisterWindowClass();
	void DrawLegend( CDC *lpDC );
	void DrawCoordinateAxis( CDC *lpDC );
	void DrawWave( CDC *lpDC );
	void DrawTimeWindow( CDC *lpDC );


	void DrawBk( CDC *lpDC );
	void DrawScaleFlag( CDC *lpDC );
	void DrawCurve( CDC *lpDC );
	virtual void DrawSection( CDC *lpDC, int newData );
	//void DrawSection( CDC *lpDC );
	int mapRadarValueToXValue( float radarValue );
	int mapTimeToYValue( float t );
	int mapIndexToCurveYValue( int index, int dataCount );
	int mapIndexToSectionYValue( int index, int dataCount );

	CRect GetUpEllipse();
	CRect GetDownEllipse();


protected:
	DECLARE_MESSAGE_MAP()

protected:
	CDC *_lpDC;//����
	bool m_bNeedReinit;
	bool m_bDrawing;
	bool _drawing;

	float m_fSenderFrequency;//�����Ƶ��
	int m_nSenderDutyRatio;//�����ռ�ձ�
	int m_nReceiverSampleCount;//���ջ���������
	int m_nReceiverSampleRatio;//���ջ�������
	int m_nReceiverDelay;//���ջ���ʱ
	

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

	DWORD _maxTime;
	DWORD _minTime;


	int _startPos;
	int _scaleRatio;

	int _sampleCount;
	double _sampleRatio;


	OpenThreads::Mutex _mutex;
	typedef std::vector<osg::ref_ptr<RadarData>> RadarDataVector;
	RadarDataVector _radarDataVector;
	RadarDataVector _tmpDataVector;


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

	int m_dCorrection;

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
