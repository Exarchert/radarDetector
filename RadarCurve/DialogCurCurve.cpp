// DialogCurCurve.cpp : 实现文件
//
#include "stdafx.h"
#include "Resource.h"
#include "RadarCurve.h"
#include "DialogCurCurve.h"
#include <sstream>
#include <RadarCurve/RadarManager.h>
#include "COM/Tools/ProcessData.h"
#include <algorithm>

// CDialogCurCurve 对话框

IMPLEMENT_DYNAMIC(CDialogCurCurve, CDialog)

CDialogCurCurve::CDialogCurCurve(separateScreen pScreenType, CWnd* pParent /*=NULL*/)
	: CDialog(CDialogCurCurve::IDD, pParent)
{
	_cfgGroupName = "curchannel";
	
	m_separateScreen = pScreenType;
	
	_dcValue = 0.0f;

	//_recordTotalWheelCount = 0;
	//_realTotalWheelCount = 0;
	for(int i=0;i<CHANNELCOUNT;i++){
		m_arrnRecordTotalWheelCount[i]=0;
		m_arrnRealTotalWheelCount[i]=0;
	}

	_wheelDistance = 0.0f;
	_gpsDistance = 0.0f;

	//计算频道的个数
	ConfigureSet *cfg = RadarManager::Instance()->getConfigureSet();
	if (cfg)
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
		//将输入的1~16变为使用标准的0~15并存进m_channelVec
		if(!(stringVec[0].compare("")==0)){//spilt""的时候产生一个有一个""的数组
			for (int i=0;i<stringVec.size();i++){
				m_channelVec.push_back(atoi(stringVec[i].c_str())-1);
			}
		}
		m_channelCount = m_channelVec.size();
	}
	//hjl20210408以下为计算推荐速度
	m_nTrueChannelCount=RadarManager::Instance()->GetTrueChannelCount();
	float precLen = atoi ( cfg->get("radar", "preclen").c_str() );
	float fJihuaAccuracy = (float)atoi( cfg->get("radar", "precision").c_str() );
	float fwheelPrecise = RadarManager::Instance()->GetTrueAccuracyFromPreclenAndPrecindex(precLen,fJihuaAccuracy);//实际道间距
	int iSampleCountIndex = atoi ( cfg->get("radar", "sample").c_str());
	int iSampleCount = RadarManager::Instance()->getSampCount( iSampleCountIndex );//采样点
	//float temp=((float)iSampleCount*20/3);
	//float temp2=1/temp;
	//_fRecommendableSpeed=fwheelPricise*10000*temp2*3.6;
	_fRecommendableSpeed=(float)29400/m_nTrueChannelCount*fwheelPrecise/(float)iSampleCount;
	if(_fRecommendableSpeed>20){
		_fRecommendableSpeed=20+0.4*(_fRecommendableSpeed-20);
	}
	m_nInformationIndex = atoi ( cfg->get("screen", "index").c_str() );
	m_nSaveFileType=atoi(cfg->get("radar", "saveFileType").c_str());
}

CDialogCurCurve::~CDialogCurCurve()
{
	for(int i=0;i<CHANNELCOUNT;i++){
		m_arrnRecordTotalWheelCount[i]=0;
		m_arrnRealTotalWheelCount[i]=0;
	}
}

