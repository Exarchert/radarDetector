#include "StdAfx.h"
#include <vector>
#include <RadarCurve/TERDataReader.h>
#include <RadarCurve/RadarData.h>
#include <RadarCurve/RadarManager.h>
#include <RadarCurve/MeasureProject.h>
#include <OpenThreads/Thread>

const long MAX_RECV = 1024;

const unsigned char  CHANEL_INDEX_BEGIN = 0X72;
const unsigned char CHANEL_INDEX_END = 0X81;//16通道

class ReceiveTERDataThread : public OpenThreads::Thread
{
public:

	ReceiveTERDataThread( TERDataReader* lpReader )
	{
		_lpReader = lpReader;
		iChackeOut = 0;
	}

	void run();

	int cancel()
	{
		int ret = OpenThreads::Thread::cancel();
		if ( isRunning() )
		{
			join();
		}
		return ret;
	}

	int iChackeOut ;
protected:
	TERDataReader *_lpReader;
};

void ReceiveTERDataThread::run(){
	_lpReader->init();
	while( !testCancel() ){
		if (0 >= _lpReader->readData()){
			continue;
		}
		if(RadarManager::Instance()->testing()){
			_lpReader->parseDataToDisplay();
		}else{
			//_lpReader->parseSixteenData();
			_lpReader->parseData();
		}
	}
}

class ProcessTERDataThread : public OpenThreads::Thread{
public:
	ProcessTERDataThread( TERDataReader* lpReader )
	{
		_lpReader = lpReader;
	}
	~ProcessTERDataThread(){}

	void run(){
		while( !testCancel() ){
			std::vector<unsigned char *> tempQueue;
			{
				OpenThreads::ScopedLock<OpenThreads::Mutex> lock(_mutex);
				tempQueue.swap( _charDataQueue );//swap操作实现交换两个容器内所有元素的功能
			}
			if ( tempQueue.size() > 0 ){
				//unsigned long startTime = GetTickCount();
				//std::vector<std::vector<osg::ref_ptr<RadarData>>>::reverse_iterator it;
				//std::vector<std::vector<osg::ref_ptr<RadarData>>> m_vecRadarGroupData
				for ( std::vector<unsigned char *>::iterator it = tempQueue.begin(); it != tempQueue.end(); it++ ){
					unsigned char *pCData = *it;
					if ( pCData ){
						if (_lpReader){
							_lpReader->ProcessData(pCData);
						}
						pCData = NULL;
					}
				}
			}
			if ( _charDataQueue.size() == 0 ){
				_block.reset();
				_block.block();
			}
			if ( !testCancel() ){
				continue;
			}
		}
	};

	int cancel(){
		OpenThreads::Thread::cancel();
		//清空队列
		std::vector<unsigned char *> tempQueue;
		{
			OpenThreads::ScopedLock<OpenThreads::Mutex> lock(_mutex);
			tempQueue.swap( _charDataQueue );//swap操作实现交换两个容器内所有元素的功能
		}
		if ( isRunning() ){
			_block.reset();
			_block.release();
			join();
		}
		return 0;
	};

	void addData( unsigned char *data ){
		OpenThreads::ScopedLock<OpenThreads::Mutex> lock(_mutex);
		_charDataQueue.push_back( data );

		_block.reset();
		_block.release();
	};

public:
	TERDataReader *_lpReader;
	OpenThreads::Mutex _mutex;
	OpenThreads::Block _block;
	std::vector<unsigned char *> _charDataQueue;
};


TERDataReader::TERDataReader(void)
{
	_recvBuff = new unsigned char[MAX_RECV];
	_lpTERReceiveThread = NULL;
	_lpTERProcessThread = NULL;

	_channelCount = MAX_CHANNEL;

	_sampleCount = 512;
	_sampleRatio = 51.2f;
	_hadInit = false;
	m_uPort = NULL;
	m_lLen = 1024;
	m_uPort = NULL;
	m_iRadarWorkType = 2;
	m_iModeType = 0;

	m_iReadNULLTime = 0;
	
	m_bIsDataOne=true;
	m_nTERDataCount=0;
	m_bIsConnected=false;
	for (int i =0; i<16;i++){
		_latestChannelRadarData[i] = new unsigned char[_sampleCount*2];
		for ( int j=10;j<_sampleCount*2;j++){
			_latestChannelRadarData[i][j]=0;
		}
	}

}

TERDataReader::~TERDataReader(void)
{

	if ( !_lpTERReceiveThread )
	{
		delete []_recvBuff;
		return;
	}
	if ( _lpTERReceiveThread->isRunning() )
	{
		_lpTERReceiveThread->cancel();
	}

	if (m_uPort)
	{
		delete m_uPort;
		m_uPort = NULL;
	}

	delete _lpTERProcessThread;
	delete _lpTERReceiveThread;

	delete []_recvBuff;
}


void TERDataReader::setParam( unsigned char addr, unsigned char value)
{
	unsigned short tmpValue = ( 0X00FFFF & ( addr << 8 ) ) | value;
	unsigned short revnValue = htons(tmpValue);

	if(!m_bIsConnected){
		AfxMessageBox(_T("请先对瞬变电磁雷达进行通信连接"));
		return;
	}
	_client.Send( (unsigned char *)&revnValue, sizeof( revnValue ) );
}

void TERDataReader::setInit(bool value)
{
	OpenThreads::ScopedLock<OpenThreads::Mutex> lock(_lock);
	_hadInit = value;
}

void TERDataReader::init()
{
	OpenThreads::ScopedLock<OpenThreads::Mutex> lock(_lock);
	if ( _hadInit )
	{
		return;
	}

	ConfigureSet *cs = RadarManager::Instance()->getConfigureSet();
	if ( !cs )
	{
		return;
	}

	/*setParam(0X00, 0X00);
	_sampleCount = RadarManager::Instance()->getSampCount( atoi ( cs->get("radar", "sample" ).c_str() ) );
	_sampleRatio = RadarManager::Instance()->getSampRatio( atoi( cs->get("radar", "samplingRate").c_str()), 0 );

	_channelCount = atoi( cs->get("radar", "count").c_str() );
	if ( _channelCount == 0 )
	{
		_channelCount = MAX_CHANNEL;
	}

	setParam( 0X01, atoi( cs->get("radar", "sample").c_str()));
	setParam( 0X02, atoi( cs->get("radar", "samplingRate").c_str()));
	setParam( 0X03, atoi( cs->get("radar", "gain").c_str()));
	setParam( 0X04, atoi( cs->get("radar", "gainvalue").c_str()));
	setParam( 0X07, atoi( cs->get("radar", "delay").c_str()));
	setParam( 0X08, atoi( cs->get("radar", "addcount").c_str()));
	setParam( 0X0A, atoi( cs->get("radar", "testtype").c_str()));
	
	m_iModeType = atoi( cs->get("radar", "testtype").c_str());
	//int precRatio = atoi( cs->get("radar", "precratio").c_str() );
	int precRatio = RadarManager::Instance()->getPrecRatio(atoi( cs->get("radar", "precratio").c_str()));
	float precLen = atof ( cs->get("radar", "preclen").c_str() );
	//int precIndex = atoi( cs->get("radar", "precindex").c_str() );
	

	//叶博士提供的方法
	//发送给主机的值=面板值（不是下标）/（周长/720）
	//每两个脉冲距离=发送给主机的数*周长/720                     ===== 面板值
	//float fJihuaAccuracy = RadarManager::Instance()->GetJihuaAccuracy(precIndex);//
	float fJihuaAccuracy = (float)atoi( cs->get("radar", "precision").c_str() );
	int iCmdIndex = RadarManager::Instance()->GetTrueCmdIndexFromPreclenAndAccuracy(precLen,fJihuaAccuracy);
	setParam( 0X0B, iCmdIndex);
	setParam( 0XFA, atoi( cs->get("radar", "id").c_str()));
	setParam(0X00, 0X0F);
	float fp = RadarManager::Instance()->GetTrueAccuracyFromPreclenAndPrecindex(precLen,fJihuaAccuracy);
	_precValue = fp / 100.0;*/

	m_nSampleCount = atoi ( cs->get("receive", "sampleCount" ).c_str() );
	m_nSampleRatioIndex = atoi ( cs->get("receive", "samplingRate").c_str() );
	m_fSettingInterval = 100/(float)atoi( cs->get("send", "measuringWheelPointPerMeter" ).c_str() );//单位厘米
	float fWheelCircumference = atof(cs->get("send","perimeter").c_str())*100;//米转厘米
	//m_fTrueInterval = RadarManager::Instance()->GetTrueAccuracyFromPreclenAndPrecindex(fWheelCircumference,m_fSettingInterval)/100;
	int nWheelPulse = RadarManager::Instance()->getPrecRatio(atoi(cs->get("send","wheelPulse").c_str()));
	m_fTrueInterval = RadarManager::Instance()->GetTrueInterval(fWheelCircumference,m_fSettingInterval,nWheelPulse)/100;
	m_iReadNULLTime = 0;
	_hadInit = true;
}


