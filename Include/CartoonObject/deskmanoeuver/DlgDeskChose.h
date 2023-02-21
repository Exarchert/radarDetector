#pragma once
#include "afxwin.h"


// CDlgDeskChose �Ի���

class CDlgDeskChose : public CDialog
{
	DECLARE_DYNAMIC(CDlgDeskChose)

public:
	CDlgDeskChose(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgDeskChose();

// �Ի�������
	enum { IDD = IDD_DLG_DESK_CHOSE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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

	//��UI�̵߳���
	static bool ShowDeskFolderChoseDlg(CString& strDeskFolder);
};
