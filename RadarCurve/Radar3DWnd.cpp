// Radar3DWnd.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "RadarCurve.h"
#include "Radar3DWnd.h"
#include <stdlib.h>
#include <float.h>
#include <OpenThreads/ScopedLock>
#include <RadarCurve/ConfigureSet.h>
#include <RadarCurve/RadarManager.h>

// Radar3DWnd

IMPLEMENT_DYNAMIC(Radar3DWnd, CWnd)

#define SPACE_SIZE 10 //�����ļ����С
#define INTERPOLATION_SIZE 56 //��ֵ�����С
#define SCALE_HEIGHT 30 //����̶�����߶�

#define STAFF_HEIGHT 30 //����̶�����߶�
#define GRID_SPACE 15
#define POINT_SPACE 10
#define DATA_BLOCK_HEIGHT 40.0f
#define DATA_BLOCK_WIDTH 20.0f

#define TIME_STAFF_WIDTH 20 //ʱ���̶�������

#define CUR_STAFF_INTER_PERCENT 0.955

#define CUR_MARK_INTER 0.05

#define RADIUS 10

#define MAX_DRAW_COUNT 2000


Radar3DWnd::Radar3DWnd()
{
	RegisterWindowClass();

	m_dFrontViewSectionMinX = 0;
	m_dFrontViewSectionMaxX = 0;
	m_dFrontViewSectionMinY = 0;
	m_dFrontViewSectionMaxY = 0;
 
	m_dVerticalSectionMinX = 0;
	m_dVerticalSectionMaxX = 0;
	m_dVerticalSectionMinY = 0;
	m_dVerticalSectionMaxY = 0;

	m_dHorizontalSectionMinX = 0;
	m_dHorizontalSectionMaxX = 0;
	m_dHorizontalSectionMinY = 0;
	m_dHorizontalSectionMaxY = 0;

	_drawing = false;

	_startPos = 0;
	_scaleRatio = 1;

	_clickUp = false;
	_clickDown = false;

	m_scaleBrush.CreateSolidBrush( RGB ( 255, 255, 255 ) );
	m_scalePen.CreatePen( PS_SOLID, 1, RGB ( 0, 0, 0 ) );

	_staffBrush.CreateSolidBrush( RGB ( 255, 255, 255 ) );
	_staffPen.CreatePen( PS_SOLID, 1, RGB ( 0, 0, 0 ) );
	_markPen.CreatePen( PS_SOLID, 1, RGB ( 255, 255, 255 ) );
	_textFont.CreateFont( 12, 0, 0, 0, 0, FALSE, FALSE, FALSE , 0, 0, 0, 0, 0, L"����");

	_sampleCount = 0;
	_sampleRatio = 51.2;

	m_dChannelIndex=5;
	m_dDepthIndex=55;

	_lpDC = NULL;
	//_currentRadarData = NULL;

	_newData = 0;
	_needReinit = false;
	_dataAvailabled = false;

	for(int i=0;i<12;i++){
		m_dVecCorrection[i]=0;
	}
}

Radar3DWnd::~Radar3DWnd()
{
}

BOOL Radar3DWnd::RegisterWindowClass()
{
	WNDCLASS wndcls;
	//HINSTANCE hInst = AfxGetInstanceHandle();
	HINSTANCE hInst = AfxGetResourceHandle();

	if (!(::GetClassInfo(hInst, Radar3D_CLASSNAME, &wndcls)))
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
		wndcls.lpszClassName    = Radar3D_CLASSNAME;

		if (!AfxRegisterClass(&wndcls))
		{
			AfxThrowResourceException();
			return FALSE;
		}
	}

	return TRUE;
}




BEGIN_MESSAGE_MAP(Radar3DWnd, CWnd)
	ON_WM_TIMER()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_ERASEBKGND()
	ON_WM_KEYDOWN()
	ON_WM_SIZE()
END_MESSAGE_MAP()


BOOL Radar3DWnd::Create(const RECT& rect, CWnd* parent, UINT nID, LPCTSTR className, DWORD dwStyle )
{
    ASSERT(parent->GetSafeHwnd());

	if (!CWnd::Create(className, NULL, dwStyle, rect, parent, nID))
		return FALSE;

	return TRUE;
}



// Radar3DWnd ��Ϣ�������

void Radar3DWnd::SetSampleCount( int count )
{
	_sampleCount = count;
}
void Radar3DWnd::SetSampleRatio( double value )
{
	_sampleRatio = value;
}
void Radar3DWnd::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	CWnd::OnTimer(nIDEvent);

	Invalidate();
}

void Radar3DWnd::OnPaint()
{
	//CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CWnd::OnPaint()
	
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
	resetXY();
	DrawBk(&dcMem);

	int newData = 0;
	{
		OpenThreads::ScopedLock<OpenThreads::Mutex> lock(_mutex);
		_tmpDataVector = _radarDataVector;
		m_vecRadarGroupData=m_vecRadarGroupDataLatest;
		newData = _newData;
		_newData = 0;
	}

	if ( _drawing )
	{
		//DrawCurveNew(&dcMem);//����ͼ
		//DrawCurve(&dcMem);//����ͼ
		DrawSection( &dcMem, newData, m_dChannelIndex, m_dDepthIndex );//��������ͼ
		drawFrontView( &dcMem, m_dChannelIndex, m_dDepthIndex );
	}
	//DrawScaleFlag( &dcMem );//20210803hjl
	pDC->BitBlt(0,0,rc.Width(),rc.Height(),&dcMem,0,0,SRCCOPY);//20210803hjl
	dcMem.DeleteDC();   
	bmp.DeleteObject(); 
	EndPaint( &pat );
}

