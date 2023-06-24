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
	void init();//初始化
	void RefreshForCfgChange();
	void clear();
	bool FA88();
	bool startNewTest();//开始参数设置界面波形显示
	bool stopTest();//停止参数设置界面波形显示
	bool startNewWork();//通过设置项目信息创建路径开始采集储存
	bool startNewWorkWithoutProjectName();//通过时间戳创建路径开始采集储存
	bool stopWork();//停止采集

	//数据库
	//导入工程
	bool importPorject(CString strFileAllPath,bool bCmdImport = false);
	bool importPorjectEXEEX(CString strFileAllPath,bool bCmdImport = false);
	//导出工程------old
	bool exportProjectAllFile();
	bool exportAnyCountProject();
	//导出工程--参数三为FALSE时，参数1，2有效，是接受命令导出，为true时是自己选择导出
	bool exportProject(CString strPath,CString strPROJECTCREATETIME ,bool bCmdExport = false,bool bShowOverDlg = true,CString strProName=L"导出工程");
	bool exportProjectEEEX(CString strPath,CString strPROJECTCREATETIME ,bool bCmdExport = false,bool bShowOverDlg = true);
	//删除工程
	bool deleteRadarProject(CString strPROJECTCREATETIME);
	//数据导出---new-----2017年12月11日
	void DataExport(CWnd* wnd);
	

	bool m_bFlagExportOver;
	bool m_bFlagImportOver;
	int m_iChange;
	int m_iChangeStep;

	GpsReader *getGpsReader();//读gps的
	RadarDataRader *getRadarDataRader();//读雷达的

	void SetCurCurveDialogPosSize(int iType = 0);
	void showHistCurve();//展示历史数据的可视化
	void showCurCurve(LPCWSTR pszPath = NULL);//展示当前数据的可视化
	
	void showParamDialog();
	void showVideo();//展示当前摄像头

	ConfigureSet *getConfigureSet();//cfg
	MeasureProject *getCurProject();//


	void addRadarDataToParameterConfig( RadarData *d, int index );//参数设置界面的数据加入
	void addRadarDataToDisplay( RadarData *d, int index );//hjl 20210418参数设置波形显示
	void addRadarDataToThreeViewDialog( RadarData *d, int index );//数据加入三视图
	void addRadarDataToProject( RadarData *d, int index );//采集模式的新数据加入
	void addRadarDataTo3DProject( RadarData *d, int index );//三维采集模式的新数据加入
	void deleteRadarDataFromProject( int index );//去除数据显示窗口中的数据


	void loadAllProject();
	CDBConnectHelp *getDBHelp();

	//雷达通道数量
	void setRadarChannelCount( int value );
	int getRadarChannelCount()const;
	//数据可视化窗口展示
	void setCurveDisplay( bool value );
	bool getCurveDisplay()const;
	//gps使用
	void setShowGpsPos( bool value );
	bool getShowGpsPos()const;
	//gps输出
	void setExportGpsPos( bool value );//hjl-202006
	bool getExportGpsPos()const;//hjl-202006
	//kml输出
	void setExportKml( bool value );//hjl-202006
	bool getExportKml()const;//hjl-202006

	//数据库
	bool dbOpen()const;
	void setdbOpen(bool value);

	bool working()const;//工作情况
	bool testing()const;//参数设置界面使用情况
	bool AutoCorrecting()const;//自动微调情况

	void savePicPath( std::string const& picPath, int num );

	//轨迹显示界面中的元素
	void addGpsModel();
	void removeGpsModel();

	void setVideoWnd(CWnd* lHwnd){_lpVideo = lHwnd;}
	CWnd *getVideoWnd();

	//图片保存路径
	void setPicPath( std::string const& picPath );
	std::string getPicPath()const;

	//各通道名称
	void setChannelName( std::string const& name, int index );
	std::string getChannelName( int index )const;

	//设备名称
	void setDeviceCode( std::string const& code );
	std::string getDeviceCode( int iTrackNo )const;

	//各通道采样率调整
	void setChannelRatioPrect( int index, float value );
	float getChannelRatioPrect( int index )const;

	int getSettingChannelCountByIndex( int index );//获取cfg中的设置通道数
	int getTrueChannelCount(int setChannelCount);//通过设置通道数获取实际通道数
	int GetTrueChannelCount();//获取通道数
	int GetSettingChannelCount();//获取设置通道数
	float getDielectric(int index);//获取介电常数
	int getSampCount( int index );//获取采样点数
	float getSampRatio( int index, int trackNo );//获取采样率
	int getPrecRatio( int index );//获取脉冲

	//轨迹显示界面
	void CmdNewLine();
	void CmdUpdateLine( osg::Vec3d const& lonLat );
	void CmdDelLine();

	CDBTable::RowVector* getProjectList(){return &_projectList;}
	int GetAFileRadarLine(){return m_iAFileRadarLine;}
	//获取雷达工作方式
	int GetRadarWorkType(){return m_iRadarWorkType;}
	bool CheckRadarProject();
	bool ExportGprFile(void *lpProjectRow,int _channelIndex,std::string & strFilePath,CWnd* pParent = NULL);

	bool Workdetection();

	//道路检测工程管理====================================================ygq-2017年10月
	void RoadDetectionProjectManager();

	//参数配置
	//雷达参数配置
	void RadarParameterConfiguration();
	//工作方式配置
	void WorkTypeConfiguration();

	void setRadarWorkType(int value);//工作模式 usb 网口
	void setRadarDataSaveType(int value);//保存模式 文件/数据库

	void setRadarDataSavePath(CString strSavePath);
	CString getRadarDataSavePath();
	void setPicSavePath(CString strImagePath);
	std::string getPicSavePath();
	CString GetRadarWorkPath();
	//更新radar模块的配置文件
	void ReadRadarIni();
	void WriteRadarIni();

	//标记
	void DataMarkOne();
	void DataMarkTwo();
	void DataMarkThree();
	void DataMarkCustomize();

	//自动微调
	void addRadarDataToAutoCorrection( RadarData *d, int index );//自动微调
	void AutoPeakCorrection();
	void SetAutoCorrectionResult(std::vector<int> result);
	std::vector<int> GetAutoCorrectionResult();

	//人造数据 20210711hjl
	//int originalIndexToRecordIndex(int originalIndex);
	//bool artificialChannelIndexCheck(int originalIndex);
	//int getArtificalChannelIndexFromOriginalIndex(int originalIndex);

