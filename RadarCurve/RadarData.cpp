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
void RadarData::setPosition( osg::Vec3d const pos )
{
	_position = pos;
	//_receiveTime = GetTickCount();
}
osg::Vec3d RadarData::getPosition()const
{
	return _position;
}

void RadarData::setGPGGA( std::vector< std::string > vecStrGPGGA )
{
	//m_vecStrGPGGA = strGPGGA;
	//_receiveTime = GetTickCount();
	m_vecStrGPGGA.assign( vecStrGPGGA.begin(), vecStrGPGGA.end() );
}
std::vector< std::string > RadarData::getGPGGA()const
{
	return m_vecStrGPGGA;
}

void RadarData::setTime( DWORD value )
{
	_receiveTime = value;
}

DWORD RadarData::getTime()const
{
	return _receiveTime;
}

void RadarData::setData( unsigned char *buff,  int len )
{
	_dataBuff.clear();
	_dataBuff.addData( buff, len );
}

unsigned char *RadarData::getData( int &len )const//获取数据及长度
{
	len = _dataBuff.getDataLen();
	return _dataBuff.getData();
}

void RadarData::setSampleCount( int value )
{
	_sampleCount = value;
}

int RadarData::getSampleCount()const
{
	return _sampleCount;
}

void RadarData::setSampleRatio( float value )
{
	_sampleRatio = value;
}

float RadarData::getSampleRatio()const
{
	return _sampleRatio;
}

void RadarData::setLen( float value )
{
	_len = value;
}

float RadarData::getLen()const
{
	return _len;
}

short RadarData::getIndexData( int index, float *t )//按index取数据(已去掉8字节道头）
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

int RadarData::getDataCount()const
{
	return ( _sampleCount * 2 - DATA_START ) / 2;
}

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

int RadarData::getPrec()const
{
	if ( _dataBuff.getDataLen() <= DATA_START )
	{
		return 0;
	}
	unsigned char *data = (unsigned char*)( _dataBuff.getData() );
	return data[6] + data[7] * 256;
}

int RadarData::getRealWheelCount()const
{
	return _realWheelCount;
}

void RadarData::setPrec( int value ){
	int NumOne=value%256;
	int NumTwo=value/256;
	unsigned char *data = (unsigned char*)( _dataBuff.getData() );
	data[6] = NumOne; 
	data[7] = NumTwo;
}

void RadarData::setRealWheelCount( int value ){
	_realWheelCount = value;
}

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

void RadarData::setPrecLen( double vaule )
{
	_precLen = vaule;
}

double RadarData::getPrecLen()const
{
	return _precLen;
}

void RadarData::setMark( bool value )
{
	_mark = value;
}

bool RadarData::getMark()const
{
	return _mark;
}