void CDialogCurCurve::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control( pDX, IDC_CURVE_00, _curveWnd[0]);
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
	DDX_Control( pDX, IDC_CURVE_16, _curveWnd[16]);
	DDX_Control( pDX, IDC_CURVE_17, _curveWnd[17]);
	DDX_Control( pDX, IDC_CURVE_18, _curveWnd[18]);
	DDX_Control( pDX, IDC_CURVE_19, _curveWnd[19]);
	DDX_Control( pDX, IDC_CURVE_20, _curveWnd[20]);
	DDX_Control( pDX, IDC_CURVE_21, _curveWnd[21]);
	DDX_Control( pDX, IDC_CURVE_22, _curveWnd[22]);
	DDX_Control( pDX, IDC_CURVE_23, _curveWnd[23]);
	DDX_Control( pDX, IDC_CURVE_24, _curveWnd[24]);
	DDX_Control( pDX, IDC_CURVE_25, _curveWnd[25]);
	DDX_Control( pDX, IDC_CURVE_26, _curveWnd[26]);
	DDX_Control( pDX, IDC_CURVE_27, _curveWnd[27]);
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
	DDX_Control( pDX, IDC_STATIC_CHANNEL_16, _channelName[16]);
	DDX_Control( pDX, IDC_STATIC_CHANNEL_17, _channelName[17]);
	DDX_Control( pDX, IDC_STATIC_CHANNEL_18, _channelName[18]);
	DDX_Control( pDX, IDC_STATIC_CHANNEL_19, _channelName[19]);
	DDX_Control( pDX, IDC_STATIC_CHANNEL_20, _channelName[20]);
	DDX_Control( pDX, IDC_STATIC_CHANNEL_21, _channelName[21]);
	DDX_Control( pDX, IDC_STATIC_CHANNEL_22, _channelName[22]);
	DDX_Control( pDX, IDC_STATIC_CHANNEL_23, _channelName[23]);
	DDX_Control( pDX, IDC_STATIC_CHANNEL_24, _channelName[24]);
	DDX_Control( pDX, IDC_STATIC_CHANNEL_25, _channelName[25]);
	DDX_Control( pDX, IDC_STATIC_CHANNEL_26, _channelName[26]);
	DDX_Control( pDX, IDC_STATIC_CHANNEL_27, _channelName[27]);

	//DDX_Text( pDX, IDC_EDIT_LONLAT, _lonlat);
	DDX_Control( pDX, IDC_LONLAT, _lonlat);//hjl6.5 用于定时更新经纬度
	DDX_Control( pDX, IDC_GPSCONDITION, _gpsCondition);//hjl6.5 用于定时更新经纬度
	DDX_Control( pDX, IDC_SPEED, _speed);//hjl20200907 用于定时更新速度
	DDX_Control( pDX, IDC_RECOMMENDABLE_SPEED, _recommendableSpeed);//hjl20210408 用于表示推荐速度
	DDX_Control( pDX, IDC_DATAMISSING, _dataMissing);//hjl20200907 用于定时更新丢道情况
	DDX_Control( pDX, IDC_DISTANCE, _distance);//hjl20210623 用于定时更新距离

}


BEGIN_MESSAGE_MAP(CDialogCurCurve, CDialog)
	ON_MESSAGE( WM_NEW_RADAR_DATA, &CDialogCurCurve::OnRadarData )
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
	ON_COMMAND(ID_SHOW_CHANNEL_16, &CDialogCurCurve::OnShowChannel16)
	ON_COMMAND(ID_SHOW_CHANNEL_17, &CDialogCurCurve::OnShowChannel17)
	ON_COMMAND(ID_SHOW_CHANNEL_18, &CDialogCurCurve::OnShowChannel18)
	ON_COMMAND(ID_SHOW_CHANNEL_19, &CDialogCurCurve::OnShowChannel19)
	ON_COMMAND(ID_SHOW_CHANNEL_20, &CDialogCurCurve::OnShowChannel20)
	ON_COMMAND(ID_SHOW_CHANNEL_21, &CDialogCurCurve::OnShowChannel21)
	ON_COMMAND(ID_SHOW_CHANNEL_22, &CDialogCurCurve::OnShowChannel22)
	ON_COMMAND(ID_SHOW_CHANNEL_23, &CDialogCurCurve::OnShowChannel23)
	ON_COMMAND(ID_SHOW_CHANNEL_24, &CDialogCurCurve::OnShowChannel24)
	ON_COMMAND(ID_SHOW_CHANNEL_25, &CDialogCurCurve::OnShowChannel25)
	ON_COMMAND(ID_SHOW_CHANNEL_26, &CDialogCurCurve::OnShowChannel26)
	ON_COMMAND(ID_SHOW_CHANNEL_27, &CDialogCurCurve::OnShowChannel27)

	ON_BN_CLICKED(IDOK, &CDialogCurCurve::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CDialogCurCurve::OnBnClickedCancel)
	ON_WM_CONTEXTMENU()
	ON_WM_SHOWWINDOW()
	ON_WM_TIMER()
	ON_WM_CLOSE()
	ON_WM_SIZE()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()


// CDialogCurCurve 消息处理程序

