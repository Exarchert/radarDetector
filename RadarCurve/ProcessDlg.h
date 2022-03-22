#pragma once
#include "afxcmn.h"


// CProcessDlg 对话框

class CProcessDlg : public CDialog
{
	DECLARE_DYNAMIC(CProcessDlg)

public:
	CProcessDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CProcessDlg();

	void StartProcess( int allCount );
	void Step( int stepCount );
	void StepEx(int setCount = 0);

// 对话框数据
	enum { IDD = IDD_DIALOG_LOAD_PROCESS };

	int m_iChangeStep;
	bool m_bUptade;//
	bool m_bThread;
	bool m_bClose;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedOk();
	CProgressCtrl m_processCountCtrl;
	afx_msg void OnPaint();
};
