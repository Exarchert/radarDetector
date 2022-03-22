// GD_VideoShow.cpp : 实现文件
//

#include "stdafx.h"
#include "TSW/LibXIStream/XIStream.h"
#include "TSW/LibXIProperty/XIProperty.h"
//#include "CartoonEditor.h"

#include <winbase.h>
#include <shlwapi.h>
#include <com/log/COM_Log.h>
#include <RadarCurve/RadarManager.h>
#include "GD_VideoShow.h"

//#define CAPTURE_WIDTH	720
//#define CAPTURE_HEIGHT	576

#define CAPTURE_WIDTH	120
#define CAPTURE_HEIGHT	96


#define FRAME_DURATION_60	166667
#define FRAME_DURATION_25	400000
#define FRAME_DURATION_15	666667
#define FRAME_DURATION_1	1000000

#define AUDIO_SAMPLESPERSEC 48000	//44100

#define CAPTURE_COLORSPACE		XI_COLOR_YUYV // XI_COLOR_I420 // XI_COLOR_I420 // XI_COLOR_YUYV // XI_COLOR_NV12 //XI_COLOR_I420 //  // XI_COLOR_YUYV
#define CAPTURE_COMPRESSION		XI_COMPRESSION_MIDDLE

#define TIMER_ID		1
#define TIMER_CHECK		2


#define BACK_PLAY_VIDEO     WM_USER + 1001




DWORD GetImageSizeByStride(XI_COLOR_FORMAT color, int cbStride, int cy)
{
	switch (color) {
	case XI_COLOR_I420:
	case XI_COLOR_NV12:
		return (cbStride * cy * 3 / 2);

	case XI_COLOR_YUYV:
	case XI_COLOR_YVYU:
	case XI_COLOR_UYVY:
	case XI_COLOR_VYUY:
		return (cbStride * cy * 2);

	case XI_COLOR_RGB24:
		return (cbStride * cy * 3);

	case XI_COLOR_RGB32:
		return (cbStride * cy * 4);

	default:
		return 0;
	}
	return 0;
}

BOOL IsTopDown(XI_COLOR_FORMAT color)
{
	if (color == XI_COLOR_RGB24 || color == XI_COLOR_RGB32)
		return FALSE;
	return TRUE;
}


// GD_VideoShow 对话框

IMPLEMENT_DYNAMIC(GD_VideoShow, CDialog)

