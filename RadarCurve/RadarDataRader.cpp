#include "StdAfx.h"
#include <vector>
#include <RadarCurve/RadarDataRader.h>
#include <RadarCurve/RadarData.h>
#include <RadarCurve/RadarManager.h>
#include <RadarCurve/MeasureProject.h>
#include <OpenThreads/Thread>

const long MAX_RECV = 1024;

const unsigned char  CHANEL_INDEX_BEGIN = 0X72;
const unsigned char CHANEL_INDEX_END = 0X81;//16ͨ��

class ReceiveNetDataThread : public OpenThreads::Thread
{
public:

	ReceiveNetDataThread( RadarDataRader* lpReader )
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
	RadarDataRader *_lpReader;
};

void ReceiveNetDataThread::run(){
	switch (RadarManager::Instance()->GetRadarWorkType()){
		case RADAR_WORK_TYPE_ONE_USB:
			while( !testCancel() ){
				_lpReader->initU();
				if (false == _lpReader->ReadUData()){
					continue;
				}
				_lpReader->ParseUDataOneUSB();
			}
			break;
		case RADAR_WORK_TYPE_DOUBLE_USB:
			while( !testCancel() ){
				_lpReader->initU();
				if (false == _lpReader->ReadUData()){
					continue;
				}
				_lpReader->ParseUsbTwoChannelData();
			}
			break;
		case RADAR_WORK_TYPE_DOUBLE_USB_OLD:
			while( !testCancel() ){
				_lpReader->initU();
				if (false == _lpReader->ReadUData()){
					continue;
				}
				_lpReader->ParseUsbTwoChannelDataOld();
			}
			break;
		case RADAR_WORK_TYPE_FOUR_USB:
			while( !testCancel() ){
				_lpReader->initU();
				if (false == _lpReader->ReadUData()){
					continue;
				}
				_lpReader->ParseUsbFourChannelData();
			}
			break;
		case RADAR_WORK_TYPE_EIGHT:
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
			break;
		default:
			break;
	}
}


RadarDataRader::RadarDataRader(void)
{
	_recvBuff = new unsigned char[MAX_RECV];
	_lpReadThread = NULL;

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
	
	for (int i =0; i<16;i++){
		_latestChannelRadarData[i] = new unsigned char[_sampleCount*2];
		for ( int j=10;j<_sampleCount*2;j++){
			_latestChannelRadarData[i][j]=0;
		}
	}

}

RadarDataRader::~RadarDataRader(void)
{
	if ( !_lpReadThread )
	{
		delete []_recvBuff;
		return;
	}
	if ( _lpReadThread->isRunning() )
	{
		_lpReadThread->cancel();
	}


	if (m_uPort)
	{
		delete m_uPort;
		m_uPort = NULL;
	}
	delete _lpReadThread;

	delete []_recvBuff;
}


void RadarDataRader::setParam( unsigned char addr, unsigned char value)
{
	unsigned short tmpValue = ( 0X00FFFF & ( addr << 8 ) ) | value;
	unsigned short revnValue = htons(tmpValue);

	_client.Send( (unsigned char *)&revnValue, sizeof( revnValue ) );
}
void RadarDataRader::setInit(bool value)
{
	OpenThreads::ScopedLock<OpenThreads::Mutex> lock(_lock);
	_hadInit = value;
}

void RadarDataRader::init()
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

	setParam(0X00, 0X00);
	_sampleCount = RadarManager::Instance()->getSampCount( atoi ( cs->get("radar", "sample" ).c_str() ) );
	_sampleRatio = RadarManager::Instance()->getSampRatio( atoi( cs->get("radar", "sampleratio").c_str()), 0 );

	_channelCount = atoi( cs->get("radar", "count").c_str() );
	if ( _channelCount == 0 )
	{
		_channelCount = MAX_CHANNEL;
	}

	setParam( 0X01, atoi( cs->get("radar", "sample").c_str()));
	setParam( 0X02, atoi( cs->get("radar", "sampleratio").c_str()));
	setParam( 0X03, atoi( cs->get("radar", "gain").c_str()));
	setParam( 0X04, atoi( cs->get("radar", "gainvalue").c_str()));
	setParam( 0X07, atoi( cs->get("radar", "delay").c_str()));
	setParam( 0X08, atoi( cs->get("radar", "addcount").c_str()));
	setParam( 0X0A, atoi( cs->get("radar", "testtype").c_str()));
	
	m_iModeType = atoi( cs->get("radar", "testtype").c_str());
	int precRatio = atoi( cs->get("radar", "precratio").c_str() );
	float precLen = atof ( cs->get("radar", "preclen").c_str() );
	//int precIndex = atoi( cs->get("radar", "precindex").c_str() );
	

	//Ҷ��ʿ�ṩ�ķ���
	//���͸�������ֵ=���ֵ�������±꣩/���ܳ�/720��
	//ÿ�����������=���͸���������*�ܳ�/720                     ===== ���ֵ
	//float fJihuaAccuracy = RadarManager::Instance()->GetJihuaAccuracy(precIndex);//
	float fJihuaAccuracy = (float)atoi( cs->get("radar", "precision").c_str() );
	int iCmdIndex = RadarManager::Instance()->GetTrueCmdIndexFromPreclenAndAccuracy(precLen,fJihuaAccuracy);
	setParam( 0X0B, iCmdIndex);
	setParam( 0XFA, atoi( cs->get("radar", "id").c_str()));
	setParam(0X00, 0X0F);
	_hadInit = true;
	float fp = RadarManager::Instance()->GetTrueAccuracyFromPreclenAndPrecindex(precLen,fJihuaAccuracy);
	_precValue = fp / 100.0;
	m_iReadNULLTime = 0;
}


void RadarDataRader::initU()
{
	OpenThreads::ScopedLock<OpenThreads::Mutex> lock(_lock);
	if ( _hadInit )
	{
		return;
	}

	if (RadarManager::Instance()->working())
	{
		open(m_iRadarWorkType);
		_dataBuf.clear();
		_hadInit = true;
		m_iReadNULLTime = 0;
	}
}

void RadarDataRader::sendData( unsigned char *buff, int len )
{
	_client.Send( buff, len );
}

bool RadarDataRader::open(int iType)
{
	_hadInit = false;
	m_iRadarWorkType = iType;

	for ( int i = 0 ; i < MAX_CHANNEL; i ++ )
	{
		_channelWheelCount[i] = -1;
		_recordChannelWheelCount[i] = 0;//hjl20210717
		_channelLen[i] = 0;
	}

	if (NULL == m_uPort)
	{
		m_uPort = new GD_UPortClient;
	}
	ConfigureSet *cs = RadarManager::Instance()->getConfigureSet();
	if ( !cs )
	{
		return false;
	}

	m_uPort->StopSample();
	m_uPort->m_iSampleNumIndex = atoi ( cs->get("radar", "sample" ).c_str() ) ;//��������
	m_uPort->m_iSampleRateIndex = atoi( cs->get("radar", "sampleratio").c_str()) ;		//������

	m_uPort->m_iBit = atoi( cs->get("radar", "gain").c_str()) - 1;					//����ģʽ  -1
	m_uPort->m_iLevel = atoi( cs->get("radar", "gainvalue").c_str());					//����ֵ  0
		//0x05                         //������ƽ
		//0x06                         //����λ��
	m_uPort->m_attendigital = atoi( cs->get("radar", "delay" ).c_str() );		    //�ӳٵ���  delay  6
	m_uPort->m_iAddTimeIndex = atoi( cs->get("radar", "addcount" ).c_str() );			//�ۼӴ���		addcount	3	 
	m_uPort->m_attenselindex = 2;			//�ظ�Ƶ��

	m_uPort->m_bMode = atoi( cs->get("radar", "testtype").c_str());					//������ʹ��
	m_iModeType = m_uPort->m_bMode;
	
		//	 0x0A,0x00,			
	m_uPort->m_iMeasureIndex = atoi( cs->get("radar", "precindex").c_str());			//�����־���   precindex   1  //����������õ���

	_sampleCount = RadarManager::Instance()->getSampCount( atoi ( cs->get("radar", "sample" ).c_str() )  );
	//int pSampRatio = atoi( cs->get("radar", "sampleratio").c_str());//old
	_sampleRatio = RadarManager::Instance()->getSampRatio( atoi( cs->get("radar", "sampleratio").c_str()) , 0 );

	_channelCount = atoi( cs->get("radar", "count").c_str() );
	if ( _channelCount == 0 )
	{
		///Ĭ��˫ͨ��
		_channelCount = 2;
	}

	int precRatio = atoi( cs->get("radar", "precratio").c_str() );//������
	float precLen = atof ( cs->get("radar", "preclen").c_str() );//�������ܳ�
	//int precIndex = atoi( cs->get("radar", "precindex").c_str() );//����(���ڱ�ʶ��index)
	//float fJihuaAccuracy = RadarManager::Instance()->GetJihuaAccuracy(precIndex);//�����־���(0,1,2,3,4-->1,2,5,10,20),
	float fJihuaAccuracy = (float)atoi( cs->get("radar", "precision").c_str() );
	m_uPort->m_iMeasure = RadarManager::Instance()->GetTrueCmdIndexFromPreclenAndAccuracy(precLen,fJihuaAccuracy);//
	float fp = RadarManager::Instance()->GetTrueAccuracyFromPreclenAndPrecindex(precLen,fJihuaAccuracy);//���ʵ�ʲ����־���
	_precValue = fp / 100.0;//ת��Ϊ�� �Ǿ���


	//������ʼ���״�
	if (false == m_uPort->Init(m_iRadarWorkType))
	{
		delete m_uPort;
		m_uPort = NULL;
		return false;		
	}
	_hadInit = true;

	m_iReadNULLTime = 0;
	return _hadInit;
}