//void TERDataReader::initU()
//{
//	OpenThreads::ScopedLock<OpenThreads::Mutex> lock(_lock);
//	if ( _hadInit )
//	{
//		return;
//	}
//
//	if (RadarManager::Instance()->working())
//	{
//		open(m_iRadarWorkType);
//		_dataBuf.clear();
//		_hadInit = true;
//		m_iReadNULLTime = 0;
//	}
//}

void TERDataReader::sendData( unsigned char *buff, int len )
{
	if(!m_bIsConnected){
		AfxMessageBox(_T("请先对瞬变电磁雷达进行通信连接"));
		return;
	}
	_client.Send( buff, len );
}

//bool TERDataReader::open(int iType)
//{
//	_hadInit = false;
//	m_iRadarWorkType = iType;
//
//	for ( int i = 0 ; i < MAX_CHANNEL; i ++ )
//	{
//		_channelWheelCount[i] = -1;
//		_recordChannelWheelCount[i] = 0;//hjl20210717
//		_channelLen[i] = 0;
//	}
//
//	if (NULL == m_uPort)
//	{
//		m_uPort = new GD_UPortClient;
//	}
//	ConfigureSet *cs = RadarManager::Instance()->getConfigureSet();
//	if ( !cs )
//	{
//		return false;
//	}
//
//	m_uPort->StopSample();
//	m_uPort->m_iSampleNumIndex = atoi ( cs->get("radar", "sample" ).c_str() ) ;//采样点数
//	m_uPort->m_iSampleRateIndex = atoi( cs->get("radar", "sampleratio").c_str()) ;		//采样率
//
//	m_uPort->m_iBit = atoi( cs->get("radar", "gain").c_str()) - 1;					//增益模式  -1
//	m_uPort->m_iLevel = atoi( cs->get("radar", "gainvalue").c_str());					//增益值  0
//		//0x05                         //触发电平
//		//0x06                         //比特位数
//	m_uPort->m_attendigital = atoi( cs->get("radar", "delay" ).c_str() );		    //延迟调整  delay  6
//	m_uPort->m_iAddTimeIndex = atoi( cs->get("radar", "addcount" ).c_str() );			//累加次数		addcount	3	 
//	m_uPort->m_attenselindex = 2;			//重复频率
//
//	m_uPort->m_bMode = atoi( cs->get("radar", "testtype").c_str());					//测量轮使能
//	m_iModeType = m_uPort->m_bMode;
//	
//		//	 0x0A,0x00,			
//	m_uPort->m_iMeasureIndex = atoi( cs->get("radar", "precindex").c_str());			//测量轮精度   precindex   1  //这真的起作用的吗
//
//	_sampleCount = RadarManager::Instance()->getSampCount( atoi ( cs->get("radar", "sample" ).c_str() )  );
//	//int pSampRatio = atoi( cs->get("radar", "sampleratio").c_str());//old
//	_sampleRatio = RadarManager::Instance()->getSampRatio( atoi( cs->get("radar", "sampleratio").c_str()) , 0 );
//
//	_channelCount = atoi( cs->get("radar", "count").c_str() );
//	if ( _channelCount == 0 )
//	{
//		///默认双通道
//		_channelCount = 2;
//	}
//
//	//int precRatio = atoi( cs->get("radar", "precratio").c_str() );//脉冲数
//	int precRatio = RadarManager::Instance()->getPrecRatio(atoi( cs->get("radar", "precratio").c_str()));//脉冲数
//	float precLen = atof ( cs->get("radar", "preclen").c_str() );//测量轮周长
//	//int precIndex = atoi( cs->get("radar", "precindex").c_str() );//精度(用于标识的index)
//	//float fJihuaAccuracy = RadarManager::Instance()->GetJihuaAccuracy(precIndex);//测量轮精度(0,1,2,3,4-->1,2,5,10,20),
//	float fJihuaAccuracy = (float)atoi( cs->get("radar", "precision").c_str() );
//	m_uPort->m_iMeasure = RadarManager::Instance()->GetTrueCmdIndexFromPreclenAndAccuracy(precLen,fJihuaAccuracy);//
//	float fp = RadarManager::Instance()->GetTrueAccuracyFromPreclenAndPrecindex(precLen,fJihuaAccuracy);//获得实际测量轮精度
//	_precValue = fp / 100.0;//转化为米 是精度
//
//
//	//真正初始化雷达
//	if (false == m_uPort->Init(m_iRadarWorkType))
//	{
//		delete m_uPort;
//		m_uPort = NULL;
//		return false;		
//	}
//	_hadInit = true;
//
//	m_iReadNULLTime = 0;
//	return _hadInit;
//}

