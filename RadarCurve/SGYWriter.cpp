#include "StdAfx.h"
#include "SGYWriter.h"
#include "SGYHeader.h"

#include <RadarCurve/RadarManager.h>
#include "VideoTemplateTab.h"
//C++
#include <sstream>
#include <io.h>
#include <fcntl.h>
#include <iomanip>
#include <string> 
#include <vector>
#include <iostream>
#include <direct.h>//用来创建文件夹
using namespace std;

SGYWriter::SGYWriter(void)
{
	_originalSGYPath="";
	_strTimeTab="";
	_lpFile = NULL;

	_markNo = 1;

	_totalLen = 0;

	_dataCount=0;
	m_dLastRecordedDataCount=1;
	m_dStartDataCount=0;

	m_nUpload=0;

	m_nRoadPart=1;
	m_bIsChange=false;
	m_bIsChangedJustNow=false;

	writeGpsFlag=false;
	writeKmlFlag=false;
}

SGYWriter::~SGYWriter(void)
{
}

bool SGYWriter::openSGY( std::string const& filePath, std::string const& paramXML, int trackNo )
{	
	_originalSGYPath=filePath;
	_lpFile = fopen( filePath.c_str(), "wb" );

	_iTrackNo = trackNo;
	//_lpFile = OpenFile( filePath.c_str(), NULL, OF_CREATE  );

	if ( !_lpFile )
	{
		return false;
	}
	
	//s.open( LPCTSTR( wstr ), std::ios_base::out | std::ios_base::binary );
	//if ( !s.good() )
	//{
		//return false;
	//}

	for( int i = 0; i < 8; i ++ )
	{
		_wheelNum[i] = -1;
	}

	//hjl20210408 更新参数获取
	/*
	_set.fromGroupXML( paramXML );
	int precRatio = atoi( _set.get("radar", "precratio").c_str() );
	float precLen = atoi ( _set.get("radar", "preclen").c_str() );
	int precIndex = atoi( _set.get("radar", "precindex").c_str() );
	int iDielectric = atoi( _set.get("radar", "dielectric").c_str() );
	int iSmpNum = atoi ( _set.get("radar", "sample").c_str());
	int iSampRate = atoi ( _set.get("radar", "sampleratio" ).c_str() );
	*/

	//hjl20210408 更新参数获取
	ConfigureSet *cfg = RadarManager::Instance()->getConfigureSet();
	int precRatio = atoi( cfg->get("radar", "precratio").c_str() );
	float precLen = atoi ( cfg->get("radar", "preclen").c_str() );
	//int precIndex = atoi( cfg->get("radar", "precindex").c_str() );
	int iDielectric = atoi( cfg->get("radar", "dielectric").c_str() );
	int iSmpNum = atoi ( cfg->get("radar", "sample").c_str());
	int iSampRate = atoi ( cfg->get("radar", "sampleratio" ).c_str() );

	_dielectric = RadarManager::Instance()->getDielectric( iDielectric );//介电常数
	_sampRate = RadarManager::Instance()->getSampRatio( iSampRate, trackNo );//采样率
	_samNum = RadarManager::Instance()->getSampCount( iSmpNum );//采样点
	_dTimeWnd = 1.0 / _sampRate * _samNum;//时窗

	//float fJihuaAccuracy = RadarManager::Instance()->GetJihuaAccuracy(precIndex);
	float fJihuaAccuracy = (float)atoi( cfg->get("radar", "precision").c_str() );
	float fp = RadarManager::Instance()->GetTrueAccuracyFromPreclenAndPrecindex(precLen,fJihuaAccuracy);
	_precValue = fp / 100.0;

	std::stringstream ss;
	ss << _iTrackNo;//20211226
	_dCorrection=atoi( cfg->get("correction", ss.str()).c_str() );

	return true;
}

bool SGYWriter::openSGYForAutomation( std::string const& filePath, std::string const& paramXML, int trackNo, int documentPathLength, int namePathLength )
{	
	_originalSGYPath=filePath;
	_lpFile = fopen( filePath.c_str(), "wb" );

	_iTrackNo = trackNo;
	_documentPathLength=documentPathLength;
	_namePathLength=namePathLength;
	//_lpFile = OpenFile( filePath.c_str(), NULL, OF_CREATE  );

	if ( !_lpFile )
	{
		return false;
	}
	
	//s.open( LPCTSTR( wstr ), std::ios_base::out | std::ios_base::binary );
	//if ( !s.good() )
	//{
		//return false;
	//}

	for( int i = 0; i < 8; i ++ )
	{
		_wheelNum[i] = -1;
	}

	//hjl20210408 更新参数获取
	/*
	_set.fromGroupXML( paramXML );
	int precRatio = atoi( _set.get("radar", "precratio").c_str() );
	float precLen = atoi ( _set.get("radar", "preclen").c_str() );
	int precIndex = atoi( _set.get("radar", "precindex").c_str() );
	int iDielectric = atoi( _set.get("radar", "dielectric").c_str() );
	int iSmpNum = atoi ( _set.get("radar", "sample").c_str());
	int iSampRate = atoi ( _set.get("radar", "sampleratio" ).c_str() );
	*/

	//hjl20210408 更新参数获取
	ConfigureSet *cfg = RadarManager::Instance()->getConfigureSet();
	int precRatio = atoi( cfg->get("radar", "precratio").c_str() );
	float precLen = atoi ( cfg->get("radar", "preclen").c_str() );
	//int precIndex = atoi( cfg->get("radar", "precindex").c_str() );
	int iDielectric = atoi( cfg->get("radar", "dielectric").c_str() );
	int iSmpNum = atoi ( cfg->get("radar", "sample").c_str());
	int iSampRate = atoi ( cfg->get("radar", "sampleratio" ).c_str() );

	_dielectric = RadarManager::Instance()->getDielectric( iDielectric );//介电常数
	_sampRate = RadarManager::Instance()->getSampRatio( iSampRate, trackNo );//采样率
	_samNum = RadarManager::Instance()->getSampCount( iSmpNum );//采样点
	_dTimeWnd = 1.0 / _sampRate * _samNum;//时窗

	//float fJihuaAccuracy = RadarManager::Instance()->GetJihuaAccuracy(precIndex);
	float fJihuaAccuracy = (float)atoi( cfg->get("radar", "precision").c_str() );
	float fp = RadarManager::Instance()->GetTrueAccuracyFromPreclenAndPrecindex(precLen,fJihuaAccuracy);
	_precValue = fp / 100.0;

	std::stringstream ss;
	ss << _iTrackNo+12;//13~24刚好对应1~12
	_dCorrection=atoi( cfg->get("correction", ss.str()).c_str() );

	m_nDataCountThreshold = atoi( cfg->get("radar", "threshold").c_str() )/5;
	//m_nThreshold=;
	return true;
}

