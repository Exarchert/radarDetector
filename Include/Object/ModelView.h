#pragma once
#include "../Object/resource.h"
#include <Object/ModelView/ssCoreView.h>
// CModelView 对话框

namespace EMObj
{
class  CModelView : public CDialog
{
	DECLARE_DYNAMIC(CModelView)

public:
	CModelView(CWnd* pParent = NULL/*,osg::Node* node =NULL*/);   // 标准构造函数
	virtual ~CModelView();

	bool SetPreviewModel(osg::Node* node);

// 对话框数据
	enum { IDD = IDD_Dlg_View };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	// 实现
protected:
	HICON m_hIcon;
	osg::Node* m_node;
	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
//	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

private:
	ssCoreView* m_SSCoreView;

	CRect m_rect;//保持窗口大小

	afx_msg void OnSize(UINT, int, int);
	afx_msg void OnDestroy();
public:
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
};
}