// 	bool DrowRoud(DrawInfo* pStu);
// 	void ClearSelectRoud(DrawInfo* pStu);
// 	void ClearAllRoud();

	//////////////////////////////////////////////////////////////////////////

	//视屏--以下代码是底层直接用，不是OCX调用

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


	//显示或隐藏读数据为空的提示框
	void ShowDlgPromptNULL();
	void HideDlgPromptNULL();

	CString GetImportProCreateTime(){return m_cstrImportProCreateTime;};

	//获取界面显示的精度---就是计划精度，传入下标
	//float GetJihuaAccuracy(int index);

	//用于获取真实测量轮精度
	float GetPrecratioFromPreclenAndAccuracy(float fCLen,float fAccuracy);
	int GetTrueCmdIndexFromPreclenAndAccuracy(float fCLen,float fAccuracy);
	float GetTrueAccuracyFromPreclenAndPrecindex(float fCLen,float fAccuracy);
	float GetTrueInterval(float fWheelCircumference, float fSettingInterval, int nWheelPulse);

	//TER
	bool m_bTERWorking;//TER工作
	osg::ref_ptr<MeasureProject> m_MPTERCurProject;
	TERDataReader m_TERReader;
	//osg::ref_ptr<MeasureProject> _lpTERCurProject;
	void TERParameterConfiguration();
	void TERSetting();
	void showTERDisplay();
	bool TERStartWork();
	void addTERDataToProject( TERData *d );//采集模式的新数据加入
	bool TERStopWork();
	void TERSocketConnect();
	void TERSocketDisconnect();
	void TERSendParameter(int index);
public:
	enum WorkType//usb还是网口
	{
		USB,
		NETCable
	};

	enum USBType//几通道的usb
	{
		OneChannel = 1,
		TwoChannel = 2,
		TwoChannelOld = 3,
		FourChannel = 4,
	};

	WorkType m_WorkType;//新的
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
	bool _working;//采集工作
	bool _testing;//hjl 20210418参数设置波形显示
	bool m_bIsAutoCorrecting;//正在自动微调
	int m_nUpload;//hjl20211231上传标识
	//int m_dTwelveFlag;//12通道模式标识

	bool _curveDisplay;//数据窗口显示标识
	bool _showGps;//使用gps
	bool _exportGps;//输出gps
	bool _exportKml;//输出kml

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
	int m_nSettingChannelCount;//设置的通道值
	int m_nSaveFileType;

	CWnd *_lpVideo;
	

	std::string _deviceCode;

	osg::ref_ptr<osg::MatrixTransform> _lineGroup;
	osg::ref_ptr<osg::Geometry> _lineLineGemotry;

	RECT m_rtCurCurveDelPos;	
	int m_iAFileRadarLine;
	int m_iTopWindowType;//工作方式
	
	
	
	CDlgPromptMessage * m_dlgPromptNULL;//提示读取数据为空
	CString m_cstrImportProCreateTime;//记录导入工程的过程创建时间

	std::string m_strLastProjectCreatTime;	//最后一次选择开始工作的工程创建时间	
	
	//雷达模块工作的路径-ygq
	int m_iRadarWorkType;//雷达工作方式，连接方式，usb1通道、usb2通道，网口8通道。
	int m_iSaveOracle;//雷达数据保存的方式，文件和数据库两种

	CString m_strRadarPath;
	
	//cfg配置器
	osg::ref_ptr<ConfigureSet> m_configureSet;

	//雷达专门的配置文件
	CString m_strRadarIni;
	//街景存储路径
	std::string m_strImagePath;

	//雷达数据存储方式为文件存储时的保存路径
	CString m_strSavePath;
	CDlgWorkDetection* m_pDlgWorkDetection;

	std::map<CString, osg::ref_ptr<DetectRoadModel>> m_pDetectRoadModel;


};
