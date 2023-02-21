#pragma once
#include "afxwin.h"


// CDlgDeskChose 对话框

class CDlgDeskChose : public CDialog
{
	DECLARE_DYNAMIC(CDlgDeskChose)

public:
	CDlgDeskChose(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgDeskChose();

// 对话框数据
	enum { IDD = IDD_DLG_DESK_CHOSE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	CComboBox m_DeskList;
	afx_msg void OnCbnSelchangeComboDesklist();
	afx_msg void OnBnClickedCheckShow();
	virtual BOOL OnInitDialog();

	std::map<CString,CString>	m_FolderMap;

	BOOL m_bUnShowDlg;
public:
	CString m_CurDeskFolder;

	//仅UI线程调用
	static bool ShowDeskFolderChoseDlg(CString& strDeskFolder);
};