//bool TERDataReader::openForSetting( std::string const& targetIP, unsigned int port )
//{
//	for ( int i = 0 ; i < MAX_CHANNEL; i ++ )
//	{
//		_channelWheelCount[i] = -1;
//		_recordChannelWheelCount[i]=0;
//		_channelLen[i] = 0;
//	}
//	_client.setServerIP( targetIP );
//	_client.setServerPort( port );
//	bool value = _client.Connect();//创建无线网络通信是否成功
//	if ( value )
//	{
//		//init();
//		_hadInit = false;
//		if ( _lpTERReceiveThread )
//		{
//			_lpTERReceiveThread->cancel();
//			delete _lpTERReceiveThread;
//			_lpTERReceiveThread = NULL;
//		}
//		setParam( 0X00, 0X00 );//发送参数给硬件
// 		int recvNum = 0;
// 		while ( recvNum = _client.Recv( _recvBuff, 512 ) )//20170322 参数2为lnWantRecvLen 应该是用来调整接收时点的
// 		{
// 			Sleep( 10 );//20170322
// 		}
//		setParam( 0XFA, 0X88 );
//		Sleep( 100 );		//
//		recvNum = 0;
//		while ( recvNum += _client.Recv( _recvBuff, 512 ) )
//		//while ( recvNum += _client.Recv( _recvBuff, 256 ) )
//		{
//			_dataBuf.addData( _recvBuff, recvNum );
//			//if ( recvNum >= 512 )//交委的512
//			//{
//			//	break;
//			//}
//			if ( recvNum >= 256 )//20170321  256个字节
//			{
//				break;
//			}
//		}
//		parseHeadData( (char*)_dataBuf.getData(), _dataBuf.getDataLen() );//根据头信息初始化采集界面的通道名称之类的
//		_dataBuf.clear();
//// 		sendData( 0XFA );
//// 		sendData( 0X88 );
//		//_lpTERReceiveThread = new ReceiveTERDataThread( this );
//		//_lpTERReceiveThread->start();
//	}
//	return value;
//}

void TERDataReader::SocketConnect( std::string const& targetIP, unsigned int port ){
	//m_nTERDataCount=0;
	_client.setServerIP( targetIP );
	_client.setServerPort( port );
	bool value = _client.Connect();//创建无线网络通信是否成功
	m_bIsConnected=value;
	if ( value ){
		AfxMessageBox(_T("瞬变电磁雷达进行通信连接成功"));
	}else{
		AfxMessageBox(_T("瞬变电磁雷达进行通信连接失败"));
	}
}

void TERDataReader::SocketDisconnect(){
	if(m_bIsConnected){
		_client.CloseDevice();
		m_bIsConnected=false;
	}
}

