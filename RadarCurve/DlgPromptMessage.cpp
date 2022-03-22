// DlgPromptMessage.cpp : ʵ���ļ�
//

#include "stdafx.h"

#include "DlgPromptMessage.h"


// CDlgPromptMessage �Ի���

IMPLEMENT_DYNAMIC(CDlgPromptMessage, CDialog)

CDlgPromptMessage::CDlgPromptMessage(CWnd* pParent /*=NULL*/)
: CDialog(CDlgPromptMessage::IDD, pParent)
{

	m_strTite = _T("��ʾ��Ϣ");
	m_strInfo = _T("����");
	m_strOK = _T("ȷ��");
	m_strCancle = _T("ȡ��");
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


// CDlgPromptMessage ��Ϣ�������

void CDlgPromptMessage::SetPromptMessage(CString _strTite,CString _strInfo,CString _strOK,CString _strCancle)
{
	m_strTite = _strTite;
	m_strInfo = _strInfo;
	m_strOK = _strOK;
	m_strCancle = _strCancle;
}

void CDlgPromptMessage::OnBnClickedBtnPromptOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnOK();
}

void CDlgPromptMessage::OnBnClickedBtnPromptCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnCancel();
}

BOOL CDlgPromptMessage::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

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
	// �쳣: OCX ����ҳӦ���� FALSE
}
