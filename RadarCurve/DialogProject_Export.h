#pragma once
#include "afxcmn.h"
#include <RadarCurve/DBTable.h>
#include <vector>
#include "RadarCurve/RadarData.h"

// CDialogProject_Export 对话框

class CDialogProject_Export : public CDialog
{
	DECLARE_DYNAMIC(CDialogProject_Export)

public:
	CDialogProject_Export(CDBTable::RowVector & projectList, int iType, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDialogProject_Export();

// 对话框数据
	enum { IDD = IDD_DIALOG_PROJECT_EXPORT };//IDD_DIALOG_PROJECT

	CString m_strFolderPath ;//
	CString m_selectProjectName;
	CString m_selectProjectTab;
	float m_selectLen;

	int m_indexSelect;


	int m_iType;//对话框样式 

	CDBTable::RowVector &_projectList;

	osg::ref_ptr<DBRow> _selectRow;

	void ReInit();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	void prepareDBTab( CString const& projectCreatTime );

	//void showHistoryDlg( int index );

	DECLARE_MESSAGE_MAP()

private:
	CString m_strWndText;
	void writeDisLLPic(std::string videoTabSerialNum, RadarData *lpRadarData,std::ofstream& infile);
public:

	CListCtrl m_projectList;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnNMDblclkListProject(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMRclickListProject(NMHDR *pNMHDR, LRESULT *pResult);

	afx_msg void OnBnClickedButtonPositonexport();
	void OnBnClickedBtnExportAllSgy();
	CString m_strEditExportPath;

	void SetMyWndText(CString str);


	std::vector<CString> m_vPath;
	std::vector<CString> m_vProTime;
	std::vector<CString> m_vProName;
	afx_msg void OnBnClickedBtnexportfile();
};