bool SGYWriter::closeSGY()
{
	fclose( _lpFile );
	_lpFile = 0;
	_originalSGYPath="";
	_strTimeTab="";
	m_nRoadPart=1;
	return true;
}


bool SGYWriter::changeSaveFile()
{
	fclose( _lpFile );
	_lpFile=0;

	m_nRoadPart=m_nRoadPart+1;
	std::stringstream ss;
	ss << m_nRoadPart;

	std::string filePath;
	filePath = _originalSGYPath;
	filePath.replace(_documentPathLength+2*_namePathLength+2, 1, ss.str() );//string.replace(a,b,c)第a位开始的b个字符替换成c

	if(m_nRoadPart<10){
		if (0 != access(filePath.substr(0,_documentPathLength+2*_namePathLength+3).c_str(), 0)){
			mkdir(filePath.substr(0,_documentPathLength+2*_namePathLength+3).c_str());   // 返回 0 表示创建成功，-1 表示失败
		}
	}else if(m_nRoadPart<100){
		if (0 != access(filePath.substr(0,_documentPathLength+2*_namePathLength+4).c_str(), 0)){
			mkdir(filePath.substr(0,_documentPathLength+2*_namePathLength+4).c_str());   // 返回 0 表示创建成功，-1 表示失败
		}
	}else{
		if (0 != access(filePath.substr(0,_documentPathLength+2*_namePathLength+5).c_str(), 0)){
			mkdir(filePath.substr(0,_documentPathLength+2*_namePathLength+5).c_str());   // 返回 0 表示创建成功，-1 表示失败
		}
	}

	if(_iTrackNo<12){
		if(m_nRoadPart<10){
			filePath.replace(_documentPathLength+3*_namePathLength+5, 1, ss.str() );
		}else if(m_nRoadPart<100){
			filePath.replace(_documentPathLength+3*_namePathLength+6, 1, ss.str() );
		}else{
			filePath.replace(_documentPathLength+3*_namePathLength+7, 1, ss.str() );
		}
	}else{
		if(m_nRoadPart<10){
			filePath.replace(_documentPathLength+3*_namePathLength+18, 1, ss.str() );
			if (0 != access(filePath.substr(0,_documentPathLength+2*_namePathLength+16).c_str(), 0)){
				mkdir(filePath.substr(0,_documentPathLength+2*_namePathLength+16).c_str());   // 返回 0 表示创建成功，-1 表示失败
			}
		}else if(m_nRoadPart<100){
			filePath.replace(_documentPathLength+3*_namePathLength+19, 1, ss.str() );
			if (0 != access(filePath.substr(0,_documentPathLength+2*_namePathLength+17).c_str(), 0)){
				mkdir(filePath.substr(0,_documentPathLength+2*_namePathLength+17).c_str());   // 返回 0 表示创建成功，-1 表示失败
			}
		}else{
			filePath.replace(_documentPathLength+3*_namePathLength+20, 1, ss.str() );
			if (0 != access(filePath.substr(0,_documentPathLength+2*_namePathLength+18).c_str(), 0)){
				mkdir(filePath.substr(0,_documentPathLength+2*_namePathLength+18).c_str());   // 返回 0 表示创建成功，-1 表示失败
			}
		}
	}
	_lpFile = fopen( filePath.c_str(), "wb" );

	if(writeGpsFlag){
		fclose( _lpGPSRawFile );
		_lpGPSRawFile = 0;
		fclose( _lpGPSCorFile );
		_lpGPSCorFile = 0;
		fclose( _lpGPSCorFileForUpload );
		_lpGPSCorFileForUpload = 0;
		fclose( _lpGPSCsvFile );
		_lpGPSCsvFile = 0;

		std::string rawFilePath;
		std::string corFilePath;
		std::string corFileForUploadPath;
		std::string csvFilePath;
		rawFilePath = filePath.substr(0,filePath.length() - 6);
		corFilePath = filePath.substr(0,filePath.length() - 6);
		corFileForUploadPath = filePath.substr(0,filePath.length() - 6);
		csvFilePath = filePath.substr(0,filePath.length() - 6);
		rawFilePath+=".raw";
		corFilePath+="gps.cor";
		corFileForUploadPath+="gpsForUpload.cor";
		csvFilePath+=".csv";

		_lpGPSRawFile = fopen( rawFilePath.c_str(), "wt" );
		_lpGPSCorFile = fopen( corFilePath.c_str(), "wt" );
		_lpGPSCorFileForUpload = fopen( corFileForUploadPath.c_str(), "wt" );
		_lpGPSCsvFile = fopen( csvFilePath.c_str(), "wt" );
	
	}

	return true;
}

