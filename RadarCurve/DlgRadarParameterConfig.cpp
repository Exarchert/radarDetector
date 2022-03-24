// DlgRadarParameterConfig.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "RadarCurve.h"
#include "DlgRadarParameterConfig.h"
#include "RadarCurve/RadarManager.h"
#include "RadarCurve/ConfigureSet.h"

//C++
#include <sstream>



// DlgRadarParameterConfig �Ի���

IMPLEMENT_DYNAMIC(DlgRadarParameterConfig, CDialog)

DlgRadarParameterConfig::DlgRadarParameterConfig(CWnd* pParent /*=NULL*/)
	: CDialog(DlgRadarParameterConfig::IDD, pParent)
{
	m_pConfigureSet = RadarManager::Instance()->getConfigureSet();
}

DlgRadarParameterConfig::~DlgRadarParameterConfig()
{
	RadarManager::Instance()->stopTest();
}

void DlgRadarParameterConfig::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_SERVER, m_serverIP);//���ڵ�ַ
	DDX_Text(pDX, IDC_EDIT_PORT, m_serverPort );//�˿�	

	DDX_Text(pDX, IDC_EDIT_FTPSERVERIP, m_cstrFtpServerIP);//���ڵ�ַ
	DDX_Text(pDX, IDC_EDIT_FTPPORT, m_cstrFtpPort );//�˿�	
	DDX_Text(pDX, IDC_EDIT_FTPACCOUNT, m_cstrFtpAccount);//���ڵ�ַ
	DDX_Text(pDX, IDC_EDIT_FTPPASSWORD, m_cstrFtpPassword );//�˿�	

	DDX_Text(pDX, IDC_EDIT_RADAR_ID, m_id );//id��
	DDX_Text(pDX, IDC_EDIT_CHANNEL_COUNT, m_channelCount );//ͨ������
	DDX_Control(pDX, IDC_COMBO_DIELECTRIC, m_dielectricComboBox);//��糣��
	DDX_Control(pDX, IDC_COMBO_SAMPLE, m_sampleComboBox);//��������
	DDX_Control(pDX, IDC_COMBO_SAMPLE_RATIO, m_sampleRatioComboBox);//������
	DDX_Control(pDX, IDC_COMBO_GAIN, m_gainComboBox);//����ģʽ
	//DDX_Control(pDX, IDC_SLIDER_GAIN_VALUE, m_gainValueSlider);//��������
	DDX_Text(pDX, IDC_EDIT_GAIN_VALUE, m_gainValue );//����ֵ
	//DDX_Control(pDX, IDC_SLIDER_DELAY, m_delaySlider);//�ӳ�����
	DDX_Text(pDX, IDC_EDIT_DELAY, m_delayValue );//�ӳ�ֵ 
	DDX_Control(pDX, IDC_COMBO_ADD_COUNT, m_addCountComboBox);//�ۼӴ���
	DDX_Control(pDX, IDC_COMBO_TEST_TYPE, m_testTypeComboBox);//������ģʽ
	DDX_Text(pDX, IDC_EDIT_CELIANGLUN_LEN, m_precLen );//�������ܳ�
	//DDX_Control(pDX, IDC_COMBO_CELIANLUN_JINGDU, m_precIndex);//�����־���
	DDX_Text(pDX, IDC_EDIT_WHEELPRECISION, m_precision);//�����־���
	DDX_Text(pDX, IDC_EDIT_CELIANGLUN_RATIO, m_precRatio );//����
	
	DDX_Text(pDX, IDC_EDIT_SCREEN_0, m_screenFirstChannels);
	DDV_MaxChars(pDX, m_screenFirstChannels, 40);
	DDX_Text(pDX, IDC_EDIT_SCREEN_1, m_screenSecondChannels);
	DDV_MaxChars(pDX, m_screenSecondChannels, 40);
	DDX_Text(pDX, IDC_EDIT_SCREEN_2, m_screenThirdChannels);
	DDV_MaxChars(pDX, m_screenThirdChannels, 40);
	DDX_Text(pDX, IDC_EDIT_SCREEN_3, m_screenFourthChannels);
	DDV_MaxChars(pDX, m_screenFourthChannels, 40);

	//DDX_Control( pDX, IDC_SPECTRUM_1, _spectrumWnd[0]);
	//DDX_Control( pDX, IDC_SPECTRUM_2, _spectrumWnd[1]);

	//gps
	DDX_Text(pDX, IDC_EDIT_ComNumber, m_comNumber);//���ڵ�ַ
	DDX_Text(pDX, IDC_EDIT_PortByte, m_portByte );//�˿�	
	DDX_Control(pDX, IDC_COMBO_PARITYBIT, m_parityBit );//У��λ	
	DDX_Text(pDX, IDC_EDIT_DATABIT, m_dataBit );//����λ	
	DDX_Control(pDX, IDC_COMBO_STOPBIT, m_stopBit );//ֹͣλ	

	//���������
	DDX_Text(pDX, IDC_EDIT_TIMEWINDOW, m_timeWindow);//̽��ʱ��
	DDX_Text(pDX, IDC_EDIT_MAXDEPTH, m_maxDepth );//������	
	DDX_Text(pDX, IDC_EDIT_SCANSPEED, m_scanSpeed );//ɨ���ٶ�	

	//�Զ�����
	DDX_Text(pDX, IDC_EDIT_MARKVALUE, m_markValue );//���ֵ

	//�����ʾ
	DDX_Text(pDX, IDC_EDIT_DISPLAYEXTRACT, m_diplayExtract );//���ֵ

	//����
	DDX_Control( pDX, IDC_WAVEFORM1, _waveFormWnd[0]);
	DDX_Control( pDX, IDC_WAVEFORM2, _waveFormWnd[1]);
	DDX_Control( pDX, IDC_WAVEFORM3, _waveFormWnd[2]);
	DDX_Control( pDX, IDC_WAVEFORM4, _waveFormWnd[3]);
	DDX_Control( pDX, IDC_WAVEFORM5, _waveFormWnd[4]);
	DDX_Control( pDX, IDC_WAVEFORM6, _waveFormWnd[5]);
	DDX_Control( pDX, IDC_WAVEFORM7, _waveFormWnd[6]);
	DDX_Control( pDX, IDC_WAVEFORM8, _waveFormWnd[7]);
	DDX_Control( pDX, IDC_WAVEFORM9, _waveFormWnd[8]);
	DDX_Control( pDX, IDC_WAVEFORM10, _waveFormWnd[9]);
	DDX_Control( pDX, IDC_WAVEFORM11, _waveFormWnd[10]);
	DDX_Control( pDX, IDC_WAVEFORM12, _waveFormWnd[11]);
	DDX_Control( pDX, IDC_WAVEFORM13, _waveFormWnd[12]);
	DDX_Control( pDX, IDC_WAVEFORM14, _waveFormWnd[13]);
	DDX_Control( pDX, IDC_WAVEFORM15, _waveFormWnd[14]);
	DDX_Control( pDX, IDC_WAVEFORM16, _waveFormWnd[15]);
	DDX_Control( pDX, IDC_STATIC_CHANNEL1, _channelName[0]);
	DDX_Control( pDX, IDC_STATIC_CHANNEL2, _channelName[1]);
	DDX_Control( pDX, IDC_STATIC_CHANNEL3, _channelName[2]);
	DDX_Control( pDX, IDC_STATIC_CHANNEL4, _channelName[3]);
	DDX_Control( pDX, IDC_STATIC_CHANNEL5, _channelName[4]);
	DDX_Control( pDX, IDC_STATIC_CHANNEL6, _channelName[5]);
	DDX_Control( pDX, IDC_STATIC_CHANNEL7, _channelName[6]);
	DDX_Control( pDX, IDC_STATIC_CHANNEL8, _channelName[7]);
	DDX_Control( pDX, IDC_STATIC_CHANNEL9, _channelName[8]);
	DDX_Control( pDX, IDC_STATIC_CHANNEL10, _channelName[9]);
	DDX_Control( pDX, IDC_STATIC_CHANNEL11, _channelName[10]);
	DDX_Control( pDX, IDC_STATIC_CHANNEL12, _channelName[11]);
	DDX_Control( pDX, IDC_STATIC_CHANNEL13, _channelName[12]);
	DDX_Control( pDX, IDC_STATIC_CHANNEL14, _channelName[13]);
	DDX_Control( pDX, IDC_STATIC_CHANNEL15, _channelName[14]);
	DDX_Control( pDX, IDC_STATIC_CHANNEL16, _channelName[15]);

	DDX_Text( pDX, IDC_EDIT_CORRECTION1, m_dCorrection[0]);
	DDX_Text( pDX, IDC_EDIT_CORRECTION2, m_dCorrection[1]);
	DDX_Text( pDX, IDC_EDIT_CORRECTION3, m_dCorrection[2]);
	DDX_Text( pDX, IDC_EDIT_CORRECTION4, m_dCorrection[3]);
	DDX_Text( pDX, IDC_EDIT_CORRECTION5, m_dCorrection[4]);
	DDX_Text( pDX, IDC_EDIT_CORRECTION6, m_dCorrection[5]);
	DDX_Text( pDX, IDC_EDIT_CORRECTION7, m_dCorrection[6]);
	DDX_Text( pDX, IDC_EDIT_CORRECTION8, m_dCorrection[7]);
	DDX_Text( pDX, IDC_EDIT_CORRECTION9, m_dCorrection[8]);
	DDX_Text( pDX, IDC_EDIT_CORRECTION10, m_dCorrection[9]);
	DDX_Text( pDX, IDC_EDIT_CORRECTION11, m_dCorrection[10]);
	DDX_Text( pDX, IDC_EDIT_CORRECTION12, m_dCorrection[11]);
	DDX_Text( pDX, IDC_EDIT_CORRECTION13, m_dCorrection[12]);
	DDX_Text( pDX, IDC_EDIT_CORRECTION14, m_dCorrection[13]);
	DDX_Text( pDX, IDC_EDIT_CORRECTION15, m_dCorrection[14]);
	DDX_Text( pDX, IDC_EDIT_CORRECTION16, m_dCorrection[15]);

	DDX_Control( pDX, IDC_EDIT_CORRECTION1, _channelCorrection[0]);
	DDX_Control( pDX, IDC_EDIT_CORRECTION2, _channelCorrection[1]);
	DDX_Control( pDX, IDC_EDIT_CORRECTION3, _channelCorrection[2]);
	DDX_Control( pDX, IDC_EDIT_CORRECTION4, _channelCorrection[3]);
	DDX_Control( pDX, IDC_EDIT_CORRECTION5, _channelCorrection[4]);
	DDX_Control( pDX, IDC_EDIT_CORRECTION6, _channelCorrection[5]);
	DDX_Control( pDX, IDC_EDIT_CORRECTION7, _channelCorrection[6]);
	DDX_Control( pDX, IDC_EDIT_CORRECTION8, _channelCorrection[7]);
	DDX_Control( pDX, IDC_EDIT_CORRECTION9, _channelCorrection[8]);
	DDX_Control( pDX, IDC_EDIT_CORRECTION10, _channelCorrection[9]);
	DDX_Control( pDX, IDC_EDIT_CORRECTION11, _channelCorrection[10]);
	DDX_Control( pDX, IDC_EDIT_CORRECTION12, _channelCorrection[11]);
	DDX_Control( pDX, IDC_EDIT_CORRECTION13, _channelCorrection[12]);
	DDX_Control( pDX, IDC_EDIT_CORRECTION14, _channelCorrection[13]);
	DDX_Control( pDX, IDC_EDIT_CORRECTION15, _channelCorrection[14]);
	DDX_Control( pDX, IDC_EDIT_CORRECTION16, _channelCorrection[15]);
									  
}


