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

	void SetSampleCount( int count );//设置采样点数
	void SetSampleRatio( double value );//设置采样频率

	BOOL Create(const RECT& rect, CWnd* parent, UINT nID, LPCTSTR className = RadarCurve_CLASSNAME,
		DWORD dwStyle = WS_CHILD | WS_BORDER | WS_TABSTOP | WS_VISIBLE);

	void SetName( CString const& name );//设置当前通道窗口的显示名称
	//void SetRadiaIndex( int index);

	void setCorrection(int value);//设置延迟微调的修正值

	void StartDraw();//开始绘制
	void EndDraw();//停止绘制

	void addData( RadarData *lpData );//把新数据加入进_radarDataVector
	void setData( RadarData *lpData );//设置当前数据_currentRadarData
	//void deleteData( );
	COLORREF getColor( float radarValue );//根据雷达数据值获取rgb值

	void clearData();//清空_radarDataVector

protected:
	BOOL RegisterWindowClass();
	void DrawBk( CDC *lpDC );//绘制背景
	void DrawScaleFlag( CDC *lpDC );//绘制用于线性增益的元素
	void DrawCurve( CDC *lpDC );//绘制波形图
	//void DrawCurveNew( CDC *lpDC );
	virtual void DrawSection( CDC *lpDC, int newData );//绘制波谱图
	//void DrawSection( CDC *lpDC );
	int mapRadarValueToXValue( float radarValue , float indexRatio);//根据雷达数据值radarValue，及雷达数据值序号与采样点数比值indexRatio计算波形图X值
	//int mapTimeToYValue( float t );//根据时间绘制出y值
	int mapIndexToCurveYValue( int index, int sampleCount );//根据雷达数据的序数计算出绘制波型的y值
	int mapIndexToSectionYValue( int index, int sampleCount );//根据雷达数据的序数计算出绘制波谱的y值

	CRect GetUpEllipse();
	CRect GetDownEllipse();

	
protected:
	DECLARE_MESSAGE_MAP()

protected:
	bool _drawing;
	int _curveCount;

	int _curveMinY;//波形线区域
	int _curveMaxY;
	int _curveMinX;
	int _curveMaxX;

	int _sectionMinX;//波谱图区域
	int _sectionMaxX;
	int _sectionMinY;
	int _sectionMaxY;

	int _staffMinY;//上方距离显示区域
	int _staffMaxY;

	DWORD _maxTime;
	DWORD _minTime;


	int _startPos;//线性增益相关
	int _scaleRatio;//线性增益相关

	int _sampleCount;//采样点数
	double _sampleRatio;//采样频率

	CDC *_lpDC;//画布

	OpenThreads::Mutex _mutex;
	typedef std::vector<osg::ref_ptr<RadarData>> RadarDataVector;
	RadarDataVector _radarDataVector;//用来接收新数据的数据
	RadarDataVector _tmpDataVector;//用来可视化的数组
	osg::ref_ptr<RadarData> _currentRadarData;

	bool _clickUp;
	bool _clickDown;


	CBrush _staffBrush;
	CPen _staffPen;
	CPen _markPen;
	CFont _textFont;

	CBmpDC _bmpDCSource;
	CBmpDC _bmpDCTmp;

	int _newData;//需要新绘制的数据数量

	bool _needReinit;

	bool _dataAvailabled;

	int m_dCorrection;//延迟微调的修正值

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


