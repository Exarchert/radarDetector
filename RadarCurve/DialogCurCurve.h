#pragma once
#include "resource.h"
#include <time.h>
#include "RadarCurveWnd.h"

// CDialogCurCurve 对话框

#define WM_NEW_RADAR_DATA WM_USER + 10

#define MAX_CHANNEL 16

#define CHANNELCOUNT 28

class CDialogCurCurve : public CDialog
{
	DECLARE_DYNAMIC(CDialogCurCurve)
public:
	//实时曲线左右屏分屏显示
	enum separateScreen
	{
		screenFirst = 0,
		screenSecond = 1,
		screenThird = 2,
		screenFourth = 3
	};
public:
	CDialogCurCurve(separateScreen pScreenType, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDialogCurCurve();

// 对话框数据
	enum { IDD = IDD_DIALOG_CUR_CURVE };
	void resizeChannelWnd();

	void AddRadarData( RadarData *lpData , int index );
	//void DeleteRadarData( int index );

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	void changeChannel( int index, bool flag );
	void changeChannel( int index );

	void updateMenuState( int index, CMenu *lpMenu );

	DECLARE_MESSAGE_MAP()
	afx_msg LRESULT OnRadarData( WPARAM wParam, LPARAM lParam );


	std::string _cfgGroupName;


protected:
	RadarCurveWnd _curveWnd[CHANNELCOUNT];
	CStatic _channelName[CHANNELCOUNT];
	unsigned int _dcValue;

	int m_nTrueChannelCount;//通道个数

	osg::Vec3d _position;//hjl6.5 用于定时更新经纬度
	CString _CstrLonlat;//hjl6.5 用于定时更新经纬度
	CStatic _lonlat;//hjl6.5 用于定时更新经纬度

	CString _CstrGpsCondition;//记录gps情况
	CStatic _gpsCondition;//记录gps情况

	int m_arrnRecordTotalWheelCount[CHANNELCOUNT];//记录经过总道数 
	int m_arrnRealTotalWheelCount[CHANNELCOUNT];//记录实际总道数
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

	//对话框在那个屏幕上
	separateScreen m_separateScreen;

	int m_nSaveFileType;

	//频道个数
	int m_channelCount;
	std::vector<int> m_channelVec;

	int m_nInformationIndex;

	CMenu m_pMenu;
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnShowChannel00();
	afx_msg void OnShowChannel01();
	afx_msg void OnShowChannel02();
	afx_msg void OnShowChannel03();
	afx_msg void OnShowChannel04();
	afx_msg void OnShowChannel05();
	afx_msg void OnShowChannel06();
	afx_msg void OnShowChannel07();
	afx_msg void OnShowChannel08();
	afx_msg void OnShowChannel09();
	afx_msg void OnShowChannel10();
	afx_msg void OnShowChannel11();
	afx_msg void OnShowChannel12();
	afx_msg void OnShowChannel13();
	afx_msg void OnShowChannel14();
	afx_msg void OnShowChannel15();
	afx_msg void OnShowChannel16();
	afx_msg void OnShowChannel17();
	afx_msg void OnShowChannel18();
	afx_msg void OnShowChannel19();
	afx_msg void OnShowChannel20();
	afx_msg void OnShowChannel21();
	afx_msg void OnShowChannel22();
	afx_msg void OnShowChannel23();
	afx_msg void OnShowChannel24();
	afx_msg void OnShowChannel25();
	afx_msg void OnShowChannel26();
	afx_msg void OnShowChannel27();

	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);//未利用
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);//设置timer的刷新间隔 隔一段时间更新一次“实时更新参数”
	afx_msg void OnTimer(UINT_PTR nIDEvent);//隔一段时间更新一次“实时更新参数”
	afx_msg void OnClose();
	afx_msg void OnSize(UINT nType, int cx, int cy);

	std::vector<int> RemoveChannels();

};