bool SGYWriter::openGPS( std::string const& corFilePath, std::string const& corForUploadFilePath, std::string const& csvFilePath ){
	_indexOfRecordedGpsData = 1;
	_dLastDistanceGps=0.0;
	m_fLastRecordedGpsDistance=0.0;
	writeGpsFlag=true;

	string rawFilePath=corFilePath.substr(0,corFilePath.length() - 3)+"raw";
	_lpGPSRawFile = fopen( rawFilePath.c_str(), "wt" );

	_lpGPSCorFile = fopen( corFilePath.c_str(), "wt" );
	if(m_nUpload==1){
		_lpGPSCorFileForUpload = fopen( corForUploadFilePath.c_str(), "wt" );
	}
	_lpGPSCsvFile = fopen( csvFilePath.c_str(), "wt" );
	if ( !_lpGPSCorFile || !_lpGPSCsvFile ){
		return false;
	}
	if(m_nUpload==1){
		if ( !_lpGPSCorFileForUpload ){
			return false;
		}
	}
	fprintf(_lpGPSCsvFile, "道数,gps距离,测量轮距离,x,y,z,状态\n");
	return true;
}

bool SGYWriter::closeGPS()
{
	_indexOfRecordedGpsData = 1;
	_dLastDistanceGps=0.0;
	m_fLastRecordedGpsDistance=0.0;
	writeGpsFlag=false;
	fclose( _lpGPSCorFile );
	_lpGPSCorFile = 0;
	fclose( _lpGPSRawFile );
	_lpGPSRawFile = 0;
	if(m_nUpload==1){
		fclose( _lpGPSCorFileForUpload );
		_lpGPSCorFileForUpload = 0;
	}
	fclose( _lpGPSCsvFile );
	_lpGPSCsvFile = 0;
	//oldDis=0.0;
	return true;
}

bool SGYWriter::openKml( std::string const& filePath )
{
	_dLastDistanceKml=0.0;
	writeKmlFlag=true;
	_lpKmlFile = fopen( filePath.c_str(), "wt" );
	if ( !_lpKmlFile ){
		return false;
	}
	return true;
}

bool SGYWriter::closeKml()
{
	_dLastDistanceKml=0.0;
	writeKmlFlag=false;
	fclose( _lpKmlFile );
	_lpKmlFile = 0;
	//oldDis=0.0;
	return true;
}


void SGYWriter::writeHead( int trackNum,std::string _projectNameTime )
{

	//int num = sizeof(SegyReelHdrStruct);

	//CString str;
	//str.Format(L"%d",num);
	//MessageBox(NULL,str,NULL,1);

	SegyReelHdrStruct sSegyReelHdr;
	int iWorkMode = atoi ( _set.get("radar", "testtype").c_str());
	//if (m_sSendPara.btWheel == 1)
	//{
	//	iWorkMode = 2;
	//}
/*
	<group name="radar">
		<item name="addcount" value="0" />
		<item name="count" value="0" />
		<item name="delay" value="0" />
		<item name="gain" value="0" />
		<item name="gainvalue" value="0" />
		<item name="id" value="136" />
		<item name="precindex" value="0" />
		<item name="preclen" value="100" />
		<item name="precratio" value="720" />
		<item name="sample" value="0" />
		<item name="sampleratio" value="0" />
		<item name="testtype" value="0" />
	</group>
*/


	sprintf(sSegyReelHdr.comment, "This SEG-Y format data file was generated from IECAS-GPR system.Institute of Electronics,Chinese Academy of Sciences.(www.ie.cas.cn)\n"
		"DeviceModel:%s\n" 
		"Dielectric:%.3f\n" 

		"WorkMode:%d\n"		
		"TrigMode:%d\n"		
		"ScanSpeed:%.3f\n" 
		"WheelPrecise:%.3f\n" 
		"SampleNum:%d\n"   
		"SampleRate:%.3f\n" 
		"TimeWnd:%.3f\n"    
		"GainMode:%d\n"     
		"GainVal:%d\n"      
		"AddTimes:%d\n"     
		"TimeDelay:%d\n"   

		"WheelPerimeter:%.3f\n" 
		"WheelPulseNum:%d\n"    

		"RepeatFreq:%.3f\n"   
		"SampleBit:%d\n"       
		"TrigLevel:%d\n"      

		"DCValue:%.3f\n"      
		"MarkNo:%d\n",        

		RadarManager::Instance()->getDeviceCode( _iTrackNo ).c_str(), //strDeviceModel,
		_dielectric,//m_dDielectric,
		atoi( _set.get("radar","testtype").c_str()) ? 2:0,//iWorkMode,
		0,//m_sSendPara.btTrigMode,
		200 * 1000 /_samNum / pow(2.0, atof(_set.get("radar", "addcount").c_str())),//m_dRealTrcs,
		_precValue,//m_dWheelPrecise,
		_samNum,//m_iSmpNum,
		_sampRate,//m_dSmpRate /pow(10.0,9),
		_dTimeWnd,//m_dTimeWnd,
		atoi(_set.get("radar","gain").c_str()),//m_sSendPara.btGainMode,//增益模式
		atoi(_set.get("radar", "gainvalue").c_str()),//m_sSendPara.btGainVal,//增益值
		atoi(_set.get("radar", "addcount").c_str()),//m_sSendPara.btAddTimes,
		atoi(_set.get("radar", "delay").c_str()),//m_sSendPara.btTimeDelay,//延迟

		atof(_set.get("radar", "preclen").c_str()),//m_dPerimeter,
		atoi(_set.get("radar", "precratio").c_str()),//m_iPulseNum,

		0, //m_dRepeatFreq,
		0, //m_iSampleBit,
		0, //m_sSendPara.btTrigLevel,

		0, //m_dDCValue[iPortNo],
		0 //m_iMarkNo);

		);

	sSegyReelHdr.ntrpr = (short)trackNum;
	sSegyReelHdr.hdt = (short)(1000.0 *_dTimeWnd/_samNum);
	
	sSegyReelHdr.hns = _samNum;
	sSegyReelHdr.nso = _samNum;
	sSegyReelHdr.format = 3;
	sSegyReelHdr.dDCValue = 0.0;//m_dDCValue[iPortNo];

	sSegyReelHdr.fSampRate = _sampRate;
	sSegyReelHdr.iRealSampNum = _samNum - 8;



	//

	

	time_t _t = atoi(_projectNameTime.c_str());
	struct tm *lt = localtime( &_t);

	CStringA cstrTemp;
	int iyear,imouth,iday,ihour,imin,isec,ird;
	iyear = lt->tm_year + 1900  ;
	char*pT = &sSegyReelHdr.chProjectTime[0];

	imouth = lt->tm_mon + 1;

	iday = lt->tm_mday;

	ihour = lt->tm_hour;

	imin = lt->tm_min;

	isec = lt->tm_sec;

	cstrTemp.Format("%04d年%02d月%02d日%02d时%02d分%02d秒",iyear,imouth,iday,ihour,imin,isec);

	int i =0;

	char* cTem = cstrTemp.GetBuffer();
	for (;i < cstrTemp.GetLength();++i)
	{
		sSegyReelHdr.chProjectTime[i] = *cTem++;
	}
	sSegyReelHdr.chProjectTime[i] = '\0';

	fwrite( (char*)&sSegyReelHdr, sizeof( sSegyReelHdr ), 1, _lpFile );
	//s.write( (char*)&sSegyReelHdr, sizeof( sSegyReelHdr ) );
}