GD_VideoShow::GD_VideoShow(CWnd* pParent /*=NULL*/)
	: CDialog(GD_VideoShow::IDD, pParent)
	, m_iEdit(1)
	, m_iEdit2(2)
	, m_iEdit3(3)
	, m_iEdit4(4)
{
	XIS_Initialize();
	int cVideoCaptures = XIS_GetVideoCaptureCount();
	if (cVideoCaptures <= 0) 
	{
		COM::WRITE_TO_LOG_PTR->WriteToLog(COM::LEVEL_WARNING,"视屏","没有找到设备！");	
	}
	m_bInitWndShow = false;
	HANDLE _handle = NULL;
	m_mapHVideoDevice = new map<int,HANDLE>;
	m_mapHVideoDevice->insert(map<int,HANDLE>::value_type(1,_handle)); 
	m_mapHVideoDevice->insert(map<int,HANDLE>::value_type(2,_handle)); 
	m_mapHVideoDevice->insert(map<int,HANDLE>::value_type(3,_handle)); 
	m_mapHVideoDevice->insert(map<int,HANDLE>::value_type(4,_handle)); 

	m_mapHProperty = new map<int,HANDLE>;
	m_mapHProperty->insert(map<int,HANDLE>::value_type(1,_handle)); 
	m_mapHProperty->insert(map<int,HANDLE>::value_type(2,_handle)); 
	m_mapHProperty->insert(map<int,HANDLE>::value_type(3,_handle)); 
	m_mapHProperty->insert(map<int,HANDLE>::value_type(4,_handle)); 
	
	m_mapHRenderer = new map<int,HANDLE>;
	m_mapHRenderer->insert(map<int,HANDLE>::value_type(1,_handle)); 
	m_mapHRenderer->insert(map<int,HANDLE>::value_type(2,_handle)); 
	m_mapHRenderer->insert(map<int,HANDLE>::value_type(3,_handle)); 
	m_mapHRenderer->insert(map<int,HANDLE>::value_type(4,_handle)); 
	m_mapIVideoDeviceSel = new map<int,int>;
	m_mapIVideoDeviceSel->insert(map<int,int>::value_type(1,-2)); 
	m_mapIVideoDeviceSel->insert(map<int,int>::value_type(2,-2)); 
	m_mapIVideoDeviceSel->insert(map<int,int>::value_type(3,-2)); 
	m_mapIVideoDeviceSel->insert(map<int,int>::value_type(4,-2)); 
	
	m_mapGuidMonitor = new map<int,GUID> ;
	GUID guidMonitor = GUID_NULL;
	m_mapGuidMonitor->insert(map<int,GUID>::value_type(1,guidMonitor)); 
	m_mapGuidMonitor->insert(map<int,GUID>::value_type(2,guidMonitor)); 
	m_mapGuidMonitor->insert(map<int,GUID>::value_type(3,guidMonitor)); 
	m_mapGuidMonitor->insert(map<int,GUID>::value_type(4,guidMonitor)); 

	m_mapHMP4Muxer = new map<int,HANDLE>;
	m_mapHMP4Muxer->insert(map<int,HANDLE>::value_type(1,_handle)); 
	m_mapHMP4Muxer->insert(map<int,HANDLE>::value_type(2,_handle)); 
	m_mapHMP4Muxer->insert(map<int,HANDLE>::value_type(3,_handle)); 
	m_mapHMP4Muxer->insert(map<int,HANDLE>::value_type(4,_handle)); 


	m_mapHMP4MuxerA = new map<int,HANDLE>;
	m_mapHMP4MuxerA->insert(map<int,HANDLE>::value_type(1,_handle)); 
	m_mapHMP4MuxerA->insert(map<int,HANDLE>::value_type(2,_handle)); 
	m_mapHMP4MuxerA->insert(map<int,HANDLE>::value_type(3,_handle)); 
	m_mapHMP4MuxerA->insert(map<int,HANDLE>::value_type(4,_handle)); 

	m_mapHAudioDevice = new map<int,HANDLE>;
	m_mapHAudioDevice->insert(map<int,HANDLE>::value_type(1,_handle)); 
	m_mapHAudioDevice->insert(map<int,HANDLE>::value_type(2,_handle)); 
	m_mapHAudioDevice->insert(map<int,HANDLE>::value_type(3,_handle)); 
	m_mapHAudioDevice->insert(map<int,HANDLE>::value_type(4,_handle)); 


	m_mapHAVIMuxer = new map<int,HANDLE>;
	m_mapHAVIMuxer->insert(map<int,HANDLE>::value_type(1,_handle)); 
	m_mapHAVIMuxer->insert(map<int,HANDLE>::value_type(2,_handle)); 
	m_mapHAVIMuxer->insert(map<int,HANDLE>::value_type(3,_handle)); 
	m_mapHAVIMuxer->insert(map<int,HANDLE>::value_type(4,_handle)); 


	m_mapBSaveImageFirst = new map<int,bool>;
	m_mapBSaveImageFirst->insert(map<int,bool>::value_type(1,false)); 
	m_mapBSaveImageFirst->insert(map<int,bool>::value_type(2,false)); 
	m_mapBSaveImageFirst->insert(map<int,bool>::value_type(3,false)); 
	m_mapBSaveImageFirst->insert(map<int,bool>::value_type(4,false)); 


	m_mapBBigenSaveImage = new map<int,bool>;
	m_mapBBigenSaveImage->insert(map<int,bool>::value_type(1,false)); 
	m_mapBBigenSaveImage->insert(map<int,bool>::value_type(2,false)); 
	m_mapBBigenSaveImage->insert(map<int,bool>::value_type(3,false)); 
	m_mapBBigenSaveImage->insert(map<int,bool>::value_type(4,false)); 


	m_mapDConutTime = new map<int,WORD>;
	m_mapDConutTime->insert(map<int,WORD>::value_type(1,0)); 
	m_mapDConutTime->insert(map<int,WORD>::value_type(2,0)); 
	m_mapDConutTime->insert(map<int,WORD>::value_type(3,0)); 
	m_mapDConutTime->insert(map<int,WORD>::value_type(4,0)); 



	m_mapBOpenDevice = new map<int,bool>;
	m_mapBOpenDevice->insert(map<int,bool>::value_type(1,false)); 
	m_mapBOpenDevice->insert(map<int,bool>::value_type(2,false)); 
	m_mapBOpenDevice->insert(map<int,bool>::value_type(3,false)); 
	m_mapBOpenDevice->insert(map<int,bool>::value_type(4,false)); 
	

	m_mapWStaticTite = new map<int,CStatic*>;
	m_mapWStaticTite->insert(map<int,CStatic*>::value_type(1,&m_sTite)); 
	m_mapWStaticTite->insert(map<int,CStatic*>::value_type(2,&m_sTite2)); 
	m_mapWStaticTite->insert(map<int,CStatic*>::value_type(3,&m_sTite3)); 
	m_mapWStaticTite->insert(map<int,CStatic*>::value_type(4,&m_sTite4)); 

	m_mapStaticWindow = new map<int,CStatic*>;
	m_mapStaticWindow->insert(map<int,CStatic*>::value_type(1,&m_sWindow)); 
	m_mapStaticWindow->insert(map<int,CStatic*>::value_type(2,&m_sWindow2)); 
	m_mapStaticWindow->insert(map<int,CStatic*>::value_type(3,&m_sWindow3)); 
	m_mapStaticWindow->insert(map<int,CStatic*>::value_type(4,&m_sWindow4)); 

	m_mapStaticLineRoud = new map<int,CStatic*>;
	m_mapStaticLineRoud->insert(map<int,CStatic*>::value_type(1,&m_sLine)); 
	m_mapStaticLineRoud->insert(map<int,CStatic*>::value_type(2,&m_sLine2)); 
	m_mapStaticLineRoud->insert(map<int,CStatic*>::value_type(3,&m_sLine3)); 
	m_mapStaticLineRoud->insert(map<int,CStatic*>::value_type(4,&m_sLine4)); 

	m_mapBtnOK = new map<int,CButton*>;
	m_mapBtnOK->insert(map<int,CButton*>::value_type(1,&m_btnPlay)); 
	m_mapBtnOK->insert(map<int,CButton*>::value_type(2,&m_btnPlay2)); 
	m_mapBtnOK->insert(map<int,CButton*>::value_type(3,&m_btnPlay3)); 
	m_mapBtnOK->insert(map<int,CButton*>::value_type(4,&m_btnPlay4)); 

	m_mapEditIn = new map<int,CEdit*>;
	m_mapEditIn->insert(map<int,CEdit*>::value_type(1,&m_eInLine)); 
	m_mapEditIn->insert(map<int,CEdit*>::value_type(2,&m_eInLine2)); 
	m_mapEditIn->insert(map<int,CEdit*>::value_type(3,&m_eInLine3)); 
	m_mapEditIn->insert(map<int,CEdit*>::value_type(4,&m_eInLine4)); 

	m_mapEditValue = new map<int,int*>;
	m_mapEditValue->insert(map<int,int*>::value_type(1,&m_iEdit)); 
	m_mapEditValue->insert(map<int,int*>::value_type(2,&m_iEdit2)); 
	m_mapEditValue->insert(map<int,int*>::value_type(3,&m_iEdit3)); 
	m_mapEditValue->insert(map<int,int*>::value_type(4,&m_iEdit4)); 

	
		
	m_cx = CAPTURE_WIDTH;
	m_cy = CAPTURE_HEIGHT;
	m_nFrameDuration = FRAME_DURATION_25;

	OSVERSIONINFO osvi;
	ZeroMemory(&osvi, sizeof(OSVERSIONINFO));
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	GetVersionEx(&osvi);
	m_dwMajorVersion = osvi.dwMajorVersion;
	m_bOverlay = (m_dwMajorVersion <= 5);
	m_bVideoPreview = TRUE;
	m_nVideoDeviceCount = -1;
	m_sType = GD_None;
	m_iCountTimeOne = 0;

	
	TCHAR szAppPath[MAX_PATH];
	::GetModuleFileName(AfxGetInstanceHandle(), szAppPath, MAX_PATH);
	::PathRemoveFileSpec(szAppPath);
	CString strIni = szAppPath;
	strIni += L"\\ProjectData\\sysconfig.ini";
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

	ZeroMemory(szKeyVal,256*sizeof(TCHAR));
	::GetPrivateProfileString(_T("VIDEOSOURCE"),_T("SHOWWINDOWUNM"),_T(""), szKeyVal, MAX_PATH, strIni);
	m_iShowWindowNum = _ttoi(szKeyVal);
	if (m_iShowWindowNum != 2 && m_iShowWindowNum != 4)
	{
		m_iShowWindowNum = SHOW_WINDW_SUM_2;
	}


}