bool RadarDataRader::openForSetting( std::string const& targetIP, unsigned int port )
{
	for ( int i = 0 ; i < MAX_CHANNEL; i ++ )
	{
		_channelWheelCount[i] = -1;
		_recordChannelWheelCount[i]=0;
		_channelLen[i] = 0;
	}
	_client.setServerIP( targetIP );
	_client.setServerPort( port );
	bool value = _client.Connect();//������������ͨ���Ƿ�ɹ�
	if ( value )
	{
		//init();
		_hadInit = false;
		if ( _lpReadThread )
		{
			_lpReadThread->cancel();
			delete _lpReadThread;
			_lpReadThread = NULL;
		}
		setParam( 0X00, 0X00 );//���Ͳ�����Ӳ��
 		int recvNum = 0;
 		while ( recvNum = _client.Recv( _recvBuff, 512 ) )//20170322 ����2ΪlnWantRecvLen Ӧ����������������ʱ���
 		{
 			Sleep( 10 );//20170322
 		}
		setParam( 0XFA, 0X88 );
		Sleep( 100 );		//
		recvNum = 0;
		while ( recvNum += _client.Recv( _recvBuff, 512 ) )
		//while ( recvNum += _client.Recv( _recvBuff, 256 ) )
		{
			_dataBuf.addData( _recvBuff, recvNum );
			//if ( recvNum >= 512 )//��ί��512
			//{
			//	break;
			//}
			if ( recvNum >= 256 )//20170321  256���ֽ�
			{
				break;
			}
		}
		parseHeadData( (char*)_dataBuf.getData(), _dataBuf.getDataLen() );//����ͷ��Ϣ��ʼ���ɼ������ͨ������֮���
		_dataBuf.clear();
// 		sendData( 0XFA );
// 		sendData( 0X88 );
		//_lpReadThread = new ReceiveNetDataThread( this );
		//_lpReadThread->start();
	}
	return value;
}

bool RadarDataRader::open( std::string const& targetIP, unsigned int port )
{

	for ( int i = 0 ; i < MAX_CHANNEL; i ++ )
	{
		_channelWheelCount[i] = -1;
		_recordChannelWheelCount[i]=0;
		_channelLen[i] = 0;
	}

	_client.setServerIP( targetIP );
	_client.setServerPort( port );
	bool value = _client.Connect();//������������ͨ���Ƿ�ɹ�
	if ( value )
	{
		//init();
		_hadInit = false;
		if ( _lpReadThread )
		{
			_lpReadThread->cancel();
			delete _lpReadThread;
			_lpReadThread = NULL;
		}

		setParam( 0X00, 0X00 );//���Ͳ�����Ӳ��
 		int recvNum = 0;
 		while ( recvNum = _client.Recv( _recvBuff, 512 ) )//20170322 ����2ΪlnWantRecvLen Ӧ����������������ʱ���
 		{
 			Sleep( 10 );//20170322
 		}

		/*//20210623hjl ��ʽ��ʼ��FA 88���޸�ͨ������֮���
		setParam( 0XFA, 0X88 );
		Sleep( 100 );		//
		recvNum = 0;
		while ( recvNum += _client.Recv( _recvBuff, 512 ) )
		//while ( recvNum += _client.Recv( _recvBuff, 256 ) )
		{
			_dataBuf.addData( _recvBuff, recvNum );
			//if ( recvNum >= 512 )//��ί��512
			//{
			//	break;
			//}
			if ( recvNum >= 256 )//20170321  256���ֽ�
			{
				break;
			}
		}

		parseHeadData( (char*)_dataBuf.getData(), _dataBuf.getDataLen() );//����ͷ��Ϣ��ʼ���ɼ������ͨ������֮���*/
		_dataBuf.clear();


		//sendData( 0XFA );
		//sendData( 0X88 );
		//_lpReadThread = new ReceiveNetDataThread( this );
		//_lpReadThread->start();
	}


	return value;
}

void RadarDataRader::ReadThreadStart(bool flag /*= true*/)
{
	if (true == flag )
	{
		if( NULL != _lpReadThread)
		{
			_lpReadThread->cancel();
			delete _lpReadThread;
			_lpReadThread = NULL;
		}
		_lpReadThread = new ReceiveNetDataThread( this );
		_lpReadThread->start();
	}
}

void RadarDataRader::parseHeadData( char *buff, int len )
{
	RadarManager *lpManager = RadarManager::Instance();
	if ( !lpManager )
	{
		return;
	}

	int startPos = 0;
	int endPos = 0;
	for ( int i = 0; i < len - 1; i ++ )//���� SSID=MC8;
	{
		if ( buff[i] == '=' )
		{
			startPos = i + 1;
		}
		if ( buff[i] == ';' )//�ֺ�
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
	for ( int i = 0; i < len - 14; i ++ )//����PORTNAME1=S0200:5.12G;
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
			lpManager->setChannelName( name, RadarManager::Instance()->originalIndexToRecordIndex(index*2) );//16ͨ����һ����ʾ����
			lpManager->setChannelName( name, RadarManager::Instance()->originalIndexToRecordIndex(index*2+1) );//16ͨ����һ����ʾ����
			if (index*2!=0&&index*2!=12){
				lpManager->setChannelName( name, RadarManager::Instance()->originalIndexToRecordIndex(RadarManager::Instance()->getArtificalChannelIndexFromOriginalIndex(index*2)));//16ͨ����һ����ʾ����
			}
			if ((index*2+1)!= 15){
				lpManager->setChannelName( name, RadarManager::Instance()->originalIndexToRecordIndex(RadarManager::Instance()->getArtificalChannelIndexFromOriginalIndex(index*2+1)));//16ͨ����һ����ʾ����
			}*/

			if (index<6){
				lpManager->setChannelName( name, index*2 );//16ͨ����һ����ʾ����
				lpManager->setChannelName( name, index*2+1 );//16ͨ����һ����ʾ����
				lpManager->setChannelName( name, index*2+12 );//28ͨ���ĸ���
				lpManager->setChannelName( name, index*2+13 );//28ͨ���ĸ���
			}else{
				lpManager->setChannelName( name, index*2+12 );//16ͨ����һ����ʾ����
				lpManager->setChannelName( name, index*2+13 );//16ͨ����һ����ʾ����
			}

			std::string strRatio;
			int ratioStartPos = 0;
			int ratioEndPos = 0;
			for ( int ratioStart = i + 15; ratioStart < len; ratioStart  ++)//�ҵ���������Ϣ����ֹ������ֹ��
			{
				if ( buff[ratioStart] == ':' )//ð��
				{
					if ( ratioStartPos <= 0 )
					{
						ratioStartPos = ratioStart;
					}
				}
				if ( buff[ratioStart] == ';')//�ֺ�
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
			////lpManager->setChannelRatioPrect(  index, 2 );
		}
	}
}