bool TERDataReader::SetSenderFree(){
	if(!m_bIsConnected){
		AfxMessageBox(_T("请先对瞬变电磁雷达进行通信连接"));
		return false;
	}
	unsigned char *temp = new unsigned char[6];
	temp[0]=0x02;
	temp[1]=0x21;
	temp[2]=0x55;
	temp[3]=0x55;
	temp[4]=0x01;
	temp[5]=0x02;
	_client.Send( temp, 6 );
	_client.Recv( _recvBuff, 5 );
	if(!(_recvBuff[0]==0x06/*&&_recvBuff[1]==0x21&&_recvBuff[2]==0x55&&_recvBuff[3]==0x55*/)){
		return false;
	}
	return true;
}
bool TERDataReader::SetSenderParameter(){//02 20 55 55 05 01 0d 0a 00 00 e7 03 cc 00 40 01 03 00 00 00//0620
	if(!m_bIsConnected){
		AfxMessageBox(_T("请先对瞬变电磁雷达进行通信连接"));
		return false;
	}
	ConfigureSet *cs = RadarManager::Instance()->getConfigureSet();
	if ( !cs ){
		return false;
	}
	unsigned char *temp = new unsigned char[20];
	temp[0]=0x02;
	temp[1]=0x20;
	temp[2]=0x55;
	temp[3]=0x55;
	temp[4]=0x05;
	temp[5]=0x01;
	//频率
	//temp[6]=0x0d;
	temp[6]=(short)atoi(cs->get("send", "frequency").c_str())+1;
	//占空比
	//temp[7]=0x0a;
	temp[7]=(short)atoi(cs->get("send", "dutyRatio").c_str());
	//延时时间
	//temp[8]=0x00;
	//temp[9]=0x00;
	short sDelayTime=(short)atoi(cs->get("send", "delay").c_str());
	temp[8]=*((unsigned char*)&sDelayTime);
	temp[9]=*((unsigned char*)&sDelayTime+1);
	//激发次数 脉冲数
	temp[10]=0xe7;
	temp[11]=0x03;
	//测量轮脉冲次数 触发间隔
	//temp[12]=0xcc;//12*8+12=108
	//temp[13]=0x00;
	int nPulse = RadarManager::Instance()->getPrecRatio(atoi( cs->get("send", "wheelPulse").c_str()));
	int nPointPerMeter=atoi(cs->get("send", "measuringWheelPointPerMeter").c_str());
	double dPerimeter=atof(cs->get("send", "perimeter").c_str());
	short sInterval=(short)((float)nPulse*4.0/((float)nPointPerMeter*dPerimeter));
	temp[12]=*((unsigned char*)&sInterval);
	temp[13]=*((unsigned char*)&sInterval+1);
	//gps同步周期
	//temp[14]=0x40;
	//temp[15]=0x01;
	short sGPS=(short)(atoi(cs->get("send", "GPS").c_str())*2);
	temp[14]=*((unsigned char*)&sGPS);
	temp[15]=*((unsigned char*)&sGPS+1);
	//模式选择 01手动 02GPS 03测量轮
	//temp[16]=0x03;
	temp[16]=(short)atoi(cs->get("send", "mode").c_str())+1;
	//线圈选择
	temp[17]=(short)atoi(cs->get("send", "coil").c_str());
	//叠加次数
	//temp[18]=0x00;
	//temp[19]=0x00;
	short sStackingFold=(short)atoi(cs->get("send", "StackingFold").c_str());
	temp[18]=*((unsigned char*)&sStackingFold);
	temp[19]=*((unsigned char*)&sStackingFold+1);
	_client.Send( temp, 20 );
	_client.Recv( _recvBuff, 5 );
	if(!(_recvBuff[0]==0x06/*&&_recvBuff[1]==0x20&&_recvBuff[2]==0x55&&_recvBuff[3]==0x55*/)){
		return false;
	}
	return true;
}
bool TERDataReader::SetSenderReady(){//02 21 55 55 01 03
	if(!m_bIsConnected){
		AfxMessageBox(_T("请先对瞬变电磁雷达进行通信连接"));
		return false;
	}
	unsigned char *temp = new unsigned char[6];
	temp[0]=0x02;
	temp[1]=0x21;
	temp[2]=0x55;
	temp[3]=0x55;
	temp[4]=0x01;
	temp[5]=0x03;
	_client.Send( temp, 6 );
	_client.Recv( _recvBuff, 5);
	if(!(_recvBuff[0]==0x06/*&&_recvBuff[1]==0x21&&_recvBuff[2]==0x55&&_recvBuff[3]==0x55*/)){
		return false;
	}
	return true;
}
bool TERDataReader::SetSenderStart(){//02 21 55 55 01 05
	if(!m_bIsConnected){
		AfxMessageBox(_T("请先对瞬变电磁雷达进行通信连接"));
		return false;
	}
	unsigned char *temp = new unsigned char[6];
	temp[0]=0x02;
	temp[1]=0x21;
	temp[2]=0x55;
	temp[3]=0x55;
	temp[4]=0x01;
	temp[5]=0x05;
	_client.Send( temp, 6 );
	_client.Recv( _recvBuff, 5);
	if(!(_recvBuff[0]==0x06/*&&_recvBuff[1]==0x21&&_recvBuff[2]==0x55&&_recvBuff[3]==0x55*/)){
		return false;
	}
	return true;
}
bool TERDataReader::SetSenderStop(){//02 21 55 55 01 06 
	if(!m_bIsConnected){
		AfxMessageBox(_T("请先对瞬变电磁雷达进行通信连接"));
		return false;
	}
	unsigned char *temp = new unsigned char[6];
	temp[0]=0x02;
	temp[1]=0x21;
	temp[2]=0x55;
	temp[3]=0x55;
	temp[4]=0x01;
	temp[5]=0x06;
	_client.Send( temp, 6 );
	Sleep(200);
	_client.Recv( _recvBuff, 5);
	if(!(_recvBuff[0]==0x06&&_recvBuff[1]==0x21&&_recvBuff[2]==0x55&&_recvBuff[3]==0x55&&_recvBuff[4]==0x06)){
		return false;
	}
	return true;
}
bool TERDataReader::SetReceiverFree(){//02 03 55 55 09 02
	if(!m_bIsConnected){
		AfxMessageBox(_T("请先对瞬变电磁雷达进行通信连接"));
		return false;
	}
	unsigned char *temp = new unsigned char[6];
	temp[0]=0x02;
	temp[1]=0x03;
	temp[2]=0x55;
	temp[3]=0x55;
	temp[4]=0x09;
	temp[5]=0x02;
	_client.Send( temp, 6 );
	_client.Recv( _recvBuff, 5);
	if(!(_recvBuff[0]==0x06/*&&_recvBuff[1]==0x21&&_recvBuff[2]==0x55&&_recvBuff[3]==0x55*/)){
		return false;
	}
	return true;
}
bool TERDataReader::SetReceiverParameter(){//02 01 55 55 05 05 03 4a 01 01 00 00 40 0d 50 00 0d 0a 40 01
	if(!m_bIsConnected){
		AfxMessageBox(_T("请先对瞬变电磁雷达进行通信连接"));
		return false;
	}
	ConfigureSet *cs = RadarManager::Instance()->getConfigureSet();
	if ( !cs ){
		return false;
	}
	unsigned char *temp = new unsigned char[20];
	temp[0]=0x02;
	temp[1]=0x01;
	temp[2]=0x55;
	temp[3]=0x55;
	temp[4]=0x05;
	//接收采样率
	temp[5]=atoi(cs->get("receive", "samplingRate").c_str())+1;
	//采集触发模式
	//temp[6]=0x03;//01-同步线缆触发 02-GPS同步触发 03-测量轮触发 04-手动触发 05-zigbee触发 
	temp[6]=(short)atoi(cs->get("send", "mode").c_str())+1;
	//采样道数
	//temp[7]=0x4a;
	//temp[8]=0x01;
	short sSampleCount=(short)atoi(cs->get("receive", "sampleCount").c_str());
	temp[7]=*((unsigned char*)&sSampleCount);
	temp[8]=*((unsigned char*)&sSampleCount+1);
	//放大增益
	//temp[9]=0x01;
	temp[9]=(short)atoi(cs->get("receive", "gain").c_str())+1;
	//叠加次数
	short sStackingFold=(short)atoi(cs->get("receive", "stackingFold").c_str());
	temp[10]=*((unsigned char*)&sStackingFold);
	temp[11]=*((unsigned char*)&sStackingFold+1);
	//总道数
	temp[12]=0x40;
	temp[13]=0x0d;
	//延时时间
	//temp[14]=0x50;
	//temp[15]=0x00;
	short sDelayTime=(short)atoi(cs->get("receive", "delay").c_str())*10;
	temp[14]=*((unsigned char*)&sDelayTime);
	temp[15]=*((unsigned char*)&sDelayTime+1);
	//发射频率
	//temp[16]=0x0d;
	temp[16]=(short)atoi(cs->get("send", "frequency").c_str())+1;
	//占空比
	//temp[17]=0x0a;
	temp[17]=(short)atoi(cs->get("send", "dutyRatio").c_str());
	//gps同步周期
	//temp[18]=0x40;
	//temp[19]=0x01;
	short sGPS=(short)(atoi(cs->get("send", "GPS").c_str())*2);
	temp[18]=*((unsigned char*)&sGPS);
	temp[19]=*((unsigned char*)&sGPS+1);
	_client.Send( temp, 20 );
	_client.Recv( _recvBuff, 5);
	if(!(_recvBuff[0]==0x06/*&&_recvBuff[1]==0x20&&_recvBuff[2]==0x55&&_recvBuff[3]==0x55*/)){
		return false;
	}
	return true;
}
bool TERDataReader::SetReceiverReady(){//02 03 55 55 09 03
	if(!m_bIsConnected){
		AfxMessageBox(_T("请先对瞬变电磁雷达进行通信连接"));
		return false;
	}
	unsigned char *temp = new unsigned char[6];
	temp[0]=0x02;
	temp[1]=0x03;
	temp[2]=0x55;
	temp[3]=0x55;
	temp[4]=0x09;
	temp[5]=0x03;
	_client.Send( temp, 6 );
	_client.Recv( _recvBuff, 5);
	if(!(_recvBuff[0]==0x06/*&&_recvBuff[1]==0x03&&_recvBuff[2]==0x55&&_recvBuff[3]==0x55*/)){
		return false;
	}
	return true;
}
bool TERDataReader::SetReceiverStart(){//02 02 55 55 06 05
	if(!m_bIsConnected){
		AfxMessageBox(_T("请先对瞬变电磁雷达进行通信连接"));
		return false;
	}
	unsigned char *temp = new unsigned char[6];
	temp[0]=0x02;
	temp[1]=0x02;
	temp[2]=0x55;
	temp[3]=0x55;
	temp[4]=0x06;
	temp[5]=0x05;
	_client.Send( temp, 6 );
	_client.Recv( _recvBuff, 5);
	if(!(_recvBuff[0]==0x06/*&&_recvBuff[1]==0x03&&_recvBuff[2]==0x55&&_recvBuff[3]==0x55*/)){
		return false;
	}
	return true;
}
bool TERDataReader::SetReceiverStop(){//02 02 55 55 06 06
	if(!m_bIsConnected){
		AfxMessageBox(_T("请先对瞬变电磁雷达进行通信连接"));
		return false;
	}
	unsigned char *temp = new unsigned char[6];
	temp[0]=0x02;
	temp[1]=0x02;
	temp[2]=0x55;
	temp[3]=0x55;
	temp[4]=0x06;
	temp[5]=0x06;
	_client.Send( temp, 6 );
	Sleep(200);
	_client.Recv( _recvBuff, 5);
	if(!(_recvBuff[0]==0x06&&_recvBuff[1]==0x02&&_recvBuff[2]==0x55&&_recvBuff[3]==0x55&&_recvBuff[4]==0x06)){
		return false;
	}
	return true;
}

