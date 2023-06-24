#pragma once
#include "afxwin.h"
#include "Resource.h"

#include <list>
#include <map>
// GD_VideoShow 对话框
using namespace std;

#define SHOW_WINDW_SUM_2		2
#define SHOW_WINDW_SUM_4		4
#define SHOW_WINDW_SUM_MAX		4

#define I_WINDOW_1				1
#define I_WINDOW_2				2
#define I_WINDOW_3				3
#define I_WINDOW_4				4
//#define IDD_DIALOG_VIDEO                1062
class  GD_VideoShow : public CDialog
{
	DECLARE_DYNAMIC(GD_VideoShow)

public:
	GD_VideoShow(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~GD_VideoShow();

	void SetPicPath( CString picPath );
	CString GetPicPath();

	void SetShowWindowSum(int sum);
	int GetShowWindowSum();

	void SetProPath( CString proPath );

	enum { IDD = IDD_DIALOG_VIDEO };

	enum ShowType
	{
		GD_None,
		GD_Show,//
		GD_BackPlay,//
	};


protected:

	map<int,GUID>* m_mapGuidMonitor;
	map<int,HANDLE>* m_mapHVideoDevice;
	map<int,HANDLE>* m_mapHProperty;
	map<int,HANDLE>* m_mapHRenderer;
	map<int,HANDLE>* m_mapHMP4Muxer;
	map<int,HANDLE>* m_mapHMP4MuxerA;
	map<int,HANDLE>* m_mapHAudioDevice;
	map<int,HANDLE>* m_mapHAVIMuxer;
	map<int,bool>* m_mapBSaveImageFirst;
	map<int,bool>* m_mapBBigenSaveImage;
	map<int,bool>* m_mapBOpenDevice;
	map<int,WORD>* m_mapDConutTime;//
	map<int,int>* m_mapIVideoDeviceSel;//
	map<int,CStatic*>* m_mapWStaticTite;//
	map<int,CStatic*>* m_mapStaticWindow;//
	map<int,CStatic*>* m_mapStaticLineRoud;//
	map<int,CButton*>* m_mapBtnOK;
	map<int,CEdit*>* m_mapEditIn;//
	map<int,int*>* m_mapEditValue;//



	//控件
	int m_iEdit;
	int m_iEdit2;
	int m_iEdit3;
	int m_iEdit4;

	CStatic m_sWindow;//
	CStatic m_sWindow2;
	CStatic m_sWindow3;
	CStatic m_sWindow4;

	CButton m_btnPlay;//
	CButton m_btnPlay2;
	CButton m_btnPlay3;
	CButton m_btnPlay4;

	CStatic m_sTite;//
	CStatic m_sTite2;
	CStatic m_sTite3;
	CStatic m_sTite4;

	CStatic m_sLine;
	CStatic m_sLine2;
	CStatic m_sLine3;
	CStatic m_sLine4;

	CEdit m_eInLine;
	CEdit m_eInLine2;
	CEdit m_eInLine3;
	CEdit m_eInLine4;

protected:
	bool        m_bInitWndShow;
	int			m_cx;
	int			m_cy;
	int			m_nFrameDuration;


	CString m_proPath;
	CString     m_cPatrFile;//储存图片地方
	int         m_iOnceTime;//
	ShowType m_sType;//


	//
	DWORD		m_dwMajorVersion;
	BOOL		m_bOverlay;
	BOOL		m_bVideoPreview;
	int			m_nVideoDeviceCount;
	int			m_iShowWindowNum;

	int			m_iCountTimeOne;

protected:
	static void VideoCaptureCallback(const BYTE * pbyImage, int cbImageStride, void * pvParam, UINT64 u64TimeStamp) {
		GD_VideoShow * pThis = (GD_VideoShow *)pvParam;
		pThis->VideoCaptureCallbackInternal(pbyImage, cbImageStride, u64TimeStamp,I_WINDOW_1);
	}
	static void VideoCaptureCallback2(const BYTE * pbyImage, int cbImageStride, void * pvParam, UINT64 u64TimeStamp) {
		GD_VideoShow * pThis = (GD_VideoShow *)pvParam;
		pThis->VideoCaptureCallbackInternal(pbyImage, cbImageStride, u64TimeStamp,I_WINDOW_2);
	}
	static void VideoCaptureCallback3(const BYTE * pbyImage, int cbImageStride, void * pvParam, UINT64 u64TimeStamp) {
		GD_VideoShow * pThis = (GD_VideoShow *)pvParam;
		pThis->VideoCaptureCallbackInternal(pbyImage, cbImageStride, u64TimeStamp,I_WINDOW_3);
	}
	static void VideoCaptureCallback4(const BYTE * pbyImage, int cbImageStride, void * pvParam, UINT64 u64TimeStamp) {
		GD_VideoShow * pThis = (GD_VideoShow *)pvParam;
		pThis->VideoCaptureCallbackInternal(pbyImage, cbImageStride, u64TimeStamp,I_WINDOW_4);
	}
	void VideoCaptureCallbackInternal(const BYTE * pbyImage, int cbImageStride, UINT64 u64TimeStamp, int iWindow);
public:

	//
	BOOL OpenDevice(int nSel,int iWindow);
	void ResizeWindow(int iWindow);//
	void UpdateRenderer(int iWindow);//

	void OnFileStop(int iWindow);
	//void OnFileStop2();
	void SaveData(int iWindow);
	void SaveData(int iWindow,std::string strOriginalName,int nQuality);
	void SaveAllData(std::string strOriginalName,int nQuality);
	//void SaveData2();
	void OpenData();
	void OpenData2();

	//获得年月日时分秒
	CString GetSysTimeYMDHMS();
	void CalculateTime(int iWindow);//
	void CalculateTime2();

	void VideoShow();
	//void VideoBackplay();

	void SetShowType(ShowType type){m_sType = type;}
	ShowType GetShowType(){return m_sType;}



	void StartSaveImage(bool bSaveImage = true);
	void StopSaveImage(bool bSaveImage = true);

	void BnClickedButtonPlay(int iWindow);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	afx_msg void OnMove(int x, int y);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedButtonPlayleft();
	afx_msg void OnBnClickedButtonPlayleft2();
	afx_msg void OnBnClickedButtonPlayleft3();
	afx_msg void OnBnClickedButtonPlayleft4();
	afx_msg void OnClose();
protected:
	virtual void OnOK();
};
