#pragma once
#include "Receive.h"
#include "Sender.h"

// CSheetBase

class CSheetBase : public CPropertySheet
{
	DECLARE_DYNAMIC(CSheetBase)

public:
	CSheetBase(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CSheetBase(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	virtual ~CSheetBase();

protected:
	DECLARE_MESSAGE_MAP()

public:
	CReceive m_receive;
	CSender m_sender;
	virtual BOOL OnInitDialog();
};


