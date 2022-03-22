#include "StdAfx.h"
#include <TlHelp32.h>

#include "RadarCurve/CmdCenter/CmdCenter.h"
#include <RadarCurve/RadarManager.h>
#include <Kernel/KernelExport.h>
#include "GD_VideoShow.h"
#include "GD_VideoBack.h"

//����ļ�OCX��Ч
GD_VideoShow* g_pVideoPlayDlg = NULL;
GD_VideoBack* g_pVideoPlaybackDlg = NULL;

CString  g_strDisposeDataPath;
CString  g_strDisposeDataName;

CString  g_strShowDataPath;
CString  g_strShowDataName;

 void Radar::CmdRadarManagerInit()
 {
	 //��ʼ��RadarManager
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


	 //�ⲿexe�ĵ���
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

// 	��ʼ��GD_VideoShow��GD_VideoBack
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

	//��ʼ����ͼƬ
	g_pVideoPlayDlg->StartSaveImage(true);
}

void Radar::CmdEndRadarWorkDlg()
{
	//�����ʵ��
	if ( !RadarManager::Instance()->stopWork() )
	{
		return;
	}

	//ֹͣ����ͼƬ
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

//����Windows����
bool DisposeWinExec(CString strName,CString strPath)
{
	if (strName.IsEmpty() || strPath.IsEmpty())
	{
		return false;
	}
	HANDLE SnapShot=::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);
	if(SnapShot==NULL)
	{
		::MessageBox(NULL,L"��⵱ǰ����ʧ��!",NULL,0);
		return false;
	}

	SHFILEINFO shSmall;
	int nIndex;
	CString str,prcnum;
	PROCESSENTRY32 ProcessInfo;//����������Ϣ����
	ProcessInfo.dwSize=sizeof(ProcessInfo);//����ProcessInfo�Ĵ�С
	//����ϵͳ�е�һ�����̵���Ϣ
	BOOL Status=Process32First(SnapShot,&ProcessInfo);
	int m_nProcess=0,num=0;
	bool flag = true;
	while(Status)
	{
		m_nProcess++;
		num++;
		//��ȡ�����ļ���Ϣ
		SHGetFileInfo(ProcessInfo.szExeFile,0,&shSmall,
			sizeof(shSmall),SHGFI_ICON|SHGFI_SMALLICON);
		//���б�ؼ������ӳ������
		if (0 == StrCmp(strName.GetBuffer(),ProcessInfo.szExeFile))
		{
			flag = false;
			break;
		}
		//��ȡ��һ�����̵���Ϣ
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
		strTemp.Format(L"<������Ϣ �����״﹤��='�ɹ�' ���̴���ʱ��='%s'></������Ϣ>",RadarManager::Instance()->GetImportProCreateTime());
		return strTemp;
	} 
	else
	{
		return _T("<������Ϣ �����״﹤��='ʧ��'></������Ϣ>");
	}
	
}

bool Radar::CmdDeleteRadarProject(CString strPROJECTCREATETIME)
{
	return RadarManager::Instance()->deleteRadarProject(strPROJECTCREATETIME);
}

//��ʾ��������
void Radar::CmdVideoPlay()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	g_pVideoPlayDlg->VideoShow();	//SW_NORMAL	
}

//��ʾ�ط�
void Radar::CmdVideoPlayback()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	g_pVideoPlaybackDlg->VideoBackplay();	//SW_NORMAL	
}

//ѡ������GPR
void Radar::SelectCreateGPR()
{

	RadarManager::Instance()->CheckRadarProject();	//SW_NORMAL	
}
