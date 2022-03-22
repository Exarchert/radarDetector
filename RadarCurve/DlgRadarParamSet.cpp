// DialogRadarParam.cpp : 实现文件
//

#include "stdafx.h"
#include "RadarCurve.h"
#include "DlgRadarParamSet.h"

#include <RadarCurve/ConfigureSet.h>
#include <RadarCurve/RadarManager.h>
#include <WinSock2.h>
#include <sstream>
#include <RadarCurve/RadarDataRader.h>
// DlgRadarParamSet 对话框

IMPLEMENT_DYNAMIC(DlgRadarParamSet, CDialog)

DlgRadarParamSet::DlgRadarParamSet(CWnd* pParent /*=NULL*/)
	: CDialog(DlgRadarParamSet::IDD, pParent)
	, m_serverPort(5050)
	, m_id(L"0X88")
	, m_serverIP(L"127.0.0.1")
	, m_precLen(0.0)
	,m_precRatio(720)

	, m_LeftScreenChannels(_T(""))
	, m_RightScreenChannels(_T(""))
{

}

DlgRadarParamSet::~DlgRadarParamSet()
{
}

void DlgRadarParamSet::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_SERVER, m_serverIP);
	DDX_Control(pDX, IDC_COMBO_SAMPLE, m_sampleComboBox);
	DDX_Control(pDX, IDC_COMBO_SAMPLE_RATIO, m_sampleRatioComboBox);
	DDX_Control(pDX, IDC_COMBO_GAIN, m_gainComboBox);
	DDX_Control(pDX, IDC_SLIDER_GAIN_VALUE, m_gainValueSlider);
	DDX_Control(pDX, IDC_SLIDER_DELAY, m_delaySlider);
	DDX_Control(pDX, IDC_COMBO_ADD_COUNT, m_addCountComboBox);//累加次数
	DDX_Control(pDX, IDC_COMBO_TEST_TYPE, m_testTypeComboBox);
	DDX_Control(pDX, IDC_EDIT_GAIN_VALUE, m_gainValue );
	DDX_Control(pDX, IDC_EDIT_DELAY, m_delayValue );
	DDX_Text(pDX, IDC_EDIT_RADAR_ID, m_id );
	DDX_Text(pDX, IDC_EDIT_PORT, m_serverPort );	
	DDX_Text(pDX, IDC_EDIT_CHANNEL_COUNT, m_channelCount );
	DDX_Text(pDX, IDC_EDIT_DB_SOURCE, m_dbSource );
	DDX_Text(pDX, IDC_EDIT_DB_NAME, m_dbName );
	DDX_Text(pDX, IDC_EDIT_USER_NAME, m_dbUser );
	DDX_Text(pDX, IDC_EDIT_PWD, m_dbPwd );
	DDX_Text(pDX, IDC_EDIT_CELIANGLUN_LEN, m_precLen );
	DDX_Text(pDX, IDC_EDIT_CELIANGLUN_RATIO, m_precRatio );//脉冲
	DDX_Control(pDX, IDC_COMBO_CELIANLUN_JINGDU, m_precIndex);
	DDX_Text(pDX, IDC_EDIT_LeftScreen, m_LeftScreenChannels);
	DDV_MaxChars(pDX, m_LeftScreenChannels, 20);
	DDX_Text(pDX, IDC_EDIT_RightScreen, m_RightScreenChannels);
	DDV_MaxChars(pDX, m_RightScreenChannels, 20);
}


BEGIN_MESSAGE_MAP(DlgRadarParamSet, CDialog)
	ON_BN_CLICKED(ID_BTNSET_OK, &DlgRadarParamSet::OnBnClickedBtnsetOk)
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_BUTTON_APPLY, &DlgRadarParamSet::OnBnClickedButtonApply)
	ON_BN_CLICKED(ID_BTNSET_CANCEL, &DlgRadarParamSet::OnBnClickedBtnsetCancel)
END_MESSAGE_MAP()


// DlgRadarParamSet 消息处理程序

