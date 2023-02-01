#pragma once
#include <RadarCurve/RadarData.h>
#include <string>
#include <fstream>
#include <RadarCurve/ConfigureSet.h>

class SGYWriter
{
public:
	SGYWriter(void);
	~SGYWriter(void);

	bool openSGY( std::string const& filePath, std::string const &paramXML, int trackNo );
	bool openSGYForAutomation( std::string const& filePath, std::string const &paramXML, int trackNo, int documentPathLength, int namePathLength );
	bool closeSGY();
	bool changeSaveFile();//����д��·��

	bool openGPS( std::string const& filePath );
	bool openGPS( std::string const& corFilePath, std::string const& csvFilePath );
	bool openGPS( std::string const& corFilePath, std::string const& corForUploadFilePath, std::string const& csvFilePath );
	bool closeGPS();
	bool writeGpsFlag;//�Ƿ�дgps�ļ�
	bool writeGpsCsvFlag;//�Ƿ�дgps�ļ�
	bool writeGpsCorFlag;//�Ƿ�дgps�ļ�

	bool openKml( std::string const& filePath );
	bool closeKml();
	bool writeKmlFlag;//�Ƿ�дkml�ļ�

	void writeHead( int trackNum,std::string _projectNameTime );//д��sgy���ļ�ͷ
	void writeRadarData( RadarData *lpData );//д���״�����
	//void writeRadarDataForAutomation( RadarData *lpData );//�Զ�ģʽ�µ�д���״�����
	void writeGPSData( RadarData *lpData );//д��gps����
	void writeKmlHead();//д��kml��ͷ����
	void writeKml( RadarData *lpData );//д��kml
	void writeKmlEnding();//д��kml��β

	void writeDisLLPic(std::string videoTabSerialNum, RadarData *lpRadarData,std::ofstream& infile);
	void writeDisLL(RadarData *lpRadarData,std::ofstream& infile, float& pOldDis,const float pInterval);

	void setDataCountThreshold(int value);//�����ϴ���ֵ
	void setNeedToChange(bool value);//�����Ƿ���Ҫ��������·��
	void setUploadFlag(int value);//�����Ƿ��ϴ�
	void setSeperateType(int value);//���÷ֶ����� 0���ֶ� 1ʱ��ֶ� 2����ֶ�
	void setSaveFileType(int value);//���ô����ļ�����
	void setPathElement(std::string path, std::string projectName, std::string projectTab);//����·������Ԫ��
	//void uploadData()
protected:
	//double getTotalLen( int iTracNo, int wheelCount );
protected:
	FILE *_lpRadarDataFile;//�����ļ�
	FILE *_lpGPSFile;//sgyģʽ�µ�gps�ļ�
	FILE *_lpGPSCorFile;//cor
	FILE *_lpGPSRawFile;//��¼ԭʼGPGGA��raw
	FILE *_lpGPSCorFileForUpload;//�����ϴ���cor
	FILE *_lpGPSCsvFile;//��¼gps��Ϣ��csv
	FILE *_lpKmlFile;//kml

	int m_nSaveFileType;//�����ļ����� 0;Sgy  1:rd3
	int m_nUpload;//�Ƿ��ϴ�
	int m_nSeperateType;//�ֶ����� 0�����ϴ� 1��ʱ��ֶ� 2������ֶ�
 
	std::string _originalSGYPath;//��һ������·��
	std::string _strTimeTab;//ʱ���

	int _documentPathLength;//�ļ���·������
	int _namePathLength;//�ļ����Ƴ���
	std::string m_strPath;
	std::string m_strProjectName;
	std::string m_strProjectTab;

	int m_dRoadPart;//�Զ���������·�����ɵ�·�����
	int m_dLastRecordedDataCount;//��һ·�ε�������
	int m_dStartDataCount;//��һ��
	
	int m_dDataCount;//д�����������
	int m_dDataCountThreshold;//�ϴ���ֵ �������ֵ����һ���ļ����� ��д�õ�����ϴ�
	bool m_bNeedToChangePath;//�����������趨����ֵ����Ҫ��������·��
	bool m_bHasChangedPath;//�Ѿ���������·��
	bool m_bNeedToSaveAsFirstKml;//�Ѿ���������·��


	ConfigureSet _set;//cfg

	double _dTimeWnd;//ʱ��
	float _dielectric;//��糣��
	
	int _wheelNum[8];

	double _precValue;
	int _samNum;//��������
	float _sampRate;//������

	int _markNo;

	double _totalLen;//д�����ݵľ��볤��

	int _iTrackNo;//ͨ�����

	//int _indexOfRecordedGpsData;//��֪��

	float _dLastDistanceGps;//gps��һ�μ�¼�ľ���
	float _dLastDistanceKml;//kml��һ�μ�¼�ľ���
	osg::Vec3d m_pLastPos;
	std::vector< std::string > m_vecStrLastGPGGA;//��һ����¼�����ݵ�GPGGA
	float m_fLastRecordedGpsDistance;//��һ����¼�����ݵ�GPS����

	int m_dCorrection;//�ӳ�΢��ֵ
};