GD_VideoShow::~GD_VideoShow()
{

	if (true == (*m_mapBOpenDevice)[1])
	{
		//
	}
	if (true == (*m_mapBOpenDevice)[2])
	{
		//
	}
}

void GD_VideoShow::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_PART, m_iEdit);
	DDX_Text(pDX, IDC_EDIT_PART2, m_iEdit2);
	DDX_Text(pDX, IDC_EDIT_PART3, m_iEdit3);
	DDX_Text(pDX, IDC_EDIT_PART4, m_iEdit4);

	DDX_Control(pDX, IDC_STATIC_VIDEO_1, m_sWindow);
	DDX_Control(pDX, IDC_STATIC_VIDEO_2, m_sWindow2);
	DDX_Control(pDX, IDC_STATIC_VIDEO_3, m_sWindow3);
	DDX_Control(pDX, IDC_STATIC_VIDEO_4, m_sWindow4);
	
	DDX_Control(pDX, IDC_BUTTON_PLAYLEFT, m_btnPlay);
	DDX_Control(pDX, IDC_BUTTON_PLAYLEFT2, m_btnPlay2);
	DDX_Control(pDX, IDC_BUTTON_PLAYLEFT3, m_btnPlay3);
	DDX_Control(pDX, IDC_BUTTON_PLAYLEFT4, m_btnPlay4);

	DDX_Control(pDX, IDC_STATIC_PLAY_TOPL1, m_sTite);
	DDX_Control(pDX, IDC_STATIC_PLAY_TOPL2, m_sTite2);
	DDX_Control(pDX, IDC_STATIC_PLAY_TOPL3, m_sTite3);
	DDX_Control(pDX, IDC_STATIC_PLAY_TOPL4, m_sTite4);

	DDX_Control(pDX, IDC_STATIC_LINE1, m_sLine);
	DDX_Control(pDX, IDC_STATIC_LINE2, m_sLine2);
	DDX_Control(pDX, IDC_STATIC_LINE3, m_sLine3);
	DDX_Control(pDX, IDC_STATIC_LINE4, m_sLine4);

	DDX_Control(pDX, IDC_EDIT_PART, m_eInLine);
	DDX_Control(pDX, IDC_EDIT_PART2, m_eInLine2);
	DDX_Control(pDX, IDC_EDIT_PART3, m_eInLine3);
	DDX_Control(pDX, IDC_EDIT_PART4, m_eInLine4);
}


BEGIN_MESSAGE_MAP(GD_VideoShow, CDialog)
	ON_WM_CREATE()
	ON_WM_TIMER()
	ON_WM_PAINT()
	ON_WM_DESTROY()
	ON_WM_MOVE()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON_PLAYLEFT, &GD_VideoShow::OnBnClickedButtonPlayleft)
	ON_BN_CLICKED(IDC_BUTTON_PLAYLEFT2, &GD_VideoShow::OnBnClickedButtonPlayleft2)
	ON_BN_CLICKED(IDC_BUTTON_PLAYLEFT3, &GD_VideoShow::OnBnClickedButtonPlayleft3)
	ON_BN_CLICKED(IDC_BUTTON_PLAYLEFT4, &GD_VideoShow::OnBnClickedButtonPlayleft4)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// GD_VideoShow 消息处理程序

void GD_VideoShow::SetShowWindowSum(int sum)
{
	m_iShowWindowNum = sum;
}
int GD_VideoShow::GetShowWindowSum()
{
	return m_iShowWindowNum;
}

int GD_VideoShow::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	// TODO:  在此添加您专用的创建代码
	m_nVideoDeviceCount = XIS_GetVideoCaptureCount();
	BOOL _bSupport = XIS_IsSupportH264HD();
	SetTimer(TIMER_ID, 100, NULL);
	SetTimer(TIMER_CHECK, 5000, NULL);
	PostMessage(WM_SIZE,0,0);
	return 0;
}

BOOL GD_VideoShow::OpenDevice(int nSel,int iWindow)
{
	if ((*m_mapIVideoDeviceSel)[iWindow] == nSel)
		return TRUE;

	if ((*m_mapIVideoDeviceSel)[iWindow] != -1) {
		if ((*m_mapHProperty)[iWindow]) {
			XIP_ClosePropertyHandle((*m_mapHProperty)[iWindow]);
			(*m_mapHProperty)[iWindow] = NULL;
		}

		if ((*m_mapHVideoDevice)[iWindow]) {
			XIS_StopVideoCapture((*m_mapHVideoDevice)[iWindow]);
			XIS_CloseVideoCapture((*m_mapHVideoDevice)[iWindow]);
			(*m_mapHVideoDevice)[iWindow] = NULL;
		}

		if ((*m_mapHRenderer)[iWindow]) {
			XIS_DestroyVideoRenderer((*m_mapHRenderer)[iWindow]);
			(*m_mapHRenderer)[iWindow]= NULL;
		}
		(*m_mapIVideoDeviceSel)[iWindow] = -1;
	}

	VIDEO_CAPTURE_INFO vidCapInfo;
	if (!XIS_GetVideoCaptureInfo(nSel, &vidCapInfo))
		return FALSE;

	(*m_mapHVideoDevice)[iWindow] = XIS_OpenVideoCapture(vidCapInfo.szDShowID);
	if (NULL == (*m_mapHVideoDevice)[iWindow])
		return FALSE;

	XIS_SetVideoCaptureBuffering((*m_mapHVideoDevice)[iWindow], FALSE);

	(*m_mapHProperty)[iWindow] = XIS_OpenVideoCapturePropertyHandle((*m_mapHVideoDevice)[iWindow]);
	if (NULL != (*m_mapHProperty)[iWindow]) {
		CHAR szSerialNo[MAX_SERIAL_NO] = {0};
		XIP_GetSerialNo((*m_mapHProperty)[iWindow], szSerialNo);
	}

	GUID guidMonitor = (*m_mapGuidMonitor)[iWindow];
	if (!XIS_GetMonitorGUIDFromWindow(m_sWindow2.GetSafeHwnd(), &guidMonitor))
		guidMonitor = GUID_NULL;

	(*m_mapHRenderer)[iWindow] = XIS_CreateVideoRenderer(
		&guidMonitor, 
		(*m_mapStaticWindow)[iWindow]->GetSafeHwnd(),
		CAPTURE_COLORSPACE, 
		m_cx, 
		m_cy, 
		m_dwMajorVersion <= 5, 
		TRUE
		);
	(*m_mapGuidMonitor)[iWindow] = guidMonitor;
	if (NULL == (*m_mapHRenderer)[iWindow])
		return FALSE;

	(*m_mapIVideoDeviceSel)[iWindow] = nSel;
	ResizeWindow(iWindow);
	return TRUE;
}