void Radar3DWnd::resetXY()//���Ʊ��� 
{
	CRect rc;
	GetClientRect(&rc);
	
	//����ͼ����
	m_dFrontViewSectionMinX = rc.Width() - INTERPOLATION_SIZE - SPACE_SIZE;
	m_dFrontViewSectionMaxX = rc.Width() - SPACE_SIZE;
	m_dFrontViewSectionMinY = SPACE_SIZE + SCALE_HEIGHT + SPACE_SIZE + INTERPOLATION_SIZE + SPACE_SIZE;
	m_dFrontViewSectionMaxY = rc.Height() - SPACE_SIZE;

	//����������
	m_dVerticalSectionMinX = SPACE_SIZE;
	m_dVerticalSectionMaxX = rc.Width() - SPACE_SIZE - INTERPOLATION_SIZE - SPACE_SIZE;
	m_dVerticalSectionMinY = SPACE_SIZE + SCALE_HEIGHT + SPACE_SIZE + INTERPOLATION_SIZE + SPACE_SIZE;
	m_dVerticalSectionMaxY = rc.Height() - SPACE_SIZE;

	//����������
	m_dHorizontalSectionMinX = SPACE_SIZE;
	m_dHorizontalSectionMaxX = rc.Width() - SPACE_SIZE - INTERPOLATION_SIZE - SPACE_SIZE;
	m_dHorizontalSectionMinY = SPACE_SIZE + SCALE_HEIGHT + SPACE_SIZE;
	m_dHorizontalSectionMaxY = SPACE_SIZE + SCALE_HEIGHT + SPACE_SIZE + INTERPOLATION_SIZE;

	//����̶�����
	m_dScaleMinX = SPACE_SIZE;
	m_dScaleMaxX = rc.Width() - SPACE_SIZE - INTERPOLATION_SIZE - SPACE_SIZE;
	m_dScaleMinY = SPACE_SIZE;
	m_dScaleMaxY = SPACE_SIZE + SCALE_HEIGHT;
}