BOOL CDialogCurCurve::OnInitDialog()
{
	CDialog::OnInitDialog();
	m_pMenu.LoadMenu(IDR_MENU_SHOW_CHANNEL);

	std::vector<int> pChannelRemoved = RemoveChannels();

	for (int i = pChannelRemoved.size()-1; i > -1; i--){//删了之后总index也变了 所以从最小删到最大会出错 需从最大删到最小
		//int转string
		std::stringstream ss;
		ss<<pChannelRemoved[i];
		int menuIndex;
		ss>>menuIndex;
		//去除菜单
		BOOL removeResult = m_pMenu.GetSubMenu(0)->RemoveMenu(menuIndex,MF_BYPOSITION);
		if(!removeResult){::AfxMessageBox(L"通道控制失效！" );}
	}

	// TODO:  在此添加额外的初始化
	int nScreenWidth, nScreenHeight;  
	nScreenWidth = GetSystemMetrics(SM_CXSCREEN);  
	nScreenHeight = GetSystemMetrics(SM_CYSCREEN);
	
	/*
	if ( nScreenWidth > 1500 ){
		nScreenWidth = 1500;
	}*/

	//nScreenHeight /= 4;
	nScreenHeight = (nScreenHeight-100)/4;
	if ( nScreenHeight > 500 ){
		nScreenHeight = 500;
	}

	ConfigureSet *cfg = RadarManager::Instance()->getConfigureSet();
	if ( !cfg ){
		return TRUE;
	}

	//MoveWindow( 0, nScreenHeight*m_separateScreen + 50, nScreenWidth, nScreenHeight );
	if (m_separateScreen==0){
		if(m_nSaveFileType==0){//存sgy
			MoveWindow( 0, nScreenHeight*m_separateScreen + 50, nScreenWidth, nScreenHeight*2 );
		}else if(m_nSaveFileType==1){//存rd3
			MoveWindow( -5, 50, GetSystemMetrics(SM_CXSCREEN)/2+10, nScreenHeight*2 );
			if (RadarManager::Instance()->GetTrueChannelCount()==15){
				MoveWindow( 0, nScreenHeight*m_separateScreen + 50, nScreenWidth, nScreenHeight*2 );
			}
		}
	}
	if (m_separateScreen==1){
		//MoveWindow( 0, nScreenHeight*m_separateScreen + 50, nScreenWidth, nScreenHeight );
		/*if(m_channelCount==7){
			MoveWindow( GetSystemMetrics(SM_CXSCREEN)/2, 50, GetSystemMetrics(SM_CXSCREEN)/2+10, nScreenHeight*2 );
		}
		MoveWindow( 0, nScreenHeight*m_separateScreen + 50, nScreenWidth, nScreenHeight );*/
		MoveWindow( GetSystemMetrics(SM_CXSCREEN)/2, 50, GetSystemMetrics(SM_CXSCREEN)/2+10, nScreenHeight*2 );
	}
	if (m_separateScreen==2){
		MoveWindow( 0, nScreenHeight*m_separateScreen + 50, nScreenWidth/2, nScreenHeight*2 );
	}/*
	if (m_separateScreen==3){
		MoveWindow( 0, nScreenHeight*m_separateScreen + 50, nScreenWidth/2, nScreenHeight );
	}*/

	
	//隐藏要移除的频道
	for (int i = 0; i != pChannelRemoved.size(); ++i){
		changeChannel( pChannelRemoved[i], false );
	}
	//控制对话框要显示的频道的显隐
	for (int i = 0; i < m_channelVec.size(); i++){
		std::string channel;
		std::stringstream ss;
		ss<<m_channelVec[i];
		ss>>channel;//int to string
		if ( cfg->get(_cfgGroupName, channel) == "true" ){
			changeChannel( m_channelVec[i], true );
		}else{
			changeChannel( m_channelVec[i], false );
		}
	}

	//以下为速度计算模块部分
	_tLastTime=time(NULL);
	_fLastLen=0;
	_fCurLen=0;
	for(int i=0;i<CHANNELCOUNT;i++){
	//for(int i=0;i<8;i++){
		_iDisplayFlag[i]=0;
	}
	_iDisplayExtractValue = atoi(RadarManager::Instance()->getConfigureSet()->get("displayExtract", "displayExtractValue").c_str());
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

LRESULT CDialogCurCurve::OnRadarData( WPARAM wParam, LPARAM lParam )
{
	RadarData *lpData = (RadarData*)wParam;
	int index = lParam;
	if ( !lpData )
	{
		return 0;
	}
	if ( index >= CHANNELCOUNT )
	{
		return 0;
	}
	_curveWnd[index].addData( lpData );
	return 0;
}

void CDialogCurCurve::AddRadarData( RadarData *lpData , int index )
{
	if ( index >= CHANNELCOUNT ){
		return ;
	}
	
	if(m_arrnRecordTotalWheelCount[index] == lpData->getRealWheelCount()){
		return;
	}
	m_arrnRecordTotalWheelCount[index] = lpData->getRealWheelCount();
	m_arrnRealTotalWheelCount[index] = m_arrnRealTotalWheelCount[index]+1;
	if(m_arrnRealTotalWheelCount[index]==1){//不要第一个数据，第一个数据总是上一次采集的，原因不明
		return;
	}

	if(_iDisplayFlag[index]==0){
		_curveWnd[index].addData( lpData );
		//_curveWnd[index].setData( lpData );
	}
	//_curveWnd[index].addData( lpData );
	if ( index == m_nInformationIndex ){
		_dcValue = lpData->getDCValue();//电压

		//hjl6.5 以下两行用于定时更新经纬度显示
		_position = lpData->getPosition();
		_CstrLonlat.Format(L"经纬度：（%0.10f,%0.10f）",_position.x(),_position.y());	

		//hjl20210302以下为计算丢道情况;
		//_recordTotalWheelCount = lpData->getPrec();
		//_recordTotalWheelCount = lpData->getRealWheelCount();
		//_realTotalWheelCount = _realTotalWheelCount+1;

		//hjl20210623 用于定时更新距离
		_wheelDistance = lpData->getPrecLen();
		_gpsDistance = lpData->getLen();

		//hjl20200907以下为计算速度
		_fCurLen=lpData->getPrecLen();
	}
	
	_iDisplayFlag[index] += 1;
	if(_iDisplayFlag[index]==_iDisplayExtractValue){//数越大抽的道越多
		_iDisplayFlag[index]=0;
	}
}

/*
void CDialogCurCurve::DeleteRadarData( int index )
{
	if ( index >= CHANNELCOUNT ){
		return ;
	}
	_curveWnd[index].deleteData();
}*/

void CDialogCurCurve::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
// 	for ( int i = 0; i < MAX_CHANNEL; i ++ )
// 	{
// 		changeChannel( i, false );
// 	}
	//OnOK();
}