void GD_VideoShow::ResizeWindow(int iWindow)
{
	CRect rcWnd;
	if ((*m_mapHProperty)[1] != NULL) {
		XI_DEVICE_TYPE devType = XIP_GetDeviceType((*m_mapHProperty)[iWindow]);
		int nLeft, nTop, nWidth = 0, nHeight = 0;

		if (XI_DEVICE_HDVIDEO_CAPTURE == devType) {
			XIPHD_GetDefClip((*m_mapHProperty)[iWindow], &nLeft, &nTop, &nWidth, &nHeight);
		}
		else {
			XIPCVBS_GetClip((*m_mapHProperty)[iWindow], &nLeft, &nTop, &nWidth, &nHeight);
		}
		nWidth = max(CAPTURE_WIDTH, min(2048, nWidth));
		nHeight = max(CAPTURE_HEIGHT, min(1536, nHeight));
		m_cx = nWidth;
		m_cy = nHeight;
	}
	CRect wndRect;
	(*m_mapStaticWindow)[iWindow]->GetClientRect(wndRect);
	rcWnd.SetRect(0, 0, m_cx, m_cy);
	AdjustWindowRectEx(&rcWnd, WS_OVERLAPPEDWINDOW, TRUE, WS_EX_CLIENTEDGE);
	XIS_StopVideoCapture((*m_mapHVideoDevice)[iWindow]);
	XIS_SetVideoCaptureFormat((*m_mapHVideoDevice)[iWindow], CAPTURE_COLORSPACE, m_cx, m_cy, m_nFrameDuration);

	if (I_WINDOW_1 == iWindow)
	{
		XIS_SetVideoCaptureCallbackEx((*m_mapHVideoDevice)[iWindow], VideoCaptureCallback, this, FALSE);
	}
	if (I_WINDOW_2 == iWindow)
	{
		XIS_SetVideoCaptureCallbackEx((*m_mapHVideoDevice)[iWindow], VideoCaptureCallback2, this, FALSE);
	}
	if (I_WINDOW_3 == iWindow)
	{
		XIS_SetVideoCaptureCallbackEx((*m_mapHVideoDevice)[iWindow], VideoCaptureCallback3, this, FALSE);
	}
	if (I_WINDOW_4 == iWindow)
	{
		XIS_SetVideoCaptureCallbackEx((*m_mapHVideoDevice)[iWindow], VideoCaptureCallback4, this, FALSE);
	}
	

	GUID guidMonitor = (*m_mapGuidMonitor)[iWindow];
	if ((*m_mapHRenderer)[iWindow]) {
		XIS_ResetVideoRenderer((*m_mapHRenderer)[iWindow], &guidMonitor, (*m_mapStaticWindow)[iWindow]->GetSafeHwnd(), CAPTURE_COLORSPACE, m_cx, m_cy, m_bOverlay, TRUE);
		UpdateRenderer(iWindow);
	}

	(*m_mapGuidMonitor)[iWindow] = guidMonitor;
	XIS_StartVideoCapture((*m_mapHVideoDevice)[iWindow]);
}

void GD_VideoShow::VideoCaptureCallbackInternal(const BYTE * pbyImage, int cbImageStride, UINT64 u64TimeStamp, int iWindow)
{
	if (XIPHD_IsSignalPresent((*m_mapHProperty)[iWindow]) == S_OK)
	{
		if ((*m_mapHRenderer)[iWindow] && m_bVideoPreview) {
			XIS_VideoRendererDrawImage((*m_mapHRenderer)[iWindow], pbyImage, cbImageStride, IsTopDown(CAPTURE_COLORSPACE));
		}
	}
}

void GD_VideoShow::UpdateRenderer(int iWindow)
{
	GUID guidMonitor = (*m_mapGuidMonitor)[iWindow];
	if ((*m_mapHRenderer)[iWindow]) {
		GUID guidNew;
		if (XIS_GetMonitorGUIDFromWindow((*m_mapStaticWindow)[iWindow]->GetSafeHwnd(), &guidNew) 
			&& guidNew != guidMonitor) {
				guidMonitor = guidNew;
				XIS_ResetVideoRenderer(
					(*m_mapHRenderer)[iWindow], 
					&guidMonitor, 
					(*m_mapStaticWindow)[iWindow]->GetSafeHwnd(), 
					CAPTURE_COLORSPACE, 
					m_cx, 
					m_cy, 
					m_bOverlay, 
					TRUE
					);
		}

		CRect rcDest;
		(*m_mapStaticWindow)[iWindow]->GetClientRect(&rcDest);
		XIS_SetVideoRendererPosition((*m_mapHRenderer)[iWindow], NULL, rcDest);
	}
	(*m_mapGuidMonitor)[iWindow] = guidMonitor;
}

void GD_VideoShow::CalculateTime(int iWindow)
{
	SYSTEMTIME lpTime;

	GetLocalTime(&lpTime);

	if (true == (*m_mapBSaveImageFirst)[iWindow] )
	{
		(*m_mapBSaveImageFirst)[iWindow] = false;
		(*m_mapDConutTime)[iWindow] = lpTime.wSecond;
	}
	else
	{
		if ((*m_mapDConutTime)[iWindow] >  lpTime.wSecond)
		{
			(*m_mapDConutTime)[iWindow] -= 60;
		}
		if (((*m_mapDConutTime)[iWindow] + m_iOnceTime) <= lpTime.wSecond )
		{
			(*m_mapDConutTime)[iWindow] = lpTime.wSecond;
		}
	} 
}

