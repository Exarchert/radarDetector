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

		int m_dRoadPart;//��¼�Զ��ָ��ʱ�ĵڼ���
		//int m_nFinishedPart;//��¼���浽�ڼ���

		void CompleteData( RadarData *rd );//��radarData����gps����Ϣ
		void addData( RadarData *rd, int channelIndex );//����ͨ����Ű������ݷ������Ӧ�߳�
		void addGroupData( std::vector<osg::ref_ptr<RadarData>> radarDataGroup );//�ȰѸ�ͨ��һ���������������������addData
		void AddEachDataForGroup( RadarData *rd, int channelIndex );//����ͨ����Ű������ݷ������Ӧ�߳� û�и���radarData��Ϣ
		void deleteData( int index );//���ڲ����ֻع�ɾ������
		void addSaveCmd(SaveDataCmd *lpCMD );

		void checkRadarData(RadarData *rd);//��������Ƿ��η��ȹ�С

		ReceiveQueue getDataQueue();//�����������

		//��Ŀ����
		void setProjectName( std::string const& name );
		std::string getProjectName()const;

		//��Ŀʱ���
		void setProjectTab( std::string const& tabName );
		std::string getProjectTab()const;

		void setProjectRow( DBRow *lpRow );
		void updateProjectInfo( float curLen );

		void setMark();//�����Ƿ���Ϊ���

		//�״﹤����ʽ����Ϊ˫ͨ�����Ƿ񱣴�oracle���ݿ⣬�������ļ�Ϊ�����·��
		void SetSaveOracleOrFile(int iRadarWorkType,int isaveOracle,CString strPath);

		void writeRadFile();//д����rad
		//void writeRadFileForGroup(int part);
		//void writeRadFileForLastGroup();
		void writeTxtFile();//д����txt
		void writeNameCsvFile();//��¼������־
		void copyRadarData();//�������� 1-12������17-28
		//void copyRadarDataForSevenChannel();//�������� 1-7������8-14
		//void copyRadarDataForPart();//�������� 1-12������17-28
		void copyRadarDataForPart( int part );//��������
		void copyTxtFileForPart( int part );//��������Araayset.txt�ļ�����·���ļ�
		void writeRadFileForPart( int part );//�ڱ���д�����ϴ���rad

		void StartAutoCorrection();
		void addDataToAutoCorrection( RadarData *rd, int channelIndex );//�Զ�΢��
		void AutoCorrection(RadarData *rd );//�Զ�΢��

		//�ϴ�ģʽ�õ���
		//void changeLocalPath();//�л����ش���·��
		//void getOnlinePath();
		void uploadFileToFtp(std::string strLocalPath, std::string strOnlinePath, int part);//�����ϴ�·���ͱ���·�����ϴ�ĳ�����ݣ�����¼�ϴ�������
		void uploadDataForPart(int part);//�ϴ�ĳpart��rd3
		void uploadDataForPartForRepairment(std::string strLocalPath, std::string strOnlinePath, int part);//���ݱ���·�����ϴ�·�������ϴ�ĳ�����ݣ�������part�����¸ò��ֵ��ϴ�������
		void uploadRadFileForGroup( int part );//�ϴ�ĳpart��rad
		void uploadTxtFileForGroup(int part);//�ϴ�ĳpart��txt
		void uploadGpsDataByPost(osg::Vec3d pos);//�ϴ�gps����
		void postEndMessage(int part);//ĳ�ε����������ϴ���Ϻ��ƽ̨ͨѶ�������ָ��
		void addDataToPostFinishedMessageThread(int part);//ĳ�ε����������ϴ���Ϻ��ƽ̨ͨѶ�������ָ��
		//void writeEndFile();
		//void uploadEndFile(int part);

		bool isDataInPartUploaded(int part);

		//void setStartTimeTab();
		void initOprTab();//���벢��ʼ��opr
		void changeTimeTab();//����ʱ���
		void changeFtpFolderPath();//����ftp���ϴ�·��

		//TER
		void SetTERThread( CString strPath );
		void addTERData( TERData *rd );

		OpenThreads::Thread *getAvaliadThread();

	protected:
		FILE *m_fpLog;//log�ļ�
		FILE *m_fpLogForRepairment;//�����ϴ����ֵ�log�ļ�
		int m_nTrueChannelCount;
		int m_nChannelCountForUpload;
		int m_nSaveFileType;//�����ļ�����
		int m_nUpload;//�Ƿ���Ҫ�ϴ�
		//int m_dTwelveFlag;//12ͨ��ģʽ �����ò���
		int m_dSeperateType;//�ֶ�ģʽ 0���ֶ� 1ʱ��ֶ� 2����ֶ�
		int m_nArr2DChannel[8];//���ڼ�¼2ά��������Щͨ��


		std::vector<std::vector<int>> m_nVecVecAutoCorrectionPointIndex;//���ڼ�¼�Զ�΢��ʱ��ͨ����������
		//std::vector<int> m_nVecAutoCorrectionPointIndexCount;//���ڼ�¼�Ѽ��������������
		std::vector<int> m_nVecAutoCorrectionResult;//���ڼ�¼�Զ�΢��ʱ��ͨ���Ľ��

		int m_dWaveWaringInterval;//���ι�С�����������
		int m_dWaveWaringThreshold;//���ι�С��ֵ

		std::string _projectName;//����ȫ��
		std::string _projectTab;//����ʱ��
		
		int m_nInitOpr;//opr���ɳɹ�flag
		std::string m_strStartTimeTab;//��ʼʱ��
		std::string m_strOprTab;//opr��ʶ
		std::string m_strTimeTab;//��ǰʱ��α�ʶ
		std::string m_strFtpFolderPath;//��ǰʹ�õ�ftp�ļ���·��

		std::string m_strTaskCode;//��Ŀ��ʶ
		std::string m_strEquipmentName;//�豸��ʶ

		std::string m_strPostServerIp;//socketͨѶʹ�õ���ַip
		int m_nPostPort;//socketͨѶʹ�õ���ַ�˿�

		int m_nCopyFinishedFlag;

		std::vector<bool> m_vecbUploadConditionForEachPart;//���ڼ�¼�����Ƿ���ȫ�ϴ��ɹ�
		std::vector<std::vector<bool>> m_vecVecUploadSuccessFlag;//���ڼ�¼���εĸ��ļ����ϴ����
		std::vector<std::vector<bool>> m_vecVecProcessFinishedFlag;//���ڼ�¼���εĸ��ļ��Ƿ��Ѿ���������
		std::vector<std::string> m_vecStrTimeTab;//��part�ҹ���ʱ��α�ʶ

		float _lastLen;
		float m_fLastGpsLen;
		float m_fLastPostedGpsLen;
		osg::Vec3d m_pLastPos;
		int m_dGpsTimeStamp;//�ô�gps���ݾ��ϴ���ʱ���
		

		//int _recordTotalWheelCount;//��¼�����ܵ��� 
		//int _realTotalWheelCount;//��¼ʵ���ܵ���
		//int _currentWheelCount;//��¼�����ַ��ص���
		int m_arrnRecordTotalWheelCount[CHANNELCOUNT];//��¼�����ܵ��� 
		int m_arrnRealTotalWheelCount[CHANNELCOUNT];//��¼ʵ���ܵ���
		std::vector<int> m_nVecOriginWheelCountForPart;//��¼�ֶκ�ÿһ����ʼ�������ݵĵڼ���
		//std::vector<int> m_vecLastUploadWheelCount;

		//int _tempWheelCount;
		//int _lastWheelCount;//��¼��һ�ηֶ�ʱ����

		OpenThreads::Mutex _dataMutex;

		ReceiveQueue _dataQueue;

		OpenThreads::Thread *_lpThread;

		osg::ref_ptr<DBRow> _projectRow;//һ������ȫ��������Ϣ

		bool _setMark[CHANNELCOUNT];

		std::string _channelName[CHANNELCOUNT];

		OpenThreads::Mutex _threadMutex;
		typedef std::vector<OpenThreads::Thread*> ThreadVector;
		ThreadVector _threadVector;

		long _maxID;

		bool m_bIsBegin;//�Ƿ�ʼ����
		int m_iRadarWorkType;//������ʽ   ˫ͨ������ͬʱ���䣨U�ڣ� �� ����8ͨ��  �������䣨���ڣ�
		int m_iSaveOracle;//����oracle  0��ֱ�ӱ����ļ������˫ͨ���״�
		CString m_strSavePath;//�����ļ���Ϊ�����·��

	
		OpenThreads::Thread *_lpThreadUploadGps;//ʵʱ�ϴ�gps�����߳�
		OpenThreads::Thread *_lpThreadSaveOnline;//�ֶ��ϴ��ļ��߳�
		OpenThreads::Thread *_lpThreadPostFinishedMessage;//�ֶ��ϴ��ļ��߳�
		OpenThreads::Thread *_lpThreadSaveOnlineForRepairment;//ʧ�ܲ��������ϴ��ļ��߳�
		OpenThreads::Thread *_lpThreadCheckData;//����������͵ľ���ģ��
		OpenThreads::Thread *_lpThreadGroupDataProcess;//��ά�����鴦���߳�
		OpenThreads::Thread *_lpThreadSaveFile[CHANNELCOUNT];//�����ļ��߳�
		OpenThreads::Thread *_lpThreadAutoCorrection;//�Զ�΢���߳�
		OpenThreads::Thread *_lpThreadCopyData;//�ֶ�ģʽ�µĸ�������
		SGYWriter* m_SGYWriter[CHANNELCOUNT];//����SGY�ļ�
	
		bool m_bTERIsBegin;//TER�Ƿ�ʼ����
		CString m_strTERSavePath;//ter���ݵı���·��
		OpenThreads::Thread *m_ThreadTERSaveFile;
		//SaveTERDataThread *m_ThreadTERSaveFile;
		TERDataWriter* m_TERDataWriter;//����TER�ļ�

		//���ڰ��κŸ���·����
		int _documentPathLength;
		int _namePathLength;
		int _onlineDocumentPathLength;
		int _onlineNamePathLength;

		//bool m_bIsAdd;//�Ƿ���������Ҫ����
		int m_nLastSecond;//��һ����������
		int m_nTimeInterval;//������һ��/������һ�з�
		int m_nWheelCountInterval;//���ٵ�һ�з�
		


		char _strLocalRadarDataPath[CHANNELCOUNT][300];//�ϴ����ô���·�� ����rd3
		std::string _strLocalGPSCorPath;//�ϴ����ô���·�� ����cor
		std::string _strLocalGPSCorForUploadPath;//�ϴ����ô���·�� ����corForUpload
		std::string _strLocalGPSCsvPath;//�ϴ����ô���·�� ����csv
		std::string _strLocalGPSKmlPath;//�ϴ����ô���·�� ����kml
		//char _strOnlineRadarDataPath[CHANNELCOUNT][200];//ʵʱ�������ô���·��
		//std::string _strOnlineGPSCorPath;//ʵʱ�������ô���·��
		//std::string _strOnlineGPSCsvPath;//ʵʱ�������ô���·��;
};
