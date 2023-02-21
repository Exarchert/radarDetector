#pragma once
#include "afxwin.h"


// CDlgDeskNew 对话框

class CDlgDeskNew : public CDialog
{
	DECLARE_DYNAMIC(CDlgDeskNew)

public:
	CDlgDeskNew(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgDeskNew();

// 对话框数据
	enum { IDD = IDD_DLG_DESK_CREATE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnNew();
	afx_msg void OnBnClickedOk();
	afx_msg void OnCbnSelchangeComboDesklist();
	afx_msg void OnClose();

	afx_msg void OnBnClickedBtnGetPos();

	virtual BOOL OnInitDialog();
	virtual void PostNcDestroy();

	bool ReadPosInfo(CString strFolder);

public:
	bool	m_bChanged;
	CString m_CurDeskFolder;

	double m_DeskLon;
	double m_DeskLat;
	double m_DeskAlt;
	float  m_DeskYaw;
	float  m_DeskPitch;
	float  m_DeskRoll;

	CComboBox m_DeskList;
	std::map<CString,CString>	m_FolderMap;
};

