#pragma once
#include <RadarCurve/export.h>
#include <osg/Referenced>
#include <RadarCurve/RadarData.h>
#include <RadarCurve/TERData.h>
#include <RadarCurve/GpsReader.h>
#include <OpenThreads/Mutex>
#include <queue>
#include <string>
#include <OpenThreads/Thread>
#include <RadarCurve/DBRow.h>
#include <osg/ref_ptr>

#define MAX_CHANNEL 16
#define TWO_CHANNEL 2
#define FOUR_CHANNEL 4
#define CHANNELCOUNT 28

class TERDataWriter;
class SGYWriter;
class SaveTERDataThread;
class SaveFileDataThread;
class SaveOnlineThread;

class SaveDataCmd : public osg::Referenced{
	public:
		SaveDataCmd(){};
		virtual void exec( std::string const& projectTab ) = 0;
		virtual void exec( SGYWriter* ){};
		virtual void exec( TERDataWriter* ){};
};


class RADAR_EXPORT MeasureProject : public osg::Referenced{
	public:
		typedef std::queue<RadarData> ReceiveQueue;
		MeasureProject(void);
		~MeasureProject(void);

		int m_dRoadPart;//记录自动分割储存时的第几道
		//int m_nFinishedPart;//记录保存到第几道

		void CompleteData( RadarData *rd );//给radarData赋上gps等信息
		void addData( RadarData *rd, int channelIndex );//按照通道序号把新数据分配进相应线程
		void addGroupData( std::vector<osg::ref_ptr<RadarData>> radarDataGroup );//先把各通道一组数据完整接收完再逐个addData
		void AddEachDataForGroup( RadarData *rd, int channelIndex );//按照通道序号把新数据分配进相应线程 没有更新radarData信息
		void deleteData( int index );//用于测量轮回滚删除数据
		void addSaveCmd(SaveDataCmd *lpCMD );

		void checkRadarData(RadarData *rd);//检查数据是否波形幅度过小

		ReceiveQueue getDataQueue();//返回任务队列

		//项目名称
		void setProjectName( std::string const& name );
		std::string getProjectName()const;

		//项目时间戳
		void setProjectTab( std::string const& tabName );
		std::string getProjectTab()const;

		void setProjectRow( DBRow *lpRow );
		void updateProjectInfo( float curLen );

		void setMark();//设置是否置为标记

		//雷达工作方式，若为双通道，是否保存oracle数据库，若保存文件为保存的路径
		void SetSaveOracleOrFile(int iRadarWorkType,int isaveOracle,CString strPath);

		void writeRadFile();//写本地rad
		//void writeRadFileForGroup(int part);
		//void writeRadFileForLastGroup();
		void writeTxtFile();//写本地txt
		void writeNameCsvFile();//记录工作日志
		void copyRadarData();//复制数据 1-12复制至17-28
		//void copyRadarDataForSevenChannel();//复制数据 1-7复制至8-14
		//void copyRadarDataForPart();//复制数据 1-12复制至17-28
		void copyRadarDataForPart( int part );//复制数据
		void copyTxtFileForPart( int part );//拷贝本地Araayset.txt文件至各路段文件
		void writeRadFileForPart( int part );//在本地写用于上传的rad

		void StartAutoCorrection();
		void addDataToAutoCorrection( RadarData *rd, int channelIndex );//自动微调
		void AutoCorrection(RadarData *rd );//自动微调

		//上传模式用到的
		//void changeLocalPath();//切换本地储存路径
		//void getOnlinePath();
		void uploadFileToFtp(std::string strLocalPath, std::string strOnlinePath, int part);//根据上传路径和本地路径来上传某个数据，并记录上传完成情况
		void uploadDataForPart(int part);//上传某part的rd3
		void uploadDataForPartForRepairment(std::string strLocalPath, std::string strOnlinePath, int part);//根据本地路径、上传路径重新上传某个数据，并根据part来更新该部分的上传完成情况
		void uploadRadFileForGroup( int part );//上传某part的rad
		void uploadTxtFileForGroup(int part);//上传某part的txt
		void uploadGpsDataByPost(osg::Vec3d pos);//上传gps数据
		void postEndMessage(int part);//某段的所有数据上传完毕后跟平台通讯发送完成指令
		void addDataToPostFinishedMessageThread(int part);//某段的所有数据上传完毕后跟平台通讯发送完成指令
		//void writeEndFile();
		//void uploadEndFile(int part);

		bool isDataInPartUploaded(int part);

		//void setStartTimeTab();
		void initOprTab();//申请并初始化opr
		void changeTimeTab();//更换时间戳
		void changeFtpFolderPath();//更换ftp端上传路径

		//TER
		void SetTERThread( CString strPath );
		void addTERData( TERData *rd );

		OpenThreads::Thread *getAvaliadThread();

	protected:
		FILE *m_fpLog;//log文件
		FILE *m_fpLogForRepairment;//重新上传部分的log文件
		int m_nTrueChannelCount;
		int m_nChannelCountForUpload;
		int m_nSaveFileType;//储存文件类型
		int m_nUpload;//是否需要上传
		//int m_dTwelveFlag;//12通道模式 基本用不上
		int m_dSeperateType;//分段模式 0不分段 1时间分段 2距离分段
		int m_nArr2DChannel[8];//用于记录2维主机用哪些通道


		std::vector<std::vector<int>> m_nVecVecAutoCorrectionPointIndex;//用于记录自动微调时各通道的起跳点
		//std::vector<int> m_nVecAutoCorrectionPointIndexCount;//用于记录已计算的起跳点数量
		std::vector<int> m_nVecAutoCorrectionResult;//用于记录自动微调时各通道的结果

