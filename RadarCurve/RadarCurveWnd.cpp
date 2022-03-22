// RadarCurveWnd.cpp : 实现文件
//

#include "stdafx.h"
#include "RadarCurve.h"
#include "RadarCurveWnd.h"
#include <stdlib.h>
#include <float.h>
#include <OpenThreads/ScopedLock>
#include <RadarCurve/ConfigureSet.h>
#include <RadarCurve/RadarManager.h>

// RadarCurveWnd

IMPLEMENT_DYNAMIC(RadarCurveWnd, CWnd)

#define STAFF_HEIGHT 30 //距离刻度区域高度
#define GRID_SPACE 15
#define POINT_SPACE 10
#define DATA_BLOCK_HEIGHT 40.0f
#define DATA_BLOCK_WIDTH 20.0f

#define TIME_STAFF_WIDTH 20 //时窗刻度区域宽度

#define CUR_STAFF_INTER_PERCENT 0.955

#define CUR_MARK_INTER 0.05

#define RADIUS 10

#define MAX_DRAW_COUNT 3000


RadarCurveWnd::RadarCurveWnd()
{
	RegisterWindowClass();

	_curveMinY = 0;
	_curveMaxY = 0;

	_drawing = false;


	_startPos = 0;
	_scaleRatio = 1;

	_clickUp = false;
	_clickDown = false;

	_staffBrush.CreateSolidBrush( RGB ( 255, 255, 255 ) );

	_staffPen.CreatePen( PS_SOLID, 1, RGB ( 0, 0, 0 ) );

	_markPen.CreatePen( PS_SOLID, 1, RGB ( 255, 255, 255 ) );

	_textFont.CreateFont( 12, 0, 0, 0, 0, FALSE, FALSE, FALSE 
		, 0, 0, 0, 0, 0, L"仿宋"
		);

	_sampleCount = 0;
	_sampleRatio = 51.2;

	_lpDC = NULL;
	//_currentRadarData = NULL;

	_newData = 0;
	_needReinit = false;
	_dataAvailabled = false;

	_dCorrection=0;
}

RadarCurveWnd::~RadarCurveWnd()
{
}

BOOL RadarCurveWnd::RegisterWindowClass()
{
	WNDCLASS wndcls;
	//HINSTANCE hInst = AfxGetInstanceHandle();
	HINSTANCE hInst = AfxGetResourceHandle();

	if (!(::GetClassInfo(hInst, RadarCurve_CLASSNAME, &wndcls)))
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
		wndcls.lpszClassName    = RadarCurve_CLASSNAME;

		if (!AfxRegisterClass(&wndcls))
		{
			AfxThrowResourceException();
			return FALSE;
		}
	}

	return TRUE;
}




BEGIN_MESSAGE_MAP(RadarCurveWnd, CWnd)
	ON_WM_TIMER()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_ERASEBKGND()
	ON_WM_KEYDOWN()
	ON_WM_SIZE()
END_MESSAGE_MAP()


BOOL RadarCurveWnd::Create(const RECT& rect, CWnd* parent, UINT nID, LPCTSTR className, DWORD dwStyle )
{
    ASSERT(parent->GetSafeHwnd());

	if (!CWnd::Create(className, NULL, dwStyle, rect, parent, nID))
		return FALSE;

	return TRUE;
}



// RadarCurveWnd 消息处理程序

void RadarCurveWnd::SetSampleCount( int count )
{
	_sampleCount = count;
}
void RadarCurveWnd::SetSampleRatio( double value )
{
	_sampleRatio = value;
}
void RadarCurveWnd::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CWnd::OnTimer(nIDEvent);

	Invalidate();
}