bool RadarDataRader::close()
{
	switch (RadarManager::Instance()->GetRadarWorkType())
	{
	case RADAR_WORK_TYPE_ONE_USB:
		if ( _lpReadThread )
		{
			_lpReadThread->cancel();
			delete _lpReadThread;
			_lpReadThread = NULL;
		}
		if (m_uPort)
		{
			delete m_uPort;
			m_uPort = NULL;
		}
		//Sleep( 50 );
		break;
	case RADAR_WORK_TYPE_DOUBLE_USB:
		//Sleep( 50 );
		if ( _lpReadThread )
		{
			_lpReadThread->cancel();
			delete _lpReadThread;
			_lpReadThread = NULL;
		}
		if (m_uPort)
		{
			delete m_uPort;
			m_uPort = NULL;
		}
		//Sleep( 50 );
		break;
	case RADAR_WORK_TYPE_DOUBLE_USB_OLD:
		//Sleep( 50 );
		if ( _lpReadThread )
		{
			_lpReadThread->cancel();
			delete _lpReadThread;
			_lpReadThread = NULL;
		}
		if (m_uPort)
		{
			delete m_uPort;
			m_uPort = NULL;
		}
		//Sleep( 50 );
		break;
	case RADAR_WORK_TYPE_FOUR_USB:
		//Sleep( 50 );
		if ( _lpReadThread )
		{
			_lpReadThread->cancel();
			delete _lpReadThread;
			_lpReadThread = NULL;
		}
		if (m_uPort)
		{
			delete m_uPort;
			m_uPort = NULL;
		}
		//Sleep( 50 );
		break;
	case RADAR_WORK_TYPE_EIGHT:
		if ( !_lpReadThread )
		{
			_client.CloseDevice();

			return true;
		}
		setParam(0X00, 0X00);
		Sleep( 50 );
		if ( _lpReadThread )
		{
			_lpReadThread->cancel();
			delete _lpReadThread;
			_lpReadThread = NULL;
		}
		_client.CloseDevice();
	  break;
	//default:
		//break;
	}
	return true;
}

int RadarDataRader::readData()
{
	long wantToRecv = MAX_RECV;
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




bool RadarDataRader::isOpen()
{
	return _client.m_bConnected;
}

void RadarDataRader::parseData()
{
	int channelData = /*_channelCount * */_sampleCount * sizeof ( short );
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
		//��λ����ͷ��Ϣ
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

		int wheelCount = rd->getPrec();//ԭʼ�����ֵ���
		startPos += channelData;

		int addFlag=1;//���ڸ��ݵ����з������ȷ���費��Ҫ�������������
		
		if ( _channelWheelCount[index] < 0 ){//��һ�ֳ�ʼ��
			_channelWheelCount[index] = wheelCount;//��¼��ǰ����
			_recordChannelWheelCount[index] = 1;//���������0
			_channelLen[index] = 0;//�ܳ��ȹ�0
			rd->setPrecLen( 0.0f );//�״������ܳ�����Ϣ��0
			rd->setRealWheelCount(1);
			//rd->setPrec(0);//�״����ݵ�������Ϊ0
		}else if( _recordChannelWheelCount[index]==1&&((wheelCount-_channelWheelCount[index])>100||(_channelWheelCount[index]-wheelCount)>100)){
			_channelWheelCount[index] = wheelCount;//��¼��ǰ����
			_recordChannelWheelCount[index] = 2;
			_channelLen[index] += _precValue*1;
			rd->setPrecLen( _channelLen[index] );//�״������ܳ�����Ϣ��0
			rd->setRealWheelCount(2);
		}else{
			int count;
			if ( wheelCount < _channelWheelCount[index]&& (_channelWheelCount[index]-wheelCount)>65500){//ת��65535���¹�0
				count = wheelCount + 65535 - _channelWheelCount[index];
			}else{
				count = wheelCount  - _channelWheelCount[index];
			}
			_channelWheelCount[index] = wheelCount;
			_channelLen[index] += _precValue * count;
			_recordChannelWheelCount[index] += count;
			rd->setPrecLen ( _channelLen[index] );
			rd->setRealWheelCount(_recordChannelWheelCount[index]);
			//rd->setPrec(_recordChannelWheelCount[index]);
			if(count==0){
				addFlag=0;
			}
		}
		
		if(addFlag){
			if(index<12){
				RadarManager::Instance()->addRadarDataToProject( rd.get(), index );
				RadarManager::Instance()->addRadarDataToDisplay( rd.get(), index+12 );
				RadarManager::Instance()->addRadarDataToThreeViewDialog( rd.get(), index );
			}else{
				RadarManager::Instance()->addRadarDataToProject( rd.get(), index+12 );
			}
		}
		//if(index!=15){
		//	//��¼���µ��״�������
		//	/*
		//	delete[] _latestChannelRadarData[index];
		//	int lentemp;
		//	unsigned char *lpBuff = rd->getData( lentemp );
		//	unsigned char *temp = new unsigned char[_sampleCount*2];
		//	for ( int i=10;i<_sampleCount*2;i++){
		//		temp[i]=lpBuff[i];
		//	}
		//	_latestChannelRadarData[index]=temp;
		//	*/
		//
		//	//RadarManager::Instance()->addRadarDataToProject( rd.get(), index );

		//	/*
		//	RadarManager::Instance()->addRadarDataToProject( rd.get(), RadarManager::Instance()->originalIndexToRecordIndex(index) );
		//	if(index!=0&&index!=12){
		//		artificializeRadarData( index );
		//	}*/
		//	
		//	RadarManager::Instance()->addRadarDataToProject( rd.get(), index+12 );
		//	if (index<13){
		//		RadarManager::Instance()->addRadarDataToDisplay( rd.get(), index);
		//	}

		//	//_dataBuf.remove( startPos);
		//}
	}
	_dataBuf.remove( startPos );
}

void RadarDataRader::parseDataToDisplay()
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
		//��λ����ͷ��Ϣ
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
		int wheelCount = rd->getPrec();//�����ֵ���
		startPos += channelData;
		/*
		if ( _channelWheelCount[index] < 0 )//��һ�ֳ�ʼ��
		{
			_channelWheelCount[index] = wheelCount;//��¼��ǰ����
			_recordChannelWheelCount[index] = 0;//���������0
			_channelLen[index] = 0;//�ܳ��ȹ�0
			rd->setPrecLen( 0.0f );//�״������ܳ�����Ϣ��0
			rd->setPrec(0);//�״����ݵ�������Ϊ0
		}else{
			int count ;
			if ( wheelCount < _channelWheelCount[index]&& _channelWheelCount[index]>65500){//ת��65535���¹�0
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

/*
bool RadarDataRader::SetChannelLen(RadarData* _radar , int index)
{
	int wheelCount = _radar->getPrec();//��ǰ�״����ݼ�¼�ĵ���
	
	if ( _channelWheelCount[index] < 0 ){
		if ( wheelCount == 0 || wheelCount == 1 ){
			_channelWheelCount[index] = wheelCount;
			_channelLen[index] = 0;
			_radar->setPrecLen( 0.0f );
		}else{
			return false;
		}
	}else{
		int count ;//��ǰ������֮ǰ�����˼��������ڼ�����·��
		if ( wheelCount < _channelWheelCount[index] && wheelCount < 5 && _channelWheelCount[index] > 65530 ){//wheelCount�Ǹ�ͨ��date[5]*256+date[6]���������ൽ65535�������ʮ��λ��������ൽ65535������תһȦ���1���Լ�65535
			count = wheelCount + 65535 - _channelWheelCount[index];
		}
		else if (wheelCount > _channelWheelCount[index] + 60000 ){//һ��ʼ�͵��˵����
			count = wheelCount  - _channelWheelCount[index] - 65535;
		}else{
			count = wheelCount  - _channelWheelCount[index];
		}
		
		_channelWheelCount[index] = wheelCount;
		_channelLen[index] += _precValue * (double)count;
		_radar->setPrecLen ( _channelLen[index] );
	}
	return true;
}*/

