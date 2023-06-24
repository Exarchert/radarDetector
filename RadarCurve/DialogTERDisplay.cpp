// DialogCurCurve.cpp : 实现文件
//
#include "stdafx.h"
#include "Resource.h"
#include "RadarCurve.h"
#include "DialogTERDisplay.h"
#include <sstream>
#include <RadarCurve/RadarManager.h>
#include "COM/Tools/ProcessData.h"
#include <algorithm>

// CDialogTERDisplay 对话框

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

	//计算频道的个数
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
		//将输入的1~16变为使用标准的0~15并存进m_channelVec
		if(!(stringVec[0].compare("")==0)){//spilt""的时候产生一个有一个""的数组
			for (int i=0;i<stringVec.size();i++){
				m_channelVec.push_back(atoi(stringVec[i].c_str())-1);
			}
		}
		m_channelCount = m_channelVec.size();
	}*/
	/*
	//hjl20210408以下为计算推荐速度
	float precLen = atoi ( cfg->get("radar", "preclen").c_str() );
	//int precIndex = atoi( cfg->get("radar", "precindex").c_str() );
	//float fJihuaAccuracy = RadarManager::Instance()->GetJihuaAccuracy(precIndex);
	float fJihuaAccuracy = (float)atoi( cfg->get("radar", "precision").c_str() );
	float fwheelPricise = RadarManager::Instance()->GetTrueAccuracyFromPreclenAndPrecindex(precLen,fJihuaAccuracy);//实际道间距
	int iSampleCountIndex = atoi ( cfg->get("radar", "sample").c_str());
	int iSampleCount = RadarManager::Instance()->getSampCount( iSampleCountIndex );//采样点
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
	DDX_Control( pDX, IDC_GPSCONDITION, _gpsCondition);//hjl6.5 用于定时更新经纬度
	DDX_Control( pDX, IDC_SPEED, _speed);//hjl20200907 用于定时更新速度
	DDX_Control( pDX, IDC_RECOMMENDABLE_SPEED, _recommendableSpeed);//hjl20210408 用于表示推荐速度
	DDX_Control( pDX, IDC_DATAMISSING, _dataMissing);//hjl20200907 用于定时更新丢道情况
	DDX_Control( pDX, IDC_DISTANCE, _distance);//hjl20210623 用于定时更新距离*/

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


// CDialogTERDisplay 消息处理程序

BOOL CDialogTERDisplay::OnInitDialog()
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
	//以下为速度计算模块部分
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
	// 异常: OCX 属性页应返回 FALSE
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
	//以下为计算丢道情况;
	
	if(lpData->getRecordWheelCount()!=0){
		m_nRecordTotalWheelCount = lpData->getRecordWheelCount();
		m_nRealTotalWheelCount = m_nRealTotalWheelCount+1;
	}else{
		m_nRecordTotalWheelCount=m_nRecordTotalWheelCount+1;
		lpData->setRecordWheelCount(m_nRecordTotalWheelCount);
	}

	m_TERDisplayWnd.addTERData( lpData );

	//用于定时更新经纬度显示
	_position = lpData->getPosition();
	m_cstrLonlat.Format(L"经纬度：（%0.10f,%0.10f）",_position.x(),_position.y());	

	//用于定时更新距离
	_wheelDistance = lpData->getPrecLen();
	_gpsDistance = lpData->getLen();

	//用于计算速度
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
	if ( flag ){//显示该通道并赋值并开始绘图
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
}*/

void CDialogTERDisplay::OnContextMenu(CWnd* /*pWnd*/, CPoint point)
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

void CDialogTERDisplay::updateMenuState( int index, CMenu *lpMenu )
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



/*void CDialogTERDisplay::changeChannel( int index )//数据窗口实现显隐通道数据函数
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
}*/

void CDialogTERDisplay::resizeChannelWnd(){

	CRect fullRect;
	GetClientRect( fullRect );

	const int space = 10;
	const int namespaceHeight = 20;

	int dislpalyWidth = fullRect.Width()-space*2;

	m_TERDisplayWnd.MoveWindow( space, space, dislpalyWidth, fullRect.Height()-space*4 );
	
	/*_recommendableSpeed.MoveWindow(fullRect.Width()-225,fullRect.Height() - namespaceHeight,125,namespaceHeight);//hjl 推荐速度显示窗口定位
	_gpsCondition.MoveWindow(250,fullRect.Height() - namespaceHeight,100,namespaceHeight);*/

	_lonlat.MoveWindow(space,fullRect.Height() - namespaceHeight,250,namespaceHeight);//经纬度显示窗口定位
	_speed.MoveWindow(fullRect.Width()-100,fullRect.Height() - namespaceHeight,100,namespaceHeight);//hjl 速度显示窗口定位
	_dataMissing.MoveWindow(fullRect.Width()-350,fullRect.Height() - namespaceHeight,150,namespaceHeight);//hjl 丢道情况显示窗口定位
	_distance.MoveWindow(space+300,fullRect.Height() - namespaceHeight,250,namespaceHeight);//hjl20210623 用于定时更新距离
}

void CDialogTERDisplay::OnShowWindow(BOOL bShow, UINT nStatus){
	CDialog::OnShowWindow(bShow, nStatus);

	if ( bShow ){
		SetTimer( 1, 200, NULL );//更新频率 *千毫秒一更
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
	
	*/
	_lonlat.SetWindowText( m_cstrLonlat );//hjl6.5 定时更新经纬度
	
	_CstrDataMissing.Format(L"丢道/总道：%d/%d",m_nRecordTotalWheelCount-m_nRealTotalWheelCount, m_nRecordTotalWheelCount);	
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

	/*_CstrRecommendableSpeed.Format(L"推荐速度：%0.2fkmph",_fRecommendableSpeed/3.5);
	_recommendableSpeed.SetWindowText( _CstrRecommendableSpeed );//显示速度 hjl20210408 推荐速度显示模块*/
}

void CDialogTERDisplay::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
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


