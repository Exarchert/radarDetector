// DialogCurCurve.cpp : ʵ���ļ�
//
#include "stdafx.h"
#include "Resource.h"
#include "RadarCurve.h"
#include "DialogThreeView.h"
#include <sstream>
#include <RadarCurve/RadarManager.h>
#include "COM/Tools/ProcessData.h"
#include <algorithm>

// CDialogCurCurve �Ի���

IMPLEMENT_DYNAMIC(CDialogThreeView, CDialog)

CDialogThreeView::CDialogThreeView( CWnd* pParent /*=NULL*/)
	: CDialog(CDialogThreeView::IDD, pParent)
{
	m_dChannelIndex=1;
	m_dDepthIndex=0;
	//_cfgGroupName = "curchannel";
	//
	
	//_dcValue = 0.0f;

	//_recordTotalWheelCount = 0;
	//_realTotalWheelCount = -1;

	//_wheelDistance = 0.0f;
	//_gpsDistance = 0.0f;

	////����Ƶ���ĸ���
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
	//	//�������1~16��Ϊʹ�ñ�׼��0~15�����m_channelVec
	//	if(!(stringVec[0].compare("")==0)){//spilt""��ʱ�����һ����һ��""������
	//		for (int i=0;i<stringVec.size();i++){
	//			m_channelVec.push_back(atoi(stringVec[i].c_str())-1);
	//		}
	//	}
	//	m_channelCount = m_channelVec.size();
	//}
	////hjl20210408����Ϊ�����Ƽ��ٶ�
	//float precLen = atoi ( cfg->get("radar", "preclen").c_str() );
	//float fJihuaAccuracy = (float)atoi( cfg->get("radar", "precision").c_str() );
	//float fwheelPricise = RadarManager::Instance()->GetTrueAccuracyFromPreclenAndPrecindex(precLen,fJihuaAccuracy);//ʵ�ʵ����
	//int iSampleCountIndex = atoi ( cfg->get("radar", "sample").c_str());
	//int iSampleCount = RadarManager::Instance()->getSampCount( iSampleCountIndex );//������
	//_fRecommendableSpeed=fwheelPricise*10000/((float)iSampleCount*20/3)*3.6;
}

CDialogThreeView::~CDialogThreeView()
{
}

void CDialogThreeView::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control( pDX, IDC_3D, m_3DWnd);
	DDX_Text(pDX, IDC_EDIT_CHANNEL, m_dChannelIndex );//�������Ӧͨ��
	DDX_Text(pDX, IDC_EDIT_DEPTH, m_dDepthIndex );//�������Ӧ���
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
	DDX_Control( pDX, IDC_LONLAT, _lonlat);//hjl6.5 ���ڶ�ʱ���¾�γ��
	DDX_Control( pDX, IDC_GPSCONDITION, _gpsCondition);//hjl6.5 ���ڶ�ʱ���¾�γ��
	DDX_Control( pDX, IDC_SPEED, _speed);//hjl20200907 ���ڶ�ʱ�����ٶ�
	DDX_Control( pDX, IDC_RECOMMENDABLE_SPEED, _recommendableSpeed);//hjl20210408 ���ڱ�ʾ�Ƽ��ٶ�
	DDX_Control( pDX, IDC_DATAMISSING, _dataMissing);//hjl20200907 ���ڶ�ʱ���¶������
	DDX_Control( pDX, IDC_DISTANCE, _distance);//hjl20210623 ���ڶ�ʱ���¾���
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
//	ON_EN_KILLFOCUS(IDC_EDIT_CHANNEL, &CDialogThreeView::OnEnKillfocusEditChannel)
//	ON_EN_KILLFOCUS(IDC_EDIT_DEPTH, &CDialogThreeView::OnEnKillfocusEditDepth)
//ON_EN_KILLFOCUS(IDC_EDIT_CHANNEL, &CDialogThreeView::OnEnKillfocusEditChannel)
//ON_EN_KILLFOCUS(IDC_EDIT_DEPTH, &CDialogThreeView::OnEnKillfocusEditDepth)
ON_BN_CLICKED(IDC_BUTTON_SWITCH, &CDialogThreeView::OnBnClickedButtonSwitch)
END_MESSAGE_MAP()


// CDialogCurCurve ��Ϣ�������

