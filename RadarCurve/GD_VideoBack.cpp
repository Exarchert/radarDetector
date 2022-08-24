// GD_VideoBack.cpp : 实现文件
//

#include "stdafx.h"
//#include "CartoonEditor.h"
#include "GD_VideoBack.h"
#include <winbase.h>
#include <shlwapi.h>
#include <RadarCurve/RadarManager.h>
#include <gdiplus.h>
#include <Shlwapi.h>
using namespace Gdiplus;
#pragma comment( lib, "gdiplus.lib" )


#define TIMER_ID		1
#define TIMER_CHECK		2


#define BACK_PLAY_VIDEO     WM_USER + 1001


//GDI
ULONG_PTR gdiplusTokenBack;
BOOL InitGDIBack()
{

	INITCOMMONCONTROLSEX InitCtrls;

	InitCtrls.dwSize = sizeof(InitCtrls);

	InitCtrls.dwICC = ICC_WIN95_CLASSES;

	InitCommonControlsEx(&InitCtrls);

	AfxEnableControlContainer();

	GdiplusStartupInput gdiplusStartupInput;

	GdiplusStartup(&gdiplusTokenBack,&gdiplusStartupInput,NULL);

	//省略其他代码
	return FALSE;
}


void LoadPngBack(PTCHAR pPath,HDC& hDC,	CRect & wndRect)

{

	//if(_access(".//FileManege",0)==-1)
	//{
	//	//文件不在
	//	return;
	//}
	Graphics graphics(hDC);
	Image image(pPath, TRUE);
	//picture\\test.png是png文件的路径
	graphics.DrawImage(&image,0,0,wndRect.Width(),wndRect.Height());
}


//GDI  end



// GD_VideoBack 对话框

IMPLEMENT_DYNAMIC(GD_VideoBack, CDialog)

GD_VideoBack::GD_VideoBack(CWnd* pParent /*=NULL*/)
	: CDialog(GD_VideoBack::IDD, pParent)
{
	//初始化GDI
	InitGDIBack();
	m_bInitWndShow = false;
	m_bBackImageLeft = FALSE;
	m_bBackImageRight = FALSE;

	m_leftBackPlay = false;
	m_rightBackPlay = false;
	//获取存储image路径
	TCHAR szAppPath[MAX_PATH];
	::GetModuleFileName(AfxGetInstanceHandle(), szAppPath, MAX_PATH);
	::PathRemoveFileSpec(szAppPath);
	CString strIni = szAppPath;
	//strIni += L"\\ProjectData\\sysconfig.ini";
	strIni += L"\\Radar\\RadarConfig.ini";
	TCHAR szKeyVal[256];
	::GetPrivateProfileString(_T("VIDEOSOURCE"),_T("IMAGEPATH"),_T(""), szKeyVal, MAX_PATH, strIni);
	m_cPatrFile = szAppPath;
	ZeroMemory(szKeyVal,256*sizeof(TCHAR));
	::GetPrivateProfileString(_T("VIDEOSOURCE"),_T("INTERVAL"),_T(""), szKeyVal, MAX_PATH, strIni);
	m_iOnceTime = _ttoi(szKeyVal);
	if (m_iOnceTime == 0)
	{
		m_iOnceTime = 10;
	}
}

GD_VideoBack::~GD_VideoBack()
{

	//退出GDI
	GdiplusShutdown(gdiplusTokenBack);
}

void GD_VideoBack::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_VIDEOBACK_1, m_sWindowLeft);
	DDX_Control(pDX, IDC_STATIC_VIDEOBACK_2, m_sWindowRight);
	//DDX_Control(pDX, IDC_BUTTON_ACCEPTLEFT, m_btnBackLeft);
	//DDX_Control(pDX, IDC_BUTTON_ACCEPTRIGHT, m_btnBackRight);
}


BEGIN_MESSAGE_MAP(GD_VideoBack, CDialog)
	ON_WM_CREATE()
	ON_WM_TIMER()
	ON_WM_PAINT()
	ON_WM_DESTROY()
	ON_WM_MOVE()
	ON_WM_SIZE()
	ON_WM_CLOSE()
	ON_MESSAGE(BACK_PLAY_VIDEO,OnUserBackPlay)
END_MESSAGE_MAP()


// GD_VideoBack 消息处理程序


int GD_VideoBack::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码

	SetTimer(TIMER_ID, 100, NULL);

	SetTimer(TIMER_CHECK, 5000, NULL);
	PostMessage(WM_SIZE,0,0);
	return 0;
}


