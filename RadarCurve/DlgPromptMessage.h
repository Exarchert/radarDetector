#pragma once
#include "resource.h"

// CDlgPromptMessage 对话框

class CDlgPromptMessage : public CDialog
{
	DECLARE_DYNAMIC(CDlgPromptMessage)

public:
	CDlgPromptMessage(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgPromptMessage();

// 对话框数据
	enum { IDD = IDD_DIALOG_MESSAGE_INFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

protected:

	CString m_strTite;
	CString m_strInfo;
	CString m_strOK;
	CString m_strCancle;
public:
	void SetPromptMessage(CString _strTite,CString _strInfo,CString _strOK,CString _strCancle);
	afx_msg void OnBnClickedBtnPromptOk();
	afx_msg void OnBnClickedBtnPromptCancel();
	virtual BOOL OnInitDialog();
};