bool TERDataReader::open( std::string const& targetIP, unsigned int port )
{
	m_nTERDataCount=0;
	bool value = m_bIsConnected;
	if(!m_bIsConnected){
		_client.setServerIP( targetIP );
		_client.setServerPort( port );
		value = _client.Connect();//创建无线网络通信是否成功
		m_bIsConnected=value;
	}
	if ( value )
	{
		//init();
		_hadInit = false;
		if ( _lpTERReceiveThread )
		{
			_lpTERReceiveThread->cancel();
			delete _lpTERReceiveThread;
			_lpTERReceiveThread = NULL;
		}

		if ( _lpTERProcessThread )
		{
			_lpTERProcessThread->cancel();
			delete _lpTERProcessThread;
			_lpTERProcessThread = NULL;
		}

		//雷达空闲02 21 55 55 01 02
		Sleep(500);
		SetSenderFree();
		Sleep(500);
		SetSenderParameter();
		Sleep(500);
		SetSenderReady();
		Sleep(500);
		bool bSetSenderStartSuccess=SetSenderStart();
		while(!bSetSenderStartSuccess){
			Sleep(500);
			bSetSenderStartSuccess=SetSenderStart();
		}	
 		
		Sleep(500);
		SetReceiverFree();
		Sleep(500);
		SetReceiverParameter();
		Sleep(500);
		SetReceiverReady();
		Sleep(1000);
		bool bSetReceiverStartSuccess=SetReceiverStart();
		while(!bSetReceiverStartSuccess){
			Sleep(500);
			bSetReceiverStartSuccess=SetReceiverStart();
		}	
 		
		int recvNum = 0;
 		while ( recvNum = _client.Recv( _recvBuff, 512 ) )//20170322 参数2为lnWantRecvLen 应该是用来调整接收时点的
 		{
 			Sleep( 10 );//20170322
 		}

		_dataBuf.clear();

	}

	return value;
}

void TERDataReader::ReadThreadStart(bool flag /*= true*/)
{
	if (true == flag )
	{
		if( NULL != _lpTERReceiveThread)
		{
			_lpTERReceiveThread->cancel();
			delete _lpTERReceiveThread;
			_lpTERReceiveThread = NULL;
		}
		_lpTERReceiveThread = new ReceiveTERDataThread( this );
		_lpTERReceiveThread->start();

		if( NULL != _lpTERProcessThread)
		{
			_lpTERProcessThread->cancel();
			delete _lpTERProcessThread;
			_lpTERProcessThread = NULL;
		}
		_lpTERProcessThread = new ProcessTERDataThread( this );
		_lpTERProcessThread->start();
	}
}

void TERDataReader::parseHeadData( char *buff, int len )
{
	RadarManager *lpManager = RadarManager::Instance();
	if ( !lpManager )
	{
		return;
	}

	int startPos = 0;
	int endPos = 0;
	for ( int i = 0; i < len - 1; i ++ )//例子 SSID=MC8;
	{
		if ( buff[i] == '=' )
		{
			startPos = i + 1;
		}
		if ( buff[i] == ';' )//分号
		{
			endPos = i;
		}
		if ( endPos > 0 )
		{
			break;
		}
	}

	char deviceName[32] = {'\0'};
	if ( endPos - startPos < 32 )
	{
		for ( int i = 0; i < endPos - startPos; i ++ )
		{
			deviceName[i] = buff[startPos + i];
		}
		RadarManager::Instance()->setDeviceCode( std::string(deviceName) );
	}


	float basicRatio =-1.0f;

	//PARTNAME
	for ( int i = 0; i < len - 14; i ++ )//例子PORTNAME1=S0200:5.12G;
	{
		if ( buff[i] == 'P'
			&& buff[i + 1] == 'O'
			&& buff[i + 2] == 'R'
			&& buff[i + 3] == 'T'
			&& buff[i + 4] == 'N'
			&& buff[i + 5] == 'A'
			&& buff[i + 6] == 'M'
			&& buff[i + 7] == 'E'
			)
		{
			int index = buff[i + 8] - '1';
			char name[6];
			name[0] = buff[i + 10];
			name[1] = buff[i + 11];
			name[2] = buff[i + 12];
			name[3] = buff[i + 13];
			name[4] = buff[i + 14];
			name[5] = '\0';
			
			//lpManager->setChannelName( name, index );//01234567891011121314
			/*
			lpManager->setChannelName( name, RadarManager::Instance()->originalIndexToRecordIndex(index*2) );//16通道里一个表示两个
			lpManager->setChannelName( name, RadarManager::Instance()->originalIndexToRecordIndex(index*2+1) );//16通道里一个表示两个
			if (index*2!=0&&index*2!=12){
				lpManager->setChannelName( name, RadarManager::Instance()->originalIndexToRecordIndex(RadarManager::Instance()->getArtificalChannelIndexFromOriginalIndex(index*2)));//16通道里一个表示两个
			}
			if ((index*2+1)!= 15){
				lpManager->setChannelName( name, RadarManager::Instance()->originalIndexToRecordIndex(RadarManager::Instance()->getArtificalChannelIndexFromOriginalIndex(index*2+1)));//16通道里一个表示两个
			}*/
			/*
			if (index<6){
				lpManager->setChannelName( name, index*2 );//16通道里一个表示两个
				lpManager->setChannelName( name, index*2+1 );//16通道里一个表示两个
				lpManager->setChannelName( name, index*2+12 );//28通道的复制
				lpManager->setChannelName( name, index*2+13 );//28通道的复制
			}else{
				lpManager->setChannelName( name, index*2+12 );//16通道里一个表示两个
				lpManager->setChannelName( name, index*2+13 );//16通道里一个表示两个
			}*/
			lpManager->setChannelName( name, index*2 );//16通道里一个表示两个
			lpManager->setChannelName( name, index*2+1 );//16通道里一个表示两个
			if(index<6){
				lpManager->setChannelName( name, index*2+16 );//16通道里一个表示两个
				lpManager->setChannelName( name, index*2+17 );//16通道里一个表示两个
			}

			std::string strRatio;
			int ratioStartPos = 0;
			int ratioEndPos = 0;
			for ( int ratioStart = i + 15; ratioStart < len; ratioStart  ++)//找到采样率信息的起止点与终止点
			{
				if ( buff[ratioStart] == ':' )//冒号
				{
					if ( ratioStartPos <= 0 )
					{
						ratioStartPos = ratioStart;
					}
				}
				if ( buff[ratioStart] == ';')//分号
				{
					ratioEndPos = ratioStart;
				}

				if ( ratioStartPos > 0 && ratioEndPos > 0 )
				{
					break;
				}
			}

			for ( int j = ratioStartPos + 1; j < ratioEndPos; j ++ )
			{
				strRatio += buff[j];
			}

			float channelRatio = atof ( strRatio.c_str() );
			if ( basicRatio < 0 )
			{
				basicRatio = channelRatio;
			}

			if ( channelRatio == 0.0f )
			{
				channelRatio = basicRatio;
			}
			lpManager->setChannelRatioPrect(  index, basicRatio / channelRatio );
		}
	}
}

