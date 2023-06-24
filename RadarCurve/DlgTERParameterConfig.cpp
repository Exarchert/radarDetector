// DlgTERParameterConfig.cpp : 实现文件

#include "stdafx.h"
#include "DlgTERParameterConfig.h"
#include "RadarCurve/RadarManager.h"
#include "RadarCurve/ConfigureSet.h"

#include <vector>
#include <sstream>


// DlgTERParameterConfig 对话框
IMPLEMENT_DYNAMIC(DlgTERParameterConfig, CDialog)

DlgTERParameterConfig::DlgTERParameterConfig(CWnd* pParent /*=NULL*/)
	: CDialog(DlgTERParameterConfig::IDD, pParent)
	, m_gpsEdit(_T("160"))
	, m_wheelEdit(_T("30"))
	, m_perimeterEdit(_T("0.500"))
	, m_cstrSenderDelayEdit(_T("0"))
	, m_dutyEdit(_T("10"))

	, m_channelEdit(_T("330"))
	, m_cstrReceiveDelayEdit(_T("0"))
	, m_stackFoldEdit(_T("0"))
{
	
}

/*DlgTERParameterConfig::~DlgTERParameterConfig()
{

}*/

void DlgTERParameterConfig::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_GPSEDIT, m_gpsEdit);
	DDX_Text(pDX, IDC_WHEELEDIT, m_wheelEdit);
	DDX_Text(pDX, IDC_PERIMETEREDIT, m_perimeterEdit);
	DDX_Control(pDX, IDC_COMBO_TER_WHEELPULSE, m_comboxTERWheelPulse );//脉冲
	DDX_Control(pDX, IDC_COMBO_SENDER_WAVE, m_comboSenderWave);
	DDX_Control(pDX, IDC_HZCOMBO, m_hzCombo);
	DDX_Control(pDX, IDC_COILCOMBO, m_coilCombo);
	DDX_Text(pDX, IDC_EDIT_SENDERDELAY, m_cstrSenderDelayEdit);
	DDX_Text(pDX, IDC_DUTYEDIT, m_dutyEdit);
	//DDX_Text(pDX, IDC_STACKEDIT, m_stackingFoldEdit);

	DDX_Control(pDX, IDC_SAMPCOMBO, m_sampCombo);
	DDX_Control(pDX, IDC_GAINCOMBO, m_gainCombo);
	DDX_Text(pDX, IDC_CHANNELEDIT, m_channelEdit);
	DDX_Text(pDX, IDC_EDIT_RECEIVERDELAY, m_cstrReceiveDelayEdit);
	DDX_Text(pDX, IDC_STACKFOLDEDIT, m_stackFoldEdit);

	DDX_Control(pDX, IDC_TER_PARA_WND, m_TERParaWnd);
}


BEGIN_MESSAGE_MAP(DlgTERParameterConfig, CDialog)
	//ON_WM_PAINT()
	ON_WM_CLOSE()
	
	ON_BN_CLICKED(IDC_BUTTON_TER_SAVEPARA, &DlgTERParameterConfig::OnBnClickedTERSaveParaButton)
	ON_BN_CLICKED(IDC_BUTTON_TER_SOCKETCONNECT, &DlgTERParameterConfig::OnBnClickedTERSocketConnectButton)
	ON_BN_CLICKED(IDC_BUTTON_TER_SOCKETDISCONNECT, &DlgTERParameterConfig::OnBnClickedTERSocketDisconnectButton)

	ON_BN_CLICKED(IDC_SYNSENDBUTTON, &DlgTERParameterConfig::OnBnClickedSynButton)
	ON_BN_CLICKED(IDC_HAND_RADIO, &DlgTERParameterConfig::OnBnClickedHandRadio)
	ON_BN_CLICKED(IDC_GPS_RADIO, &DlgTERParameterConfig::OnBnClickedGpsRadio)
	ON_BN_CLICKED(IDC_METER_RADIO, &DlgTERParameterConfig::OnBnClickedMeterRadio)
	ON_BN_CLICKED(IDC_READYSEND_BUTTON, &DlgTERParameterConfig::OnBnClickedReadyButton)
	ON_BN_CLICKED(IDC_STARTSEND_BUTTON, &DlgTERParameterConfig::OnBnClickedStartButton)
	ON_BN_CLICKED(IDC_STOPSEND_BUTTON, &DlgTERParameterConfig::OnBnClickedStopButton)

	ON_BN_CLICKED(IDC_BUTTON_RECEIVE_SYN, &DlgTERParameterConfig::OnBnClickedSynreceiveButton)
	ON_BN_CLICKED(IDC_READYRECEIVE_BUTTON, &DlgTERParameterConfig::OnBnClickedReadyreceiveButton)
	ON_BN_CLICKED(IDC_STARTRECEIVE_BUTTON, &DlgTERParameterConfig::OnBnClickedStartreceiveButton)
	ON_BN_CLICKED(IDC_BUTTON_RECEIVE_STOP, &DlgTERParameterConfig::OnBnClickedStopreceiveButton)
	ON_CBN_SELCHANGE(IDC_HZCOMBO, &DlgTERParameterConfig::OnCbnSelchangeHzcombo)
	ON_BN_CLICKED(IDC_BUTTON_TIMEWINDOW, &DlgTERParameterConfig::OnBnClickedButtonTimewindow)