void GD_VideoShow::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (m_bInitWndShow == false)
	{
		m_bInitWndShow = true;
		ShowWindow(false);
	}

	if (nIDEvent == TIMER_ID && (*m_mapHProperty)[1] != NULL) {
		XI_DEVICE_TYPE devType = XIP_GetDeviceType((*m_mapHProperty)[1]);
		BOOL bChanged = FALSE;

		if (XI_DEVICE_HDVIDEO_CAPTURE == devType) {
			bChanged = (XIPHD_IsSignalChanged((*m_mapHProperty)[1]) == S_OK);
		}
		else {
			bChanged = (XIPCVBS_IsSignalChanged((*m_mapHProperty)[1]) == S_OK);
		}

		if (bChanged) {
			if ((*m_mapHMP4Muxer)[1] != NULL) {
				OnFileStop(I_WINDOW_1);
			}
			ResizeWindow(I_WINDOW_1);
		}
		SaveData(I_WINDOW_1);
	}

	if (nIDEvent == TIMER_ID && (*m_mapHProperty)[2] != NULL) {
		XI_DEVICE_TYPE devType = XIP_GetDeviceType((*m_mapHProperty)[2]);
		BOOL bChanged = FALSE;

		if (XI_DEVICE_HDVIDEO_CAPTURE == devType) {
			bChanged = (XIPHD_IsSignalChanged((*m_mapHProperty)[2]) == S_OK);
		}
		else {
			bChanged = (XIPCVBS_IsSignalChanged((*m_mapHProperty)[2]) == S_OK);
		}

		if (bChanged) {
			if ((*m_mapHMP4Muxer)[2] != NULL) {
				OnFileStop(I_WINDOW_2);
			}
			ResizeWindow(I_WINDOW_2);
		}
		SaveData(I_WINDOW_2);
	}


	if(SHOW_WINDW_SUM_4 == m_iShowWindowNum)
	{

		if (nIDEvent == TIMER_ID && (*m_mapHProperty)[3] != NULL) {
			XI_DEVICE_TYPE devType = XIP_GetDeviceType((*m_mapHProperty)[3]);
			BOOL bChanged = FALSE;

			if (XI_DEVICE_HDVIDEO_CAPTURE == devType) {
				bChanged = (XIPHD_IsSignalChanged((*m_mapHProperty)[3]) == S_OK);
			}
			else {
				bChanged = (XIPCVBS_IsSignalChanged((*m_mapHProperty)[3]) == S_OK);
			}

			if (bChanged) {
				if ((*m_mapHMP4Muxer)[3] != NULL) {
					OnFileStop(I_WINDOW_3);
				}
				ResizeWindow(I_WINDOW_3);
			}
			SaveData(I_WINDOW_3);
		}
		if (nIDEvent == TIMER_ID && (*m_mapHProperty)[4] != NULL) {
			XI_DEVICE_TYPE devType = XIP_GetDeviceType((*m_mapHProperty)[4]);
			BOOL bChanged = FALSE;

			if (XI_DEVICE_HDVIDEO_CAPTURE == devType) {
				bChanged = (XIPHD_IsSignalChanged((*m_mapHProperty)[4]) == S_OK);
			}
			else {
				bChanged = (XIPCVBS_IsSignalChanged((*m_mapHProperty)[4]) == S_OK);
			}

			if (bChanged) {
				if ((*m_mapHMP4Muxer)[4] != NULL) {
					OnFileStop(I_WINDOW_4);
				}
				ResizeWindow(I_WINDOW_4);
			}
			SaveData(I_WINDOW_4);
		}
	}
	CDialog::OnTimer(nIDEvent);
}

void GD_VideoShow::OnFileStop(int iWindow)
{
	if ((*m_mapHMP4Muxer)[iWindow] != NULL) {
		XIS_StopMP4Muxer((*m_mapHMP4Muxer)[iWindow]);
		XIS_DestroyMP4Muxer((*m_mapHMP4Muxer)[iWindow]);
		(*m_mapHMP4Muxer)[iWindow] = NULL;
	}
	if ((*m_mapHMP4MuxerA)[iWindow] != NULL) {
		XIS_StopMP4Muxer((*m_mapHMP4MuxerA)[iWindow]);
		XIS_DestroyMP4Muxer((*m_mapHMP4MuxerA)[iWindow]);
		(*m_mapHMP4MuxerA)[iWindow] = NULL;
	}

	if ((*m_mapHAudioDevice)[iWindow] != NULL) {
		XIS_StopAVIMuxer((*m_mapHAudioDevice)[iWindow]);
		(*m_mapHAudioDevice)[iWindow] = NULL;
	}
	if ((*m_mapHVideoDevice)[iWindow] != NULL) {
		XIS_CloseVideoH264Encoder((*m_mapHVideoDevice)[iWindow], 0);
		XIS_CloseVideoH264Encoder((*m_mapHVideoDevice)[iWindow], 1);
	}

	if ((*m_mapHAudioDevice)[iWindow] != NULL) {
		XIS_SetAudioWaveFileOutput((*m_mapHAudioDevice)[iWindow], NULL);
		XIS_CloseAudioAACEncoder((*m_mapHAudioDevice)[iWindow]);
	}
}

void GD_VideoShow::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码

	if ((*m_mapHRenderer)[1]) {
		XIS_VideoRendererRepaintRect((*m_mapHRenderer)[1], NULL);
	}
	if ((*m_mapHRenderer)[2]) {
		XIS_VideoRendererRepaintRect((*m_mapHRenderer)[2], NULL);
	}

	if(SHOW_WINDW_SUM_4 == m_iShowWindowNum)
	{
		if ((*m_mapHRenderer)[3]) {
			XIS_VideoRendererRepaintRect((*m_mapHRenderer)[3], NULL);
		}
		if ((*m_mapHRenderer)[4]) {
			XIS_VideoRendererRepaintRect((*m_mapHRenderer)[4], NULL);
		}		
	}
}

void GD_VideoShow::OnDestroy()
{
	CDialog::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
	KillTimer(TIMER_ID);
	KillTimer(TIMER_CHECK);

	for (int i = 1; i <= SHOW_WINDW_SUM_MAX ; ++i)
	{
		if (i > m_iShowWindowNum)
			break;

		if ((*m_mapHMP4Muxer)[i]) {
			OnFileStop(i);
			}
		if ((*m_mapHProperty)[i]) {
			XIP_ClosePropertyHandle((*m_mapHProperty)[i]);
			(*m_mapHProperty)[i] = NULL;
		}

		if ((*m_mapHAudioDevice)[i] != NULL) {
			XIS_StopAudioCapture((*m_mapHAudioDevice)[i]);
			XIS_CloseAudioCapture((*m_mapHAudioDevice)[i]);
			(*m_mapHAudioDevice)[i] = NULL;
		}

		if ((*m_mapHVideoDevice)[i]) {
			XIS_StopVideoCapture((*m_mapHVideoDevice)[i]);
			XIS_CloseVideoCapture((*m_mapHVideoDevice)[i]);
			(*m_mapHVideoDevice)[i] = NULL;
		}

		if ((*m_mapHRenderer)[i]) {
			XIS_DestroyVideoRenderer((*m_mapHRenderer)[i]);
			(*m_mapHRenderer)[i] = NULL;
		}
		(*m_mapIVideoDeviceSel)[i] = -1;
	}
}

