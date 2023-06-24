// DialogCurCurve.cpp : ʵ���ļ�
//
#include "stdafx.h"
#include "Resource.h"
#include "RadarCurve.h"
#include "DialogTERDisplay.h"
#include <sstream>
#include <RadarCurve/RadarManager.h>
#include "COM/Tools/ProcessData.h"
#include <algorithm>

// CDialogTERDisplay �Ի���

IMPLEMENT_DYNAMIC(CDialogTERDisplay, CDialog)

CDialogTERDisplay::CDialogTERDisplay(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogTERDisplay::IDD, pParent)
{
	_cfgGroupName = "curchannel";
	
	_dcValue = 0.0f;

	m_nRecordTotalWheelCount=0;
	m_nRealTotalWheelCount=0;

	_wheelDistance = 0.0f;
	_gpsDistance = 0.0f;

	//����Ƶ���ĸ���
	ConfigureSet *cfg = RadarManager::Instance()->getConfigureSet();
	/*if (cfg)
	{
		std::string strScreen;
		if (pScreenType == separateScreen::screenFirst){
			strScreen = "first";
		}else if (pScreenType == separateScreen::screenSecond){
			strScreen = "second";
		}else if (pScreenType == separateScreen::screenThird){
			strScreen = "third";
		}else if (pScreenType == separateScreen::screenFourth){
			strScreen = "fourth";
		}
		std::string strChannels = cfg->get("screen",strScreen);

		std::vector<std::string> stringVec;
		stringVec = COM::split(strChannels,",");
		//�������1~16��Ϊʹ�ñ�׼��0~15�����m_channelVec
		if(!(stringVec[0].compare("")==0)){//spilt""��ʱ�����һ����һ��""������
			for (int i=0;i<stringVec.size();i++){
				m_channelVec.push_back(atoi(stringVec[i].c_str())-1);
			}
		}
		m_channelCount = m_channelVec.size();
	}*/
	/*
	//hjl20210408����Ϊ�����Ƽ��ٶ�
	float precLen = atoi ( cfg->get("radar", "preclen").c_str() );
	//int precIndex = atoi( cfg->get("radar", "precindex").c_str() );
	//float fJihuaAccuracy = RadarManager::Instance()->GetJihuaAccuracy(precIndex);
	float fJihuaAccuracy = (float)atoi( cfg->get("radar", "precision").c_str() );
	float fwheelPricise = RadarManager::Instance()->GetTrueAccuracyFromPreclenAndPrecindex(precLen,fJihuaAccuracy);//ʵ�ʵ����
	int iSampleCountIndex = atoi ( cfg->get("radar", "sample").c_str());
	int iSampleCount = RadarManager::Instance()->getSampCount( iSampleCountIndex );//������
	//_fRecommendableSpeed=fwheelPricise*10000*temp2*3.6;
	_fRecommendableSpeed=fwheelPricise*10000/((float)iSampleCount*20/3)*3.6;
	//m_nInformationIndex = atoi ( cfg->get("screen", "index").c_str() );*/

}

CDialogTERDisplay::~CDialogTERDisplay()
{
	/*for(int i=0;i<CHANNELCOUNT;i++){
		m_arrnRecordTotalWheelCount[i]=0;
		m_arrnRealTotalWheelCount[i]=0;
	}*/
	m_nRecordTotalWheelCount=0;
	m_nRealTotalWheelCount=0;

}

void CDialogTERDisplay::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control( pDX, IDC_TER_DISPLAY, m_TERDisplayWnd );

	//DDX_Text( pDX, IDC_EDIT_LONLAT, _lonlat);
	/*
	DDX_Control( pDX, IDC_GPSCONDITION, _gpsCondition);//hjl6.5 ���ڶ�ʱ���¾�γ��
	DDX_Control( pDX, IDC_SPEED, _speed);//hjl20200907 ���ڶ�ʱ�����ٶ�
	DDX_Control( pDX, IDC_RECOMMENDABLE_SPEED, _recommendableSpeed);//hjl20210408 ���ڱ�ʾ�Ƽ��ٶ�
	DDX_Control( pDX, IDC_DATAMISSING, _dataMissing);//hjl20200907 ���ڶ�ʱ���¶������
	DDX_Control( pDX, IDC_DISTANCE, _distance);//hjl20210623 ���ڶ�ʱ���¾���*/

	DDX_Control( pDX, IDC_TER_LONLAT, _lonlat);
	DDX_Control( pDX, IDC_TER_SPEED, _speed);
	DDX_Control( pDX, IDC_TER_DATAMISSING, _dataMissing);
	DDX_Control( pDX, IDC_TER_DISTANCE, _distance);
}


