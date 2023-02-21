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

	void onReadData( unsigned char *lpData, int len );//�������

	void getCurrentGpsImformation(osg::Vec3d &postion, std::vector< std::string > &GPGGA);//��ȡ¼������µľ�γ�Ⱥ�GPGGA
	osg::Vec3d getCurPos();//��ȡ¼������µľ�γ��
	float getCurLen();//��ȡ����
	int getCondition();//��ȡ���µ�GPS״̬
	std::vector< std::string > getCurrentGpsDataBuff();//��ȡ¼������µĸ���Ҫ���GPGGA
	std::vector< std::string > getCurrentRawGpsDataBuff();//��ȡ���յ������µ�GPGGA
	void parseData();
protected:

	void parseGNGST( std::vector< std::string > valueList );
	void parseGPGSV( std::vector< std::string > valueList );
	void parseGBDGSV( std::vector< std::string > valueList );
	void parseGNGSA( std::vector< std::string > valueList );
	void parseGPRMC( std::vector< std::string > valueList );
	void parseGPGGA( std::vector< std::string > valueList );//����gpgga�ĸ�ʽ��������
	void parseGNGGA( std::vector< std::string > valueList );

protected:

	OpenThreads::Mutex _mutex;
	CSerialPort _seriaPort;
	CharDataBuf _dataBuff;//���յ���gps��Ϣ

	GpsData _lastData;//¼�����һ������
	GpsData _curData;//¼������µ�����
	
	float m_fLastTime;//¼�����һ�����ݵ�ʱ��
	float m_fCurrentTime;//¼������µ����ݵ�ʱ��

	time_t _lastTime;

	int _iCondition;//��ǰ��GPS״̬
	int m_nLastCondition;//�ϴε�GPS״̬
	
	std::vector< std::string > m_vecStrCurrentGPGGA;//¼��ĸ���Ҫ���GPGGA����
	std::vector< std::string > _currentRawGpsDataBuff;//���յ�������GPGGA����

	float _len;//�ܳ�

	bool _first;//�����ж��Ƿ��ǵ�һ����
	bool _second;//�����ж��Ƿ��ǵڶ�����

	int m_nGpsUpdateFailCount;//��¼���ڳ����ƶϷ�Χ��¼��ʧ�ܵĴ���

	float m_fGpsSpan;//gps��Ƶ�� һ�봫���ٴ�

	osg::observer_ptr<EMObj::CObj> _obj;//ָ��
};