BEGIN_MESSAGE_MAP(DlgRadarParameterConfig, CDialog)
	ON_WM_HSCROLL()
	ON_WM_CLOSE()
	ON_BN_CLICKED(ID_BTNSET_OK, &DlgRadarParameterConfig::OnBnClickedBtnsetOk)
	ON_BN_CLICKED(ID_BTNSET_CANCEL, &DlgRadarParameterConfig::OnBnClickedBtnsetCancel)
	//ON_BN_CLICKED(IDC_BUTTON_APPLY, &DlgRadarParameterConfig::OnBnClickedButtonApply)
	ON_BN_CLICKED(IDC_BUTTON_APPLY, &DlgRadarParameterConfig::OnBnClickedButtonWaveFormTest)
	
//	ON_CBN_EDITCHANGE(IDC_COMBO_DIELECTRIC, &DlgRadarParameterConfig::OnCbnEditchangeComboDielectric)
	ON_CBN_SELCHANGE(IDC_COMBO_DIELECTRIC, &DlgRadarParameterConfig::OnCbnSelchangeComboDielectric)
//	ON_CBN_CLOSEUP(IDC_COMBO_DIELECTRIC, &DlgRadarParameterConfig::OnCbnCloseupComboDielectric)
	ON_CBN_SELCHANGE(IDC_COMBO_SAMPLE, &DlgRadarParameterConfig::OnCbnSelchangeComboSample)
	ON_CBN_SELCHANGE(IDC_COMBO_SAMPLE_RATIO, &DlgRadarParameterConfig::OnCbnSelchangeComboSampleRatio)
