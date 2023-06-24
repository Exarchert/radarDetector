#pragma once
#include <RadarCurve/export.h>
#include <RadarCurve/singleton.h>
#include <RadarCurve/RadarDataRader.h>
#include <RadarCurve/TERDataReader.h>
#include <RadarCurve/GpsReader.h>
#include <RadarCurve/MeasureProject.h>
#include <string>
#include <vector>
#include <osg/ref_ptr>
#include <RadarCurve/ConfigureSet.h>

#include <OpenThreads/Mutex>
#include <RadarCurve/DBConnectBase.h>
#include <RadarCurve/DBRow.h>
#include <RadarCurve/DBTable.h>

#include <osg/MatrixTransform>
#include <osg/Geometry>

#define GPS_OBJ_ID -1111
#define MAX_CHANNEL 16

class GD_VideoShow;
class GD_VideoBack;
class CDlgPromptMessage;
class DetectRoadModel;
class CDlgWorkDetection;

class RADAR_EXPORT RadarManager : public Singleton<RadarManager>
{
protected:
	friend class Singleton<RadarManager>;

	RadarManager();
	~RadarManager();

public:
	void init();//��ʼ��
	void RefreshForCfgChange();
	void clear();
	bool FA88();
	bool startNewTest();//��ʼ�������ý��沨����ʾ
	bool stopTest();//ֹͣ�������ý��沨����ʾ
	bool startNewWork();//ͨ��������Ŀ��Ϣ����·����ʼ�ɼ�����
	bool startNewWorkWithoutProjectName();//ͨ��ʱ�������·����ʼ�ɼ�����
	bool stopWork();//ֹͣ�ɼ�

	//���ݿ�
	//���빤��
	bool importPorject(CString strFileAllPath,bool bCmdImport = false);
	bool importPorjectEXEEX(CString strFileAllPath,bool bCmdImport = false);
	//��������------old
	bool exportProjectAllFile();
	bool exportAnyCountProject();
	//��������--������ΪFALSEʱ������1��2��Ч���ǽ����������Ϊtrueʱ���Լ�ѡ�񵼳�
	bool exportProject(CString strPath,CString strPROJECTCREATETIME ,bool bCmdExport = false,bool bShowOverDlg = true,CString strProName=L"��������");
	bool exportProjectEEEX(CString strPath,CString strPROJECTCREATETIME ,bool bCmdExport = false,bool bShowOverDlg = true);
	//ɾ������
	bool deleteRadarProject(CString strPROJECTCREATETIME);
	//���ݵ���---new-----2017��12��11��
	void DataExport(CWnd* wnd);
	

	bool m_bFlagExportOver;
	bool m_bFlagImportOver;
	int m_iChange;
	int m_iChangeStep;

	GpsReader *getGpsReader();//��gps��
	RadarDataRader *getRadarDataRader();//���״��

	void SetCurCurveDialogPosSize(int iType = 0);
	void showHistCurve();//չʾ��ʷ���ݵĿ��ӻ�
	void showCurCurve(LPCWSTR pszPath = NULL);//չʾ��ǰ���ݵĿ��ӻ�
	
	void showParamDialog();
	void showVideo();//չʾ��ǰ����ͷ

	ConfigureSet *getConfigureSet();//cfg
	MeasureProject *getCurProject();//


	void addRadarDataToParameterConfig( RadarData *d, int index );//�������ý�������ݼ���
	void addRadarDataToDisplay( RadarData *d, int index );//hjl 20210418�������ò�����ʾ
	void addRadarDataToThreeViewDialog( RadarData *d, int index );//���ݼ�������ͼ
	void addRadarDataToProject( RadarData *d, int index );//�ɼ�ģʽ�������ݼ���
	void addRadarDataTo3DProject( RadarData *d, int index );//��ά�ɼ�ģʽ�������ݼ���
	void deleteRadarDataFromProject( int index );//ȥ��������ʾ�����е�����


	void loadAllProject();
	CDBConnectHelp *getDBHelp();

	//�״�ͨ������
	void setRadarChannelCount( int value );
	int getRadarChannelCount()const;
	//���ݿ��ӻ�����չʾ
	void setCurveDisplay( bool value );
	bool getCurveDisplay()const;
	//gpsʹ��
	void setShowGpsPos( bool value );
	bool getShowGpsPos()const;
	//gps���
	void setExportGpsPos( bool value );//hjl-202006
	bool getExportGpsPos()const;//hjl-202006
	//kml���
	void setExportKml( bool value );//hjl-202006
	bool getExportKml()const;//hjl-202006

	//���ݿ�
	bool dbOpen()const;
	void setdbOpen(bool value);

	bool working()const;//�������
	bool testing()const;//�������ý���ʹ�����
	bool AutoCorrecting()const;//�Զ�΢�����

