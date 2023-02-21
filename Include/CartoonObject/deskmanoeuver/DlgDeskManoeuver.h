#pragma once
#include "../../../CartoonObject/resource.h"
#include <CartoonObject/DeskManoeuver/PlotCartoon/DlgPlotCart.h>
#include <CartoonObject\DeskManoeuver\DisasterSource\DlgDisaster.h>
#include <CartoonObject\DeskManoeuver\TabCtrlBase.h>
#include <CartoonObject\DeskManoeuver\CharacterRole\DlgRole.h>
#include <CartoonObject\DeskManoeuver\MoveElement\DlgMovement.h>
#include <CartoonObject\DeskManoeuver\FireEquipment\DlgFireEquip.h>
#include <CartoonObject\DeskManoeuver\UserScreenMark\DlgScreenTextMark.h>
#include <CartoonObject\DeskManoeuver\ScriptGenerateManager.h>
#include <CartoonObject\DeskManoeuver\HotArea\EHHotArea.h>
#include <CartoonObject\DeskManoeuver\HelpSrc\DlgHelpSrc.h>
#include "ListCtrlBase.h"
#include "DlgEventList.h"


// CDlgDeskManoeuver 对话框
namespace CartoonObject
{
	class CDlgDeskManoeuver : public CDialog
	{
		DECLARE_DYNAMIC(CDlgDeskManoeuver)

	public:
		CDlgDeskManoeuver(CWnd* pParent = NULL);   // 标准构造函数
		virtual ~CDlgDeskManoeuver();

		// 对话框数据
		enum { IDD = IDD_DLG_BASE };

	protected:
		virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

		DECLARE_MESSAGE_MAP()
		afx_msg void OnClose();
		virtual BOOL OnInitDialog();

		afx_msg void OnBtnGenerate();	
		afx_msg void OnNMRClickElementList(NMHDR *pNMHDR, LRESULT *pResult);	
		afx_msg void OnDelElement();
		afx_msg void OnBrowserElement();
		afx_msg void OnBtnBrowser();
		afx_msg void OnBtnEventList();
		afx_msg void OnCbnSelDesklist();

		LRESULT OnAddHotArea(WPARAM wParam,LPARAM lParam);
		LRESULT OnTabChanged(WPARAM wParam,LPARAM lParam);


	public:
		void AddItemToElementList(CDeskManoElements* pElement);
		void InsertItemToElementList(CDeskManoElements* pElement);

		void FindDeskFile();

	protected:
		CTabCtrlBase			m_TabCtrl;
		CDlgFireEquip*			m_pDlgFireEquip;
		CDlgDisaster*			m_pDlgDisaster;
		CDlgPlotCart*			m_pDlgPlotCart;
		CDlgRole*				m_pDlgRole;
		CDlgMovement*			m_pDlgMovement;
		CDlgScreenTextMark*		m_pDlgText;
		CDlgHelpSrc*			m_pDlgHelpSrc;

		int						m_TabCount;
		CString					m_ScriptName;
		CString					m_preDestFile;

	protected:
		CListCtrlBase m_ListCtrl;
		CComboBox m_DeskList;
		osg::ref_ptr<CScriptGenerateManager>	m_ScriptCreateManager;
		osg::ref_ptr<CEHHotArea>	m_EHHotArea;

	protected:
		float GetTimeLengthFromArchiveFile(CString szFilePath);
		float GetTimeLengthFromXmlFile(CString szFilePath);
		virtual void PostNcDestroy();

		bool ParserXmlFile(CString szFilePath);
		void ParserDeskElement(CString strAttri);

	public:
		static CString	m_CaseFolder;

		void SetCaseFolder(CString strFolder);
		static CString GetCaseFolder(CString& strFolder = CString(_T("")));

	public:
		//大事件列表
		CDlgEventList	m_DlgEvent;
		bool			m_bChanged;
	};
}
