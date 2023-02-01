// Receive.cpp : 实现文件
//

#include "stdafx.h"
#include <vector>
#include <sstream>
#include "RadarCurve.h"
#include "Receive.h"
#include "RadarCurve/RadarManager.h"
#include "RadarCurve/ConfigureSet.h"


// CReceive 对话框

IMPLEMENT_DYNAMIC(CReceive, CPropertyPage)

CReceive::CReceive()
	: CPropertyPage(CReceive::IDD)
	, m_channelEdit(_T("330"))
	, m_delayEdit(_T("0"))
	, m_stackFoldEdit(_T("0"))
{

}

CReceive::~CReceive()
{
}

void CReceive::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SAMPCOMBO, m_sampCombo);
	DDX_Control(pDX, IDC_GAINCOMBO, m_gainCombo);
	DDX_Text(pDX, IDC_CHANNELEDIT, m_channelEdit);
	DDX_Text(pDX, IDC_DELAYEDIT, m_delayEdit);
	DDX_Text(pDX, IDC_STACKFOLDEDIT, m_stackFoldEdit);
}


BEGIN_MESSAGE_MAP(CReceive, CPropertyPage)

	ON_BN_CLICKED(IDC_SYNRECEIVE_BUTTON, &CReceive::OnBnClickedSynreceiveButton)
	ON_BN_CLICKED(IDC_READYRECEIVE_BUTTON, &CReceive::OnBnClickedReadyreceiveButton)
	ON_BN_CLICKED(IDC_STARTRECEIVE_BUTTON, &CReceive::OnBnClickedStartreceiveButton)
	ON_BN_CLICKED(IDC_STOPRECEIVE_BUTTON, &CReceive::OnBnClickedStopreceiveButton)
END_MESSAGE_MAP()


// CReceive 消息处理程序




BOOL CReceive::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	// TODO:  在此添加额外的初始化
	CString strSamp[] = {_T("52734"),_T("26376"),_T("17578"),_T("13183"),_T("10546"),_T("8789"),_T("7533"),_T("6591"),_T("5859"),_T("5273"),_T("4794"),_T("4394"),_T("4056"),_T("3515"),_T("2929"),_T("2511"),_T("2028"),_T("1506"),_T("1014")}; 
	for(int i = 0;i<(sizeof(strSamp)/sizeof(CString));i++)
	{
		m_sampCombo.InsertString(i,strSamp[i]);
	}
	m_sampCombo.SetCurSel(4);
	CString strGain[] = {_T("1"),_T("2"),_T("4"),_T("8"),_T("16"),_T("32"),_T("64"),_T("128"),};
	for(int i = 0;i<(sizeof(strGain)/sizeof(CString));i++)
	{
		m_gainCombo.InsertString(i,strGain[i]);
	}
	m_gainCombo.SetCurSel(0);

	ConfigureSet* pConfigureSet = RadarManager::Instance()->getConfigureSet();
	std::string str = "";
	str = pConfigureSet->get("receive","samplingRate");
	if(!str.empty())
	{
		m_sampCombo.SetCurSel(atoi(str.c_str()));
		str = pConfigureSet->get("receive","sampleCount");
		m_channelEdit = CString(str.c_str());
		str = pConfigureSet->get("receive","delay");
		m_delayEdit = CString(str.c_str());
		str = pConfigureSet->get("receive","stackingFold");
		m_stackFoldEdit = CString(str.c_str());
		str = pConfigureSet->get("receive","gain");
		m_gainCombo.SetCurSel(atoi(str.c_str()));
		UpdateData(false);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CReceive::setConfigure()
{
	UpdateData(true);
	ConfigureSet* pConfigureSet = RadarManager::Instance()->getConfigureSet();

	std::stringstream str;
	str << m_sampCombo.GetCurSel();
	pConfigureSet->set("receive","samplingRate",str.str());
	std::string strs = CT2A(m_channelEdit.GetString());
	pConfigureSet->set("receive","sampleCount",strs);
	strs = CT2A(m_delayEdit.GetString());
	pConfigureSet->set("receive","delay",strs);
	strs = CT2A(m_stackFoldEdit.GetString());
	pConfigureSet->set("receive","stackingFold",strs);
	std::stringstream strGain;
	strGain << m_gainCombo.GetCurSel();
	pConfigureSet->set("receive","gain",strGain.str());
}

void CReceive::OnBnClickedSynreceiveButton()
{
	// TODO: 在此添加控件通知处理程序代码
	setConfigure();
	RadarManager::Instance()->TERSendParameter(4);
	
}

void CReceive::OnBnClickedReadyreceiveButton()
{
	// TODO: 在此添加控件通知处理程序代码
	setConfigure();
	RadarManager::Instance()->TERSendParameter(5);
}

void CReceive::OnBnClickedStartreceiveButton()
{
	// TODO: 在此添加控件通知处理程序代码
	setConfigure();
	RadarManager::Instance()->TERSendParameter(6);
}

void CReceive::OnBnClickedStopreceiveButton()
{
	// TODO: 在此添加控件通知处理程序代码
	RadarManager::Instance()->TERSendParameter(7);
}	
