#include "StdAfx.h"
#include "GPRWriter.h"

#include "SGYHeader.h"

#include <io.h>
#include <fcntl.h>
#include <RadarCurve/RadarManager.h>


GPRWriter::GPRWriter(void)
{
	_lpFile = NULL;

	_markNo = 1;

	_totalLen = 0;

	_gprTrace.fGPSOffset = 0;
}

GPRWriter::~GPRWriter(void)
{
}

bool GPRWriter::openGPR( std::string const& filePath, std::string const& paramXML, int trackNo )
{
	_lpFile = fopen( filePath.c_str(), "wb" );

	_iTrackNo = trackNo;

	if ( !_lpFile )
	{
		return false;
	}

	for( int i = 0; i < 8; i ++ )
	{
		_wheelNum[i] = -1;
	}

	_set.fromGroupXML( paramXML );

	int precRatio = atoi( _set.get("radar", "precratio").c_str() );
	float precLen = atoi ( _set.get("radar", "preclen").c_str() );
	//int precIndex = atoi( _set.get("radar", "precindex").c_str() );
	int iSmpNum = atoi ( _set.get("radar", "sample").c_str());
	int iSampRate = atoi ( _set.get("radar", "sampleratio" ).c_str() );

	_sampRate = RadarManager::Instance()->getSampRatio( iSampRate, trackNo );

	_samNum = RadarManager::Instance()->getSampCount( iSmpNum );

	_dTimeWnd = 1.0 / _sampRate * _samNum;

	//float fJihuaAccuracy = RadarManager::Instance()->GetJihuaAccuracy(precIndex);
	float fJihuaAccuracy = (float)atoi( _set.get("radar", "precision").c_str() );
	float fp = RadarManager::Instance()->GetTrueAccuracyFromPreclenAndPrecindex(precLen,fJihuaAccuracy);
	_precValue = fp / 100.0;

	m_iTrcNum = 0;

	return true;
}
bool GPRWriter::closeGPR()
{
	fclose( _lpFile );
	_lpFile = 0;
	return true;
}

void GPRWriter::writeHead( int trackNum,std::string _projectNameTime )
{
	ZeroMemory(&sGPRHead, sizeof(_GPR_HEAD));

	strcpy(sGPRHead.chDescription, "文件描述");
	strcpy(sGPRHead.chClrFileName, "");
	sGPRHead.dLenInt = 0.91027580112195494 ;
	sGPRHead.iGainPtNum = 2;
	for (int k = 0; k < 10; k++)
	{
		sGPRHead.sHandGainPt[k].x = 0;
		sGPRHead.sHandGainPt[k].y = 0;
	}
	sGPRHead.dMaxGainTime = -1.0;


	sGPRHead.chFileFlag[0] = 'G';
	sGPRHead.chFileFlag[1] = 'P';
	sGPRHead.chFileFlag[2] = 'R';
	sGPRHead.dDielectric = 7; 


	sGPRHead.dTimeWnd = _dTimeWnd;
	sGPRHead.iSmpNum =_samNum + 45;



	sGPRHead.iSwMode = 1;
	sGPRHead.iTrcInt = 1;
	sGPRHead.iTrcDis = 5;
	sGPRHead.iMaxAmp = 10;
	sGPRHead.bNormal = TRUE;


	sGPRHead.iLfAxMode = 100;
	sGPRHead.iRtAxMode = 10;
	sGPRHead.iTpAxMode = 10;
	sGPRHead.iBtAxMode = 1;
	//
	sGPRHead.iLfMinSmp = 100;
	sGPRHead.dLfMinTime = 50;
	sGPRHead.dLfMinDepth = 1;
	//
	sGPRHead.iRtMinSmp = 100;
	sGPRHead.dRtMinTime = 50;
	sGPRHead.dRtMinDepth = 1;
	//
	sGPRHead.iTpMinTrc = 100;
	sGPRHead.dTpMinDis = 10;
	//
	sGPRHead.iBtMinTrc = 100;
	sGPRHead.dBtMinDis = 10;
	//
	sGPRHead.iShowMileAxis = 0;	  
	sGPRHead.fStartBigMile = 0;  
	sGPRHead.fStartSmallMile = 0;
	//
	sGPRHead.bAutoAxis = TRUE;
	sGPRHead.bChsAxis = TRUE;
	sGPRHead.iZeroPos = 0;

	sGPRHead.iTrcNum = 13013;

	sGPRHead.bShowFileTitle = false;


	sprintf((char*)&sGPRHead.ucRadarModel,RadarManager::Instance()->getDeviceCode( _iTrackNo ).c_str());


	int isize = sizeof(_GPR_HEAD);

	fwrite( (char*)&sGPRHead, isize, 1, _lpFile );
}
double GPRWriter::getTotalLen( int iTracNo, int wheelCount )
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

