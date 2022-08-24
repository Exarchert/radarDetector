// RadarCurveWnd.cpp : ʵ���ļ�
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

#define STAFF_HEIGHT 30 //����̶�����߶�
#define GRID_SPACE 15//�����е��߶μ������
#define POINT_SPACE 10
#define DATA_BLOCK_HEIGHT 40.0f
#define DATA_BLOCK_WIDTH 20.0f

#define TIME_STAFF_WIDTH 20 //ʱ���̶�������

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
		, 0, 0, 0, 0, 0, L"����"
		);

	_sampleCount = 0;
	_sampleRatio = 51.2;

	_lpDC = NULL;
	//_currentRadarData = NULL;

	_newData = 0;
	_needReinit = false;
	_dataAvailabled = false;

	m_dCorrection=0;
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


//���ò�������
void RadarCurveWnd::SetSampleCount( int count ){
	_sampleCount = count;
}
//���ò�����
void RadarCurveWnd::SetSampleRatio( double value ){
	_sampleRatio = value;
}

void RadarCurveWnd::OnTimer(UINT_PTR nIDEvent)
{
	CWnd::OnTimer(nIDEvent);
	Invalidate();
}

//ÿ�λ��ƵĹ��� _drawing===trueʱ����Ƴ�ͼ��
void RadarCurveWnd::OnPaint()
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

	if (!_lpDC){
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
	DrawBk(&dcMem);//����

	int newData = 0;
	{
		OpenThreads::ScopedLock<OpenThreads::Mutex> lock(_mutex);
		_tmpDataVector = _radarDataVector;
		newData = _newData;
		_newData = 0;
	}

	if ( _drawing ){
		DrawCurve(&dcMem);//����ͼ
		//DrawSection( &dcMem, newData );//����ͼ
	}
	//DrawScaleFlag( &dcMem );//���������������
	pDC->BitBlt(0,0,rc.Width(),rc.Height(),&dcMem,0,0,SRCCOPY);//20210803hjl
	dcMem.DeleteDC();   
	bmp.DeleteObject(); 
	EndPaint( &pat );
}