void RadarCurveWnd::OnPaint()
{
	//CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CWnd::OnPaint()
	
	////////////////////////////////////////////////////////////////////////////
	//SYSTEMTIME time;
	//GetSystemTime(&time);
	//int begin = time.wMilliseconds;
	//int second = time.wSecond;
	//int end = 0;
	////////////////////////////////////////////////////////////////////////////

	if (!_lpDC)
	{
		_lpDC = new CPaintDC(this);
	}
	CRect rc;
	GetClientRect(&rc);

	PAINTSTRUCT pat;
	pat.hdc = _lpDC->GetSafeHdc();
	pat.fErase = true;
	
	pat.rcPaint = rc;
	
	BeginPaint( &pat );
	
	CDC *pDC = _lpDC;



	CDC dcMem;
	
	CBitmap bmp; 
	dcMem.CreateCompatibleDC(pDC); 
	bmp.CreateCompatibleBitmap(pDC,rc.Width(),rc.Height());
	CBitmap *pOldBit=dcMem.SelectObject(&bmp);
	dcMem.FillSolidRect(rc,RGB(240,240,240));
	DrawBk(&dcMem);

	int newData = 0;
	{
		OpenThreads::ScopedLock<OpenThreads::Mutex> lock(_mutex);
		_tmpDataVector = _radarDataVector;
		newData = _newData;
		_newData = 0;
	}

	if ( _drawing )
	{
		//DrawCurveNew(&dcMem);//波形图
		DrawCurve(&dcMem);//波形图
		//DrawSection( &dcMem, newData );//波普图
	}
	//DrawScaleFlag( &dcMem );//20210803hjl
	pDC->BitBlt(0,0,rc.Width(),rc.Height(),&dcMem,0,0,SRCCOPY);//20210803hjl
	dcMem.DeleteDC();   
	bmp.DeleteObject(); 
	EndPaint( &pat );
}