BOOL DlgRadarParamSet::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_sampleComboBox.AddString(L"256");
	m_sampleComboBox.AddString(L"512");
	m_sampleComboBox.AddString(L"1024");
	m_sampleComboBox.AddString(L"2048");
	switch (RadarManager::Instance()->GetRadarWorkType())
	{
	case RADAR_WORK_TYPE_ONE_USB:
		m_sampleRatioComboBox.AddString(L"25.6G");
		m_sampleRatioComboBox.AddString(L"12.8G");
		m_sampleRatioComboBox.AddString(L"6.4G");
		m_sampleRatioComboBox.AddString(L"3.2G");
		m_sampleRatioComboBox.AddString(L"1.6G");
		m_sampleRatioComboBox.AddString(L"0.8G");
		break;
	case RADAR_WORK_TYPE_DOUBLE_USB:
		m_sampleRatioComboBox.AddString(L"20.0G");
		m_sampleRatioComboBox.AddString(L"12.0G");
		m_sampleRatioComboBox.AddString(L"8.0G");
		m_sampleRatioComboBox.AddString(L"6.4G");
		break;
	case RADAR_WORK_TYPE_FOUR_USB:
		m_sampleRatioComboBox.AddString(L"16.0G");
		m_sampleRatioComboBox.AddString(L"8.0G");
		m_sampleRatioComboBox.AddString(L"4.0G");
		break;
	case RADAR_WORK_TYPE_EIGHT:
		m_sampleRatioComboBox.AddString(L"51.2G");
		m_sampleRatioComboBox.AddString(L"25.6G");
		m_sampleRatioComboBox.AddString(L"12.8G");
		m_sampleRatioComboBox.AddString(L"6.4G");
		break;
	default:
		break;
	}
	m_gainComboBox.AddString(L"固定增益");
	m_gainComboBox.AddString(L"变增益模式0");
	m_gainComboBox.AddString(L"变增益模式1");

	m_addCountComboBox.AddString(L"1");
	m_addCountComboBox.AddString(L"2");
	m_addCountComboBox.AddString(L"4");
	m_addCountComboBox.AddString(L"8");
	m_addCountComboBox.AddString(L"16");
	m_addCountComboBox.AddString(L"32");
	m_addCountComboBox.AddString(L"64");
	m_addCountComboBox.AddString(L"128");
	m_addCountComboBox.AddString(L"256");

	m_testTypeComboBox.AddString(L"时间触发");
	m_testTypeComboBox.AddString(L"距离触发");

	m_precIndex.AddString(L"1cm");
	m_precIndex.AddString(L"2cm");
	m_precIndex.AddString(L"5cm");
	m_precIndex.AddString(L"10cm");
	m_precIndex.AddString(L"20cm");

	Init();


	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void DlgRadarParamSet::Init()
{
	ConfigureSet *cs = RadarManager::Instance()->getConfigureSet();
	if ( !cs )
	{
		return;
	}

	USES_CONVERSION;
	m_serverIP = A2W(cs->get("net", "addr").c_str());


	m_serverPort = atoi ( cs->get("net", "port").c_str() );


	m_channelCount = atoi ( cs->get("radar", "count").c_str());


	CString idText;
	idText.Format(L"0X%X", atoi( cs->get("radar", "id").c_str()));
	m_id = idText;
	m_sampleComboBox.SetCurSel( atoi ( cs->get("radar", "sample").c_str() ) );
	std::string sampleratioStr = cs->get("radar", "sampleratio");
	m_sampleRatioComboBox.SetCurSel( atoi(sampleratioStr.c_str()) );
	m_gainComboBox.SetCurSel(atoi(cs->get("radra", "gain").c_str()));
	m_gainValueSlider.SetRangeMin( 0 );
	m_gainValueSlider.SetRangeMax( 255 );
	m_gainValueSlider.SetPos( atoi( cs->get("radar", "gainvalue").c_str() ) );
	m_gainValue.SetWindowText( A2W( cs->get("radar", "gainvalue").c_str() ) );
	m_delaySlider.SetRangeMin( 0 );
	m_delaySlider.SetRangeMax( 255 );
	m_delaySlider.SetPos( atoi( cs->get("radar", "delay").c_str()));
	m_delayValue.SetWindowText ( A2W ( cs->get("radar", "delay").c_str() ) );
	m_addCountComboBox.SetCurSel( atoi ( cs->get("radar", "addcount").c_str()) );
	m_testTypeComboBox.SetCurSel( atoi ( cs->get("radar", "testtype").c_str()) );

	m_precIndex.SetCurSel( atoi ( cs->get("radar", "precindex").c_str()));
	m_precLen = atof ( cs->get("radar", "preclen").c_str() );
	m_precRatio = atoi ( cs->get("radar", "precratio").c_str());



	m_dbSource = A2W( cs->get("db", "dbsource").c_str());
	m_dbName = A2W( cs->get("db", "dbname").c_str());
	m_dbUser = A2W( cs->get("db", "user").c_str());
	m_dbPwd = A2W( cs->get("db", "pwd").c_str());

	m_LeftScreenChannels = A2W(cs->get("screen","left").c_str());
	m_RightScreenChannels = A2W(cs->get("screen","right").c_str());
	UpdateData( FALSE );

}
void DlgRadarParamSet::OnBnClickedBtnsetOk()
{
	// TODO: 在此添加控件通知处理程序代码
	OpenThreads::ScopedLock<OpenThreads::Mutex> lock(_lock);

	UpdateData( TRUE );

	ConfigureSet *cs = RadarManager::Instance()->getConfigureSet();
	if ( !cs )
	{
		return;
	}
	USES_CONVERSION;
	{
		cs->set( "net", "addr", W2A( m_serverIP) );
	}


	{
		std::stringstream ss;
		ss << m_serverPort;
		cs->set("net", "port", ss.str());
	}

	{

		std::stringstream ss;
		ss << m_channelCount;
		cs->set("radar", "count", ss.str());
	}

	{

		std::stringstream ss;
		ss << 0X88;
		cs->set("radar", "id", ss.str());
	}
	{
		std::stringstream ss;
		ss << m_sampleComboBox.GetCurSel();
		cs->set("radar", "sample", ss.str());
	}
	{
		std::stringstream ss;
		ss << m_sampleRatioComboBox.GetCurSel();
		std::string tempStr = ss.str();
		std::string sampleratioStr = cs->get("radar", "sampleratio");
		cs->set("radar", "sampleratio", ss.str());
	}
	{
		std::stringstream ss;
		ss << m_gainComboBox.GetCurSel();
		cs->set("radar", "gain", ss.str());
	}
	{
		std::stringstream ss;
		ss << m_gainValueSlider.GetPos();
		cs->set("radar", "gainvalue", ss.str());
	
	}
	{
		std::stringstream ss;
		ss << m_delaySlider.GetPos();
		cs->set("radar", "delay", ss.str());
	}
	{
		std::stringstream ss;
		ss << m_addCountComboBox.GetCurSel();
		cs->set("radar", "addcount", ss.str());
	}
	{
		std::stringstream ss;
		ss << m_testTypeComboBox.GetCurSel();
		cs->set("radar", "testtype", ss.str());
	}
	{
		std::stringstream ss;
		ss << m_precIndex.GetCurSel();
		cs->set("radar", "precindex", ss.str());
	}
	{
		std::stringstream ss;
		ss << m_precLen;
		cs->set("radar", "preclen", ss.str());
	}
	{
		std::stringstream ss;
		ss << m_precRatio;
		cs->set("radar", "precratio", ss.str());
	}

	// db
	{
		cs->set("db", "dbsource", W2A( m_dbSource ) );
		cs->set("db", "dbname", W2A( m_dbName ) );
		cs->set("db", "user", W2A( m_dbUser ) );
		cs->set("db", "pwd", W2A( m_dbPwd ) );
	}

	//screen
	{
		cs->set("screen", "left", W2A( m_LeftScreenChannels ) );
		cs->set("screen", "right", W2A( m_RightScreenChannels ) );
	}

	cs->write();
	std::string sampleratioStr = cs->get("radar", "sampleratio");
	OnOK();
	//关闭对话框后，数据库应该重新连接-有问题
	//RadarManager::Instance()->init();
}