void Radar3DWnd::DrawBk( CDC *lpDC )//���Ʊ��� 
{

	CRect rc;
	GetClientRect(&rc);

	//int width = GRID_WIDTH;//����ͼ���
	////int height = rc.Height();
	//int curvRectX1 = rc.Width() - GRID_WIDTH - TIME_STAFF_WIDTH;//����ͼX1  TIME_STAFF_WIDTHʱ���̶�������
	//int curvRectX2 = curvRectX1 + GRID_WIDTH;//����ͼX2
	//int curvRectY1 = STAFF_HEIGHT;//STAFF_HEIGHT����̶�����߶�
	//int curvRectY2 = rc.Height();

	CBrush blackBrush( RGB( 0, 0, 0 ) );//���ױ�����ɫ
	CBrush greyBrush( RGB( 128, 128, 128 ) );//���ױ�����ɫ
	CBrush whiteBrush( RGB( 255, 255, 255 ) );//���ױ�����ɫ

	CRect r1(m_dFrontViewSectionMinX, m_dFrontViewSectionMinY, m_dFrontViewSectionMaxX, m_dFrontViewSectionMaxY);
	lpDC->FillRect( r1, &greyBrush );
	CRect r2(m_dVerticalSectionMinX, m_dVerticalSectionMinY, m_dVerticalSectionMaxX, m_dVerticalSectionMaxY);
	lpDC->FillRect( r2, &greyBrush );
	CRect r3(m_dHorizontalSectionMinX, m_dHorizontalSectionMinY, m_dHorizontalSectionMaxX, m_dHorizontalSectionMaxY);
	lpDC->FillRect( r3, &greyBrush );
	CRect r4(m_dScaleMinX, m_dScaleMinY, m_dScaleMaxX, m_dScaleMaxY);
	lpDC->FillRect( r4, &greyBrush );


	///*-----------------------��ʼ�̻�����ͼ����-----------------------*/
	//CBrush brushCurvRect(RGB(75,75,75));//����ͼ����ǳ��ɫ
	//CRect rectCurvRect(curvRectX1,curvRectY1 ,curvRectX2,curvRectY2);
	//lpDC->FillRect(rectCurvRect,&brushCurvRect);
	////���������
	//int xLineCount = ( curvRectX2 - curvRectX1 ) / GRID_SPACE;
	//int yLineCount = ( curvRectY2 - curvRectY1 ) / GRID_SPACE ;
	//int curvRectInterP5 = xLineCount / 10.0 *3.0 ;

	////bg-grid-vertical ���������е�������
	//CPen pen_grid_vert(PS_DOT,1,RGB(105,255,105));//��ɫ����
	//lpDC->SelectObject(pen_grid_vert);
	//lpDC->SetBkMode(TRANSPARENT);
	//for(int i=1;i<xLineCount;++i){
	//	lpDC->MoveTo(curvRectX1 + i * GRID_SPACE, curvRectY1);
	//	lpDC->LineTo(curvRectX1 + i * GRID_SPACE, curvRectY2);
	//}

	////bg-grid-mid ���������еĺ�����
	//CPen pen_grid_mid(PS_DOT,1,RGB(255,105,105));//��ɫ����
	//lpDC->SelectObject(pen_grid_mid);
	//lpDC->MoveTo(curvRectX1+GRID_SPACE/2.0*xLineCount,curvRectY1);
	//lpDC->LineTo(curvRectX1+GRID_SPACE/2.0*xLineCount,curvRectY2);

	////bg-grid-horizon ���������е��̺���
	//CPen pen_grid_hori(PS_DOT,1,RGB(105,255,105));//��ɫ����
	//lpDC->SelectObject(pen_grid_hori);
	//lpDC->SetBkMode(TRANSPARENT);
	//for(int i=1;i<=yLineCount;++i)
	//{
	//	lpDC->MoveTo(curvRectX1,curvRectY1+i * GRID_SPACE);
	//	lpDC->LineTo(curvRectX2,curvRectY1+i * GRID_SPACE);
	//}

	////���ſ̶� ʱ��
	//int staffWidth = TIME_STAFF_WIDTH;//����ʱ���̶ȿ��
	////�̶ȴ��İ׵�
	////lpDC->FillSolidRect( curvRectX2, curvRectY1, TIME_STAFF_WIDTH, curvRectY2 - curvRectY1, RGB( 255, 255, 255 ) );//(����x��y,��ȣ��߶ȣ���ɫ)
	//lpDC->FillSolidRect( curvRectX2, curvRectY1, TIME_STAFF_WIDTH, curvRectY2 - curvRectY1, RGB( 240, 240, 240 ) );//(����x��y,��ȣ��߶ȣ���ɫ)
	////����
	//int curStaffCxLen = staffWidth / 3;
	//CString strMark;
	//CPen pen_staff_vline(PS_SOLID,1,RGB(0,0,0));
	//lpDC->SelectObject(pen_staff_vline);
	//int staffStartX = curvRectX2 + 2;
	////��ֱ����
	//lpDC->MoveTo(staffStartX,curvRectY1);
	//lpDC->LineTo(staffStartX,curvRectY2);
	////�����
	//int count = int( 1 / _sampleRatio * _sampleCount + 0.5 );//��������
	//float staffYSpace = (curvRectY2 - curvRectY1) * 1.0f / count;//���
	//CFont *lpOldFont = lpDC->SelectObject( &_textFont );
	//count += 1;
	////����̶ȳ��е��߼�����
	//for(int i = 0;i <= count;i ++ ){
	//	if ( i % 10 == 0 ){//����̶ȳ��е�ˮƽ���߼�����
	//		if ( i == count ){
	//			lpDC->MoveTo(staffStartX, curvRectY1 + i*staffYSpace - 1);
	//			lpDC->LineTo(staffStartX+curStaffCxLen, curvRectY1 + i*staffYSpace - 1);
	//		}else{
	//			lpDC->MoveTo(staffStartX, curvRectY1 + i*staffYSpace);
	//			lpDC->LineTo(staffStartX + curStaffCxLen, curvRectY1 + i*staffYSpace);
	//		}
	//		strMark.Format(L"%d", i);	
	//		if ( i == 0 ){
	//			lpDC->TextOut(staffStartX + curStaffCxLen + 1, curvRectY1 + i * staffYSpace, strMark);
	//		}else if ( i / 2 == count / 2 ){
	//			lpDC->TextOut(staffStartX + curStaffCxLen + 1, curvRectY1 + i * staffYSpace - 12, strMark);
	//		}else{
	//			lpDC->TextOut(staffStartX + curStaffCxLen + 1, curvRectY1 + i * staffYSpace - 6, strMark);
	//		}
	//	}else{//����̶ȳ��е�ˮƽ����
	//		if ( i == count ){
	//			lpDC->MoveTo(staffStartX, curvRectY1 + i*staffYSpace - 1);
	//			lpDC->LineTo(staffStartX+curStaffCxLen / 2, curvRectY1 + i*staffYSpace - 1);
	//		}else{
	//			lpDC->MoveTo(staffStartX, curvRectY1 + i*staffYSpace);
	//			lpDC->LineTo(staffStartX+curStaffCxLen / 2, curvRectY1 + i*staffYSpace);
	//		}
	//	}
	//}
	///*-----------------------��������ͼ�����̻�-----------------------*/

	//lpDC->SelectObject( lpOldFont );//��
	

	////����ͼ����XY
	//_curveMinX = curvRectX1;
	//_curveMaxX = curvRectX2;
	//_curveMinY = curvRectY1;
	//_curveMaxY = curvRectY2;
	////����ͼ����XY
	//_sectionMinX = 0;
	//_sectionMaxX = curvRectX1;
	//_sectionMinY = curvRectY1;
	//_sectionMaxY = curvRectY2;
	////����̶�����Y(X�벨��ͼһ��)
	//_staffMinY = 0;
	//_staffMaxY = curvRectY1;
}

void Radar3DWnd::DrawScaleFlag( CDC *lpDC )
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

	lpDC->Ellipse( upEllipse );//��ɫ�������϶˵���
	lpDC->Ellipse( downEllipse);//��ɫ�������¶˵���
}

CRect Radar3DWnd::GetUpEllipse()
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

CRect Radar3DWnd::GetDownEllipse()
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

int Radar3DWnd::mapRadarValueToXValue( float radarValue, float temp)
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

int Radar3DWnd::mapTimeToYValue( float t )
{
	float st = ( _maxTime - t ) /*/ 1000.0f*/;

	float y = st * POINT_SPACE;
	if ( y > _curveMaxY )
	{
		return -1;
	}
	return _curveMaxY - y;
}

