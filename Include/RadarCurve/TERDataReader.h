#pragma once
#include <RadarCurve/export.h>
#include <OpenThreads/Thread>
#include <RadarCurve/RadarSocketClient.h>
#include <RadarCurve/DataBuff.h>
#include <string>
#include <vector>
#include "../../RadarCurve/GD_UPortClient.h"

#define MAX_CHANNEL 16


class RadarData;
class RADAR_EXPORT TERDataReader
{
public:
	TERDataReader(void);
	~TERDataReader(void);

	bool openForSetting( std::string const& targetIP, unsigned int port );
	bool open( std::string const& targetIP, unsigned int port );
	bool open(int iType=2);
	bool close();

	int readData();
	void parseData();
	void parseDataToDisplay();
	void ProcessData(unsigned char *lpDataBuff);

	void sendData( unsigned char *buff, int len );

	void setInit(bool value);
	void init();
	void initU();
	void setParam( unsigned char addr, unsigned char value );

	bool isOpen();

	bool ReadUData();

	void parseSixteenData();

	void ReadThreadStart(bool flag = true);

	void SocketConnect( std::string const& targetIP, unsigned int port );
	void SocketDisconnect();
	bool SetSenderFree();
	bool SetSenderParameter();
	bool SetSenderReady();
	bool SetSenderStart();
	bool SetSenderStop();
	bool SetReceiverFree();
	bool SetReceiverParameter();
	bool SetReceiverReady();
	bool SetReceiverStart();
	bool SetReceiverStop();
protected:
	void parseHeadData( char *buff, int len );
	bool SetChannelLen(RadarData* _radar , int index);
	//bool SetChannelLenNew(RadarData* _radar , int index , int &flag);

protected:
	OpenThreads::Thread *_lpTERReceiveThread;
	OpenThreads::Thread *_lpTERProcessThread;
	
	CRadarSocketClient _client;
	bool m_bIsConnected;

	unsigned char *_recvBuff;

	CharDataBuf _dataBuf;

	int m_nSampleCount;
	int m_nSampleRatioIndex;
	int m_nTERDataCount;
	float m_fSettingInterval;//单位是厘米
	float m_fTrueInterval;//单位是米


	int _sampleCount;
	float _sampleRatio;
	int _channelCount;

	int m_iModeType;//是否测量轮
	bool _hadInit;

	int _channelWheelCount[MAX_CHANNEL];//记录传上来的道数
	int _recordChannelWheelCount[MAX_CHANNEL];//储存进sgy的道数  传上来的不一定从零开始的 用这个实现从零开始
	double _channelLen[MAX_CHANNEL];

	unsigned char *_latestChannelRadarData[MAX_CHANNEL];//20210711 hjl 储存最新的一次的雷达数据 供数据拟合用

	double _precValue; 

	GD_UPortClient * m_uPort;
	UCHAR m_ubuf[2048] ;
	long m_lLen;

	int m_iRadarWorkType;

	OpenThreads::Mutex _lock;

	int m_iReadNULLTime;

	bool m_bIsDataOne;
	std::vector<float> m_vecfDataOne;//返回的两段数据中的第一段
	std::vector<float> m_vecfDataTwo;//返回的两段数据中的第二段

};
