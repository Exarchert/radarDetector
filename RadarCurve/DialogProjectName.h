#pragma once

class ConfigureSet;
// CDialogProjectName �Ի���

class CDialogProjectName : public CDialog
{
	DECLARE_DYNAMIC(CDialogProjectName)

public:
	CDialogProjectName( CString const& title, CString const& msg, float startLen, CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDialogProjectName();

// �Ի�������
	enum { IDD = IDD_DIALOG_INPUT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	CString m_title;
	CString m_projectName;
	CString m_cstrRoadName;
	CString m_cstrRoadSection;
	CString m_cstrRoadNumber;
	CComboBox m_ccomboboxDirection;
	CString m_cstrDirection;
	CString m_cstrStakeNumber;
	//CComboBox m_daohao;
	
	float m_fDistance;
	float m_startLen;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	CString m_strDaohao;
	
	ConfigureSet* m_pConfigureSet;
};