int Radar3DWnd::mapIndexToCurveYValue( int index, int dataCount )
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

int Radar3DWnd::mapIndexToSectionYValue( int index, int dataCount )
{
	float st = index * 1.0f * ( _sectionMaxY - _sectionMinY ) / dataCount/* + _sectionMinY*/;//����ͼ���ȴ����һ�ݻ����ģ����Բ���+_sectionMinY
	return st;
}

void Radar3DWnd::StartDraw()
{
	_drawing = true;
	this->SetTimer(1, 35, NULL );
}

void Radar3DWnd::EndDraw()
{
	_drawing = false;
	this->KillTimer(1 );
}
/*
void Radar3DWnd::DrawCurve( CDC *lpDC )//���Ʋ���ͼ
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

	CPen pen_grid_vert(PS_SOLID,1,RGB(200,255,105));//���û�ɫ
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
*/

std::vector<float> interpolation( float min, float max, int count ){//count�м�弸��ֵ
	std::vector<float> result;
	float delta=max-min;
	for(int i=1;i<count+1;i++){
		result.push_back((float)min+delta/(count+1)*i);
	}
	return result;
}

void Radar3DWnd::DrawSection( CDC *lpDC, int newData, int channelIndex, int depthIndex )//���Ʋ�������
{
	if ( !_drawing ){
		return;
	}
	if (m_vecRadarGroupData.size() == 0){
		return;
	}

	//�Ϸ��̶�
	CRect scaleRect;
	scaleRect.left = m_dScaleMinX;
	scaleRect.right = m_dScaleMaxX;
	scaleRect.top = m_dScaleMinY;
	scaleRect.bottom = m_dScaleMaxY;

	lpDC->FillRect( scaleRect, &_staffBrush );//�̶Ȱ׵�

	//����������
	CRect verticalSectionRect;
	verticalSectionRect.left = m_dVerticalSectionMinX;
	verticalSectionRect.right = m_dVerticalSectionMaxX;
	verticalSectionRect.top = m_dVerticalSectionMinY;
	verticalSectionRect.bottom = m_dVerticalSectionMaxY;

	//������������
	CRect sectionRect;
	sectionRect.left = m_dVerticalSectionMinX;
	sectionRect.right = m_dVerticalSectionMaxX;
	sectionRect.top = m_dHorizontalSectionMinY;
	sectionRect.bottom = m_dVerticalSectionMaxY;


	CBrush blackBruh( RGB( 0, 0, 0 ) );
	CBrush greyBrush(RGB(240,240,240));
	if (!_bmpDCSource.isInit() || _needReinit){//_bmpDCSource���Լ������CBmpDC��Ķ������ڰ�dc�浽bmp���
		_bmpDCSource.init( _lpDC, sectionRect );
		//_bmpDCSource.drawRect(&blackBruh);
	}
	if ( !_bmpDCTmp.isInit() || _needReinit){
		_bmpDCTmp.init(_lpDC, sectionRect);
		//_bmpDCTmp.drawRect(&greyBrush);
	}

	//BitBltһ����������� �ú�����ָ����Դ�豸���������е����ؽ���λ��ת�����Դ��͵�Ŀ���豸����
	_bmpDCTmp.getDC()->BitBlt(0, 0, sectionRect.Width() - newData, sectionRect.Height(), _bmpDCSource.getDC(), newData, 0, SRCCOPY );

	//����̶Ⱥ���
	CPen *lpOldPen = lpDC->SelectObject( &_staffPen );
	CFont *lpOldFont = lpDC->SelectObject( &_textFont );
	int scaleBaseLineY = m_dScaleMaxY - 5;
	lpDC->MoveTo( m_dScaleMinX, scaleBaseLineY );
	lpDC->LineTo( m_dScaleMaxX, scaleBaseLineY );
	
	RadarData *rd = NULL;
	//RadarDataVector::reverse_iterator it;//��������� rend()�ǵ�һ��Ԫ�� rbegin�����һ��Ԫ�ص���һ��λ�� ָ��λ����һλ��ֵ [0,1,2,3]��rpos[1]=2
	std::vector<std::vector<osg::ref_ptr<RadarData>>>::reverse_iterator it;


	//bool moveTo = false;
	//bool curveToBreak = false;
	bool verticalSectionToBreak = false;
	bool horizontalSectionToBreak = false;
	int row = 0;//�ڼ���

	int dataNum = newData;//newdataΪ�������Ĳ���
	if ( _needReinit ){
		//dataNum = _tmpDataVector.size();
		dataNum = m_vecRadarGroupData.size();
	}
	for (it = m_vecRadarGroupData.rbegin(); it != m_vecRadarGroupData.rend(); it++, row++){
		int x = ( m_dHorizontalSectionMaxX - m_dHorizontalSectionMinX )- row;//��������Χ��x����ָ����ǰѭ�����������

		//����̶�
		if ( x % 50 == 0 ){//��50���ػ�һ��
			short r = rd->getPrec();
			lpDC->MoveTo( x, scaleBaseLineY );
			lpDC->LineTo( x, m_dScaleMinY  + SCALE_HEIGHT / 4.0f * 3 );
			CString str;
			str.Format( L"%0.2f", rd->getPrecLen() );//��ȡҪ���Ƶľ��� "%0.2f"������λС��
			CRect rtext( x - 25, m_dScaleMinY + 10, x + 25, m_dScaleMinY + SCALE_HEIGHT / 4.0f * 3 );//��left,button,right,top��
			lpDC->DrawText( str, rtext, DT_CENTER | DT_BOTTOM );//���ƾ���
		}

		//������
		if(dataNum>=0){
			int interpolationValueOne=0;//����ֵ�õ�
			for(int i=0;i<12;i++){
				if((*it).size()<12){
					continue;
				}
				rd = (((*it)[i]).get());
				if ( !rd ){
					continue;
				}
				if ( x < 0 ){//�����Ƶ�ǰ��Ļ�ܷ��µ����µ�����
					horizontalSectionToBreak = true;
				}
				if ( dataNum >= 0 ){
					int horizontalSectionX = x;
					float fValue = (float)rd->getIndexData( depthIndex+m_dVecCorrection[i], NULL );
					COLORREF color;
					color = getColor(fValue);
					_bmpDCTmp.getDC()->SetPixelV( horizontalSectionX, INTERPOLATION_SIZE-i*5-1, color );
					if(i>0){
						std::vector<float> valueInterpolation=interpolation( interpolationValueOne, fValue, 4 );//�õ�������С�����
						for(int j=1;j<5;j++){
							color = getColor(valueInterpolation[4-j]);
							_bmpDCTmp.getDC()->SetPixelV( horizontalSectionX, INTERPOLATION_SIZE-i*5+j-1, color );
						}
					}
					interpolationValueOne=fValue;
				}
				if ( horizontalSectionToBreak ){
					break;
				}
			}
		}

		//����������
		if((*it).size()<12){
			continue;
		}
		rd = (((*it)[channelIndex]).get());
		if ( !rd ){
			continue;
		}
		//int x = ( m_dVerticalSectionMaxX - m_dVerticalSectionMinX )- row;//��������Χ��x����ָ����ǰѭ�����������
		//int x = ( m_dVerticalSectionMaxX - m_dVerticalSectionMinX )- row + m_dVerticalSectionMinX;//�ɵ�
		//�����Ƶ�ǰ��Ļ�ܷ��µ����µ�����
		if ( x < 0 ){
			verticalSectionToBreak = true;
		}
		//����
		if ( dataNum >= 0 ){
			int verticalSectionX = x;
 			int dDataCount = rd->getDataCount();
			int dVerticalSectionHight = verticalSectionRect.Height();
			if (dDataCount < dVerticalSectionHight && dDataCount > 0){////��������С�ڸ߶� ��Ҫ����
				/*float fTimes = ((float)dVerticalSectionHight) / ((float)dDataCount);//ÿ��������ռ�ĸ߶�

				int dInsertCount = dVerticalSectionHight - dDataCount; //��Ҫ��ȫ�����ӵ�����
				int dInsertPost = dDataCount /  dInsertCount; //����ȷ����ֵ��λ��

				float fValueOld = 0.;//��һ���ݵ�ֵ
				float fValueNew = 0.;//��ǰ���ݵ�ֵ
				//int y = 0;
				int y = m_dVerticalSectionMinY - m_dHorizontalSectionMinY;//�����������yҪ������ʼֵ
				if (fTimes >= 2.0){//��෭һ��
					dInsertPost = 1;
				}
				for ( int i = 0; i < dDataCount; i ++ ){//������x���������ϵĲ�ͬy��Ӧ��ֵ
					//fValueNew = (float)rd->getIndexData( i+m_dVecCorrection[i], NULL ) * (1.0 +  (float)_scaleRatio * (float)(i+1)/(float)dDataCount);//��Ҫ������
					fValueNew = (float)rd->getIndexData( i+m_dVecCorrection[channelIndex], NULL );//��Ҫ������
					if ( !verticalSectionToBreak ){
						COLORREF color ,colorHalf;
						if ( rd->getMark() ){
							color = RGB( 255, 255, 255 );
							colorHalf = RGB( 255, 255, 255 );
						}else{
							color = getColor(fValueNew);//���ݵ�ǰֵ����ɫ
							colorHalf = getColor((fValueOld + fValueNew) / 2.0);//ǰ������֮�����ɫ
						}

						if ( x > 0 && x <= m_dVerticalSectionMaxX ){
							if (i > 0 && (i%dInsertPost == 0)){
								_bmpDCTmp.getDC()->SetPixelV( ( verticalSectionX ), y, colorHalf );
								++y;
							}
							_bmpDCTmp.getDC()->SetPixelV( ( verticalSectionX ), y, color );
							++y;
						}
					}
					fValueOld = fValueNew;
				}*/
				
				float fValueOld = 0.;//��һ���ݵ�ֵ
				float fValueNew = 0.;//��ǰ���ݵ�ֵ
				int lastY = m_dVerticalSectionMinY - m_dHorizontalSectionMinY;
				for ( int i = 0; i < dDataCount; i ++ ){//������x���������ϵĲ�ͬ��i��Ӧ��y��Ӧ��ֵ
					fValueNew = (float)rd->getIndexData( i+m_dVecCorrection[channelIndex], NULL );
					if ( !verticalSectionToBreak ){
						COLORREF color ,colorHalf;
						if ( rd->getMark() ){
							color = RGB( 255, 255, 255 );
							colorHalf = RGB( 255, 255, 255 );
						}else{
							color = getColor(fValueNew);//���ݵ�ǰֵ����ɫ
							colorHalf = getColor((fValueOld + fValueNew) / 2.0);//ǰ������֮�����ɫ
						}
						if ( x > 0 && x <= m_dVerticalSectionMaxX ){
							//int y = mapIndexToCurveYValue( i, dDataCount )+(m_dVerticalSectionMinY - m_dHorizontalSectionMinY);
							int y = (float)i/(float)dDataCount*(float)dVerticalSectionHight+(m_dVerticalSectionMinY - m_dHorizontalSectionMinY);
							_bmpDCTmp.getDC()->SetPixelV( ( verticalSectionX ), y, color );
							if((y-lastY)>1){
								for(int j=lastY+1;j<y;j++){
									_bmpDCTmp.getDC()->SetPixelV( ( verticalSectionX ), j, colorHalf );
								}
							}
							lastY=y;
						}
					}
					fValueOld = fValueNew;
				}
			}else{//�����������ڸ߶�,��Ҫѹ��������ֵ��ʾ��mapIndexToSectionYValue���ñ�����ϵ i/i������ = y/y������ �����i��Ӧ��
				for ( int i = 0; i < dDataCount; i ++ ){
					//short v = (float)rd->getIndexData( i+m_dVecCorrection[i], NULL ) * (1.0 + (float)_scaleRatio * (float)(i+1)/(float)iDataCount);//�ɰ���short�������ʾ����
					//float value = (float)rd->getIndexData( i+m_dVecCorrection[i], NULL ) * (1.0 +  (float)_scaleRatio * (float)(i+1)/(float)dDataCount);//��Ҫ������
					float value = (float)rd->getIndexData( i+m_dVecCorrection[channelIndex], NULL );//��Ҫ������
					if ( !verticalSectionToBreak ){
						//int y = mapIndexToSectionYValue( i, rd->getDataCount() );//�ɵ�
						int y = mapIndexToSectionYValue( i, rd->getDataCount() ) + (m_dVerticalSectionMinY - m_dHorizontalSectionMinY);//�����������yҪ������ʼֵ
						COLORREF color;
						if ( rd->getMark() ){
							color = RGB( 255, 255, 255 );
						}else{
							color = getColor(value);
						}
						if ( verticalSectionX > 0 && verticalSectionX <= m_dVerticalSectionMaxX ){
							_bmpDCTmp.getDC()->SetPixelV( ( verticalSectionX ), y, color );
						}
					}
				}
			}
			dataNum--;
		}
		if ( verticalSectionToBreak ){
			break;
		}	
	}

	/*
	_bmpDCSource.getDC()->BitBlt(0, 0, verticalSectionRect.Width(), verticalSectionRect.Height(), _bmpDCTmp.getDC(), 0, 0, SRCCOPY );

	lpDC->BitBlt( staffRect.left, sectionRect.top, sectionRect.Width(), sectionRect.Height(), _bmpDCSource.getDC(), 0, 0, SRCCOPY );
	*/
	_bmpDCSource.getDC()->BitBlt(0, 0, sectionRect.Width(), sectionRect.Height(), _bmpDCTmp.getDC(), 0, 0, SRCCOPY );

	lpDC->BitBlt( sectionRect.left, sectionRect.top, sectionRect.Width(), sectionRect.Height(), _bmpDCSource.getDC(), 0, 0, SRCCOPY );

	lpDC->SelectObject( lpOldPen );
	lpDC->SelectObject( lpOldFont );

	/*
	CRect r1(m_dScaleMinX,m_dScaleMaxY,m_dScaleMaxX,m_dHorizontalSectionMinY);
	lpDC->FillRect( r1, &greyBrush );
	*/
	CRect r2(m_dScaleMinX,m_dHorizontalSectionMaxY,m_dScaleMaxX,m_dVerticalSectionMinY);
	lpDC->FillRect( r2, &greyBrush );
	

	_needReinit = false;
}

