// DialogCurCurve.cpp : 实现文件
//
#include "stdafx.h"
#include "Resource.h"
#include "RadarCurve.h"
#include "DialogThreeView.h"
#include <sstream>
#include <RadarCurve/RadarManager.h>
#include "COM/Tools/ProcessData.h"
#include <algorithm>

// CDialogCurCurve 对话框

IMPLEMENT_DYNAMIC(CDialogThreeView, CDialog)

CDialogThreeView::CDialogThreeView( CWnd* pParent /*=NULL*/)
	: CDialog(CDialogThreeView::IDD, pParent)
{
	m_dChannelIndex=0;
	m_dDepthIndex=0;
	m_bAutoDisplay=false;
	//_cfgGroupName = "curchannel";
	//
	
	//_dcValue = 0.0f;

	//_recordTotalWheelCount = 0;
	//_realTotalWheelCount = -1;

	//_wheelDistance = 0.0f;
	//_gpsDistance = 0.0f;

	////计算频道的个数
	//ConfigureSet *cfg = RadarManager::Instance()->getConfigureSet();
	//if (cfg)
	//{
	//	std::string strScreen;
	//	if (pScreenType == separateScreen::screenFirst){
	//		strScreen = "first";
	//	}else if (pScreenType == separateScreen::screenSecond){
	//		strScreen = "second";
	//	}else if (pScreenType == separateScreen::screenThird){
	//		strScreen = "third";
	//	}else if (pScreenType == separateScreen::screenFourth){
	//		strScreen = "fourth";
	//	}
	//	std::string strChannels = cfg->get("screen",strScreen);

	//	std::vector<std::string> stringVec;
	//	stringVec = COM::split(strChannels,",");
	//	//将输入的1~16变为使用标准的0~15并存进m_channelVec
	//	if(!(stringVec[0].compare("")==0)){//spilt""的时候产生一个有一个""的数组
	//		for (int i=0;i<stringVec.size();i++){
	//			m_channelVec.push_back(atoi(stringVec[i].c_str())-1);
	//		}
	//	}
	//	m_channelCount = m_channelVec.size();
	//}
	////hjl20210408以下为计算推荐速度
	//float precLen = atoi ( cfg->get("radar", "preclen").c_str() );
	//float fJihuaAccuracy = (float)atoi( cfg->get("radar", "precision").c_str() );
	//float fwheelPricise = RadarManager::Instance()->GetTrueAccuracyFromPreclenAndPrecindex(precLen,fJihuaAccuracy);//实际道间距
	//int iSampleCountIndex = atoi ( cfg->get("radar", "sample").c_str());
	//int iSampleCount = RadarManager::Instance()->getSampCount( iSampleCountIndex );//采样点
	//_fRecommendableSpeed=fwheelPricise*10000/((float)iSampleCount*20/3)*3.6;
}

CDialogThreeView::~CDialogThreeView()
{
}