//	ON_EN_CHANGE(IDC_EDIT_CORRECTION1, &DlgRadarParameterConfig::OnEnChangeEditCorrection1)
	ON_EN_KILLFOCUS(IDC_EDIT_CORRECTION1, &DlgRadarParameterConfig::OnEnKillfocusEditCorrection1)
	ON_EN_KILLFOCUS(IDC_EDIT_CORRECTION2, &DlgRadarParameterConfig::OnEnKillfocusEditCorrection2)
	ON_EN_KILLFOCUS(IDC_EDIT_CORRECTION3, &DlgRadarParameterConfig::OnEnKillfocusEditCorrection3)
	ON_EN_KILLFOCUS(IDC_EDIT_CORRECTION4, &DlgRadarParameterConfig::OnEnKillfocusEditCorrection4)
	ON_EN_KILLFOCUS(IDC_EDIT_CORRECTION5, &DlgRadarParameterConfig::OnEnKillfocusEditCorrection5)
	ON_EN_KILLFOCUS(IDC_EDIT_CORRECTION6, &DlgRadarParameterConfig::OnEnKillfocusEditCorrection6)
	ON_EN_KILLFOCUS(IDC_EDIT_CORRECTION7, &DlgRadarParameterConfig::OnEnKillfocusEditCorrection7)
	ON_EN_KILLFOCUS(IDC_EDIT_CORRECTION8, &DlgRadarParameterConfig::OnEnKillfocusEditCorrection8)
	ON_EN_KILLFOCUS(IDC_EDIT_CORRECTION9, &DlgRadarParameterConfig::OnEnKillfocusEditCorrection9)
	ON_EN_KILLFOCUS(IDC_EDIT_CORRECTION10, &DlgRadarParameterConfig::OnEnKillfocusEditCorrection10)
	ON_EN_KILLFOCUS(IDC_EDIT_CORRECTION11, &DlgRadarParameterConfig::OnEnKillfocusEditCorrection11)
	ON_EN_KILLFOCUS(IDC_EDIT_CORRECTION12, &DlgRadarParameterConfig::OnEnKillfocusEditCorrection12)
	ON_EN_KILLFOCUS(IDC_EDIT_CORRECTION13, &DlgRadarParameterConfig::OnEnKillfocusEditCorrection13)
	ON_EN_KILLFOCUS(IDC_EDIT_CORRECTION14, &DlgRadarParameterConfig::OnEnKillfocusEditCorrection14)
	ON_EN_KILLFOCUS(IDC_EDIT_CORRECTION15, &DlgRadarParameterConfig::OnEnKillfocusEditCorrection15)
	ON_EN_KILLFOCUS(IDC_EDIT_CORRECTION16, &DlgRadarParameterConfig::OnEnKillfocusEditCorrection16)
END_MESSAGE_MAP()


// DlgRadarParameterConfig ��Ϣ�������

BOOL DlgRadarParameterConfig::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_dielectricComboBox.AddString(L"1(����)");
	m_dielectricComboBox.AddString(L"1.9(����)");
	m_dielectricComboBox.AddString(L"2(ʯ��)");
	m_dielectricComboBox.AddString(L"2.5(ֽ)");
	m_dielectricComboBox.AddString(L"2.8(ľͷ)");
	m_dielectricComboBox.AddString(L"2.8(����)");
	m_dielectricComboBox.AddString(L"3.2(��)");
	m_dielectricComboBox.AddString(L"4(����)");
	m_dielectricComboBox.AddString(L"4(����)");
	m_dielectricComboBox.AddString(L"4.1(����)");
	m_dielectricComboBox.AddString(L"5(��ɳ)");
	m_dielectricComboBox.AddString(L"5(������:��)");
	m_dielectricComboBox.AddString(L"6(���궳��)");
	m_dielectricComboBox.AddString(L"6.4(������)");
	m_dielectricComboBox.AddString(L"7(ҳ��:ʪ)");
	m_dielectricComboBox.AddString(L"7(������:ʪ)");
	m_dielectricComboBox.AddString(L"7(����:��)");
	m_dielectricComboBox.AddString(L"8(����:ʪ)");
	m_dielectricComboBox.AddString(L"8(������:ʪ)");
	m_dielectricComboBox.AddString(L"12(ճ��)");
	m_dielectricComboBox.AddString(L"15(����)");
	m_dielectricComboBox.AddString(L"16(��)");
	m_dielectricComboBox.AddString(L"30(ʪɳ)");
	m_dielectricComboBox.AddString(L"30(����)");
	m_dielectricComboBox.AddString(L"81(ˮ)");

	/*
	m_sampleComboBox.AddString(L"256");
	m_sampleComboBox.AddString(L"512");
	m_sampleComboBox.AddString(L"1024");
	m_sampleComboBox.AddString(L"2048");

	switch (RadarManager::Instance()->GetRadarWorkType())
	{
	case RADAR_WORK_TYPE_ONE_USB:
		m_sampleRatioComboBox.AddString(L"20.48G");
		m_sampleRatioComboBox.AddString(L"10.24G");
		m_sampleRatioComboBox.AddString(L"5.12G");
		m_sampleRatioComboBox.AddString(L"2.56G");
		break;
	case RADAR_WORK_TYPE_DOUBLE_USB:
		m_sampleRatioComboBox.AddString(L"20.48G");
		m_sampleRatioComboBox.AddString(L"10.24G");
		m_sampleRatioComboBox.AddString(L"5.12G");
		m_sampleRatioComboBox.AddString(L"2.56G");
		break;
	case RADAR_WORK_TYPE_DOUBLE_USB_OLD:
		m_sampleRatioComboBox.AddString(L"20.48G");
		m_sampleRatioComboBox.AddString(L"10.24G");
		m_sampleRatioComboBox.AddString(L"5.12G");
		m_sampleRatioComboBox.AddString(L"2.56G");
		break;
	case RADAR_WORK_TYPE_FOUR_USB:
		m_sampleRatioComboBox.AddString(L"20.48G");
		m_sampleRatioComboBox.AddString(L"10.24G");
		m_sampleRatioComboBox.AddString(L"5.12G");
		m_sampleRatioComboBox.AddString(L"2.56G");
		break;
	case RADAR_WORK_TYPE_EIGHT:
		m_sampleRatioComboBox.AddString(L"20.48G");
		m_sampleRatioComboBox.AddString(L"10.24G");
		m_sampleRatioComboBox.AddString(L"5.12G");
		m_sampleRatioComboBox.AddString(L"2.56G");
		break;
	default:
		break;
	}
	*/

	m_gainComboBox.AddString(L"�̶�����");
	m_gainComboBox.AddString(L"������ģʽ0");
	m_gainComboBox.AddString(L"������ģʽ1");

	m_addCountComboBox.AddString(L"1");
	m_addCountComboBox.AddString(L"2");
	m_addCountComboBox.AddString(L"4");
	m_addCountComboBox.AddString(L"8");
	m_addCountComboBox.AddString(L"16");
	m_addCountComboBox.AddString(L"32");
	m_addCountComboBox.AddString(L"64");
	m_addCountComboBox.AddString(L"128");
	m_addCountComboBox.AddString(L"256");

	m_testTypeComboBox.AddString(L"ʱ�䴥��");
	m_testTypeComboBox.AddString(L"���봥��");

	/*m_precIndex.AddString(L"1cm");
	m_precIndex.AddString(L"2cm");
	m_precIndex.AddString(L"5cm");
	m_precIndex.AddString(L"10cm");
	m_precIndex.AddString(L"20cm");*/

	m_parityBit.AddString(L"NONE");
	m_parityBit.AddString(L"ODD");
	m_parityBit.AddString(L"EVEN");
	m_parityBit.AddString(L"MARK");
	m_parityBit.AddString(L"SPACE");

	m_stopBit.AddString(L"1");
	m_stopBit.AddString(L"1.5");
	m_stopBit.AddString(L"2");

	for(int i=0;i<16;i++){//hjl 20210418�������ò�����ʾ
		changeChannel(i,true);
	}
	resizeChannelWnd();

	InitByRadarConfig();

	OnBnClickedButtonWaveFormTest();
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

