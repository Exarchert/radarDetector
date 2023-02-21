#pragma once
#include <RadarCurve/export.h>
#include <RadarCurve/DataBuff.h>
#include <RadarCurve/SerialPort.h>
#include <RadarCurve/GpsData.h>
#include <vector>
#include <string>
#include <OpenThreads/Mutex>
#include <time.h>
#include <Object/Obj.h>

#include <osg/observer_ptr>

class RADAR_EXPORT GpsReader
{
public:
	GpsReader(void);
	~GpsReader(void);

	bool open( int portNum = 1, int baud = 19200, char parity = 'N', int databits = 8, int stopbits = 0);
	bool close();

	void onReadData( unsigned char *lpData, int len );//添加数据

	void getCurrentGpsImformation(osg::Vec3d &postion, std::vector< std::string > &GPGGA);//获取录入的最新的经纬度和GPGGA
	osg::Vec3d getCurPos();//获取录入的最新的经纬度
	float getCurLen();//获取长度
	int getCondition();//获取最新的GPS状态
	std::vector< std::string > getCurrentGpsDataBuff();//获取录入的最新的复合要求的GPGGA
	std::vector< std::string > getCurrentRawGpsDataBuff();//获取接收到的最新的GPGGA
	void parseData();
protected:

	void parseGNGST( std::vector< std::string > valueList );
	void parseGPGSV( std::vector< std::string > valueList );
	void parseGBDGSV( std::vector< std::string > valueList );
	void parseGNGSA( std::vector< std::string > valueList );
	void parseGPRMC( std::vector< std::string > valueList );
	void parseGPGGA( std::vector< std::string > valueList );//按照gpgga的格式解译数据
	void parseGNGGA( std::vector< std::string > valueList );

protected:

	OpenThreads::Mutex _mutex;
	CSerialPort _seriaPort;
	CharDataBuf _dataBuff;//接收到的gps信息

	GpsData _lastData;//录入的上一条数据
	GpsData _curData;//录入的最新的数据
	
	float m_fLastTime;//录入的上一条数据的时间
	float m_fCurrentTime;//录入的最新的数据的时间

	time_t _lastTime;

	int _iCondition;//当前的GPS状态
	int m_nLastCondition;//上次的GPS状态
	
	std::vector< std::string > m_vecStrCurrentGPGGA;//录入的复合要求的GPGGA数据
	std::vector< std::string > _currentRawGpsDataBuff;//接收到的所有GPGGA数据

	float _len;//总长

	bool _first;//用来判断是否是第一个点
	bool _second;//用来判断是否是第二个点

	int m_nGpsUpdateFailCount;//记录由于超出推断范围而录入失败的次数

	float m_fGpsSpan;//gps的频率 一秒传多少次

	osg::observer_ptr<EMObj::CObj> _obj;//指针
};