//���Ʊ���   �ڵ׺�����
void RadarCurveWnd::DrawBk( CDC *lpDC )
{
	CRect rc;
	GetClientRect(&rc);

	int width = GRID_WIDTH;//����ͼ���
	//int height = rc.Height();
	int curvRectX1 = rc.Width() - GRID_WIDTH - TIME_STAFF_WIDTH;//����ͼX1  TIME_STAFF_WIDTHʱ���̶�������
	int curvRectX2 = curvRectX1 + GRID_WIDTH;//����ͼX2
	int curvRectY1 = STAFF_HEIGHT;//STAFF_HEIGHT����̶�����߶�
	int curvRectY2 = rc.Height();


	//CBrush blackBruh( RGB( 0, 0, 0 ) );//���ױ�����ɫ
	///*CRect r;
	//r.left = 0;
	//r.right = curvRectX1;
	//r.top = 0;
	//r.bottom = r.top + rc.Height();*/
	//CRect r(0,0,curvRectX1,rc.Height());
	//lpDC->FillRect( r, &blackBruh );


	/*-----------------------��ʼ�̻�����ͼ����-----------------------*/
	CBrush brushCurvRect(RGB(75,75,75));//����ͼ����ǳ��ɫ
	CRect rectCurvRect(curvRectX1,curvRectY1 ,curvRectX2,curvRectY2);
	lpDC->FillRect(rectCurvRect,&brushCurvRect);
	//���������
	int xLineCount = ( curvRectX2 - curvRectX1 ) / GRID_SPACE;
	int yLineCount = ( curvRectY2 - curvRectY1 ) / GRID_SPACE ;
	int curvRectInterP5 = xLineCount / 10.0 *3.0 ;

	//bg-grid-vertical ���������е�������
	CPen pen_grid_vert(PS_DOT,1,RGB(105,255,105));//��ɫ����
	lpDC->SelectObject(pen_grid_vert);
	lpDC->SetBkMode(TRANSPARENT);
	for(int i=1;i<xLineCount;++i){
		lpDC->MoveTo(curvRectX1 + i * GRID_SPACE, curvRectY1);
		lpDC->LineTo(curvRectX1 + i * GRID_SPACE, curvRectY2);
	}

	//bg-grid-mid ���������еĺ�����
	CPen pen_grid_mid(PS_DOT,1,RGB(255,105,105));//��ɫ����
	lpDC->SelectObject(pen_grid_mid);
	lpDC->MoveTo(curvRectX1+GRID_SPACE/2.0*xLineCount,curvRectY1);
	lpDC->LineTo(curvRectX1+GRID_SPACE/2.0*xLineCount,curvRectY2);

	//bg-grid-horizon ���������е��̺���
	CPen pen_grid_hori(PS_DOT,1,RGB(105,255,105));//��ɫ����
	lpDC->SelectObject(pen_grid_hori);
	lpDC->SetBkMode(TRANSPARENT);
	for(int i=1;i<=yLineCount;++i)
	{
		lpDC->MoveTo(curvRectX1,curvRectY1+i * GRID_SPACE);
		lpDC->LineTo(curvRectX2,curvRectY1+i * GRID_SPACE);
	}

	//���ſ̶� ʱ��
	int staffWidth = TIME_STAFF_WIDTH;//����ʱ���̶ȿ��
	//�̶ȴ��İ׵�
	//lpDC->FillSolidRect( curvRectX2, curvRectY1, TIME_STAFF_WIDTH, curvRectY2 - curvRectY1, RGB( 255, 255, 255 ) );//(����x��y,��ȣ��߶ȣ���ɫ)
	lpDC->FillSolidRect( curvRectX2, curvRectY1, TIME_STAFF_WIDTH, curvRectY2 - curvRectY1, RGB( 240, 240, 240 ) );//(����x��y,��ȣ��߶ȣ���ɫ)
	//����
	int curStaffCxLen = staffWidth / 3;
	CString strMark;
	CPen pen_staff_vline(PS_SOLID,1,RGB(0,0,0));
	lpDC->SelectObject(pen_staff_vline);
	int staffStartX = curvRectX2 + 2;
	//��ֱ����
	lpDC->MoveTo(staffStartX,curvRectY1);
	lpDC->LineTo(staffStartX,curvRectY2);
	//�����
	int count = int( 1 / _sampleRatio * _sampleCount + 0.5 );//��������
	float staffYSpace = (curvRectY2 - curvRectY1) * 1.0f / count;//
	CFont *lpOldFont = lpDC->SelectObject( &_textFont );
	count += 1;
	//����̶ȳ��е��߼�����
	for(int i = 0;i <= count;i ++ ){
		if ( i % 10 == 0 ){//����̶ȳ��е�ˮƽ���߼�����
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
		}else{//����̶ȳ��е�ˮƽ����
			if ( i == count ){
				lpDC->MoveTo(staffStartX, curvRectY1 + i*staffYSpace - 1);
				lpDC->LineTo(staffStartX+curStaffCxLen / 2, curvRectY1 + i*staffYSpace - 1);
			}else{
				lpDC->MoveTo(staffStartX, curvRectY1 + i*staffYSpace);
				lpDC->LineTo(staffStartX+curStaffCxLen / 2, curvRectY1 + i*staffYSpace);
			}
		}
	}
	/*-----------------------��������ͼ�����̻�-----------------------*/

	lpDC->SelectObject( lpOldFont );
	//����ͼ����XY
	_curveMinX = curvRectX1;
	_curveMaxX = curvRectX2;
	_curveMinY = curvRectY1;
	_curveMaxY = curvRectY2;
	//����ͼ����XY
	_sectionMinX = 0;
	_sectionMaxX = curvRectX1;
	_sectionMinY = curvRectY1;
	_sectionMaxY = curvRectY2;
	//����̶�����Y(X�벨��ͼһ��)
	_staffMinY = 0;
	_staffMaxY = curvRectY1;
}

//������������ؼ�
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

	lpDC->Ellipse( upEllipse );//��ɫ�������϶˵���
	lpDC->Ellipse( downEllipse);//��ɫ�������¶˵���
}

//���������������
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