int getSampleCountValue(int index);
float getSampleRationValue(int index);
float getDielectricValue(int index);
// ���������ļ���ʼ������

wchar_t* charToLPCTSTR(const char *cTemp){
	int num = MultiByteToWideChar(0,0,cTemp,-1,NULL,0);//��ó��ֽ�����Ŀռ�;
	wchar_t *wTemp = new wchar_t[num];//LPCTSTR;
	MultiByteToWideChar(0,0,cTemp,-1,wTemp,num);//char* ת���� LPCTSTR
	return wTemp;
}


void DlgRadarParameterConfig::InitByRadarConfig()
{
	if ( !m_pConfigureSet )
	{
		return;
	}

	m_sampleComboBox.AddString(charToLPCTSTR(m_pConfigureSet->get("sampleNum", "0").c_str()));
	m_sampleComboBox.AddString(charToLPCTSTR(m_pConfigureSet->get("sampleNum", "1").c_str()));
	m_sampleComboBox.AddString(charToLPCTSTR(m_pConfigureSet->get("sampleNum", "2").c_str()));
	m_sampleComboBox.AddString(charToLPCTSTR(m_pConfigureSet->get("sampleNum", "3").c_str()));

	m_sampleRatioComboBox.AddString(charToLPCTSTR(m_pConfigureSet->get("sampleRate", "0").c_str()));
	m_sampleRatioComboBox.AddString(charToLPCTSTR(m_pConfigureSet->get("sampleRate", "1").c_str()));
	m_sampleRatioComboBox.AddString(charToLPCTSTR(m_pConfigureSet->get("sampleRate", "2").c_str()));
	m_sampleRatioComboBox.AddString(charToLPCTSTR(m_pConfigureSet->get("sampleRate", "3").c_str()));


	USES_CONVERSION;
	m_serverIP = A2W(m_pConfigureSet->get("net", "addr").c_str());
	m_serverPort = atoi ( m_pConfigureSet->get("net", "port").c_str() );
	m_channelCount = atoi ( m_pConfigureSet->get("radar", "count").c_str());

	m_cstrFtpServerIP = A2W(m_pConfigureSet->get("ftp", "serverIP").c_str());
	m_cstrFtpPort = A2W(m_pConfigureSet->get("ftp", "port").c_str());
	m_cstrFtpAccount = A2W(m_pConfigureSet->get("ftp", "account").c_str());
	m_cstrFtpPassword = A2W(m_pConfigureSet->get("ftp", "password").c_str());

	CString idText;
	idText.Format(L"0X%X", atoi( m_pConfigureSet->get("radar", "id").c_str()));
	m_id = idText;
	m_dielectricComboBox.SetCurSel ( atoi ( m_pConfigureSet->get("radar", "dielectric").c_str() ) );
	m_sampleComboBox.SetCurSel( atoi ( m_pConfigureSet->get("radar", "sample").c_str() ) );
	m_sampleRatioComboBox.SetCurSel( atoi( m_pConfigureSet->get("radar", "sampleratio").c_str() ) );
	m_gainComboBox.SetCurSel(atoi(m_pConfigureSet->get("radra", "gain").c_str()));
	//m_gainValueSlider.SetRangeMin( 0 );
	//m_gainValueSlider.SetRangeMax( 255 );
	//m_gainValueSlider.SetPos( atoi( m_pConfigureSet->get("radar", "gainvalue").c_str() ) );
	//m_gainValue.SetWindowText( A2W( m_pConfigureSet->get("radar", "gainvalue").c_str() ) );
	m_gainValue = atoi ( m_pConfigureSet->get("radar", "gainvalue").c_str() );
	//m_delaySlider.SetRangeMin( 0 );
	//m_delaySlider.SetRangeMax( 255 );
	//m_delaySlider.SetPos( atoi( m_pConfigureSet->get("radar", "delay").c_str()));
	//m_delayValue.SetWindowText ( A2W ( m_pConfigureSet->get("radar", "delay").c_str() ) );
	m_delayValue = atoi ( m_pConfigureSet->get("radar", "delay").c_str() );
	m_addCountComboBox.SetCurSel( atoi ( m_pConfigureSet->get("radar", "addcount").c_str()) );
	m_testTypeComboBox.SetCurSel( atoi ( m_pConfigureSet->get("radar", "testtype").c_str()) );

	//m_precIndex.SetCurSel( atoi ( m_pConfigureSet->get("radar", "precindex").c_str()));
	m_precision = atof ( m_pConfigureSet->get("radar", "precision").c_str() );
	m_precLen = atof ( m_pConfigureSet->get("radar", "preclen").c_str() );
	m_precRatio = atoi ( m_pConfigureSet->get("radar", "precratio").c_str());


	m_screenFirstChannels = A2W(m_pConfigureSet->get("screen","first").c_str());
	m_screenSecondChannels = A2W(m_pConfigureSet->get("screen","second").c_str());
	m_screenThirdChannels = A2W(m_pConfigureSet->get("screen","third").c_str());
	m_screenFourthChannels = A2W(m_pConfigureSet->get("screen","fourth").c_str());

	m_comNumber = atoi ( m_pConfigureSet->get("com", "port").c_str() );
	m_portByte = atoi ( m_pConfigureSet->get("com", "baud").c_str() );
	m_parityBit.SetCurSel( atoi ( m_pConfigureSet->get("com", "parity").c_str()) );
	m_dataBit = atoi ( m_pConfigureSet->get("com", "databits").c_str() );
	m_stopBit.SetCurSel( atoi ( m_pConfigureSet->get("com", "stopbits").c_str()) );

	m_markValue = atoi ( m_pConfigureSet->get("mark", "markValue").c_str() );

	m_diplayExtract = atoi ( m_pConfigureSet->get("displayExtract", "displayExtractValue").c_str() );

	//̽��ʱ��=��������/Ƶ��
	m_timeWindow = getSampleCountValue(atoi(m_pConfigureSet->get("radar", "sample").c_str()))/getSampleRationValue(atoi(m_pConfigureSet->get("radar", "sampleratio").c_str()));
	//������=��������/������Ƶ��*�����½�糣����*0.15
	m_maxDepth = getSampleCountValue(atoi(m_pConfigureSet->get("radar", "sample").c_str()))/(getSampleRationValue(atoi(m_pConfigureSet->get("radar", "sampleratio").c_str()))*sqrt(getDielectricValue(atoi(m_pConfigureSet->get("radar", "dielectric").c_str()))))*0.15;
	//ɨ������=100000/��������
	m_scanSpeed = 100000/getSampleCountValue(atoi(m_pConfigureSet->get("radar", "sample").c_str()));

	for (int i=0;i<16;i++){
		{//Ϊ��ss�������� ������һֱ<<�����ַ�
			std::stringstream ss;
			ss << i+12;
			m_dCorrection[i] = atoi ( m_pConfigureSet->get("correction", ss.str()).c_str() );
		}
	}

	UpdateData( FALSE );
}