void DlgRadarParamSet::OnHScroll(UINT   nSBCode,   UINT   nPos,   CScrollBar*   pScrollBar)
{
	if ( pScrollBar->GetSafeHwnd() == m_gainValueSlider.GetSafeHwnd() )
	{
		SetDlgItemInt( IDC_EDIT_GAIN_VALUE, m_gainValueSlider.GetPos() );
		/*m_gainValue.SetDlgItemInt( nPos );*/
	}
	if ( pScrollBar->GetSafeHwnd() == m_delaySlider.GetSafeHwnd() )
	{
		SetDlgItemInt( IDC_EDIT_DELAY, m_delaySlider.GetPos() );
	}

	return;
}

void DlgRadarParamSet::OnBnClickedButtonApply()
{
	// TODO: 在此添加控件通知处理程序代码
	OpenThreads::ScopedLock<OpenThreads::Mutex> lock(_lock);

	UpdateData( TRUE );

	ConfigureSet *cs = RadarManager::Instance()->getConfigureSet();
	if ( !cs )
	{
		return;
	}
	USES_CONVERSION;
	{
		cs->set( "net", "addr", W2A( m_serverIP) );
	}


	{
		std::stringstream ss;
		ss << m_serverPort;
		cs->set("net", "port", ss.str());
	}

	{

		std::stringstream ss;
		ss << m_channelCount;
		cs->set("radar", "count", ss.str());
	}

	{

		std::stringstream ss;
		ss << 0X88;
		cs->set("radar", "id", ss.str());
	}
	{
		std::stringstream ss;
		ss << m_sampleComboBox.GetCurSel();
		cs->set("radar", "sample", ss.str());
	}
	{
		std::stringstream ss;
		ss << m_sampleRatioComboBox.GetCurSel();
		cs->set("radar", "sampleratio", ss.str());
	}
	{
		std::stringstream ss;
		ss << m_gainComboBox.GetCurSel();
		cs->set("radar", "gain", ss.str());
	}
	{
		std::stringstream ss;
		ss << m_gainValueSlider.GetPos();
		cs->set("radar", "gainvalue", ss.str());

	}
	{
		std::stringstream ss;
		ss << m_delaySlider.GetPos();
		cs->set("radar", "delay", ss.str());
	}
	{
		std::stringstream ss;
		ss << m_addCountComboBox.GetCurSel();
		cs->set("radar", "addcount", ss.str());
	}
	{
		std::stringstream ss;
		ss << m_testTypeComboBox.GetCurSel();
		cs->set("radar", "testtype", ss.str());
	}
	{
		std::stringstream ss;
		ss << m_precIndex.GetCurSel();
		cs->set("radar", "precindex", ss.str());
	}
	{
		std::stringstream ss;
		ss << m_precLen;
		cs->set("radar", "preclen", ss.str());
	}
	{
		std::stringstream ss;
		ss << m_precRatio;
		cs->set("radar", "precratio", ss.str());
	}

	// db
	{
		cs->set("db", "dbsource", W2A( m_dbSource ) );
		cs->set("db", "dbname", W2A( m_dbName ) );
		cs->set("db", "user", W2A( m_dbUser ) );
		cs->set("db", "pwd", W2A( m_dbPwd ) );
	}


	cs->write();


	if ( RadarManager::Instance()->getRadarDataRader() )
	{
		RadarDataRader *lpReader = RadarManager::Instance()->getRadarDataRader();
		lpReader->setInit( false );
	}
	//RadarManager::Instance()->getRadarDataRader()->setInit( false );
}

void DlgRadarParamSet::OnBnClickedBtnsetCancel()
{
	// TODO: 在此添加控件通知处理程序代码

	OnCancel();
}
