#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CDlgWorkDetection �Ի���

class CDlgWorkDetection : public CDialog
{
	DECLARE_DYNAMIC(CDlgWorkDetection)

public:
	CDlgWorkDetection(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgWorkDetection();

// �Ի�������
	enum { IDD = IDD_DLG_WorkDetection };

	CDialog *_loadProcess;
	CString m_pRadarTrackPointsPath;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	void GetAccountLength( const long & startLen,CString & strZhunagHaoEX,CString & strJULI );
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnExtracttrackpoints();
private:
	// �����б�
	CListCtrl m_ProjectListCtrl;
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedBtnDraw();
	// ��ȡ���
	float m_Interval;
	// �߿�
	float m_pLineWidth;
	afx_msg void OnEnChangeEditinterval();
	afx_msg void OnEnChangeEditLinewidth();
};
