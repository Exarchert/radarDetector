#include "StdAfx.h"
#include <RadarCurve/RadarData.h>
#include <memory.h>
#include <WinSock2.h>


#define DATA_START 8 //前8项是记录东西的

const float DIV_G = 1.0f / 1000/* / 1000*/;
RadarData::RadarData(void)
{
	_receiveTime = GetTickCount();

	_realWheelCount = 0;
	_sampleCount = 0;
	_sampleRatio = 0;
	_precLen = 0.0;
	_mark = false;
	_len = 0.0f;
}

RadarData::~RadarData(void)
{
	_dataBuff;

	//int a = 0;
	//a++;
}

//设置经纬度值
void RadarData::setPosition( osg::Vec3d const pos )
{
	_position = pos;
	//_receiveTime = GetTickCount();
}

//设置经纬度值
osg::Vec3d RadarData::getPosition()const
{
	return _position;
}

//设置GPGGA
void RadarData::setGPGGA( std::vector< std::string > vecStrGPGGA )
{
	//m_vecStrGPGGA = strGPGGA;
	//_receiveTime = GetTickCount();
	m_vecStrGPGGA.assign( vecStrGPGGA.begin(), vecStrGPGGA.end() );
}

//获取GPGGA
std::vector< std::string > RadarData::getGPGGA()const
{
	return m_vecStrGPGGA;
}

//设置接收时间
void RadarData::setTime( DWORD value )
{
	_receiveTime = value;
}

//获取接收时间
DWORD RadarData::getTime()const
{
	return _receiveTime;
}

//设置雷达数据
void RadarData::setData( unsigned char *buff,  int len )
{
	_dataBuff.clear();
	_dataBuff.addData( buff, len );
}

//获取数据及长度
unsigned char *RadarData::getData( int &len )const
{
	len = _dataBuff.getDataLen();
	return _dataBuff.getData();
}

//设置采样点数
void RadarData::setSampleCount( int value )
{
	_sampleCount = value;
}

//获取采样点数
int RadarData::getSampleCount()const
{
	return _sampleCount;
}

//设置采样率
void RadarData::setSampleRatio( float value )
{
	_sampleRatio = value;
}

//获取采样率
float RadarData::getSampleRatio()const
{
	return _sampleRatio;
}

//设置gps长度
void RadarData::setLen( float value )
{
	_len = value;
}

//获取gps长度
float RadarData::getLen()const
{
	return _len;
}

//按index取数据(已去掉8字节道头）
short RadarData::getIndexData( int index, float *t )
{
	short value = 0;
	if(_dataBuff.getData()==NULL){
		return 0;
	}
	if ( index * 2 + DATA_START + 2 > _dataBuff.getDataLen() )
	{
		if ( t )
		{
			*t = 0;
		}
		return 0;
	}

	short *data = (short*)(_dataBuff.getData() + DATA_START);//以指针的形式来取数组
	value = data[index];

	if ( t )
	{
		*t = _receiveTime + ( getDataCount() - index ) * 1.0f /*/ _sampleRatio */* DIV_G;
	}

	return value;
}

//获取采样点数-2的值
int RadarData::getDataCount()const
{
	return ( _sampleCount * 2 - DATA_START ) / 2;
}

//获取通道序数
int RadarData::getChannelIndex()const
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
int RadarData::getPrec()const
{
	if ( _dataBuff.getDataLen() <= DATA_START )
	{
		return 0;
	}
	unsigned char *data = (unsigned char*)( _dataBuff.getData() );
	return data[6] + data[7] * 256;
}

//获取实际测量轮道数
int RadarData::getRealWheelCount()const
{
	return _realWheelCount;
}

//设置数据中的测量轮道数
void RadarData::setPrec( int value ){
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
void RadarData::setRealWheelCount( int value ){
	_realWheelCount = value;
}

//设置电压
unsigned short RadarData::getDCValue()const
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
void RadarData::setPrecLen( double vaule )
{
	_precLen = vaule;
}

//获取测量轮距离
double RadarData::getPrecLen()const
{
	return _precLen;
}

//设置是否作为标记
void RadarData::setMark( bool value )
{
	_mark = value;
}

//获取是否作为标记
bool RadarData::getMark()const
{
	return _mark;
}

