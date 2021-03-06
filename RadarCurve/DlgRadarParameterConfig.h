#pragma once
//OSG
#include <OpenThreads/ScopedLock>
#include <OpenThreads/Mutex>
#include "WaveFormWnd.h"

class ConfigureSet;

// DlgRadarParameterConfig 对话框
class DlgRadarParameterConfig : public CDialog
{
	DECLARE_DYNAMIC(DlgRadarParameterConfig)

public:
	DlgRadarParameterConfig(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~DlgRadarParameterConfig();

// 对话框数据
	enum { IDD = IDD_Dlg_RadarParameterConfig };

private:
	CString m_serverIP;
	short m_serverPort;

	CString m_cstrFtpServerIP;
	CString m_cstrFtpPort;
	CString m_cstrFtpAccount;
	CString m_cstrFtpPassword;

	CString m_id;
	CComboBox m_dielectricComboBox;
	CComboBox m_sampleComboBox;
	CComboBox m_sampleRatioComboBox;
	CComboBox m_gainComboBox;
	//CSliderCtrl m_gainValueSlider;
	//CEdit m_gainValue;
	int m_gainValue;
	//CSliderCtrl m_delaySlider;
	//CEdit m_delayValue;
	int m_delayValue;
	CComboBox m_addCountComboBox;
	CComboBox m_testTypeComboBox;
	short m_channelCount;
	int m_precision;
	float m_precLen;
	int m_precRatio;

	//CComboBox m_precIndex;

	//各屏幕要展示的频道
	CString m_screenFirstChannels;
	CString m_screenSecondChannels;
	CString m_screenThirdChannels;
	CString m_screenFourthChannels;

	//gps
	short m_comNumber;//com口
	int m_portByte;//波特率
	CComboBox m_parityBit;//校验位
	int m_dataBit;//数据位
	CComboBox m_stopBit;//停止位

	//因变量参数
	float m_timeWindow;
	float m_maxDepth;
	int m_scanSpeed;

	//自定义标记
	int m_markValue;//自定义标记值
	
	//抽道显示
	int m_diplayExtract;//m_diplayExtract道中抽一道显示

	ConfigureSet* m_pConfigureSet;
	OpenThreads::Mutex _lock;

	WaveFormWnd _waveFormWnd[16];//hjl 20210418参数设置波形显示
	CStatic _channelName[16];//hjl 20210418参数设置波形显示
	CStatic _channelCorrection[16];//hjl 20210805矫正设置
	int m_dCorrection[16];

protected:
	//SpectrumWnd _spectrumWnd[8];

protected:
	virtual void DoDataExchange(CDataExchange* pDX);   // DDX/DDV 支持
	

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
private:
	// 根据配置文件初始化界面
	void InitByRadarConfig();
public:
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnBnClickedBtnsetOk();
	afx_msg void OnBnClickedBtnsetCancel();
	afx_msg void OnBnClickedButtonApply();
	afx_msg void OnBnClickedButtonWaveFormTest();
	afx_msg void OnClose();
	afx_msg void OnEnChangeEditPort();
//	afx_msg void OnCbnEditchangeComboDielectric();
	afx_msg void OnCbnSelchangeComboDielectric();
//	afx_msg void OnCbnCloseupComboDielectric();
	afx_msg void OnCbnSelchangeComboSample();
	afx_msg void OnCbnSelchangeComboSampleRatio();

	afx_msg void OnEnKillfocusEditCorrection1();
	afx_msg void OnEnKillfocusEditCorrection2();
	afx_msg void OnEnKillfocusEditCorrection3();
	afx_msg void OnEnKillfocusEditCorrection4();
	afx_msg void OnEnKillfocusEditCorrection5();
	afx_msg void OnEnKillfocusEditCorrection6();
	afx_msg void OnEnKillfocusEditCorrection7();
	afx_msg void OnEnKillfocusEditCorrection8();
	afx_msg void OnEnKillfocusEditCorrection9();
	afx_msg void OnEnKillfocusEditCorrection10();
	afx_msg void OnEnKillfocusEditCorrection11();
	afx_msg void OnEnKillfocusEditCorrection12();
	afx_msg void OnEnKillfocusEditCorrection13();
	afx_msg void OnEnKillfocusEditCorrection14();
	afx_msg void OnEnKillfocusEditCorrection15();
	afx_msg void OnEnKillfocusEditCorrection16();

	void changeChannel( int index, bool flag );//hjl 20210418参数设置波形显示
	void resizeChannelWnd();//hjl 20210418参数设置波形显示
	void AddRadarData( RadarData *lpData , int index );//hjl 20210418参数设置波形显示


};