//��ȡU�ڵ�����
bool RadarDataRader::ReadUData()
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

//����usb�ӿڵĵ�ͨ�����״�����
void RadarDataRader::ParseUDataOneUSB()
{
	//��������        channelDataΪһ�����ݵĴ�С
	int channelData =   m_uPort->m_iTotalByte   ;//����ʱ  
	int len = _dataBuf.getDataLen();
	if ( len < channelData )
	{
		return;
	}
	unsigned int startPos = 0;
	int AbufLen = channelData ;//�ѵ����ݵĳ���
	int iASample = channelData / 2;//��������

	while ( true )
	{
		unsigned char *lpDataBuff = _dataBuf.getData();
		bool findHead = false;
		if ( len - startPos < channelData )
		{
			break;
		}

		osg::ref_ptr<RadarData> rdA = new RadarData;
		//osg::ref_ptr<RadarData> rdB = new RadarData;

		rdA->SetBufLength(AbufLen);
		//rdB->SetBufLength(AbufLen);

		rdA->setSampleCount( iASample );//��������
		rdA->setSampleRatio( _sampleRatio );//������
		//rdB->setSampleCount(  iASample );//��������
		//rdB->setSampleRatio( _sampleRatio );//������

		rdA->AddDataToEnd(&lpDataBuff[ startPos ],channelData);

		char* p =  (char*)lpDataBuff + startPos;
		++p;

		//if('s' == (*p))
		//{
		//	rdA->AddDataToEnd(&lpDataBuff[ startPos ],channelData);
		//}
		//else if('r' == (*p))
		//{
		//	rdB->AddDataToEnd(&lpDataBuff[ startPos ],channelData);
		//}
		//		
		startPos += channelData;

		if (1 == m_iModeType)
		{

			if('s' == (*p))
			{
				SetChannelLen(rdA.get(),0);
			}
			else if('r' == (*p))
			{
				SetChannelLen(rdA.get(),1);
			}
			SetChannelLen(rdA.get(),0);
			//SetChannelLen(rdB.get(),1);
		}
		else
		{
			static int a = 0;
			static unsigned long startTime = GetTickCount();
			unsigned long endTime = GetTickCount();
			a++;
			if ( endTime != startTime )
			{
				//TRACE("use Time : %f, count = %d, pre second save count : %f\n", ( endTime - startTime ) / 1000.0f, tmpQueue.size(), 1000 / ( (endTime - startTime) * 1.0f / tmpQueue.size()) )  ;
				if (a > 12)
				{
					int as = endTime - startTime;
					if (as < 100)
					{
						Sleep(100-as);
					}
					//TRACE("use Time endTime: %d, startTime = %d,\r\n",endTime,startTime);
					startTime = endTime;
					a = 0;
				}
			}
			_channelLen[0] += 0.01;
			rdA->setPrecLen ( _channelLen[0] );
			//rdB->setPrecLen ( _channelLen[0] );
		}
		
// 		if('s' == (*p))
// 		{
// 			RadarManager::Instance()->addRadarDataToProject( rdA.get(), 0 );
// 		}
// 		else if('r' == (*p))
// 		{
// 			RadarManager::Instance()->addRadarDataToProject( rdA.get(), 1 );
// 			//RadarManager::Instance()->addRadarDataToProject( rdB.get(), 1 );
// 		}

		//1���ж����������ĸ�ͨ��
		std::string HeadSign((char*)lpDataBuff);
		HeadSign = HeadSign.substr(0,4);

		if (HeadSign.compare("TrTr")==0)
		{
			RadarManager::Instance()->addRadarDataToProject( rdA.get(), 0 );
		}
		else if (HeadSign.compare("TsTs")==0)
		{
			RadarManager::Instance()->addRadarDataToProject( rdA.get(), 1 );
		}
//   	else if (HeadSign.compare("TtTt")==0)
//   	{
//   		RadarManager::Instance()->addRadarDataToProject( rdA.get(), 2 );
//   	}
//   	else if (HeadSign.compare("TuTu")==0)
//   	{
//   		RadarManager::Instance()->addRadarDataToProject( rdA.get(), 3 );
//   	}
	}
	_dataBuf.remove( startPos );
}

//����������usb�ӿڵ�˫ͨ�����״�����
void RadarDataRader::ParseUsbTwoChannelDataOld()
{
	////��������     channelDataΪһ�����ݵĴ�С
	int channelData =   m_uPort->m_iTotalByte   ;
	int len = _dataBuf.getDataLen();
	if ( len < channelData )
	{
		return;
	}

	unsigned int startPos = 0;
	int AbufLen = channelData / 2;//�ѵ����ݵĳ���
	int iASample = channelData / 4;//��������
	while ( true )
	{
		unsigned char *lpDataBuff = _dataBuf.getData();
		bool findHead = false;
		if ( len - startPos < channelData )
		{
			break;
		}

		osg::ref_ptr<RadarData> rdA = new RadarData;
		osg::ref_ptr<RadarData> rdB = new RadarData;

		rdA->SetBufLength(AbufLen);
		rdB->SetBufLength(AbufLen);

		//��������
		rdA->setSampleCount(  iASample );
		rdB->setSampleCount(  iASample );

		//������
		rdA->setSampleRatio( _sampleRatio );
		rdB->setSampleRatio( _sampleRatio );

		unsigned char* ctr= (unsigned char*)"TrTr";
		unsigned char* cts= (unsigned char*)"TsTs";
		{
			rdA->AddDataToEnd(ctr,4);
			rdB->AddDataToEnd(cts,4);
			rdA->AddDataToEnd(&lpDataBuff[ startPos],4);
			rdB->AddDataToEnd(&lpDataBuff[ startPos],4);
			long CH_len = iASample - 4;
			for (int i = 0 ; i <  CH_len ;++i)
			{
				//rdA->AddDataToEnd(&lpDataBuff[ startPos + 2*i + 4 ],2);
				//rdB->AddDataToEnd(&lpDataBuff[ startPos + 2*i + 8 + iASample*2],2);
				rdA->AddDataToEnd(&lpDataBuff[ startPos + 4*i + 4 ],2);
				rdB->AddDataToEnd(&lpDataBuff[ startPos + 4*i + 6 ],2);
			}
		}
		startPos += channelData;
		if (1 == m_iModeType)
		{
			SetChannelLen(rdA.get(),0);
			SetChannelLen(rdB.get(),1);
		}
		else
		{
			static int a = 0;
			static unsigned long startTime = GetTickCount();
			unsigned long endTime = GetTickCount();
			a++;
			if ( endTime != startTime )
			{
				//TRACE("use Time : %f, count = %d, pre second save count : %f\n", ( endTime - startTime ) / 1000.0f, tmpQueue.size(), 1000 / ( (endTime - startTime) * 1.0f / tmpQueue.size()) )  ;
				if (a > 12)
				{
					int as = endTime - startTime;
					if (as < 100)
					{
						Sleep(100-as);
					}
					//TRACE("use Time endTime: %d, startTime = %d,\r\n",endTime,startTime);
					startTime = endTime;
					a = 0;
				}
			}
			_channelLen[0] += 0.01;
			rdA->setPrecLen ( _channelLen[0] );
			rdB->setPrecLen ( _channelLen[0] );
		}
		RadarManager::Instance()->addRadarDataToProject( rdA.get(), 0 );
		RadarManager::Instance()->addRadarDataToProject( rdB.get(), 1 );
	}
	_dataBuf.remove( startPos );
	//����Ϊold
}