BEGIN_MESSAGE_MAP(CDialogTERDisplay, CDialog)
	//ON_MESSAGE( WM_NEW_RADAR_DATA, &CDialogTERDisplay::OnRadarData )
	/*ON_COMMAND(ID_SHOW_CHANNEL_00, &CDialogTERDisplay::OnShowChannel00)
	ON_COMMAND(ID_SHOW_CHANNEL_01, &CDialogTERDisplay::OnShowChannel01)
	ON_COMMAND(ID_SHOW_CHANNEL_02, &CDialogTERDisplay::OnShowChannel02)
	ON_COMMAND(ID_SHOW_CHANNEL_03, &CDialogTERDisplay::OnShowChannel03)
	ON_COMMAND(ID_SHOW_CHANNEL_04, &CDialogTERDisplay::OnShowChannel04)
	ON_COMMAND(ID_SHOW_CHANNEL_05, &CDialogTERDisplay::OnShowChannel05)
	ON_COMMAND(ID_SHOW_CHANNEL_06, &CDialogTERDisplay::OnShowChannel06)
	ON_COMMAND(ID_SHOW_CHANNEL_07, &CDialogTERDisplay::OnShowChannel07)
	ON_COMMAND(ID_SHOW_CHANNEL_08, &CDialogTERDisplay::OnShowChannel08)
	ON_COMMAND(ID_SHOW_CHANNEL_09, &CDialogTERDisplay::OnShowChannel09)
	ON_COMMAND(ID_SHOW_CHANNEL_10, &CDialogTERDisplay::OnShowChannel10)
	ON_COMMAND(ID_SHOW_CHANNEL_11, &CDialogTERDisplay::OnShowChannel11)
	ON_COMMAND(ID_SHOW_CHANNEL_12, &CDialogTERDisplay::OnShowChannel12)
	ON_COMMAND(ID_SHOW_CHANNEL_13, &CDialogTERDisplay::OnShowChannel13)
	ON_COMMAND(ID_SHOW_CHANNEL_14, &CDialogTERDisplay::OnShowChannel14)
	ON_COMMAND(ID_SHOW_CHANNEL_15, &CDialogTERDisplay::OnShowChannel15)
	ON_COMMAND(ID_SHOW_CHANNEL_16, &CDialogTERDisplay::OnShowChannel16)
	ON_COMMAND(ID_SHOW_CHANNEL_17, &CDialogTERDisplay::OnShowChannel17)
	ON_COMMAND(ID_SHOW_CHANNEL_18, &CDialogTERDisplay::OnShowChannel18)
	ON_COMMAND(ID_SHOW_CHANNEL_19, &CDialogTERDisplay::OnShowChannel19)
	ON_COMMAND(ID_SHOW_CHANNEL_20, &CDialogTERDisplay::OnShowChannel20)
	ON_COMMAND(ID_SHOW_CHANNEL_21, &CDialogTERDisplay::OnShowChannel21)
	ON_COMMAND(ID_SHOW_CHANNEL_22, &CDialogTERDisplay::OnShowChannel22)
	ON_COMMAND(ID_SHOW_CHANNEL_23, &CDialogTERDisplay::OnShowChannel23)
	ON_COMMAND(ID_SHOW_CHANNEL_24, &CDialogTERDisplay::OnShowChannel24)
	ON_COMMAND(ID_SHOW_CHANNEL_25, &CDialogTERDisplay::OnShowChannel25)
	ON_COMMAND(ID_SHOW_CHANNEL_26, &CDialogTERDisplay::OnShowChannel26)
	ON_COMMAND(ID_SHOW_CHANNEL_27, &CDialogTERDisplay::OnShowChannel27)*/

	ON_BN_CLICKED(IDOK, &CDialogTERDisplay::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDialogTERDisplay::OnBnClickedCancel)
	ON_WM_CONTEXTMENU()
	ON_WM_SHOWWINDOW()
	ON_WM_TIMER()
	ON_WM_CLOSE()
	ON_WM_SIZE()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()