		int m_dWaveWaringInterval;//波形过小检测间隔（道）
		int m_dWaveWaringThreshold;//波形过小阈值

		std::string _projectName;//工程全称
		std::string _projectTab;//工程时间
		
		int m_nInitOpr;//opr生成成功flag
		std::string m_strStartTimeTab;//开始时间
		std::string m_strOprTab;//opr标识
		std::string m_strTimeTab;//当前时间段标识
		std::string m_strFtpFolderPath;//当前使用的ftp文件夹路径

		std::string m_strTaskCode;//项目标识
		std::string m_strEquipmentName;//设备标识

		std::string m_strPostServerIp;//socket通讯使用的网址ip
		int m_nPostPort;//socket通讯使用的网址端口

		int m_nCopyFinishedFlag;

		std::vector<bool> m_vecbUploadConditionForEachPart;//用于记录各段是否完全上传成功
		std::vector<std::vector<bool>> m_vecVecUploadSuccessFlag;//用于记录各段的各文件的上传情况
		std::vector<std::vector<bool>> m_vecVecProcessFinishedFlag;//用于记录各段的各文件是否已经走完流程
		std::vector<std::string> m_vecStrTimeTab;//跟part挂钩的时间段标识

		float _lastLen;
		float m_fLastGpsLen;
		float m_fLastPostedGpsLen;
		osg::Vec3d m_pLastPos;
		int m_dGpsTimeStamp;//该次gps数据局上传的时间戳
		

		//int _recordTotalWheelCount;//记录经过总道数 
		//int _realTotalWheelCount;//记录实际总道数
		//int _currentWheelCount;//记录测量轮返回道数
		int m_arrnRecordTotalWheelCount[CHANNELCOUNT];//记录经过总道数 
		int m_arrnRealTotalWheelCount[CHANNELCOUNT];//记录实际总道数
		std::vector<int> m_nVecOriginWheelCountForPart;//记录分段后每一段起始是总数据的第几道
		//std::vector<int> m_vecLastUploadWheelCount;

		//int _tempWheelCount;
		//int _lastWheelCount;//记录上一次分段时道数

		OpenThreads::Mutex _dataMutex;

		ReceiveQueue _dataQueue;

		OpenThreads::Thread *_lpThread;

		osg::ref_ptr<DBRow> _projectRow;//一个工程全部属性信息

		bool _setMark[CHANNELCOUNT];

		std::string _channelName[CHANNELCOUNT];

		OpenThreads::Mutex _threadMutex;
		typedef std::vector<OpenThreads::Thread*> ThreadVector;
		ThreadVector _threadVector;

		long _maxID;

		bool m_bIsBegin;//是否开始工作
		int m_iRadarWorkType;//工作方式   双通道数据同时传输（U口） ， 还是8通道  单个传输（网口）
		int m_iSaveOracle;//保存oracle  0是直接保存文件，针对双通道雷达
		CString m_strSavePath;//保存文件，为保存的路径

	
		OpenThreads::Thread *_lpThreadUploadGps;//实时上传gps数据线程
		OpenThreads::Thread *_lpThreadSaveOnline;//分段上传文件线程
		OpenThreads::Thread *_lpThreadPostFinishedMessage;//分段上传文件线程
		OpenThreads::Thread *_lpThreadSaveOnlineForRepairment;//失败部分重新上传文件线程
		OpenThreads::Thread *_lpThreadCheckData;//波形起伏过低的警告模块
		OpenThreads::Thread *_lpThreadGroupDataProcess;//三维数据组处理线程
		OpenThreads::Thread *_lpThreadSaveFile[CHANNELCOUNT];//保存文件线程
		OpenThreads::Thread *_lpThreadAutoCorrection;//自动微调线程
		OpenThreads::Thread *_lpThreadCopyData;//分段模式下的复制数据
		SGYWriter* m_SGYWriter[CHANNELCOUNT];//保存SGY文件
	
		bool m_bTERIsBegin;//TER是否开始工作
		CString m_strTERSavePath;//ter数据的保存路径
		OpenThreads::Thread *m_ThreadTERSaveFile;
		//SaveTERDataThread *m_ThreadTERSaveFile;
		TERDataWriter* m_TERDataWriter;//保存TER文件

		//用于按段号更新路径名
		int _documentPathLength;
		int _namePathLength;
		int _onlineDocumentPathLength;
		int _onlineNamePathLength;

		//bool m_bIsAdd;//是否有任务需要增添
		int m_nLastSecond;//上一段最后的秒数
		int m_nTimeInterval;//多少秒一传/多少秒一切分
		int m_nWheelCountInterval;//多少道一切分
		


		char _strLocalRadarDataPath[CHANNELCOUNT][300];//上传所用储存路径 本地rd3
		std::string _strLocalGPSCorPath;//上传所用储存路径 本地cor
		std::string _strLocalGPSCorForUploadPath;//上传所用储存路径 本地corForUpload
		std::string _strLocalGPSCsvPath;//上传所用储存路径 本地csv
		std::string _strLocalGPSKmlPath;//上传所用储存路径 本地kml
		//char _strOnlineRadarDataPath[CHANNELCOUNT][200];//实时传输所用储存路径
		//std::string _strOnlineGPSCorPath;//实时传输所用储存路径
		//std::string _strOnlineGPSCsvPath;//实时传输所用储存路径;
};