double SGYWriter::getTotalLen( int iTracNo, int wheelCount )
{
	if ( iTracNo > 8 )
	{
		return 0.0;
	}

	if ( iTracNo < 0 )
	{
		return 0.0f;
	}

	if ( _wheelNum[iTracNo] == -1 )
	{
		_wheelNum[iTracNo] = wheelCount;

		return 0.f;
	}

	int count;
	if ( wheelCount < _wheelNum[iTracNo] && wheelCount == 0 )
	{
		count = wheelCount + 65535 - _wheelNum[iTracNo];
	}
	else
	{
		count = wheelCount - _wheelNum[iTracNo];
	}
	_wheelNum[iTracNo] = wheelCount;
	double len = count * _precValue;

	_totalLen += len;

	return _totalLen;
}

void SGYWriter::writeRadarData( RadarData *lpData )
{
	if(m_nUpload==1){
		if(_dataCount>(m_nDataCountThreshold-1)&&m_bIsChange){
			changeSaveFile();
			m_bIsChange=false;
			//_dataCount=0;
			m_bIsChangedJustNow=true;
		}
	}

	int len = 0;
	unsigned short *lpBuff = ( unsigned short *)(lpData->getData( len ));
	if ( !lpBuff ){
		return;
	}

	/*
	SegyTraceHdrStruct SegyTrcHdr;
	SegyTrcHdr.iDataFlag = 0x1010;

	SegyTrcHdr.tracl = lpData->getPrec();//道数
	//SegyTrcHdr.offset = (long)(getTotalLen( lpData->getChannelIndex(), lpData->getPrec() ) *1000.0);
	SegyTrcHdr.offset = (long)(lpData->getPrecLen()*1000.0);
	SegyTrcHdr.ns = _samNum;

	{
		SegyTrcHdr.dt = (USHORT)(1000.0 *_dTimeWnd /_samNum);
	}

	osg::Vec3d pos = lpData->getPosition();
	SegyTrcHdr.dPosX = pos.x();
	SegyTrcHdr.dPosY = pos.y();
	SegyTrcHdr.dPosZ = pos.z();

	time_t _t = lpData->getTime();
	struct tm *lt = localtime( &_t);
	SegyTrcHdr.ucSystemYear = lt->tm_year + 1900 - 2000;
	SegyTrcHdr.ucSystemMonth = lt->tm_mon + 1;
	SegyTrcHdr.ucSystemDay = lt->tm_mday;
	SegyTrcHdr.ucSystemHour = lt->tm_hour;
	SegyTrcHdr.ucSystemMinute = lt->tm_min;
	SegyTrcHdr.ucSystemSecond = lt->tm_sec;
	SegyTrcHdr.shSystemMilSecond = 0;
	SegyTrcHdr.timbas = 1;

	memcpy(SegyTrcHdr.ucVoltage, lpBuff+2, 2);
	memcpy(SegyTrcHdr.ucTrcCount, lpBuff+3, 2);

	if (lpData->getMark())
	{
		SegyTrcHdr.marks_indicator = 0xAA;
		SegyTrcHdr.mark_number = _markNo;
		_markNo ++;
	}
	*/
	// 钢筋仪
	//SegyTrcHdr.usMetalDiameter = lpBuff[9];
	//SegyTrcHdr.usMetalDepth = lpBuff[11];
	//SegyTrcHdr.usMetalFlag = lpBuff[15];


	//fwrite( (char*)&SegyTrcHdr, sizeof( SegyTrcHdr ), 1, _lpFile);//fwrite(指向要被写入的元素数组的指针, 要被写入的每个元素的大小, 元素的个数, 指向 FILE 对象的指针)//20210618hjl

	//fwrite( (char*)lpBuff, len, 1, _lpFile);	
	/*
	if(m_nUpload==0){
		fwrite(((char*)lpBuff + 8 + _dCorrection*2), len - 8 -_dCorrection*2, 1, _lpFile);//8是头8位道头信息 不存进裸数据  _correction*2 一个点两个字节
		//char *temp = new char[8+_dCorrection*2];
		//for(int i=0;i<8+_dCorrection*2;i++){
		//	temp[i] = (char)lpBuff[_samNum*2-1];
		//}
		//fwrite( temp, 8+_dCorrection*2, 1, _lpFile );
		fwrite(((char*)lpBuff + _samNum*2 - 8 -_dCorrection*2), 8 + _dCorrection*2, 1, _lpFile);
	}
	if(m_nUpload==1){
		fwrite(((char*)lpBuff + 8 + _dCorrection*2), len - 8 -_dCorrection*2, 1, _lpFile);//8是头8位道头信息 不存进裸数据  _correction*2 一个点两个字节
		//char *temp = new char[8+_dCorrection*2];
		//for(int i=0;i<8+_dCorrection*2;i++){
		//	temp[i] = (char)lpBuff[_samNum*2-1];
		//}
		//fwrite( temp, 8+_dCorrection*2, 1, _lpFile );
		fwrite(((char*)lpBuff + _samNum*2 - 8 -_dCorrection*2), 8 + _dCorrection*2, 1, _lpFile);
	}
	*/
	//fwrite(((char*)lpBuff + 8 + _dCorrection*2), len - 8 -_dCorrection*2, 1, _lpFile);//8是头8位道头信息 不存进裸数据  _correction*2 一个点两个字节
	fwrite(((char*)lpBuff + 8 + _dCorrection*2), 1, len - 8 -_dCorrection*2, _lpFile);//8是头8位道头信息 不存进裸数据  _correction*2 一个点两个字节
	//fwrite(((char*)lpBuff + _samNum*2 - 8 -_dCorrection*2), 8 + _dCorrection*2, 1, _lpFile);
	/*
	char *temp = new char[8+_dCorrection*2];
	for(int i=0;i<8+_dCorrection*2;i++){
		temp[i] = '0';
	}*/
	short *temp = new short[4+_dCorrection];
	for(int i=0;i<4+_dCorrection;i++){
		temp[i] = 0;
	}
	fwrite((char*)temp, 1, 8 + _dCorrection*2, _lpFile);

	/*if(m_nUpload==1){
		_dataCount=_dataCount+1;
	}*/
	_dataCount=_dataCount+1;
}

