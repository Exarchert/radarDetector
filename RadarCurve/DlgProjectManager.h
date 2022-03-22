#pragma once


// DlgProjectManager 对话框

class DlgProjectManager : public CDialog
{
	DECLARE_DYNAMIC(DlgProjectManager)

public:
	DlgProjectManager(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~DlgProjectManager();

// 对话框数据
	enum { IDD = IDD_DLGPROJECTMANAGER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