void DlgRadarParameterConfig::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	/*
	if ( pScrollBar->GetSafeHwnd() == m_gainValueSlider.GetSafeHwnd() )
	{
		SetDlgItemInt( IDC_EDIT_GAIN_VALUE, m_gainValueSlider.GetPos() );
		//m_gainValue.SetDlgItemInt( nPos );
	}
	if ( pScrollBar->GetSafeHwnd() == m_delaySlider.GetSafeHwnd() )
	{
		SetDlgItemInt( IDC_EDIT_DELAY, m_delaySlider.GetPos() );
	}

	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
	*/
}

void DlgRadarParameterConfig::OnBnClickedBtnsetOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	RadarManager::Instance()->stopTest();

	OpenThreads::ScopedLock<OpenThreads::Mutex> lock(_lock);

	UpdateData( TRUE );

	if ( !m_pConfigureSet )
	{
		return;
	}
	USES_CONVERSION;
	{
		m_pConfigureSet->set( "net", "addr", W2A( m_serverIP) );
		m_pConfigureSet->set( "ftp", "serverIP", W2A( m_cstrFtpServerIP) );
		m_pConfigureSet->set( "ftp", "port", W2A( m_cstrFtpPort) );
		m_pConfigureSet->set( "ftp", "account", W2A( m_cstrFtpAccount) );
		m_pConfigureSet->set( "ftp", "password", W2A( m_cstrFtpPassword) );
	}

	{
		std::stringstream ss;
		ss << m_serverPort;
		m_pConfigureSet->set("net", "port", ss.str());
	}
	{
		std::stringstream ss;
		ss << m_channelCount;
		m_pConfigureSet->set("radar", "count", ss.str());
	}
	{
		std::stringstream ss;
		ss << 0X88;
		m_pConfigureSet->set("radar", "id", ss.str());
	}
	{
		std::stringstream ss;
		ss << m_dielectricComboBox.GetCurSel();
		m_pConfigureSet->set("radar", "dielectric", ss.str());
	}
	{
		std::stringstream ss;
		ss << m_sampleComboBox.GetCurSel();
		m_pConfigureSet->set("radar", "sample", ss.str());
	}
	{
		std::stringstream ss;
		ss << m_sampleRatioComboBox.GetCurSel();
		m_pConfigureSet->set("radar", "sampleratio", ss.str());
	}
	{
		std::stringstream ss;
		ss << m_gainComboBox.GetCurSel();
		m_pConfigureSet->set("radar", "gain", ss.str());
	}
	{
		std::stringstream ss;
		ss << m_gainValue;
		m_pConfigureSet->set("radar", "gainvalue", ss.str());
	}
	{
		std::stringstream ss;
		ss << m_delayValue;
		m_pConfigureSet->set("radar", "delay", ss.str());
	}
	{
		std::stringstream ss;
		ss << m_addCountComboBox.GetCurSel();
		m_pConfigureSet->set("radar", "addcount", ss.str());
	}
	{
		std::stringstream ss;
		ss << m_testTypeComboBox.GetCurSel();
		m_pConfigureSet->set("radar", "testtype", ss.str());
	}
	{
		std::stringstream ss;
		ss << m_precision;
		m_pConfigureSet->set("radar", "precision", ss.str());
	}
	{
		std::stringstream ss;
		ss << m_precLen;
		m_pConfigureSet->set("radar", "preclen", ss.str());
	}
	{
		std::stringstream ss;
		ss << m_precRatio;
		m_pConfigureSet->set("radar", "precratio", ss.str());
	}
	{
		std::stringstream ss;
		ss << m_comNumber;
		m_pConfigureSet->set("com", "port", ss.str());
	}
	{
		std::stringstream ss;
		ss << m_portByte;
		m_pConfigureSet->set("com", "baud", ss.str());
	}
	{
		std::stringstream ss;
		ss << m_parityBit.GetCurSel();
		m_pConfigureSet->set("com", "parity", ss.str());
	}
	{
		std::stringstream ss;
		ss << m_dataBit;
		m_pConfigureSet->set("com", "databits", ss.str());
	}
	{
		std::stringstream ss;
		ss << m_stopBit.GetCurSel();
		m_pConfigureSet->set("com", "stopbits", ss.str());
	}

	//screen
	{
		m_pConfigureSet->set("screen", "first", W2A( m_screenFirstChannels ) );
		m_pConfigureSet->set("screen", "second", W2A( m_screenSecondChannels ) );
		m_pConfigureSet->set("screen", "third", W2A( m_screenThirdChannels ) );
		m_pConfigureSet->set("screen", "fourth", W2A( m_screenFourthChannels ) );
	}

	{
		std::stringstream ss;
		ss << m_markValue;
		m_pConfigureSet->set("mark", "markValue", ss.str());
	}

	{
		std::stringstream ss;
		ss << m_diplayExtract;
		m_pConfigureSet->set("displayExtract", "displayExtractValue", ss.str());
	}

	for(int i=0; i<16; i++){
		{
			std::stringstream ss1;
			ss1 << m_dCorrection[i];
			std::stringstream ss2;
			ss2 << i+12;
			m_pConfigureSet->set("correction", ss2.str(), ss1.str());
			if( i<12 ){
				std::stringstream ss3;
				ss3 << i;
				m_pConfigureSet->set("correction", ss3.str(), ss1.str());
			}
		}
	}

	m_pConfigureSet->write();

	OnOK();
}