//����usb�ӿڵ�˫ͨ�����״�����
void RadarDataRader::ParseUsbTwoChannelData()
{
	////��������     channelDataΪһ�����ݵĴ�С
	//int channelData =   m_uPort->m_iTotalByte   ;
	//int len = _dataBuf.getDataLen();
	//if ( len < channelData )
	//{
	//	return;
	//}

	//unsigned int startPos = 0;
	//int AbufLen = channelData / 2;//�ѵ����ݵĳ���
	//int iASample = channelData / 4;//��������
	//while ( true )
	//{
	//	unsigned char *lpDataBuff = _dataBuf.getData();
	//	bool findHead = false;
	//	if ( len - startPos < channelData )
	//	{
	//		break;
	//	}

	//	osg::ref_ptr<RadarData> rdA = new RadarData;
	//	osg::ref_ptr<RadarData> rdB = new RadarData;

	//	rdA->SetBufLength(AbufLen);
	//	rdB->SetBufLength(AbufLen);

	//	//��������
	//	rdA->setSampleCount(  iASample );
	//	rdB->setSampleCount(  iASample );

	//	//������
	//	rdA->setSampleRatio( _sampleRatio );
	//	rdB->setSampleRatio( _sampleRatio );

	//	unsigned char* ctr= (unsigned char*)"TrTr";
	//	unsigned char* cts= (unsigned char*)"TsTs";
	//	{
	//		rdA->AddDataToEnd(ctr,4);
	//		rdB->AddDataToEnd(cts,4);
	//		rdA->AddDataToEnd(&lpDataBuff[ startPos],4);
	//		rdB->AddDataToEnd(&lpDataBuff[ startPos],4);
	//		long CH_len = iASample - 4;
	//		for (int i = 0 ; i <  CH_len ;++i)
	//		{
	//			rdA->AddDataToEnd(&lpDataBuff[ startPos + 2*i + 4 ],2);
	//			rdB->AddDataToEnd(&lpDataBuff[ startPos + 2*i + 8 + iASample*2],2);
	//			//rdA->AddDataToEnd(&lpDataBuff[ startPos + 4*i + 4 ],2);
	//			//rdB->AddDataToEnd(&lpDataBuff[ startPos + 4*i + 6 ],2);
	//		}
	//	}
	//	startPos += channelData;
	//	if (1 == m_iModeType)
	//	{
	//		SetChannelLen(rdA.get(),0);
	//		SetChannelLen(rdB.get(),1);
	//	}
	//	else
	//	{
	//		static int a = 0;
	//		static unsigned long startTime = GetTickCount();
	//		unsigned long endTime = GetTickCount();
	//		a++;
	//		if ( endTime != startTime )
	//		{
	//			//TRACE("use Time : %f, count = %d, pre second save count : %f\n", ( endTime - startTime ) / 1000.0f, tmpQueue.size(), 1000 / ( (endTime - startTime) * 1.0f / tmpQueue.size()) )  ;
	//			if (a > 12)
	//			{
	//				int as = endTime - startTime;
	//				if (as < 100)
	//				{
	//					Sleep(100-as);
	//				}

	//				//TRACE("use Time endTime: %d, startTime = %d,\r\n",endTime,startTime);
	//				startTime = endTime;
	//				a = 0;
	//			}
	//		}
	//		_channelLen[0] += 0.01;
	//		rdA->setPrecLen ( _channelLen[0] );
	//		rdB->setPrecLen ( _channelLen[0] );
	//	}
	//	RadarManager::Instance()->addRadarDataToProject( rdA.get(), 0 );
	//	RadarManager::Instance()->addRadarDataToProject( rdB.get(), 1 );
	//}
	//_dataBuf.remove( startPos );
	//����Ϊold


	//��������        channelDataΪһ�����ݵĴ�С
	int channelData =   m_uPort->m_iTotalByte   ;//����ʱ  
	int len = _dataBuf.getDataLen();
	if ( len < channelData )
	{
		return;
	}
	unsigned int startPos = 0;
	int AbufLen = channelData;//�ѵ����ݵĳ���
	int iASample = channelData / 2;//��������

	int addDataFlag = 1;//���ڴ���ع���flag

	while ( true )
	{
		unsigned char *lpDataBuff = _dataBuf.getData();
		bool findHead = false;
		if ( len - startPos < channelData )
		{
			break;
		}

		osg::ref_ptr<RadarData> rdA = new RadarData;
		//osg::ref_ptr<RadarData> rdB = new RadarData;

		rdA->SetBufLength(AbufLen);
		//rdB->SetBufLength(AbufLen);

		rdA->setSampleCount( iASample );//��������
		rdA->setSampleRatio( _sampleRatio );//������

		rdA->AddDataToEnd(&lpDataBuff[ startPos ],channelData);

		char* p =  (char*)lpDataBuff + startPos;
		++p;

		startPos += channelData;

		bool addDataFlag=true;

		if (1 == m_iModeType)//������ģʽ
		{
			if('s' == (*p))
			{
				if(rdA.get()->getPrec()<_channelWheelCount[0]){
					addDataFlag=false;
				}
				SetChannelLen(rdA.get(),0);
			}
			else if('r' == (*p))
			{
				if(rdA.get()->getPrec()<_channelWheelCount[1]){
					addDataFlag=false;
				}
				SetChannelLen(rdA.get(),1);
			}
			//SetChannelLen(rdA.get(),0);
			//SetChannelLen(rdB.get(),1);
		}
		else//ʱ���ģʽ
		{
			static int a = 0;
			static unsigned long startTime = GetTickCount();
			unsigned long endTime = GetTickCount();
			a++;
			if ( endTime != startTime )
			{
				//TRACE("use Time : %f, count = %d, pre second save count : %f\n", ( endTime - startTime ) / 1000.0f, tmpQueue.size(), 1000 / ( (endTime - startTime) * 1.0f / tmpQueue.size()) )  ;
				if (a > 12)
				{
					int as = endTime - startTime;
					if (as < 100)
					{
						Sleep(100-as);
					}
					//TRACE("use Time endTime: %d, startTime = %d,\r\n",endTime,startTime);
					startTime = endTime;
					a = 0;
				}
			}
			_channelLen[0] += 0.01;
			rdA->setPrecLen ( _channelLen[0] );
			//rdB->setPrecLen ( _channelLen[0] );
		}

		//1���ж����������ĸ�ͨ��
		std::string HeadSign((char*)lpDataBuff);
		HeadSign = HeadSign.substr(0,4);

		if (HeadSign.compare("TrTr")==0&&addDataFlag==true){
			RadarManager::Instance()->addRadarDataToProject( rdA.get(), 0 );
		}else if(HeadSign.compare("TrTr")==0&&addDataFlag==false){
			RadarManager::Instance()->deleteRadarDataFromProject( 0 );
		}else if (HeadSign.compare("TsTs")==0&&addDataFlag==true){
			RadarManager::Instance()->addRadarDataToProject( rdA.get(), 1 );
		}else if (HeadSign.compare("TsTs")==0&&addDataFlag==false){
			RadarManager::Instance()->deleteRadarDataFromProject( 1 );
		}
	}
	_dataBuf.remove( startPos );

	//����
	/*
	//��������        channelDataΪһ�����ݵĴ�С
	int channelData =   m_uPort->m_iTotalByte   ;//����ʱ  
	int len = _dataBuf.getDataLen();
	if ( len < channelData )
	{
		return;
	}
	unsigned int startPos = 0;
	int AbufLen = channelData;//�ѵ����ݵĳ���
	int iASample = channelData / 2;//��������

	int addDataFlag = 1;//��������ع�

	while ( true )
	{
		unsigned char *lpDataBuff = _dataBuf.getData();
		bool findHead = false;
		if ( len - startPos < channelData )
		{
			break;
		}

		osg::ref_ptr<RadarData> rdA = new RadarData;
		//osg::ref_ptr<RadarData> rdB = new RadarData;

		rdA->SetBufLength(AbufLen);
		//rdB->SetBufLength(AbufLen);

		rdA->setSampleCount( iASample );//��������
		rdA->setSampleRatio( _sampleRatio );//������
		//rdB->setSampleCount(  iASample );//��������
		//rdB->setSampleRatio( _sampleRatio );//������

		rdA->AddDataToEnd(&lpDataBuff[ startPos ],channelData);

		char* p =  (char*)lpDataBuff + startPos;
		++p;

		//if('s' == (*p))
		//{
		//	rdA->AddDataToEnd(&lpDataBuff[ startPos ],channelData);
		//}
		//else if('r' == (*p))
		//{
		//	rdB->AddDataToEnd(&lpDataBuff[ startPos ],channelData);
		//}	
		startPos += channelData;

		if (1 == m_iModeType)//������ģʽ
		{

			if('s' == (*p))
			{
				SetChannelLen(rdA.get(),0);
				//SetChannelLenNew(rdA.get(),0,addDataFlag);
			}
			else if('r' == (*p))
			{
				SetChannelLen(rdA.get(),1);
			}
			//SetChannelLen(rdA.get(),0);
			//SetChannelLen(rdB.get(),1);
		}
		else//ʱ���ģʽ
		{
			static int a = 0;
			static unsigned long startTime = GetTickCount();
			unsigned long endTime = GetTickCount();
			a++;
			if ( endTime != startTime )
			{
				//TRACE("use Time : %f, count = %d, pre second save count : %f\n", ( endTime - startTime ) / 1000.0f, tmpQueue.size(), 1000 / ( (endTime - startTime) * 1.0f / tmpQueue.size()) )  ;
				if (a > 12)
				{
					int as = endTime - startTime;
					if (as < 100)
					{
						Sleep(100-as);
					}
					//TRACE("use Time endTime: %d, startTime = %d,\r\n",endTime,startTime);
					startTime = endTime;
					a = 0;
				}
			}
			_channelLen[0] += 0.01;
			rdA->setPrecLen ( _channelLen[0] );
			//rdB->setPrecLen ( _channelLen[0] );
		}

		// 		if('s' == (*p))
		// 		{
		// 			RadarManager::Instance()->addRadarDataToProject( rdA.get(), 0 );
		// 		}
		// 		else if('r' == (*p))
		// 		{
		// 			RadarManager::Instance()->addRadarDataToProject( rdA.get(), 1 );
		// 			//RadarManager::Instance()->addRadarDataToProject( rdB.get(), 1 );
		// 		}

		//1���ж����������ĸ�ͨ��
		std::string HeadSign((char*)lpDataBuff);
		HeadSign = HeadSign.substr(0,4);

		if (HeadSign.compare("TrTr")==0)
		{
			RadarManager::Instance()->addRadarDataToProject( rdA.get(), 0 );
		}
		else if (HeadSign.compare("TsTs")==0)
		{
			RadarManager::Instance()->addRadarDataToProject( rdA.get(), 1 );
		}

		//   	else if (HeadSign.compare("TtTt")==0)
		//   	{
		//   		RadarManager::Instance()->addRadarDataToProject( rdA.get(), 2 );
		//   	}
		//   	else if (HeadSign.compare("TuTu")==0)
		//   	{
		//   		RadarManager::Instance()->addRadarDataToProject( rdA.get(), 3 );
		//   	}

	}
	_dataBuf.remove( startPos )
	*/
}