void SGYWriter::writeRadarDataForAutomation( RadarData *lpData )
{
	if(_dataCount>(m_nDataCountThreshold-1)&&m_bIsChange){
		changeSaveFile();
		m_bIsChange=false;
		//_dataCount=0;
		m_bIsChangedJustNow=true;
	}
	int len = 0;
	unsigned short *lpBuff = ( unsigned short *)(lpData->getData( len ));
	if ( !lpBuff )
	{
		return;
	}
	
	fwrite(((char*)lpBuff + 8 + _dCorrection*2), len - 8 -_dCorrection*2, 1, _lpFile);//8是头8位道头信息 不存进裸数据  _correction*2 一个点两个字节
	fwrite(((char*)lpBuff + _samNum*2 - 8 -_dCorrection*2), 8 + _dCorrection*2, 1, _lpFile);

	_dataCount=_dataCount+1;
}

void SGYWriter::writeDisLLPic(std::string videoTabSerialNum, RadarData *lpRadarData,std::ofstream& infile)
{
	//获取距离
	infile.precision(5);
	//double distance = getTotalLen( 0, lpRadarData->getPrec() );
	double distance = lpRadarData->getLen();


	bool isWrite = false;
	if (_dLastDistanceGps == 0.0)
	{
		isWrite = true;
	}

	if ((distance-_dLastDistanceGps)>=1.0)
	{
		isWrite = true;
	}
	else
	{
		isWrite = false;
	}

	if (isWrite)
	{
		osg::Vec3d pos = lpRadarData->getPosition();
		//获取vediotaB中时间对应的图片AB的路径
		DWORD receiveTime = lpRadarData->getTime();

		std::stringstream ss;
		ss << receiveTime;

		VideoTemplateTab pVideoTemplateTab;
		pVideoTemplateTab.setProjectName(videoTabSerialNum);

		//根据receiveTime去重获取图片的路径
		std::string strPicPathA,strPicPathB;
		bool status = pVideoTemplateTab.getPicPath(RadarManager::Instance()->getDBHelp(), ss.str(),strPicPathA,strPicPathB);
		if (status)
		{
			infile<<"distance："<<setiosflags(ios::left)<<setiosflags(ios::fixed)<<setprecision(1)<<setw(5)<<setfill('0')<<distance<<"  "
				<<setiosflags(ios::left)<<setw(12)<<setfill('0')<<setprecision(12)<<pos.x()<<" "
				<<setiosflags(ios::left)<<setw(12)<<setfill('0')<<setprecision(12)<<pos.y()<<" "
				<<strPicPathA<<" "<<strPicPathB<<std::endl;
		}
		else
		{
			infile<<"distance："<<setiosflags(ios::left)<<setiosflags(ios::fixed)<<setprecision(1)<<setw(5)<<setfill('0')<<distance<<"  "
				<<setiosflags(ios::left)<<setw(12)<<setfill('0')<<setprecision(12)<<pos.x()<<" "
				<<setiosflags(ios::left)<<setw(12)<<setfill('0')<<setprecision(12)<<pos.y()<<" "
				<<"没有照片"<<std::endl;
		}
		_dLastDistanceGps = distance;
	}
}