void Radar3DWnd::drawFrontView( CDC *lpDC, int channelIndex, int depthIndex ){
	if ( !_drawing ){
		return;
	}
	if (m_vecRadarGroupData.size() == 0){
		return;
	}

	//ǰ������
	CRect frontViewSectionRect;
	frontViewSectionRect.left = m_dFrontViewSectionMinX;
	frontViewSectionRect.right = m_dFrontViewSectionMaxX;
	frontViewSectionRect.top = m_dFrontViewSectionMinY;
	frontViewSectionRect.bottom = m_dFrontViewSectionMaxY;

	RadarData *rd = NULL;
	std::vector<std::vector<osg::ref_ptr<RadarData>>>::reverse_iterator it;
	it = m_vecRadarGroupData.rbegin();
	
	int dFrontViewSectionHight = frontViewSectionRect.Height();
	float *interpolationValueOne=new float[dFrontViewSectionHight*2];//����ֵ�õ�
	for(int j=0;j<dFrontViewSectionHight*2;j++){interpolationValueOne[j]=0.0;}
	
	int dDataCount = _sampleCount-4;

	for(int i=0;i<12;i++){
		if((*it).size()<12){
			continue;
		}
		rd = (((*it)[i]).get());
		if ( !rd ){
			continue;
		}
		
		if (dDataCount < dFrontViewSectionHight && dDataCount > 0){////��������С�ڸ߶� ��Ҫ����
			/*float fTimes = ((float)dFrontViewSectionHight) / ((float)dDataCount);//ÿ��������ռ�ĸ߶�
			int dInsertCount = dFrontViewSectionHight - dDataCount; //��Ҫ��ȫ�����ӵ�����
			int dInsertPost = dDataCount /  dInsertCount; //����ȷ����ֵ��λ��
			float fValueOld = 0.;//��һ���ݵ�ֵ
			float fValueNew = 0.;//��ǰ���ݵ�ֵ
			//int y = 0;
			int y = m_dFrontViewSectionMinY;//�����������yҪ������ʼֵ
			if (fTimes >= 2.0){//��෭һ��
				dInsertPost = 1;
			}
			for ( int j=0; j<dDataCount; j++ ){//������x���������ϵĲ�ͬy��Ӧ��ֵ
				//fValueNew = (float)rd->getIndexData( i+m_dVecCorrection[i], NULL ) * (1.0 +  (float)_scaleRatio * (float)(i+1)/(float)dDataCount);//��Ҫ������
				fValueNew = (float)rd->getIndexData( j+m_dVecCorrection[i], NULL );//��Ҫ������
				COLORREF color ,colorHalf;
				if ( rd->getMark() ){
					color = RGB( 255, 255, 255 );
					colorHalf = RGB( 255, 255, 255 );
				}else{
					color = getColor(fValueNew);//���ݵ�ǰֵ����ɫ
					colorHalf = getColor((fValueOld + fValueNew) / 2.0);//ǰ������֮�����ɫ
				}
				if (j>0 && (j%dInsertPost == 0)){
					lpDC->SetPixelV( m_dFrontViewSectionMinX+i*5, y, colorHalf );
					if(i>0){
						std::vector<float> valueInterpolation=interpolation( interpolationValueOne[y-m_dFrontViewSectionMinY], (fValueOld+fValueNew)/2.0, 4 );//�õ�������ֵ1��ֵ2�����
						COLORREF colorTemp;
						for(int k=1;k<5;k++){
							colorTemp=getColor(valueInterpolation[4-k]);
							lpDC->SetPixelV( m_dFrontViewSectionMinX+i*5-k, y, colorTemp );
						}
					}
					interpolationValueOne[y-m_dFrontViewSectionMinY]=(fValueOld + fValueNew)/2.0;
					++y;
				}
				lpDC->SetPixelV( m_dFrontViewSectionMinX+i*5, y, color );
				if(i>0){
					std::vector<float> valueInterpolation=interpolation( interpolationValueOne[y-m_dFrontViewSectionMinY], fValueNew, 4 );//�õ�������ֵ1��ֵ2�����
					COLORREF colorTemp;
					for(int k=1;k<5;k++){
						colorTemp=getColor(valueInterpolation[4-k]);
						lpDC->SetPixelV( m_dFrontViewSectionMinX+i*5-k, y, colorTemp );
					}
				}
				interpolationValueOne[y-m_dFrontViewSectionMinY]=fValueNew;
				++y;
				fValueOld = fValueNew;
			}*/
			
			float fValueOld = 0.;//��һ���ݵ�ֵ
			float fValueNew = 0.;//��ǰ���ݵ�ֵ
			int lastY = m_dFrontViewSectionMinY;
			for ( int j = 0; j < dDataCount; j ++ ){//������x���������ϵĲ�ͬ��i��Ӧ��y��Ӧ��ֵ
				fValueNew = (float)rd->getIndexData( j+m_dVecCorrection[i], NULL );
				COLORREF color ,colorHalf;
				if ( rd->getMark() ){
					color = RGB( 255, 255, 255 );
					colorHalf = RGB( 255, 255, 255 );
				}else{
					color = getColor(fValueNew);//���ݵ�ǰֵ����ɫ
					colorHalf = getColor((fValueOld + fValueNew) / 2.0);//ǰ������֮�����ɫ
				}
				int y = (float)j/(float)dDataCount*(float)dFrontViewSectionHight+m_dFrontViewSectionMinY;
				lpDC->SetPixelV( m_dFrontViewSectionMinX+i*5, y, color );
				if(i>0){
					std::vector<float> valueInterpolation=interpolation( interpolationValueOne[y-m_dFrontViewSectionMinY], fValueNew, 4 );//�õ�������ֵ1��ֵ2�����
					COLORREF colorTemp;
					for(int k=1;k<5;k++){
						colorTemp=getColor(valueInterpolation[4-k]);
						lpDC->SetPixelV( m_dFrontViewSectionMinX+i*5-k, y, colorTemp );
					}
				}
				interpolationValueOne[y-m_dFrontViewSectionMinY]=fValueNew;
				if((y-lastY)>1){
					for(int k=lastY+1;k<y;k++){
						lpDC->SetPixelV( m_dFrontViewSectionMinX+i*5, k, colorHalf );
						if(i>0){
							std::vector<float> valueInterpolation=interpolation( interpolationValueOne[k-m_dFrontViewSectionMinY], (fValueOld+fValueNew)/2.0, 4 );//�õ�������ֵ1��ֵ2�����
							COLORREF colorTemp;
							for(int l=1;l<5;l++){
								colorTemp=getColor(valueInterpolation[4-l]);
								lpDC->SetPixelV( m_dFrontViewSectionMinX+i*5-l, k, colorTemp );
							}
						}
						interpolationValueOne[k-m_dFrontViewSectionMinY]=(fValueOld+fValueNew)/2.0;
					}
				}
				lastY=y;
				fValueOld = fValueNew;
			}
		}else{//�����������ڸ߶�,��Ҫѹ��������ֵ��ʾ��mapIndexToSectionYValue���ñ�����ϵ i/i������ = y/y������ �����i��Ӧ��
			for ( int j=0; j<dDataCount; j++ ){
				//short v = (float)rd->getIndexData( i+m_dVecCorrection[i], NULL ) * (1.0 + (float)_scaleRatio * (float)(i+1)/(float)iDataCount);//�ɰ���short�������ʾ����
				//float value = (float)rd->getIndexData( i+m_dVecCorrection[i]i, NULL ) * (1.0 +  (float)_scaleRatio * (float)(i+1)/(float)dDataCount);//��Ҫ������
				float fValue = (float)rd->getIndexData( j+m_dVecCorrection[i], NULL );//��Ҫ������
				//int y = mapIndexToSectionYValue( i, rd->getDataCount() );//�ɵ�
				int y = mapIndexToSectionYValue( j, rd->getDataCount() ) + m_dFrontViewSectionMinY;//�����������yҪ������ʼֵ
				COLORREF color;
				if ( rd->getMark() ){
					color = RGB( 255, 255, 255 );
				}else{
					color = getColor(fValue);
				}
				lpDC->SetPixelV( m_dFrontViewSectionMinX+i*5+1, y, color );
				if(i>0){
					std::vector<float> valueInterpolation=interpolation( interpolationValueOne[j], fValue, 4 );//�õ�������С�����
					COLORREF colorTemp;
					for(int k=1;k<5;k++){
						colorTemp=getColor(valueInterpolation[4-k]);
						lpDC->SetPixelV( m_dFrontViewSectionMinX+i*5+1-k, y, colorTemp );
					}
				}
				interpolationValueOne[j]=fValue;
			}
		}
	}

	//��Ӧ�����滭����
	int dDepthY;
	//int dFrontViewSectionHight = frontViewSectionRect.Height();
	if (dDataCount < dFrontViewSectionHight && dDataCount > 0){////��������С�ڸ߶� ��Ҫ����
		dDepthY=(int)((float)depthIndex/(float)dDataCount*(float)dFrontViewSectionHight) + m_dFrontViewSectionMinY;
	}else{//�����������ڸ߶�,��Ҫѹ��������ֵ��ʾ��mapIndexToSectionYValue���ñ�����ϵ i/i������ = y/y������ �����i��Ӧ��
		dDepthY = mapIndexToSectionYValue( depthIndex, rd->getDataCount() ) + m_dFrontViewSectionMinY;
	}
	CPen penRed( PS_SOLID,1,RGB(255,105,105));//��ɫ����
	lpDC->SelectObject(penRed);
	lpDC->MoveTo( m_dFrontViewSectionMinX, dDepthY );
	lpDC->LineTo( m_dFrontViewSectionMaxX, dDepthY );

	//��Ӧ�����滭����
	int dChannelX;
	dChannelX=channelIndex*5+m_dFrontViewSectionMinX;
	CPen penGreen(PS_SOLID ,1,RGB(105,255,105));//��ɫ����
	lpDC->SelectObject(penGreen);
	lpDC->MoveTo( dChannelX, m_dFrontViewSectionMinY );
	lpDC->LineTo( dChannelX, m_dFrontViewSectionMaxY );
}

