#include "StdAfx.h"
#include <RadarCurve/GpsReader.h>
#include <OpenThreads/ScopedLock>
#include <RadarCurve/RadarManager.h>
#include <Object/CmdCenter/CmdCenter.h>
#include <Object/ShowObj/ScreenMark/ScreenMark.h>
#include <Object/ObjManager.h>
#include <osgAPEx/CoordinateTool>

#include <Kernel/KernelExport.h>
//���ַ���str���ַ�splitChar�ָ����vector
std::vector< std::string > SpliteStr( std::string const& str, char splitChar ){
	std::vector< std::string > result;
	std::string tmpStr = str;
	size_t findP;
	while ((findP = tmpStr.find(splitChar)) != std::string::npos){//��ͷһֱ�ҷָ���ֱ���Ҳ���
		std::string subStr = tmpStr.substr(0, findP);//�ָ���ǰ����Ƿݶ���������
		tmpStr = tmpStr.substr(findP + 1, tmpStr.length() - findP - 1);//�ָ���������Ƿݴ����������ظ�����
		result.push_back( subStr );
	}
	if (tmpStr.length() > 0){//�Ҳ����ָ�������������
		result.push_back(tmpStr);
	}
	return result;
}

GpsReader::GpsReader(void){
	_lastTime = time(NULL);
	_first = true;
	_iCondition = 0;
	_len = 0;
}

GpsReader::~GpsReader(void){
}

//���ݸ�������com�ںţ������ʵȣ���������
bool GpsReader::open( int portNum, int baud , char parity , int databits , int stopbits ){
	_seriaPort.SetGpsReader( this );
	if ( !_seriaPort.InitPort( NULL, portNum, baud, parity, databits, stopbits ) ){
		return false;
	}
	_first = true;
	_seriaPort.StartMonitoring();
	_len = 0;
	//_curData.setReceiveData(osg::Vec3d(1,1,1));
	//_lastData.setReceiveData(osg::Vec3d(1,1,1));
	_curData.setReceiveData(osg::Vec3d(0,0,0));
	_lastData.setReceiveData(osg::Vec3d(0,0,0));
	m_nGpsUpdateFailCount = 0;

	ConfigureSet *cfg = RadarManager::Instance()->getConfigureSet();
	m_fGpsFrequency = 1/atoi( cfg->get("com", "frequency").c_str() );

	return true;
}

bool GpsReader::close(){
	_seriaPort.StopMonitoring();
	return true;
}


void GpsReader::onReadData( unsigned char *lpData, int len ){
	if ( len <= 0 ){
		return;	
	}
	_dataBuff.addData( lpData, len );
}

void GpsReader::parseData(){//��ȡ����databuff,�Ȱ�$�֣��ٰ���,���֣�
	if ( !_dataBuff.getData() ){
		return;
	}

	std::string buff;
	for ( unsigned int i = 0; i < _dataBuff.getDataLen(); i ++ ){
		buff += _dataBuff.getData()[i];
	}
    
	std::vector< std::string > dataList = SpliteStr( buff, '$');//��$��Ϊ�������ݣ�GGA��ʽ�Ļ�ÿ�����ݺ���13����Ϣ��������ٶ�GPS 0183Э��
	for ( std::vector< std::string >::iterator it = dataList.begin(); it != dataList.end(); it ++ ){
		std::vector< std::string > gpsInfoStr = SpliteStr(*it, ',');//������ÿ����Ϣ�ֳ������� ������Ϣͷ����һ����ж�����ʲô��ʽ�����ж�Ӧ�Ľ��븳ֵ
		if ( gpsInfoStr.size() == 0 ){
			continue;
		}
		std::string v = gpsInfoStr[0];
		if (  v == std::string("GNGST")){
			parseGNGST( gpsInfoStr );
		}else if ( v == std::string("GPGSV")){
			parseGPGSV( gpsInfoStr );
		}else if ( v == std::string("GBDGSV")){
			parseGBDGSV( gpsInfoStr );
		}else if ( v == std::string("GNGSA")){
			parseGNGSA( gpsInfoStr );
		}else if ( v == std::string("GPRMC")){
			parseGPRMC( gpsInfoStr );
		}else if ( v == std::string("GPGGA")){
			if( gpsInfoStr.size()!=15 ||  gpsInfoStr[14].size()<3  /*|| !strcmp(gpsInfoStr[14].substr(gpsInfoStr[14].length()-2,2).c_str(),"\r\n")*/){//����ɸ�����ϸ������
				continue;
			}
			{
				OpenThreads::ScopedLock<OpenThreads::Mutex> lock(_mutex);
				_currentRawGpsDataBuff=gpsInfoStr;
			}
			parseGPGGA( gpsInfoStr );//��gpgga��ʽ�������ݽ���
			//m_vecStrCurrentGPGGA=gpsInfoStr;
		}else{
			continue;
		}
	}
	_dataBuff.clear();//�Ѷ������Ϣ��buff�����
}