// CDialogTERDisplay ��Ϣ�������

BOOL CDialogTERDisplay::OnInitDialog()
{
	CDialog::OnInitDialog();
	m_pMenu.LoadMenu(IDR_MENU_SHOW_CHANNEL);

	std::vector<int> pChannelRemoved = RemoveChannels();

	for (int i = pChannelRemoved.size()-1; i > -1; i--){//ɾ��֮����indexҲ���� ���Դ���Сɾ��������� ������ɾ����С
		//intתstring
		std::stringstream ss;
		ss<<pChannelRemoved[i];
		int menuIndex;
		ss>>menuIndex;
		//ȥ���˵�
		BOOL removeResult = m_pMenu.GetSubMenu(0)->RemoveMenu(menuIndex,MF_BYPOSITION);
		if(!removeResult){::AfxMessageBox(L"ͨ������ʧЧ��" );}
	}

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	int nScreenWidth, nScreenHeight;  
	nScreenWidth = GetSystemMetrics(SM_CXSCREEN);  
	nScreenHeight = GetSystemMetrics(SM_CYSCREEN);
	nScreenHeight = (nScreenHeight-100)/2;
	if ( nScreenHeight > 500 ){
		nScreenHeight = 500;
	}
	
	MoveWindow( nScreenWidth/2, 50, nScreenWidth/2, nScreenHeight );

	ConfigureSet *lpSet = RadarManager::Instance()->getConfigureSet();

	int nSampleCount = atoi ( lpSet->get("receive", "sampleCount").c_str());
	int nSampleRateIndex = atoi ( lpSet->get("receive", "samplingRate").c_str() );
	
	m_TERDisplayWnd.SetSampleCount(nSampleCount);
	m_TERDisplayWnd.SetSampleRatio(RadarManager::Instance()->getSampRatio( nSampleRateIndex, 0 ));

	m_TERDisplayWnd.StartDraw();
	m_TERDisplayWnd.ShowWindow( SW_SHOW );
	

	/*
	//����Ϊ�ٶȼ���ģ�鲿��
	_tLastTime=time(NULL);
	_fLastLen=0;
	_fCurLen=0;
	for(int i=0;i<CHANNELCOUNT;i++){
	//for(int i=0;i<8;i++){
		_iDisplayFlag[i]=0;
	}
	_iDisplayExtractValue = atoi(RadarManager::Instance()->getConfigureSet()->get("displayExtract", "displayExtractValue").c_str());
	*/
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

/*LRESULT CDialogTERDisplay::OnTERData( WPARAM wParam, LPARAM lParam )
{
	RadarData *lpData = (TERData*)wParam;
	if ( !lpData ){
		return 0;
	}
	TERDisplayWnd.addTERData( lpData );
	return 0;
}*/

void CDialogTERDisplay::AddTERData( TERData *lpData ){
	
	/*if(m_arrnRecordTotalWheelCount[index] == lpData->getRealWheelCount()){
		return;
	}*/
	//����Ϊ���㶪�����;
	
	if(lpData->getRecordWheelCount()!=0){
		m_nRecordTotalWheelCount = lpData->getRecordWheelCount();
		m_nRealTotalWheelCount = m_nRealTotalWheelCount+1;
	}else{
		m_nRecordTotalWheelCount=m_nRecordTotalWheelCount+1;
		lpData->setRecordWheelCount(m_nRecordTotalWheelCount);
	}

	m_TERDisplayWnd.addTERData( lpData );

	//���ڶ�ʱ���¾�γ����ʾ
	_position = lpData->getPosition();
	m_cstrLonlat.Format(L"��γ�ȣ���%0.10f,%0.10f��",_position.x(),_position.y());	

	//���ڶ�ʱ���¾���
	_wheelDistance = lpData->getPrecLen();
	_gpsDistance = lpData->getLen();

	//���ڼ����ٶ�
	_fCurLen=lpData->getPrecLen();
}

/*
void CDialogTERDisplay::DeleteRadarData( int index )
{
	if ( index >= CHANNELCOUNT ){
		return ;
	}
	_curveWnd[index].deleteData();
}*/

void CDialogTERDisplay::OnBnClickedOk()
{
	OnOK();
}

void CDialogTERDisplay::OnBnClickedCancel()
{
	OnCancel();
}

/*void CDialogTERDisplay::changeChannel( int channelIndex, bool flag )
{
	if ( channelIndex >= CHANNELCOUNT ){
		return ;
	}
	if ( flag ){//��ʾ��ͨ������ֵ����ʼ��ͼ
		ConfigureSet *lpSet = RadarManager::Instance()->getConfigureSet();
		int iSmpNum = atoi ( lpSet->get("radar", "sample").c_str());
		int iSampRate = atoi ( lpSet->get("radar", "sampleratio" ).c_str() );
		_curveWnd[channelIndex].SetSampleCount( RadarManager::Instance()->getSampCount( iSmpNum ) );
		_curveWnd[channelIndex].SetSampleRatio( RadarManager::Instance()->getSampRatio( iSampRate, channelIndex ) );

		std::stringstream ss;
		//ss << channelIndex+12;//
		ss << channelIndex;//20211226
		_curveWnd[channelIndex].setCorrection( atoi( lpSet->get("correction", ss.str()).c_str() ) );

		_curveWnd[channelIndex].StartDraw();
		_curveWnd[channelIndex].ShowWindow( SW_SHOW );
		_channelName[channelIndex].ShowWindow( SW_SHOW );

		USES_CONVERSION;

		RadarManager *lpProject = RadarManager::Instance();
		if ( lpProject ){
			CString titleName;
			CString name;
			name = A2W( lpProject->getChannelName( channelIndex ).c_str() );
			titleName.Format(L"ͨ��%d-%s", channelIndex + 1, name);//origin
			//titleName.Format(L"ͨ��%d-%s", RadarManager::Instance()->originalIndexToRecordIndex(channelIndex) + 1,name);//20210222 
			//titleName.Format(L"ͨ��%d", channelIndex + 1);//20210222 temp
			//titleName.Format(L"ͨ��%d-%s", (channelIndex + 1)/2, name);//xining

			_channelName[channelIndex].SetWindowText( titleName );
		}
		int menuPosIndex;
		std::vector<int>::iterator vectorIndex = find(m_channelVec.begin(),m_channelVec.end(), channelIndex);//�ҵ�channelIndex������ͨ������m_channelVec���index
		if (vectorIndex != m_channelVec.end()){
			menuPosIndex = distance(m_channelVec.begin(), vectorIndex);
			m_pMenu.GetSubMenu(0)->CheckMenuItem( menuPosIndex, MF_BYPOSITION | MF_CHECKED );
		}
	}else{
		_curveWnd[channelIndex].EndDraw();
		_curveWnd[channelIndex].ShowWindow( SW_HIDE );
		_channelName[channelIndex].ShowWindow( SW_HIDE );

		int menuPosIndex;
		std::vector<int>::iterator vectorIndex = find(m_channelVec.begin(),m_channelVec.end(), channelIndex);
		if (vectorIndex != m_channelVec.end()){
			menuPosIndex = distance(m_channelVec.begin(), vectorIndex);
			m_pMenu.GetSubMenu(0)->CheckMenuItem( menuPosIndex, MF_BYPOSITION | MF_UNCHECKED );
		}
	}

	resizeChannelWnd();
}*/

void CDialogTERDisplay::OnContextMenu(CWnd* /*pWnd*/, CPoint point)
{
	// TODO: �ڴ˴������Ϣ����������

//  	for ( int i = 0; i < m_channelVec.size(); i ++ )
//  	{
// 		std::stringstream ss;
// 		ss<<m_channelVec[i];
// 
// 		int menuIndex;
// 		ss>>menuIndex;
//  		updateMenuState( menuIndex, m_pMenu.GetSubMenu( 0 ) );
//  	}

	BOOL aaaa = m_pMenu.GetSubMenu(0)->TrackPopupMenu(TPM_LEFTALIGN|TPM_LEFTBUTTON|TPM_RIGHTBUTTON, point.x, point.y, AfxGetMainWnd());
}

void CDialogTERDisplay::updateMenuState( int index, CMenu *lpMenu )
{
	/*
	if ( index >= 8 ){//��ʱhjl20210203
		return ;
	}*/
	
	if ( index >= CHANNELCOUNT ){
		return ;
	}
	ConfigureSet *cfg = RadarManager::Instance()->getConfigureSet();
	lpMenu = m_pMenu.GetSubMenu(0);

	if ( !cfg )
	{
		return;
	}
	std::stringstream ss;
	ss << "channel" << index;
	std::string channel = ss.str();

	if ( cfg->get(_cfgGroupName, channel) == "true" )
	{
		lpMenu->CheckMenuItem( index, MF_BYPOSITION | MF_CHECKED );
	}else
	{
		lpMenu->CheckMenuItem( index, MF_BYPOSITION | MF_UNCHECKED );
	}
}



/*void CDialogTERDisplay::changeChannel( int index )//���ݴ���ʵ������ͨ�����ݺ���
{
	if ( index >= CHANNELCOUNT ){
		return ;
	}

	ConfigureSet *cfg = RadarManager::Instance()->getConfigureSet();
	if ( !cfg ){
		return;
	}
	//intתstring �Ѵ�������index��ͨ����ʶX����������channel��ֵΪ"channalX"�����ڸ�cfg����
	std::stringstream ss;
	ss << index;
	std::string channel = ss.str();
	//�Ѿ����˾͸ĳ��� ��֮��Ȼ
	if ( cfg->get(_cfgGroupName, channel) == "true" ){
		cfg->set(_cfgGroupName, channel,"false" );
		changeChannel( index, false );
	}else{
		cfg->set(_cfgGroupName, channel,"true" );
		changeChannel( index, true );
	}

	cfg->write();
}*/

void CDialogTERDisplay::resizeChannelWnd(){

	CRect fullRect;
	GetClientRect( fullRect );

	const int space = 10;
	const int namespaceHeight = 20;

	int dislpalyWidth = fullRect.Width()-space*2;

	m_TERDisplayWnd.MoveWindow( space, space, dislpalyWidth, fullRect.Height()-space*4 );
	
	/*_recommendableSpeed.MoveWindow(fullRect.Width()-225,fullRect.Height() - namespaceHeight,125,namespaceHeight);//hjl �Ƽ��ٶ���ʾ���ڶ�λ
	_gpsCondition.MoveWindow(250,fullRect.Height() - namespaceHeight,100,namespaceHeight);*/

	_lonlat.MoveWindow(space,fullRect.Height() - namespaceHeight,250,namespaceHeight);//��γ����ʾ���ڶ�λ
	_speed.MoveWindow(fullRect.Width()-100,fullRect.Height() - namespaceHeight,100,namespaceHeight);//hjl �ٶ���ʾ���ڶ�λ
	_dataMissing.MoveWindow(fullRect.Width()-350,fullRect.Height() - namespaceHeight,150,namespaceHeight);//hjl ���������ʾ���ڶ�λ
	_distance.MoveWindow(space+300,fullRect.Height() - namespaceHeight,250,namespaceHeight);//hjl20210623 ���ڶ�ʱ���¾���
}

void CDialogTERDisplay::OnShowWindow(BOOL bShow, UINT nStatus){
	CDialog::OnShowWindow(bShow, nStatus);

	if ( bShow ){
		SetTimer( 1, 200, NULL );//����Ƶ�� *ǧ����һ��
	}else{
		KillTimer( 1 );
	}
}

void CDialogTERDisplay::OnTimer(UINT_PTR nIDEvent)
{
	/*if ( _dcValue > 0 )
	{
		CString title;
		float k;
		float d;
		float p = 12;
		switch (RadarManager::Instance()->GetRadarWorkType())
		{
		case RADAR_WORK_TYPE_ONE_USB:
			p = (float)(_dcValue) * (5.00000000/4096) * 4;
			title.Format(L"ʵʱ��ѹ��%f", p);
			break;
		case RADAR_WORK_TYPE_DOUBLE_USB:
			p = (float)(_dcValue) * (5.00000000/4096) * 4;
			title.Format(L"ʵʱ��ѹ��%f", p);
			break;
		case RADAR_WORK_TYPE_DOUBLE_USB_OLD:
			p = (float)(_dcValue) * (5.00000000/4096) * 4;
			title.Format(L"ʵʱ��ѹ��%f", p);
			break;
		case RADAR_WORK_TYPE_FOUR_USB:
			p = (float)(_dcValue) * (5.00000000/4096) * 4;
			title.Format(L"ʵʱ��ѹ��%f", p);
			break;
		case RADAR_WORK_TYPE_EIGHT:
			k = atof(RadarManager::Instance()->getConfigureSet()->get("radar", "K" ).c_str());
			d = 3.3 * 6 / 4095 * k;
			p = ( d - 12 ) / ( 4.8 ) * 100;
			title.Format(L"ʵʱ����--��ѹ��%f", p);
			title += L"%";
			break;
		default:
			break;
		}
		SetWindowText( title );
	}



	switch ( RadarManager::Instance()->getGpsReader()->getCondition()){
		case 0:
			_CstrGpsCondition.Format(L"��ʼ��");
			break;
		case 1:
			_CstrGpsCondition.Format(L"���㶨λ");
			break;
		case 2:
			_CstrGpsCondition.Format(L"��ֶ�λ");
			break;
		case 3:
			_CstrGpsCondition.Format(L"��ЧPPS");
			break;
		case 4:
			_CstrGpsCondition.Format(L"�̶���");
			break;
		case 5:
			_CstrGpsCondition.Format(L"������");
			break;
		case 6:
			_CstrGpsCondition.Format(L"���ڹ���");
			break;
		default:
			_CstrGpsCondition.Format(L"��ʼ��");
			break;
	}
	_gpsCondition.SetWindowText( _CstrGpsCondition );
	
	*/
	_lonlat.SetWindowText( m_cstrLonlat );//hjl6.5 ��ʱ���¾�γ��
	
	_CstrDataMissing.Format(L"����/�ܵ���%d/%d",m_nRecordTotalWheelCount-m_nRealTotalWheelCount, m_nRecordTotalWheelCount);	
	_dataMissing.SetWindowText( _CstrDataMissing );//hjl20210302 ��ʱ���¶������

	_CstrDistance.Format(L"�����־���/gps���룺%.2f/%.2f",_wheelDistance,_gpsDistance);	
	_distance.SetWindowText( _CstrDistance );//hjl20210623 ���ڶ�ʱ���¾���

	//�ٶȼ��㲿�� hjl20200907
	time_t curT = time(NULL);
	if(curT - _tLastTime > 1){//2�����һ��
		_fSpeed=(_fCurLen-_fLastLen)/(curT - _tLastTime)*3.6;
		_fLastLen=_fCurLen;
		_tLastTime=curT;
		_CstrSpeed.Format(L"�ٶȣ�%0.2fmph",_fSpeed);	
	}
	_speed.SetWindowText( _CstrSpeed );//��ʾ�ٶ� hjl20200907 �ٶ���ʾģ��

	/*_CstrRecommendableSpeed.Format(L"�Ƽ��ٶȣ�%0.2fkmph",_fRecommendableSpeed/3.5);
	_recommendableSpeed.SetWindowText( _CstrRecommendableSpeed );//��ʾ�ٶ� hjl20210408 �Ƽ��ٶ���ʾģ��*/
}

void CDialogTERDisplay::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	KillTimer( 1 );
	//CDialog::OnClose();
	ShowWindow(FALSE);
}