bool TERDataReader::close(){
	if(!m_bIsConnected){
		//AfxMessageBox(_T("请先对瞬变电磁雷达进行通信连接"));
		return false;
	}
	if ( !_lpTERReceiveThread )
	{
		SetReceiverStop();
		Sleep(2000);
		SetSenderStop();
		Sleep(2000);
		SetSenderStop();//保证关成功
		Sleep(2000);
		_client.CloseDevice();
		return true;
	}

	if ( _lpTERProcessThread )
	{
		_lpTERProcessThread->cancel();
		delete _lpTERProcessThread;
		_lpTERProcessThread = NULL;
	}

	if ( _lpTERReceiveThread )
	{
		_lpTERReceiveThread->cancel();
		delete _lpTERReceiveThread;
		_lpTERReceiveThread = NULL;
	}

	Sleep(500);
	bool bIsSuccessed;
	int nSetCount;

	bIsSuccessed=SetSenderStop();
	nSetCount=0;
	while(!bIsSuccessed){
		bIsSuccessed=SetSenderStop();
		nSetCount=nSetCount+1;
		Sleep(200);
		if(nSetCount>10){
			AfxMessageBox(_T("自动停止发射机失败，请手动停止"));
			break;
		}
	}
	/*bIsSuccessed=false;
	nSetCount=0;
	while(bIsSuccessed){
		bIsSuccessed=SetSenderStop();
		nSetCount=nSetCount+1;
		Sleep(200);
		if(nSetCount>10){
			AfxMessageBox(_T("自动停止发射机失败，请手动停止"));
			break;
		}
	}*/

	Sleep(1000);

	bIsSuccessed=SetReceiverStop();
	nSetCount=0;
	while(!bIsSuccessed){
		bIsSuccessed=SetReceiverStop();
		nSetCount=nSetCount+1;
		Sleep(200);
		if(nSetCount>20){
			AfxMessageBox(_T("自动停止接收机失败，请手动停止"));
			break;
		}
	}
	/*bIsSuccessed=false;
	nSetCount=0;
	while(bIsSuccessed){
		bIsSuccessed=SetReceiverStop();
		nSetCount=nSetCount+1;
		Sleep(200);
		if(nSetCount>10){
			AfxMessageBox(_T("自动停止接收机失败，请手动停止"));
			break;
		}
	}*/

	


	/*while(!SetReceiverStop()){
		SetReceiverStop();
		Sleep(1000);
	}
	Sleep(1000);
	while(!SetReceiverStop()){
		SetReceiverStop();
		Sleep(1000);
	}
	Sleep(1000);
	while(!SetSenderStop()){
		SetSenderStop();
		Sleep(1000);
	}
	Sleep(1000);
	while(!SetSenderStop()){//保证关成功
		SetSenderStop();
		Sleep(1000);
	}*/
	
	
	_client.CloseDevice();
	m_bIsConnected=false;
	return true;
}

int TERDataReader::readData(){
	//long wantToRecv = MAX_RECV;
	long wantToRecv=(m_nSampleCount*3+8)*2;
	memset( _recvBuff, 0, MAX_RECV );
	int recvNum = _client.Recv( _recvBuff, wantToRecv );
	if ( recvNum == -1 )
	{
		_hadInit = false;
		_client.ReConnect();
		return recvNum;
	}

	if ( !_hadInit )
	{
		return recvNum;
	}
	if ( recvNum > 0 )
	{
		_dataBuf.addData( _recvBuff, recvNum );
	}
	return recvNum;
}




bool TERDataReader::isOpen()
{
	return _client.m_bConnected;
}


extern float GetFloatFromPack( unsigned char* in_lp_num , char in_bits_flag ){
	int onepoint = 0;
	float op = 0;
	// float maxvalue = 5.0f;
	float maxvalue = 2.5f;

	switch(in_bits_flag){
		case 8:
			onepoint = ((int)in_lp_num[0]);
			onepoint = onepoint<<16;
			if ( onepoint == 0x00800000 ){
				op = -maxvalue;// * (float)onepoint * pow(2, 23) * 5 / (pow(2, 23) - 1);
			}else{
				if ( !(onepoint & 0x00800000) ){
					op = (float)onepoint * maxvalue / (pow( (float)2, (float)23) - 1);
				}else{
					onepoint = ((~onepoint)&0x00ffffff) + 0x000001;
					op = -1 * (float)onepoint * maxvalue / (pow((float)2, (float)23) - 1);
				}
			}
			break;
		case 16:
			onepoint = ((int)in_lp_num[0]) + ((int)in_lp_num[1]<<8);
			onepoint = onepoint<<8;
			if ( onepoint == 0x00800000 ){
				op = -maxvalue;// * (float)onepoint * pow(2, 23) * 5 / (pow(2, 23) - 1);
			}else{
				if ( !(onepoint & 0x00800000) ){
					op = (float)onepoint * maxvalue / (pow((float)2, (float)23) - 1);
				}else{
					onepoint = ((~onepoint)&0x00ffffff) + 0x000001;
					op = -1 * (float)onepoint * maxvalue / (pow((float)2, (float)23) - 1);
				}
			}
			break;
		case 24:
			//onepoint = ((int)in_lp_num[0]) + ((int)in_lp_num[1]<<8) + ((int)in_lp_num[2]<<16);
			onepoint = ((int)in_lp_num[2]) + ((int)in_lp_num[1]<<8) + ((int)in_lp_num[0]<<16);
			if ( onepoint == 0x00800000 ){
				op = -maxvalue;// * (float)onepoint * pow(2, 23) * 5 / (pow(2, 23) - 1);
			}else{
				if ( !(onepoint & 0x00800000) ){
					op = (float)onepoint * maxvalue / (pow((float)2, (float)23) - 1);
				}else{
					onepoint = ((~onepoint)&0x00ffffff) + 0x000001;
					op = -1 * (float)onepoint * maxvalue / (pow( (float)2, (float)23) - 1);
				}
			}
			break;
		default:
			break;
	}

	return op;
}

void TERDataReader::parseData()
{
	//int channelData = /*_channelCount * */m_nSampleCount * 3 + 8;
	int channelData = /*_channelCount * */m_nSampleCount * 3 + 10;
	int len = _dataBuf.getDataLen();

	if ( len < channelData ){
		return;
	}
	unsigned int startPos = 0;
	
	while ( true ){
		unsigned char *lpDataBuff = _dataBuf.getData();
		bool findHead = false;

		if ( len - startPos < channelData ){
			break;
		}
		//定位到道头信息
		for (; startPos < len - 4; startPos++){
			if (lpDataBuff[startPos] == unsigned char(0X02) && lpDataBuff[1 + startPos] == unsigned char(0X04) && lpDataBuff[2 + startPos] == unsigned char(0X55) && lpDataBuff[3 + startPos] == unsigned char(0X55)){
				findHead = true;
				break;
			}
		}
		if ( !findHead ){
			break;
		}
		if ( startPos + channelData > _dataBuf.getDataLen() ){
			break;
		}

		unsigned char *tempBuff=new unsigned char[channelData];
		for(int i=0;i<channelData;i++){
			tempBuff[i]=lpDataBuff[startPos+i];
		}	
		((ProcessTERDataThread*)_lpTERProcessThread)->addData(tempBuff);

		/*
		//处理裸数据
		for(int i=0;i<m_nSampleCount;i++){
			if(m_bIsDataOne){
				//m_vecfDataOne.push_back(GetFloatFromPack(lpDataBuff+startPos+6+i*3,24));
				m_vecfDataOne.push_back(GetFloatFromPack(lpDataBuff+startPos+8+i*3,24));
			}else{
				//m_vecfDataTwo.push_back(GetFloatFromPack(lpDataBuff+startPos+6+i*3,24))
				m_vecfDataTwo.push_back(GetFloatFromPack(lpDataBuff+startPos+8+i*3,24));
			}
		}

		if(m_bIsDataOne==false){
			//获取道号 不再是short 现在是int
			//short sRecordWheelCount=(lpDataBuff[4] + lpDataBuff[5] * 256)/2;
			int nRecordWheelCount=(lpDataBuff[4] + lpDataBuff[5] * 256 +  lpDataBuff[6] * 65536 + lpDataBuff[7] * 16777216)/2+1;

			float *fTempBuff = new float[m_nSampleCount];
			for(int i=0;i<m_nSampleCount;i++){
				fTempBuff[i]=m_vecfDataOne[i]-m_vecfDataTwo[i];
				if(fTempBuff[i]<-2.5){
					fTempBuff[i]=-2.5;
				}else if(fTempBuff[i]>2.5){
					fTempBuff[i]=2.5;
				}
			}
			m_vecfDataOne.clear();
			m_vecfDataTwo.clear();

			osg::ref_ptr<TERData> data = new TERData;
			data->setSampleCount( m_nSampleCount );
			data->setSampleRatio( m_nSampleRatioIndex );
			data->setData( (unsigned char*)fTempBuff, m_nSampleCount*sizeof(float) );
			data->setPrecLen( m_fTrueInterval*nRecordWheelCount );
			data->setRecordWheelCount(nRecordWheelCount);
			
			//丢道时补空数据
			if(nRecordWheelCount>m_nTERDataCount+1){
				int nEmptyTERDataCount=nRecordWheelCount-m_nTERDataCount-1;
				for(int i=0;i<nEmptyTERDataCount;i++){
					int nTempWheelCount=m_nTERDataCount+1+i;
					osg::ref_ptr<TERData> data = new TERData;
					data->setSampleCount( m_nSampleCount );
					data->setSampleRatio( m_nSampleRatioIndex );
					float *fEmptyBuff = new float[m_nSampleCount];
					for(int j=0;j<m_nSampleCount;j++){
						fEmptyBuff[i]=0;
					}
					data->setData( (unsigned char*)fEmptyBuff, m_nSampleCount*sizeof(float) );
					data->setPrecLen( m_fTrueInterval*nTempWheelCount );
					data->setRecordWheelCount(0);//用来作为空数据的标识
					RadarManager::Instance()->addTERDataToProject( data.get() );
				}
			}


			RadarManager::Instance()->addTERDataToProject( data.get() );
			m_nTERDataCount=nRecordWheelCount;
		}
		if(m_bIsDataOne==true){
			m_bIsDataOne=false;
		}else{
			m_bIsDataOne=true;
		}*/
		startPos += channelData;
	}
	_dataBuf.remove( startPos );
}