void CDialogThreeView::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control( pDX, IDC_3D, m_3DWnd);
	DDX_Text(pDX, IDC_EDIT_CHANNEL, m_dChannelIndex );//纵剖面对应通道
	DDX_Control(pDX, IDC_COMBO_CHANNEL, m_comboBoxChannel);//纵剖面对应通道
	DDX_Text(pDX, IDC_EDIT_DEPTH, m_dDepthIndex );//横切面对应深度
	DDX_Check(pDX, IDC_CHECK_AUTODISPLAY, m_bAutoDisplay);
	DDX_Control( pDX, IDC_STATIC_CURRENTCHANNEL, m_csCurrentChannel);//hjl6.5 用于定时更新经纬度
	/*
	
	DDX_Control( pDX, IDC_CURVE_01, _curveWnd[1]);
	DDX_Control( pDX, IDC_CURVE_02, _curveWnd[2]);
	DDX_Control( pDX, IDC_CURVE_03, _curveWnd[3]);
	DDX_Control( pDX, IDC_CURVE_04, _curveWnd[4]);
	DDX_Control( pDX, IDC_CURVE_05, _curveWnd[5]);
	DDX_Control( pDX, IDC_CURVE_06, _curveWnd[6]);
	DDX_Control( pDX, IDC_CURVE_07, _curveWnd[7]);
	DDX_Control( pDX, IDC_CURVE_08, _curveWnd[8]);
	DDX_Control( pDX, IDC_CURVE_09, _curveWnd[9]);
	DDX_Control( pDX, IDC_CURVE_10, _curveWnd[10]);
	DDX_Control( pDX, IDC_CURVE_11, _curveWnd[11]);
	DDX_Control( pDX, IDC_CURVE_12, _curveWnd[12]);
	DDX_Control( pDX, IDC_CURVE_13, _curveWnd[13]);
	DDX_Control( pDX, IDC_CURVE_14, _curveWnd[14]);
	DDX_Control( pDX, IDC_CURVE_15, _curveWnd[15]);
	DDX_Control( pDX, IDC_STATIC_CHANNEL_00, _channelName[0]);
	DDX_Control( pDX, IDC_STATIC_CHANNEL_01, _channelName[1]);
	DDX_Control( pDX, IDC_STATIC_CHANNEL_02, _channelName[2]);
	DDX_Control( pDX, IDC_STATIC_CHANNEL_03, _channelName[3]);
	DDX_Control( pDX, IDC_STATIC_CHANNEL_04, _channelName[4]);
	DDX_Control( pDX, IDC_STATIC_CHANNEL_05, _channelName[5]);
	DDX_Control( pDX, IDC_STATIC_CHANNEL_06, _channelName[6]);
	DDX_Control( pDX, IDC_STATIC_CHANNEL_07, _channelName[7]);
	DDX_Control( pDX, IDC_STATIC_CHANNEL_08, _channelName[8]);
	DDX_Control( pDX, IDC_STATIC_CHANNEL_09, _channelName[9]);
	DDX_Control( pDX, IDC_STATIC_CHANNEL_10, _channelName[10]);
	DDX_Control( pDX, IDC_STATIC_CHANNEL_11, _channelName[11]);
	DDX_Control( pDX, IDC_STATIC_CHANNEL_12, _channelName[12]);
	DDX_Control( pDX, IDC_STATIC_CHANNEL_13, _channelName[13]);
	DDX_Control( pDX, IDC_STATIC_CHANNEL_14, _channelName[14]);
	DDX_Control( pDX, IDC_STATIC_CHANNEL_15, _channelName[15]);

	//DDX_Text( pDX, IDC_EDIT_LONLAT, _lonlat);
	DDX_Control( pDX, IDC_LONLAT, _lonlat);//hjl6.5 用于定时更新经纬度
	DDX_Control( pDX, IDC_GPSCONDITION, _gpsCondition);//hjl6.5 用于定时更新经纬度
	DDX_Control( pDX, IDC_SPEED, _speed);//hjl20200907 用于定时更新速度
	DDX_Control( pDX, IDC_RECOMMENDABLE_SPEED, _recommendableSpeed);//hjl20210408 用于表示推荐速度
	DDX_Control( pDX, IDC_DATAMISSING, _dataMissing);//hjl20200907 用于定时更新丢道情况
	DDX_Control( pDX, IDC_DISTANCE, _distance);//hjl20210623 用于定时更新距离
	*/

}


BEGIN_MESSAGE_MAP(CDialogThreeView, CDialog)
	//ON_MESSAGE( WM_NEW_RADAR_DATA, &CDialogThreeView::OnRadarData )
	/*
	ON_COMMAND(ID_SHOW_CHANNEL_00, &CDialogCurCurve::OnShowChannel00)
	ON_COMMAND(ID_SHOW_CHANNEL_01, &CDialogCurCurve::OnShowChannel01)
	ON_COMMAND(ID_SHOW_CHANNEL_02, &CDialogCurCurve::OnShowChannel02)
	ON_COMMAND(ID_SHOW_CHANNEL_03, &CDialogCurCurve::OnShowChannel03)
	ON_COMMAND(ID_SHOW_CHANNEL_04, &CDialogCurCurve::OnShowChannel04)
	ON_COMMAND(ID_SHOW_CHANNEL_05, &CDialogCurCurve::OnShowChannel05)
	ON_COMMAND(ID_SHOW_CHANNEL_06, &CDialogCurCurve::OnShowChannel06)
	ON_COMMAND(ID_SHOW_CHANNEL_07, &CDialogCurCurve::OnShowChannel07)
	ON_COMMAND(ID_SHOW_CHANNEL_08, &CDialogCurCurve::OnShowChannel08)
	ON_COMMAND(ID_SHOW_CHANNEL_09, &CDialogCurCurve::OnShowChannel09)
	ON_COMMAND(ID_SHOW_CHANNEL_10, &CDialogCurCurve::OnShowChannel10)
	ON_COMMAND(ID_SHOW_CHANNEL_11, &CDialogCurCurve::OnShowChannel11)
	ON_COMMAND(ID_SHOW_CHANNEL_12, &CDialogCurCurve::OnShowChannel12)
	ON_COMMAND(ID_SHOW_CHANNEL_13, &CDialogCurCurve::OnShowChannel13)
	ON_COMMAND(ID_SHOW_CHANNEL_14, &CDialogCurCurve::OnShowChannel14)
	ON_COMMAND(ID_SHOW_CHANNEL_15, &CDialogCurCurve::OnShowChannel15)
	ON_BN_CLICKED(IDOK, &CDialogCurCurve::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDialogCurCurve::OnBnClickedCancel)
	ON_WM_CONTEXTMENU()
	ON_WM_SHOWWINDOW()
	ON_WM_TIMER()
	ON_WM_CLOSE()
	ON_WM_SIZE()
	ON_WM_KEYDOWN()*/
	//ON_EN_KILLFOCUS(IDC_EDIT_CHANNEL, &CDialogThreeView::OnEnKillfocusEditChannel)
	//ON_EN_KILLFOCUS(IDC_EDIT_DEPTH, &CDialogThreeView::OnEnKillfocusEditDepth)
	//ON_EN_KILLFOCUS(IDC_EDIT_CHANNEL, &CDialogThreeView::OnEnKillfocusEditChannel)
	//ON_EN_KILLFOCUS(IDC_EDIT_DEPTH, &CDialogThreeView::OnEnKillfocusEditDepth)
	ON_BN_CLICKED(IDC_BUTTON_SWITCH, &CDialogThreeView::OnBnClickedButtonSwitch)
	ON_BN_CLICKED(IDC_CHECK_AUTODISPLAY, &CDialogThreeView::OnBnClickedCheckAutodisplay)
