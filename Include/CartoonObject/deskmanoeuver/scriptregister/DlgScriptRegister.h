#pragma once
#include "ListBoxMenu.h"
// CDlgScriptRegister �Ի���

class CDlgScriptRegister : public CDialog
{
	DECLARE_DYNAMIC(CDlgScriptRegister)

public:
	CDlgScriptRegister(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgScriptRegister();

// �Ի�������
	enum { IDD = IDD_DLG_SCRIPT_REGISTER };

	enum
	{
		WM_SCRIPT_REGISTER_MSG	= WM_USER + 0x00010,
		WM_SCRIPT_BROWSER		= WM_SCRIPT_REGISTER_MSG + 1
	};

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
	afx_msg void OnNMRClickElementList(NMHDR *pNMHDR, LRESULT *pResult);	
	afx_msg void OnClose();
	virtual BOOL OnInitDialog();
	virtual void PostNcDestroy();

	LRESULT OnRegister(WPARAM wParam,LPARAM lParam);
	LRESULT OnBrowser(WPARAM wParam,LPARAM lParam);
	void OnUnRegister();
	void OnBrowserRegisterScript();
	//�������ݰ�����Ŀ¼
public:
	CString	m_CaseFolder;
	CListBoxMenu m_ListBox;
	CListCtrl m_ListCtrl;
	std::map<CString,CString> m_RegisterVec;

	afx_msg void OnBnClickedOk();
	void SetCaseFolder(CString strDeskFolder);
private:
	void PlayScript(CString szScriptName);
};
