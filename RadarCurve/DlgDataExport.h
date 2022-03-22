#pragma once
#include "afxcmn.h"

#include <RadarCurve/DBTable.h>
#include "RadarCurve/RadarData.h"
#include <vector>

// DlgDataExport �Ի���

class DlgDataExport : public CDialog
{
	DECLARE_DYNAMIC(DlgDataExport)

public:
	DlgDataExport(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~DlgDataExport();

// �Ի�������
	enum { IDD = IDD_DlgDataExport };

	void writeDisLLPic(std::string videoTabSerialNum, RadarData *lpRadarData,std::ofstream& infile);
	void writeDisLLPicNew(RadarData *lpRadarData,std::ofstream& infile);

	//��ȡҪѡ��Ĺ�������
	bool GetSelectProjectIndex(std::vector<int>& pSelectedIndex);
private:
	int m_indexSelect;

	CDBTable::RowVector* m_projectList;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnexportgps();
	afx_msg void OnBnClickedCancel();
	virtual BOOL OnInitDialog();
	CListCtrl m_ProjectListCtrl;
//	afx_msg void OnLvnKeydownListProjectInfo(NMHDR *pNMHDR, LRESULT *pResult);
	
	afx_msg void OnBnClickedBtnexportproject();
	afx_msg void OnBnClickedBtnfilepath();
	// �ļ�·��
	CString m_strEditExportPath;
	afx_msg void OnBnClickedBtnexportfile();
};
