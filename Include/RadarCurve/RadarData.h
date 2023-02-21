#pragma once
#include <RadarCurve/export.h>
#include <osg/Vec3d>
#include <RadarCurve/GpsData.h>
#include <time.h>
#include <RadarCurve/DataBuff.h>
#include <RadarCurve/DBRow.h>
#include <vector>
#include <string.h>


class RADAR_EXPORT RadarData : public DBRow
{
public:
	RadarData(void);
	~RadarData(void);

	void setPosition( osg::Vec3d const pos );//���þ�γ��ֵ
	osg::Vec3d getPosition()const;//��ȡ��γ��ֵ
	void setGPGGA(std::vector< std::string > vecStrGPGGA);//����GPGGA
	std::vector< std::string > getGPGGA()const;//��ȡGPGGA

	void setTime( DWORD value );
	DWORD getTime()const;

	void setData( unsigned char *buff,  int len );//�����״�����
	void setSampleCount( int value );//���ò�������
	void setSampleRatio( float value );//���ò�����
	void setLen( float value );//����gps����
	void setMark( bool value );//�����Ƿ���Ϊ���

	unsigned char *getData( int &len )const;//��ȡ�״�����
	int getSampleCount()const;//��ȡ��������
	float getSampleRatio()const;//��ȡ������
	float getLen()const;//��ȡgps����
	bool getMark()const;//��ȡ�Ƿ���Ϊ���


	short getIndexData( int index, float *t );//������ȡ�״�����ֵ

	int getDataCount()const;//��ȡ��������-4��ֵ

	int getChannelIndex()const;//��ȡͨ������

	void setPrec( int value );//���ò����ֵ���
	int getPrec()const;//���ز����ֵ���
	void setRealWheelCount( int value );//����ͳ�ƵĲ����ֵ���
	int getRealWheelCount()const;//����ͳ�ƵĲ����ֵ���
	
	void setPrecLen( double vaule );//���ò����־���
	double getPrecLen()const;//��ȡ�����־���

	unsigned short getDCValue()const;//��ȡ��ѹ

	void SetBufLength(unsigned int len ){_dataBuff.setBuffLen(len);}
	void AddDataToEnd(unsigned char* cp,unsigned int len){_dataBuff.AddDataToEnd(cp,len);}//�������
	void AddDataToEnd(char* cp,unsigned int len){_dataBuff.AddDataToEnd((unsigned char*)cp,len);}//�������

protected:
	CharDataBuf _dataBuff;
	osg::Vec3d _position;
	DWORD _receiveTime;

	std::vector< std::string > m_vecStrGPGGA;//������¼gpgga

	int _realWheelCount;//ʵ��ͳ�Ʋ����ֵ���

	int _sampleCount;//��������

	float _sampleRatio;//������

	float _len;//gps����

	double _precLen;//�����־��� 

	bool _mark;
};