void DlgRadarParameterConfig::OnBnClickedBtnsetCancel(){
	RadarManager::Instance()->stopTest();
	USES_CONVERSION;
	{
		std::stringstream ss;
		ss << m_testTypeComboBox.GetCurSel();
		m_pConfigureSet->set("radar", "testtype", ss.str());
	}
	OnCancel();
}

void DlgRadarParameterConfig::OnBnClickedButtonApply()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData( TRUE );

	if ( !m_pConfigureSet )
	{
		return;
	}
	USES_CONVERSION;
	{
		m_pConfigureSet->set( "net", "addr", W2A( m_serverIP) );
		m_pConfigureSet->set( "ftp", "serverIP", W2A( m_cstrFtpServerIP) );
		m_pConfigureSet->set( "ftp", "port", W2A( m_cstrFtpPort) );
		m_pConfigureSet->set( "ftp", "account", W2A( m_cstrFtpAccount) );
		m_pConfigureSet->set( "ftp", "password", W2A( m_cstrFtpPassword) );
	}
	{
		std::stringstream ss;
		ss << m_serverPort;
		m_pConfigureSet->set("net", "port", ss.str());
	}
	{
		std::stringstream ss;
		ss << m_channelCount;
		m_pConfigureSet->set("radar", "count", ss.str());
	}

	{
		std::stringstream ss;
		ss << 0X88;
		m_pConfigureSet->set("radar", "id", ss.str());
	}
	{
		std::stringstream ss;
		ss << m_dielectricComboBox.GetCurSel();
		m_pConfigureSet->set("radar", "dielectric", ss.str());
	}
	{
		std::stringstream ss;
		ss << m_sampleComboBox.GetCurSel();
		m_pConfigureSet->set("radar", "sample", ss.str());
	}
	{
		std::stringstream ss;
		ss << m_sampleRatioComboBox.GetCurSel();
		m_pConfigureSet->set("radar", "sampleratio", ss.str());
	}
	{
		std::stringstream ss;
		ss << m_gainComboBox.GetCurSel();
		m_pConfigureSet->set("radar", "gain", ss.str());
	}
	{
		std::stringstream ss;
		ss << m_gainValue;
		m_pConfigureSet->set("radar", "gainvalue", ss.str());
	}
	{
		std::stringstream ss;
		ss << m_delayValue;
		m_pConfigureSet->set("radar", "delay", ss.str());
	}
	{
		std::stringstream ss;
		ss << m_addCountComboBox.GetCurSel();
		m_pConfigureSet->set("radar", "addcount", ss.str());
	}
	{
		std::stringstream ss;
		ss << m_testTypeComboBox.GetCurSel();
		m_pConfigureSet->set("radar", "testtype", ss.str());
	}
	{
		std::stringstream ss;
		ss << m_precision;
		m_pConfigureSet->set("radar", "precision", ss.str());
	}
	{
		std::stringstream ss;
		ss << m_precLen;
		m_pConfigureSet->set("radar", "preclen", ss.str());
	}
	{
		std::stringstream ss;
		ss << m_precRatio;
		m_pConfigureSet->set("radar", "precratio", ss.str());
	}
	{
		std::stringstream ss;
		ss << m_comNumber;
		m_pConfigureSet->set("com", "port", ss.str());
	}
	{
		std::stringstream ss;
		ss << m_portByte;
		m_pConfigureSet->set("com", "baud", ss.str());
	}
		{
		std::stringstream ss;
		ss << m_parityBit.GetCurSel();
		m_pConfigureSet->set("com", "parity", ss.str());
	}
	{
		std::stringstream ss;
		ss << m_dataBit;
		m_pConfigureSet->set("com", "databits", ss.str());
	}
	{
		std::stringstream ss;
		ss << m_stopBit.GetCurSel();
		m_pConfigureSet->set("com", "stopbits", ss.str());
	}

	{
		std::stringstream ss;
		ss << m_markValue;
		m_pConfigureSet->set("mark", "markValue", ss.str());
	}

	{
		std::stringstream ss;
		ss << m_diplayExtract;
		m_pConfigureSet->set("displayExtract", "displayExtractValue", ss.str());
	}

	for(int i=0; i<16; i++){
		{
			std::stringstream ss1;
			ss1 << m_dCorrection[i];
			std::stringstream ss2;
			ss2 << i+12;
			m_pConfigureSet->set("correction", ss2.str(), ss1.str());
			if( i<12 ){
				std::stringstream ss3;
				ss3 << i;
				m_pConfigureSet->set("correction", ss3.str(), ss1.str());
			}
		}
	}

	m_pConfigureSet->write();

	if ( RadarManager::Instance()->getRadarDataRader() )
	{
		RadarDataRader *lpReader = RadarManager::Instance()->getRadarDataRader();
		lpReader->setInit( false );
	}
}