void CDialogCurCurve::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
// 	for ( int i = 0; i < MAX_CHANNEL; i ++ )
// 	{
// 		changeChannel( i, false );
// 	}

	OnCancel();
}

void CDialogCurCurve::changeChannel( int channelIndex, bool flag )
{
	if ( channelIndex >= CHANNELCOUNT ){
		return ;
	}
	if ( flag ){//显示该通道并赋值并开始绘图
		ConfigureSet *lpSet = RadarManager::Instance()->getConfigureSet();
		int iSmpNum = atoi ( lpSet->get("radar", "sample").c_str());
		int iSampRate = atoi ( lpSet->get("radar", "sampleratio" ).c_str() );
		_curveWnd[channelIndex].SetSampleCount( RadarManager::Instance()->getSampCount( iSmpNum ) );
		_curveWnd[channelIndex].SetSampleRatio( RadarManager::Instance()->getSampRatio( iSampRate, channelIndex ) );

		std::stringstream ss;
		//ss << channelIndex+12;//
		ss << channelIndex;//20211226
		_curveWnd[channelIndex].SetCorrection( atoi( lpSet->get("correction", ss.str()).c_str() ) );
		bool iType=(bool)atoi( lpSet->get("radar", "saveFileType").c_str());
		_curveWnd[channelIndex].SetSpectrumDisplay(!iType);
		/*if(iType==1){
			_curveWnd[channelIndex].SetSpectrumDisplay(true);
		}else{
			_curveWnd[channelIndex].SetSpectrumDisplay(false);
		}*/
		_curveWnd[channelIndex].StartDraw();
		_curveWnd[channelIndex].ShowWindow( SW_SHOW );
		_channelName[channelIndex].ShowWindow( SW_SHOW );

		USES_CONVERSION;

		RadarManager *lpProject = RadarManager::Instance();
		if ( lpProject ){
			CString titleName;
			CString name;
			name = A2W( lpProject->getChannelName( channelIndex ).c_str() );
			titleName.Format(L"通道%d-%s", channelIndex + 1, name);//origin
			//titleName.Format(L"通道%d-%s", RadarManager::Instance()->originalIndexToRecordIndex(channelIndex) + 1,name);//20210222 
			//titleName.Format(L"通道%d", channelIndex + 1);//20210222 temp
			//titleName.Format(L"通道%d-%s", (channelIndex + 1)/2, name);//xining

			_channelName[channelIndex].SetWindowText( titleName );
		}
		int menuPosIndex;
		std::vector<int>::iterator vectorIndex = find(m_channelVec.begin(),m_channelVec.end(), channelIndex);//找到channelIndex在现有通道数组m_channelVec里的index
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
}

void CDialogCurCurve::OnContextMenu(CWnd* /*pWnd*/, CPoint point)
{
	// TODO: 在此处添加消息处理程序代码

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

void CDialogCurCurve::updateMenuState( int index, CMenu *lpMenu )
{
	/*
	if ( index >= 8 ){//暂时hjl20210203
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

void CDialogCurCurve::OnShowChannel00()
{
	changeChannel( 0 );
}
void CDialogCurCurve::OnShowChannel01()
{
	changeChannel( 1 );
}
void CDialogCurCurve::OnShowChannel02()
{
	changeChannel( 2 );
}
void CDialogCurCurve::OnShowChannel03()
{
	changeChannel( 3 );
}
void CDialogCurCurve::OnShowChannel04()
{
	changeChannel( 4 );
}
void CDialogCurCurve::OnShowChannel05()
{
	changeChannel( 5 );
}
void CDialogCurCurve::OnShowChannel06()
{
	changeChannel( 6 );
}
void CDialogCurCurve::OnShowChannel07()
{
	changeChannel( 7 );
}
void CDialogCurCurve::OnShowChannel08()
{
	changeChannel( 8 );
}
void CDialogCurCurve::OnShowChannel09()
{
	changeChannel( 9 );
}
void CDialogCurCurve::OnShowChannel10()
{
	changeChannel( 10 );
}
void CDialogCurCurve::OnShowChannel11()
{
	changeChannel( 11 );
}
void CDialogCurCurve::OnShowChannel12()
{
	changeChannel( 12 );
}
void CDialogCurCurve::OnShowChannel13()
{
	changeChannel( 13 );
}
void CDialogCurCurve::OnShowChannel14()
{
	changeChannel( 14 );
}
void CDialogCurCurve::OnShowChannel15()
{
	changeChannel( 15 );
}
void CDialogCurCurve::OnShowChannel16()
{
	changeChannel( 16 );
}
void CDialogCurCurve::OnShowChannel17()
{
	changeChannel( 17 );
}
void CDialogCurCurve::OnShowChannel18()
{
	changeChannel( 18 );
}
void CDialogCurCurve::OnShowChannel19()
{
	changeChannel( 19 );
}
void CDialogCurCurve::OnShowChannel20()
{
	changeChannel( 20 );
}
void CDialogCurCurve::OnShowChannel21()
{
	changeChannel( 21 );
}
void CDialogCurCurve::OnShowChannel22()
{
	changeChannel( 22 );
}
void CDialogCurCurve::OnShowChannel23()
{
	changeChannel( 23 );
}
void CDialogCurCurve::OnShowChannel24()
{
	changeChannel( 24 );
}
void CDialogCurCurve::OnShowChannel25()
{
	changeChannel( 25 );
}
void CDialogCurCurve::OnShowChannel26()
{
	changeChannel( 26 );
}
void CDialogCurCurve::OnShowChannel27()
{
	changeChannel( 27 );
}

void CDialogCurCurve::changeChannel( int index )//数据窗口实现显隐通道数据函数
{
	if ( index >= CHANNELCOUNT ){
		return ;
	}

	ConfigureSet *cfg = RadarManager::Instance()->getConfigureSet();
	if ( !cfg ){
		return;
	}
	//int转string 把传进来的index（通道标识X）赋给变量channel（值为"channalX"）用于给cfg联动
	std::stringstream ss;
	ss << index;
	std::string channel = ss.str();
	//已经显了就改成隐 反之亦然
	if ( cfg->get(_cfgGroupName, channel) == "true" ){
		cfg->set(_cfgGroupName, channel,"false" );
		changeChannel( index, false );
	}else{
		cfg->set(_cfgGroupName, channel,"true" );
		changeChannel( index, true );
	}

	cfg->write();
}

void CDialogCurCurve::resizeChannelWnd()
{
	int showCount = 0;
	for( int i = 0; i < CHANNELCOUNT; i ++ )
	//for( int i = 0; i < 8; i ++ )
	{
		if ( !_curveWnd[i].GetSafeHwnd() )
		{
			return;
		}
		showCount += _curveWnd[i].IsWindowVisible() ? 1 : 0;
	}

	if ( showCount == 0 )
	{
		return;
	}

	CRect fullRect;
	GetClientRect( fullRect );

	const int space = 10;
	const int namespaceHeight = 20;

	int curWidth = ( fullRect.Width() - space * ( showCount - 1 ) ) / showCount;

	CRect staticRect;
	_channelName[0].GetClientRect( staticRect );

	int startX = 0;
	for( int i = 0; i < CHANNELCOUNT; i ++ )
	{
		if ( !_curveWnd[i].IsWindowVisible() ){
			continue;
		}
		_curveWnd[i].MoveWindow( startX, namespaceHeight, curWidth, fullRect.Height() - namespaceHeight - 20 );
		//_channelName[i].MoveWindow( startX, 3, staticRect.Width(), namespaceHeight );
		_channelName[i].MoveWindow( startX, 3, curWidth, namespaceHeight );

		startX += curWidth + space;
	}
	
	_speed.MoveWindow(fullRect.Width()-100,fullRect.Height() - namespaceHeight,100,namespaceHeight);//hjl 速度显示窗口定位
	_recommendableSpeed.MoveWindow(fullRect.Width()-225,fullRect.Height() - namespaceHeight,125,namespaceHeight);//hjl 推荐速度显示窗口定位
	_dataMissing.MoveWindow(fullRect.Width()-350,fullRect.Height() - namespaceHeight,150,namespaceHeight);//hjl 丢道情况显示窗口定位
	_distance.MoveWindow(fullRect.Width()-600,fullRect.Height() - namespaceHeight,250,namespaceHeight);//hjl20210623 用于定时更新距离
	_lonlat.MoveWindow(1,fullRect.Height() - namespaceHeight,250,namespaceHeight);//hjl6.5经纬度显示窗口定位
	_gpsCondition.MoveWindow(250,fullRect.Height() - namespaceHeight,100,namespaceHeight);
}

void CDialogCurCurve::OnShowWindow(BOOL bShow, UINT nStatus){
	CDialog::OnShowWindow(bShow, nStatus);

	if ( bShow ){
		SetTimer( 1, 200, NULL );//更新频率 *千毫秒一更
	}else{
		KillTimer( 1 );
	}
}

void CDialogCurCurve::OnTimer(UINT_PTR nIDEvent)
{
	if ( _dcValue > 0 )
	{
		CString title;
		float k;
		float d;
		float p = 12;
		switch (RadarManager::Instance()->GetRadarWorkType())
		{
		case RADAR_WORK_TYPE_ONE_USB:
			p = (float)(_dcValue) * (5.00000000/4096) * 4;
			title.Format(L"实时电压：%f", p);
			break;
		case RADAR_WORK_TYPE_DOUBLE_USB:
			p = (float)(_dcValue) * (5.00000000/4096) * 4;
			title.Format(L"实时电压：%f", p);
			break;
		case RADAR_WORK_TYPE_DOUBLE_USB_OLD:
			p = (float)(_dcValue) * (5.00000000/4096) * 4;
			title.Format(L"实时电压：%f", p);
			break;
		case RADAR_WORK_TYPE_FOUR_USB:
			p = (float)(_dcValue) * (5.00000000/4096) * 4;
			title.Format(L"实时电压：%f", p);
			break;
		case RADAR_WORK_TYPE_EIGHT:
			k = atof(RadarManager::Instance()->getConfigureSet()->get("radar", "K" ).c_str());
			d = 3.3 * 6 / 4095 * k;
			p = ( d - 12 ) / ( 4.8 ) * 100;
			title.Format(L"实时曲线--电压：%f", p);
			title += L"%";
			break;
		default:
			break;
		}
		SetWindowText( title );
	}


	_CstrDataMissing.Format(L"丢道/总道：%d/%d",m_arrnRecordTotalWheelCount[m_nInformationIndex]-m_arrnRealTotalWheelCount[m_nInformationIndex],m_arrnRecordTotalWheelCount[m_nInformationIndex]);	
	_dataMissing.SetWindowText( _CstrDataMissing );//hjl20210302 定时更新丢道情况

	_CstrDistance.Format(L"测量轮距离/gps距离：%.2f/%.2f",_wheelDistance,_gpsDistance);	
	_distance.SetWindowText( _CstrDistance );//hjl20210623 用于定时更新距离

	//速度计算部分 hjl20200907
	time_t curT = time(NULL);
	if(curT - _tLastTime > 1){//2秒更新一次
		_fSpeed=(_fCurLen-_fLastLen)/(curT - _tLastTime)*3.6;
		_fLastLen=_fCurLen;
		_tLastTime=curT;
		_CstrSpeed.Format(L"速度：%0.2fmph",_fSpeed);	
	}
	_speed.SetWindowText( _CstrSpeed );//显示速度 hjl20200907 速度显示模块

	_CstrRecommendableSpeed.Format(L"推荐速度：%0.2fkmph",_fRecommendableSpeed);
	_recommendableSpeed.SetWindowText( _CstrRecommendableSpeed );//显示速度 hjl20210408 推荐速度显示模块

	switch ( RadarManager::Instance()->getGpsReader()->getCondition()){
		case 0:
			_CstrGpsCondition.Format(L"初始化");
			break;
		case 1:
			_CstrGpsCondition.Format(L"单点定位");
			break;
		case 2:
			_CstrGpsCondition.Format(L"差分定位");
			break;
		case 3:
			_CstrGpsCondition.Format(L"无效PPS");
			break;
		case 4:
			_CstrGpsCondition.Format(L"固定解");
			break;
		case 5:
			_CstrGpsCondition.Format(L"浮动解");
			break;
		case 6:
			_CstrGpsCondition.Format(L"正在估算");
			break;
		default:
			_CstrGpsCondition.Format(L"初始化");
			break;
	}
	_gpsCondition.SetWindowText( _CstrGpsCondition );
	_lonlat.SetWindowText( _CstrLonlat );//hjl6.5 定时更新经纬度

}

void CDialogCurCurve::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	KillTimer( 1 );
	//CDialog::OnClose();
	ShowWindow(FALSE);
}

void CDialogCurCurve::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	resizeChannelWnd();
}


std::vector<int> CDialogCurCurve::RemoveChannels()
{
	std::vector<int> AllChannel;
	for (int i=0;i<CHANNELCOUNT;i++){
		AllChannel.push_back(i);
	}

	//要去除的频道
	std::vector<int> pChannelRemoved(CHANNELCOUNT);
	std::vector<int>::iterator itor;

	std::sort(m_channelVec.begin(),m_channelVec.end());
	std::sort(AllChannel.begin(),AllChannel.end());
	itor = set_difference(AllChannel.begin(), AllChannel.end(),m_channelVec.begin(),m_channelVec.end(), pChannelRemoved.begin());//set_difference求两个集合的差集，结果集合中包含所有属于第一个集合但不属于第二个集合的元素
	pChannelRemoved.resize(itor-pChannelRemoved.begin());//重新确定pChannelRemoved大小
	//删除pChannelRemoved为空的元素
	/*
	std::vector<int>::iterator tempItor = pChannelRemoved.begin();
	for(tempItor; tempItor != pChannelRemoved.end();){
		if (*tempItor == ""){
			tempItor = pChannelRemoved.erase(tempItor);//为了使指针指向下一个数据 直接vector.erase(temp)会使temp++成为野指针。
		}else{
			++tempItor;
		}
	}
	*/
			
	return pChannelRemoved;
}


