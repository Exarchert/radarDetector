#pragma once

#include <OpenThreads/ScopedLock>
#include <OpenThreads/Mutex>
#include "TERParaWnd.h"
#include "resource.h"

class ConfigureSet;

// DlgTERParameterConfig 对话框
class DlgTERParameterConfig : public CDialog
{
	DECLARE_DYNAMIC(DlgTERParameterConfig)

public:
	DlgTERParameterConfig(CWnd* pParent = NULL);   // 标准构造函数
//	virtual ~DlgTERParameterConfig();

// 对话框数据
	enum { IDD = IDD_DIALOG_TER_PARA };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	void setConfigure();
	void RefreshTimeWindow();

protected:
	TERParaWnd m_TERParaWnd;

private:
	CString m_wheelEdit;
	CString m_perimeterEdit;
	CComboBox m_comboxTERWheelPulse;
	CComboBox m_comboSenderWave;
	CComboBox m_hzCombo;
	CComboBox m_coilCombo;
	CString m_cstrSenderDelayEdit;
	CString m_dutyEdit;
	CString m_gpsEdit;
	CString m_stackingFoldEdit;

	//afx_msg void OnPaint();

public:
	afx_msg void OnClose();
	afx_msg void OnBnClickedTERSaveParaButton();
	afx_msg void OnBnClickedTERSocketConnectButton();
	afx_msg void OnBnClickedTERSocketDisconnectButton();
	afx_msg void OnBnClickedSynButton();
	afx_msg void OnBnClickedHandRadio();
	afx_msg void OnBnClickedGpsRadio();
	afx_msg void OnBnClickedMeterRadio();
	afx_msg void OnBnClickedReadyButton();
	afx_msg void OnBnClickedStartButton();
	afx_msg void OnBnClickedStopButton();


private:
	CComboBox m_sampCombo;
	CComboBox m_gainCombo;
	CString m_channelEdit;
	CString m_cstrReceiveDelayEdit;
	CString m_stackFoldEdit;
public:
	afx_msg void OnBnClickedSynreceiveButton();
	afx_msg void OnBnClickedReadyreceiveButton();
	afx_msg void OnBnClickedStartreceiveButton();
	afx_msg void OnBnClickedStopreceiveButton();
	afx_msg void OnCbnSelchangeHzcombo();
	afx_msg void OnBnClickedButtonTimewindow();
};