void RadarCurveWnd::DrawBk( CDC *lpDC )//绘制背景   黑底红绿线
{
	CRect rc;
	GetClientRect(&rc);

	int width = GRID_WIDTH;//波形图宽度
	//int height = rc.Height();
	int curvRectX1 = rc.Width() - GRID_WIDTH - TIME_STAFF_WIDTH;//波形图X1  TIME_STAFF_WIDTH时窗刻度区域宽度
	int curvRectX2 = curvRectX1 + GRID_WIDTH;//波形图X2
	int curvRectY1 = STAFF_HEIGHT;//STAFF_HEIGHT距离刻度区域高度
	int curvRectY2 = rc.Height();


	//CBrush blackBruh( RGB( 0, 0, 0 ) );//波谱背景黑色
	///*CRect r;
	//r.left = 0;
	//r.right = curvRectX1;
	//r.top = 0;
	//r.bottom = r.top + rc.Height();*/
	//CRect r(0,0,curvRectX1,rc.Height());
	//lpDC->FillRect( r, &blackBruh );


	/*-----------------------开始刻画波形图背景-----------------------*/
	CBrush brushCurvRect(RGB(75,75,75));//波形图背景浅黑色
	CRect rectCurvRect(curvRectX1,curvRectY1 ,curvRectX2,curvRectY2);
	lpDC->FillRect(rectCurvRect,&brushCurvRect);
	//算多少条线
	int xLineCount = ( curvRectX2 - curvRectX1 ) / GRID_SPACE;
	int yLineCount = ( curvRectY2 - curvRectY1 ) / GRID_SPACE ;
	int curvRectInterP5 = xLineCount / 10.0 *3.0 ;

	//bg-grid-vertical 背景网格中的绿竖线
	CPen pen_grid_vert(PS_DOT,1,RGB(105,255,105));//绿色竖线
	lpDC->SelectObject(pen_grid_vert);
	lpDC->SetBkMode(TRANSPARENT);
	for(int i=1;i<xLineCount;++i){
		lpDC->MoveTo(curvRectX1 + i * GRID_SPACE, curvRectY1);
		lpDC->LineTo(curvRectX1 + i * GRID_SPACE, curvRectY2);
	}

	//bg-grid-mid 背景网格中的红中线
	CPen pen_grid_mid(PS_DOT,1,RGB(255,105,105));//红色中线
	lpDC->SelectObject(pen_grid_mid);
	lpDC->MoveTo(curvRectX1+GRID_SPACE/2.0*xLineCount,curvRectY1);
	lpDC->LineTo(curvRectX1+GRID_SPACE/2.0*xLineCount,curvRectY2);

	//bg-grid-horizon 背景网格中的绿横线
	CPen pen_grid_hori(PS_DOT,1,RGB(105,255,105));//绿色横线
	lpDC->SelectObject(pen_grid_hori);
	lpDC->SetBkMode(TRANSPARENT);
	for(int i=1;i<=yLineCount;++i)
	{
		lpDC->MoveTo(curvRectX1,curvRectY1+i * GRID_SPACE);
		lpDC->LineTo(curvRectX2,curvRectY1+i * GRID_SPACE);
	}

	//右排刻度 时窗
	int staffWidth = TIME_STAFF_WIDTH;//右排时窗刻度宽度
	//刻度处的白底
	//lpDC->FillSolidRect( curvRectX2, curvRectY1, TIME_STAFF_WIDTH, curvRectY2 - curvRectY1, RGB( 255, 255, 255 ) );//(左上x、y,宽度，高度，颜色)
	lpDC->FillSolidRect( curvRectX2, curvRectY1, TIME_STAFF_WIDTH, curvRectY2 - curvRectY1, RGB( 240, 240, 240 ) );//(左上x、y,宽度，高度，颜色)
	//参数
	int curStaffCxLen = staffWidth / 3;
	CString strMark;
	CPen pen_staff_vline(PS_SOLID,1,RGB(0,0,0));
	lpDC->SelectObject(pen_staff_vline);
	int staffStartX = curvRectX2 + 2;
	//竖直黑线
	lpDC->MoveTo(staffStartX,curvRectY1);
	lpDC->LineTo(staffStartX,curvRectY2);
	//横黑线
	int count = int( 1 / _sampleRatio * _sampleCount + 0.5 );//横线数量
	float staffYSpace = (curvRectY2 - curvRectY1) * 1.0f / count;//间隔
	CFont *lpOldFont = lpDC->SelectObject( &_textFont );
	count += 1;
	//纵向刻度尺中的线及数字
	for(int i = 0;i <= count;i ++ ){
		if ( i % 10 == 0 ){//纵向刻度尺中的水平长线及数字
			if ( i == count ){
				lpDC->MoveTo(staffStartX, curvRectY1 + i*staffYSpace - 1);
				lpDC->LineTo(staffStartX+curStaffCxLen, curvRectY1 + i*staffYSpace - 1);
			}else{
				lpDC->MoveTo(staffStartX, curvRectY1 + i*staffYSpace);
				lpDC->LineTo(staffStartX + curStaffCxLen, curvRectY1 + i*staffYSpace);
			}
			strMark.Format(L"%d", i);	
			if ( i == 0 ){
				lpDC->TextOut(staffStartX + curStaffCxLen + 1, curvRectY1 + i * staffYSpace, strMark);
			}else if ( i / 2 == count / 2 ){
				lpDC->TextOut(staffStartX + curStaffCxLen + 1, curvRectY1 + i * staffYSpace - 12, strMark);
			}else{
				lpDC->TextOut(staffStartX + curStaffCxLen + 1, curvRectY1 + i * staffYSpace - 6, strMark);
			}
		}else{//纵向刻度尺中的水平短线
			if ( i == count ){
				lpDC->MoveTo(staffStartX, curvRectY1 + i*staffYSpace - 1);
				lpDC->LineTo(staffStartX+curStaffCxLen / 2, curvRectY1 + i*staffYSpace - 1);
			}else{
				lpDC->MoveTo(staffStartX, curvRectY1 + i*staffYSpace);
				lpDC->LineTo(staffStartX+curStaffCxLen / 2, curvRectY1 + i*staffYSpace);
			}
		}
	}
	/*-----------------------结束波形图背景刻画-----------------------*/

	lpDC->SelectObject( lpOldFont );
	//波形图区域XY
	_curveMinX = curvRectX1;
	_curveMaxX = curvRectX2;
	_curveMinY = curvRectY1;
	_curveMaxY = curvRectY2;
	//波谱图区域XY
	_sectionMinX = 0;
	_sectionMaxX = curvRectX1;
	_sectionMinY = curvRectY1;
	_sectionMaxY = curvRectY2;
	//距离刻度区域Y(X与波普图一样)
	_staffMinY = 0;
	_staffMaxY = curvRectY1;
}

