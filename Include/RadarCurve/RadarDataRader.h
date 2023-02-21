#pragma once
#include <RadarCurve/export.h>
#include <OpenThreads/Thread>
#include <RadarCurve/RadarSocketClient.h>
#include <RadarCurve/DataBuff.h>
#include <string>
#include "../../RadarCurve/GD_UPortClient.h"

#define MAX_CHANNEL 16


class RadarData;
class RADAR_EXPORT RadarDataRader
{
public:
	RadarDataRader(void);
	~RadarDataRader(void);

	bool openForAutoCorrecting( std::string const& targetIP, unsigned int port );
	bool openForSetting( std::string const& targetIP, unsigned int port );
	bool open( std::string const& targetIP, unsigned int port );
	bool open(int iType=2);
	bool close();

	int readData();
	void parseData();
	void parseDataToDisplay();
	void parseDataToAutoCorrection();

	void sendData( unsigned char *buff, int len );


	void setInit(bool value);
	void init();
	void initU();
	void setParam( unsigned char addr, unsigned char value );

	bool isOpen();

	bool ReadUData();
	void ParseUDataOneUSB();
	void ParseUsbTwoChannelData();
	void ParseUsbTwoChannelDataOld();
	void ParseUsbFourChannelData();
	void parseSixteenData();

	void ReadThreadStart(bool flag = true);

	//void MarkOne();
	//void Mark(int numberOne, int numberTwo);
	void Mark(int markValue);

	//�������� 20210711hjl
	void artificializeRadarData(int originalIndex);

protected:
	void parseHeadData( char *buff, int len );
	bool SetChannelLen(RadarData* _radar , int index);
	//bool SetChannelLenNew(RadarData* _radar , int index , int &flag);

protected:
	OpenThreads::Thread *_lpReadThread;
	CRadarSocketClient _client;

	int m_nChannelCount;

	unsigned char *_recvBuff;

	CharDataBuf _dataBuf;

	int _sampleCount;
	float _sampleRatio;
	int _channelCount;

	bool m_bNeed3DDisplay;
	bool m_bNeedCopyInPreview;

	int m_iModeType;//�Ƿ������
	bool _hadInit;

	int _channelWheelCount[MAX_CHANNEL];//��¼�������ĵ���
	int _recordChannelWheelCount[MAX_CHANNEL];//�����sgy�ĵ���  �������Ĳ�һ�����㿪ʼ�� �����ʵ�ִ��㿪ʼ
	double _channelLen[MAX_CHANNEL];

	unsigned char *_latestChannelRadarData[MAX_CHANNEL];//20210711 hjl �������µ�һ�ε��״����� �����������

	double _precValue; 

	GD_UPortClient * m_uPort;
	UCHAR m_ubuf[2048] ;
	long m_lLen;

	int m_iRadarWorkType;

	OpenThreads::Mutex _lock;

	int m_iReadNULLTime;


};
