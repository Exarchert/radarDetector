#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CDlgWorkDetection 对话框

class CDlgWorkDetection : public CDialog
{
	DECLARE_DYNAMIC(CDlgWorkDetection)

public:
	CDlgWorkDetection(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgWorkDetection();

// 对话框数据
	enum { IDD = IDD_DLG_WorkDetection };

	CDialog *_loadProcess;
	CString m_pRadarTrackPointsPath;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	void GetAccountLength( const long & startLen,CString & strZhunagHaoEX,CString & strJULI );
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnExtracttrackpoints();
private:
	// 工程列表
	CListCtrl m_ProjectListCtrl;
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedBtnDraw();
	// 提取间隔
	float m_Interval;
	// 线宽
	float m_pLineWidth;
	afx_msg void OnEnChangeEditinterval();
	afx_msg void OnEnChangeEditLinewidth();
};
