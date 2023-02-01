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
	bool changeSaveFile();//更换写入路径

	bool openGPS( std::string const& filePath );
	bool openGPS( std::string const& corFilePath, std::string const& csvFilePath );
	bool openGPS( std::string const& corFilePath, std::string const& corForUploadFilePath, std::string const& csvFilePath );
	bool closeGPS();
	bool writeGpsFlag;//是否写gps文件
	bool writeGpsCsvFlag;//是否写gps文件
	bool writeGpsCorFlag;//是否写gps文件

	bool openKml( std::string const& filePath );
	bool closeKml();
	bool writeKmlFlag;//是否写kml文件

	void writeHead( int trackNum,std::string _projectNameTime );//写入sgy的文件头
	void writeRadarData( RadarData *lpData );//写入雷达数据
	//void writeRadarDataForAutomation( RadarData *lpData );//自动模式下的写入雷达数据
	void writeGPSData( RadarData *lpData );//写入gps数据
	void writeKmlHead();//写入kml开头部分
	void writeKml( RadarData *lpData );//写入kml
	void writeKmlEnding();//写入kml结尾

	void writeDisLLPic(std::string videoTabSerialNum, RadarData *lpRadarData,std::ofstream& infile);
	void writeDisLL(RadarData *lpRadarData,std::ofstream& infile, float& pOldDis,const float pInterval);

	void setDataCountThreshold(int value);//设置上传阈值
	void setNeedToChange(bool value);//设置是否需要更换保存路径
	void setUploadFlag(int value);//设置是否上传
	void setSeperateType(int value);//设置分段类型 0不分段 1时间分段 2距离分段
	void setSaveFileType(int value);//设置储存文件类型
	void setPathElement(std::string path, std::string projectName, std::string projectTab);//设置路径生成元素
	//void uploadData()
protected:
	//double getTotalLen( int iTracNo, int wheelCount );
protected:
	FILE *_lpRadarDataFile;//数据文件
	FILE *_lpGPSFile;//sgy模式下的gps文件
	FILE *_lpGPSCorFile;//cor
	FILE *_lpGPSRawFile;//记录原始GPGGA的raw
	FILE *_lpGPSCorFileForUpload;//用于上传的cor
	FILE *_lpGPSCsvFile;//记录gps信息的csv
	FILE *_lpKmlFile;//kml

	int m_nSaveFileType;//保存文件类型 0;Sgy  1:rd3
	int m_nUpload;//是否上传
	int m_nSeperateType;//分段类型 0：不上传 1：时间分段 2：距离分段
 
	std::string _originalSGYPath;//第一个保存路径
	std::string _strTimeTab;//时间戳

	int _documentPathLength;//文件夹路径长度
	int _namePathLength;//文件名称长度
	std::string m_strPath;
	std::string m_strProjectName;
	std::string m_strProjectTab;

	int m_dRoadPart;//自动更换保存路径生成的路段序号
	int m_dLastRecordedDataCount;//上一路段的数据量
	int m_dStartDataCount;//上一道
	
	int m_dDataCount;//写入的数据总量
	int m_dDataCountThreshold;//上传阈值 超过这个值就另开一个文件储存 把写好的这个上传
	bool m_bNeedToChangePath;//数据量到了设定的阈值，需要更换保存路径
	bool m_bHasChangedPath;//已经更换保存路径
	bool m_bNeedToSaveAsFirstKml;//已经更换保存路径


	ConfigureSet _set;//cfg

	double _dTimeWnd;//时窗
	float _dielectric;//介电常数
	
	int _wheelNum[8];

	double _precValue;
	int _samNum;//采样点数
	float _sampRate;//采样率

	int _markNo;

	double _totalLen;//写入数据的距离长度

	int _iTrackNo;//通道序号

	//int _indexOfRecordedGpsData;//不知道

	float _dLastDistanceGps;//gps上一次记录的距离
	float _dLastDistanceKml;//kml上一次记录的距离
	osg::Vec3d m_pLastPos;
	std::vector< std::string > m_vecStrLastGPGGA;//上一个记录的数据的GPGGA
	float m_fLastRecordedGpsDistance;//上一个记录的数据的GPS距离

	int m_dCorrection;//延迟微调值
};