void GD_VideoShow::OnMove(int x, int y)
{
	CDialog::OnMove(x, y);

	// TODO: 在此处添加消息处理程序代码
	UpdateRenderer(I_WINDOW_1);
	UpdateRenderer(I_WINDOW_2);
	if (SHOW_WINDW_SUM_4 == m_iShowWindowNum)
	{
		UpdateRenderer(I_WINDOW_3);
		UpdateRenderer(I_WINDOW_4);
	}
}

void GD_VideoShow::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	if (0 == m_iCountTimeOne)
	{
		m_iCountTimeOne++;
		return;
	}
	if (SHOW_WINDW_SUM_2 == m_iShowWindowNum)
	{
		if (400 < cx  && 200 < cy)
		{
			CWnd* lWnd = NULL;
			int halfX = cx / 2;
			int halfY = cy / 2;
			int jiangeX = 10;
			int jiangeY = 20;
			int jiamMap = 3;

			int btnSpaceY = 50;
			int btnHeight = cy - 40;

			lWnd = (*m_mapStaticWindow)[1];
			lWnd->SetWindowPos( NULL,jiangeX,jiangeY,halfX - jiangeX - jiangeX ,cy - jiangeY - btnSpaceY,
				SWP_NOZORDER );
			lWnd->UpdateWindow();
			lWnd->UpdateData();
			
			lWnd = (*m_mapWStaticTite)[1];
			lWnd->SetWindowPos( NULL,jiamMap,jiamMap,halfX - jiamMap - jiamMap ,cy - jiamMap - jiamMap,
				SWP_NOZORDER );
			lWnd->UpdateWindow();
			
			lWnd = (*m_mapStaticLineRoud)[1];
			lWnd->SetWindowPos( NULL,halfX - 190,btnHeight + 5,0 ,0,
				SWP_NOZORDER | SWP_NOSIZE);
			lWnd->UpdateWindow();
			
			lWnd = (*m_mapEditIn)[1];
			lWnd->SetWindowPos( NULL,halfX - 160,btnHeight,0 ,0,
				SWP_NOZORDER  | SWP_NOSIZE);
			lWnd->UpdateWindow();
			
			lWnd = (*m_mapBtnOK)[1];
			lWnd->SetWindowPos( NULL,halfX - 100,btnHeight,0 ,0,
				SWP_NOZORDER  | SWP_NOSIZE);
			lWnd->UpdateWindow();

			lWnd = (*m_mapStaticWindow)[2];
			lWnd->SetWindowPos( NULL,jiangeX + halfX,jiangeY,halfX - jiangeX - jiangeX ,cy - jiangeY - btnSpaceY,
				SWP_NOZORDER );
			lWnd->UpdateWindow();
			lWnd->UpdateData();
			
			lWnd = (*m_mapWStaticTite)[2];
			lWnd->SetWindowPos( NULL,jiamMap + halfX,jiamMap,halfX - jiamMap - jiamMap ,cy - jiamMap - jiamMap,
				SWP_NOZORDER );
			lWnd->UpdateWindow();
			
			lWnd = (*m_mapStaticLineRoud)[2];
			lWnd->SetWindowPos( NULL,cx - 190,btnHeight + 5,0 ,0,
				SWP_NOZORDER  | SWP_NOSIZE);
			lWnd->UpdateWindow();
			
			lWnd = (*m_mapEditIn)[2];
			lWnd->SetWindowPos( NULL,cx - 160,btnHeight,0 ,0,
				SWP_NOZORDER  | SWP_NOSIZE);
			lWnd->UpdateWindow();
			
			lWnd = (*m_mapBtnOK)[2];
			lWnd->SetWindowPos( NULL,cx - 100,btnHeight,0 ,0,
				SWP_NOZORDER  | SWP_NOSIZE);
			lWnd->UpdateWindow();
			UpdateWindow();
		}
		UpdateRenderer(I_WINDOW_1);
		UpdateRenderer(I_WINDOW_2);
	}
	else if (SHOW_WINDW_SUM_4 == m_iShowWindowNum)
	{
		if (400 < cx  && 250 < cy)
		{
			CWnd* lWnd = NULL;
			int halfX = cx / 2;
			int halfY = cy / 2;
			int jiangeX = 10;
			int jiangeY = 20;
			int jiamMap = 3;

			int btnSpaceY = 50;
			int btnHeight = halfY - 40;

			
			lWnd = (*m_mapStaticWindow)[1];
			lWnd->SetWindowPos( NULL,jiangeX,jiangeY,halfX - jiangeX - jiangeX ,halfY - jiangeY - btnSpaceY,
				SWP_NOZORDER );
			lWnd->UpdateWindow();
			lWnd->UpdateData();
			
			lWnd = (*m_mapWStaticTite)[1];
			lWnd->SetWindowPos( NULL,jiamMap,jiamMap,halfX - jiamMap - jiamMap ,halfY - jiamMap - jiamMap,
				SWP_NOZORDER );
			lWnd->UpdateWindow();
			
			lWnd = (*m_mapStaticLineRoud)[1];
			lWnd->SetWindowPos( NULL,halfX - 190,btnHeight + 5,0 ,0,
				SWP_NOZORDER | SWP_NOSIZE);
			lWnd->UpdateWindow();
			
			lWnd = (*m_mapEditIn)[1];
			lWnd->SetWindowPos( NULL,halfX - 160,btnHeight,0 ,0,
				SWP_NOZORDER  | SWP_NOSIZE);
			lWnd->UpdateWindow();
			
			lWnd = (*m_mapBtnOK)[1];
			lWnd->SetWindowPos( NULL,halfX - 100,btnHeight,0 ,0,
				SWP_NOZORDER  | SWP_NOSIZE);
			lWnd->UpdateWindow();
			lWnd = (*m_mapStaticWindow)[2];
			lWnd->SetWindowPos( NULL,jiangeX + halfX,jiangeY,halfX - jiangeX - jiangeX ,halfY - jiangeY - btnSpaceY,
				SWP_NOZORDER );
			lWnd->UpdateWindow();
			lWnd->UpdateData();
			
			lWnd = (*m_mapWStaticTite)[2];
			lWnd->SetWindowPos( NULL,jiamMap + halfX,jiamMap,halfX - jiamMap - jiamMap ,halfY - jiamMap - jiamMap,
				SWP_NOZORDER );
			lWnd->UpdateWindow();
			
			lWnd = (*m_mapStaticLineRoud)[2];
			lWnd->SetWindowPos( NULL,cx - 190,btnHeight + 5,0 ,0,
				SWP_NOZORDER  | SWP_NOSIZE);
			lWnd->UpdateWindow();
			
			lWnd = (*m_mapEditIn)[2];
			lWnd->SetWindowPos( NULL,cx - 160,btnHeight,0 ,0,
				SWP_NOZORDER  | SWP_NOSIZE);
			lWnd->UpdateWindow();
			
			lWnd = (*m_mapBtnOK)[2];
			lWnd->SetWindowPos( NULL,cx - 100,btnHeight,0 ,0,
				SWP_NOZORDER  | SWP_NOSIZE);
			lWnd->UpdateWindow();

			
			lWnd = (*m_mapStaticWindow)[3];
			lWnd->SetWindowPos( NULL,jiangeX,jiangeY + halfY,halfX - jiangeX - jiangeX ,halfY - jiangeY - btnSpaceY,
				SWP_NOZORDER );
			lWnd->UpdateWindow();
			lWnd->UpdateData();
			
			lWnd = (*m_mapWStaticTite)[3];
			lWnd->SetWindowPos( NULL,jiamMap,jiamMap + halfY,halfX - jiamMap - jiamMap ,halfY - jiamMap - jiamMap,
				SWP_NOZORDER );
			lWnd->UpdateWindow();
			
			lWnd = (*m_mapStaticLineRoud)[3];
			lWnd->SetWindowPos( NULL,halfX - 190,btnHeight + halfY + 5,0 ,0,
				SWP_NOZORDER | SWP_NOSIZE);
			lWnd->UpdateWindow();
			
			lWnd = (*m_mapEditIn)[3];
			lWnd->SetWindowPos( NULL,halfX - 160,btnHeight + halfY,0 ,0,
				SWP_NOZORDER  | SWP_NOSIZE);
			lWnd->UpdateWindow();
			
			lWnd = (*m_mapBtnOK)[3];
			lWnd->SetWindowPos( NULL,halfX - 100,btnHeight + halfY,0 ,0,
				SWP_NOZORDER  | SWP_NOSIZE);
			lWnd->UpdateWindow();

			lWnd = (*m_mapStaticWindow)[4];
			lWnd->SetWindowPos( NULL,jiangeX + halfX,jiangeY + halfY,halfX - jiangeX - jiangeX ,halfY - jiangeY - btnSpaceY,
				SWP_NOZORDER );
			lWnd->UpdateWindow();
			lWnd->UpdateData();
			lWnd = (*m_mapWStaticTite)[4];
			lWnd->SetWindowPos( NULL,jiamMap + halfX,jiamMap + halfY,halfX - jiamMap - jiamMap ,halfY - jiamMap - jiamMap,
				SWP_NOZORDER );
			lWnd->UpdateWindow();
			lWnd = (*m_mapStaticLineRoud)[4];
			lWnd->SetWindowPos( NULL,cx - 190,btnHeight + halfY + 5,0 ,0,
				SWP_NOZORDER  | SWP_NOSIZE);
			lWnd->UpdateWindow();
			lWnd = (*m_mapEditIn)[4];
			lWnd->SetWindowPos( NULL,cx - 160,btnHeight + halfY,0 ,0,
				SWP_NOZORDER  | SWP_NOSIZE);
			lWnd->UpdateWindow();
			lWnd = (*m_mapBtnOK)[4];
			lWnd->SetWindowPos( NULL,cx - 100,btnHeight + halfY,0 ,0,
				SWP_NOZORDER  | SWP_NOSIZE);
			lWnd->UpdateWindow();
			UpdateWindow();
		}
		UpdateRenderer(I_WINDOW_1);
		UpdateRenderer(I_WINDOW_2);
		UpdateRenderer(I_WINDOW_3);
		UpdateRenderer(I_WINDOW_4);
	}
}

