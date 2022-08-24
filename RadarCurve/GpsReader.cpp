#include "StdAfx.h"
#include <RadarCurve/GpsReader.h>
#include <OpenThreads/ScopedLock>
#include <RadarCurve/RadarManager.h>
#include <Object/CmdCenter/CmdCenter.h>
#include <Object/ShowObj/ScreenMark/ScreenMark.h>
#include <Object/ObjManager.h>
#include <osgAPEx/CoordinateTool>

#include <Kernel/KernelExport.h>
//将字符串str按字符splitChar分割，返回vector
std::vector< std::string > SpliteStr( std::string const& str, char splitChar ){
	std::vector< std::string > result;
	std::string tmpStr = str;
	size_t findP;
	while ((findP = tmpStr.find(splitChar)) != std::string::npos){//从头一直找分隔符直至找不到
		std::string subStr = tmpStr.substr(0, findP);//分隔符前面的那份丢入结果数组
		tmpStr = tmpStr.substr(findP + 1, tmpStr.length() - findP - 1);//分隔符后面的那份存起来继续重复操作
		result.push_back( subStr );
	}
	if (tmpStr.length() > 0){//找不到分隔符就完整返回
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

//根据各参数（com口号，波特率等）进行连接
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

void GpsReader::parseData(){//读取整个databuff,先按$分，再按“,”分，
	if ( !_dataBuff.getData() ){
		return;
	}

	std::string buff;
	for ( unsigned int i = 0; i < _dataBuff.getDataLen(); i ++ ){
		buff += _dataBuff.getData()[i];
	}
    
	std::vector< std::string > dataList = SpliteStr( buff, '$');//按$分为多条数据，GGA格式的话每条数据含有13项信息，具体请百度GPS 0183协议
	for ( std::vector< std::string >::iterator it = dataList.begin(); it != dataList.end(); it ++ ){
		std::vector< std::string > gpsInfoStr = SpliteStr(*it, ',');//按，把每条信息分成若干项 根据信息头（第一项）来判断属于什么格式并进行对应的解译赋值
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
			if( gpsInfoStr.size()!=15 ||  gpsInfoStr[14].size()<3  /*|| !strcmp(gpsInfoStr[14].substr(gpsInfoStr[14].length()-2,2).c_str(),"\r\n")*/){//用于筛掉不合格的数据
				continue;
			}
			{
				OpenThreads::ScopedLock<OpenThreads::Mutex> lock(_mutex);
				_currentRawGpsDataBuff=gpsInfoStr;
			}
			parseGPGGA( gpsInfoStr );//以gpgga格式进行数据解译
			//m_vecStrCurrentGPGGA=gpsInfoStr;
		}else{
			continue;
		}
	}
	_dataBuff.clear();//把读完的信息从buff里清掉
}

osg::Vec3d GpsReader::getCurPos(){//获取或插值获取xyz信息
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

//用于获取当前经纬度值和GPGGA信息条
void GpsReader::getCurrentGpsImformation(osg::Vec3d &postion, std::vector< std::string > &vecStrGPGGA){
	OpenThreads::ScopedLock<OpenThreads::Mutex> lock(_mutex);
	postion=_curData.getPos();
	vecStrGPGGA.assign(m_vecStrCurrentGPGGA.begin(),m_vecStrCurrentGPGGA.end());
}

//返回当前gps距离
float GpsReader::getCurLen(){
	OpenThreads::ScopedLock<OpenThreads::Mutex> lock(_mutex);
	return _len;
}

//返回rtk状态
int GpsReader::getCondition(){
	OpenThreads::ScopedLock<OpenThreads::Mutex> lock(_mutex);
	return _iCondition;
}

//返回当前gpgga信息条
std::vector< std::string > GpsReader::getCurrentGpsDataBuff(){
	OpenThreads::ScopedLock<OpenThreads::Mutex> lock(_mutex);
	return m_vecStrCurrentGPGGA;
}

//返回原始的未处理过的GPGGA的信息条
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
		utcTime = valueList[1];//时间
		latStr = valueList[2];// 纬度，格式为度分ddmm.mmmm(第一位是零也将传送)
		lonStr = valueList[4];// 经度，格式为度分dddmm.mmmm(第一位是零也将传送)
		latDu = latStr.substr( 0, 2 );//纬度 0~90 两位
		latFen = latStr.substr( 2, latStr.length() - 2 );
		lonDu = lonStr.substr( 0, 3 );//经度0~180 三位
		lonFen = lonStr.substr( 3, lonStr.length() - 3 );
		hightStr = valueList[9];
		strCondition = valueList[6];
	}

	_iCondition=atoi(strCondition.c_str());

	{
		//修改_lastData、_curData
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

	//计算总长
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
	//以下为地图显示更新
	if ( curTime - _lastTime > 2 ){		// 两秒秒更新一次
		if ((0 >= _lastData.getPos().x() )|| ( 0 >= _lastData.getPos().y())){
			return ;
		}
		
		//if ( !_obj.valid() ){
		//	_obj = EMObj::GetObjManager()->GetObj( GPS_OBJ_ID );
		//}
		////下面的dynamic_cast是将一个基类对象指针（或引用）转换到继承类指针，dynamic_cast会根据基类指针是否真正指向继承类指针来做相应处理
		//EMObj::CSOScreenMark *lpSM = dynamic_cast<EMObj::CSOScreenMark*>(_obj.get());
		//if ( lpSM ){
		//	lpSM->SetPosition( _lastData.getPos() );
		//}
		////更新定位
		//if ( RadarManager::Instance()->getShowGpsPos() ){
		//	EMObj::CmdObjLocate( GPS_OBJ_ID, EMObj::CObj::LocateStyle_Normal );
		//}
		
		//更新长度
		if ( RadarManager::Instance()->getCurProject() ){
			RadarManager::Instance()->getCurProject()->updateProjectInfo( _len );
		}
		RadarManager::Instance()->CmdUpdateLine( osg::Vec3d(_lastData.getPos().x(),_lastData.getPos().y(),2) );//更新路径显示
		//Kernel::DirectTo(_lastData.getPos().x(),  _lastData.getPos().y(), 2000, 0, -90, 0 );
		Kernel::DirectTo(_lastData.getPos().x() - 0.005868,  _lastData.getPos().y() + 0.002337,  2000, 0, -90, 0 );
		//Kernel::DirectTo(_lastData.getPos().x() - 0.00744163,  _lastData.getPos().y() + 0.0036461,  2000, 0, -90, 0 );
		_lastTime = curTime;
	}
}
*/

//解译gpgga信息条，获取经纬度值
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
		utcTime = valueList[1];//时间
		latStr = valueList[2];// 纬度，格式为度分ddmm.mmmm(第一位是零也将传送)
		lonStr = valueList[4];// 经度，格式为度分dddmm.mmmm(第一位是零也将传送)
		latDu = latStr.substr( 0, 2 );//纬度 0~90 两位
		latFen = latStr.substr( 2, latStr.length() - 2 );
		lonDu = lonStr.substr( 0, 3 );//经度0~180 三位
		lonFen = lonStr.substr( 3, lonStr.length() - 3 );
		hightStr = valueList[9];
		strCondition = valueList[6];
	}
	_iCondition=atoi(strCondition.c_str());



	GpsData newData;
	newData.setReceiveData( osg::Vec3d( atof( lonDu.c_str() ) + atof( lonFen.c_str()) / 60.0f
		,  atof( latDu.c_str() ) + atof( latFen.c_str()) / 60.0f
		, atof( hightStr.c_str()))
		);//设置当前经纬度值

	bool gpsDataUpdateFlag=0;
	if( _first ){//是否第一次记录经纬度值
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

		osg::Vec3d supposedXYZ = (_curData.getPos()-_lastData.getPos())*(atof(utcTime.c_str())-m_fLastTime)/(m_fCurrentTime-m_fLastTime)+_lastData.getPos();//用当前时间及之前的两个点推算出的速度来推定现在的经纬度

		osg::Vec3d newXYZ;
		osgAPEx::ConvertLongLatHeightToXYZ( newData.getPos().x(), newData.getPos().y(), newData.getPos().z(), newXYZ );
		
		/*
		if(m_nLastCondition==4){//上一次是固定解这个距离才有意义才能用来筛选
			if((newXYZ-curXYZ).length()<15){//新旧距离少于15m 用于筛飞的点
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
		float fSupposedDistance=5*(1+m_nGpsUpdateFailCount)*m_fGpsFrequency;//最大速度5m每秒
		if((newXYZ-curXYZ).length()<m_fGpsFrequency||(newXYZ-supposedXYZ).length()<m_fGpsFrequency){//新旧距离少于15m才录入 用于筛偏离过大的点
			{
				OpenThreads::ScopedLock<OpenThreads::Mutex> lock(_mutex);
				_lastData = _curData;
				_curData = newData;
				m_vecStrCurrentGPGGA.assign(valueList.begin(),valueList.end());//录入gpgga
				m_nLastCondition=_iCondition;//更新gps状态
				m_fLastTime=m_fCurrentTime;//更新上次时间
				m_fCurrentTime=atof(utcTime.c_str());//更新当前时间
			}
			_len += ( newXYZ - curXYZ ).length();
			gpsDataUpdateFlag=1;
		}
	}

	if(gpsDataUpdateFlag){//更新成功
		m_nGpsUpdateFailCount=0;
	}else{
		m_nGpsUpdateFailCount=m_nGpsUpdateFailCount+1;
	}
	
	//录入失败一定次数后车子必定已原离上一次有效数据的位置 再按照距离筛点必定被筛掉 所以要强制录入
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
	//以下为地图显示更新
	if ( curTime - _lastTime > 2 ){		// 两秒秒更新一次
		if ((0 >= _lastData.getPos().x() )|| ( 0 >= _lastData.getPos().y())){//经纬度值不为0
			return ;
		}
		
		//if ( !_obj.valid() ){
		//	_obj = EMObj::GetObjManager()->GetObj( GPS_OBJ_ID );
		//}
		////下面的dynamic_cast是将一个基类对象指针（或引用）转换到继承类指针，dynamic_cast会根据基类指针是否真正指向继承类指针来做相应处理
		//EMObj::CSOScreenMark *lpSM = dynamic_cast<EMObj::CSOScreenMark*>(_obj.get());
		//if ( lpSM ){
		//	lpSM->SetPosition( _lastData.getPos() );
		//}
		////更新定位
		//if ( RadarManager::Instance()->getShowGpsPos() ){
		//	EMObj::CmdObjLocate( GPS_OBJ_ID, EMObj::CObj::LocateStyle_Normal );
		//}
		
		//更新长度
		if ( RadarManager::Instance()->getCurProject() ){
			RadarManager::Instance()->getCurProject()->updateProjectInfo( _len );
		}
		RadarManager::Instance()->CmdUpdateLine( osg::Vec3d(_lastData.getPos().x(),_lastData.getPos().y(),2) );//更新路径显示
		//Kernel::DirectTo(_lastData.getPos().x(),  _lastData.getPos().y(), 2000, 0, -90, 0 );
		Kernel::DirectTo(_lastData.getPos().x() - 0.005868,  _lastData.getPos().y() + 0.002337,  2000, 0, -90, 0 );//更新zoom
		//Kernel::DirectTo(_lastData.getPos().x() - 0.00744163,  _lastData.getPos().y() + 0.0036461,  2000, 0, -90, 0 );
		_lastTime = curTime;
	}
}