void SGYWriter::writeDisLL(RadarData *lpRadarData,std::ofstream& infile, float& pOldDis,const float pInterval)
{
	//获取距离
	infile.precision(5);
	//double distance = getTotalLen( 0, lpRadarData->getPrec() );
	double distance = lpRadarData->getLen();

	bool isWrite = false;
	if ((distance-pOldDis)>=pInterval)
	{
		isWrite = true;
	}
	else
	{
		isWrite = false;
	}

	if (pOldDis == 0.0)
	{
		isWrite = true;
	}

	if (isWrite)
	{
		osg::Vec3d pos = lpRadarData->getPosition();
		infile<<setiosflags(ios::left)<<setiosflags(ios::fixed)<<setprecision(1)<<setw(5)<<setfill('0')<<distance<<" "
			<<setiosflags(ios::left)<<setw(12)<<setfill('0')<<setprecision(12)<<pos.x()<<" "
			<<setiosflags(ios::left)<<setw(12)<<setfill('0')<<setprecision(12)<<pos.y()<<" "
			<<setiosflags(ios::left)<<setw(12)<<setfill('0')<<setprecision(12)<<0.1<<std::endl;
		
		pOldDis = distance;
	}
}


string DoubleToString(const double value)                                                                                                                                                                                                          
{
   std::ostringstream result;
   result << value;
   return result.str();
}

string to_StringForInt(int n)
{
    int m = n;
    char s[100];
    char ss[100];
    int i=0,j=0;
    if (n < 0)// 处理负数
    {
        m = 0 - m;
        j = 1;
        ss[0] = '-';
    }    
    while (m>0)
    {
        s[i++] = m % 10 + '0';
        m /= 10;
    }
    s[i] = '\0';
    i = i - 1;
    while (i >= 0)
    {
        ss[j++] = s[i--];
    }    
    ss[j] = '\0';    
    return ss;
}