void GPRWriter::writeRadarData( RadarData *lpData )
{
	int len = 0;
	//获取数据及长度 lpBuff储存数据 len储存数据长度
	unsigned short *lpBuff = ( unsigned short *)(lpData->getData( len ));
	if ( !lpBuff )
	{
		return;
	}
	
	time_t _t = lpData->getTime();//return _receiveTime
	struct tm *lt = localtime( &_t);

	//获取时间
	_gprTrace.ucYear = lt->tm_year + 1900 - 2000;
	_gprTrace.ucMonth = lt->tm_mon + 1;
	_gprTrace.ucDay = lt->tm_mday;
	_gprTrace.ucHour = lt->tm_hour;
	_gprTrace.ucMin = lt->tm_min;
	_gprTrace.ucSec = lt->tm_sec;
	_gprTrace.usMilSec = 0;
	
	//获取gps信息
	osg::Vec3d pos = lpData->getPosition();
	_gprTrace.dPosX = pos.x();
	_gprTrace.dPosY = pos.y();
	_gprTrace.dPosZ = pos.z();
		
	memcpy(_gprTrace.ucVoltage, lpBuff+4, 2);
 	memcpy(_gprTrace.ucTrcCount, lpBuff+6, 2);
	
	//总长度
	_gprTrace.fWheelOffset = (float)(getTotalLen( lpData->getChannelIndex(), lpData->getPrec() ) *1.0);




	if (lpData->getMark())
	{

	}


	_gprTrace.usMetalDiameter = lpBuff[9];
	_gprTrace.usMetalDepth = lpBuff[11];
	unsigned int usMetalFlag = lpBuff[15];
	_gprTrace.bMetalFlag = usMetalFlag > 0;

	if(m_iTrcNum == 0)
	{
		// 计算GPS距离
		cFirstVector.X = _gprTrace.dPosX;
		cFirstVector.Y = _gprTrace.dPosY;
		cFirstVector.Z = _gprTrace.dPosZ;
		//
		cFirstVector.Angle2Vector3D();
		 

		iFirstTrcOffset = _gprTrace.fWheelOffset * 1000;
	}

	// 计算GPS距离
	CVector3D cSecondVector;
	cSecondVector.X = _gprTrace.dPosX;
	cSecondVector.Y = _gprTrace.dPosY;
	cSecondVector.Z = _gprTrace.dPosZ;


	cSecondVector.Angle2Vector3D();
	CVector3D cDiffVector = cSecondVector - cFirstVector;
	float fTrcGPSDis = (float)cDiffVector.Length();
	cFirstVector = cSecondVector;
	//
	_gprTrace.fGPSOffset = _gprTrace.fGPSOffset + fTrcGPSDis;


	_gprTrace.usMarkFlag = 0xAA;

		


	int isize =  sizeof( _GPR_TRACE );

	int dataLen = sGPRHead.iSmpNum * 2 - isize;

	fwrite( (char*)&_gprTrace,isize, 1, _lpFile);

	unsigned short ushort = 0;

	int si = sizeof(unsigned short);
	len = ( len - 8 )/ 2;

	lpBuff = lpBuff + 3;
	for (int i = 0; i < len ; ++ i)
	{
		lpBuff++;
		ushort = *lpBuff;
		ushort += 23768;
		fwrite( &ushort , 2 , 1, _lpFile);
	}
	char buff[8];
	fwrite( buff, 8, 1, _lpFile );
	m_iTrcNum++;
}

void GPRWriter::buchongHead()
{
	long offset = _gprTrace.fWheelOffset * 1000;
	sGPRHead.iTrcNum = m_iTrcNum;
	sGPRHead.dLenInt = (double)(offset - iFirstTrcOffset) /(sGPRHead.iTrcNum *1000.0);
	if (sGPRHead.dLenInt <= 0)
	{
		sGPRHead.dLength = 10.0;
		sGPRHead.dLenInt = 10.0 /sGPRHead.iTrcNum;
	}
	else
	{
		sGPRHead.dLength = (offset - iFirstTrcOffset) /1000.0;
		sGPRHead.dLenInt = sGPRHead.dLength /sGPRHead.iTrcNum;
	}

	int m_iGPRHeadLen = sizeof(_GPR_HEAD);
	
	fseek(_lpFile,0, CFile::begin);
	fwrite( &sGPRHead, m_iGPRHeadLen, 1, _lpFile );

}