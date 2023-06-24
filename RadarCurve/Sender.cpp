// Sender.cpp : 实现文件
//

#include "stdafx.h"
#include "RadarCurve.h"
#include "Sender.h"
#include "RadarCurve/RadarManager.h"
#include "RadarCurve/ConfigureSet.h"
#include <sstream>


// CSender 对话框

IMPLEMENT_DYNAMIC(CSender, CPropertyPage)

CSender::CSender()
	: CPropertyPage(CSender::IDD)
	, m_gpsEdit(_T("160"))
	, m_wheelEdit(_T("30"))
	, m_perimeterEdit(_T("0.500"))
	, m_delayEdit(_T("0"))
	, m_dutyEdit(_T("10"))
{

}

CSender::~CSender()
{
}

void CSender::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_GPSEDIT, m_gpsEdit);
	DDX_Text(pDX, IDC_WHEELEDIT, m_wheelEdit);
	DDX_Text(pDX, IDC_PERIMETEREDIT, m_perimeterEdit);
	DDX_Control(pDX, IDC_HZCOMBO, m_hzCombo);
	DDX_Control(pDX, IDC_COILCOMBO, m_coilCombo);
	DDX_Text(pDX, IDC_DELAYEDIT, m_delayEdit);
	DDX_Text(pDX, IDC_DUTYEDIT, m_dutyEdit);
	DDX_Text(pDX, IDC_STACKEDIT, m_stackingFoldEdit);
	
}


BEGIN_MESSAGE_MAP(CSender, CPropertyPage)
	//ON_WM_PAINT()
	ON_BN_CLICKED(IDC_SYNSENDBUTTON, &CSender::OnBnClickedSynButton)
	ON_BN_CLICKED(IDC_HAND_RADIO, &CSender::OnBnClickedHandRadio)
	ON_BN_CLICKED(IDC_GPS_RADIO, &CSender::OnBnClickedGpsRadio)
	ON_BN_CLICKED(IDC_METER_RADIO, &CSender::OnBnClickedMeterRadio)
	ON_BN_CLICKED(IDC_READYSEND_BUTTON, &CSender::OnBnClickedReadyButton)
	ON_BN_CLICKED(IDC_STARTSEND_BUTTON, &CSender::OnBnClickedStartButton)
	ON_BN_CLICKED(IDC_STOPSEND_BUTTON, &CSender::OnBnClickedStopButton)
END_MESSAGE_MAP()


// CSender 消息处理程序