void SGYWriter::writeGPSData( RadarData *lpRadarData )
{
	std::vector< std::string > _currentRawGps(RadarManager::Instance()->getGpsReader()->getCurrentRawGpsDataBuff());
	for(int i=0; i<_currentRawGps.size(); i++){
		fprintf(_lpGPSRawFile,_currentRawGps[i].c_str() );
		if(i!=_currentRawGps.size()-1){
			fprintf(_lpGPSRawFile,"," );
		}
	}
	_currentRawGps.clear();

	//std::vector< std::string > _currentGps(RadarManager::Instance()->getGpsReader()->getCurrentGpsDataBuff());
	std::vector< std::string > _currentGps(lpRadarData->getGPGGA());
	if ( _currentGps.size()!=15 || _currentGps[14].size()<3 || _currentGps[2]=="" || _currentGps[4]=="" ){//第二个判断是确保最后一项有换行符 三四判断经纬度不为零
		return;
	}

	int wheelCount = lpRadarData->getPrec();//测量轮道数
	int realWheelCount = lpRadarData->getRealWheelCount();
	float distanceGps = lpRadarData->getLen();
	int distanceWheel = (int)lpRadarData->getPrecLen();
	osg::Vec3d pos = lpRadarData->getPosition();

	//csv的
	fprintf(_lpGPSCsvFile, "%d,%d,%d,%.9f,%.9f,%.4f,", realWheelCount,(int)distanceGps,distanceWheel,(double)pos.x(),(double)pos.y(),(double)pos.z());
	//switch ( RadarManager::Instance()->getGpsReader()->getCondition()){
	switch ( atoi(_currentGps[6].c_str()) ){
		case 0:
			fprintf(_lpGPSCsvFile, "初始化");
			break;
		case 1:
			fprintf(_lpGPSCsvFile, "单点定位");
			break;
		case 2:
			fprintf(_lpGPSCsvFile, "差分定位");
			break;
		case 3:
			fprintf(_lpGPSCsvFile, "无效PPS");
			break;
		case 4:
			fprintf(_lpGPSCsvFile, "固定解");
			break;
		case 5:
			fprintf(_lpGPSCsvFile, "浮动解");
			break;
		case 6:
			fprintf(_lpGPSCsvFile, "正在估算");
			break;
		default:
			fprintf(_lpGPSCsvFile, "初始化");
			break;
	}
	fprintf(_lpGPSCsvFile, "\n");

	//时间戳
	time_t nowtime;
	//首先创建一个time_t 类型的变量nowtime
	struct tm* p;
	//然后创建一个新时间结构体指针 p 
	time(&nowtime);
	//使用该函数就可得到当前系统时间，使用该函数需要将传入time_t类型变量nowtime的地址值。
	p = localtime(&nowtime);
	std::string strTimeTab = to_StringForInt(p->tm_year+1900);
	if(p->tm_mon+1<10){
		strTimeTab=strTimeTab+"-"+"0"+to_StringForInt(p->tm_mon+1);
	}else{
		strTimeTab=strTimeTab+"-"+to_StringForInt(p->tm_mon+1);
	}
	if(p->tm_mday<10){
		strTimeTab=strTimeTab+"-"+"0"+to_StringForInt(p->tm_mday);
	}else{
		strTimeTab=strTimeTab+"-"+to_StringForInt(p->tm_mday);
	}
	if(p->tm_hour==0){
		strTimeTab=strTimeTab+" "+"00";
	}else if(p->tm_hour<10){
		strTimeTab=strTimeTab+" "+"0"+to_StringForInt(p->tm_hour);
	}else{
		strTimeTab=strTimeTab+" "+to_StringForInt(p->tm_hour);
	}
	if(p->tm_min==0){
		strTimeTab=strTimeTab+":"+"00";
	}else if(p->tm_min<10){
		strTimeTab=strTimeTab+":"+"0"+to_StringForInt(p->tm_min);
	}else{
		strTimeTab=strTimeTab+":"+to_StringForInt(p->tm_min);
	}
	if(p->tm_sec==0){
		strTimeTab=strTimeTab+":"+"00";
	}else if(p->tm_sec<10){
		strTimeTab=strTimeTab+":"+"0"+to_StringForInt(p->tm_sec);
	}else{
		strTimeTab=strTimeTab+":"+to_StringForInt(p->tm_sec);
	}
	
	//cor
	//if(_indexOfRecordedGpsData%4==0){
	//if((realWheelCount-1)%4==0){//四道一存
		//fprintf(_lpGPSCorFile, "%d %d ", wheelCount,_indexOfRecordedGpsData);
		//fprintf(_lpGPSCorFile, "%d %d ", wheelCount,realWheelCount);
		fprintf(_lpGPSCorFile, "%d %d ", wheelCount,_dataCount);
		fprintf(_lpGPSCorFile,_currentGps[2].c_str() );
		fprintf(_lpGPSCorFile, " ");
		fprintf(_lpGPSCorFile,_currentGps[4].c_str() );
		fprintf(_lpGPSCorFile, " ");
		fprintf(_lpGPSCorFile,_currentGps[9].c_str() );
		fprintf(_lpGPSCorFile, " ");
		
		fprintf(_lpGPSCorFile, strTimeTab.c_str());
		fprintf(_lpGPSCorFile, " $GPGGA");
		for(int i =1; i<(_currentGps.size()-1); i++){
			fprintf(_lpGPSCorFile,"," );
			fprintf(_lpGPSCorFile,_currentGps[i].c_str() );
		}
		fprintf(_lpGPSCorFile,"," );
		fprintf(_lpGPSCorFile,_currentGps[_currentGps.size()-1].substr(0,_currentGps[_currentGps.size()-1].length() - 2).c_str() );
		fprintf(_lpGPSCorFile, "\r\n");
	//}

	//corForUpload
	if(m_nUpload==1){
		if(m_bIsChangedJustNow){//刚转完文件必保存第一个数据
			//fprintf(_lpGPSCorFileForUpload, "%d %d ", (_dataCount+1),_indexOfRecordedGpsData);
			//fprintf(_lpGPSCorFileForUpload, "%d %d ", m_dLastRecordedDataCount,_indexOfRecordedGpsData);
			fprintf(_lpGPSCorFileForUpload, "%d %d ", m_dLastRecordedDataCount,1);
			//fprintf(_lpGPSCorFile, "%d %d ", wheelCount,realWheelCount);
			//fprintf(_lpGPSCorFileForUpload,_currentGps[2].c_str() );
			fprintf(_lpGPSCorFileForUpload, "%.9f", m_pLastPos.x());
			fprintf(_lpGPSCorFileForUpload, " ");
			fprintf(_lpGPSCorFileForUpload, "%.9f", m_pLastPos.y());
			fprintf(_lpGPSCorFileForUpload, " ");
			fprintf(_lpGPSCorFileForUpload, "%f", m_pLastPos.z());
			fprintf(_lpGPSCorFileForUpload, " ");
			fprintf(_lpGPSCorFileForUpload, strTimeTab.c_str());
			fprintf(_lpGPSCorFileForUpload, " $GPGGA");
			/*
			for(int i =1; i<(_currentGps.size()-1); i++){
				fprintf(_lpGPSCorFileForUpload,",");
				fprintf(_lpGPSCorFileForUpload, _currentGps[i].c_str());
			}*/
			for(int i =1; i<(m_vecStrLastGPGGA.size()-1); i++){
				fprintf(_lpGPSCorFileForUpload,",");
				fprintf(_lpGPSCorFileForUpload, m_vecStrLastGPGGA[i].c_str());
			}
			fprintf(_lpGPSCorFileForUpload, ",");
			//fprintf(_lpGPSCorFileForUpload, _currentGps[_currentGps.size()-1].substr(0,_currentGps[_currentGps.size()-1].length() - 2).c_str());
			fprintf(_lpGPSCorFileForUpload, m_vecStrLastGPGGA[m_vecStrLastGPGGA.size()-1].substr(0,m_vecStrLastGPGGA[m_vecStrLastGPGGA.size()-1].length() - 2).c_str());
			//fprintf(_lpGPSFile,_currentGps[_currentGps.size()-1].substr(0,7).c_str() );
			fprintf(_lpGPSCorFileForUpload, "\r\n");
			_indexOfRecordedGpsData+=1;
			m_dLastRecordedDataCount=_dataCount;
			m_dStartDataCount=_dataCount-1;
			m_fLastRecordedGpsDistance=_dLastDistanceGps;
			m_vecStrLastGPGGA.assign(_currentGps.begin(),_currentGps.end());
			m_pLastPos=pos;
			_dLastDistanceGps=distanceGps;
			m_bIsChangedJustNow=false;
		}else if((distanceGps-_dLastDistanceGps)>0||_dLastDistanceGps == 0.0){//距离大于设定值后才保存
			//if(strcmp(strTimeTab.c_str(),_strTimeTab.c_str())==0){//时间跟上次不同
			if(m_fLastRecordedGpsDistance!=_dLastDistanceGps){//这是为了防止啥情况？？？？？哭。。。。
				//_dLastDistanceGps = distanceGps;
				//_strTimeTab=strTimeTab;
				//用于上传的
				//fprintf(_lpGPSCorFileForUpload, "%d %d ", (_dataCount+1),_indexOfRecordedGpsData);
				fprintf(_lpGPSCorFileForUpload, "%d %d ", m_dLastRecordedDataCount,m_dLastRecordedDataCount-m_dStartDataCount);
				//fprintf(_lpGPSCorFile, "%d %d ", wheelCount,realWheelCount);
				//fprintf(_lpGPSCorFileForUpload,_currentGps[2].c_str() );
				fprintf(_lpGPSCorFileForUpload, "%.9f", m_pLastPos.x());
				fprintf(_lpGPSCorFileForUpload, " ");
				fprintf(_lpGPSCorFileForUpload, "%.9f", m_pLastPos.y());
				fprintf(_lpGPSCorFileForUpload, " ");
				fprintf(_lpGPSCorFileForUpload, "%f", m_pLastPos.z());
				fprintf(_lpGPSCorFileForUpload, " ");
				fprintf(_lpGPSCorFileForUpload, strTimeTab.c_str());
				fprintf(_lpGPSCorFileForUpload, " $GPGGA");
				/*
				for(int i =1; i<(_currentGps.size()-1); i++){
					fprintf(_lpGPSCorFileForUpload,",");
					fprintf(_lpGPSCorFileForUpload, _currentGps[i].c_str());
				}*/
				for(int i =1; i<(m_vecStrLastGPGGA.size()-1); i++){
					fprintf(_lpGPSCorFileForUpload,",");
					fprintf(_lpGPSCorFileForUpload, m_vecStrLastGPGGA[i].c_str());
				}
				fprintf(_lpGPSCorFileForUpload, ",");
				//fprintf(_lpGPSCorFileForUpload, _currentGps[_currentGps.size()-1].substr(0,_currentGps[_currentGps.size()-1].length() - 2).c_str());
				fprintf(_lpGPSCorFileForUpload, m_vecStrLastGPGGA[m_vecStrLastGPGGA.size()-1].substr(0,m_vecStrLastGPGGA[m_vecStrLastGPGGA.size()-1].length() - 2).c_str());
				//fprintf(_lpGPSFile,_currentGps[_currentGps.size()-1].substr(0,7).c_str() );
				fprintf(_lpGPSCorFileForUpload, "\r\n");
				_indexOfRecordedGpsData+=1;
			}
			m_dLastRecordedDataCount=_dataCount;
			m_fLastRecordedGpsDistance=_dLastDistanceGps;
			m_vecStrLastGPGGA.assign(_currentGps.begin(),_currentGps.end());
			m_pLastPos=pos;
			_dLastDistanceGps=distanceGps;
		}
	}
	//_currentGps.clear();
}


