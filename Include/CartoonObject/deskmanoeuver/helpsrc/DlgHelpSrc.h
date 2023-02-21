#pragma once
#include "..\..\..\CartoonObject\resource.h"
#include <CartoonObject\DeskManoeuver\ListCtrlBase.h>
#include <CartoonObject\DeskManoeuver\HelpSrc\EHHelpSrc.h>


namespace CartoonObject
{
	// CDlgDisaster �Ի���

	class CDlgHelpSrc : public CDialog
	{
		DECLARE_DYNAMIC(CDlgHelpSrc)

	public:
		CDlgHelpSrc(CWnd* pParent = NULL);   // ��׼���캯��
		virtual ~CDlgHelpSrc();

		// �Ի�������
		enum { IDD = IDD_DLG_DESK_MANOEUVER };

	protected:
		virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
		LRESULT OnListCtrlLeave(WPARAM wParam,LPARAM lParam);
		LRESULT OnAddHelpSrc(WPARAM wParam,LPARAM lParam);

		DECLARE_MESSAGE_MAP()
	public:
		virtual BOOL OnInitDialog();
		void SetActive(BOOL bActive);

	protected:
		CListCtrlBase	m_HelpSrcListCtrl;
		//������
		osg::ref_ptr<CEHHelpSrc>	m_EHHelpSrc;
	};
}