BOOL CDialogThreeView::OnInitDialog()
{
	CDialog::OnInitDialog();

	//m_pMenu.LoadMenu(IDR_MENU_SHOW_CHANNEL);

	//std::vector<int> pChannelRemoved = RemoveChannels();

	//for (int i = pChannelRemoved.size()-1; i > -1; i--){//ɾ��֮����indexҲ���� ���Դ���Сɾ��������� ������ɾ����С
	//	//intתstring
	//	std::stringstream ss;
	//	ss<<pChannelRemoved[i];
	//	int menuIndex;
	//	ss>>menuIndex;
	//	//ȥ���˵�
	//	BOOL removeResult = m_pMenu.GetSubMenu(0)->RemoveMenu(menuIndex,MF_BYPOSITION);
	//	if(!removeResult){::AfxMessageBox(L"ͨ������ʧЧ��" );}
	//}

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	
	int nScreenWidth, nScreenHeight;  
	nScreenWidth = GetSystemMetrics(SM_CXSCREEN);  
	nScreenHeight = GetSystemMetrics(SM_CYSCREEN);
	nScreenHeight = (nScreenHeight-100)/4;
	if ( nScreenHeight > 500 ){
		nScreenHeight = 500;
	}
	MoveWindow( 0, nScreenHeight*2 + 50, nScreenWidth/2, nScreenHeight*2 );


	ConfigureSet *lpSet = RadarManager::Instance()->getConfigureSet();
	int dSmpNumIndex = atoi ( lpSet->get("radar", "sample").c_str());
	int dSampRateIndex = atoi ( lpSet->get("radar", "sampleratio" ).c_str() );
	int dDielectricIndex = atoi ( lpSet->get("radar", "dielectric" ).c_str() );
	
	m_3DWnd.SetSampleCount( RadarManager::Instance()->getSampCount( dSmpNumIndex ) );
	m_3DWnd.SetSampleRatio( RadarManager::Instance()->getSampRatio( dSampRateIndex, 0 ) );
	m_3DWnd.SetDielectric( RadarManager::Instance()->getDielectric( dDielectricIndex ) );
	m_3DWnd.setChannelIndex(m_dChannelIndex-1);
	m_3DWnd.setDepthIndex(m_dDepthIndex);

	for(int i=0;i<12;i++){
		std::stringstream ss;
		ss << i;
		m_3DWnd.setCorrection( atoi( lpSet->get("correction", ss.str()).c_str() ), i );
	}

	m_3DWnd.StartDraw();
	m_3DWnd.ShowWindow( SW_SHOW );

	resizeChannelWnd();

	//UpdateData( FALSE );

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
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

//void CDialogThreeView::OnEnKillfocusEditChannel(){
//	UpdateData( FALSE );
//	if(m_dChannelIndex>12){
//		m_dChannelIndex=12;
//	}
//	if(m_dChannelIndex<1){
//		m_dChannelIndex=1;
//	}
//	m_3DWnd.setChannelIndex(m_dChannelIndex-1);
//	UpdateData( FALSE );
//}

//void CDialogThreeView::OnEnKillfocusEditDepth(){
//	UpdateData( FALSE );
//	ConfigureSet *lpSet = RadarManager::Instance()->getConfigureSet();
//	int iSmpNum = atoi ( lpSet->get("radar", "sample").c_str());
//	if(m_dDepthIndex>iSmpNum-5){
//		m_dDepthIndex=iSmpNum-5;
//	}
//	if(m_dDepthIndex<0){
//		m_dDepthIndex=0;
//	}
//	m_3DWnd.setDepthIndex(m_dDepthIndex);
//	UpdateData( FALSE );
//}


void CDialogThreeView::OnBnClickedButtonSwitch()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData( TRUE );
	if(m_dChannelIndex>12){
		m_dChannelIndex=12;
	}
	if(m_dChannelIndex<1){
		m_dChannelIndex=1;
	}
	m_3DWnd.setChannelIndex(m_dChannelIndex-1);
	ConfigureSet *lpSet = RadarManager::Instance()->getConfigureSet();
	int iSmpNum = RadarManager::Instance()->getSampCount( atoi ( lpSet->get("radar", "sample").c_str()) );
	if(m_dDepthIndex>iSmpNum-5){
		m_dDepthIndex=iSmpNum-5;
	}
	if(m_dDepthIndex<0){
		m_dDepthIndex=0;
	}
	m_3DWnd.setDepthIndex(m_dDepthIndex);
	UpdateData( FALSE );
}