void GD_VideoBack::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	if (m_bInitWndShow == false)
	{
		m_bInitWndShow = true;
		ShowWindow(false);
	}

	if (TIMER_ID == nIDEvent)
	{
		if (m_leftBackPlay == true)
		{
			if ( true == m_bBackImageLeft)
			{
				m_bBackImageLeft = FALSE;

				USES_CONVERSION;
				CString strPath = m_cPatrFile;
				strPath += m_cstrImageNameLeft;		
				CRect wndRect;
				m_sWindowLeft.GetClientRect(wndRect);
				LoadPngBack(strPath.GetBuffer(),m_sWindowLeft.GetWindowDC()->m_hDC,wndRect);
			}
		}
		if (m_rightBackPlay == true)
		{
			if ( true == m_bBackImageRight)
			{
				m_bBackImageRight = FALSE;

				USES_CONVERSION;
				CString strPath = m_cPatrFile;

				strPath += m_cstrImageNameRight;	
				CRect wndRect;
				m_sWindowRight.GetClientRect(wndRect);
				LoadPngBack(strPath.GetBuffer(),m_sWindowRight.GetWindowDC()->m_hDC,wndRect);
			}
		}
	}
	CDialog::OnTimer(nIDEvent);
}

void GD_VideoBack::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialog::OnPaint()

	//CRect   rect;
	//GetClientRect(rect);  
	//dc.FillSolidRect(rect,RGB(255,255,255)); 

	//UpdateData();
}

void GD_VideoBack::OnDestroy()
{
	CDialog::OnDestroy();

	// TODO: 在此处添加消息处理程序代码

	KillTimer(TIMER_ID);
	KillTimer(TIMER_CHECK);
}

void GD_VideoBack::OnMove(int x, int y)
{
	CDialog::OnMove(x, y);

	// TODO: 在此处添加消息处理程序代码
}

void GD_VideoBack::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);
	// TODO: 在此处添加消息处理程序代码
	static int a = 0;
	if (a == 0)
	{
		a++; 
		return;
	}
	if (200 < cx  && 150 < cy)
	{

		CWnd* lWnd = NULL;
		int halfX = cx / 2;
		int halfY = cy / 2;

	
		int jiangeX = 10;
		int jiangeY = 20;
		int jiangeY2 = 10;
		int jiamMap = 3;

		m_sWindowLeft.SetWindowPos( NULL,jiangeX,jiangeY,halfX - jiangeX - jiangeX ,cy - jiangeY - jiangeY2,
			SWP_NOZORDER );
		m_sWindowLeft.UpdateWindow();
		m_bBackImageLeft = true;



		lWnd = GetDlgItem(IDC_STATIC_BACKLEFT);
		lWnd->SetWindowPos( NULL,jiamMap,jiamMap,halfX - jiamMap - jiamMap ,cy - jiamMap - jiamMap,
			SWP_NOZORDER );
		lWnd->UpdateWindow();

		m_sWindowRight.SetWindowPos( NULL,jiangeX + halfX,jiangeY,halfX - jiangeX - jiangeX ,cy - jiangeY - jiangeY2,
			SWP_NOZORDER );
		m_sWindowRight.UpdateWindow();
		m_bBackImageLeft = true;


		lWnd = GetDlgItem(IDC_STATIC_BACKRIGHT);
		lWnd->SetWindowPos( NULL,jiamMap + halfX,jiamMap,halfX - jiamMap - jiamMap ,cy - jiamMap - jiamMap,
			SWP_NOZORDER );
		lWnd->UpdateWindow();
		
		UpdateWindow();
	}
	m_bBackImageRight = true;
}

void GD_VideoBack::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CDialog::OnClose();
}

LRESULT GD_VideoBack::OnUserBackPlay(WPARAM wParam,LPARAM lParam)
{

	int iNumVideo = wParam;
	CString strName = *(CString*)lParam;
	if (strName.GetLength() < 1)
	{
		return 1L;
	}

	if (iNumVideo == 1)
	{
		m_bBackImageLeft = true;
		m_cstrImageNameLeft = strName;
	}
	if (iNumVideo == 2)
	{
		m_bBackImageRight = true;
		m_cstrImageNameRight = strName;
	}

	::SetWindowPos(GetSafeHwnd(),HWND_TOPMOST,0,0,0,0,SWP_NOSIZE|SWP_NOMOVE);
	VideoBackplay();
	return 1L;

}

void GD_VideoBack::VideoBackplay()
{
	CRect wndRect;
	this->GetClientRect(wndRect);
	OnSize(0,wndRect.Width(),wndRect.Height());
	m_leftBackPlay = true;
	m_rightBackPlay = true;
	ShowWindow(SW_SHOW);	//SW_NORM
	UpdateWindow();
	UpdateData();
}


void GD_VideoBack::SetPicPath( CString picPath )
{
	m_cPatrFile = picPath;
}