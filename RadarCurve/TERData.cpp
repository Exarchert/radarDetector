#include "StdAfx.h"
#include <RadarCurve/TERData.h>
#include <memory.h>
#include <WinSock2.h>


#define DATA_START 8 //前8项是记录东西的

const float DIV_G = 1.0f / 1000/* / 1000*/;
TERData::TERData(void)
{
	_receiveTime = GetTickCount();
	m_recordWheelCount = 0;
	_sampleCount = 0;
	_sampleRatio = 0;
	_precLen = 0.0;
	_mark = false;
	_len = 0.0f;
}

TERData::~TERData(void)
{
	_dataBuff;

	//int a = 0;
	//a++;
}

//设置经纬度值
void TERData::setPosition( osg::Vec3d const pos )
{
	_position = pos;
	//_receiveTime = GetTickCount();
}

//设置经纬度值
osg::Vec3d TERData::getPosition()const
{
	return _position;
}

//设置GPGGA
void TERData::setGPGGA( std::vector< std::string > vecStrGPGGA )
{
	//m_vecStrGPGGA = strGPGGA;
	//_receiveTime = GetTickCount();
	m_vecStrGPGGA.assign( vecStrGPGGA.begin(), vecStrGPGGA.end() );
}

//获取GPGGA
std::vector< std::string > TERData::getGPGGA()const
{
	return m_vecStrGPGGA;
}

//设置接收时间
void TERData::setTime( DWORD value )
{
	_receiveTime = value;
}

//获取接收时间
DWORD TERData::getTime()const
{
	return _receiveTime;
}

//设置雷达数据
void TERData::setData( unsigned char *buff,  int len )
{
	_dataBuff.clear();
	_dataBuff.addData( buff, len );
}

//获取数据及长度
unsigned char *TERData::getData( int &len )const
{
	len = _dataBuff.getDataLen();
	return _dataBuff.getData();
}

//设置采样点数
void TERData::setSampleCount( int value )
{
	_sampleCount = value;
}

//获取采样点数
int TERData::getSampleCount()const
{
	return _sampleCount;
}

//设置采样率
void TERData::setSampleRatio( float value )
{
	_sampleRatio = value;
}

//获取采样率
float TERData::getSampleRatio()const
{
	return _sampleRatio;
}

//设置gps长度
void TERData::setLen( float value )
{
	_len = value;
}

//获取gps长度
float TERData::getLen()const
{
	return _len;
}

//按index取数据(已去掉8字节道头）
float TERData::getIndexData( int index, float *t )
{
	float value = 0;
	if(_dataBuff.getData()==NULL){
		return 0;
	}
	if ( index * 4 > _dataBuff.getDataLen() )
	{
		if ( t )
		{
			*t = 0;
		}
		return 0;
	}

	float *data = (float*)(_dataBuff.getData());//以指针的形式来取数组
	
	value = data[index];

	if ( t )
	{
		*t = _receiveTime + ( getDataCount() - index ) * 1.0f /*/ _sampleRatio */* DIV_G;
	}

	return value;
}

//获取采样点数-2的值
int TERData::getDataCount()const
{
	return _sampleCount;
}

//获取通道序数
int TERData::getChannelIndex()const
{
	short value = 0;
	if ( _dataBuff.getDataLen() > 4 )
	{

		int index = _dataBuff.getData()[1] - 0X72;
		return index;
	}
	return 0;
}

//获取数据中的测量轮道数
int TERData::getPrec()const
{
	if ( _dataBuff.getDataLen() <= DATA_START )
	{
		return 0;
	}
	unsigned char *data = (unsigned char*)( _dataBuff.getData() );
	return data[4] + data[5] * 256;
}

//获取实际测量轮道数
int TERData::getRecordWheelCount()const
{
	return m_recordWheelCount;
}

//设置数据中的测量轮道数
void TERData::setPrec( int value ){
	/*
	int NumOne=value%256;
	int NumTwo=value/256;
	unsigned char *data = (unsigned char*)( _dataBuff.getData() );
	data[6] = NumOne; 
	data[7] = NumTwo;
	*/
	unsigned short *data = (unsigned short*)( _dataBuff.getData() );
}

//设置实际测量轮道数
void TERData::setRecordWheelCount( int value ){
	m_recordWheelCount = value;
}

//设置电压
unsigned short TERData::getDCValue()const
{
	unsigned short  value = 0;
	if ( _dataBuff.getDataLen() <= DATA_START )
	{
		return 0;
	}

	short *data = (short*)(_dataBuff.getData() );
	value = data[2];

	return value;
}

//设置测量轮距离
void TERData::setPrecLen( double vaule )
{
	_precLen = vaule;
}

//获取测量轮距离
double TERData::getPrecLen()const
{
	return _precLen;
}

//设置是否作为标记
void TERData::setMark( bool value )
{
	_mark = value;
}

//获取是否作为标记
bool TERData::getMark()const
{
	return _mark;
}