osg::Vec3d GpsReader::getCurPos(){//��ȡ���ֵ��ȡxyz��Ϣ
	OpenThreads::ScopedLock<OpenThreads::Mutex> lock(_mutex);
	DWORD curTick = GetTickCount();

	if ( _lastData.getRecvTime() == 0 )
	{
		return _lastData.getPos();
	}

	if ( _curData.getRecvTime() )
	{
		return _curData.getPos();
	}

	if ( _curData.getRecvTime() == _lastData.getRecvTime() )
	{
		return _curData.getPos();
	}
	/*
	osg::Vec3d curPos = (_curData.getPos() - _lastData.getPos() )
						* (
								( curTick - _lastData.getRecvTime() )
								/ ( ( _curData.getRecvTime() - _lastData.getRecvTime() ) * 1.0f )
						)
						+ _curData.getPos();
						*/
	
	
	osg::Vec3d curPos = (_curData.getPos() - _lastData.getPos() )
						* (
								( curTick - _lastData.getRecvTime() )
								/ ( ( _curData.getRecvTime() - _lastData.getRecvTime() ) * 1.0f )
						)
						+ _lastData.getPos();
	return curPos;
}

//���ڻ�ȡ��ǰ��γ��ֵ��GPGGA��Ϣ��
void GpsReader::getCurrentGpsImformation(osg::Vec3d &postion, std::vector< std::string > &vecStrGPGGA){
	OpenThreads::ScopedLock<OpenThreads::Mutex> lock(_mutex);
	postion=_curData.getPos();
	vecStrGPGGA.assign(m_vecStrCurrentGPGGA.begin(),m_vecStrCurrentGPGGA.end());
}

//���ص�ǰgps����
float GpsReader::getCurLen(){
	OpenThreads::ScopedLock<OpenThreads::Mutex> lock(_mutex);
	return _len;
}

//����rtk״̬
int GpsReader::getCondition(){
	OpenThreads::ScopedLock<OpenThreads::Mutex> lock(_mutex);
	return _iCondition;
}

//���ص�ǰgpgga��Ϣ��
std::vector< std::string > GpsReader::getCurrentGpsDataBuff(){
	OpenThreads::ScopedLock<OpenThreads::Mutex> lock(_mutex);
	return m_vecStrCurrentGPGGA;
}

//����ԭʼ��δ�������GPGGA����Ϣ��
std::vector< std::string > GpsReader::getCurrentRawGpsDataBuff(){
	OpenThreads::ScopedLock<OpenThreads::Mutex> lock(_mutex);
	return _currentRawGpsDataBuff;
}

