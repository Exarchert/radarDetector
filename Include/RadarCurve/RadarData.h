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

	void setPosition( osg::Vec3d const pos );//设置经纬度值
	osg::Vec3d getPosition()const;//获取经纬度值
	void setGPGGA(std::vector< std::string > vecStrGPGGA);//设置GPGGA
	std::vector< std::string > getGPGGA()const;//获取GPGGA

	void setTime( DWORD value );
	DWORD getTime()const;

	void setData( unsigned char *buff,  int len );//设置雷达数据
	void setSampleCount( int value );//设置采样点数
	void setSampleRatio( float value );//设置采样率
	void setLen( float value );//设置gps距离
	void setMark( bool value );//设置是否作为标记

	unsigned char *getData( int &len )const;//获取雷达数据
	int getSampleCount()const;//获取采样点数
	float getSampleRatio()const;//获取采样率
	float getLen()const;//获取gps距离
	bool getMark()const;//获取是否作为标记


	short getIndexData( int index, float *t );//按索引取雷达数据值

	int getDataCount()const;//获取采样点数-4的值

	int getChannelIndex()const;//获取通道序数

	void setPrec( int value );//设置测量轮道数
	int getPrec()const;//返回测量轮道数
	void setRealWheelCount( int value );//设置统计的测量轮道数
	int getRealWheelCount()const;//返回统计的测量轮道数
	
	void setPrecLen( double vaule );//设置测量轮距离
	double getPrecLen()const;//获取测量轮距离

	unsigned short getDCValue()const;//获取电压

	void SetBufLength(unsigned int len ){_dataBuff.setBuffLen(len);}
	void AddDataToEnd(unsigned char* cp,unsigned int len){_dataBuff.AddDataToEnd(cp,len);}//添加数据
	void AddDataToEnd(char* cp,unsigned int len){_dataBuff.AddDataToEnd((unsigned char*)cp,len);}//添加数据

protected:
	CharDataBuf _dataBuff;
	osg::Vec3d _position;
	DWORD _receiveTime;

	std::vector< std::string > m_vecStrGPGGA;//用来记录gpgga

	int _realWheelCount;//实际统计测量轮道数

	int _sampleCount;//采样点数

	float _sampleRatio;//采样率

	float _len;//gps距离

	double _precLen;//测量轮距离 

	bool _mark;
};
