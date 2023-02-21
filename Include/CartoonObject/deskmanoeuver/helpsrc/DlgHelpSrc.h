#pragma once
#include "..\..\..\CartoonObject\resource.h"
#include <CartoonObject\DeskManoeuver\ListCtrlBase.h>
#include <CartoonObject\DeskManoeuver\HelpSrc\EHHelpSrc.h>


namespace CartoonObject
{
	// CDlgDisaster 对话框

	class CDlgHelpSrc : public CDialog
	{
		DECLARE_DYNAMIC(CDlgHelpSrc)

	public:
		CDlgHelpSrc(CWnd* pParent = NULL);   // 标准构造函数
		virtual ~CDlgHelpSrc();

		// 对话框数据
		enum { IDD = IDD_DLG_DESK_MANOEUVER };

	protected:
		virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
		LRESULT OnListCtrlLeave(WPARAM wParam,LPARAM lParam);
		LRESULT OnAddHelpSrc(WPARAM wParam,LPARAM lParam);

		DECLARE_MESSAGE_MAP()
	public:
		virtual BOOL OnInitDialog();
		void SetActive(BOOL bActive);

	protected:
		CListCtrlBase	m_HelpSrcListCtrl;
		//监听类
		osg::ref_ptr<CEHHelpSrc>	m_EHHelpSrc;
	};
}
