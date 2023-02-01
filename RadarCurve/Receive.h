#pragma once
#include "afxwin.h"


// CReceive 对话框

class CReceive : public CPropertyPage
{
	DECLARE_DYNAMIC(CReceive)

public:
	CReceive();
	virtual ~CReceive();

// 对话框数据
	enum { IDD = IDD_PAGE2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
private:
	void setConfigure();


private:
	CComboBox m_sampCombo;
	CComboBox m_gainCombo;
	CString m_channelEdit;
	CString m_delayEdit;
	CString m_stackFoldEdit;
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedSynreceiveButton();
	afx_msg void OnBnClickedReadyreceiveButton();
	afx_msg void OnBnClickedStartreceiveButton();
	afx_msg void OnBnClickedStopreceiveButton();
};
