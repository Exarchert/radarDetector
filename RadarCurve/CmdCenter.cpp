#include "StdAfx.h"
#include <TlHelp32.h>

#include "RadarCurve/CmdCenter/CmdCenter.h"
#include <RadarCurve/RadarManager.h>
#include <Kernel/KernelExport.h>
#include "GD_VideoShow.h"
#include "GD_VideoBack.h"

//这个文件OCX有效
GD_VideoShow* g_pVideoPlayDlg = NULL;
GD_VideoBack* g_pVideoPlaybackDlg = NULL;

CString  g_strDisposeDataPath;
CString  g_strDisposeDataName;

CString  g_strShowDataPath;
CString  g_strShowDataName;

 void Radar::CmdRadarManagerInit()
 {
	 //初始化RadarManager
	 RadarManager::Instance()->init();

	 AFX_MANAGE_STATE(AfxGetStaticModuleState());
	 CString strImagePath = Kernel::GetpKC()->m_BinPath + L"\\ProjectData\\DataImage\\";
	 if (!g_pVideoPlayDlg || !g_pVideoPlaybackDlg)
	 {
		g_pVideoPlayDlg = new GD_VideoShow();
		g_pVideoPlaybackDlg = new GD_VideoBack();

		
	 }
	 
	 g_pVideoPlayDlg->SetPicPath(strImagePath);
	 g_pVideoPlaybackDlg->SetPicPath(strImagePath );
	 USES_CONVERSION;
	 RadarManager::Instance()->setPicPath( W2A(strImagePath) );

	 g_pVideoPlayDlg->Create(GD_VideoShow::IDD);
	 g_pVideoPlayDlg->ShowWindow(false);
	 
	 g_pVideoPlaybackDlg->Create(GD_VideoBack::IDD);
	 g_pVideoPlaybackDlg->ShowWindow(false);
	 RadarManager::Instance()->setVideoWnd( g_pVideoPlaybackDlg );


	 //外部exe的调用
	 TCHAR szAppPath[MAX_PATH];
	 ::GetModuleFileName(AfxGetInstanceHandle(), szAppPath, MAX_PATH);
	 ::PathRemoveFileSpec(szAppPath);

	 CString strIni = szAppPath;
	 strIni += L"\\ProjectData\\sysconfig.ini";
	 TCHAR szKeyVal[256];
	 ZeroMemory(szKeyVal,256*sizeof(TCHAR));
	 ::GetPrivateProfileString(_T("WINEXEC"),_T("DISPOSEDATAPATH"),_T(""), szKeyVal, MAX_PATH, strIni);
	 g_strDisposeDataPath = szKeyVal;
	 if (!PathIsDirectory(g_strDisposeDataPath) ) 
	 {
		 g_strDisposeDataPath = szAppPath;
		 g_strDisposeDataPath += L"\\";
		 g_strDisposeDataPath += szKeyVal;
	 } 
	 ZeroMemory(szKeyVal,256*sizeof(TCHAR));
	 ::GetPrivateProfileString(_T("WINEXEC"),_T("DISPOSEDATANAME"),_T(""), szKeyVal, MAX_PATH, strIni);
	 g_strDisposeDataName = szKeyVal;
	 ZeroMemory(szKeyVal,256*sizeof(TCHAR));
	 ::GetPrivateProfileString(_T("WINEXEC"),_T("SHOWDATAPATH"),_T(""), szKeyVal, MAX_PATH, strIni);
	 g_strShowDataPath = szKeyVal;
	 if (!PathIsDirectory(g_strShowDataPath) ) 
	 {
		 g_strShowDataPath = szAppPath ;
		 g_strShowDataPath += L"\\" ;
		 g_strShowDataPath += szKeyVal;
	 } 
	 ZeroMemory(szKeyVal,256*sizeof(TCHAR));
	 ::GetPrivateProfileString(_T("WINEXEC"),_T("SHOWDATANAME"),_T(""), szKeyVal, MAX_PATH, strIni);
	 g_strShowDataName = szKeyVal;
 }