	void savePicPath( std::string const& picPath, int num );

	//�켣��ʾ�����е�Ԫ��
	void addGpsModel();
	void removeGpsModel();

	void setVideoWnd(CWnd* lHwnd){_lpVideo = lHwnd;}
	CWnd *getVideoWnd();

	//ͼƬ����·��
	void setPicPath( std::string const& picPath );
	std::string getPicPath()const;

	//��ͨ������
	void setChannelName( std::string const& name, int index );
	std::string getChannelName( int index )const;

	//�豸����
	void setDeviceCode( std::string const& code );
	std::string getDeviceCode( int iTrackNo )const;

	//��ͨ�������ʵ���
	void setChannelRatioPrect( int index, float value );
	float getChannelRatioPrect( int index )const;

	int getSettingChannelCountByIndex( int index );//��ȡcfg�е�����ͨ����
	int getTrueChannelCount(int setChannelCount);//ͨ������ͨ������ȡʵ��ͨ����
	int GetTrueChannelCount();//��ȡͨ����
	int GetSettingChannelCount();//��ȡ����ͨ����
	float getDielectric(int index);//��ȡ��糣��
	int getSampCount( int index );//��ȡ��������
	float getSampRatio( int index, int trackNo );//��ȡ������
	int getPrecRatio( int index );//��ȡ����

	//�켣��ʾ����
	void CmdNewLine();
	void CmdUpdateLine( osg::Vec3d const& lonLat );
	void CmdDelLine();

	CDBTable::RowVector* getProjectList(){return &_projectList;}
	int GetAFileRadarLine(){return m_iAFileRadarLine;}
	//��ȡ�״﹤����ʽ
	int GetRadarWorkType(){return m_iRadarWorkType;}
	bool CheckRadarProject();
	bool ExportGprFile(void *lpProjectRow,int _channelIndex,std::string & strFilePath,CWnd* pParent = NULL);

	bool Workdetection();

	//��·��⹤�̹���====================================================ygq-2017��10��
	void RoadDetectionProjectManager();

	//��������
	//�״��������
	void RadarParameterConfiguration();
	//������ʽ����
	void WorkTypeConfiguration();

	void setRadarWorkType(int value);//����ģʽ usb ����
	void setRadarDataSaveType(int value);//����ģʽ �ļ�/���ݿ�

	void setRadarDataSavePath(CString strSavePath);
	CString getRadarDataSavePath();
	void setPicSavePath(CString strImagePath);
	std::string getPicSavePath();
	CString GetRadarWorkPath();
	//����radarģ��������ļ�
	void ReadRadarIni();
	void WriteRadarIni();

	//���
	void DataMarkOne();
	void DataMarkTwo();
	void DataMarkThree();
	void DataMarkCustomize();

	//�Զ�΢��
	void addRadarDataToAutoCorrection( RadarData *d, int index );//�Զ�΢��
	void AutoPeakCorrection();
	void SetAutoCorrectionResult(std::vector<int> result);
	std::vector<int> GetAutoCorrectionResult();

	//�������� 20210711hjl
	//int originalIndexToRecordIndex(int originalIndex);
	//bool artificialChannelIndexCheck(int originalIndex);
	//int getArtificalChannelIndexFromOriginalIndex(int originalIndex);

// 	bool DrowRoud(DrawInfo* pStu);
// 	void ClearSelectRoud(DrawInfo* pStu);
// 	void ClearAllRoud();

	//////////////////////////////////////////////////////////////////////////

	//����--���´����ǵײ�ֱ���ã�����OCX����

	GD_VideoShow* m_gdVideoPlay;
	GD_VideoBack* m_gdVideoBack;
	std::string m_strImageSaveDocumentPath;
	int m_nAddDataCount;
	int m_nImageSaveInterval;
	int m_nImageSaveQuality;

	void InitVideo(CString strImagePath,CWnd* wnd);
	void VideoPlay();
	void VideoBack();
	void SetVideoProPath(CString str);
	void StartSaveImage(BOOL flag);
	void StopSaveImage(BOOL flag);
	void SetVideoWindowPos(int iType = 0);

	//////////////////////////////////////////////////////////////////////////


	//��ʾ�����ض�����Ϊ�յ���ʾ��
	void ShowDlgPromptNULL();
	void HideDlgPromptNULL();

	CString GetImportProCreateTime(){return m_cstrImportProCreateTime;};

	//��ȡ������ʾ�ľ���---���Ǽƻ����ȣ������±�
	//float GetJihuaAccuracy(int index);

