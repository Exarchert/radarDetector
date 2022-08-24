#include "StdAfx.h"
#include <RadarCurve/RadarData.h>
#include <memory.h>
#include <WinSock2.h>


#define DATA_START 8 //ǰ8���Ǽ�¼������

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

//���þ�γ��ֵ
void RadarData::setPosition( osg::Vec3d const pos )
{
	_position = pos;
	//_receiveTime = GetTickCount();
}

//���þ�γ��ֵ
osg::Vec3d RadarData::getPosition()const
{
	return _position;
}

//����GPGGA
void RadarData::setGPGGA( std::vector< std::string > vecStrGPGGA )
{
	//m_vecStrGPGGA = strGPGGA;
	//_receiveTime = GetTickCount();
	m_vecStrGPGGA.assign( vecStrGPGGA.begin(), vecStrGPGGA.end() );
}

//��ȡGPGGA
std::vector< std::string > RadarData::getGPGGA()const
{
	return m_vecStrGPGGA;
}

//���ý���ʱ��
void RadarData::setTime( DWORD value )
{
	_receiveTime = value;
}

//��ȡ����ʱ��
DWORD RadarData::getTime()const
{
	return _receiveTime;
}

//�����״�����
void RadarData::setData( unsigned char *buff,  int len )
{
	_dataBuff.clear();
	_dataBuff.addData( buff, len );
}

//��ȡ���ݼ�����
unsigned char *RadarData::getData( int &len )const
{
	len = _dataBuff.getDataLen();
	return _dataBuff.getData();
}

//���ò�������
void RadarData::setSampleCount( int value )
{
	_sampleCount = value;
}

//��ȡ��������
int RadarData::getSampleCount()const
{
	return _sampleCount;
}

//���ò�����
void RadarData::setSampleRatio( float value )
{
	_sampleRatio = value;
}

//��ȡ������
float RadarData::getSampleRatio()const
{
	return _sampleRatio;
}

//����gps����
void RadarData::setLen( float value )
{
	_len = value;
}

//��ȡgps����
float RadarData::getLen()const
{
	return _len;
}

//��indexȡ����(��ȥ��8�ֽڵ�ͷ��
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

	short *data = (short*)(_dataBuff.getData() + DATA_START);//��ָ�����ʽ��ȡ����
	value = data[index];

	if ( t )
	{
		*t = _receiveTime + ( getDataCount() - index ) * 1.0f /*/ _sampleRatio */* DIV_G;
	}

	return value;
}

//��ȡ��������-2��ֵ
int RadarData::getDataCount()const
{
	return ( _sampleCount * 2 - DATA_START ) / 2;
}

//��ȡͨ������
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

//��ȡ�����еĲ����ֵ���
int RadarData::getPrec()const
{
	if ( _dataBuff.getDataLen() <= DATA_START )
	{
		return 0;
	}
	unsigned char *data = (unsigned char*)( _dataBuff.getData() );
	return data[6] + data[7] * 256;
}

//��ȡʵ�ʲ����ֵ���
int RadarData::getRealWheelCount()const
{
	return _realWheelCount;
}

//���������еĲ����ֵ���
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

//����ʵ�ʲ����ֵ���
void RadarData::setRealWheelCount( int value ){
	_realWheelCount = value;
}

//���õ�ѹ
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

//���ò����־���
void RadarData::setPrecLen( double vaule )
{
	_precLen = vaule;
}

//��ȡ�����־���
double RadarData::getPrecLen()const
{
	return _precLen;
}

//�����Ƿ���Ϊ���
void RadarData::setMark( bool value )
{
	_mark = value;
}

//��ȡ�Ƿ���Ϊ���
bool RadarData::getMark()const
{
	return _mark;
}

