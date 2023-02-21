#pragma once
#include "..\..\..\CartoonObject\resource.h"
#include <CartoonObject\DeskManoeuver\PlotCartoon\ListBoxBase.h>

// CDlgPlotCart 对话框

namespace CartoonObject
{
	class CDlgPlotCart : public CDialog
	{
		DECLARE_DYNAMIC(CDlgPlotCart)

	public:
		CDlgPlotCart(CWnd* pParent = NULL);   // 标准构造函数
		virtual ~CDlgPlotCart();

		// 对话框数据
		enum { IDD = IDD_DLG_PLOT_CART };

		enum
		{
			WM_PLOTADD_MSG = WM_USER + 0x00010,
			WM_PLOTDEL_MSG = WM_USER + 0x00011,
			WM_PLOTBROWSER_MSG = WM_USER + 0x00012,
			WM_PLOTINSERT_MSG = WM_USER + 0x00013
		};

	protected:
		virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

		DECLARE_MESSAGE_MAP()
		LRESULT OnAddPlot(WPARAM wParam,LPARAM lParam);
		LRESULT OnDeletePlot(WPARAM wParam,LPARAM lParam);
		LRESULT OnBrowserPlot(WPARAM wParam,LPARAM lParam);
		LRESULT OnInsertPlot(WPARAM wParam,LPARAM lParam);
	public:
		virtual BOOL OnInitDialog();
		void SetActive(BOOL bActive);

	protected:
		CListBoxBase	m_PlotListBox;
		std::map<CString,CString>	m_RegisterVec;
	};
}
