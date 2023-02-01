#pragma once
#include "afxwin.h"


// CSender 对话框

class CSender : public CPropertyPage
{
	DECLARE_DYNAMIC(CSender)

public:
	CSender();
	virtual ~CSender();

// 对话框数据
	enum { IDD = IDD_PAGE1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	CString m_wheelEdit;
	CString m_perimeterEdit;
	CComboBox m_hzCombo;
	CComboBox m_coilCombo;
	CString m_delayEdit;
	CString m_dutyEdit;
	CString m_gpsEdit;
	CString m_stackingFoldEdit;
public:
	virtual BOOL OnInitDialog();
	//afx_msg void OnPaint();
public:
	void setConfigure();

	afx_msg void OnBnClickedSynButton();
	afx_msg void OnBnClickedHandRadio();
	afx_msg void OnBnClickedGpsRadio();
	afx_msg void OnBnClickedMeterRadio();
	afx_msg void OnBnClickedReadyButton();
	afx_msg void OnBnClickedStartButton();
	afx_msg void OnBnClickedStopButton();
};