void TERDataReader::ProcessData(unsigned char *lpDataBuff){
	for(int i=0;i<m_nSampleCount;i++){
		if(m_bIsDataOne){
			//m_vecfDataOne.push_back(GetFloatFromPack(lpDataBuff+startPos+6+i*3,24));
			m_vecfDataOne.push_back(GetFloatFromPack(lpDataBuff+8+i*3,24));
		}else{
			//m_vecfDataTwo.push_back(GetFloatFromPack(lpDataBuff+startPos+6+i*3,24))
			m_vecfDataTwo.push_back(GetFloatFromPack(lpDataBuff+8+i*3,24));
		}
	}

	if(m_bIsDataOne==false){
		//获取道号 不再是short 现在是int
		//short sRecordWheelCount=(lpDataBuff[4] + lpDataBuff[5] * 256)/2;
		int nRecordWheelCount=(lpDataBuff[4] + lpDataBuff[5] * 256 +  lpDataBuff[6] * 65536 + lpDataBuff[7] * 16777216)/2+1;

		float *fTempBuff = new float[m_nSampleCount];
		for(int i=0;i<m_nSampleCount;i++){
			fTempBuff[i]=m_vecfDataOne[i]-m_vecfDataTwo[i];
			if(fTempBuff[i]<-2.5){
				fTempBuff[i]=-2.5;
			}else if(fTempBuff[i]>2.5){
				fTempBuff[i]=2.5;
			}
		}
		m_vecfDataOne.clear();
		m_vecfDataTwo.clear();

		osg::ref_ptr<TERData> data = new TERData;
		data->setSampleCount( m_nSampleCount );
		data->setSampleRatio( m_nSampleRatioIndex );
		data->setData( (unsigned char*)fTempBuff, m_nSampleCount*sizeof(float) );
		data->setPrecLen( m_fTrueInterval*nRecordWheelCount );
		data->setRecordWheelCount(nRecordWheelCount);
		
		//丢道时补空数据
		if(nRecordWheelCount>m_nTERDataCount+1){
			int nEmptyTERDataCount=nRecordWheelCount-m_nTERDataCount-1;
			for(int i=0;i<nEmptyTERDataCount;i++){
				int nTempWheelCount=m_nTERDataCount+1+i;
				osg::ref_ptr<TERData> data = new TERData;
				data->setSampleCount( m_nSampleCount );
				data->setSampleRatio( m_nSampleRatioIndex );
				float *fEmptyBuff = new float[m_nSampleCount];
				for(int j=0;j<m_nSampleCount;j++){
					fEmptyBuff[i]=0;
				}
				data->setData( (unsigned char*)fEmptyBuff, m_nSampleCount*sizeof(float) );
				data->setPrecLen( m_fTrueInterval*nTempWheelCount );
				data->setRecordWheelCount(0);//用来作为空数据的标识
				RadarManager::Instance()->addTERDataToProject( data.get() );
			}
		}


		RadarManager::Instance()->addTERDataToProject( data.get() );
		m_nTERDataCount=nRecordWheelCount;
	}
	if(m_bIsDataOne==true){
		m_bIsDataOne=false;
	}else{
		m_bIsDataOne=true;
	}
}


void TERDataReader::parseDataToDisplay()
{
	int channelData = /*_channelCount * */_sampleCount * sizeof ( short );
	int len = _dataBuf.getDataLen();

	if ( len < channelData ){
		return;
	}
	unsigned int startPos = 0;
	
	while ( true )
	{
		unsigned char *lpDataBuff = _dataBuf.getData();
		bool findHead = false;

		if ( len - startPos < channelData ){
			break;
		}
		//定位到道头信息
		for (; startPos < len - 4; startPos++){
			if (lpDataBuff[startPos] == unsigned char(0X54) && lpDataBuff[2 + startPos] == unsigned char(0X54)
				&& ( ( lpDataBuff[1 + startPos] >= CHANEL_INDEX_BEGIN )&& ( lpDataBuff[1 + startPos] <= CHANEL_INDEX_END ) )
				&& ( ( lpDataBuff[3 + startPos] >= CHANEL_INDEX_BEGIN )&& ( lpDataBuff[3 + startPos] <= CHANEL_INDEX_END ) )
				)
			{
				findHead = true;
				break;
			}
		}

		if ( !findHead ){
			break;
		}

		if ( startPos + channelData > _dataBuf.getDataLen() ){
			break;
		}

		osg::ref_ptr<RadarData> rd = new RadarData;
		rd->setSampleCount( _sampleCount );
		rd->setSampleRatio( _sampleRatio );
		rd->setData( lpDataBuff + startPos, channelData );
		int index = lpDataBuff[startPos + 1] - CHANEL_INDEX_BEGIN;
		int wheelCount = rd->getPrec();//测量轮道数
		startPos += channelData;
		/*
		if ( _channelWheelCount[index] < 0 )//第一轮初始化
		{
			_channelWheelCount[index] = wheelCount;//记录当前道数
			_recordChannelWheelCount[index] = 0;//储存道数归0
			_channelLen[index] = 0;//总长度归0
			rd->setPrecLen( 0.0f );//雷达数据总长度信息设0
			rd->setPrec(0);//雷达数据道数储存为0
		}else{
			int count ;
			if ( wheelCount < _channelWheelCount[index]&& _channelWheelCount[index]>65500){//转满65535重新归0
				count = wheelCount + 65535 - _channelWheelCount[index];
			}else{
				count = wheelCount  - _channelWheelCount[index];
			}
			_channelWheelCount[index] = wheelCount;
			_channelLen[index] += _precValue * count;
			_recordChannelWheelCount[index] += count;
			rd->setPrecLen ( _channelLen[index] );
			rd->setPrec(_recordChannelWheelCount[index]);
		}
		*/

		RadarManager::Instance()->addRadarDataToParameterConfig( rd.get(), index );

		//RadarManager::Instance()->addRadarDataToParameterConfig( rd.get(), index );
		//_dataBuf.remove( startPos);
	}
	_dataBuf.remove( startPos );
}



