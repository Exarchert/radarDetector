// SheetBase.cpp : 实现文件
//

#include "stdafx.h"
#include "RadarCurve.h"
#include "SheetBase.h"




// CSheetBase

IMPLEMENT_DYNAMIC(CSheetBase, CPropertySheet)

CSheetBase::CSheetBase(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
	AddPage(&m_sender);
	AddPage(&m_receive);

}

CSheetBase::CSheetBase(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
	AddPage(&m_sender);
	AddPage(&m_receive);
}

CSheetBase::~CSheetBase()
{}


BEGIN_MESSAGE_MAP(CSheetBase, CPropertySheet)
END_MESSAGE_MAP()


// CSheetBase 消息处理程序


BOOL CSheetBase::OnInitDialog()
{
	BOOL bResult = CPropertySheet::OnInitDialog();
	GetDlgItem(IDOK)->ShowWindow(SW_HIDE); 

	GetDlgItem(IDHELP)->ShowWindow(SW_HIDE);

	GetDlgItem(IDCANCEL)->ShowWindow(SW_HIDE); 

	GetDlgItem(ID_APPLY_NOW)->ShowWindow(SW_HIDE);

	// TODO:  在此添加您的专用代码
	SetActivePage(&m_receive);
	SetActivePage(&m_sender);

	return bResult;
}