//����usb�ӿڵ���ͨ�����״�����
void RadarDataRader::ParseUsbFourChannelData(){
	//��������        channelDataΪһ�����ݵĴ�С
	int channelData =   m_uPort->m_iTotalByte   ;//����ʱ  
	int len = _dataBuf.getDataLen();
	if ( len < channelData ){
		return;
	}
	unsigned int startPos = 0;
	int AbufLen = channelData ;//�ѵ����ݵĳ���
	int iASample = channelData / 2;//��������

	while ( true ){
		unsigned char *lpDataBuff = _dataBuf.getData();//�ò����õ���������ͷ����TrTr��ע���Ǹ�ָ��
 		bool findHead = false;
		if ( len - startPos < channelData ){
			break;
		}

		osg::ref_ptr<RadarData> rdA = new RadarData;
		rdA->SetBufLength(AbufLen);
		rdA->setSampleCount( iASample );//��������
		rdA->setSampleRatio( _sampleRatio );//������

		rdA->AddDataToEnd(&lpDataBuff[ startPos ],channelData);

		char* p =  (char*)lpDataBuff + startPos;
		++p;

		//if('s' == (*p))
		//{
		//	rdA->AddDataToEnd(&lpDataBuff[ startPos ],channelData);
		//}
		//else if('r' == (*p))
		//{
		//	rdB->AddDataToEnd(&lpDataBuff[ startPos ],channelData);
		//}
		//		
		startPos += channelData;

		if (1 == m_iModeType){//�����ִ���
			if('s' == (*p)){
				SetChannelLen(rdA.get(),0);
			}
			else if('r' == (*p)){
				SetChannelLen(rdA.get(),1);
			}
			if('t' == (*p)){
				SetChannelLen(rdA.get(),2);
			}
			else if('u' == (*p)){
				SetChannelLen(rdA.get(),3);
			}
		}
		else{//ʱ�䴥��
			static int a = 0;
			static unsigned long startTime = GetTickCount();
			unsigned long endTime = GetTickCount();
			a++;
			if ( endTime != startTime ){
				//TRACE("use Time : %f, count = %d, pre second save count : %f\n", ( endTime - startTime ) / 1000.0f, tmpQueue.size(), 1000 / ( (endTime - startTime) * 1.0f / tmpQueue.size()) )  ;
				if (a > 12){
					int as = endTime - startTime;
					if (as < 100){
						Sleep(100-as);
					}
					//TRACE("use Time endTime: %d, startTime = %d,\r\n",endTime,startTime);
					startTime = endTime;
					a = 0;
				}
			}
			_channelLen[0] += 0.01;
			rdA->setPrecLen ( _channelLen[0] );
			//rdB->setPrecLen ( _channelLen[0] );
		}

		//�ж����������ĸ�ͨ��
		std::string HeadSign((char*)lpDataBuff);
		HeadSign = HeadSign.substr(0,4);

		if (HeadSign.compare("TrTr")==0){
			RadarManager::Instance()->addRadarDataToProject( rdA.get(), 0 );
		}
		else if (HeadSign.compare("TsTs")==0){
			RadarManager::Instance()->addRadarDataToProject( rdA.get(), 1 );
		}
		else if (HeadSign.compare("TtTt")==0){
			RadarManager::Instance()->addRadarDataToProject( rdA.get(), 2 );
		}
		else if (HeadSign.compare("TuTu")==0){
			RadarManager::Instance()->addRadarDataToProject( rdA.get(), 3 );
		}
	}
	_dataBuf.remove( startPos );

	
	/*
	////��������     channelDataΪһ�����ݵĴ�С
	int channelData =   m_uPort->m_iTotalByte   ;
	int len = _dataBuf.getDataLen();
	if ( len < channelData )
	{
		return;
	}

	unsigned int startPos = 0;
	int AbufLen = channelData / 2;//�ѵ����ݵĳ���
	int iASample = channelData / 4;//��������
	while ( true )
	{
		unsigned char *lpDataBuff = _dataBuf.getData();
		bool findHead = false;
		if ( len - startPos < channelData )
		{
			break;
		}

		osg::ref_ptr<RadarData> rdA = new RadarData;
		osg::ref_ptr<RadarData> rdB = new RadarData;

		rdA->SetBufLength(AbufLen);
		rdB->SetBufLength(AbufLen);

		//��������
		rdA->setSampleCount(  iASample );
		rdB->setSampleCount(  iASample );

		//������
		rdA->setSampleRatio( _sampleRatio );
		rdB->setSampleRatio( _sampleRatio );

		unsigned char* ctr= (unsigned char*)"TrTr";
		unsigned char* cts= (unsigned char*)"TsTs";
		{
			rdA->AddDataToEnd(ctr,4);
			rdB->AddDataToEnd(cts,4);
			rdA->AddDataToEnd(&lpDataBuff[ startPos],4);
			rdB->AddDataToEnd(&lpDataBuff[ startPos],4);
			long CH_len = iASample - 4;
			for (int i = 0 ; i <  CH_len ;++i)
			{
				//rdA->AddDataToEnd(&lpDataBuff[ startPos + 2*i + 4 ],2);
				//rdB->AddDataToEnd(&lpDataBuff[ startPos + 2*i + 8 + iASample*2],2);
				rdA->AddDataToEnd(&lpDataBuff[ startPos + 4*i + 4 ],2);
				rdB->AddDataToEnd(&lpDataBuff[ startPos + 4*i + 6 ],2);
			}
		}
		startPos += channelData;
		if (1 == m_iModeType)
		{
			SetChannelLen(rdA.get(),0);
			SetChannelLen(rdB.get(),1);
		}
		else
		{
			static int a = 0;
			static unsigned long startTime = GetTickCount();
			unsigned long endTime = GetTickCount();
			a++;
			if ( endTime != startTime )
			{
				//TRACE("use Time : %f, count = %d, pre second save count : %f\n", ( endTime - startTime ) / 1000.0f, tmpQueue.size(), 1000 / ( (endTime - startTime) * 1.0f / tmpQueue.size()) )  ;
				if (a > 12)
				{
					int as = endTime - startTime;
					if (as < 100)
					{
						Sleep(100-as);
					}

					//TRACE("use Time endTime: %d, startTime = %d,\r\n",endTime,startTime);
					startTime = endTime;
					a = 0;
				}
			}
			_channelLen[0] += 0.01;
			rdA->setPrecLen ( _channelLen[0] );
			rdB->setPrecLen ( _channelLen[0] );
		}
		RadarManager::Instance()->addRadarDataToProject( rdA.get(), 0 );
		RadarManager::Instance()->addRadarDataToProject( rdB.get(), 1 );
	}
	_dataBuf.remove( startPos );
	//����Ϊold*/
}