END_MESSAGE_MAP()


// DlgTERParameterConfig 消息处理程序


wchar_t* charToLPCTSTR2(const char *cTemp){
	int num = MultiByteToWideChar(0,0,cTemp,-1,NULL,0);//获得长字节所需的空间;
	wchar_t *wTemp = new wchar_t[num];//LPCTSTR;
	MultiByteToWideChar(0,0,cTemp,-1,wTemp,num);//char* 转换成 LPCTSTR
	return wTemp;
}

BOOL DlgTERParameterConfig::OnInitDialog()
{
	CDialog::OnInitDialog();

	ConfigureSet* pConfigureSet = RadarManager::Instance()->getConfigureSet();

	std::string str = "";
	// TODO:  在此添加额外的初始化
	//send
	int nPrecRatioOptionCount=atoi(pConfigureSet->get("comboBox", "precRatioOptionCount").c_str());
	for(int i=0;i<nPrecRatioOptionCount;i++){
		std::stringstream ss;
		ss<<i;
		std::string strTag="precRatio"+ss.str();
		m_comboxTERWheelPulse.AddString(charToLPCTSTR2(pConfigureSet->get("comboBox", strTag).c_str()));
	}
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

	CString strWave[] = {_T("双极性方波"),_T("微分脉冲")};
	for (int i = 0; i < (sizeof(strWave)/sizeof(CString)); i++){
		m_comboSenderWave.InsertString(i,strWave[i]);

	}
	m_comboSenderWave.SetCurSel(0);

	
	int nSelect=atoi(pConfigureSet->get("send","mode").c_str());
	if(nSelect==0){
		((CButton *)GetDlgItem(IDC_HAND_RADIO))->SetCheck(TRUE);
	}else if(nSelect==1){
		((CButton *)GetDlgItem(IDC_GPS_RADIO))->SetCheck(TRUE);
	}else if(nSelect==2){
		((CButton *)GetDlgItem(IDC_METER_RADIO))->SetCheck(TRUE);
	}
	
	str = pConfigureSet->get("send","GPS");
	if(!str.empty())
	{
		m_gpsEdit = CString(str.c_str());
		str = pConfigureSet->get("send","measuringWheelPointPerMeter");
		m_wheelEdit = CString(str.c_str());
		str = pConfigureSet->get("send", "wheelPulse");
		m_comboxTERWheelPulse.SetCurSel(atoi(str.c_str()));//脉冲
		str = pConfigureSet->get("send","waveType");
		m_comboSenderWave.SetCurSel(atoi(str.c_str()));
		str = pConfigureSet->get("send","frequency");
		m_hzCombo.SetCurSel(atoi(str.c_str()));
		str = pConfigureSet->get("send","coil");
		m_coilCombo.SetCurSel(atoi(str.c_str()));
		str = pConfigureSet->get("send","delay");
		m_cstrSenderDelayEdit = CString(str.c_str());
		str = pConfigureSet->get("send","dutyRatio");
		m_dutyEdit = CString(str.c_str());
		str = pConfigureSet->get("send","perimeter");
		m_perimeterEdit = CString(str.c_str());
		//str = pConfigureSet->get("send","stackingFold");
		//m_stackingFoldEdit = CString(str.c_str());
		
		//UpdateData(false);
	}

	//recieve
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

	str = pConfigureSet->get("receive","samplingRate");
	if(!str.empty())
	{
		m_sampCombo.SetCurSel(atoi(str.c_str()));
		str = pConfigureSet->get("receive","sampleCount");
		m_channelEdit = CString(str.c_str());
		str = pConfigureSet->get("receive","delay");
		m_cstrReceiveDelayEdit = CString(str.c_str());
		str = pConfigureSet->get("receive","stackingFold");
		m_stackFoldEdit = CString(str.c_str());
		str = pConfigureSet->get("receive","gain");
		m_gainCombo.SetCurSel(atoi(str.c_str()));
		//UpdateData(false);
	}
	
	RefreshTimeWindow();

	UpdateData(false);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

//void DlgTERParameterConfig::OnPaint()
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

void DlgTERParameterConfig::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	RadarManager::Instance()->TERSocketDisconnect();

	EndDialog(IDCANCEL);
	CDialog::OnClose();
}

