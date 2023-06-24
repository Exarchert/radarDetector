#include "StdAfx.h"
#include "math.h"
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

double getDistanceFromTwoGpsPoints(double lon1, double lat1, double lon2, double lat2){
	double PI=acos(-1.0);
	return 6371004*sqrt(pow(cos(lon1*PI/180.0)*(lon2*PI/180.0-lat1*PI/180.0),2)+pow((lat2*PI/180.0-lon1*PI/180.0),2));
}

GpsReader::GpsReader(void){
	_lastTime = time(NULL);
	_first = true;
	_second = false;
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
	_second = false;
	_seriaPort.StartMonitoring();
	_len = 0;
	//_curData.setReceiveData(osg::Vec3d(1,1,1));
	//_lastData.setReceiveData(osg::Vec3d(1,1,1));
	_curData.setReceiveData(osg::Vec3d(0,0,0));
	_lastData.setReceiveData(osg::Vec3d(0,0,0));
	m_nGpsUpdateFailCount = 0;

	ConfigureSet *cfg = RadarManager::Instance()->getConfigureSet();
	m_fGpsSpan = 1.0/(float)atoi( cfg->get("com", "frequency").c_str() );

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
		//判断有没有违规字符
		string strTemp=*it;
		bool isValid=true;
		for(int j=0; j<strTemp.length();j++){
			char ch=strTemp[j];
			int ntemp=ch;
			if(ntemp>127||ntemp<0){
				isValid=false;
				break;
			}
		}
		if(!isValid){
			m_nGpsUpdateFailCount=m_nGpsUpdateFailCount+1;
			continue;
		}

		std::vector< std::string > gpsInfoStr = SpliteStr(*it, ',');//按，把每条信息分成若干项 根据信息头（第一项）来判断属于什么格式并进行对应的解译赋值
		if ( gpsInfoStr.size() == 0 ){
			m_nGpsUpdateFailCount=m_nGpsUpdateFailCount+1;
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
				m_nGpsUpdateFailCount=m_nGpsUpdateFailCount+1;
				continue;
			}
			{
				OpenThreads::ScopedLock<OpenThreads::Mutex> lock(_mutex);
				_currentRawGpsDataBuff=gpsInfoStr;
			}
			parseGPGGA( gpsInfoStr );//以gpgga格式进行数据解译
			//m_vecStrCurrentGPGGA=gpsInfoStr;
		}else if ( v == std::string("GNGGA")){
			//if( gpsInfoStr.size()!=15 ||  gpsInfoStr[14].size()<3  /*|| !strcmp(gpsInfoStr[14].substr(gpsInfoStr[14].length()-2,2).c_str(),"\r\n")*/){//用于筛掉不合格的数据
			//	m_nGpsUpdateFailCount=m_nGpsUpdateFailCount+1;
			//	continue;
			//}
			{
				OpenThreads::ScopedLock<OpenThreads::Mutex> lock(_mutex);
				_currentRawGpsDataBuff=gpsInfoStr;
			}
			parseGNGGA( gpsInfoStr );//以gpgga格式进行数据解译
			//m_vecStrCurrentGPGGA=gpsInfoStr;
		}else{
			m_nGpsUpdateFailCount=m_nGpsUpdateFailCount+1;
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


int LongLat2XY(int nType,double longitude,double latitude,double &X,double &Y)
{
    int ProjNo=0; int ZoneWide; //带宽
    double longitude1,latitude1, longitude0,latitude0, X0,Y0, xval,yval;
    double a, f, e2, ee, NN, T, C, A, M, iPI;
    iPI = 0.0174532925199433;  //3.1415926535898/180.0;
    ZoneWide = 3;  //3度带宽
    //ZoneWide = 6; 6度带宽

	if(nType==0)
	{
		a=6378245.0; f=1.0/298.3; //54年北京坐标系参数
	}
	else if(nType==1)
	{
		a=6378140.0; f=1/298.257; //80年西安坐标系参数
	}
	else 
	{
		a = 6378137.0; f = 1.0/298.257223563;//WGS84坐标系参数
	}
    //ProjNo = (int)(longitude / ZoneWide) ;      //6度带
    //longitude0 = ProjNo * ZoneWide + ZoneWide / 2; //6度带
    ProjNo = (int)(longitude / ZoneWide+0.5) ;
    // ProjNo = (int)(longitude / ZoneWide) ; //--带号
    longitude0 = ProjNo * ZoneWide ; //--中央子午线
    longitude0 = longitude0 * iPI ;//--中央子午线转化为弧度
    latitude0=0;
    longitude1 = longitude * iPI ; //经度转换为弧度
    latitude1 = latitude * iPI ; //纬度转换为弧度
    e2=2*f-f*f;
    ee=e2*(1.0-e2);
    NN=a/sqrt(1.0-e2*sin(latitude1)*sin(latitude1));
    T=tan(latitude1)*tan(latitude1);
    C=ee*cos(latitude1)*cos(latitude1);
    A=(longitude1-longitude0)*cos(latitude1);

    M=a*((1-e2/4-3*e2*e2/64-5*e2*e2*e2/256)*latitude1-(3*e2/8+3*e2*e2/32+45*e2*e2*e2/1024)*sin(2*latitude1)
         +(15*e2*e2/256+45*e2*e2*e2/1024)*sin(4*latitude1)-(35*e2*e2*e2/3072)*sin(6*latitude1));
    xval = NN*(A+(1-T+C)*A*A*A/6+(5-18*T+T*T+72*C-58*ee)*A*A*A*A*A/120);
    yval = M+NN*tan(latitude1)*(A*A/2+(5-T+9*C+4*C*C)*A*A*A*A/24
                                +(61-58*T+T*T+600*C-330*ee)*A*A*A*A*A*A/720);
    //X0 = 1000000L*(ProjNo+1)+500000L; //6度带
    X0 = 1000000L*ProjNo+500000L;  //3度带
    Y0 = 0;
    xval = xval+X0; yval = yval+Y0;

    X= xval;
    Y= yval;
    //printf("%lf   %lf\r\n",xval,yval);
    return 1;
}

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

	double dTempX=0.0;
	double dTempY=0.0;

	GpsData newData;
	newData.setReceiveData( osg::Vec3d( atof( lonDu.c_str() ) + atof( lonFen.c_str()) / 60.0f
		,  atof( latDu.c_str() ) + atof( latFen.c_str()) / 60.0f
		, atof( hightStr.c_str()))
		);//设置当前经纬度值

	bool gpsDataUpdateFlag=0;
	if( _first ){//是否第一次记录经纬度值
		_first = false;
		_second = true;
		{
			OpenThreads::ScopedLock<OpenThreads::Mutex> lock(_mutex);
			_curData=newData;
			m_vecStrCurrentGPGGA.assign(valueList.begin(),valueList.end());
			m_nLastCondition=_iCondition;
			m_fCurrentTime=atof(utcTime.c_str());
		}
		gpsDataUpdateFlag=1;
	}else{
		//osg::Vec3d curXYZ;//xyz数据 数值几万的那种
		//osgAPEx::ConvertLongLatHeightToXYZ( _curData.getPos().x(), _curData.getPos().y(), _curData.getPos().z(), curXYZ );
		LongLat2XY(2, _curData.getPos().x(), _curData.getPos().y(), dTempX, dTempY);
		osg::Vec3d curXYZ(dTempX,dTempY,_curData.getPos().z());

		GpsData supposedData;
		osg::Vec3d supposedXYZ;
		if(_second==false){//第3个开始（前面有2个）才能计算
			//newData.setReceiveData( osg::Vec3d( atof( lonDu.c_str() ) + atof( lonFen.c_str()) / 60.0f,  atof( latDu.c_str() ) + atof( latFen.c_str()) / 60.0f, atof( hightStr.c_str())));
			//supposedData.getPos()= (_curData.getPos()-_lastData.getPos())*(atof(utcTime.c_str())-m_fLastTime)/(m_fCurrentTime-m_fLastTime)+_lastData.getPos();
			//用当前时间及之前的两个点的经纬度与时间推算出的速度来推定现在的经纬度
			double dLongitude=(_curData.getPos().x()-_lastData.getPos().x())*(atof(utcTime.c_str())-m_fLastTime)/(m_fCurrentTime-m_fLastTime)+_lastData.getPos().x();//x3=(x2-x1)*(t3-t1)/(t2-t1)+x1
			double dLatitude=(_curData.getPos().y()-_lastData.getPos().y())*(atof(utcTime.c_str())-m_fLastTime)/(m_fCurrentTime-m_fLastTime)+_lastData.getPos().y();
			double dAltitude=(_curData.getPos().z()-_lastData.getPos().z())*(atof(utcTime.c_str())-m_fLastTime)/(m_fCurrentTime-m_fLastTime)+_lastData.getPos().z();
			supposedData.setReceiveData( osg::Vec3d( dLongitude, dLatitude, dAltitude ) );
			//osg::Vec3d supposedXYZ = (_curData.getPos()-_lastData.getPos())*(atof(utcTime.c_str())-m_fLastTime)/(m_fCurrentTime-m_fLastTime)+_lastData.getPos();
			//osgAPEx::ConvertLongLatHeightToXYZ( supposedData.getPos().x(), supposedData.getPos().y(), supposedData.getPos().z(), supposedXYZ );
			LongLat2XY(2, dLongitude, dLatitude, dTempX, dTempY);
			supposedXYZ=osg::Vec3d(dTempX,dTempY,dAltitude);
		}
	
		//osg::Vec3d newXYZ;
		//osgAPEx::ConvertLongLatHeightToXYZ( newData.getPos().x(), newData.getPos().y(), newData.getPos().z(), newXYZ );
		LongLat2XY(2, newData.getPos().x(), newData.getPos().y(), dTempX, dTempY);
		osg::Vec3d newXYZ(dTempX,dTempY,newData.getPos().z());

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
		float fTheoreticalMaxDistance=6*(1+m_nGpsUpdateFailCount)*m_fGpsSpan;//理论上两点的最大距离 最大速度6m每秒 
		//double length1=getDistanceFromTwoGpsPoints(newXYZ.x(),newXYZ.y(),curXYZ.x(),curXYZ.y());
		//double length2=getDistanceFromTwoGpsPoints(newXYZ.x(),newXYZ.y(),supposedXYZ.x(),supposedXYZ.y());
		double dDistanceNewCur=0.0,dDistanceNewSupposed=0.0;
		//length1=sqrt(pow((newXYZ.x()-curXYZ.x()),2)+pow((newXYZ.y()-curXYZ.y()),2));//新到的点与上一次录入的点的距离
		dDistanceNewCur=( newXYZ - curXYZ ).length();//新到的点与上一次录入的点的距离
		if(_second==false){
			//length2=sqrt(pow((newXYZ.x()-supposedXYZ.x()),2)+pow((newXYZ.y()-supposedXYZ.y()),2));//新到的点与拟合点的距离
			dDistanceNewSupposed=fabs(( newXYZ - supposedXYZ ).length());//新到的点与拟合点的距离
		}
		
		bool bNeedInput=false;
		if(_second||dDistanceNewCur<fTheoreticalMaxDistance||fabs(dDistanceNewSupposed-dDistanceNewCur)<1){//新旧距离少于理论最大距离才录入,或者超速情况下，跟预定点距离少于1m   用于筛偏离过大的点
			bNeedInput=true;
		}

		if(bNeedInput){
			{
				OpenThreads::ScopedLock<OpenThreads::Mutex> lock(_mutex);
				_lastData = _curData;
				_curData = newData;
				m_vecStrCurrentGPGGA.assign(valueList.begin(),valueList.end());//录入gpgga
				m_nLastCondition=_iCondition;//更新gps状态
				m_fLastTime=m_fCurrentTime;//更新上次时间
				m_fCurrentTime=atof(utcTime.c_str());//更新当前时间
			}
			_len += dDistanceNewCur;
			gpsDataUpdateFlag=1;
		}

		if(_second){
			_second=false;
		}
	}

	if(gpsDataUpdateFlag){//更新成功
		m_nGpsUpdateFailCount=0;
	}else{
		m_nGpsUpdateFailCount=m_nGpsUpdateFailCount+1;
	}
	
	//录入失败一定次数后车子必定已原离上一次有效数据的位置 再按照距离筛点必定被筛掉 所以要强制录入
	if(m_nGpsUpdateFailCount>7&&(_iCondition==1||_iCondition==2||_iCondition==4||_iCondition==5)){
		//osg::Vec3d curXYZ;
		//osgAPEx::ConvertLongLatHeightToXYZ( _curData.getPos().x(), _curData.getPos().y(), _curData.getPos().z(), curXYZ );
		LongLat2XY(2, _curData.getPos().x(), _curData.getPos().y(), dTempX, dTempY);
		osg::Vec3d curXYZ(dTempX,dTempY,_curData.getPos().z());

		//osg::Vec3d newXYZ;
		//osgAPEx::ConvertLongLatHeightToXYZ( newData.getPos().x(), newData.getPos().y(), newData.getPos().z(), newXYZ );
		LongLat2XY(2, newData.getPos().x(), newData.getPos().y(), dTempX, dTempY);
		osg::Vec3d newXYZ(dTempX,dTempY,newData.getPos().z());

		{
			OpenThreads::ScopedLock<OpenThreads::Mutex> lock(_mutex);
			_lastData = _curData;
			_curData = newData;
			m_vecStrCurrentGPGGA.assign(valueList.begin(),valueList.end());
			m_nLastCondition=_iCondition;
			m_fLastTime=m_fCurrentTime;
			m_fCurrentTime=atof(utcTime.c_str());
		}
		//_len += getDistanceFromTwoGpsPoints(newXYZ.x(),newXYZ.y(),curXYZ.x(),curXYZ.y());
		_len += (newXYZ-curXYZ).length();
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

void GpsReader::parseGNGGA( std::vector< std::string > valueList ){
	parseGPGGA(valueList);

	//if ( valueList.size() < 15 ){
	//	return;
	//}

	//if(!valueList[2].compare("")||!valueList[4].compare("")){
	//	return;
	//}
	//if (valueList[2].find(".")!=4){
	//	return;
	//}
	//if (valueList[4].find(".")!=5){
	//	return;
	//}
	//if(atoi(valueList[6].c_str())<0||atoi(valueList[6].c_str())>6){
	//	return;
	//}


	//std::string utcTime = "0";
	//std::string latStr = "0";
	//std::string lonStr = "0";
	//std::string latDu = "0";
	//std::string latFen = "6";
	//std::string lonDu = "0";
	//std::string lonFen = "6";
	//std::string hightStr = "0";
	//std::string strCondition = "0";

	//
	//if ( valueList[2] != "" && valueList[4]!= "" ){
	//	utcTime = valueList[1];//时间
	//	latStr = valueList[2];// 纬度，格式为度分ddmm.mmmm(第一位是零也将传送)
	//	lonStr = valueList[4];// 经度，格式为度分dddmm.mmmm(第一位是零也将传送)
	//	latDu = latStr.substr( 0, 2 );//纬度 0~90 两位
	//	latFen = latStr.substr( 2, latStr.length() - 2 );
	//	lonDu = lonStr.substr( 0, 3 );//经度0~180 三位
	//	lonFen = lonStr.substr( 3, lonStr.length() - 3 );
	//	hightStr = valueList[9];
	//	strCondition = valueList[6];
	//}
	//_iCondition=atoi(strCondition.c_str());



	//GpsData newData;
	//newData.setReceiveData( osg::Vec3d( atof( lonDu.c_str() ) + atof( lonFen.c_str()) / 60.0f
	//	,  atof( latDu.c_str() ) + atof( latFen.c_str()) / 60.0f
	//	, atof( hightStr.c_str()))
	//	);//设置当前经纬度值

	//bool gpsDataUpdateFlag=0;
	//if( _first ){//是否第一次记录经纬度值
	//	_first = false;
	//	_second = true;
	//	{
	//		OpenThreads::ScopedLock<OpenThreads::Mutex> lock(_mutex);
	//		_curData=newData;
	//		m_vecStrCurrentGPGGA.assign(valueList.begin(),valueList.end());
	//		m_nLastCondition=_iCondition;
	//		m_fCurrentTime=atof(utcTime.c_str());
	//	}
	//	gpsDataUpdateFlag=1;
	//}else{
	//	osg::Vec3d curXYZ;
	//	osgAPEx::ConvertLongLatHeightToXYZ( _curData.getPos().x(), _curData.getPos().y(), _curData.getPos().z(), curXYZ );

	//	GpsData supposedData;
	//	osg::Vec3d supposedXYZ;
	//	if(_second==false){
	//		//newData.setReceiveData( osg::Vec3d( atof( lonDu.c_str() ) + atof( lonFen.c_str()) / 60.0f,  atof( latDu.c_str() ) + atof( latFen.c_str()) / 60.0f, atof( hightStr.c_str())));
	//		//supposedData.getPos()= (_curData.getPos()-_lastData.getPos())*(atof(utcTime.c_str())-m_fLastTime)/(m_fCurrentTime-m_fLastTime)+_lastData.getPos();//用当前时间及之前的两个点推算出的速度来推定现在的经纬度
	//		double dx=(_curData.getPos().x()-_lastData.getPos().x())*(atof(utcTime.c_str())-m_fLastTime)/(m_fCurrentTime-m_fLastTime)+_lastData.getPos().x();
	//		double dy=(_curData.getPos().y()-_lastData.getPos().y())*(atof(utcTime.c_str())-m_fLastTime)/(m_fCurrentTime-m_fLastTime)+_lastData.getPos().y();
	//		double dz=(_curData.getPos().z()-_lastData.getPos().z())*(atof(utcTime.c_str())-m_fLastTime)/(m_fCurrentTime-m_fLastTime)+_lastData.getPos().z();
	//		supposedData.setReceiveData( osg::Vec3d( dx, dy, dz ) );
	//		//osg::Vec3d supposedXYZ = (_curData.getPos()-_lastData.getPos())*(atof(utcTime.c_str())-m_fLastTime)/(m_fCurrentTime-m_fLastTime)+_lastData.getPos();
	//		osgAPEx::ConvertLongLatHeightToXYZ( supposedData.getPos().x(), supposedData.getPos().y(), supposedData.getPos().z(), supposedXYZ );
	//	}
	//	

	//	osg::Vec3d newXYZ;
	//	osgAPEx::ConvertLongLatHeightToXYZ( newData.getPos().x(), newData.getPos().y(), newData.getPos().z(), newXYZ );
	//	
	//	/*
	//	if(m_nLastCondition==4){//上一次是固定解这个距离才有意义才能用来筛选
	//		if((newXYZ-curXYZ).length()<15){//新旧距离少于15m 用于筛飞的点
	//			{
	//				OpenThreads::ScopedLock<OpenThreads::Mutex> lock(_mutex);
	//				_lastData = _curData;
	//				_curData = newData;
	//				m_vecStrCurrentGPGGA=valueList;
	//				m_nLastCondition=_iCondition;
	//			}
	//			_len += ( newXYZ - curXYZ ).length();
	//			gpsDataUpdateFlag=1;
	//		}
	//	}else{
	//		{
	//			OpenThreads::ScopedLock<OpenThreads::Mutex> lock(_mutex);
	//			_lastData = _curData;
	//			_curData = newData;
	//			_currentGpsDataBuff=valueList;
	//			m_nLastCondition=_iCondition;
	//		}
	//		_len += ( newXYZ - curXYZ ).length();
	//		gpsDataUpdateFlag=1;
	//	}*/
	//	float fSupposedDistance=5*(1+m_nGpsUpdateFailCount)*m_fGpsSpan;//最大速度5m每秒 
	//	//double length1=getDistanceFromTwoGpsPoints(newXYZ.x(),newXYZ.y(),curXYZ.x(),curXYZ.y());
	//	//double length2=getDistanceFromTwoGpsPoints(newXYZ.x(),newXYZ.y(),supposedXYZ.x(),supposedXYZ.y());
	//	double length1=0.0,length2=0.0;
	//	length1=sqrt(pow((newXYZ.x()-curXYZ.x()),2)+pow((newXYZ.y()-curXYZ.y()),2));//新到的点与上一次录入的点的距离
	//	if(_second==false){
	//		length2=sqrt(pow((newXYZ.x()-supposedXYZ.x()),2)+pow((newXYZ.y()-supposedXYZ.y()),2));//新到的点与拟合点的距离
	//	}
	//	
	//	bool bNeedInput=false;
	//	if(_second||length1<fSupposedDistance||fabs(length2-length1)<1){//新旧距离少于15m才录入,或者超速情况下，跟预定点距离少于1m   用于筛偏离过大的点
	//		bNeedInput=true;
	//	}

	//	if(bNeedInput){
	//		{
	//			OpenThreads::ScopedLock<OpenThreads::Mutex> lock(_mutex);
	//			_lastData = _curData;
	//			_curData = newData;
	//			m_vecStrCurrentGPGGA.assign(valueList.begin(),valueList.end());//录入gpgga
	//			m_nLastCondition=_iCondition;//更新gps状态
	//			m_fLastTime=m_fCurrentTime;//更新上次时间
	//			m_fCurrentTime=atof(utcTime.c_str());//更新当前时间
	//		}
	//		_len += length1;
	//		gpsDataUpdateFlag=1;
	//	}

	//	if(_second){
	//		_second=false;
	//	}
	//}

	//if(gpsDataUpdateFlag){//更新成功
	//	m_nGpsUpdateFailCount=0;
	//}else{
	//	m_nGpsUpdateFailCount=m_nGpsUpdateFailCount+1;
	//}
	//
	////录入失败一定次数后车子必定已原离上一次有效数据的位置 再按照距离筛点必定被筛掉 所以要强制录入
	//if(m_nGpsUpdateFailCount>7&&(_iCondition==1||_iCondition==2||_iCondition==4||_iCondition==5)){
	//	osg::Vec3d curXYZ;
	//	osgAPEx::ConvertLongLatHeightToXYZ( _curData.getPos().x(), _curData.getPos().y(), _curData.getPos().z(), curXYZ );
	//	osg::Vec3d newXYZ;
	//	osgAPEx::ConvertLongLatHeightToXYZ( newData.getPos().x(), newData.getPos().y(), newData.getPos().z(), newXYZ );
	//	{
	//		OpenThreads::ScopedLock<OpenThreads::Mutex> lock(_mutex);
	//		_lastData = _curData;
	//		_curData = newData;
	//		m_vecStrCurrentGPGGA.assign(valueList.begin(),valueList.end());
	//		m_nLastCondition=_iCondition;
	//		m_fLastTime=m_fCurrentTime;
	//		m_fCurrentTime=atof(utcTime.c_str());
	//	}
	//	//_len += getDistanceFromTwoGpsPoints(newXYZ.x(),newXYZ.y(),curXYZ.x(),curXYZ.y());
	//	_len += (newXYZ-curXYZ).length();
	//	m_nGpsUpdateFailCount=0;
	//}

	//time_t curTime = time(NULL);
	////以下为地图显示更新
	//if ( curTime - _lastTime > 2 ){		// 两秒秒更新一次
	//	if ((0 >= _lastData.getPos().x() )|| ( 0 >= _lastData.getPos().y())){//经纬度值不为0
	//		return ;
	//	}
	//	
	//	//if ( !_obj.valid() ){
	//	//	_obj = EMObj::GetObjManager()->GetObj( GPS_OBJ_ID );
	//	//}
	//	////下面的dynamic_cast是将一个基类对象指针（或引用）转换到继承类指针，dynamic_cast会根据基类指针是否真正指向继承类指针来做相应处理
	//	//EMObj::CSOScreenMark *lpSM = dynamic_cast<EMObj::CSOScreenMark*>(_obj.get());
	//	//if ( lpSM ){
	//	//	lpSM->SetPosition( _lastData.getPos() );
	//	//}
	//	////更新定位
	//	//if ( RadarManager::Instance()->getShowGpsPos() ){
	//	//	EMObj::CmdObjLocate( GPS_OBJ_ID, EMObj::CObj::LocateStyle_Normal );
	//	//}
	//	
	//	//更新长度
	//	if ( RadarManager::Instance()->getCurProject() ){
	//		RadarManager::Instance()->getCurProject()->updateProjectInfo( _len );
	//	}
	//	RadarManager::Instance()->CmdUpdateLine( osg::Vec3d(_lastData.getPos().x(),_lastData.getPos().y(),2) );//更新路径显示
	//	//Kernel::DirectTo(_lastData.getPos().x(),  _lastData.getPos().y(), 2000, 0, -90, 0 );
	//	Kernel::DirectTo(_lastData.getPos().x() - 0.005868,  _lastData.getPos().y() + 0.002337,  2000, 0, -90, 0 );//更新zoom
	//	//Kernel::DirectTo(_lastData.getPos().x() - 0.00744163,  _lastData.getPos().y() + 0.0036461,  2000, 0, -90, 0 );
	//	_lastTime = curTime;
	//}
}