void GpsReader::parseGNGST( std::vector< std::string > valueList )
{

}
void GpsReader::parseGPGSV( std::vector< std::string > valueList )
{

}
void GpsReader::parseGBDGSV( std::vector< std::string > valueList )
{

}
void GpsReader::parseGNGSA( std::vector< std::string > valueList )
{

}
void GpsReader::parseGPRMC( std::vector< std::string > valueList )
{

}
/*
void GpsReader::parseGPGGA( std::vector< std::string > valueList ){
	
	if ( valueList.size() < 15 ){
		return;
	}

	if(!valueList[2].compare("")||!valueList[4].compare("")){
		return;
	}

	std::string utcTime = "0";
	std::string latStr = "0";
	std::string lonStr = "0";
	std::string latDu = "0";
	std::string latFen = "6";
	std::string lonDu = "0";
	std::string lonFen = "6";
	std::string hightStr = "0";
	std::string strCondition = "0";

	if ( valueList[2] != "" && valueList[4]!= "" ){
		utcTime = valueList[1];//ʱ��
		latStr = valueList[2];// γ�ȣ���ʽΪ�ȷ�ddmm.mmmm(��һλ����Ҳ������)
		lonStr = valueList[4];// ���ȣ���ʽΪ�ȷ�dddmm.mmmm(��һλ����Ҳ������)
		latDu = latStr.substr( 0, 2 );//γ�� 0~90 ��λ
		latFen = latStr.substr( 2, latStr.length() - 2 );
		lonDu = lonStr.substr( 0, 3 );//����0~180 ��λ
		lonFen = lonStr.substr( 3, lonStr.length() - 3 );
		hightStr = valueList[9];
		strCondition = valueList[6];
	}

	_iCondition=atoi(strCondition.c_str());

	{
		//�޸�_lastData��_curData
		OpenThreads::ScopedLock<OpenThreads::Mutex> lock(_mutex);
		_lastData = _curData;
		_curData.setReceiveData( osg::Vec3d( atof( lonDu.c_str() ) + atof( lonFen.c_str()) / 60.0f
			,  atof( latDu.c_str() ) + atof( latFen.c_str()) / 60.0f
			, atof( hightStr.c_str()))
			);
		//_curData.setReceiveData( osg::Vec3d( atof( lonDu.c_str() ) + atof( lonFen.c_str()) / 60.0f
		//	,  atof( latDu.c_str() ) + atof( latFen.c_str()) / 60.0f
		//	, 0)
		//	);
	}

	//�����ܳ�
	if ( _first ){
		_first = false;
	}else{
		osg::Vec3d lastXYZ;
		osgAPEx::ConvertLongLatHeightToXYZ( _lastData.getPos().x(), _lastData.getPos().y(), _lastData.getPos().z(), lastXYZ );

		osg::Vec3d curXYZ;
		osgAPEx::ConvertLongLatHeightToXYZ( _curData.getPos().x(), _curData.getPos().y(), _curData.getPos().z(), curXYZ );

		_len += ( curXYZ - lastXYZ ).length();
	}

	time_t curTime = time(NULL);
	//����Ϊ��ͼ��ʾ����
	if ( curTime - _lastTime > 2 ){		// ���������һ��
		if ((0 >= _lastData.getPos().x() )|| ( 0 >= _lastData.getPos().y())){
			return ;
		}
		
		//if ( !_obj.valid() ){
		//	_obj = EMObj::GetObjManager()->GetObj( GPS_OBJ_ID );
		//}
		////�����dynamic_cast�ǽ�һ���������ָ�루�����ã�ת�����̳���ָ�룬dynamic_cast����ݻ���ָ���Ƿ�����ָ��̳���ָ��������Ӧ����
		//EMObj::CSOScreenMark *lpSM = dynamic_cast<EMObj::CSOScreenMark*>(_obj.get());
		//if ( lpSM ){
		//	lpSM->SetPosition( _lastData.getPos() );
		//}
		////���¶�λ
		//if ( RadarManager::Instance()->getShowGpsPos() ){
		//	EMObj::CmdObjLocate( GPS_OBJ_ID, EMObj::CObj::LocateStyle_Normal );
		//}
		
		//���³���
		if ( RadarManager::Instance()->getCurProject() ){
			RadarManager::Instance()->getCurProject()->updateProjectInfo( _len );
		}
		RadarManager::Instance()->CmdUpdateLine( osg::Vec3d(_lastData.getPos().x(),_lastData.getPos().y(),2) );//����·����ʾ
		//Kernel::DirectTo(_lastData.getPos().x(),  _lastData.getPos().y(), 2000, 0, -90, 0 );
		Kernel::DirectTo(_lastData.getPos().x() - 0.005868,  _lastData.getPos().y() + 0.002337,  2000, 0, -90, 0 );
		//Kernel::DirectTo(_lastData.getPos().x() - 0.00744163,  _lastData.getPos().y() + 0.0036461,  2000, 0, -90, 0 );
		_lastTime = curTime;
	}
}
*/