void RadarDataRader::parseSixteenData()
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
		////��λ����һ����ͷ��Ϣ
		//for (; startPos < len - 4; startPos++){
		//	//"TrTr"
		//	if (lpDataBuff[startPos] == unsigned char(0X54) && lpDataBuff[1 + startPos] == unsigned char(0X72) && lpDataBuff[2 + startPos] == unsigned char(0X54) && lpDataBuff[3 + startPos] == unsigned char(0X72)){
		//		findHead = true;
		//		break;
		//	}
		//}
		//��λ������һ����ͷ��Ϣ
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
			int wheelCount = rd[i]->getPrec();//�����ֵ���
			

			if ( _channelWheelCount[index] < 0 ){//��һ�ֳ�ʼ��
				_channelWheelCount[index] = wheelCount;//��¼��ǰ����
				_recordChannelWheelCount[index] = 0;//���������0
				_channelLen[index] = 0;//�ܳ��ȹ�0
				rd[i]->setPrecLen( 0.0f );//�״������ܳ�����Ϣ��0
				rd[i]->setPrec(0);//�״����ݵ�������Ϊ0
			}else{
				int count ;
				if ( wheelCount < _channelWheelCount[index]&& _channelWheelCount[index]>65500){//ת��65535���¹�0
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
	_dataBuf.remove( startPos );//�����ȥstartPos��Ԫ��
}

bool RadarDataRader::SetChannelLen(RadarData* _radar , int index)
{
	int wheelCount = _radar->getPrec();//��ǰ�״����ݼ�¼�ĵ���
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
		int count ;//��ǰ������֮ǰ�����˼��������ڼ�����·��
		//if ( wheelCount < _channelWheelCount[index] && wheelCount == 0 && _channelWheelCount[index] % 65535 == 0 )//wheelCount�Ǹ�ͨ��date[5]*256+date[6]���������ൽ65535�������ʮ��λ��������ൽ65535������תһȦ���1���Լ�65535
		/*
		if ( wheelCount < _channelWheelCount[index] && wheelCount == 0 && _channelWheelCount[index] % 65535 == 0 &&_channelLen[index] >= 60000*_precValue ){//wheelCount�Ǹ�ͨ��date[5]*256+date[6]���������ൽ65535�������ʮ��λ��������ൽ65535������תһȦ���1���Լ�65535
			count = wheelCount + 65535 - _channelWheelCount[index];
		}
		*/
		if ( wheelCount < _channelWheelCount[index] && wheelCount < 5 && _channelWheelCount[index] > 65530 ){//wheelCount�Ǹ�ͨ��date[5]*256+date[6]���������ൽ65535�������ʮ��λ��������ൽ65535������תһȦ���1���Լ�65535
			count = wheelCount + 65535 - _channelWheelCount[index];
		}
		else if (wheelCount > _channelWheelCount[index] + 60000 ){//һ��ʼ�͵��˵����
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

bool RadarDataRader::SetChannelLenNew( RadarData* _radar , int index , int &flag )
{
	int wheelCount = _radar->getPrec();
	//if (wheelCount > 2)
	//{
	//	wheelCount++;
	//}
	//if (0 > wheelCount)
	//{
	//	wheelCount++;
	//}
	if ( _channelWheelCount[index] < 0 )
	{
		if ( wheelCount == 0 || wheelCount == 1 )
		{
			_channelWheelCount[index] = wheelCount;
			_channelLen[index] = 0;
			_radar->setPrecLen( 0.0f );
		}
		else
		{
			return false;
		}
	}
	else
	{
		int count ;
		if ( wheelCount < _channelWheelCount[index] && wheelCount == 0 && _channelWheelCount[index] % 65535 == 0 )//wheelCount�Ǹ�ͨ��date[5]*256+date[6]���������ൽ65535�������ʮ��λ��������ൽ65535������תһȦ���1���Լ�65535
		{
			count = wheelCount + 65535 - _channelWheelCount[index];
		}
		else
		{
			count = wheelCount  - _channelWheelCount[index];
		}
		_channelWheelCount[index] = wheelCount;
		_channelLen[index] = _channelLen[index] + _precValue * (double)count;
		_radar->setPrecLen ( _channelLen[index] );
		if ( count < 0 )
		{
			flag = 0;
		}
	}
	return true;
}



//��� 
/*
void RadarDataRader::MarkOne()
{
	//unsigned char *lpDataBuff = _dataBuf.getData();

	unsigned char *temp = new unsigned char[_sampleCount*2];
	for ( int i=0;i<_sampleCount*2;i++){
		//temp[i]=0x1000000000000000;
		temp[i]=0;
	}

	for ( int i=0;i<_sampleCount*2;i++){
		//temp[i]=0x1000000000000000;
		if(i%2==0){
			temp[i]=-255;
		}
	}

	for ( int i=0;i<10;i++){
		//temp[i]=0x1000000000000000;
		temp[i]=0;
	}

	unsigned char *lpDataBuff = temp;

	osg::ref_ptr<RadarData> rdA = new RadarData;
	rdA->SetBufLength(_sampleCount*2);
	rdA->setSampleCount( _sampleCount );//��������
	rdA->setSampleRatio( _sampleRatio );//������

	rdA->AddDataToEnd(&lpDataBuff[ 0 ],_sampleCount*2);

	switch (RadarManager::Instance()->GetRadarWorkType())
	{
	case RADAR_WORK_TYPE_ONE_USB:
		RadarManager::Instance()->addRadarDataToProject( rdA.get(), 0 );
		break;
	case RADAR_WORK_TYPE_DOUBLE_USB:
		RadarManager::Instance()->addRadarDataToProject( rdA.get(), 0 );
		RadarManager::Instance()->addRadarDataToProject( rdA.get(), 1 );
		break;
	case RADAR_WORK_TYPE_DOUBLE_USB_OLD:
		RadarManager::Instance()->addRadarDataToProject( rdA.get(), 0 );
		RadarManager::Instance()->addRadarDataToProject( rdA.get(), 1 );
		break;
	case RADAR_WORK_TYPE_FOUR_USB:
		RadarManager::Instance()->addRadarDataToProject( rdA.get(), 0 );
		RadarManager::Instance()->addRadarDataToProject( rdA.get(), 1 );
		RadarManager::Instance()->addRadarDataToProject( rdA.get(), 2 );
		RadarManager::Instance()->addRadarDataToProject( rdA.get(), 3 );
		break;
	case RADAR_WORK_TYPE_EIGHT:
		{
			int tempArr[8]={RadarManager::Instance()->int_Check_Channel1,RadarManager::Instance()->int_Check_Channel2,RadarManager::Instance()->int_Check_Channel3,RadarManager::Instance()->int_Check_Channel4,RadarManager::Instance()->int_Check_Channel5,RadarManager::Instance()->int_Check_Channel6,RadarManager::Instance()->int_Check_Channel7};
			for (int i=0;i<8;i++){
				if (tempArr[i]==1){
					RadarManager::Instance()->addRadarDataToProject( rdA.get(), i );
				}
			}
		}
		break;
	default:
		break;
	}
	delete[] temp;
}
*/

void RadarDataRader::Mark(int numberOne, int numberTwo)
{
	//unsigned char *lpDataBuff = _dataBuf.getData();

	unsigned char *temp = new unsigned char[_sampleCount*2];
	//��ʼ��10������Ϊ����ͷ�����Ϣ �����֡���ѹ֮���
	for ( int i=0;i<10;i++){
		temp[i]=0;
	}
	//ʣ�µ�����Ϊ�״����ݣ�ÿ��ֵ�����������ɣ�value=a+b*256
	for ( int i=10;i<_sampleCount*2;i++){
		if(i%2==0){
			temp[i]=numberOne;
		}else{
			temp[i]=numberTwo;
		}
	}

	unsigned char *lpDataBuff = temp;

	osg::ref_ptr<RadarData> rd = new RadarData;
	rd->SetBufLength(_sampleCount*2);
	rd->setSampleCount( _sampleCount );//��������
	rd->setSampleRatio( _sampleRatio );//������

	rd->AddDataToEnd(&lpDataBuff[ 0 ],_sampleCount*2);

	switch (RadarManager::Instance()->GetRadarWorkType())
	{
	case RADAR_WORK_TYPE_ONE_USB:
		RadarManager::Instance()->addRadarDataToProject( rd.get(), 0 );
		break;
	case RADAR_WORK_TYPE_DOUBLE_USB:
		RadarManager::Instance()->addRadarDataToProject( rd.get(), 0 );
		RadarManager::Instance()->addRadarDataToProject( rd.get(), 1 );
		break;
	case RADAR_WORK_TYPE_DOUBLE_USB_OLD:
		RadarManager::Instance()->addRadarDataToProject( rd.get(), 0 );
		RadarManager::Instance()->addRadarDataToProject( rd.get(), 1 );
		break;
	case RADAR_WORK_TYPE_FOUR_USB:
		RadarManager::Instance()->addRadarDataToProject( rd.get(), 0 );
		RadarManager::Instance()->addRadarDataToProject( rd.get(), 1 );
		RadarManager::Instance()->addRadarDataToProject( rd.get(), 2 );
		RadarManager::Instance()->addRadarDataToProject( rd.get(), 3 );
		break;
	case RADAR_WORK_TYPE_EIGHT:
		{
			/*
			int tempArr[MAX_CHANNEL];
			for (int i=0;i<MAX_CHANNEL;i++){
				tempArr[i]=RadarManager::Instance()->int_Check_Channel[i];
			}*/

			//={RadarManager::Instance()->int_Check_Channel1,RadarManager::Instance()->int_Check_Channel2,RadarManager::Instance()->int_Check_Channel3,RadarManager::Instance()->int_Check_Channel4,RadarManager::Instance()->int_Check_Channel5,RadarManager::Instance()->int_Check_Channel6,RadarManager::Instance()->int_Check_Channel7,RadarManager::Instance()->int_Check_Channel8};
			for (int index=0;index<16;index++){//13-24����1-12���Ƶ� �����ڱ�ǵ�ʱ���ù�13-24
				//if (tempArr[index]==1){
				if (RadarManager::Instance()->int_Check_Channel[index]==1){
					if (index<12){
						RadarManager::Instance()->addRadarDataToDisplay( rd.get(), index);
					}else{
						RadarManager::Instance()->addRadarDataToProject( rd.get(), index+12 );
					}
				}
			}
		}
		break;
	default:
		break;
	}
	delete[] temp;
}

short getFirstValueFromRadarDataValue(int markValue){//��һ�������������� ����-1=255 -2=254
	if(markValue>=0){
		if (markValue<256){
			return markValue;
		}
		return markValue%256;
	}else{
		if (markValue>-256){
			return markValue+256;
		}
		return markValue%(-256)+256;//+256Ϊ������������������������ȷ��ʾ-1=255 -2=254
	}
}

short getSecondValueFromRadarDataValue(int markValue){
	//int i=0;
	if (markValue>=0){
		if (markValue<256){
			return 0;
		}
		return markValue/256;
	}else{
		if (markValue>-256){
			return -1;
		}else if(markValue%(-256)!=0){//ֻҪ�������� ��һ������������Ҫ+256��ʹ��λһ������ ���Եڶ�����-1����Ӧ
			return -markValue/(-256)-1;
		}
		return -markValue/(-256);
	}
}

/*
void RadarDataRader::artificializeRadarData(int originalIndex)
{	
	unsigned char *temp = new unsigned char[_sampleCount*2];
	
	//for ( int i=0;i<10;i++){
		//temp[i]=_latestChannelRadarData[originalIndex-1][i];
	//}

	short channelOneFirstValue,channelOneSecondValue,channelTwoFirstValue,channelTwoSecondValue,processedValue;
	for ( int i=0; i<_sampleCount; i++ ){
		if((short)_latestChannelRadarData[originalIndex-1][i*2]<128){//channelOneFirstValue
			channelOneFirstValue=(short)_latestChannelRadarData[originalIndex-1][i*2];
		}else{
			channelOneFirstValue=(short)_latestChannelRadarData[originalIndex-1][i*2]-256;
		}
		if((short)_latestChannelRadarData[originalIndex-1][i*2+1]<128){//channelOneSecondValue
			channelOneSecondValue=(short)_latestChannelRadarData[originalIndex-1][i*2+1];
		}else{
			channelOneSecondValue=(short)_latestChannelRadarData[originalIndex-1][i*2+1]-256;
		}
		if((short)_latestChannelRadarData[originalIndex][i*2]<128){//channelTwoFirstValue
			channelTwoFirstValue=(short)_latestChannelRadarData[originalIndex][i*2];
		}else{
			channelTwoFirstValue=(short)_latestChannelRadarData[originalIndex][i*2]-256;
		}
		if((short)_latestChannelRadarData[originalIndex][i*2+1]<128){//channelTwoSecondValue
			channelTwoSecondValue=(short)_latestChannelRadarData[originalIndex][i*2+1];
		}else{
			channelTwoSecondValue=(short)_latestChannelRadarData[originalIndex][i*2+1]-256;
		}
	
		processedValue=(channelOneFirstValue+channelOneSecondValue*256+channelTwoFirstValue+channelTwoSecondValue*256)/2;
		if(processedValue>=0){
			if (processedValue<256){
				temp[i*2] = processedValue;
				temp[i*2+1] = 0;
			}
			temp[i*2] = processedValue%256;
			temp[i*2+1] = processedValue/256;
		}else{
			if (processedValue>-256){
				temp[i*2] = processedValue+256;
				temp[i*2+1] = -1;
			}else if(processedValue%(-256)!=0){//ֻҪ�������� ��һ������������Ҫ+256��ʹ��λһ������ ���Եڶ�����-1����Ӧ
				temp[i*2] = processedValue%(-256)+256;//+256Ϊ������������������������ȷ��ʾ-1=255 -2=254
				temp[i*2+1] = -processedValue/(-256)-1;
			}
			temp[i*2] = processedValue%(-256)+256;//+256Ϊ������������������������ȷ��ʾ-1=255 -2=254
			temp[i*2+1] = -processedValue/(-256);
		}
		if(temp[i*2]<0){
			temp[i*2]=temp[i*2]+256;
		}
		if(temp[i*2+1]<0){
			temp[i*2+1]=temp[i*2+1]+256;
		}
	}

	unsigned char *lpDataBuff = (unsigned char*)temp;

	osg::ref_ptr<RadarData> rd = new RadarData;
	rd->SetBufLength(_sampleCount*2);
	rd->setSampleCount( _sampleCount );//��������
	rd->setSampleRatio( _sampleRatio );//������
	rd->AddDataToEnd(&lpDataBuff[ 0 ],_sampleCount*2);
	
	//RadarManager::Instance()->addRadarDataToProject( rd.get(), RadarManager::Instance()->originalIndexToRecordIndex(RadarManager::Instance()->getArtificalChannelIndexFromOriginalIndex(originalIndex)) );
	RadarManager::Instance()->addRadarDataToProject( rd.get(), RadarManager::Instance()->getArtificalChannelIndexFromOriginalIndex(originalIndex));
	delete[] temp;
}*/