void RadarCurveWnd::DrawScaleFlag( CDC *lpDC )
{
	int radius = 10;
	CRect upEllipse = GetUpEllipse();
	CRect downEllipse = GetDownEllipse();

	CPen linePen(PS_SOLID,1,RGB(255,255,0));

	lpDC->SelectObject( linePen );
	lpDC->MoveTo( upEllipse.CenterPoint() );
	lpDC->LineTo( downEllipse.CenterPoint() );

	CBrush eclipBrush(RGB(255, 255, 0));
	lpDC->SelectObject( eclipBrush );

	lpDC->Ellipse( upEllipse );//黄色增益线上端的球
	lpDC->Ellipse( downEllipse);//黄色增益线下端的球
}

CRect RadarCurveWnd::GetUpEllipse()
{
	int radius = RADIUS;
	CRect upEllipse;
	{
		int center = _startPos / 100.0f * ( _curveMaxX - _curveMinX ) + _sectionMaxX/*_curveMinX*/;
		if ( center > _curveMaxX )
		{
			center = _curveMaxX;
		}
		upEllipse.left = center;
		upEllipse.right = upEllipse.left + radius;
		upEllipse.top = STAFF_HEIGHT;
		upEllipse.bottom = upEllipse.top + radius;
	}
	return upEllipse;
}

CRect RadarCurveWnd::GetDownEllipse()
{
	int radius = RADIUS;
	CRect downEllipse;
	{
		int center = _scaleRatio / 100.0f * ( _curveMaxX - _curveMinX ) + _sectionMaxX/*_curveMinX*/;
		if ( center > _curveMaxX )
		{
			center = _curveMaxX;
		}
		downEllipse.left = center;
		downEllipse.right = downEllipse.left + radius;

		downEllipse.top = _curveMaxY -  radius;
		downEllipse.bottom = _curveMaxY;
	}
	return downEllipse;
}

int RadarCurveWnd::mapRadarValueToXValue( float radarValue, float temp)
{
	//radarValue = _startPos + radarValue;

	//float ratio = radarValue / SHRT_MAX;
	//ratio += 1;
	//ratio /= 2;
	//if ( ratio > 1 )
	//{
	//	ratio = 1;
	//}
	//if ( ratio < 0 )
	//{
	//	ratio = 0;
	//}
	//ratio = _scaleRatio * ratio;

	//float result = ( ratio * ( _curveMaxX - _curveMinX ) + _curveMinX );
	//if ( result > _curveMaxX )
	//{
	//	result = _curveMaxX;
	//}

	//return result;
	////return /*_maxX - */( ratio * ( _curveMaxX - _curveMinX ) + _curveMinX );

	radarValue = _startPos + radarValue;

	float ratio = radarValue / SHRT_MAX;
	
	if(_scaleRatio>_startPos){
		ratio = ratio * (1.0 + (float)(_scaleRatio-_startPos-1) * temp);
	}else{
		ratio = ratio * (1.0 + (float)(_startPos-_scaleRatio-1) * (1-temp));
	}

	ratio += 1;
	ratio /= 2;
	if ( ratio > 1 ){
		ratio = 1;
	}
	if ( ratio < 0 ){
		ratio = 0;
	}

	//ratio = _scaleRatio * ratio;

	float result = ( ratio * ( _curveMaxX - _curveMinX ) + _curveMinX );
	if ( result > _curveMaxX )
	{
		result = _curveMaxX;
	}

	return result;
}

int RadarCurveWnd::mapTimeToYValue( float t )
{
	float st = ( _maxTime - t ) /*/ 1000.0f*/;

	float y = st * POINT_SPACE;
	if ( y > _curveMaxY )
	{
		return -1;
	}
	return _curveMaxY - y;
}

int RadarCurveWnd::mapIndexToCurveYValue( int index, int dataCount )
{
	//float st = index * DATA_BLOCK_HEIGHT / dataCount;
	float st = index * 1.0f * ( _curveMaxY - _curveMinY ) / dataCount + _curveMinY;
	float y = st ;
	if ( y > _curveMaxY )
	{
		return -1;
	}
	return int(y);
}