	//���ڻ�ȡ��ʵ�����־���
	float GetPrecratioFromPreclenAndAccuracy(float fCLen,float fAccuracy);
	int GetTrueCmdIndexFromPreclenAndAccuracy(float fCLen,float fAccuracy);
	float GetTrueAccuracyFromPreclenAndPrecindex(float fCLen,float fAccuracy);
	float GetTrueInterval(float fWheelCircumference, float fSettingInterval, int nWheelPulse);

	//TER
	bool m_bTERWorking;//TER����
	osg::ref_ptr<MeasureProject> m_MPTERCurProject;
	TERDataReader m_TERReader;
	//osg::ref_ptr<MeasureProject> _lpTERCurProject;
	void TERParameterConfiguration();
	void TERSetting();
	void showTERDisplay();
	bool TERStartWork();
	void addTERDataToProject( TERData *d );//�ɼ�ģʽ�������ݼ���
	bool TERStopWork();
	void TERSocketConnect();
	void TERSocketDisconnect();
	void TERSendParameter(int index);
public:
	enum WorkType//usb��������
	{
		USB,
		NETCable
	};

	enum USBType//��ͨ����usb
	{
		OneChannel = 1,
		TwoChannel = 2,
		TwoChannelOld = 3,
		FourChannel = 4,
	};

	WorkType m_WorkType;//�µ�
	USBType m_USBType;

	enum SaveType
	{
		Oracle,
		File,
	};

	SaveType m_SaveType;

	int int_Check_Channel[28];

protected:


	bool exportSqlVideoIni(std::string strTime,std::string strFileName,std::string strFilePath);
	//bool exportSqlDataRadarIni(std::string strTime,std::string strFileName,std::string strFilePath);
	bool exportSqlDataRadarIni(std::string strTime,std::string strFileName,std::string strFilePath,int iSmpNum,int iFileRadarCount);
	void StepEx(bool flagIn = false);

	bool initDataTab(const char *dbsrc, const char *dbname, const char *user, const char *pass, std::string &projectName);
	
	
protected:
	bool _dbOpen;
	bool _working;//�ɼ�����
	bool _testing;//hjl 20210418�������ò�����ʾ
	bool m_bIsAutoCorrecting;//�����Զ�΢��
	int m_nUpload;//hjl20211231�ϴ���ʶ
	//int m_dTwelveFlag;//12ͨ��ģʽ��ʶ

	bool _curveDisplay;//���ݴ�����ʾ��ʶ
	bool _showGps;//ʹ��gps
	bool _exportGps;//���gps
	bool _exportKml;//���kml

	bool m_bIsGpsConnected;

	CDialog *_loadProcess;

	CDBTable::RowVector _projectList;

	CDBTable::RowVector _projectExportList;
	CDBTable::RowVector _projectImportList;

	OpenThreads::Mutex _lock;
	osg::ref_ptr<MeasureProject> _lpCurProject;

	GpsReader _gpsReader;
	RadarDataRader _radarReader;
	

	std::vector<osg::ref_ptr<RadarData>> m_vec3DRadarDataGroup;

	std::vector<int> m_nVecAutoCorrectionResult;

	CDBConnectHelp _dbHelp;

	int m_nTrueChannelCount;
	int m_nSettingChannelCount;//���õ�ͨ��ֵ
	int m_nSaveFileType;

	CWnd *_lpVideo;
	

	std::string _deviceCode;

	osg::ref_ptr<osg::MatrixTransform> _lineGroup;
	osg::ref_ptr<osg::Geometry> _lineLineGemotry;

	RECT m_rtCurCurveDelPos;	
	int m_iAFileRadarLine;
	int m_iTopWindowType;//������ʽ
	
	
	
	CDlgPromptMessage * m_dlgPromptNULL;//��ʾ��ȡ����Ϊ��
	CString m_cstrImportProCreateTime;//��¼���빤�̵Ĺ��̴���ʱ��

	std::string m_strLastProjectCreatTime;	//���һ��ѡ��ʼ�����Ĺ��̴���ʱ��	
	
	//�״�ģ�鹤����·��-ygq
	int m_iRadarWorkType;//�״﹤����ʽ�����ӷ�ʽ��usb1ͨ����usb2ͨ��������8ͨ����
	int m_iSaveOracle;//�״����ݱ���ķ�ʽ���ļ������ݿ�����

	CString m_strRadarPath;
	
	//cfg������
	osg::ref_ptr<ConfigureSet> m_configureSet;

	//�״�ר�ŵ������ļ�
	CString m_strRadarIni;
	//�־��洢·��
	std::string m_strImagePath;

	//�״����ݴ洢��ʽΪ�ļ��洢ʱ�ı���·��
	CString m_strSavePath;
	CDlgWorkDetection* m_pDlgWorkDetection;

	std::map<CString, osg::ref_ptr<DetectRoadModel>> m_pDetectRoadModel;


};