//����gpgga��Ϣ������ȡ��γ��ֵ
void GpsReader::parseGPGGA( std::vector< std::string > valueList ){
	
	if ( valueList.size() < 15 ){
		return;
	}

	if(!valueList[2].compare("")||!valueList[4].compare("")){
		return;
	}
	if (valueList[2].find(".")!=4){
		return;
	}
	if (valueList[4].find(".")!=5){
		return;
	}
	if(atoi(valueList[6].c_str())<0||atoi(valueList[6].c_str())>6){
		return;
	}


	std::string utcTime = "0";
	std::string latStr = "0";
	std::string lonStr = "0";
	std::string latDu = "0";
	std::string latFen = "6";
	std::string lonDu = "0";
	std::string lonFen = "6";
	std::string hightStr = "0";
	std::string strCondition = "0";

	
	if ( valueList[2] != "" && valueList[4]!= "" ){
		utcTime = valueList[1];//ʱ��
		latStr = valueList[2];// γ�ȣ���ʽΪ�ȷ�ddmm.mmmm(��һλ����Ҳ������)
		lonStr = valueList[4];// ���ȣ���ʽΪ�ȷ�dddmm.mmmm(��һλ����Ҳ������)
		latDu = latStr.substr( 0, 2 );//γ�� 0~90 ��λ
		latFen = latStr.substr( 2, latStr.length() - 2 );
		lonDu = lonStr.substr( 0, 3 );//����0~180 ��λ
		lonFen = lonStr.substr( 3, lonStr.length() - 3 );
		hightStr = valueList[9];
		strCondition = valueList[6];
	}
	_iCondition=atoi(strCondition.c_str());



	GpsData newData;
	newData.setReceiveData( osg::Vec3d( atof( lonDu.c_str() ) + atof( lonFen.c_str()) / 60.0f
		,  atof( latDu.c_str() ) + atof( latFen.c_str()) / 60.0f
		, atof( hightStr.c_str()))
		);//���õ�ǰ��γ��ֵ

	bool gpsDataUpdateFlag=0;
	if( _first ){//�Ƿ��һ�μ�¼��γ��ֵ
		_first = false;
		{
			OpenThreads::ScopedLock<OpenThreads::Mutex> lock(_mutex);
			_curData=newData;
			m_vecStrCurrentGPGGA.assign(valueList.begin(),valueList.end());
			m_nLastCondition=_iCondition;
			m_fCurrentTime=atof(utcTime.c_str());
		}
		gpsDataUpdateFlag=1;
	}else{
		osg::Vec3d curXYZ;
		osgAPEx::ConvertLongLatHeightToXYZ( _curData.getPos().x(), _curData.getPos().y(), _curData.getPos().z(), curXYZ );

		osg::Vec3d supposedXYZ = (_curData.getPos()-_lastData.getPos())*(atof(utcTime.c_str())-m_fLastTime)/(m_fCurrentTime-m_fLastTime)+_lastData.getPos();//�õ�ǰʱ�估֮ǰ����������������ٶ����ƶ����ڵľ�γ��

		osg::Vec3d newXYZ;
		osgAPEx::ConvertLongLatHeightToXYZ( newData.getPos().x(), newData.getPos().y(), newData.getPos().z(), newXYZ );
		
		/*
		if(m_nLastCondition==4){//��һ���ǹ̶������������������������ɸѡ
			if((newXYZ-curXYZ).length()<15){//�¾ɾ�������15m ����ɸ�ɵĵ�
				{
					OpenThreads::ScopedLock<OpenThreads::Mutex> lock(_mutex);
					_lastData = _curData;
					_curData = newData;
					m_vecStrCurrentGPGGA=valueList;
					m_nLastCondition=_iCondition;
				}
				_len += ( newXYZ - curXYZ ).length();
				gpsDataUpdateFlag=1;
			}
		}else{
			{
				OpenThreads::ScopedLock<OpenThreads::Mutex> lock(_mutex);
				_lastData = _curData;
				_curData = newData;
				_currentGpsDataBuff=valueList;
				m_nLastCondition=_iCondition;
			}
			_len += ( newXYZ - curXYZ ).length();
			gpsDataUpdateFlag=1;
		}*/
		float fSupposedDistance=5*(1+m_nGpsUpdateFailCount)*m_fGpsFrequency;//����ٶ�5mÿ��
		if((newXYZ-curXYZ).length()<m_fGpsFrequency||(newXYZ-supposedXYZ).length()<m_fGpsFrequency){//�¾ɾ�������15m��¼�� ����ɸƫ�����ĵ�
			{
				OpenThreads::ScopedLock<OpenThreads::Mutex> lock(_mutex);
				_lastData = _curData;
				_curData = newData;
				m_vecStrCurrentGPGGA.assign(valueList.begin(),valueList.end());//¼��gpgga
				m_nLastCondition=_iCondition;//����gps״̬
				m_fLastTime=m_fCurrentTime;//�����ϴ�ʱ��
				m_fCurrentTime=atof(utcTime.c_str());//���µ�ǰʱ��
			}
			_len += ( newXYZ - curXYZ ).length();
			gpsDataUpdateFlag=1;
		}
	}

	if(gpsDataUpdateFlag){//���³ɹ�
		m_nGpsUpdateFailCount=0;
	}else{
		m_nGpsUpdateFailCount=m_nGpsUpdateFailCount+1;
	}
	
	//¼��ʧ��һ���������ӱض���ԭ����һ����Ч���ݵ�λ�� �ٰ��վ���ɸ��ض���ɸ�� ����Ҫǿ��¼��
	if(m_nGpsUpdateFailCount>7&&(_iCondition==1||_iCondition==2||_iCondition==4||_iCondition==5)){
		osg::Vec3d curXYZ;
		osgAPEx::ConvertLongLatHeightToXYZ( _curData.getPos().x(), _curData.getPos().y(), _curData.getPos().z(), curXYZ );
		osg::Vec3d newXYZ;
		osgAPEx::ConvertLongLatHeightToXYZ( newData.getPos().x(), newData.getPos().y(), newData.getPos().z(), newXYZ );
		{
			OpenThreads::ScopedLock<OpenThreads::Mutex> lock(_mutex);
			_lastData = _curData;
			_curData = newData;
			m_vecStrCurrentGPGGA.assign(valueList.begin(),valueList.end());
			m_nLastCondition=_iCondition;
			m_fLastTime=m_fCurrentTime;
			m_fCurrentTime=atof(utcTime.c_str());
		}
		_len += ( newXYZ - curXYZ ).length();
		m_nGpsUpdateFailCount=0;
	}

	time_t curTime = time(NULL);
	//����Ϊ��ͼ��ʾ����
	if ( curTime - _lastTime > 2 ){		// ���������һ��
		if ((0 >= _lastData.getPos().x() )|| ( 0 >= _lastData.getPos().y())){//��γ��ֵ��Ϊ0
			return ;
		}
		
		//if ( !_obj.valid() ){
		//	_obj = EMObj::GetObjManager()->GetObj( GPS_OBJ_ID );
		//}
		////�����dynamic_cast�ǽ�һ���������ָ�루�����ã�ת�����̳���ָ�룬dynamic_cast����ݻ���ָ���Ƿ�����ָ��̳���ָ��������Ӧ����
		//EMObj::CSOScreenMark *lpSM = dynamic_cast<EMObj::CSOScreenMark*>(_obj.get());
		//if ( lpSM ){
		//	lpSM->SetPosition( _lastData.getPos() );
		//}
		////���¶�λ
		//if ( RadarManager::Instance()->getShowGpsPos() ){
		//	EMObj::CmdObjLocate( GPS_OBJ_ID, EMObj::CObj::LocateStyle_Normal );
		//}
		
		//���³���
		if ( RadarManager::Instance()->getCurProject() ){
			RadarManager::Instance()->getCurProject()->updateProjectInfo( _len );
		}
		RadarManager::Instance()->CmdUpdateLine( osg::Vec3d(_lastData.getPos().x(),_lastData.getPos().y(),2) );//����·����ʾ
		//Kernel::DirectTo(_lastData.getPos().x(),  _lastData.getPos().y(), 2000, 0, -90, 0 );
		Kernel::DirectTo(_lastData.getPos().x() - 0.005868,  _lastData.getPos().y() + 0.002337,  2000, 0, -90, 0 );//����zoom
		//Kernel::DirectTo(_lastData.getPos().x() - 0.00744163,  _lastData.getPos().y() + 0.0036461,  2000, 0, -90, 0 );
		_lastTime = curTime;
	}
}