END_MESSAGE_MAP()


/*std::string intToString(int n){
	std::stringstream ss;
	ss<<n;
	return ss.str();
}*/
// CDialogCurCurve 消息处理程序

BOOL CDialogThreeView::OnInitDialog()
{
	CDialog::OnInitDialog();

	ConfigureSet *lpSet = RadarManager::Instance()->getConfigureSet();
	
	m_dChannelIndex=atoi( lpSet->get("threeDWindow", "selectedChannelIndex").c_str());
	m_dDepthIndex=atoi( lpSet->get("threeDWindow", "selectedDepth").c_str());

	int nSampleNumIndex = atoi ( lpSet->get("radar", "sample").c_str());
	int nSampleRateIndex = atoi ( lpSet->get("radar", "sampleratio" ).c_str() );
	int nDielectricIndex = atoi ( lpSet->get("radar", "dielectric" ).c_str() );
	
	m_3DWnd.SetChannelCount( RadarManager::Instance()->GetTrueChannelCount() );
	m_3DWnd.SetSettingChannelCount( RadarManager::Instance()->GetSettingChannelCount() );
	m_3DWnd.SetSampleCount( RadarManager::Instance()->getSampCount( nSampleNumIndex ) );
	m_3DWnd.SetSampleRatio( RadarManager::Instance()->getSampRatio( nSampleRateIndex, 0 ) );
	m_3DWnd.SetDielectric( RadarManager::Instance()->getDielectric( nDielectricIndex ) );
	m_3DWnd.setChannelIndex(m_dChannelIndex);
	m_3DWnd.setDepthIndex(m_dDepthIndex);

	std::string vecStrTemp[28]={"01","02","03","04","05","06","07","08","09","10","11","12","13","14","15","16","17","18","19","20","21","22","23","24","25","26","27","28"};
	int nChannelCount=RadarManager::Instance()->GetTrueChannelCount();
	int nSettingChannelCount=RadarManager::Instance()->GetSettingChannelCount();
	/*if(nChannelCount==16){
		for (int i=0;i<28;i++){
			CString cstrTemp;
			cstrTemp=vecStrTemp[i].c_str();
			m_comboBoxChannel.AddString(cstrTemp);
		}
	}else{
		for (int i=0;i<nChannelCount;i++){
			CString cstrTemp;
			cstrTemp=vecStrTemp[i].c_str();
			m_comboBoxChannel.AddString(cstrTemp);
		}
	}*/
	for (int i=0;i<nSettingChannelCount;i++){
		CString cstrTemp;
		cstrTemp=vecStrTemp[i].c_str();
		m_comboBoxChannel.AddString(cstrTemp);
	}
	if(m_dChannelIndex<nChannelCount){
		m_comboBoxChannel.SetCurSel ( m_dChannelIndex );
	}else{//上一次用16这次用了6 所选通道数大于本次使用最大通道数的时候
		m_dChannelIndex=0;
		{
			std::stringstream ss;
			ss << m_dDepthIndex;
			lpSet->set("threeDWindow", "selectedChannelIndex", ss.str());
		}
		lpSet->write();
		m_comboBoxChannel.SetCurSel ( m_dChannelIndex );
	}
	
	int nScreenWidth, nScreenHeight;  
	nScreenWidth = GetSystemMetrics(SM_CXSCREEN);  
	nScreenHeight = GetSystemMetrics(SM_CYSCREEN);
	nScreenHeight = (nScreenHeight-100)/4;
	if ( nScreenHeight > 500 ){
		nScreenHeight = 500;
	}
	//MoveWindow( 5, nScreenHeight*2 + 50, nScreenWidth/2-10, nScreenHeight*2 );
	MoveWindow( 0, nScreenHeight*2 + 50, nScreenWidth/2, nScreenHeight*2 );

	for(int i=0;i<16;i++){
		std::stringstream ss;
		ss << i;
		m_3DWnd.SetCorrection( atoi( lpSet->get("correction", ss.str()).c_str() ), i );
	}

	time_t mytime = time(NULL);
	m_3DWnd.setStartTime(mytime);
	m_3DWnd.setAutoDisplayFlag(false);

	m_3DWnd.StartDraw();
	m_3DWnd.ShowWindow( SW_SHOW );

	resizeChannelWnd();
	m_cstrCurrentChannel.Format(L"当前通道：%d",m_dChannelIndex+1);	
	m_csCurrentChannel.SetWindowText( m_cstrCurrentChannel );

	UpdateData( FALSE );

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

/*
LRESULT CDialogThreeView::OnRadarData( WPARAM wParam, LPARAM lParam )
{
	RadarData *lpData = (RadarData*)wParam;
	int index = lParam;
	if ( !lpData )
	{
		return 0;
	}
	if ( index >= MAX_CHANNEL )
	{
		return 0;
	}
	_curveWnd[index].addRadarDataGroup( lpData );
	return 0;
}*/

void CDialogThreeView::addGroupRadarData( std::vector<osg::ref_ptr<RadarData>> radarDataGroup ){
	m_3DWnd.addGroupData( radarDataGroup );
}

/*
void CDialogThreeView::addRadarData( RadarData *lpData, int index ){
	m_3DWnd.addDataByIndex( lpData, index );
}*/

void CDialogThreeView::resizeChannelWnd(){
	CRect fullRect;
	GetClientRect( fullRect );
	m_3DWnd.MoveWindow( 0, 25, fullRect.Width(), fullRect.Height()-25 );
}


void CDialogThreeView::OnBnClickedButtonSwitch()
{
	ConfigureSet *cfgSet = RadarManager::Instance()->getConfigureSet();

	UpdateData( TRUE );
	m_bAutoDisplay=false;
	m_3DWnd.setAutoDisplayFlag(m_bAutoDisplay);
	m_dChannelIndex=m_comboBoxChannel.GetCurSel();
	m_3DWnd.setArtificialFlag(false);
	if(m_dChannelIndex>=12){
		m_3DWnd.setArtificialFlag(true);
	}
	/*if(m_dChannelIndex>=16){
		m_dChannelIndex=m_dChannelIndex-16;
	}*/
	m_3DWnd.setChannelIndex(m_dChannelIndex);
	
	int iSmpNum = RadarManager::Instance()->getSampCount( atoi ( cfgSet->get("radar", "sample").c_str()) );
	if(m_dDepthIndex>iSmpNum-5){
		m_dDepthIndex=iSmpNum-5;
	}
	if(m_dDepthIndex<0){
		m_dDepthIndex=0;
	}
	m_3DWnd.setDepthIndex(m_dDepthIndex);
	UpdateData( FALSE );

	//储存通道跟深度的选择
	{
		std::stringstream ss;
		ss << m_dChannelIndex;
		cfgSet->set("threeDWindow", "selectedChannelIndex", ss.str());
	}
	{
		std::stringstream ss;
		ss << m_dDepthIndex;
		cfgSet->set("threeDWindow", "selectedDepth", ss.str());
	}
	cfgSet->write();

	//显示当前显示的通道index
	setCurrentIndexDisplay(m_dChannelIndex);
}

void CDialogThreeView::OnBnClickedCheckAutodisplay()
{
	UpdateData( TRUE );
	m_3DWnd.setAutoDisplayFlag(m_bAutoDisplay);
	UpdateData( FALSE );
}

void CDialogThreeView::setCurrentIndexDisplay( int currentChannelIndex )
{
	m_cstrCurrentChannel.Format(L"当前通道：%d",currentChannelIndex+1);	
	m_csCurrentChannel.SetWindowText( m_cstrCurrentChannel );
}
