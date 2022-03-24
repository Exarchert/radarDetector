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
	bool changeSaveFile();

	bool openGPS( std::string const& corFilePath, std::string const& corForUploadFilePath, std::string const& csvFilePath );
	bool closeGPS();
	bool writeGpsFlag;

	bool openKml( std::string const& filePath );
	bool closeKml();
	bool writeKmlFlag;

	void writeHead( int trackNum,std::string _projectNameTime );
	void writeRadarData( RadarData *lpData );
	void writeRadarDataForAutomation( RadarData *lpData );
	void writeGPSData( RadarData *lpData );
	void writeKmlHead();
	void writeKml( RadarData *lpData );
	void writeKmlEnding();

	void writeDisLLPic(std::string videoTabSerialNum, RadarData *lpRadarData,std::ofstream& infile);
	void writeDisLL(RadarData *lpRadarData,std::ofstream& infile, float& pOldDis,const float pInterval);

	void setDataCountThreshold(int value);
	void setIsChange(bool value);
	void setUploadFlag(int value);
	//void uploadData()
protected:
	double getTotalLen( int iTracNo, int wheelCount );
protected:
	FILE *_lpFile;
	FILE *_lpGPSCorFile;
	FILE *_lpGPSRawFile;
	FILE *_lpGPSCorFileForUpload;
	FILE *_lpGPSCsvFile;
	FILE *_lpKmlFile;

	std::string _originalSGYPath;
	std::string _strTimeTab;

	int _documentPathLength;
	int _namePathLength;

	int m_nRoadPart;
	int m_dLastRecordedDataCount;
	
	int _dataCount;
	int m_nUpload;
	int m_nDataCountThreshold;//几道一传
	bool m_bIsChange;
	bool m_bIsChangedJustNow;

	ConfigureSet _set;

	double _dTimeWnd;
	float _dielectric;
	int _samNum;

	int _wheelNum[8];


	double _precValue;
	float _sampRate;

	int _markNo;

	double _totalLen;

	int _iTrackNo;

	int _indexOfRecordedGpsData;

	float _dLastDistanceGps;
	float _dLastDistanceKml;
	osg::Vec3d m_pLastPos;
	std::vector< std::string > m_vecStrLastGPGGA;
	float m_fLastRecordedGpsDistance;

	int _dCorrection;
};
