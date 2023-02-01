#pragma once
#include "resource.h"
#include <time.h>
#include "TERDisplayWnd.h"

// CDialogTERDisplay 对话框

#define WM_NEW_RADAR_DATA WM_USER + 10

#define MAX_CHANNEL 16

#define CHANNELCOUNT 28

class CDialogTERDisplay : public CDialog
{
	DECLARE_DYNAMIC(CDialogTERDisplay)
/*public:
	//实时曲线左右屏分屏显示
	enum separateScreen
	{
		screenFirst = 0,
		screenSecond = 1,
		screenThird = 2,
		screenFourth = 3
	};*/
public:
	CDialogTERDisplay(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDialogTERDisplay();

// 对话框数据
	enum { IDD = IDD_DIALOG_TER_DISPLAY };
	void resizeChannelWnd();

	void AddTERData( TERData *lpData );
	//void DeleteRadarData( int index );

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	//void changeChannel( int index, bool flag );
	//void changeChannel( int index );

	void updateMenuState( int index, CMenu *lpMenu );

	DECLARE_MESSAGE_MAP()
	afx_msg LRESULT OnRadarData( WPARAM wParam, LPARAM lParam );

	std::string _cfgGroupName;

protected:
	TERDisplayWnd m_TERDisplayWnd;
	//CStatic _channelName[CHANNELCOUNT];
	unsigned int _dcValue;

	osg::Vec3d _position;//hjl6.5 用于定时更新经纬度
	CString m_cstrLonlat;//hjl6.5 用于定时更新经纬度
	CStatic _lonlat;//hjl6.5 用于定时更新经纬度

	CString _CstrGpsCondition;//记录gps情况
	CStatic _gpsCondition;//记录gps情况

	//int m_arrnRecordTotalWheelCount[CHANNELCOUNT];//记录经过总道数 
	//int m_arrnRealTotalWheelCount[CHANNELCOUNT];//记录实际总道数
	int m_nRecordTotalWheelCount;//记录经过总道数 
	int m_nRealTotalWheelCount;//记录实际总道数
	//int _recordTotalWheelCount;//记录经过总道数 用于定时更新丢道情况
	//int _realTotalWheelCount;//记录实际总道数 用于定时更新丢道情况
	CString _CstrDataMissing;//hjl20210302 用于定时更新丢道情况
	CStatic _dataMissing;//hjl20210302 用于定时更新丢道情况

	float _wheelDistance;//hjl20210623 用于定时更新距离
	float _gpsDistance;//hjl20210623 用于定时更新距离
	CString _CstrDistance;//hjl20210623 用于定时更新距离
	CStatic _distance;//hjl20210623 用于定时更新距离
	
	time_t _tLastTime;//记录时间 hjl20200907 速度显示模块
	float _fLastLen;//记录距离长度 hjl20200907 速度显示模块
	float _fCurLen;//记录距离长度 hjl20200907 速度显示模块
	float _fSpeed;//计算速度 hjl20200907 速度显示模块
	CString _CstrSpeed;//用cstr接收经纬度 hjl20200907 速度显示模块
	CStatic _speed;//显示速度 hjl20200907 速度显示模块

	float _fRecommendableSpeed;//计算速度 hjl20200907 速度显示模块
	CString _CstrRecommendableSpeed;//用cstr接收经纬度 hjl20210408 推荐速度显示模块
	CStatic _recommendableSpeed;//显示速度 hjl20210408 推荐速度显示模块


	int _iDisplayFlag[CHANNELCOUNT];//hjl11.9 用于抽道显示 用于判定当前道是否显示
	int _iDisplayExtractValue;//hjl11.9 用于抽道显示 _iDisplayExtractValue道中抽出一道显示


	//频道个数
	int m_channelCount;
	std::vector<int> m_channelVec;

	//int m_nInformationIndex;

	CMenu m_pMenu;
public:
	virtual BOOL OnInitDialog();

	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);//未利用
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);//设置timer的刷新间隔 隔一段时间更新一次“实时更新参数”
	afx_msg void OnTimer(UINT_PTR nIDEvent);//隔一段时间更新一次“实时更新参数”
	afx_msg void OnClose();
	afx_msg void OnSize(UINT nType, int cx, int cy);

	std::vector<int> RemoveChannels();

};