//读取U口的数据
bool TERDataReader::ReadUData()
{
	if ( !_hadInit )
	{
		return false;
	}

	long wantToRecv = MAX_RECV;

	if (m_uPort->Recv(_recvBuff,wantToRecv))
	{
		_dataBuf.addData( _recvBuff, wantToRecv );
		return true;
	}
	return  false;

	memset( _recvBuff, 0, MAX_RECV );
	for (int i = 0; i < MAX_RECV;++i)
	{
		_recvBuff[i] = '1';
	}
	_dataBuf.addData( _recvBuff, wantToRecv );
	return false;
}




void TERDataReader::parseSixteenData()
{
	int oneChannelData = /*_channelCount * */_sampleCount * sizeof ( short );
	int sixteenChannelData = oneChannelData * 16;
	int len = _dataBuf.getDataLen();

	if ( len < sixteenChannelData ){
		return;
	}
	unsigned int startPos = 0;
	
	while ( true ){
		unsigned char *lpDataBuff = _dataBuf.getData();
		bool findHead = false;

		if ( len - startPos < sixteenChannelData ){
			break;
		}
		////定位到第一道道头信息
		//for (; startPos < len - 4; startPos++){
		//	//"TrTr"
		//	if (lpDataBuff[startPos] == unsigned char(0X54) && lpDataBuff[1 + startPos] == unsigned char(0X72) && lpDataBuff[2 + startPos] == unsigned char(0X54) && lpDataBuff[3 + startPos] == unsigned char(0X72)){
		//		findHead = true;
		//		break;
		//	}
		//}
		//定位到任意一道道头信息
		for (; startPos < len - 4; startPos++){
			if (lpDataBuff[startPos] == unsigned char(0X54) && lpDataBuff[2 + startPos] == unsigned char(0X54)
				&& ( ( lpDataBuff[1 + startPos] >= CHANEL_INDEX_BEGIN )&& ( lpDataBuff[1 + startPos] <= CHANEL_INDEX_END ) )
				&& ( ( lpDataBuff[3 + startPos] >= CHANEL_INDEX_BEGIN )&& ( lpDataBuff[3 + startPos] <= CHANEL_INDEX_END ) )
				)
			{
				findHead = true;
				break;
			}
		}

		if ( !findHead ){
			break;
		}

		if ( startPos + sixteenChannelData > _dataBuf.getDataLen() ){
			break;
		}
		
		std::vector<osg::ref_ptr<RadarData>> rd(16);
		for (int i=0;i<16;i++){
			rd[i] = new RadarData;
			rd[i]->setSampleCount( _sampleCount );
			rd[i]->setSampleRatio( _sampleRatio );
			rd[i]->setData( lpDataBuff + startPos + oneChannelData * i, oneChannelData );
			int index = lpDataBuff[startPos + oneChannelData * i + 1] - CHANEL_INDEX_BEGIN;
			int wheelCount = rd[i]->getPrec();//测量轮道数
			

			if ( _channelWheelCount[index] < 0 ){//第一轮初始化
				_channelWheelCount[index] = wheelCount;//记录当前道数
				_recordChannelWheelCount[index] = 0;//储存道数归0
				_channelLen[index] = 0;//总长度归0
				rd[i]->setPrecLen( 0.0f );//雷达数据总长度信息设0
				rd[i]->setPrec(0);//雷达数据道数储存为0
			}else{
				int count ;
				if ( wheelCount < _channelWheelCount[index]&& _channelWheelCount[index]>65500){//转满65535重新归0
					count = wheelCount + 65535 - _channelWheelCount[index];
				}else{
					count = wheelCount  - _channelWheelCount[index];
				}
				_channelWheelCount[index] = wheelCount;
				_channelLen[index] += _precValue * count;
				_recordChannelWheelCount[index] += count;
				rd[i]->setPrecLen ( _channelLen[index] );
				rd[i]->setPrec(_recordChannelWheelCount[index]);
			}

			RadarManager::Instance()->addRadarDataToProject( rd[i].get(), index );
		}
		startPos += sixteenChannelData;
		//_dataBuf.remove( startPos);
	}
	_dataBuf.remove( startPos );//数组减去startPos个元素
}

bool TERDataReader::SetChannelLen(RadarData* _radar , int index)
{
	int wheelCount = _radar->getPrec();//当前雷达数据记录的道数
	//if (wheelCount > 2)
	//{
	//	wheelCount++;
	//}
	//if (0 > wheelCount)
	//{
	//	wheelCount++;
	//}
	if ( _channelWheelCount[index] < 0 ){
		if ( wheelCount == 0 || wheelCount == 1 ){
			_channelWheelCount[index] = wheelCount;
			_channelLen[index] = 0;
			_radar->setPrecLen( 0.0f );
		}else{
			return false;
		}
	}else{
		int count ;//当前道数比之前增加了几道，用于计算总路程
		//if ( wheelCount < _channelWheelCount[index] && wheelCount == 0 && _channelWheelCount[index] % 65535 == 0 )//wheelCount是个通过date[5]*256+date[6]来计算的最多到65535（计算机十六位二进制最多到65535），再转一圈会变1所以加65535
		/*
		if ( wheelCount < _channelWheelCount[index] && wheelCount == 0 && _channelWheelCount[index] % 65535 == 0 &&_channelLen[index] >= 60000*_precValue ){//wheelCount是个通过date[5]*256+date[6]来计算的最多到65535（计算机十六位二进制最多到65535），再转一圈会变1所以加65535
			count = wheelCount + 65535 - _channelWheelCount[index];
		}
		*/
		if ( wheelCount < _channelWheelCount[index] && wheelCount < 5 && _channelWheelCount[index] > 65530 ){//wheelCount是个通过date[5]*256+date[6]来计算的最多到65535（计算机十六位二进制最多到65535），再转一圈会变1所以加65535
			count = wheelCount + 65535 - _channelWheelCount[index];
		}
		else if (wheelCount > _channelWheelCount[index] + 60000 ){//一开始就倒了的情况
			count = wheelCount  - _channelWheelCount[index] - 65535;
		}else{
			count = wheelCount  - _channelWheelCount[index];
		}
		//count = wheelCount  - _channelWheelCount[index];
		_channelWheelCount[index] = wheelCount;
		_channelLen[index] += _precValue * (double)count;
		_radar->setPrecLen ( _channelLen[index] );
	}
	return true;
}