void DlgRadarParameterConfig::OnBnClickedButtonWaveFormTest()
{
	OnBnClickedButtonApply();
	m_pConfigureSet->set("radar", "testtype", "0");//z��ʱ��Ϊʱ��ɼ�

	//���²�����ʾ���ڵĲ����ʺͲ�����
	ConfigureSet *lpSet = RadarManager::Instance()->getConfigureSet();
	int iSmpNum = atoi ( lpSet->get("radar", "sample").c_str());
	int iSampRate = atoi ( lpSet->get("radar", "sampleratio" ).c_str() );
	for(int index=0;index<16;index++){
		_waveFormWnd[index].SetSampleCount( RadarManager::Instance()->getSampCount( iSmpNum ) );
		_waveFormWnd[index].SetSampleRatio( RadarManager::Instance()->getSampRatio( iSampRate, index ) );
	}

	RadarManager::Instance()->startNewTest();
}

void DlgRadarParameterConfig::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	RadarManager::Instance()->stopTest();
	USES_CONVERSION;
	{
		std::stringstream ss;
		ss << m_testTypeComboBox.GetCurSel();
		m_pConfigureSet->set("radar", "testtype", ss.str());
	}
	EndDialog(IDCANCEL);
	CDialog::OnClose();
}

float getSampleRationValue(int index){
	float tempArr[4]={20.48,10.24,5.12,2.56};
	return tempArr[index];
}

int getSampleCountValue(int index){
	int tempArr[4]={256,512,1024,2048};
	return tempArr[index];
}

float getDielectricValue(int index){
	int tempArr[25]={1,1.9,2,2.5,2.8,2.8,3.2,4,4,4.1,5,5,6,6.4,7,7,7,8,8,12,15,16,30,30,81};
	return tempArr[index];
}


void DlgRadarParameterConfig::OnCbnSelchangeComboDielectric()
{
	//������=��������/������Ƶ��*�����½�糣����*0.15
	m_maxDepth = getSampleCountValue(m_sampleComboBox.GetCurSel())/(getSampleRationValue(m_sampleRatioComboBox.GetCurSel())*sqrt(getDielectricValue(m_dielectricComboBox.GetCurSel())))*0.15;
	UpdateData( FALSE );
	OnBnClickedButtonWaveFormTest();
}


void DlgRadarParameterConfig::OnCbnSelchangeComboSample()
{
	//̽��ʱ��=��������/Ƶ��
	m_timeWindow = getSampleCountValue(m_sampleComboBox.GetCurSel())/getSampleRationValue(m_sampleRatioComboBox.GetCurSel());
	//������=��������/������Ƶ��*�����½�糣����*0.15
	m_maxDepth = getSampleCountValue(m_sampleComboBox.GetCurSel())/(getSampleRationValue(m_sampleRatioComboBox.GetCurSel())*sqrt(getDielectricValue(m_dielectricComboBox.GetCurSel())))*0.15;
	//ɨ������=100000/��������
	m_scanSpeed = 100000/getSampleCountValue(m_sampleComboBox.GetCurSel());
	UpdateData( FALSE );
	OnBnClickedButtonWaveFormTest();
}

void DlgRadarParameterConfig::OnCbnSelchangeComboSampleRatio()
{
	//̽��ʱ��=��������/Ƶ��
	m_timeWindow = getSampleCountValue(m_sampleComboBox.GetCurSel())/getSampleRationValue(m_sampleRatioComboBox.GetCurSel());
	//������=��������/������Ƶ��*�����½�糣����*0.15
	m_maxDepth = getSampleCountValue(m_sampleComboBox.GetCurSel())/(getSampleRationValue(m_sampleRatioComboBox.GetCurSel())*sqrt(getDielectricValue(m_dielectricComboBox.GetCurSel())))*0.15;
	UpdateData( FALSE );
	OnBnClickedButtonWaveFormTest();
}

void DlgRadarParameterConfig::changeChannel( int index, bool flag )//hjl 20210418�������ò�����ʾ
{
	if ( index >= 16 ){
		return;
	}

	if ( flag ){
		ConfigureSet *lpSet = RadarManager::Instance()->getConfigureSet();
		int iSmpNum = atoi ( lpSet->get("radar", "sample").c_str());
		int iSampRate = atoi ( lpSet->get("radar", "sampleratio" ).c_str() );

		_waveFormWnd[index].SetSampleCount( RadarManager::Instance()->getSampCount( iSmpNum ) );
		_waveFormWnd[index].SetSampleRatio( RadarManager::Instance()->getSampRatio( iSampRate, index ) );

		std::stringstream ss;
		ss << index+12;
		_waveFormWnd[index].setCorrection( atoi( lpSet->get("correction", ss.str()).c_str() ) );
		
		_waveFormWnd[index].StartDraw();
		_waveFormWnd[index].ShowWindow( SW_SHOW );
		_channelName[index].ShowWindow( SW_SHOW );

		USES_CONVERSION;
		
		RadarManager *lpProject = RadarManager::Instance();
		if ( lpProject )
		{
			CString titleName;
			CString name;
			name = A2W( lpProject->getChannelName(index+12).c_str() );//12+12+4 ֱ��+12��ȫ��16����˳����
			//name = A2W( "" );//12+12+4 ֱ��+12��ȫ��16����˳����
			if(index<12){
				titleName.Format(L"ͨ��%d-%s", index + 1, name);//origin
			}else{
				titleName.Format(L"ͨ��%d-%s", index + 13, name);//origin
			}

			_channelName[index].SetWindowText( titleName );
		}

		/*
		int menuPosIndex;
		std::vector<std::string>::iterator it;
		std::stringstream ss;
		ss<<index;
		it = find(m_channelVec.begin(),m_channelVec.end(), ss.str());
		if (it != m_channelVec.end())
		{
			menuPosIndex = distance(m_channelVec.begin(), it);
			m_pMenu.GetSubMenu(0)->CheckMenuItem( menuPosIndex, MF_BYPOSITION | MF_CHECKED );
		}*/

	}else{
		_waveFormWnd[index].EndDraw();
		_waveFormWnd[index].ShowWindow( SW_HIDE );
		_channelName[index].ShowWindow( SW_HIDE );

		/*
		int menuPosIndex;
		std::vector<std::string>::iterator it;
		std::stringstream ss;
		ss<<index;
		it = find(m_channelVec.begin(),m_channelVec.end(), ss.str());
		if (it != m_channelVec.end()){
			menuPosIndex = distance(m_channelVec.begin(), it);
			m_pMenu.GetSubMenu(0)->CheckMenuItem( menuPosIndex, MF_BYPOSITION | MF_UNCHECKED );
		}*/
	}
	//resizeChannelWnd();
}