void GD_VideoShow::OnBnClickedButtonPlayleft()
{
	// TODO: 在此添加控件通知处理程序代码
	BnClickedButtonPlay(I_WINDOW_1);
}

void GD_VideoShow::OnBnClickedButtonPlayleft2()
{
	// TODO: 在此添加控件通知处理程序代码
	BnClickedButtonPlay(I_WINDOW_2);
}

void GD_VideoShow::OnBnClickedButtonPlayleft3()
{
	// TODO: 在此添加控件通知处理程序代码
	BnClickedButtonPlay(I_WINDOW_3);
}

void GD_VideoShow::OnBnClickedButtonPlayleft4()
{
	// TODO: 在此添加控件通知处理程序代码
	BnClickedButtonPlay(I_WINDOW_4);
}

void GD_VideoShow::BnClickedButtonPlay(int iWindow)
{
	UpdateData();
	if ((*(*m_mapEditValue)[iWindow]) < 1 || (*(*m_mapEditValue)[iWindow]) > 6)
	{
		AfxMessageBox(_T("线路为1-6 !"));
		return;
	}
	OpenDevice((*(*m_mapEditValue)[iWindow]) - 1 ,iWindow); 
}
void GD_VideoShow::SetPicPath( CString picPath )
{
	m_cPatrFile = picPath;
}
CString GD_VideoShow::GetPicPath()
{
	return m_cPatrFile;
}

void GD_VideoShow::SetProPath( CString proPath )
{
	m_proPath = proPath;
}

void GD_VideoShow::SaveData(int iWindow)
{
	if (!(*m_mapBBigenSaveImage)[iWindow])
	{
		return;
	}

	//if (iWindow > SHOW_WINDW_SUM_2)
	//{
	//	return;
	//}

	CalculateTime(iWindow);

	USES_CONVERSION;
	CString strFileName = m_proPath;//A2W(strProPath.c_str());
	CString strName;
	if(I_WINDOW_1 == iWindow)
	{
		strName.Format(_T("%s\\A_%s.png"),strFileName,GetSysTimeYMDHMS());
	}
	else 
	{
		if(I_WINDOW_2 == iWindow)
		{
			strName.Format(_T("%s\\B_%s.png"),strFileName,GetSysTimeYMDHMS());
		}
		else 
		{
			if(I_WINDOW_3 == iWindow)
			{
				strName.Format(_T("%s\\C_%s.png"),strFileName,GetSysTimeYMDHMS());
			}
			else 
			{
				if(I_WINDOW_4 == iWindow)
				{
					strName.Format(_T("%s\\D_%s.png"),strFileName,GetSysTimeYMDHMS());
				}
			}
		}
	}
	CString strPath = m_cPatrFile;
	//strPath += L"\\";
	strPath += strName;

	//////////////////////////////////////////////////////////////////////////0617
	
	XIS_VideoCaptureCreateSnapShot((*m_mapHVideoDevice)[iWindow], strPath, 75);

	std::string strPng = W2A(strName);
	RadarManager::Instance()->savePicPath( strPng,iWindow);
}