void CDialogTERDisplay::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	resizeChannelWnd();
}


std::vector<int> CDialogTERDisplay::RemoveChannels()
{
	std::vector<int> AllChannel;
	for (int i=0;i<CHANNELCOUNT;i++){
		AllChannel.push_back(i);
	}

	//Ҫȥ����Ƶ��
	std::vector<int> pChannelRemoved(CHANNELCOUNT);
	std::vector<int>::iterator itor;

	std::sort(m_channelVec.begin(),m_channelVec.end());
	std::sort(AllChannel.begin(),AllChannel.end());
	itor = set_difference(AllChannel.begin(), AllChannel.end(),m_channelVec.begin(),m_channelVec.end(), pChannelRemoved.begin());//set_difference���������ϵĲ����������а����������ڵ�һ�����ϵ������ڵڶ������ϵ�Ԫ��
	pChannelRemoved.resize(itor-pChannelRemoved.begin());//����ȷ��pChannelRemoved��С
	//ɾ��pChannelRemovedΪ�յ�Ԫ��
	/*
	std::vector<int>::iterator tempItor = pChannelRemoved.begin();
	for(tempItor; tempItor != pChannelRemoved.end();){
		if (*tempItor == ""){
			tempItor = pChannelRemoved.erase(tempItor);//Ϊ��ʹָ��ָ����һ������ ֱ��vector.erase(temp)��ʹtemp++��ΪҰָ�롣
		}else{
			++tempItor;
		}
	}
	*/
			
	return pChannelRemoved;
}


