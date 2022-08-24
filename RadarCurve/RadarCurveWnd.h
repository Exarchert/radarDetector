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

	void SetSampleCount( int count );//���ò�������
	void SetSampleRatio( double value );//���ò���Ƶ��

	BOOL Create(const RECT& rect, CWnd* parent, UINT nID, LPCTSTR className = RadarCurve_CLASSNAME,
		DWORD dwStyle = WS_CHILD | WS_BORDER | WS_TABSTOP | WS_VISIBLE);

	void SetName( CString const& name );//���õ�ǰͨ�����ڵ���ʾ����
	//void SetRadiaIndex( int index);

	void setCorrection(int value);//�����ӳ�΢��������ֵ

	void StartDraw();//��ʼ����
	void EndDraw();//ֹͣ����

	void addData( RadarData *lpData );//�������ݼ����_radarDataVector
	void setData( RadarData *lpData );//���õ�ǰ����_currentRadarData
	//void deleteData( );
	COLORREF getColor( float radarValue );//�����״�����ֵ��ȡrgbֵ

	void clearData();//���_radarDataVector

protected:
	BOOL RegisterWindowClass();
	void DrawBk( CDC *lpDC );//���Ʊ���
	void DrawScaleFlag( CDC *lpDC );//�����������������Ԫ��
	void DrawCurve( CDC *lpDC );//���Ʋ���ͼ
	//void DrawCurveNew( CDC *lpDC );
	virtual void DrawSection( CDC *lpDC, int newData );//���Ʋ���ͼ
	//void DrawSection( CDC *lpDC );
	int mapRadarValueToXValue( float radarValue , float indexRatio);//�����״�����ֵradarValue�����״�����ֵ��������������ֵindexRatio���㲨��ͼXֵ
	//int mapTimeToYValue( float t );//����ʱ����Ƴ�yֵ
	int mapIndexToCurveYValue( int index, int sampleCount );//�����״����ݵ�������������Ʋ��͵�yֵ
	int mapIndexToSectionYValue( int index, int sampleCount );//�����״����ݵ�������������Ʋ��׵�yֵ

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

	int _staffMinY;//�Ϸ�������ʾ����
	int _staffMaxY;

	DWORD _maxTime;
	DWORD _minTime;


	int _startPos;//�����������
	int _scaleRatio;//�����������

	int _sampleCount;//��������
	double _sampleRatio;//����Ƶ��

	CDC *_lpDC;//����

	OpenThreads::Mutex _mutex;
	typedef std::vector<osg::ref_ptr<RadarData>> RadarDataVector;
	RadarDataVector _radarDataVector;//�������������ݵ�����
	RadarDataVector _tmpDataVector;//�������ӻ�������
	osg::ref_ptr<RadarData> _currentRadarData;

	bool _clickUp;
	bool _clickDown;


	CBrush _staffBrush;
	CPen _staffPen;
	CPen _markPen;
	CFont _textFont;

	CBmpDC _bmpDCSource;
	CBmpDC _bmpDCTmp;

	int _newData;//��Ҫ�»��Ƶ���������

	bool _needReinit;

	bool _dataAvailabled;

	int m_dCorrection;//�ӳ�΢��������ֵ

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