//���������������
CRect RadarCurveWnd::GetDownEllipse(){
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

//�����״�����ֵradarValue�����״�����ֵ��������������ֵindexRatio���㲨��ͼXֵ
int RadarCurveWnd::mapRadarValueToXValue( float radarValue, float indexRatio)
{
	float ratio = radarValue / SHRT_MAX;
	//���������������״�����ֵռshort��Χ�ı�ֵ
	radarValue = _startPos + radarValue;
	if(_scaleRatio>_startPos){
		ratio = ratio * (1.0 + (float)(_scaleRatio-_startPos-1) * indexRatio);
	}else{
		ratio = ratio * (1.0 + (float)(_startPos-_scaleRatio-1) * (1-indexRatio));
	}
	ratio += 1;
	ratio /= 2;
	if ( ratio > 1 ){
		ratio = 1;
	}
	if ( ratio < 0 ){
		ratio = 0;
	}
	
	//�����״�����ֵ��short��Χ�ı�ֵ������ڲ���ͼ�е�xֵ
	float result = ( ratio * ( _curveMaxX - _curveMinX ) + _curveMinX );
	if ( result > _curveMaxX ){
		result = _curveMaxX;
	}

	return result;
}


//int RadarCurveWnd::mapTimeToYValue( float t )
//{
//	float st = ( _maxTime - t ) /*/ 1000.0f*/;
//
//	float y = st * POINT_SPACE;
//	if ( y > _curveMaxY )
//	{
//		return -1;
//	}
//	return _curveMaxY - y;
//}

//�����״����ݵ�����index����������sampleCount��������Ʋ��ε�yֵ
int RadarCurveWnd::mapIndexToCurveYValue( int index, int sampleCount ){
	//float st = index * DATA_BLOCK_HEIGHT / dataCount;
	float y = index * 1.0f * ( _curveMaxY - _curveMinY ) / sampleCount + _curveMinY;//��������y/Y=index/sampleCount
	if ( y > _curveMaxY ){//������� 
		return -1;
	}
	return int(y);
}

//�����״����ݵ�������������Ʋ��׵�yֵ
int RadarCurveWnd::mapIndexToSectionYValue( int index, int sampleCount ){
	float y = index * 1.0f * ( _sectionMaxY - _sectionMinY ) / sampleCount/* + _sectionMinY*/;//����ͼ���ȴ����һ�ݻ����ģ����Բ���+_sectionMinY
	return y;
}

//��ʼ����
void RadarCurveWnd::StartDraw(){
	_drawing = true;
	this->SetTimer(1, 35, NULL );
}

//ֹͣ����
void RadarCurveWnd::EndDraw(){
	_drawing = false;
	this->KillTimer(1 );
}

//���Ʋ���ͼ
void RadarCurveWnd::DrawCurve( CDC *lpDC ){
	if ( !_drawing ){
		return;
	}
	if ( _tmpDataVector.size() == 0 ){
		return;
	}

	//_maxTime = _tmpDataVector.at(_tmpDataVector.size() - 1)->getTime();
	//_minTime = _tmpDataVector.at(0)->getTime();

	CPen pen_grid_vert(PS_SOLID,1,RGB(200,255,105));//���û�ɫ
	lpDC->SelectObject(pen_grid_vert);

	RadarData *rd = NULL;
	RadarDataVector::reverse_iterator it;//���������

	bool bFirstFlag = false;
	//bool curveToBreak = false;

	/*
	ConfigureSet *cs = RadarManager::Instance()->getConfigureSet();
	std::string strPrec = cs->get("radar", "prec");
	int iprec = atoi( strPrec.c_str() );
	*/

	int dSampleCount;
	//for (it = _tmpDataVector.rbegin(); it != _tmpDataVector.rend(); it++){//������ĩλ(�µ�)��ǰ(�ɵ�)��
	//	rd = ((*it).get());
	//	if ( !rd ){
	//		continue;
	//	}
	//	dSampleCount = rd->getDataCount();
	//	for ( int i = 0; i < dSampleCount; i ++ ){
	//		float t;
	//		//short v = rd->getIndexData( i, &t ) ;
	//		short v = rd->getIndexData( i + m_dCorrection, &t );//������������ȡ����ֵ 
	//		{
	//		// curve line
	//			int y = mapIndexToCurveYValue( i, dSampleCount );//����y
	//			//int x = mapRadarValueToXValue( v );
	//			int x = mapRadarValueToXValue( v,(float)(i+1)/(float)dSampleCount );//����x

	//			if ( y > _curveMinY && y <= _curveMaxY){//���ж�������ʹ���ڷ�Χ��
	//				if ( !bFirstFlag ){//��һ����moveTo ֮����LineTo
	//					lpDC->MoveTo(x, y);
	//					bFirstFlag = true;
	//				}else{
	//					lpDC->LineTo( x, y );
	//				}
	//			}
	//			if ( y > _curveMaxY ){
	//				break;
	//			}
	//		}
	//	}
	//	break;
	//}
	it = _tmpDataVector.rbegin();//�״������������������µ�����
	rd = ((*it).get());
	if ( !rd ){
		return;
	}
	dSampleCount = rd->getDataCount();//��������
	for ( int i = 0; i < dSampleCount; i ++ ){
		float t;
		//short v = rd->getIndexData( i, &t ) ;
		short v = rd->getIndexData( i + m_dCorrection, &t );//������������ȡ����ֵ 
		{
		// curve line
			int y = mapIndexToCurveYValue( i, dSampleCount );//����y
			//int x = mapRadarValueToXValue( v );
			int x = mapRadarValueToXValue( v,(float)(i+1)/(float)dSampleCount );//����x

			if ( y > _curveMinY && y <= _curveMaxY){//���ж�������ʹ���ڷ�Χ��
				if ( !bFirstFlag ){//��һ����moveTo ֮����LineTo
					lpDC->MoveTo(x, y);
					bFirstFlag = true;
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

/*
void RadarCurveWnd::DrawCurveNew( CDC *lpDC )//���Ʋ���ͼ
{
	if ( !_drawing ){
		return;
	}
	if ( !_dataAvailabled ){
		return;
	}
	//if ( _currentRadarData == NULL ){
	//	return;
	//}

	CPen pen_grid_vert(PS_SOLID,1,RGB(200,255,105));//���û�ɫ
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
		short value = _currentRadarData->getIndexData( i + m_dCorrection, &t );//ȡ�״����ݵ�i�����ֵ
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
}*/

//���Ʋ�������
void RadarCurveWnd::DrawSection( CDC *lpDC, int newData )
{
	if ( !_drawing ){
		return;
	}
	if (_tmpDataVector.size() == 0){
		return;
	}

	//�Ϸ��̶�����
	CRect staffRect;
	staffRect.left = _sectionMinX;
	staffRect.right = _sectionMaxX;
	staffRect.top = _staffMinY;
	staffRect.bottom = _staffMaxY;
	//��������
	CRect sectionRect;
	sectionRect.left = _sectionMinX;
	sectionRect.right = _sectionMaxX;
	sectionRect.top = _sectionMinY;
	sectionRect.bottom = _sectionMaxY;

	lpDC->FillRect( staffRect, &_staffBrush );//�̶Ȱ׵�

	CBrush blackBruh( RGB( 0, 0, 0 ) );
	//����_bmpDCSource��_bmpDCTmp
	if (!_bmpDCSource.isInit() || _needReinit){//_bmpDCSource���Լ������CBmpDC��Ķ������ڰ�dc�浽bmp���
		_bmpDCSource.init( _lpDC, sectionRect );
		//_bmpDCSource.drawRect(&blackBruh);
	}
	if ( !_bmpDCTmp.isInit() || _needReinit){
		_bmpDCTmp.init(_lpDC, sectionRect);
		//_bmpDCTmp.drawRect(&blackBruh);
	}
	//BitBltһ����������� �ú�����ָ����Դ�豸���������е����ؽ���λ��ת�����Դ��͵�Ŀ���豸����
	_bmpDCTmp.getDC()->BitBlt(0, 0, sectionRect.Width() - newData, sectionRect.Height(), _bmpDCSource.getDC(), newData, 0, SRCCOPY );

	CPen *lpOldPen = lpDC->SelectObject( &_staffPen );
	//����̶Ⱥ���
	int staffBaseLineY = _staffMaxY - 5;
	lpDC->MoveTo(_sectionMinX, staffBaseLineY);
	lpDC->LineTo( _sectionMaxX, staffBaseLineY );

	CFont *lpOldFont = lpDC->SelectObject( &_textFont );

	RadarData *rd = NULL;
	RadarDataVector::reverse_iterator it;//��������� rend()�ǵ�һ��Ԫ�� rbegin()�����һ��Ԫ��  [0,1,2,3]��rpos[1]=2

	bool moveTo = false;
	bool curveToBreak = false;
	bool sectionToBreak = false;
	int row = 0;

	int dataNum = newData;//newdataΪ��������������
	if ( _needReinit ){//��һ��
		dataNum = _tmpDataVector.size();
	}
	for (it = _tmpDataVector.rbegin(); it != _tmpDataVector.rend(); it++, row++){
		rd = ((*it).get());
		if ( !rd ){
			continue;
		}
		int x = ( _sectionMaxX - _sectionMinX ) - row;//��������Χ��x����ָ��ѭ���е�ǰ�������һ�� ѭ����row���𽥱��x�𽥱�С����ͼ˳���Ǵ������ɵ�
		//int x = ( _sectionMaxX - _sectionMinX )- row + _sectionMinX;//�ɵ�
		//�����Ƶ�ǰ��Ļ�ܷ��µ����µ�����
		if ( x < 0 ){
			sectionToBreak = true;
		}

		//�����Ϸ�����̶�ֵ
		if ( x % 50 == 0 ){//��50���ػ�һ��
			short r = rd->getPrec();
			lpDC->MoveTo( x, staffBaseLineY );
			lpDC->LineTo( x, _staffMinY  + STAFF_HEIGHT / 4.0f * 3);
			CString str;
			str.Format( L"%0.2f", rd->getPrecLen() );//��ȡҪ���Ƶľ��� "%0.2f"������λС��
			CRect rtext( x - 25, _staffMinY + 10, x + 25,_staffMinY  + STAFF_HEIGHT / 4.0f * 3 );//��left,below,right,top��
			lpDC->DrawText( str, rtext, DT_CENTER | DT_BOTTOM);//���ƾ���
		}

		if ( dataNum >= 0 ){
			int sectionX = x - _sectionMinX;
			if ( rd->getMark() ){//�����Ϊ��Ǵ�����õ���ֱ�ӻ��Ƴ�һ����ֱ����
				int dSampleCount = rd->getDataCount();//��ֵ�ϵ��ڲ�������-2
				CPen *lpOldPen = _bmpDCTmp.getDC()->SelectObject( &_markPen );
				//_bmpDCTmp.getDC()->MoveTo( sectionX, _sectionMaxY );//�ɵ�
				//_bmpDCTmp.getDC()->LineTo( sectionX, _sectionMinY );//�ɵ�
				_bmpDCTmp.getDC()->MoveTo( sectionX, 10 );
				_bmpDCTmp.getDC()->LineTo( sectionX, dSampleCount * 2 - 20 );
				_bmpDCTmp.getDC()->SelectObject( lpOldPen );
			}else{//��Ҫ����
 				int dSampleCount = rd->getDataCount();
				int dSectionHeight = sectionRect.Height();
				if (dSampleCount < dSectionHeight && dSampleCount > 0){//���ݸ߶����ڻ�������߶���Ҫ����
					float fBeishu = ((float)dSectionHeight) / ((float)dSampleCount);//ÿ��������ռ�ĸ߶�

					int dInsertCount = dSectionHeight - dSampleCount; //��Ҫ��ȫ�����ӵ�����
					int dInsertPost = dSampleCount / dInsertCount; //����ȷ����ֵ��λ�� ��������һ��

					float fOldValue = 0.;//��һ���ݵ�ֵ
					float fNewValue = 0.;//��ǰ���ݵ�ֵ
					int y = 0;

					if (fBeishu >= 2.0){//��෭һ��
						dInsertPost = 1;
					}

					for ( int i = 0; i < dSampleCount; i ++ ){//������x���������ϵĲ�ͬy��ֵ
						fNewValue = (float)rd->getIndexData( i + m_dCorrection, NULL ) * (1.0 + (float)_scaleRatio * (float)(i+1)/(float)dSampleCount);
						if ( !sectionToBreak ){
							COLORREF c ,cHalf;
							/*if ( rd->getMark() ){
								c = RGB( 255, 255, 255 );
								cHalf = RGB( 255, 255, 255 );
							}else{
								c = getColor(fNewValue);//���ݵ�ǰֵ����ɫ
								cHalf = getColor((fOldValue + fNewValue) / 2.0);//ǰ������֮�����ɫ
							}*/
							c = getColor(fNewValue);//���ݵ�ǰֵ����ɫ
							cHalf = getColor((fOldValue + fNewValue) / 2.0);//����һ��ֵ��ֵ������ɫ

							if ( x > 0 && x <= _sectionMaxX ){//�ڲ��׻�������Χ��
								if (i > 0 && (i%dInsertPost == 0)){
									_bmpDCTmp.getDC()->SetPixelV( ( sectionX ), y, cHalf );
									++y;
								}
								_bmpDCTmp.getDC()->SetPixelV( ( sectionX ), y, c );
								++y;
							}
						}
						fOldValue = fNewValue;
					}
				}else{//�����������ڸ߶�,��Ҫѹ��������ֵ��ʾ
					for ( int i = 0; i < dSampleCount; i ++ ){
						//short v = (float)rd->getIndexData( i, NULL ) * (1.0 + (float)_scaleRatio * (float)(i+1)/(float)dSampleCount);//�ɰ���short�������ʾ����
						float v = (float)rd->getIndexData( i + m_dCorrection, NULL ) * (1.0 + (float)_scaleRatio * (float)(i+1)/(float)dSampleCount);
						if ( !sectionToBreak ){
							int y = mapIndexToSectionYValue( i, rd->getDataCount() );//mapIndexToSectionYValue���ñ�����ϵ i/i������ = y/y������ �����i��Ӧ��y
							//int y = mapIndexToSectionYValue( i, rd->getDataCount() ) + _sectionMinY;//�ɵ�
							COLORREF c;
							if ( rd->getMark() ){
								c = RGB( 255, 255, 255 );
							}else{
								c = getColor(v);
							}

							if ( x > 0 && x <= _sectionMaxX ){//���ڷ�Χ��
								_bmpDCTmp.getDC()->SetPixelV( ( sectionX ), y, c );
							}
						}
					}
				}
			}
			dataNum --;
		}

		if ( sectionToBreak ){
			break;
		}
	}

	_bmpDCSource.getDC()->BitBlt(0, 0, sectionRect.Width(), sectionRect.Height(), _bmpDCTmp.getDC(), 0, 0, SRCCOPY );

	lpDC->BitBlt( staffRect.left, sectionRect.top, sectionRect.Width(), sectionRect.Height(), _bmpDCSource.getDC(), 0, 0, SRCCOPY );


	lpDC->SelectObject( lpOldPen );
	lpDC->SelectObject( lpOldFont );

	_needReinit = false;
	
}

//��ȡ�״�����ֵ��Ӧrgb
COLORREF RadarCurveWnd::getColor( float radarValue ){
	float ratio = radarValue / SHRT_MAX;
	ratio += 1;
	ratio /= 2;
	if ( ratio > 1 ){
		ratio = 1;
	}
	if ( ratio < 0 ){
		ratio = 0;
	}
	COLORREF color = RGB( int(ratio * 255), int(ratio * 255), int(ratio * 255 ) );
	return color;
}

//�������������
void RadarCurveWnd::addData( RadarData *lpData ){
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

//���õ�ǰ����
void RadarCurveWnd::setData( RadarData *lpData ){
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

//�������
void RadarCurveWnd::clearData(){
	_radarDataVector.clear();
}

//����������Ƶ���ش���
void RadarCurveWnd::OnLButtonDown(UINT nFlags, CPoint point){
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

void RadarCurveWnd::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	_clickUp = false;
	_clickDown = false;
	CWnd::OnLButtonUp(nFlags, point);
}

//����������Ƶ���ش���
void RadarCurveWnd::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	int curX = point.x - RADIUS / 2;
	if ( _clickUp ){
		_startPos = int ( ( curX - _curveMinX ) * 1.0f / ( _curveMaxX - _curveMinX ) * 100);
		if ( _startPos < 0 ){
			_startPos = 0;
		}
		Invalidate();
	}else if ( _clickDown ){
		_scaleRatio = int ( ( curX - _curveMinX ) * 1.0f / ( _curveMaxX - _curveMinX ) * 100);
		if ( _scaleRatio < 1 ){
			_scaleRatio = 1;
		}
		Invalidate();
	}
	CWnd::OnMouseMove(nFlags, point);
}

BOOL RadarCurveWnd::OnEraseBkgnd(CDC* pDC)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	return TRUE;
	return CWnd::OnEraseBkgnd(pDC);
}

void RadarCurveWnd::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
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

void RadarCurveWnd::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������

	_needReinit = true;
}

//���õ�ǰͨ�����ӳ�΢��ֵ
void RadarCurveWnd::setCorrection( int value )
{
	m_dCorrection = value;
}