int RadarCurveWnd::mapIndexToSectionYValue( int index, int dataCount )
{
	float st = index * 1.0f * ( _sectionMaxY - _sectionMinY ) / dataCount/* + _sectionMinY*/;//波谱图是先存进另一份画布的，所以不用+_sectionMinY
	return st;
}

void RadarCurveWnd::StartDraw()
{
	_drawing = true;
	this->SetTimer(1, 35, NULL );
}

void RadarCurveWnd::EndDraw()
{
	_drawing = false;
	this->KillTimer(1 );
}

void RadarCurveWnd::DrawCurve( CDC *lpDC )//绘制波形图
{
	if ( !_drawing )
	{
		return;
	}

	if ( _tmpDataVector.size() == 0 )
	{
		return;
	}

	//_maxTime = _tmpDataVector.at(_tmpDataVector.size() - 1)->getTime();
	//_minTime = _tmpDataVector.at(0)->getTime();

	CPen pen_grid_vert(PS_SOLID,1,RGB(200,255,105));//线用黄色
	lpDC->SelectObject(pen_grid_vert);

	RadarData *rd = NULL;
	RadarDataVector::reverse_iterator it;

	bool moveTo = false;
	bool curveToBreak = false;

	ConfigureSet *cs = RadarManager::Instance()->getConfigureSet();
	std::string strPrec = cs->get("radar", "prec");

	int iprec = atoi( strPrec.c_str() );

	int iDataCount;
	for (it = _tmpDataVector.rbegin(); it != _tmpDataVector.rend(); it++)
	{
		rd = ((*it).get());
		if ( !rd ){
			continue;
		}
		iDataCount = rd->getDataCount();
		for ( int i = 0; i < iDataCount; i ++ ){
			float t;
			//short v = rd->getIndexData( i, &t ) ;
			short v = rd->getIndexData( i + _dCorrection, &t );{
			// curve line
				int y = mapIndexToCurveYValue( i, iDataCount );
				//int x = mapRadarValueToXValue( v );
				int x = mapRadarValueToXValue( v,(float)(i+1)/(float)iDataCount );

				if ( y > _curveMinY && y <= _curveMaxY){
					if ( !moveTo ){
						lpDC->MoveTo(x, y);
						moveTo = true;
					}else{
						lpDC->LineTo( x, y );
					}
				}
				if ( y > _curveMaxY ){
					break;
				}
			}
		}
		break;
	}
}

void RadarCurveWnd::DrawCurveNew( CDC *lpDC )//绘制波形图
{
	if ( !_drawing ){
		return;
	}
	if ( !_dataAvailabled ){
		return;
	}
	/*if ( _currentRadarData == NULL ){
		return;
	}*/

	CPen pen_grid_vert(PS_SOLID,1,RGB(200,255,105));//线用黄色
	lpDC->SelectObject(pen_grid_vert);

	RadarData *rd = NULL;

	bool moveTo = false;
	bool curveToBreak = false;

	rd = _currentRadarData;
	if ( !rd ){
		return;
	}
	int iDataCount = _currentRadarData->getDataCount();
	for ( int i = 0; i < iDataCount; i ++ ){
		float t;
		short value = _currentRadarData->getIndexData( i + _dCorrection, &t );//取雷达数据第i个点的值
		// curve line
		{
			int y = mapIndexToCurveYValue( i, iDataCount );
			//int x = mapRadarValueToXValue( value );
			int x = mapRadarValueToXValue( value,(float)(i+1)/(float)iDataCount );

			if ( y > _curveMinY && y <= _curveMaxY){
				if ( !moveTo ){
					lpDC->MoveTo(x, y);
					moveTo = true;
				}else{
					lpDC->LineTo( x, y );
				}
			}
			if ( y > _curveMaxY ){
				break;
			}
		}
	}
}