void DlgRadarParameterConfig::resizeChannelWnd()//hjl 20210418�������ò�����ʾ
{
	/*int showCount = 0;
	for( int i = 0; i < MAX_CHANNEL; i ++ ){
		if ( !_waveFormWnd[i].GetSafeHwnd() ){
			return;
		}
		showCount += RadarManager::Instance()->int_Check_Channel[i] ;
	}
	
	if ( showCount == 0 ){
		return;
	}
	*/

	int nScreenWidth, nScreenHeight;  
	nScreenWidth = GetSystemMetrics(SM_CXSCREEN);  
	nScreenHeight = GetSystemMetrics(SM_CYSCREEN);

	CRect fullRect;
	GetClientRect( fullRect );

	const int space = 10;
	const int namespaceHeight = 20;
	int curWidth = 90;
	int wndHeight = 205;

	MoveWindow( (nScreenWidth-600-(curWidth+space)*8)/2, (nScreenHeight-550)/2, 600+(curWidth+space)*8, 550 );//������ʾ

	CRect staticRect;
	_channelName[0].GetClientRect( staticRect );

	//showCount=showCount-1;//���һ���������ʱ��
	int startX = 550;
	for( int i = 0; i < 7; i ++ ){
		_channelName[i].MoveWindow( startX+i*(curWidth + space), 4, curWidth , namespaceHeight );
		_channelCorrection[i].MoveWindow( startX+i*(curWidth + space), 1+namespaceHeight, curWidth , namespaceHeight );
		_waveFormWnd[i].MoveWindow( startX+i*(curWidth + space), 3+namespaceHeight*2, curWidth, wndHeight );
		/*
		if(showCount==0){
			_waveFormWnd[i].MoveWindow( startX, namespaceHeight, curWidth+30, fullRect.Height() - namespaceHeight - 20 -20 );//���һ�������+30����ʾʱ��
		}else{
			showCount=showCount-1;//���һ���������ʱ��
			_waveFormWnd[i].MoveWindow( startX, namespaceHeight, curWidth, fullRect.Height() - namespaceHeight - 20 -20);
		}
		//_channelName[i].MoveWindow( startX, 3, staticRect.Width() , namespaceHeight );
		_channelName[i].MoveWindow( startX, 3, curWidth , namespaceHeight );
		startX += curWidth + space;*/
	}
	_channelName[7].MoveWindow( startX+7*(curWidth + space), 4, curWidth+30, namespaceHeight );
	_channelCorrection[7].MoveWindow( startX+7*(curWidth + space), 1+namespaceHeight, curWidth , namespaceHeight );
	_waveFormWnd[7].MoveWindow( startX+7*(curWidth + space), 3+namespaceHeight*2, curWidth+30, wndHeight );//���һ�������+30����ʾʱ��

	for( int i = 8; i < 15; i ++ ){	
		_channelName[i].MoveWindow( startX+(i-8)*(curWidth + space), 4+namespaceHeight*2+wndHeight, curWidth, namespaceHeight );
		_channelCorrection[i].MoveWindow( startX+(i-8)*(curWidth + space), 1+namespaceHeight*3+wndHeight, curWidth , namespaceHeight );
		_waveFormWnd[i].MoveWindow( startX+(i-8)*(curWidth + space), 3+namespaceHeight*4+wndHeight, curWidth, wndHeight );
	}
	_channelName[15].MoveWindow( startX+7*(curWidth + space), 4+namespaceHeight*2+wndHeight, curWidth+30, namespaceHeight );
	_channelCorrection[15].MoveWindow( startX+7*(curWidth + space), 1+namespaceHeight*3+wndHeight, curWidth , namespaceHeight );
	_waveFormWnd[15].MoveWindow( startX+7*(curWidth + space), 3+namespaceHeight*4+wndHeight, curWidth+30, wndHeight );//���һ�������+30����ʾʱ��
}

void DlgRadarParameterConfig::AddRadarData( RadarData *lpData , int index )
{
	_waveFormWnd[index].addData( lpData );
}

void DlgRadarParameterConfig::OnEnKillfocusEditCorrection1(){
	//OnBnClickedButtonWaveFormTest();
}
void DlgRadarParameterConfig::OnEnKillfocusEditCorrection2(){
	//OnBnClickedButtonWaveFormTest();
}
void DlgRadarParameterConfig::OnEnKillfocusEditCorrection3(){
	//OnBnClickedButtonWaveFormTest();
}
void DlgRadarParameterConfig::OnEnKillfocusEditCorrection4(){
	//OnBnClickedButtonWaveFormTest();
}
void DlgRadarParameterConfig::OnEnKillfocusEditCorrection5(){
	//OnBnClickedButtonWaveFormTest();
}
void DlgRadarParameterConfig::OnEnKillfocusEditCorrection6(){
	//OnBnClickedButtonWaveFormTest();
}
void DlgRadarParameterConfig::OnEnKillfocusEditCorrection7(){
	//OnBnClickedButtonWaveFormTest();
}
void DlgRadarParameterConfig::OnEnKillfocusEditCorrection8(){
	//OnBnClickedButtonWaveFormTest();
}
void DlgRadarParameterConfig::OnEnKillfocusEditCorrection9(){
	//OnBnClickedButtonWaveFormTest();
}
void DlgRadarParameterConfig::OnEnKillfocusEditCorrection10(){
	//OnBnClickedButtonWaveFormTest();
}
void DlgRadarParameterConfig::OnEnKillfocusEditCorrection11(){
	//OnBnClickedButtonWaveFormTest();
}
void DlgRadarParameterConfig::OnEnKillfocusEditCorrection12(){
	//OnBnClickedButtonWaveFormTest();
}
void DlgRadarParameterConfig::OnEnKillfocusEditCorrection13(){
	//OnBnClickedButtonWaveFormTest();
}
void DlgRadarParameterConfig::OnEnKillfocusEditCorrection14(){
	//OnBnClickedButtonWaveFormTest();
}
void DlgRadarParameterConfig::OnEnKillfocusEditCorrection15(){
	//OnBnClickedButtonWaveFormTest();
}
void DlgRadarParameterConfig::OnEnKillfocusEditCorrection16(){
	//OnBnClickedButtonWaveFormTest();
}
