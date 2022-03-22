#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include <RadarCurve/ConfigureSet.h>


// DlgRadarParamSet �Ի���

class DlgRadarParamSet : public CDialog
{
	DECLARE_DYNAMIC(DlgRadarParamSet)

public:
	DlgRadarParamSet(CWnd* pParent = NULL);   // ��׼���캯��

	virtual ~DlgRadarParamSet();

	virtual BOOL OnInitDialog();
	void Init();


// �Ի�������
	enum { IDD = IDD_DIALOG_PARAM_SET_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
protected:
	CString m_serverIP;
	short m_serverPort;
	CString m_id;
	CComboBox m_sampleComboBox;
	CComboBox m_sampleRatioComboBox;
	CComboBox m_gainComboBox;
	CSliderCtrl m_gainValueSlider;
	CEdit m_gainValue;
	CSliderCtrl m_delaySlider;
	CEdit m_delayValue;
	CComboBox m_addCountComboBox;
	CComboBox m_testTypeComboBox;
	short m_channelCount;

	float m_precLen;
	int m_precRatio;

	CComboBox m_precIndex;


	CString m_dbSource;
	CString m_dbName;
	CString m_dbUser;
	CString m_dbPwd;

	OpenThreads::Mutex _lock;

public:
	afx_msg void OnBnClickedBtnsetOk();
	afx_msg void OnHScroll(UINT   nSBCode,   UINT   nPos,   CScrollBar*   pScrollBar);
	afx_msg void OnBnClickedButtonApply();
	afx_msg void OnBnClickedBtnsetCancel();
protected:
	//�����ĻҪչʾ��Ƶ��
	CString m_LeftScreenChannels;
	// �ұ���ĻҪչʾ��Ƶ��
	CString m_RightScreenChannels;
};
