#pragma once
#include "../Object/resource.h"
#include <Object/ModelView/ssCoreView.h>
// CModelView �Ի���

namespace EMObj
{
class  CModelView : public CDialog
{
	DECLARE_DYNAMIC(CModelView)

public:
	CModelView(CWnd* pParent = NULL/*,osg::Node* node =NULL*/);   // ��׼���캯��
	virtual ~CModelView();

	bool SetPreviewModel(osg::Node* node);

// �Ի�������
	enum { IDD = IDD_Dlg_View };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	// ʵ��
protected:
	HICON m_hIcon;
	osg::Node* m_node;
	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
//	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

private:
	ssCoreView* m_SSCoreView;

	CRect m_rect;//���ִ��ڴ�С

	afx_msg void OnSize(UINT, int, int);
	afx_msg void OnDestroy();
public:
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
};
}