CString GD_VideoShow::GetSysTimeYMDHMS()
{
	SYSTEMTIME lpTime;
	//GetSystemTime(&lpTime);
	GetLocalTime(&lpTime);

	CString strTime;
	CString strYear,strMonth,strDay,strHour,strMinute,strSecond;

	strYear.Format(_T("%d"),lpTime.wYear);
	if (lpTime.wMonth < 10 )
	{
		strMonth.Format(_T("0%d"),lpTime.wMonth);
	}
	else
	{
		strMonth.Format(_T("%d"),lpTime.wMonth);
	}
	if (lpTime.wDay < 10 )
	{
		strDay.Format(_T("0%d"),lpTime.wDay);
	}
	else
	{
		strDay.Format(_T("%d"),lpTime.wDay);
	}
	if (lpTime.wHour < 10 )
	{
		strHour.Format(_T("0%d"),lpTime.wHour);
	}
	else
	{
		strHour.Format(_T("%d"),lpTime.wHour);
	}
	if (lpTime.wMinute < 10 )
	{
		strMinute.Format(_T("0%d"),lpTime.wMinute);
	}
	else
	{
		strMinute.Format(_T("%d"),lpTime.wMinute);
	}
	if (lpTime.wSecond < 10 )
	{
		strSecond.Format(_T("0%d"),lpTime.wSecond);
	}
	else
	{
		strSecond.Format(_T("%d"),lpTime.wSecond);
	}

	strTime += strYear;
	strTime += strMonth;
	strTime += strDay;
	strTime += strHour;
	strTime += strMinute;
	strTime += strSecond;

	return strTime;
}

void GD_VideoShow::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CDialog::OnClose();
}

//播放
void GD_VideoShow::VideoShow()
{


	CRect wndRect;
	this->GetClientRect(wndRect);
	OnSize(0,wndRect.Width(),wndRect.Height());
	ShowWindow(SW_SHOW);

	if (SHOW_WINDW_SUM_2 == m_iShowWindowNum)
	{
		GetDlgItem(IDC_STATIC_PLAY_TOPL3)->ShowWindow(false);
		GetDlgItem(IDC_STATIC_PLAY_TOPL4)->ShowWindow(false);

		GetDlgItem(IDC_STATIC_VIDEO_3)->ShowWindow(false);
		GetDlgItem(IDC_STATIC_VIDEO_4)->ShowWindow(false);

		GetDlgItem(IDC_STATIC_LINE3)->ShowWindow(false);
		GetDlgItem(IDC_STATIC_LINE4)->ShowWindow(false);

		GetDlgItem(IDC_EDIT_PART3)->ShowWindow(false);
		GetDlgItem(IDC_EDIT_PART4)->ShowWindow(false);

		GetDlgItem(IDC_BUTTON_PLAYLEFT3)->ShowWindow(false);
		GetDlgItem(IDC_BUTTON_PLAYLEFT4)->ShowWindow(false);
	}

	if (false == (*m_mapBOpenDevice)[1])
	{
		(*m_mapBOpenDevice)[1] = true;
		OpenDevice((*(*m_mapEditValue)[1]) - 1,I_WINDOW_1);
	}
	if (false == (*m_mapBOpenDevice)[2])
	{
		(*m_mapBOpenDevice)[2] = true;
		OpenDevice((*(*m_mapEditValue)[2]) - 1,I_WINDOW_2);
	}

	if(SHOW_WINDW_SUM_4 == m_iShowWindowNum)
	{
		if (false == (*m_mapBOpenDevice)[3])
		{
			(*m_mapBOpenDevice)[3] = true;
			OpenDevice((*(*m_mapEditValue)[3]) - 1,I_WINDOW_3);
		}
		if (false == (*m_mapBOpenDevice)[4])
		{
			(*m_mapBOpenDevice)[4] = true;
			OpenDevice((*(*m_mapEditValue)[4]) - 1,I_WINDOW_4);
		}
	}
	this->SendMessage(WM_SIZE,0,0);
	UpdateWindow();
	UpdateData();
}

void GD_VideoShow::StartSaveImage( bool bSaveImage )
{
	if (false == (*m_mapBOpenDevice)[1])
	{
		(*m_mapBOpenDevice)[1] = true;
		OpenDevice((*(*m_mapEditValue)[1]) - 1,I_WINDOW_1);
	}
	if (false == (*m_mapBOpenDevice)[2])
	{
		(*m_mapBOpenDevice)[2] = true;
		OpenDevice((*(*m_mapEditValue)[2]) - 1,I_WINDOW_2);
	}

	if (SHOW_WINDW_SUM_4 == m_iShowWindowNum)
	{
		if (false == (*m_mapBOpenDevice)[3])
		{
			(*m_mapBOpenDevice)[3] = true;
			OpenDevice((*(*m_mapEditValue)[3]) - 1,I_WINDOW_3);
		}
		if (false == (*m_mapBOpenDevice)[4])
		{
			(*m_mapBOpenDevice)[4] = true;
			OpenDevice((*(*m_mapEditValue)[4]) - 1,I_WINDOW_4);
		}
		(*m_mapBSaveImageFirst)[3] = true;
		(*m_mapBBigenSaveImage)[3] = true;
		(*m_mapBSaveImageFirst)[4] = true;
		(*m_mapBBigenSaveImage)[4] = true;
	}


	(*m_mapBSaveImageFirst)[1] = true;
	(*m_mapBSaveImageFirst)[2] = true;
	(*m_mapBBigenSaveImage)[1] = true;
	(*m_mapBBigenSaveImage)[2] = true;

}


void GD_VideoShow::StopSaveImage( bool bSaveImage /*= true*/ )
{
	(*m_mapBBigenSaveImage)[1] = false;
	(*m_mapBBigenSaveImage)[2] = false;

	if (SHOW_WINDW_SUM_4 == m_iShowWindowNum)
	{
		(*m_mapBBigenSaveImage)[3] = false;
		(*m_mapBBigenSaveImage)[4] = false;
	}
}


void GD_VideoShow::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类

	//CDialog::OnOK();
}
