// ProcessDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "RadarCurve.h"
#include "ProcessDlg.h"
#include "OpenThreads\Thread"
#include "OpenThreads\Block"


// CProcessDlg 对话框


IMPLEMENT_DYNAMIC(CProcessDlg, CDialog)

CProcessDlg::CProcessDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CProcessDlg::IDD, pParent)
{

	m_bUptade = false;
	m_iChangeStep = 1;
	m_bThread = false;
	m_bClose = false;


	
	
}

CProcessDlg::~CProcessDlg()
{
}

void CProcessDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESS_COUNT, m_processCountCtrl);
}


BEGIN_MESSAGE_MAP(CProcessDlg, CDialog)
	ON_BN_CLICKED(IDCANCEL, &CProcessDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, &CProcessDlg::OnBnClickedOk)
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CProcessDlg 消息处理程序

void CProcessDlg::StartProcess( int allCount )
{
	m_processCountCtrl.SetRange(0, allCount);
	m_processCountCtrl.SetPos( 0 );
	ShowWindow( SW_SHOW );
	m_bClose = false;
}
void CProcessDlg::Step( int stepCount )
{
	m_processCountCtrl.SetPos( m_processCountCtrl.GetPos() + stepCount );
}
void CProcessDlg::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	if (true == m_bClose)
	{
		m_bClose = false;
		OnCancel();
	}
	
}

void CProcessDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	OnOK();
}

void CProcessDlg::StepEx( int setCount )
{
	m_bUptade = false;
	if (0 == setCount)
	{
		m_processCountCtrl.SetPos( m_processCountCtrl.GetPos() + m_iChangeStep );
	}
	else
	{
		m_processCountCtrl.SetPos( m_processCountCtrl.GetPos() + setCount );
	}
	UpdateData();
	UpdateWindow();
}

void CProcessDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialog::OnPaint()


	//AfxMessageBox(L"23");
	m_bUptade = true;
	//if (false == m_bThread)
	//{
	//	m_bThread = true;
	//	//m_pThread = AfxBeginThread(ThreadFunc, NULL); //启动线程

	//	saThread = new SaveDataThreadMy;
	//	saThread->start();
	//}
	//
}