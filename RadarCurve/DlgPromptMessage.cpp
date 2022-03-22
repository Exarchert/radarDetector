// DlgPromptMessage.cpp : 实现文件
//

#include "stdafx.h"

#include "DlgPromptMessage.h"


// CDlgPromptMessage 对话框

IMPLEMENT_DYNAMIC(CDlgPromptMessage, CDialog)

CDlgPromptMessage::CDlgPromptMessage(CWnd* pParent /*=NULL*/)
: CDialog(CDlgPromptMessage::IDD, pParent)
{

	m_strTite = _T("提示信息");
	m_strInfo = _T("警告");
	m_strOK = _T("确定");
	m_strCancle = _T("取消");
}

CDlgPromptMessage::~CDlgPromptMessage()
{
}

void CDlgPromptMessage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgPromptMessage, CDialog)
	ON_BN_CLICKED(IDC_BTN_PROMPT_OK, &CDlgPromptMessage::OnBnClickedBtnPromptOk)
	ON_BN_CLICKED(IDC_BTN_PROMPT_CANCEL, &CDlgPromptMessage::OnBnClickedBtnPromptCancel)
END_MESSAGE_MAP()


// CDlgPromptMessage 消息处理程序

void CDlgPromptMessage::SetPromptMessage(CString _strTite,CString _strInfo,CString _strOK,CString _strCancle)
{
	m_strTite = _strTite;
	m_strInfo = _strInfo;
	m_strOK = _strOK;
	m_strCancle = _strCancle;
}

void CDlgPromptMessage::OnBnClickedBtnPromptOk()
{
	// TODO: 在此添加控件通知处理程序代码
	OnOK();
}

void CDlgPromptMessage::OnBnClickedBtnPromptCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	OnCancel();
}

BOOL CDlgPromptMessage::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化

	CWnd* lwnd = NULL;
	SetWindowText(m_strTite);
	lwnd = this->GetDlgItem(IDC_STA_PROMPT_INFO);
	if (lwnd)
	{
		lwnd->SetWindowText(m_strInfo);
	}
	lwnd = this->GetDlgItem(IDC_BTN_PROMPT_OK);
	if (lwnd)
	{
		lwnd->SetWindowText(m_strOK);
	}
	lwnd = this->GetDlgItem(IDC_BTN_PROMPT_CANCEL);
	if (lwnd)
	{
		lwnd->SetWindowText(m_strCancle);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
