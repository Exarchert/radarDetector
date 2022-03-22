#pragma once


// CDialogProject_Import 对话框

class CDialogProject_Import : public CDialog
{
	DECLARE_DYNAMIC(CDialogProject_Import)

public:
	CDialogProject_Import(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDialogProject_Import();

// 对话框数据
	enum { IDD = IDD_DIALOG_PROJECTEXPORT };//

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