void RadarCurveWnd::DrawSection( CDC *lpDC, int newData )//绘制波普数据
{
	if ( !_drawing )
	{
		return;
	}

	if (_tmpDataVector.size() == 0)
	{
		return;
	}
	//上方刻度
	CRect staffRect;
	staffRect.left = _sectionMinX;
	staffRect.right = _sectionMaxX;
	staffRect.top = _staffMinY;
	staffRect.bottom = _staffMaxY;
	//波普区域
	CRect sectionRect;
	sectionRect.left = _sectionMinX;
	sectionRect.right = _sectionMaxX;
	sectionRect.top = _sectionMinY;
	sectionRect.bottom = _sectionMaxY;

	lpDC->FillRect( staffRect, &_staffBrush );//刻度白底

	CBrush blackBruh( RGB( 0, 0, 0 ) );
	if (!_bmpDCSource.isInit() || _needReinit)//_bmpDCSource是自己定义的CBmpDC类的对象，用于把dc存到bmp里的
	{
		_bmpDCSource.init( _lpDC, sectionRect );
		//_bmpDCSource.drawRect(&blackBruh);
	}
	if ( !_bmpDCTmp.isInit() || _needReinit)
	{
		_bmpDCTmp.init(_lpDC, sectionRect);
		//_bmpDCTmp.drawRect(&blackBruh);
	}

	//BitBlt一个计算机函数 该函数对指定的源设备环境区域中的像素进行位块转换，以传送到目标设备环境
	_bmpDCTmp.getDC()->BitBlt(0, 0, sectionRect.Width() - newData, sectionRect.Height(), _bmpDCSource.getDC(), newData, 0, SRCCOPY );


	CPen *lpOldPen = lpDC->SelectObject( &_staffPen );
	//距离刻度横线
	int staffBaseLineY = _staffMaxY - 5;
	lpDC->MoveTo(_sectionMinX, staffBaseLineY);
	lpDC->LineTo( _sectionMaxX, staffBaseLineY );

	CFont *lpOldFont = lpDC->SelectObject( &_textFont );

	RadarData *rd = NULL;
	RadarDataVector::reverse_iterator it;//反向迭代器 rend()是第一个元素 rbegin是最后一个元素的下一个位置 指向位置下一位的值 [0,1,2,3]的rpos[1]=2

	bool moveTo = false;
	bool curveToBreak = false;
	bool sectionToBreak = false;
	int row = 0;

	int dataNum = newData;//newdata为传进来的参数
	if ( _needReinit )
	{
		dataNum = _tmpDataVector.size();
	}
	for (it = _tmpDataVector.rbegin(); it != _tmpDataVector.rend(); it++, row++)
	{
		rd = ((*it).get());
		if ( !rd )
		{
			continue;
		}
		int x = ( _sectionMaxX - _sectionMinX )- row;//横向区域范围，x用以指代当前循环处理的区域
		//int x = ( _sectionMaxX - _sectionMinX )- row + _sectionMinX;//旧的
		//仅绘制当前屏幕能放下的最新的数据
		if ( x < 0 )
		{
			sectionToBreak = true;
		}

		if ( x % 50 == 0 )//隔50像素画一个
		{
			short r = rd->getPrec();
			lpDC->MoveTo( x, staffBaseLineY );
			lpDC->LineTo( x, _staffMinY  + STAFF_HEIGHT / 4.0f * 3);
			CString str;
			str.Format( L"%0.2f", rd->getPrecLen() );//获取要绘制的距离 "%0.2f"保留两位小数
			CRect rtext( x - 25, _staffMinY + 10, x + 25,_staffMinY  + STAFF_HEIGHT / 4.0f * 3 );//（left,below,right,top）
			lpDC->DrawText( str, rtext, DT_CENTER | DT_BOTTOM);//绘制距离
		}

		if ( dataNum >= 0 )
		{
			int sectionX = x - _sectionMinX;
			if ( rd->getMark() )
			{
				int iDataCount = rd->getDataCount();
				CPen *lpOldPen = _bmpDCTmp.getDC()->SelectObject( &_markPen );
				//_bmpDCTmp.getDC()->MoveTo( sectionX, _sectionMaxY );//旧的
				//_bmpDCTmp.getDC()->LineTo( sectionX, _sectionMinY );//旧的
				_bmpDCTmp.getDC()->MoveTo( sectionX, 10 );
				_bmpDCTmp.getDC()->LineTo( sectionX, iDataCount * 2 - 20 );
				_bmpDCTmp.getDC()->SelectObject( lpOldPen );
			}else
			{
 				int iDataCount = rd->getDataCount();
				int iSectionHight = sectionRect.Height();
				if (iDataCount < iSectionHight && iDataCount > 0)//需要补充
				{
					float fBeishu = ((float)iSectionHight) / ((float)iDataCount);//每个数据能占的高度

					int iInsertCount = iSectionHight - iDataCount; //需要补全来增加的数量
					int iInsertPost = iDataCount /  iInsertCount; //用以确定插值的位置

					float vOld = 0.;//上一数据的值
					float vNew = 0.;//当前数据的值
					int y = 0;

					if (fBeishu >= 2.0)//最多翻一倍
					{
						iInsertPost = 1;
					}

					for ( int i = 0; i < iDataCount; i ++ )//横坐标x的这条线上的不同y的值
					{
						vNew = (float)rd->getIndexData( i + _dCorrection, NULL ) * (1.0 + (float)_scaleRatio * (float)(i+1)/(float)iDataCount);
						if ( !sectionToBreak )
						{
							COLORREF c ,cHalf;
							if ( rd->getMark() )
							{
								c = RGB( 255, 255, 255 );
								cHalf = RGB( 255, 255, 255 );
							}else
							{
								c = getColor(vNew);//根据当前值定颜色
								cHalf = getColor((vOld + vNew) / 2.0);//前后两个之间的颜色
							}

							if ( x > 0 && x <= _sectionMaxX )
							{
								if (i > 0 && (i%iInsertPost == 0))
								{
									_bmpDCTmp.getDC()->SetPixelV( ( sectionX ), y, cHalf );
									++y;
								}
								_bmpDCTmp.getDC()->SetPixelV( ( sectionX ), y, c );
								++y;
							}
						}
						vOld = vNew;
					}
				}else//数据数量大于高度,需要压缩，即抽值显示，mapIndexToSectionYValue利用比例关系 i/i总数量 = y/y总数量 来求得i对应的
				{
					for ( int i = 0; i < iDataCount; i ++ )
					{
						//short v = (float)rd->getIndexData( i, NULL ) * (1.0 + (float)_scaleRatio * (float)(i+1)/(float)iDataCount);//旧版用short会出现显示错误
						float v = (float)rd->getIndexData( i + _dCorrection, NULL ) * (1.0 + (float)_scaleRatio * (float)(i+1)/(float)iDataCount);
						if ( !sectionToBreak )
						{
							int y = mapIndexToSectionYValue( i, rd->getDataCount() );
							//int y = mapIndexToSectionYValue( i, rd->getDataCount() ) + _sectionMinY;//旧的
							COLORREF c;
							if ( rd->getMark() )
							{
								c = RGB( 255, 255, 255 );
							}else
							{
								c = getColor(v);
							}

							if ( x > 0 && x <= _sectionMaxX )
							{
								_bmpDCTmp.getDC()->SetPixelV( ( sectionX ), y, c );
							}
						}
					}
				}
			}
			dataNum --;
		}


		if ( sectionToBreak )
		{
			break;
		}
	}

	_bmpDCSource.getDC()->BitBlt(0, 0, sectionRect.Width(), sectionRect.Height(), _bmpDCTmp.getDC(), 0, 0, SRCCOPY );

	lpDC->BitBlt( staffRect.left, sectionRect.top, sectionRect.Width(), sectionRect.Height(), _bmpDCSource.getDC(), 0, 0, SRCCOPY );


	lpDC->SelectObject( lpOldPen );
	lpDC->SelectObject( lpOldFont );

	_needReinit = false;
	
}