void DlgTERParameterConfig::setConfigure()
{
	//infoMap[_T("叠加次数")] = m_gpsEdit;
	UpdateData(true);
	ConfigureSet* pConfigureSet = RadarManager::Instance()->getConfigureSet();

	
	std::string strTemp;
	//send
	if(((CButton *)GetDlgItem(IDC_HAND_RADIO))->GetCheck()){
		pConfigureSet->set("send","mode","0");
	}else if(((CButton *)GetDlgItem(IDC_GPS_RADIO))->GetCheck()){
		pConfigureSet->set("send","mode","1");
	}else if(((CButton *)GetDlgItem(IDC_METER_RADIO))->GetCheck()){
		pConfigureSet->set("send","mode","2");
	}

	strTemp = CT2A(m_gpsEdit.GetString());
	pConfigureSet->set("send","GPS",strTemp);
	strTemp = CT2A(m_wheelEdit.GetString());
	pConfigureSet->set("send","measuringWheelPointPerMeter",strTemp);
	strTemp = CT2A(m_perimeterEdit.GetString());
	pConfigureSet->set("send","perimeter",strTemp);
	{
		std::stringstream ss;
		ss << m_comboxTERWheelPulse.GetCurSel();
		pConfigureSet->set("send", "wheelPulse", ss.str());//脉冲 0:720  1:512
	}
	{
		std::stringstream ss;
		ss << m_hzCombo.GetCurSel();
		pConfigureSet->set("send","frequency",ss.str());
	}
	{
		std::stringstream ss;
		ss << m_comboSenderWave.GetCurSel();
		pConfigureSet->set("send","waveType",ss.str());
	}
	strTemp = CT2A(m_cstrSenderDelayEdit.GetString());
	pConfigureSet->set("send","delay",strTemp);
	{
		std::stringstream ss;
		ss << m_coilCombo.GetCurSel();
		pConfigureSet->set("send","coil",ss.str());
	}
	strTemp = CT2A(m_dutyEdit.GetString());
	pConfigureSet->set("send","dutyRatio",strTemp);	
	//strs = CT2A(m_stackingFoldEdit.GetString());
	//pConfigureSet->set("send","stackingFold",strs);	

	//recieve
	{
		std::stringstream ss;
		ss << m_sampCombo.GetCurSel();
		pConfigureSet->set("receive","samplingRate",ss.str());
	}
	strTemp = CT2A(m_channelEdit.GetString());
	pConfigureSet->set("receive","sampleCount",strTemp);
	strTemp = CT2A(m_cstrReceiveDelayEdit.GetString());
	pConfigureSet->set("receive","delay",strTemp);
	strTemp = CT2A(m_stackFoldEdit.GetString());
	pConfigureSet->set("receive","stackingFold",strTemp);
	{
		std::stringstream ss;
		ss << m_gainCombo.GetCurSel();
		pConfigureSet->set("receive","gain",ss.str());
	}

	pConfigureSet->write();
	RefreshTimeWindow();
}

//更新波形显示窗口
void DlgTERParameterConfig::RefreshTimeWindow(){
	m_TERParaWnd.EndDraw();
	m_TERParaWnd.ShowWindow( SW_HIDE );
	float fArrFrequency[25]={0.0625,0.125,0.25,0.5,0.83,1,1.25,1.67,2,2.5,4,5,6.25,8,8.3,10,12.5,16,25,32,50,62.5,75,125,222};
	float fCurrentFreuency=fArrFrequency[m_hzCombo.GetCurSel()];
	m_TERParaWnd.SetSenderFrequency( fCurrentFreuency );
	m_TERParaWnd.SetSenderDutyRatio( atoi(CT2A(m_dutyEdit.GetString())) );
	m_TERParaWnd.SetReceiverSampleCount( atoi(CT2A(m_channelEdit.GetString())) );
	int nArrSampleRatio[19]={52734,26376,17578,13183,10546,8789,7533,6591,5859,5273,4794,4394,4056,3515,2929,2511,2028,1506,1014};
	int nCurrentSampleRatio=nArrSampleRatio[m_sampCombo.GetCurSel()];
	m_TERParaWnd.SetReceiverSampleRatio( nCurrentSampleRatio );
	m_TERParaWnd.SetReceiverDelay( atoi(CT2A(m_cstrReceiveDelayEdit.GetString())) );
	m_TERParaWnd.StartDraw();
	m_TERParaWnd.ShowWindow( SW_SHOW );
}