BOOL CSender::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	// TODO:  在此添加额外的初始化

	CString strFont[] = { _T("道路天线"),_T("结构天线"),_T("隧道天线"),_T("其他天线") };
	for (int i = 0; i < (sizeof(strFont)/sizeof(CString)); i++)
	{
		m_coilCombo.InsertString(i,strFont[i]);

	}
	m_coilCombo.SetCurSel(0);
	CString strHz[] = {_T("0.0625"),_T("0.125"),_T("0.25"),_T("0.5"),_T("0.83"),
		               _T("1"),_T("1.25"),_T("1.67"),_T("2"),_T("2.5"),
					   _T("4"),_T("5"),_T("6.25"),_T("8"),_T("8.3"),
	                   _T("10"),_T("12.5"),_T("16"),_T("25"),_T("32"),
	                   _T("50"),_T("62.5"),_T("75"),_T("125"),_T("222")};
	for (int i = 0; i < (sizeof(strHz)/sizeof(CString)); i++)
	{
		m_hzCombo.InsertString(i,strHz[i]);

	}
	m_hzCombo.SetCurSel(12);

	ConfigureSet* pConfigureSet = RadarManager::Instance()->getConfigureSet();
	int nSelect=atoi(pConfigureSet->get("send","mode").c_str());
	if(nSelect==0){
		((CButton *)GetDlgItem(IDC_HAND_RADIO))->SetCheck(TRUE);
	}else if(nSelect==1){
		((CButton *)GetDlgItem(IDC_GPS_RADIO))->SetCheck(TRUE);
	}else if(nSelect==2){
		((CButton *)GetDlgItem(IDC_METER_RADIO))->SetCheck(TRUE);
	}
	std::string str = "";
	str = pConfigureSet->get("send","GPS");
	if(!str.empty())
	{
		m_gpsEdit = CString(str.c_str());
		str = pConfigureSet->get("send","measuringWheelPointPerMeter");
		m_wheelEdit = CString(str.c_str());
		str = pConfigureSet->get("send","frequency");
		m_hzCombo.SetCurSel(atoi(str.c_str()));
		str = pConfigureSet->get("send","coil");
		int index = atoi(str.c_str());
		m_coilCombo.SetCurSel(atoi(str.c_str()));
		str = pConfigureSet->get("send","delay");
		m_delayEdit = CString(str.c_str());
		str = pConfigureSet->get("send","dutyRatio");
		m_dutyEdit = CString(str.c_str());
		str = pConfigureSet->get("send","perimeter");
		m_perimeterEdit = CString(str.c_str());
		str = pConfigureSet->get("send","stackingFold");
		m_stackingFoldEdit = CString(str.c_str());
		
		UpdateData(false);

	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

//void CSender::OnPaint()
//{
//	CPaintDC dc(this); // device context for painting
//	// TODO: 在此处添加消息处理程序代码
//	// 不为绘图消息调用 CPropertyPage::OnPaint()
//	CPoint start;
//	CPoint end;
//
//	CPen cPen;//创建画笔
//	cPen.CreatePen(PS_SOLID, 1, RGB(0,200,0));
//	dc.SelectObject(&cPen);
//	start.x = 300;
//	start.y = 80;
//	end.x = 300;
//	end.y = 500;
//	dc.MoveTo(start);
//	dc.LineTo(end);
//
//}

void CSender::setConfigure()
{
	//infoMap[_T("叠加次数")] = m_gpsEdit;
	UpdateData(true);
	ConfigureSet* pConfigureSet = RadarManager::Instance()->getConfigureSet();

	if(((CButton *)GetDlgItem(IDC_HAND_RADIO))->GetCheck()){
		pConfigureSet->set("send","mode","0");
	}else if(((CButton *)GetDlgItem(IDC_GPS_RADIO))->GetCheck()){
		pConfigureSet->set("send","mode","1");
	}else if(((CButton *)GetDlgItem(IDC_METER_RADIO))->GetCheck()){
		pConfigureSet->set("send","mode","2");
	}

	std::string strs = CT2A(m_gpsEdit.GetString());
	pConfigureSet->set("send","GPS",strs);
	strs = CT2A(m_wheelEdit.GetString());
	pConfigureSet->set("send","measuringWheelPointPerMeter",strs);
	strs = CT2A(m_perimeterEdit.GetString());
	pConfigureSet->set("send","perimeter",strs);
	
	std::stringstream str;
	str << m_hzCombo.GetCurSel();
	pConfigureSet->set("send","frequency",str.str());
	strs = CT2A(m_delayEdit.GetString());
	pConfigureSet->set("send","delay",strs);
	std::stringstream strCoci;
	strCoci << m_coilCombo.GetCurSel();
	pConfigureSet->set("send","coil",strCoci.str());
	strs = CT2A(m_dutyEdit.GetString());
	pConfigureSet->set("send","dutyRatio",strs);	
	strs = CT2A(m_stackingFoldEdit.GetString());
	pConfigureSet->set("send","stackingFold",strs);	

}




void CSender::OnBnClickedHandRadio()
{
	// TODO: 在此添加控件通知处理程序代码
	GetDlgItem(IDC_WHEELEDIT)->EnableWindow(FALSE);
}

void CSender::OnBnClickedGpsRadio()
{
	// TODO: 在此添加控件通知处理程序代码
	GetDlgItem(IDC_WHEELEDIT)->EnableWindow(FALSE);
}

void CSender::OnBnClickedMeterRadio()
{
	// TODO: 在此添加控件通知处理程序代码
	GetDlgItem(IDC_WHEELEDIT)->EnableWindow(TRUE);
}

void CSender::OnBnClickedSynButton()
{
	// TODO: 在此添加控件通知处理程序代码
	setConfigure();
	RadarManager::Instance()->TERSendParameter(0);
}

void CSender::OnBnClickedReadyButton()
{
	// TODO: 在此添加控件通知处理程序代码
	setConfigure();
	RadarManager::Instance()->TERSendParameter(1);
}

void CSender::OnBnClickedStartButton()
{
	// TODO: 在此添加控件通知处理程序代码
	setConfigure();
	RadarManager::Instance()->TERSendParameter(2);
}

void CSender::OnBnClickedStopButton()
{
	// TODO: 在此添加控件通知处理程序代码
	RadarManager::Instance()->TERSendParameter(3);
}
