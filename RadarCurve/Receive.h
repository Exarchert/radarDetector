#pragma once
#include "afxwin.h"


// CReceive �Ի���

class CReceive : public CPropertyPage
{
	DECLARE_DYNAMIC(CReceive)

public:
	CReceive();
	virtual ~CReceive();

// �Ի�������
	enum { IDD = IDD_PAGE2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