void DlgTERParameterConfig::OnBnClickedTERSaveParaButton(){
	setConfigure();
}

void DlgTERParameterConfig::OnBnClickedTERSocketConnectButton()
{
	// TODO: 在此添加控件通知处理程序代码
	setConfigure();
	RadarManager::Instance()->TERSocketConnect();
}

void DlgTERParameterConfig::OnBnClickedTERSocketDisconnectButton()
{
	// TODO: 在此添加控件通知处理程序代码
	setConfigure();
	RadarManager::Instance()->TERSocketDisconnect();
}

void DlgTERParameterConfig::OnBnClickedHandRadio()
{
	// TODO: 在此添加控件通知处理程序代码
	GetDlgItem(IDC_WHEELEDIT)->EnableWindow(FALSE);
}

void DlgTERParameterConfig::OnBnClickedGpsRadio()
{
	// TODO: 在此添加控件通知处理程序代码
	GetDlgItem(IDC_WHEELEDIT)->EnableWindow(FALSE);
}

void DlgTERParameterConfig::OnBnClickedMeterRadio()
{
	// TODO: 在此添加控件通知处理程序代码
	GetDlgItem(IDC_WHEELEDIT)->EnableWindow(TRUE);
}

void DlgTERParameterConfig::OnBnClickedSynButton()
{
	// TODO: 在此添加控件通知处理程序代码
	setConfigure();
	RadarManager::Instance()->TERSendParameter(0);
}

void DlgTERParameterConfig::OnBnClickedReadyButton()
{
	// TODO: 在此添加控件通知处理程序代码
	setConfigure();
	RadarManager::Instance()->TERSendParameter(1);
}

void DlgTERParameterConfig::OnBnClickedStartButton()
{
	// TODO: 在此添加控件通知处理程序代码
	setConfigure();
	RadarManager::Instance()->TERSendParameter(2);
}

void DlgTERParameterConfig::OnBnClickedStopButton()
{
	// TODO: 在此添加控件通知处理程序代码
	RadarManager::Instance()->TERSendParameter(3);
}




void DlgTERParameterConfig::OnBnClickedSynreceiveButton()
{
	// TODO: 在此添加控件通知处理程序代码
	setConfigure();
	RadarManager::Instance()->TERSendParameter(4);
	
}

void DlgTERParameterConfig::OnBnClickedReadyreceiveButton()
{
	// TODO: 在此添加控件通知处理程序代码
	setConfigure();
	RadarManager::Instance()->TERSendParameter(5);
}

void DlgTERParameterConfig::OnBnClickedStartreceiveButton()
{
	// TODO: 在此添加控件通知处理程序代码
	setConfigure();
	RadarManager::Instance()->TERSendParameter(6);
}

void DlgTERParameterConfig::OnBnClickedStopreceiveButton()
{
	// TODO: 在此添加控件通知处理程序代码
	RadarManager::Instance()->TERSendParameter(7);
}	

void DlgTERParameterConfig::OnCbnSelchangeHzcombo()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strHz[] = {_T("0.0625"),_T("0.125"),_T("0.25"),_T("0.5"),_T("0.83"),
		               _T("1"),_T("1.25"),_T("1.67"),_T("2"),_T("2.5"),
					   _T("4"),_T("5"),_T("6.25"),_T("8"),_T("8.3"),
	                   _T("10"),_T("12.5"),_T("16"),_T("25"),_T("32"),
	                   _T("50"),_T("62.5"),_T("75"),_T("125"),_T("222")};
	float fArrFrequency[25]={0.0625,0.125,0.25,0.5,0.83,1,1.25,1.67,2,2.5,4,5,6.25,8,8.3,10,12.5,16,25,32,50,62.5,75,125,222};
	float fCurrentFreuency=fArrFrequency[m_hzCombo.GetCurSel()];
	int nGps=1000.0/fCurrentFreuency;
	int nDelay=100.0/fCurrentFreuency;
	std::string strGps;
	std::string strDelay;
	{
		std::stringstream ss;
		ss<<nGps;
		strGps=ss.str();
	}
	{
		std::stringstream ss;
		ss<<nDelay;
		strDelay=ss.str();
	}
	m_gpsEdit = CString(strGps.c_str());
	m_cstrReceiveDelayEdit = CString(strDelay.c_str());
	UpdateData( FALSE );
}

void DlgTERParameterConfig::OnBnClickedButtonTimewindow()
{
	// TODO: 在此添加控件通知处理程序代码
	setConfigure();
}