void SGYWriter::writeKmlHead()
{
	ConfigureSet *cfg = RadarManager::Instance()->getConfigureSet();
	fprintf(_lpKmlFile, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
	fprintf(_lpKmlFile, "<kml xmlns=\"http://www.opengis.net/kml/2.2\"\n");
	fprintf(_lpKmlFile, "\txmlns:atom=\"http://www.w3.org/2005/Atom\"\n");
	fprintf(_lpKmlFile, "\txmlns:gx=\"http://www.google.com/kml/ext/2.2\"\n");
	fprintf(_lpKmlFile, "\t>\n");
	fprintf(_lpKmlFile, "\t<Document>\n");
	//fprintf(_lpKmlFile, "\t\t<name>test</name>\n");
	fprintf(_lpKmlFile, "\t\t<name>");
	fprintf(_lpKmlFile, cfg->get("projectname", "roadname").c_str());
	fprintf(_lpKmlFile, "</name>\n");
	fprintf(_lpKmlFile, "\t\t<Placemark>\n");
	//fprintf(_lpKmlFile, "\t\t\t<name>test</name>\n");
	fprintf(_lpKmlFile, "\t\t\t<name>");
	fprintf(_lpKmlFile, cfg->get("projectname", "roadname").c_str());
	fprintf(_lpKmlFile, "</name>\n");
	fprintf(_lpKmlFile, "\t\t\t<Style>\n");
	fprintf(_lpKmlFile, "\t\t\t\t<LineStyle><color>7fff0000</color><width>3</width></LineStyle>\n");
	fprintf(_lpKmlFile, "\t\t\t</Style>\n");
	fprintf(_lpKmlFile, "\t\t\t\t<LineString>\n");
	fprintf(_lpKmlFile, "\t\t\t\t\t<coordinates>");
}

void SGYWriter::writeKml( RadarData *lpRadarData )
{
	double distance = lpRadarData->getLen();
	//int number = lpRadarData->getPrec();//测量轮道数
	if ((distance-_dLastDistanceKml)>=1||_dLastDistanceKml == 0.0){
		osg::Vec3d pos = lpRadarData->getPosition();
		fprintf(_lpKmlFile, "%f,%f,%f ", pos.x(),pos.y(),pos.z());
		_dLastDistanceKml = distance;
	}

/*
	bool isWrite = false;
	if ((distance-_dLastDistanceKml)>=1){
		isWrite = true;
	}else{
		isWrite = false;
	}
	if (_dLastDistanceKml == 0.0){
		isWrite = true;
	}
	if (isWrite){
		osg::Vec3d pos = lpRadarData->getPosition();
		fprintf(_lpKmlFile, "%f,%f,%f ", pos.x(),pos.y(),pos.z());
		_dLastDistanceKml = distance;
	}*/
}

void SGYWriter::writeKmlEnding()
{
	fprintf(_lpKmlFile, "</coordinates>\n");
	fprintf(_lpKmlFile, "\t\t\t\t</LineString>\n");
	fprintf(_lpKmlFile, "\t\t</Placemark>\n");
	fprintf(_lpKmlFile, "\t</Document>\n");
	fprintf(_lpKmlFile, "</kml>\n");
}

void SGYWriter::setDataCountThreshold(int value)
{
	m_nDataCountThreshold = value;
}

void SGYWriter::setIsChange(bool value)
{
	m_bIsChange = value;
}

void SGYWriter::setUploadFlag(int value)
{
	m_nUpload = value;
}
