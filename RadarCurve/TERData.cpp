#include "StdAfx.h"
#include <RadarCurve/TERData.h>
#include <memory.h>
#include <WinSock2.h>


#define DATA_START 8 //ǰ8���Ǽ�¼������

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

//���þ�γ��ֵ
void TERData::setPosition( osg::Vec3d const pos )
{
	_position = pos;
	//_receiveTime = GetTickCount();
}

//���þ�γ��ֵ
osg::Vec3d TERData::getPosition()const
{
	return _position;
}

//����GPGGA
void TERData::setGPGGA( std::vector< std::string > vecStrGPGGA )
{
	//m_vecStrGPGGA = strGPGGA;
	//_receiveTime = GetTickCount();
	m_vecStrGPGGA.assign( vecStrGPGGA.begin(), vecStrGPGGA.end() );
}

//��ȡGPGGA
std::vector< std::string > TERData::getGPGGA()const
{
	return m_vecStrGPGGA;
}

//���ý���ʱ��
void TERData::setTime( DWORD value )
{
	_receiveTime = value;
}

//��ȡ����ʱ��
DWORD TERData::getTime()const
{
	return _receiveTime;
}

//�����״�����
void TERData::setData( unsigned char *buff,  int len )
{
	_dataBuff.clear();
	_dataBuff.addData( buff, len );
}

//��ȡ���ݼ�����
unsigned char *TERData::getData( int &len )const
{
	len = _dataBuff.getDataLen();
	return _dataBuff.getData();
}

//���ò�������
void TERData::setSampleCount( int value )
{
	_sampleCount = value;
}

//��ȡ��������
int TERData::getSampleCount()const
{
	return _sampleCount;
}

//���ò�����
void TERData::setSampleRatio( float value )
{
	_sampleRatio = value;
}

//��ȡ������
float TERData::getSampleRatio()const
{
	return _sampleRatio;
}

//����gps����
void TERData::setLen( float value )
{
	_len = value;
}

//��ȡgps����
float TERData::getLen()const
{
	return _len;
}

//��indexȡ����(��ȥ��8�ֽڵ�ͷ��
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

	float *data = (float*)(_dataBuff.getData());//��ָ�����ʽ��ȡ����
	
	value = data[index];

	if ( t )
	{
		*t = _receiveTime + ( getDataCount() - index ) * 1.0f /*/ _sampleRatio */* DIV_G;
	}

	return value;
}

//��ȡ��������-2��ֵ
int TERData::getDataCount()const
{
	return _sampleCount;
}

//��ȡͨ������
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

//��ȡ�����еĲ����ֵ���
int TERData::getPrec()const
{
	if ( _dataBuff.getDataLen() <= DATA_START )
	{
		return 0;
	}
	unsigned char *data = (unsigned char*)( _dataBuff.getData() );
	return data[4] + data[5] * 256;
}

//��ȡʵ�ʲ����ֵ���
int TERData::getRecordWheelCount()const
{
	return m_recordWheelCount;
}

//���������еĲ����ֵ���
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

//����ʵ�ʲ����ֵ���
void TERData::setRecordWheelCount( int value ){
	m_recordWheelCount = value;
}

//���õ�ѹ
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

//���ò����־���
void TERData::setPrecLen( double vaule )
{
	_precLen = vaule;
}

//��ȡ�����־���
double TERData::getPrecLen()const
{
	return _precLen;
}

//�����Ƿ���Ϊ���
void TERData::setMark( bool value )
{
	_mark = value;
}

//��ȡ�Ƿ���Ϊ���
bool TERData::getMark()const
{
	return _mark;
}