void Radar::CmdStartRadarWorkDlg()
{
	USES_CONVERSION;


	CString strImagePath = Kernel::GetpKC()->m_BinPath + L"\\ProjectData\\DataImage\\";
	g_pVideoPlaybackDlg->SetPicPath(strImagePath );
	g_pVideoPlayDlg->SetPicPath(strImagePath);

	RadarManager::Instance()->setPicPath( W2A(strImagePath) );

// 	初始化GD_VideoShow、GD_VideoBack
// 	HWND hWnd = Kernel::GetpKC()->GetRenderWnd();
// 
// 	g_pVideoPlayDlg = new GD_VideoShow((CWnd *)&hWnd);
// 	g_pVideoPlaybackDlg = new GD_VideoBack((CWnd *)&hWnd);

	//g_pVideoPlayDlg->Create(GD_VideoShow::IDD);
	g_pVideoPlayDlg->ShowWindow(false);

	//g_pVideoPlaybackDlg->Create(GD_VideoBack::IDD);
	g_pVideoPlaybackDlg->ShowWindow(false);
	RadarManager::Instance()->setVideoWnd(g_pVideoPlaybackDlg);

	if(!RadarManager::Instance()->startNewWork())
	{
		return;
	}

	if ( RadarManager::Instance()->getCurProject() )
	{
		g_pVideoPlayDlg->SetProPath( A2W(RadarManager::Instance()->getCurProject()->getProjectTab().c_str() ));
	}

	//开始保存图片
	g_pVideoPlayDlg->StartSaveImage(true);
}

void Radar::CmdEndRadarWorkDlg()
{
	//具体的实现
	if ( !RadarManager::Instance()->stopWork() )
	{
		return;
	}

	//停止保存图片
	g_pVideoPlayDlg->StopSaveImage(true);
}

void Radar::CmdRadarRealTimeCurves()
{
	RadarManager::Instance()->showCurCurve();
}

void Radar::CmdRadarHistoryCurves()
{
	RadarManager::Instance()->showHistCurve();
}

void Radar::CmdGPSRealTimeLocate()
{
	RadarManager::Instance()->setShowGpsPos( !RadarManager::Instance()->getShowGpsPos() );
}

void Radar::CmdRadarParametersSettings()
{
	RadarManager::Instance()->showParamDialog();
}

//启动Windows程序
bool DisposeWinExec(CString strName,CString strPath)
{
	if (strName.IsEmpty() || strPath.IsEmpty())
	{
		return false;
	}
	HANDLE SnapShot=::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);
	if(SnapShot==NULL)
	{
		::MessageBox(NULL,L"检测当前进程失败!",NULL,0);
		return false;
	}

	SHFILEINFO shSmall;
	int nIndex;
	CString str,prcnum;
	PROCESSENTRY32 ProcessInfo;//声明进程信息变量
	ProcessInfo.dwSize=sizeof(ProcessInfo);//设置ProcessInfo的大小
	//返回系统中第一个进程的信息
	BOOL Status=Process32First(SnapShot,&ProcessInfo);
	int m_nProcess=0,num=0;
	bool flag = true;
	while(Status)
	{
		m_nProcess++;
		num++;
		//获取进程文件信息
		SHGetFileInfo(ProcessInfo.szExeFile,0,&shSmall,
			sizeof(shSmall),SHGFI_ICON|SHGFI_SMALLICON);
		//在列表控件中添加映像名称
		if (0 == StrCmp(strName.GetBuffer(),ProcessInfo.szExeFile))
		{
			flag = false;
			break;
		}
		//获取下一个进程的信息
		Status=Process32Next(SnapShot,&ProcessInfo);
	}
	CloseHandle(SnapShot); 
	if (true == flag)
	{
		USES_CONVERSION;
		std::string path = W2A(strPath);
		WinExec(path.c_str(),SW_SHOW);
	}
	return flag;
}

void Radar::CmdRadarDataProcess()
{
	DisposeWinExec(g_strDisposeDataName, g_strDisposeDataPath);
}

void Radar::CmdRadarDataFormsShow()
{
	DisposeWinExec(g_strShowDataName, g_strShowDataPath);
}

void Radar::CmdExportRadarProject(CString strPath,CString strPROJECTCREATETIME)
{
	RadarManager::Instance()->exportProject( strPath, strPROJECTCREATETIME,true);
}

CString Radar::CmdImportRadarProject(CString strFileAllPath)
{
	if (true == RadarManager::Instance()->importPorject(strFileAllPath,true))
	{
		CString strTemp;
		strTemp.Format(L"<返回信息 导入雷达工程='成功' 工程创建时间='%s'></返回信息>",RadarManager::Instance()->GetImportProCreateTime());
		return strTemp;
	} 
	else
	{
		return _T("<返回信息 导入雷达工程='失败'></返回信息>");
	}
	
}

bool Radar::CmdDeleteRadarProject(CString strPROJECTCREATETIME)
{
	return RadarManager::Instance()->deleteRadarProject(strPROJECTCREATETIME);
}

//显示视屏窗口
void Radar::CmdVideoPlay()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	g_pVideoPlayDlg->VideoShow();	//SW_NORMAL	
}

//显示回放
void Radar::CmdVideoPlayback()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	g_pVideoPlaybackDlg->VideoBackplay();	//SW_NORMAL	
}

//选择生成GPR
void Radar::SelectCreateGPR()
{

	RadarManager::Instance()->CheckRadarProject();	//SW_NORMAL	
}
