#pragma once
#include <Object/ModelView/ssCoreView.h>
#include <Object/Export.h>

// StreetscapeDlalog 对话框

class OBJECT_EXPORT StreetscapeDlalog : public CDialog
{
	DECLARE_DYNAMIC(StreetscapeDlalog)

public:
	StreetscapeDlalog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~StreetscapeDlalog();

// 对话框数据
	enum { IDD = IDD_Screetscape };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
private:
	ssCoreView* m_SSCoreView;
	CRect m_rect;//保持窗口大小
	HICON m_hIcon;
private:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
public:
	afx_msg void OnPaint();
};