COLORREF RadarCurveWnd::getColor( float radarValue )
{
	float ratio = radarValue / SHRT_MAX;
	ratio += 1;
	ratio /= 2;
	if ( ratio > 1 )
	{
		ratio = 1;
	}
	if ( ratio < 0 )
	{
		ratio = 0;
	}
	COLORREF col = RGB( int(ratio * 255), int(ratio * 255), int(ratio * 255 ) );
	return col;
}

void RadarCurveWnd::addData( RadarData *lpData )
{
	/*
	{
		OpenThreads::ScopedLock<OpenThreads::Mutex> lock( _mutex );
		_radarDataVector.push_back( lpData );
	}

	while( _radarDataVector.size() > MAX_DRAW_COUNT )
	{
		_radarDataVector.erase( _radarDataVector.begin() );
	}
	*/
	
	{
		OpenThreads::ScopedLock<OpenThreads::Mutex> lock( _mutex );
		_radarDataVector.clear();
		_radarDataVector.push_back( lpData );
	}

	_maxTime = lpData->getTime();
	_minTime = _radarDataVector[0]->getTime();

	_newData++;
	if ( !_drawing )
	{
		return;
	}
}

void RadarCurveWnd::setData( RadarData *lpData )
{
	_dataAvailabled=true;
	{
		OpenThreads::ScopedLock<OpenThreads::Mutex> lock( _mutex );
		_currentRadarData = lpData;
	}
	//_currentRadarData = lpData;

	if ( !_drawing ){
		return;
	}
}
/*
void RadarCurveWnd::deleteData( )
{
	{
		OpenThreads::ScopedLock<OpenThreads::Mutex> lock( _mutex );
		_radarDataVector.pop_back( );
	}

	_newData--;
	if ( !_drawing )
	{
		return;
	}
}*/