COLORREF Radar3DWnd::getColor( float radarValue )
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

void Radar3DWnd::addGroupData( std::vector<osg::ref_ptr<RadarData>> radarDataGroup ){
	{
		OpenThreads::ScopedLock<OpenThreads::Mutex> lock( _mutex );
		m_vecRadarGroupDataLatest.push_back( radarDataGroup );
		while( m_vecRadarGroupDataLatest.size() > MAX_DRAW_COUNT ){
			m_vecRadarGroupDataLatest.erase( m_vecRadarGroupDataLatest.begin() );
		}
	}

	

	/*_maxTime = lpData->getTime();
	_minTime = _radarDataVector[0]->getTime();*/

	_newData++;
	if ( !_drawing ){
		return;
	}
}

void Radar3DWnd::addData( RadarData *lpData )
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
/*
void Radar3DWnd::addDataByIndex( RadarData *lpData, int index )
{	
	{
		OpenThreads::ScopedLock<OpenThreads::Mutex> lock( _mutex );
		_radarDataVector.clear();
		_radarDataVector.push_back( lpData );
	}


	_newData++;
	if ( !_drawing )
	{
		return;
	}
}*/

void Radar3DWnd::setData( RadarData *lpData )
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

void Radar3DWnd::clearData()
{
	_radarDataVector.clear();
	m_vecRadarGroupData.clear();
}

void Radar3DWnd::OnLButtonDown(UINT nFlags, CPoint point){
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
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

void Radar3DWnd::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	_clickUp = false;
	_clickDown = false;
	CWnd::OnLButtonUp(nFlags, point);
}

void Radar3DWnd::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
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

BOOL Radar3DWnd::OnEraseBkgnd(CDC* pDC)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	return TRUE;
	return CWnd::OnEraseBkgnd(pDC);
}

void Radar3DWnd::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
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

void Radar3DWnd::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������

	_needReinit = true;
}

void Radar3DWnd::setCorrection( int value, int index ){
	m_dVecCorrection[index] = value;
}

void Radar3DWnd::setChannelIndex( int value ){
	m_dChannelIndex = value;
}

void Radar3DWnd::setDepthIndex( int value ){
	m_dDepthIndex = value;
}