#pragma once
#include "afxcmn.h"
#include <RadarCurve/DBTable.h>


// CDialogProject 对话框

class CDialogProject : public CDialog
{
	DECLARE_DYNAMIC(CDialogProject)

public:
	CDialogProject(CDBTable::RowVector & projectList, int iType, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDialogProject();

// 对话框数据
	enum { IDD = IDD_DIALOG_PROJECT };

	CString m_selectProjectName;
	CString m_selectProjectTab;
	float m_selectLen;


	int m_iType;//

	/////-------------------------------------------
	bool	m_bProcessMsg;
	int m_iSelectIndex;//
	void * m_lpProjectRow;//

	/////-------------------------------------------
	osg::ref_ptr<DBRow> _selectRow;
	void ReInit();

private:
	CDBTable::RowVector &_projectList;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	void prepareDBTab( CString const& projectCreatTime );

	void showHistoryDlg( int index );

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonNew();
	afx_msg void OnBnClickedButtonModify();
	afx_msg void OnBnClickedButtonDel();
	CListCtrl m_projectList;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnNMDblclkListProject(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMRclickListProject(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnHisChannel1();
	afx_msg void OnHisChannel2();
	afx_msg void OnHisChannel3();
	afx_msg void OnHisChannel4();
	afx_msg void OnHisChannel5();
	afx_msg void OnHisChannel6();
	afx_msg void OnHisChannel7();
	afx_msg void OnHisChannel8();
	afx_msg void OnExportHistory();
	void GetStartLength(float & startLen, CString & strZhunagHaoEX, CString & strJULI);
	void GetZhuanghaoLength(const long & startLen,CString & strZhunagHaoEX,CString & strJULI );
	afx_msg void OnExportAllChannel();
	//afx_msg void OnBnClickedBtnExportAllSgy();
	afx_msg void OnLvnItemchangedListProject(NMHDR *pNMHDR, LRESULT *pResult);
	void initForNoProjectName();

};