void RadarCurveWnd::clearData()
{
	_radarDataVector.clear();
}

void RadarCurveWnd::OnLButtonDown(UINT nFlags, CPoint point){
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CRect rectUp = GetUpEllipse();
	CRect rectDown = GetDownEllipse();

	if ( rectUp.PtInRect( point ) ){
		_clickUp = true;
		_clickDown = false;
	}else if ( rectDown.PtInRect( point ) ){
		_clickUp = false;
		_clickDown = true;
	}else{
		_clickDown = false;
		_clickUp = false;
	}

	if ( GetParent() ){
		if ( point.x >= _sectionMinX && point.x <= _sectionMaxX ){
			CPoint screenPoint = point;
			ClientToScreen( &screenPoint );
			GetParent()->SendMessage( WM_CLICK_ON_SECTION, screenPoint.x, screenPoint.y );
		}
	}

	CWnd::OnLButtonDown(nFlags, point);
	/*_lpParent->OnLButtonDown( nFlags, point );*/
}

void RadarCurveWnd::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	_clickUp = false;
	_clickDown = false;
	CWnd::OnLButtonUp(nFlags, point);
}

void RadarCurveWnd::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	int curX = point.x - RADIUS / 2;
	if ( _clickUp )
	{
		_startPos = int ( ( curX - _curveMinX ) * 1.0f / ( _curveMaxX - _curveMinX ) * 100);
		if ( _startPos < 0 )
		{
			_startPos = 0;
		}
		Invalidate();
	}else if ( _clickDown )
	{
		_scaleRatio = int ( ( curX - _curveMinX ) * 1.0f / ( _curveMaxX - _curveMinX ) * 100);
		if ( _scaleRatio < 1 )
		{
			_scaleRatio = 1;
		}
		Invalidate();
	}
	CWnd::OnMouseMove(nFlags, point);
}

BOOL RadarCurveWnd::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	return TRUE;
	return CWnd::OnEraseBkgnd(pDC);
}

void RadarCurveWnd::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if ( RadarManager::Instance()->getCurProject() ){
 		if ( nChar == 32 ){
			RadarManager::Instance()->getCurProject()->setMark();
		}else if( nChar == '1' ){
			RadarManager::Instance()->DataMarkOne();
		}else if( nChar == '2' ){
			RadarManager::Instance()->DataMarkTwo();
		}else if( nChar == '3' ){
			RadarManager::Instance()->DataMarkThree();
		}else if( nChar == '4' ){
			RadarManager::Instance()->DataMarkCustomize();
		}else if( nChar == 87 ){
			RadarManager::Instance()->stopWork();
		}
	}
	//CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
}

void RadarCurveWnd::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码

	_needReinit = true;
}

void RadarCurveWnd::setCorrection( int value )
{
	_dCorrection = value;
}
