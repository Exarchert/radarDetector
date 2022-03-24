#include "StdAfx.h"
#include <RadarCurve/RadarManager.h>
#include <OpenThreads/ScopedLock>
#include "DialogHistoryCurve.h"
#include "DBAccess/OtlRecordset.h"
#include "DlgRadarParamSet.h"
#include <sstream>
#include "DialogProject.h"
#include "ProjectNameTab.h"
#include "DataTemplateTab.h"
#include <Object/CmdCenter/CmdCenter.h>
#include "VideoTemplateTab.h"
#include <shlwapi.h>
#include <osgApex/CoordinateTool>
#include <osg/StateAttribute>
#include <osg/LineWidth>
#include <osg/Geode>
#include <Kernel/KernelExport.h>
#include <Com/Command/Cmd.h>
#include "DialogProject_Export.h"
#include <fstream>
#include <istream>
#include <iostream>
#include <string>
#include "COM/Tools/ProcessData.h"
#include "ProcessDlg.h"
#include "DataCountTab.h"
#include <iosfwd>
#include <COM\log\com_log.h>
#include "GD_VideoShow.h"
#include "GD_VideoBack.h"
#include "GPRWriter.h"
#include <TlHelp32.h>
#include "DlgPromptMessage.h"
#include "RadarCurve\CmdCenter\CmdToRadar.h"
#include "COM\Tools\ComFunction.h"
#include "COM\KernelContext\KernelContext.h"
#include <ctime>
#include <cstdio>
  
//self
#include "DlgWorkDetection.h"
#include "DlgWorkTypeConfiguration.h"
#include "DlgDataExport.h"
#include "DialogCurCurve.h"
#include "DlgRadarParameterConfig.h"
#include "DialogThreeView.h"

using namespace std; 

CDialogCurCurve *g_CurCurveFirstDlg = NULL;
CDialogCurCurve *g_CurCurveSecondDlg = NULL;
CDialogCurCurve *g_CurCurveThirdDlg = NULL;
CDialogCurCurve *g_CurCurveFourthDlg = NULL;
CDialogThreeView *g_ThreeViewDlg = NULL;
DlgRadarParameterConfig *g_RadarParameterConfig=NULL;
CDialogProject *g_HistoryProjectDlg = NULL;

class GD_ExOrImProjectThread : public OpenThreads::Thread
{
public:

	GD_ExOrImProjectThread( RadarManager* lpReader,CString strPath,CString strPROJECTCREATETIME ,bool bCmdImExport = false ,bool bShowOverDlg = true)
	{
		m_strPath = strPath;
		m_strPROJECTCREATETIME = strPROJECTCREATETIME;
		m_bCmdExport = bCmdImExport;

		m_bCmdImport = bCmdImExport;

		m_bShowOverDlg = bShowOverDlg;
		m_bOverFlag = false;
		_lpReader = lpReader;
		m_bImport = false;
		m_bExport = false;
		m_bResult = false;
	}
	GD_ExOrImProjectThread( RadarManager* lpReader)
	{
		m_strPath = L"";
		m_strPROJECTCREATETIME = L"";
		m_bCmdExport = false;
		m_bCmdImport = false;

		m_bOverFlag = false;
		_lpReader = lpReader;
		m_bImport = false;
		m_bExport = false;
		m_bResult = false;
		m_bShowOverDlg = true;
	}
	int cancel()
	{
		int ret = OpenThreads::Thread::cancel();
		if ( isRunning() )
		{
			join();
		}
		return ret;
	}
	void run()
	{
		if (NULL != _lpReader)
		{
			if (true == m_bExport)
			{
				m_bResult = _lpReader->exportProjectEEEX(m_strPath,m_strPROJECTCREATETIME ,m_bCmdExport,m_bShowOverDlg);
				_lpReader->m_bFlagExportOver = true;
				m_bExport = false;
				m_bOverFlag = true;
			}
			if (true == m_bImport)
			{
				m_bResult = _lpReader->importPorjectEXEEX(m_strPath,m_bCmdImport);
				_lpReader->m_bFlagImportOver = true;
				m_bImport = false;
				m_bOverFlag = true;
			}
		}
	}
	bool m_bOverFlag;
	bool m_bImport;
	bool m_bExport;
	bool m_bResult;
	bool m_bShowOverDlg;
protected:
	RadarManager *_lpReader;
	CString m_strPath;
	CString m_strPROJECTCREATETIME ;
	bool m_bCmdExport;
	bool m_bCmdImport;
};

vector<string> splitEx(const string& src, string separate_character)
{
	vector<string> strs;

	int separate_characterLen = separate_character.size();//
	int lastPosition = 0,index = -1;
	while (-1 != (index = src.find(separate_character,lastPosition)))
	{
		strs.push_back(src.substr(lastPosition,index - lastPosition));
		lastPosition = index + separate_characterLen;
	}
	string lastString = src.substr(lastPosition);
	if (!lastString.empty())
		strs.push_back(lastString);
	return strs;
}


class LineCmd : public COM::CCmdObj
{
public:
	enum TYPE
	{
		T_ADD,
		T_DEL,
		T_UPDATE,
	};
	
	LineCmd( TYPE t, osg::MatrixTransform *lpNode )
	{
		_node = lpNode;
		_t = t;
	}

	LineCmd( TYPE t, osg::MatrixTransform *lpNode, osg::Geometry *lineGeometry, osg::Vec3d const& pos )
	{
		_node = lpNode;
		_t = t;
		_newPos = pos;
		_lineGemotry = lineGeometry;
	}

	virtual void DoAction();


	TYPE _t;


	osg::ref_ptr<osg::MatrixTransform> _node;
	osg::ref_ptr<osg::Geometry> _lineGemotry;
	osg::Vec3d _newPos;
};


void LineCmd::DoAction()
{
	if ( !_node.valid() ){
		return;
	}

	if ( _t == T_ADD ){
		if ( !Kernel::GetpKC()->GetTempGroupRoot()->containsNode( _node ) ){
			Kernel::GetpKC()->GetTempGroupRoot()->addChild( _node );
		}
	}else if ( _t == T_DEL ){
		Kernel::GetpKC()->GetTempGroupRoot()->removeChild( _node );
	}else{//update
		if ( !_lineGemotry.valid() ){
			return;
		}
		osg::Vec3dArray* posArray = dynamic_cast<osg::Vec3dArray*>(_lineGemotry->getVertexArray());//新建指向坐标数组的指针
		if ( !posArray ){//即原来没有 就初始化数组以及线段属性
			posArray = new osg::Vec3dArray;
			_lineGemotry->setVertexArray( posArray );
			osg::Vec4Array *lpColorArray = new osg::Vec4Array;//新建储存rgba颜色的数组
			lpColorArray->push_back( osg::Vec4( 1.0, 0.0, 0.0, 1.0 ) );//红色
			_lineGemotry->setColorBinding( osg::Geometry::BIND_OVERALL );
			_lineGemotry->setColorArray( lpColorArray );
			osg::LineWidth *lw = new osg::LineWidth;
			lw->setWidth( 4.0f );//线宽
			_lineGemotry->getOrCreateStateSet()->setAttributeAndModes( lw, osg::StateAttribute::ON );//设线宽
		}

		if ( posArray->size() == 0 ){//如果数组为0则添加第一个元素 地心
			osg::Matrixd localToWorld;
			osgAPEx::ComputeLocalToWorldTransformFromLongLatHeight( _newPos.x(), _newPos.y(), _newPos.z(), localToWorld );
			_node->setMatrix( localToWorld );
			posArray->push_back( osg::Vec3d( 0.0, 0.0, 0.0 ) );//hjl？

		}else{
			osg::Vec3d xyz;
			osgAPEx::ConvertLongLatHeightToXYZ( _newPos.x(), _newPos.y(), _newPos.z(), xyz );
			
			osg::Matrixd worldToLocal = _node->getInverseMatrix();
			osg::Vec3d localPos = xyz * worldToLocal;
			posArray->push_back( localPos );
		}

		_lineGemotry->removePrimitiveSet( 0, 1 );
		_lineGemotry->addPrimitiveSet( new osg::DrawArrays ( osg::PrimitiveSet::LINE_STRIP, 0, posArray->size() ));
	}
}


class SaveVideoCmd : public SaveDataCmd
{
public:

	virtual void exec( std::string const& projectTab )
	{

		if ( !RadarManager::Instance()->getDBHelp() )
		{
			return;
		}
		if ( !RadarManager::Instance()->getDBHelp()->GetConnect() )
		{
			return;
		}
		if ( !_lpRow.valid() )
		{
			return;
		}

		VideoTemplateTab tab;
		tab.setProjectName( projectTab );

		tab.insert( RadarManager::Instance()->getDBHelp(), _lpRow.get() );
	}
	osg::ref_ptr<VideoTemplateTab::VideoRow> _lpRow;
};

void RadarManager::CmdNewLine()
{
	_lineGroup = new osg::MatrixTransform;

	osg::Geode *lpGeode = new osg::Geode;
	_lineGroup->addChild( lpGeode );

	_lineLineGemotry = new osg::Geometry;
	lpGeode->addDrawable( _lineLineGemotry );

	COM::CCmdObjQueue *cmdQueue = NULL;
	if (Kernel::GetpKC() && (cmdQueue = Kernel::GetpKC()->m_CmdCenter.GetCmdObjList()))
	{
		osg::ref_ptr<LineCmd> pCmd = new LineCmd(LineCmd::T_ADD, _lineGroup.get());
		cmdQueue->EnterQueue(pCmd);
	}
}
void RadarManager::CmdUpdateLine( osg::Vec3d const& lonLat )
{
	COM::CCmdObjQueue *cmdQueue = NULL;
	if (Kernel::GetpKC() && (cmdQueue = Kernel::GetpKC()->m_CmdCenter.GetCmdObjList()))
	{
		osg::ref_ptr<LineCmd> pCmd = new LineCmd(LineCmd::T_UPDATE, _lineGroup.get(), _lineLineGemotry.get(), lonLat);
		cmdQueue->EnterQueue(pCmd);
	}
}
void RadarManager::CmdDelLine()
{
	COM::CCmdObjQueue *cmdQueue = NULL;
	if (Kernel::GetpKC() && (cmdQueue = Kernel::GetpKC()->m_CmdCenter.GetCmdObjList()))
	{
		osg::ref_ptr<LineCmd> pCmd = new LineCmd(LineCmd::T_DEL, _lineGroup.get());
		cmdQueue->EnterQueue(pCmd);
	}
}

RadarManager::RadarManager(void)
{
	_testing = false;
	_working = false;
	_lpCurProject = NULL;

	m_pDlgWorkDetection = NULL;

	m_configureSet = new ConfigureSet;

	_radarChannelCount = 8;

	_dbOpen = false;

	_showGps = false;
	_exportGps = false;
	_exportKml = false;
	_lpVideo = NULL;
	_loadProcess = NULL;
	
	m_iChange = 0;
	m_bFlagExportOver = false;
	m_bFlagImportOver = false;

	
	m_rtCurCurveDelPos.left = 0;
	m_rtCurCurveDelPos.top = 0;
	m_rtCurCurveDelPos.right = 0;
	m_rtCurCurveDelPos.bottom = 0;


	m_gdVideoPlay = NULL;
	m_gdVideoBack = NULL;
	m_dlgPromptNULL = NULL;

	for(int i=8;i<28;i++){//暂时20210203
		int_Check_Channel[i]=1;
	}
}

void RadarManager::InitVideo(CString strImagePath,CWnd* wnd)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if (NULL == m_gdVideoPlay)
	{
		m_gdVideoPlay = new GD_VideoShow(wnd);
		m_gdVideoBack = new GD_VideoBack(wnd);
	}
	m_gdVideoPlay->Create(GD_VideoShow::IDD);
	m_gdVideoPlay->ShowWindow(false);

	m_gdVideoBack->Create(GD_VideoBack::IDD);
	m_gdVideoBack->ShowWindow(false);

	RadarManager::Instance()->setVideoWnd( m_gdVideoBack );
	m_gdVideoBack->SetPicPath(strImagePath );
	m_gdVideoPlay->SetPicPath(strImagePath);
}
void RadarManager::VideoPlay()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if (NULL == m_gdVideoPlay)
	{
		m_gdVideoPlay = new GD_VideoShow();
		m_gdVideoPlay->Create(GD_VideoShow::IDD);
	}
	m_gdVideoPlay->VideoShow();

}
void RadarManager::VideoBack()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if (NULL == m_gdVideoBack)
	{
		m_gdVideoBack = new GD_VideoBack();
		m_gdVideoBack->Create(GD_VideoBack::IDD);
	}
	m_gdVideoBack->VideoBackplay();
}

void RadarManager::SetVideoProPath(CString str)
{
	m_gdVideoPlay->SetProPath(str);
}
void RadarManager::StartSaveImage(BOOL flag)
{
	m_gdVideoPlay->StartSaveImage(flag);
}
void RadarManager::StopSaveImage(BOOL flag)
{
	m_gdVideoPlay->StopSaveImage(flag);
}

void RadarManager::SetVideoWindowPos(int iType/* = 0*/)
{

	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	RECT rt;  
	SystemParametersInfo(SPI_GETWORKAREA,0,(PVOID)&rt,0);  
	int cx = rt.right;
	int cy = rt.bottom;


	int cxAll = GetSystemMetrics(   SM_CXSCREEN   );   
	int cyAll = GetSystemMetrics(   SM_CYSCREEN   );

	int iXLeft ;
	int iYTop ;
	int iXWidth;
	int iYHight ;

	if (0 == iType)
	{
		iXLeft = cx / 2;
		iYTop = 0;
		iXWidth = cx / 2;
		iYHight = cy / 2;
	} 
	else if (1 == iType)
	{
		iXLeft = cxAll;
		iYTop = 0;
		iXWidth = cxAll;
		iYHight = cyAll;
	}
	else if (2 == iType)
	{
		iXLeft = 0;
		iYTop = 0;
		iXWidth = cx ;
		iYHight = cy;
	}
	else if (3 == iType)
	{
		iXLeft = cxAll;
		iYTop = -cyAll;
		iXWidth = cxAll;
		iYHight = cyAll;
	}
	else if (4 == iType)
	{
		iXLeft = 0;
		iYTop = -cyAll;
		iXWidth = cxAll;
		iYHight = cyAll;
	}

	m_gdVideoPlay->ShowWindow( SW_SHOW );
	m_gdVideoPlay->SetWindowPos(NULL,iXLeft,iYTop,iXWidth,iYHight,   SWP_FRAMECHANGED | SWP_NOZORDER	);
	m_gdVideoPlay->VideoShow();
	
}

RadarManager::~RadarManager(void)
{
	stopWork();

	if (NULL != m_gdVideoPlay)
	{
		delete m_gdVideoPlay;
		m_gdVideoPlay = NULL;
	}
	if (NULL != m_gdVideoBack)
	{
		delete m_gdVideoBack;
		m_gdVideoBack = NULL;
	}
	if (NULL != m_dlgPromptNULL)
	{
		delete m_dlgPromptNULL;
		m_dlgPromptNULL = NULL;
	}
	if (NULL != m_pDlgWorkDetection)
	{
		delete m_pDlgWorkDetection;
		m_pDlgWorkDetection = NULL;
	}
	_dbHelp.Close();
}

void RadarManager::init()
{

	//雷达参数-与雷达探测的所有配置文件、临时文件都放在该文件夹下。以后放在“雷达工程文件夹”下。
	COM::CKernelContext * pKernelContext = Kernel::GetpKC();
	if (pKernelContext)
	{
		CString pDesPath,pRadarPath;
		{
			pDesPath = pKernelContext->m_BinPath+_T("\\Radar");
		}
		{
			pRadarPath.Format(_T("%s\\ExeRes\\ProTemplate\\Radar"), pKernelContext->m_BinPath);
		}

 		if (!PathFileExists(pDesPath))
 		{
  			if (!COM::FileCopy(pRadarPath, pKernelContext->m_BinPath))
  			{
  				AfxMessageBox(_T("启动雷达模块失败！"));
  			}
 		}

		m_strRadarPath = pDesPath;
		if (m_strRadarPath.Compare(_T("\\Radar"))==0)
		{
			AfxMessageBox(_T("pProPath错误！"));
			//m_strRadarPath = ;
		}

		//解析配置文件的值，从RadarConfig.ini获取工作模式、文件存储位置
		m_strRadarIni = m_strRadarPath;
		m_strRadarIni += L"\\RadarConfig.ini";

		ReadRadarIni();
	}

	USES_CONVERSION;
	//数据库配置文件
	std::string strCfgFile = W2A(m_strRadarPath);
	strCfgFile += "\\radar.cfg";
	m_configureSet->read(strCfgFile);
	setRadarChannelCount( atoi ( m_configureSet->get("radar", "count").c_str() ) );
	m_nUpload = atoi( m_configureSet->get("radar", "uploadFlag").c_str() );
// 	if ( m_configureSet->get("db", "use" ) == "true")
// 	{
// 		if ( !getDBHelp()->Open( m_configureSet->get("db","dbsource").c_str()
// 			, m_configureSet->get("db", "dbname").c_str()
// 			, m_configureSet->get("db", "user").c_str()
// 			, m_configureSet->get("db", "pwd").c_str()
// 			)
// 			)
// 		{
// 			::AfxMessageBox(  L"连接数据库失败");
// 		}
// 		else
// 		{
// 			_dbOpen = true;
// 			loadAllProject();
// 		}
// 	}

	//街景存储路径
	//CString strImagePath = m_strRadarPath + L"\\DataImage\\";
	CString strImagePath = m_strSavePath + L"DataImage\\";
	if(!PathFileExists(strImagePath))
	{
		COM::CreateMultipleDirectory(strImagePath);
	}
	m_strImagePath =W2A(strImagePath);
}


void RadarManager::ShowDlgPromptNULL()
{
	if (NULL != m_dlgPromptNULL)
	{
		m_dlgPromptNULL->ShowWindow(true);
		::SetForegroundWindow(m_dlgPromptNULL->m_hWnd);
		::SetWindowPos(m_dlgPromptNULL->GetSafeHwnd(),HWND_TOPMOST,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);
	}
	
}
void RadarManager::HideDlgPromptNULL()
{
	if (NULL != m_dlgPromptNULL)
	{
		m_dlgPromptNULL->ShowWindow(false);
	}
}


        
bool RadarManager::importPorjectEXEEX(CString strFileAllPath,bool bCmdImport)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());




	CString cstrProFilePath = L"";//
	CString cstrFolderPath = L"";//
	CString cstrProFileName = L"";//


	if (true == bCmdImport)
	{
		cstrProFilePath = strFileAllPath;//
		cstrFolderPath = cstrProFilePath;
		cstrFolderPath = cstrFolderPath.Left(cstrFolderPath.ReverseFind('\\'));//
		cstrProFileName  = cstrProFilePath;//
		cstrProFileName = cstrProFileName.Right(cstrProFileName.ReverseFind('\\'));//

	} 
	else
	{
	   CString sFilter = _T("工程配置文件(*.pro)|*.pro|");
	   CFileDialog dlgOpen(TRUE, 0, 0, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, (LPCTSTR)sFilter);
	   if(dlgOpen.DoModal() == IDCANCEL)
	   {
		   return false; 
	   }

	   cstrProFilePath = dlgOpen.GetPathName();
	   cstrFolderPath = cstrProFilePath;
	   cstrFolderPath = cstrFolderPath.Left(cstrFolderPath.ReverseFind('\\'));
	   cstrProFileName = dlgOpen.GetFileName();

	}



   CString cstrTemp;
   std::string strTemp;

   //
   osg::ref_ptr<ProjectNameTab::ProjectNameRow > lpRow = new ProjectNameTab::ProjectNameRow;
   USES_CONVERSION;
   TCHAR szKeyVal[MAX_PATH];
   ZeroMemory(szKeyVal,MAX_PATH*sizeof(TCHAR));
   ::GetPrivateProfileString(_T("Project"),_T("PROJECTNAME"),_T(""), szKeyVal, MAX_PATH, cstrProFilePath);
   lpRow->_projectName = W2A(szKeyVal);
   ZeroMemory(szKeyVal,MAX_PATH*sizeof(TCHAR));
   ::GetPrivateProfileString(_T("Project"),_T("PROJECTCREATETIME"),_T(""), szKeyVal, MAX_PATH, cstrProFilePath);
   strTemp = W2A(szKeyVal);
   CString cstrProCreateTime = szKeyVal;
   std::string strProCreateTime = strTemp;
   lpRow->_createTime = atoi(strTemp.c_str());
   ZeroMemory(szKeyVal,MAX_PATH*sizeof(TCHAR));
   ::GetPrivateProfileString(_T("Project"),_T("STARTLEN"),_T(""), szKeyVal, MAX_PATH, cstrProFilePath);
   strTemp = W2A(szKeyVal);
   lpRow->_startLen = atof(strTemp.c_str());
   ZeroMemory(szKeyVal,MAX_PATH*sizeof(TCHAR));
   ::GetPrivateProfileString(_T("Project"),_T("CURLEN"),_T(""), szKeyVal, MAX_PATH, cstrProFilePath);
   strTemp = W2A(szKeyVal);
   lpRow->_curLen = atof(strTemp.c_str());
   ZeroMemory(szKeyVal,MAX_PATH*sizeof(TCHAR));
   ::GetPrivateProfileString(_T("Project"),_T("PROJECTPARAM_FileName"),_T(""), szKeyVal, MAX_PATH, cstrProFilePath);
   cstrTemp = cstrFolderPath ;
   cstrTemp += "\\";
   cstrTemp += szKeyVal;
   ifstream in(W2A(cstrTemp), ios::in);
   istreambuf_iterator<char> beg(in), end;
   std::string strdata(beg, end);
   in.close();
   lpRow->_paramXML = strdata;

   //
   bool bCharu = true;
   for (CDBTable::RowVector::iterator ite = _projectList.begin() ; ite < _projectList.end();++ite)
   {
	   osg::ref_ptr<DBRow> lppppp = *ite;
	   if (!lppppp)
	   {
		   continue;
	   }
	   osg::ref_ptr<ProjectNameTab::ProjectNameRow> lpRowPPP = dynamic_cast<ProjectNameTab::ProjectNameRow*>(lppppp.get());
	   if ( !lpRowPPP )
	   {
		   continue;
	   }
	   if (lpRow->_createTime == lpRowPPP->_createTime)
	   {
		   CString strMsg = L"工程存在！是否继续?"; 
		   if (AfxMessageBox(strMsg, MB_YESNO) == IDYES)
		   {
			   bCharu = false;
			   break;
		   }
		   else
		   {
			   return false;
		   }
	   }
   }
   if (true == bCharu)
   {
	   ProjectNameTab tab;
	   tab.insert( &_dbHelp, lpRow );
	   _projectList.push_back( lpRow);
	   tab.close();
   }

   m_iChange = 1;//
   Sleep(300);

   ConfigureSet set;
   set.fromGroupXML(lpRow->_paramXML);
   int iSmpNum = atoi ( set.get("radar", "sample").c_str());
   iSmpNum = getSampCount( iSmpNum );

   ZeroMemory(szKeyVal,MAX_PATH*sizeof(TCHAR));
   ::GetPrivateProfileString(_T("Data"), _T("DataFileCount"),_T(""), szKeyVal, MAX_PATH, cstrProFilePath);
   cstrTemp = szKeyVal;
   strTemp = W2A(cstrTemp);
   int iFileRadarCount = atoi(strTemp.c_str());
   m_iChangeStep = 100 / (iFileRadarCount + 3);
   ++m_iChange;
   Sleep(300);

   VideoTemplateTab tabVideo;
   tabVideo.setProjectName( W2A(cstrProCreateTime ));
   if ( tabVideo.open( &_dbHelp, tabVideo.getSelectSql() ) )
   {
	   tabVideo.drop(&_dbHelp);
   }
   tabVideo.close();
   if (false == tabVideo.createTab( &_dbHelp ))
   {
	   ::MessageBox(NULL,L"导入工程",L"创建视频表失败", MB_OK); 
	   tabVideo.close();
	   return false;
   }
   tabVideo.close();
   DataTemplateTab tabData;
   tabData.setProjectName( W2A(cstrProCreateTime ));
   if ( tabData.open( &_dbHelp, tabData.getSelectSql() ) )
   {
	   tabData.drop(&_dbHelp);
   }
   tabData.close();
   if (false == tabData.createTab( &_dbHelp ))
   {
	   ::MessageBox(NULL,L"导入工程",L"创建雷达表失败", MB_OK); 
	   tabData.close();
	   return false;
   }
   tabData.close();

   ZeroMemory(szKeyVal,MAX_PATH*sizeof(TCHAR));
   ::GetPrivateProfileString(_T("Data"),_T("ImageFileName"),_T(""), szKeyVal, MAX_PATH, cstrProFilePath);
   CString strFromPath = cstrFolderPath ;
   strFromPath += "\\";
   strFromPath += szKeyVal;

   TCHAR szAppPath[MAX_PATH];
   ZeroMemory(szAppPath,MAX_PATH*sizeof(TCHAR));
   ::GetModuleFileName(AfxGetInstanceHandle(), szAppPath, MAX_PATH);
   ::PathRemoveFileSpec(szAppPath);
   CString strToPath = szAppPath;
   strToPath += L"\\ProjectData\\DataImage\\" ;
   strToPath += szKeyVal;
   if (!PathIsDirectory(strToPath) ) 
   { 
	   if (!CreateDirectory(strToPath, NULL ) ) 
	   { 
		   CString strMsg; 
		   strMsg.Format (L"创建路径\"%s\"保存图片的文件夹失败！是否继续?",strToPath); 
		   if (AfxMessageBox(strMsg, MB_YESNO) == IDNO)
		   {
			   return false;
		   }
			   
	   } 
   } 
   string strCopy = "copy ";
   strCopy += W2A(strFromPath);
   strCopy += "\\*.* ";
   strCopy += W2A(strToPath);
   system(strCopy.c_str());
   ++m_iChange;
   Sleep(300);

   ZeroMemory(szKeyVal,MAX_PATH*sizeof(TCHAR));
   ::GetPrivateProfileString(_T("Data"),_T("VideoFileName"),_T(""), szKeyVal, MAX_PATH, cstrProFilePath);
   cstrTemp = cstrFolderPath ;
   cstrTemp += "\\";
   cstrTemp += szKeyVal;
   //一次读取一行
   ifstream fin(W2A(cstrTemp),std::ifstream::binary);
   std::string s ;
   vector<string> strs;
   VideoTemplateTab tabVideoExc;
   tabVideoExc.setProjectName( W2A(cstrProCreateTime ));
   CDBConnectHelp * dbHelp = &_dbHelp;
   VideoTemplateTab::VideoRow vr ;

   int iLeth = 0;
   while (getline(fin,s))
   {
		strs = splitEx(s, "|"); 
		if (strs.size() < 2 )
		{
			continue;
		}
		vr._recvTime = atoi(strs[0].c_str());
		vr._filePath = strs[1];
		vr._cameraNum = atoi(strs[2].c_str());
		tabVideoExc.insert( dbHelp, &vr );
   }
   fin.close();
   tabVideoExc.close();
   ++m_iChange;
   Sleep(300);


   ZeroMemory(szKeyVal,MAX_PATH*sizeof(TCHAR));
   ::GetPrivateProfileString(_T("Data"),_T("DataFileName"),_T(""), szKeyVal, MAX_PATH, cstrProFilePath);
   cstrTemp = cstrFolderPath ;
   cstrTemp += "\\";
   cstrTemp += szKeyVal;



   std::string strRadarFilePath;
   char cBuf[64];
   for (int i = 0 ; i < iFileRadarCount ;++i)
   {
	   strRadarFilePath = W2A(cstrTemp);
	   strRadarFilePath += itoa(i,cBuf,10);
	   strRadarFilePath += ".da";
	   DataTemplateTab tabDataExc;
	   tabDataExc.setProjectName( W2A(cstrProCreateTime ));
	   tabDataExc.importSqlDataRadarIni(&_dbHelp, strRadarFilePath ,iSmpNum);
	   tabDataExc.close();
	   ++m_iChange;
	   Sleep(300);
   }
   ++m_iChange;

   if (true != bCmdImport)
   {
	   ::MessageBox(NULL,L"导入工程成功!",L"导入工程", MB_OK); 
   }
	return true;
}

bool RadarManager::exportSqlDataRadarIni(std::string strTime,std::string strFileName,std::string strFilePath,int iSmpNum,int iFileRadarCount)
{
	DataTemplateTab tab;
	tab.setProjectName( strTime );
	CDBTable::RowVector rv;
	std::string strSQL =  tab.getSelectSqlAll();//
	strSQL += " order by id";
	if ( !tab.open( RadarManager::Instance()->getDBHelp(),strSQL ))
	{
		return false;
	}

	std::string strRadarFilePath;
	char cBuf[64];
	for (int i = 0 ; i < iFileRadarCount ;++i)
	{
		strRadarFilePath = strFilePath;
		strRadarFilePath += "\\" + strFileName;
		memset(cBuf,NULL,sizeof(cBuf));
		strRadarFilePath += itoa(i,cBuf,10);
		strRadarFilePath += ".da";
		tab.exportSqlDataRadarIni(strRadarFilePath,m_iAFileRadarLine,iSmpNum);
		++m_iChange ;
		Sleep(300);
	}
	tab.close();
	return true;
}

void RadarManager::StepEx(bool flagIn)
{
	if (NULL != _loadProcess)
	{
		((CProcessDlg*)_loadProcess)->StepEx();
	}
}

bool RadarManager::deleteRadarProject(CString strPROJECTCREATETIME)
{
	bool flag = false;

	USES_CONVERSION;
	ProjectNameTab::ProjectNameRow *lpRow= NULL;
	INT64 iCreateTime = atoi(W2A(strPROJECTCREATETIME));
	for (int i = 0 ; i < _projectList.size() ; ++i)
	{
		osg::ref_ptr<DBRow> _selectRow = _projectList[i];
		lpRow = dynamic_cast<ProjectNameTab::ProjectNameRow*>(_selectRow.get());
		if (lpRow->_createTime == iCreateTime)
		{
			flag = true;
			SetCursor( LoadCursor(NULL, IDC_WAIT) );	
			if ( RadarManager::Instance()->dbOpen())
			{
				if ( lpRow )
				{
					DataTemplateTab dataTab;
					std::stringstream ss;
					ss << lpRow->_createTime;
					dataTab.setProjectName( ss.str() );
					dataTab.drop(RadarManager::Instance()->getDBHelp() );

					VideoTemplateTab videoTab;
					videoTab.setProjectName( ss.str() );
					videoTab.drop(RadarManager::Instance()->getDBHelp() );
				}

				ProjectNameTab tab;
				tab.del( RadarManager::Instance()->getDBHelp(), _projectList[i].get() );
			}
			_projectList.erase( _projectList.begin() + i );
			SetCursor( LoadCursor(NULL, IDC_ARROW) );
			break;
		}
	}
	return flag;
}
bool RadarManager::importPorject(CString strFileAllPath,bool bCmdImport /*= false*/)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if ( !_loadProcess )
	{
		_loadProcess = new CProcessDlg;
		_loadProcess->Create( CProcessDlg::IDD );
	}
	CProcessDlg *lpDlg = (CProcessDlg*)_loadProcess;
	m_iChangeStep = 5;
	m_bFlagImportOver = false;

	//创建导入工程的线程
	GD_ExOrImProjectThread *_lpReadThread = new GD_ExOrImProjectThread( this, strFileAllPath,L"" ,bCmdImport);

	_lpReadThread->m_bImport = true;
	if ( !_lpReadThread->isRunning() )
	{
		_lpReadThread->start();
	}

	m_iChange = 0;
	bool chuli = false;
	int iNow = 0;
	SetCursor( LoadCursor(NULL, IDC_WAIT) );
	MSG message;

	while (false == _lpReadThread->m_bOverFlag)
	{
		if (iNow < m_iChange )
		{
			if (0 == iNow)
			{
				lpDlg->StartProcess( 100);
				_loadProcess->ShowWindow( SW_SHOW );
				StepEx();
			}
			else if (1 == iNow)
			{

				lpDlg->m_iChangeStep = m_iChangeStep;
				StepEx();

			}
			if (0 != iNow && 1 != iNow)
			{
				StepEx();
			}
			++iNow;
		}
		if (m_iTopWindowType == TOP_WINDOWS_TYPE_MFC)
		{

			if(::PeekMessage(&message,NULL,0,0,PM_REMOVE))   

			{   
				if (message.hwnd == _loadProcess->m_hWnd)

				{
					::TranslateMessage(&message);   
					::DispatchMessage(&message);   
				}
			}
		}
		SetCursor( LoadCursor(NULL, IDC_WAIT) );
	}
	_loadProcess->ShowWindow( SW_HIDE );
	_loadProcess->UpdateData();
	Sleep(100);	
	lpDlg->m_bClose = true;
	_loadProcess->ShowWindow( SW_HIDE );
	Sleep(100);
	if(::PeekMessage(&message,NULL,0,0,PM_REMOVE))   
	{   
		::TranslateMessage(&message);   
		::DispatchMessage(&message);   
	}
	
	if ( _lpReadThread->isRunning() )
	{
		_lpReadThread->cancel();
	}


	if(NULL != _lpReadThread)
	{
		delete _lpReadThread;
		_lpReadThread = NULL;
	}

	if (_loadProcess)
	{
		delete _loadProcess;
		_loadProcess = NULL;
	}
	return true;
}


bool RadarManager::exportProject(CString strPath,CString strPROJECTCREATETIME ,bool bCmdExport,bool bShowOverDlg/* = true*/,CString strProName/*=L"导出工程"*/)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if ( !_loadProcess )
	{
		_loadProcess = new CProcessDlg;
		_loadProcess->Create( CProcessDlg::IDD );
	}
	CProcessDlg *lpDlg = (CProcessDlg*)_loadProcess;
	lpDlg->SetWindowText(strProName);
	m_iChangeStep = 5;
	m_bFlagExportOver = false;
	GD_ExOrImProjectThread *_lpReadThread = new GD_ExOrImProjectThread( this ,strPath, strPROJECTCREATETIME , bCmdExport, bShowOverDlg);

	_lpReadThread->m_bExport = true;
	if ( !_lpReadThread->isRunning() )
	{
		_lpReadThread->start();
	}

	m_iChange = 0;
	bool chuli = false;
	int iNow = 0;
	SetCursor( LoadCursor(NULL, IDC_WAIT) );
	MSG message;
	while (false == _lpReadThread->m_bOverFlag)
	{
		if (iNow < m_iChange )
		{
			if (0 == iNow)
			{
				lpDlg->StartProcess( 100);
				_loadProcess->UpdateData();
				_loadProcess->ShowWindow( SW_SHOW );
				StepEx();
			}
			else if (1 == iNow)
			{
				
				lpDlg->m_iChangeStep = m_iChangeStep;
				_loadProcess->UpdateData();
				StepEx();

			}
			if (0 != iNow && 1 != iNow)
			{
				StepEx();
			}
			++iNow;
		}
		if (m_iTopWindowType == TOP_WINDOWS_TYPE_MFC)
		{
			if(::PeekMessage(&message,NULL,0,0,PM_REMOVE))   

			{   
				if (message.hwnd == _loadProcess->m_hWnd)

				{
					::TranslateMessage(&message);   
					::DispatchMessage(&message);   
				}
			}
		}
		SetCursor( LoadCursor(NULL, IDC_WAIT) );
	}
	_loadProcess->ShowWindow( SW_HIDE );
	_loadProcess->UpdateData();
	Sleep(100);	
	lpDlg->m_bClose = true;
	_loadProcess->ShowWindow( SW_HIDE );
	Sleep(100);
	if(::PeekMessage(&message,NULL,0,0,PM_REMOVE))   
	{   
		::TranslateMessage(&message);   
		::DispatchMessage(&message);   
	}
	if ( _lpReadThread->isRunning() )
	{
		_lpReadThread->cancel();

	}
		if(NULL != _lpReadThread)
	{
		delete _lpReadThread;
		_lpReadThread = NULL;
	}
	return true;
}


bool RadarManager::exportProjectEEEX(CString strPath,CString strPROJECTCREATETIME ,bool bCmdExport /*=false*/,bool bShowOverDlg /*= true*/)
{
	//CDialogProjectExport dlg;
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	USES_CONVERSION;
	CString strFile;
	ProjectNameTab::ProjectNameRow *lpRow= NULL;

	CString cstrFolderPath = L"";
	if (true == bCmdExport)
	{
		if (strPath.GetLength() <= 0 || strPROJECTCREATETIME.GetLength() <= 0)
		{
			return false;
		}
		cstrFolderPath = strPath;
		INT64 iCreateTime = atoi(W2A(strPROJECTCREATETIME));
	
		bool flag = false;

		for (int i = 0 ; i < _projectList.size() ; ++i)
		{
			osg::ref_ptr<DBRow> _selectRow = _projectList[i];

			lpRow = dynamic_cast<ProjectNameTab::ProjectNameRow*>(_selectRow.get());

			if (lpRow->_createTime == iCreateTime)
			{
				flag = true;
				break;
			}
		}
		if (false == flag)
		{
			return false;
		}
		strFile = cstrFolderPath + L"\\" + A2W(lpRow->_projectName.c_str()) + L".pro";
	}
	else
	{
		//导出工程
		CDialogProject_Export dlg( _projectList, 3 );
		dlg.SetMyWndText(CString(_T("导出工程")));
		if ( dlg.DoModal() != IDOK )
		{
			return false;
		}

		if ( -1 == dlg.m_indexSelect)
		{
			return false;
		}
		//获取要导出的工程列表
		strFile = dlg.m_strFolderPath + L"\\" + dlg.m_selectProjectName + L".pro";
		osg::ref_ptr<DBRow> _selectRow = _projectList[dlg.m_indexSelect];
		lpRow = dynamic_cast<ProjectNameTab::ProjectNameRow*>(_selectRow.get());
		cstrFolderPath = dlg.m_strFolderPath;
	}

	if ( !lpRow )
	{
		return false;
	}
	CString strTemp;
	CString cstrCreateTime;
	
	FILE * fp0=fopen(W2A(strFile),"w");
	if (NULL == fp0)
	{
		return false;
	}
	fclose(fp0);
	m_iChange = 1;
	Sleep(300);
	::WritePrivateProfileString(_T("Project"), _T("PROJECTNAME"), A2W(lpRow->_projectName.c_str()), strFile);
	 strTemp.Format(L"%f",lpRow->_startLen);
	::WritePrivateProfileString(_T("Project"), _T("STARTLEN"),strTemp, strFile);
	 strTemp.Format(L"%f",lpRow->_curLen);
	::WritePrivateProfileString(_T("Project"), _T("CURLEN"),strTemp, strFile);
	::WritePrivateProfileString(_T("Project"), _T("PROJECTPARAM"), L"存在对应文件里", strFile);
	strTemp = A2W(lpRow->_projectName.c_str());
	strTemp += ".param";
	::WritePrivateProfileString(_T("Project"), _T("PROJECTPARAM_FileName"), strTemp, strFile);
	strTemp = cstrFolderPath + "\\" + strTemp;
	fp0=fopen(W2A(strTemp),"w");
	fprintf(fp0,"%s",lpRow->_paramXML.c_str());
	fclose(fp0);
	cstrCreateTime.Format(L"%ld",lpRow->_createTime);
	strTemp = cstrCreateTime;
	::WritePrivateProfileString(_T("Project"), _T("PROJECTCREATETIME"),strTemp, strFile);
	cstrCreateTime.Format(L"%ld",lpRow->_createTime);
	int iCountVideo = 0;
	int iCountData = 0;
	if (0 == iCountData)
	{
		DataTemplateTab tabD;
		tabD.setProjectName( W2A(cstrCreateTime) );
		VideoTemplateTab tabV;
		tabV.setProjectName( W2A(cstrCreateTime) );
		DataCountTab countTab;
		countTab.setTabName( tabV.getProjectName() );
		iCountVideo = countTab.getTabDataCount( RadarManager::Instance()->getDBHelp(), countTab.getSelectSql());
		countTab.setTabName( tabD.getProjectName() );
		iCountData = countTab.getTabDataCount( RadarManager::Instance()->getDBHelp(), countTab.getSelectSql());
		tabD.close();
		tabV.close();
	}
	int iFileRadarCount = iCountData / m_iAFileRadarLine;
	if (0 != iCountData % m_iAFileRadarLine)
	{
		iFileRadarCount++;
	}
	strTemp.Format(L"%d",iFileRadarCount);
	::WritePrivateProfileString(_T("Data"), _T("DataFileCount"), strTemp, strFile);

	CString strSqlTabName = L"DATA_" + cstrCreateTime + L"_TAB";
	::WritePrivateProfileString(_T("Data"), _T("SQLDataTabName"), strSqlTabName, strFile);
	CString strDataFileName = strSqlTabName ;
	::WritePrivateProfileString(_T("Data"), _T("DataFileName"), strDataFileName, strFile);
	strSqlTabName = L"VIDEO_" + cstrCreateTime + L"_TAB";
	::WritePrivateProfileString(_T("Data"), _T("SQLVideoTabName"), strSqlTabName, strFile);
	CString strVideoFileName = strSqlTabName + L".da";
	::WritePrivateProfileString(_T("Data"), _T("VideoFileName"), strVideoFileName, strFile);
	CString strImageFileName = cstrCreateTime;
	::WritePrivateProfileString(_T("Data"), _T("ImageFileName"), strImageFileName, strFile);
	ConfigureSet set;
	set.fromGroupXML(lpRow->_paramXML);
	int iSmpNum = atoi ( set.get("radar", "sample").c_str());
	iSmpNum = getSampCount( iSmpNum );
	m_iChangeStep = 100 / (iFileRadarCount + 3);
	m_iChange++;
	Sleep(300);

	//获取exe路径 
	{
		/*TCHAR szAppPath[MAX_PATH];
		::GetModuleFileName(AfxGetInstanceHandle(), szAppPath, MAX_PATH);
		::PathRemoveFileSpec(szAppPath);
		CString strFromPath = szAppPath;
		strFromPath += L"\\ProjectData\\DataImage\\" + cstrCreateTime;
		CString strToPath = cstrFolderPath + "\\" + cstrCreateTime;*/
	}
	CString strFromPath;
	strFromPath = m_strImagePath.c_str();
	strFromPath += cstrCreateTime;
	CString strToPath = cstrFolderPath + "\\" + cstrCreateTime;
	
	if (!PathIsDirectory(strToPath) ) 
	{ 
		if (!CreateDirectory(strToPath, NULL ) ) 
		{ 
			CString strMsg; 
			strMsg.Format (L"创建路径\"%s\"保存图片的文件夹失败！是否继续?",strToPath); 
			if (AfxMessageBox(strMsg, MB_YESNO) == IDYES) 
				return false;
		} 
	} 
	string strCopy = "copy ";
	strCopy += W2A(strFromPath);
	strCopy += "\\*.* ";
	strCopy += W2A(strToPath);
	system(strCopy.c_str());
	++m_iChange ;
	Sleep(300);
	exportSqlVideoIni(W2A(cstrCreateTime),W2A(strVideoFileName),W2A(cstrFolderPath.GetBuffer()));
	++m_iChange ;
	Sleep(300);
	exportSqlDataRadarIni(W2A(cstrCreateTime),W2A(strDataFileName),W2A(cstrFolderPath),iSmpNum,iFileRadarCount );
	++m_iChange ;
	Sleep(300);
	if (true == bShowOverDlg)
	{
		::MessageBox(NULL,L"导出工程成功!",L"导出工程", MB_OK); 
	}
	return true;
}

bool RadarManager::exportSqlVideoIni(std::string strTime,std::string strFileName,std::string strFilePath)
{
	VideoTemplateTab tab;
	tab.setProjectName( strTime );
	CDBTable::RowVector rv;
	std::string strSQL =  tab.getSelectSql();
	strSQL += " order by id";
	if ( !tab.open( RadarManager::Instance()->getDBHelp(), strSQL) )
	{
		return false;
	}
	strFilePath += "\\" + strFileName;
	bool flag = tab.exportSqlDataVideoIni(strFilePath);
	tab.close();
	return flag;
}

bool RadarManager::exportProjectAllFile()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CDialogProject_Export dlg( _projectList, 2 );
	dlg.SetMyWndText(CString(_T("导出SGY文件")));
	if ( dlg.DoModal() != IDOK )
	{
		return false;
	}
	return true;
}

bool RadarManager::exportAnyCountProject()
{

	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CDialogProject_Export dlg( _projectList, 3 );
	dlg.SetMyWndText(CString(_T("导出工程")));
	if ( dlg.DoModal() != IDOK )
	{
		return false;
	}



	CString strWndText;
	int iCount = dlg.m_vPath.size();
	for (int i = 0; i < iCount;++i)
	{
		strWndText.Format(L"共%d个工程，正在导出第%d个:",iCount,(i+1));
		strWndText += dlg.m_vProName[i];
		exportProject(dlg.m_vPath[i],dlg.m_vProTime[i],true,false,strWndText);
	}

	if (0 != iCount)
	{
		::MessageBox(NULL,L"导出工程成功!",L"导出工程", MB_OK); 
	}
	return TRUE;
}

char getParityBit(int index){
	string tempArr="NOEMS";
	return tempArr[index];
}

bool RadarManager::startNewWork()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if ( _working ){
		return true;
	}

	if ( _testing ){
		stopTest();
	}

	_radarReader.close();//20210621hjl 保证已经终止采集

	if (NULL == m_dlgPromptNULL){
		m_dlgPromptNULL = new CDlgPromptMessage;
		CString _strTite = _T("提示信息");
		CString _strInfo = _T("从雷达主机处读取数据连续200次为空。\r\n雷达主机没有数据,主机没有正常工作。\r\n建议停止工作重启系统或者检查设备！！");;
		CString _strOK = _T("确定");;
		CString _strCancle = _T("取消");;
		m_dlgPromptNULL->SetPromptMessage( _strTite, _strInfo, _strOK, _strCancle);
		m_dlgPromptNULL->Create(CDlgPromptMessage::IDD);
	}
	USES_CONVERSION;
	CDialogProject dlg( _projectList, 1 );
	if ( dlg.DoModal() != IDOK ){
		//::AfxMessageBox(L"没有选择工程, 不能开始工作" );
		return false;
	}
	std::string projectName = W2A( dlg.m_selectProjectName );
	std::string projectTab = W2A( dlg.m_selectProjectTab );
	m_strLastProjectCreatTime = projectTab;
	float curLen = dlg.m_selectLen;
	std::string strPath;
	OpenThreads::ScopedLock<OpenThreads::Mutex> lock(_lock);
	switch (RadarManager::Instance()->GetRadarWorkType()){
		case RADAR_WORK_TYPE_ONE_USB:
			if ( m_configureSet->get("com", "use") == "true"){
				if ( !_gpsReader.open( 
					atoi(m_configureSet->get("com","port").c_str())
					,atoi(m_configureSet->get("com","baud").c_str())
					,m_configureSet->get("com","parity").length() > 0 ? getParityBit(atoi(m_configureSet->get("com","parity").c_str())) : 'N'
					,atoi(m_configureSet->get("com","databits").c_str())
					,atoi(m_configureSet->get("com","stopbits").c_str())
					) )
				{
					::AfxMessageBox(L"连接GPS串口失败!" );
					_gpsReader.close();
				}
			}
			if (false == _radarReader.open(m_iRadarWorkType)){

				::AfxMessageBox(L"连接雷达设备失败，请检查设备是否正常连接或重启设备" );
				_gpsReader.close();
				return false;
			}
			if ( _lpCurProject ){
				delete _lpCurProject;
			}
			//OpenThreads::ScopedLock<OpenThreads::Mutex> lock(_lock);
			_lpCurProject = new MeasureProject;
			_lpCurProject->setProjectName( projectName );
			_lpCurProject->setProjectTab( projectTab );
			_lpCurProject->setProjectRow( dlg._selectRow.get() );
			strPath = m_strImagePath + projectTab;
			if (!PathIsDirectory(A2W ( strPath.c_str() ) ) ){
				CreateDirectory(A2W(strPath.c_str()),NULL);
			}
			_lpCurProject->SetSaveOracleOrFile(m_iRadarWorkType,m_iSaveOracle,m_strSavePath);
			break;

		case RADAR_WORK_TYPE_DOUBLE_USB:
			if ( m_configureSet->get("com", "use") == "true"){
				if ( !_gpsReader.open( 
					atoi(m_configureSet->get("com","port").c_str())
					,atoi(m_configureSet->get("com","baud").c_str())
					,m_configureSet->get("com","parity").length() > 0 ? getParityBit(atoi(m_configureSet->get("com","parity").c_str())) : 'N'
					,atoi(m_configureSet->get("com","databits").c_str())
					,atoi(m_configureSet->get("com","stopbits").c_str())
					) )
				{
					::AfxMessageBox(L"连接GPS串口失败!" );
					_gpsReader.close();
				}
			}

			if (false == _radarReader.open(m_iRadarWorkType)){
				::AfxMessageBox(L"连接雷达设备失败，请检查设备是否正常连接或重启设备" );
				_gpsReader.close();
				return false;
			}
			if ( _lpCurProject ){
				delete _lpCurProject;
			}
			_lpCurProject = new MeasureProject;
			_lpCurProject->setProjectName( projectName );
			_lpCurProject->setProjectTab( projectTab );
			_lpCurProject->setProjectRow( dlg._selectRow.get() );
			strPath = m_strImagePath + projectTab;
			if (!PathIsDirectory(A2W ( strPath.c_str() ) ) ){
				CreateDirectory(A2W(strPath.c_str()),NULL);
			}
			_lpCurProject->SetSaveOracleOrFile(m_iRadarWorkType,m_iSaveOracle,m_strSavePath);
			break;

		case RADAR_WORK_TYPE_DOUBLE_USB_OLD:
			if ( m_configureSet->get("com", "use") == "true"){
				if ( !_gpsReader.open( 
					atoi(m_configureSet->get("com","port").c_str())
					,atoi(m_configureSet->get("com","baud").c_str())
					,m_configureSet->get("com","parity").length() > 0 ? getParityBit(atoi(m_configureSet->get("com","parity").c_str())) : 'N'
					,atoi(m_configureSet->get("com","databits").c_str())
					,atoi(m_configureSet->get("com","stopbits").c_str())
					) )
				{
					::AfxMessageBox(L"连接GPS串口失败!" );
					_gpsReader.close();
				}
			}
			if (false == _radarReader.open(m_iRadarWorkType)){
				::AfxMessageBox(L"连接雷达设备失败，请检查设备是否正常连接或重启设备" );
				_gpsReader.close();
				return false;
			}
			if ( _lpCurProject ){
				delete _lpCurProject;
			}
			_lpCurProject = new MeasureProject;
			_lpCurProject->setProjectName( projectName );
			_lpCurProject->setProjectTab( projectTab );
			_lpCurProject->setProjectRow( dlg._selectRow.get() );
			strPath = m_strImagePath + projectTab;
			if (!PathIsDirectory(A2W ( strPath.c_str() ) ) ){
				CreateDirectory(A2W(strPath.c_str()),NULL);
			}
			_lpCurProject->SetSaveOracleOrFile(m_iRadarWorkType,m_iSaveOracle,m_strSavePath);
			break;

		case RADAR_WORK_TYPE_FOUR_USB:
			if ( m_configureSet->get("com", "use") == "true"){
				if ( !_gpsReader.open( 
					atoi(m_configureSet->get("com","port").c_str())
					,atoi(m_configureSet->get("com","baud").c_str())
					,m_configureSet->get("com","parity").length() > 0 ? getParityBit(atoi(m_configureSet->get("com","parity").c_str())) : 'N'
					,atoi(m_configureSet->get("com","databits").c_str())
					,atoi(m_configureSet->get("com","stopbits").c_str())
					) )
				{
					::AfxMessageBox(L"连接GPS串口失败!" );
					_gpsReader.close();
				}
			}
			if (false == _radarReader.open(m_iRadarWorkType)){

				::AfxMessageBox(L"连接雷达设备失败，请检查设备是否正常连接或重启设备" );
				_gpsReader.close();
				return false;
			}
			if ( _lpCurProject ){
				delete _lpCurProject;
			}
			_lpCurProject = new MeasureProject;
			_lpCurProject->setProjectName( projectName );
			_lpCurProject->setProjectTab( projectTab );
			_lpCurProject->setProjectRow( dlg._selectRow.get() );
			strPath = m_strImagePath + projectTab;
			if (!PathIsDirectory(A2W ( strPath.c_str() ) ) ){
				CreateDirectory(A2W(strPath.c_str()),NULL);
			}
			_lpCurProject->SetSaveOracleOrFile(m_iRadarWorkType,m_iSaveOracle,m_strSavePath);
			break;

		case RADAR_WORK_TYPE_EIGHT:
			//if ( m_configureSet->get("com", "use") == "true"){
			if ( RadarManager::Instance()->getShowGpsPos()){
				/*if ( !_gpsReader.open( 
					atoi(m_configureSet->get("com","port").c_str())
					,atoi(m_configureSet->get("com","baud").c_str())
					,m_configureSet->get("com","parity").length() > 0 ? getParityBit(atoi(m_configureSet->get("com","parity").c_str())) : 'N'
					,atoi(m_configureSet->get("com","databits").c_str())
					,atoi(m_configureSet->get("com","stopbits").c_str())
					) )
				{
					//正规是下面两行------------
					::AfxMessageBox(L"连接GPS串口失败!" );
					_gpsReader.close();
					//------------
				}*/
				int connectGpsFailCount=0;
				while ( !_gpsReader.open( 
					atoi(m_configureSet->get("com","port").c_str())
					,atoi(m_configureSet->get("com","baud").c_str())
					,m_configureSet->get("com","parity").length() > 0 ? getParityBit(atoi(m_configureSet->get("com","parity").c_str())) : 'N'
					,atoi(m_configureSet->get("com","databits").c_str())
					,atoi(m_configureSet->get("com","stopbits").c_str())
					) )
				{
					connectGpsFailCount=connectGpsFailCount+1;
					_gpsReader.close();
					if(connectGpsFailCount>10){
						::AfxMessageBox(L"连接GPS串口失败!" );
						break;
					}
					Sleep(100);
				}
			}
			if ( m_configureSet->get("net", "use") == "true"){
				/*if ( !_radarReader.open(
					m_configureSet->get("net","addr")
					,atoi(m_configureSet->get("net","port").c_str())
					) )
				{
					::AfxMessageBox(L"连接雷达设备失败，请检查设备是否正常连接或重启设备" );
					_gpsReader.close();
					return false;
				}*/
				int connectRadarFailCount=0;
				while ( !_radarReader.open(
					m_configureSet->get("net","addr")
					,atoi(m_configureSet->get("net","port").c_str())
					) )
				{
					connectRadarFailCount=connectRadarFailCount+1;
					if(connectRadarFailCount>10){
						_gpsReader.close();
						::AfxMessageBox(L"连接雷达设备失败，请检查设备是否正常连接或重启设备" );
						return false;
					}
				}
			}
			if ( _lpCurProject ){
				delete _lpCurProject;
			}
			//OpenThreads::ScopedLock<OpenThreads::Mutex> lock(_lock);
			_lpCurProject = new MeasureProject;
			_lpCurProject->setProjectName( projectName );
			_lpCurProject->setProjectTab( projectTab );
			_lpCurProject->setProjectRow( dlg._selectRow.get() );
			strPath = m_strImagePath + projectTab;
			if (!PathIsDirectory(A2W ( strPath.c_str() ) ) ){
				CreateDirectory(A2W(strPath.c_str()),NULL);
			}
			_lpCurProject->SetSaveOracleOrFile(m_iRadarWorkType,m_iSaveOracle,m_strSavePath);
			break;

		default:
			break;
	}
	addGpsModel();
	CmdNewLine();
	if(m_nUpload==0){
		showCurCurve(A2W(projectName.c_str()));
	}
	_working = true;
	_radarReader.ReadThreadStart(true);
	
	return true;
}

string to_String(int n)
{
    int m = n;
    char s[100];
    char ss[100];
    int i=0,j=0;
    if (n < 0)// 处理负数
    {
        m = 0 - m;
        j = 1;
        ss[0] = '-';
    }    
    while (m>0)
    {
        s[i++] = m % 10 + '0';
        m /= 10;
    }
    s[i] = '\0';
    i = i - 1;
    while (i >= 0)
    {
        ss[j++] = s[i--];
    }    
    ss[j] = '\0';    
    return ss;
}

bool RadarManager::startNewWorkWithoutProjectName()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if ( _working ){
		return true;
	}

	if ( _testing ){
		stopTest();
	}

	_radarReader.close();//20210621hjl 保证已经终止采集

	if (NULL == m_dlgPromptNULL){
		m_dlgPromptNULL = new CDlgPromptMessage;
		CString _strTite = _T("提示信息");
		CString _strInfo = _T("从雷达主机处读取数据连续200次为空。\r\n雷达主机没有数据,主机没有正常工作。\r\n建议停止工作重启系统或者检查设备！！");
		CString _strOK = _T("确定");
		CString _strCancle = _T("取消");
		m_dlgPromptNULL->SetPromptMessage( _strTite, _strInfo, _strOK, _strCancle);
		m_dlgPromptNULL->Create(CDlgPromptMessage::IDD);
	}
	USES_CONVERSION;
	
	CDialogProject dlg( _projectList, 1 );
	/*
	if ( dlg.DoModal() != IDOK ){
		//::AfxMessageBox(L"没有选择工程, 不能开始工作" );
		return false;
	}*/
	dlg.initForNoProjectName();
	time_t nowtime;
	//首先创建一个time_t 类型的变量nowtime
	struct tm* p;
	//然后创建一个新时间结构体指针 p 
	time(&nowtime);
	//使用该函数就可得到当前系统时间，使用该函数需要将传入time_t类型变量nowtime的地址值。
	p = localtime(&nowtime);

	std::string projectName = "GD";
	std::string projectTab = to_String(p->tm_year+1900);
	if(p->tm_mon+1<10){
		projectTab=projectTab+"0"+to_String(p->tm_mon+1);
	}else{
		projectTab=projectTab+to_String(p->tm_mon+1);
	}
	if(p->tm_mday<10){
		projectTab=projectTab+"0"+to_String(p->tm_mday);
	}else{
		projectTab=projectTab+to_String(p->tm_mday);
	}
	
	if(p->tm_hour==0){
		projectTab=projectTab+"00";
	}else if(p->tm_hour<10){
		projectTab=projectTab+"0"+to_String(p->tm_hour);
	}else{
		projectTab=projectTab+to_String(p->tm_hour);
	}
	if(p->tm_min==0){
		projectTab=projectTab+"00";
	}else if(p->tm_min<10){
		projectTab=projectTab+"0"+to_String(p->tm_min);
	}else{
		projectTab=projectTab+to_String(p->tm_min);
	}
	if(p->tm_sec==0){
		projectTab=projectTab+"00";
	}else if(p->tm_sec<10){
		projectTab=projectTab+"0"+to_String(p->tm_sec);
	}else{
		projectTab=projectTab+to_String(p->tm_sec);
	}

	m_strLastProjectCreatTime = projectTab;
	//float curLen = dlg.m_selectLen;
	std::string strPath;
	OpenThreads::ScopedLock<OpenThreads::Mutex> lock(_lock);
	int test =RadarManager::Instance()->GetRadarWorkType();
	switch (RadarManager::Instance()->GetRadarWorkType()){
		case RADAR_WORK_TYPE_ONE_USB:
			if ( m_configureSet->get("com", "use") == "true"){
				if ( !_gpsReader.open( 
					atoi(m_configureSet->get("com","port").c_str())
					,atoi(m_configureSet->get("com","baud").c_str())
					,m_configureSet->get("com","parity").length() > 0 ? getParityBit(atoi(m_configureSet->get("com","parity").c_str())) : 'N'
					,atoi(m_configureSet->get("com","databits").c_str())
					,atoi(m_configureSet->get("com","stopbits").c_str())
					) )
				{
					::AfxMessageBox(L"连接GPS串口失败!" );
					_gpsReader.close();
				}
			}
			if (false == _radarReader.open(m_iRadarWorkType)){

				::AfxMessageBox(L"连接雷达设备失败，请检查设备是否正常连接或重启设备" );
				_gpsReader.close();
				return false;
			}
			if ( _lpCurProject ){
				delete _lpCurProject;
			}
			//OpenThreads::ScopedLock<OpenThreads::Mutex> lock(_lock);
			_lpCurProject = new MeasureProject;
			_lpCurProject->setProjectName( projectName );
			_lpCurProject->setProjectTab( projectTab );
			_lpCurProject->setProjectRow( dlg._selectRow.get() );
			strPath = m_strImagePath + projectTab;
			if (!PathIsDirectory(A2W ( strPath.c_str() ) ) ){
				CreateDirectory(A2W(strPath.c_str()),NULL);
			}
			_lpCurProject->SetSaveOracleOrFile(m_iRadarWorkType,m_iSaveOracle,m_strSavePath);
			break;

		case RADAR_WORK_TYPE_DOUBLE_USB:
			if ( m_configureSet->get("com", "use") == "true"){
				if ( !_gpsReader.open( 
					atoi(m_configureSet->get("com","port").c_str())
					,atoi(m_configureSet->get("com","baud").c_str())
					,m_configureSet->get("com","parity").length() > 0 ? getParityBit(atoi(m_configureSet->get("com","parity").c_str())) : 'N'
					,atoi(m_configureSet->get("com","databits").c_str())
					,atoi(m_configureSet->get("com","stopbits").c_str())
					) )
				{
					::AfxMessageBox(L"连接GPS串口失败!" );
					_gpsReader.close();
				}
			}

			if (false == _radarReader.open(m_iRadarWorkType)){
				::AfxMessageBox(L"连接雷达设备失败，请检查设备是否正常连接或重启设备" );
				_gpsReader.close();
				return false;
			}
			if ( _lpCurProject ){
				delete _lpCurProject;
			}
			_lpCurProject = new MeasureProject;
			_lpCurProject->setProjectName( projectName );
			_lpCurProject->setProjectTab( projectTab );
			_lpCurProject->setProjectRow( dlg._selectRow.get() );
			strPath = m_strImagePath + projectTab;
			if (!PathIsDirectory(A2W ( strPath.c_str() ) ) ){
				CreateDirectory(A2W(strPath.c_str()),NULL);
			}
			_lpCurProject->SetSaveOracleOrFile(m_iRadarWorkType,m_iSaveOracle,m_strSavePath);
			break;

		case RADAR_WORK_TYPE_DOUBLE_USB_OLD:
			if ( m_configureSet->get("com", "use") == "true"){
				if ( !_gpsReader.open( 
					atoi(m_configureSet->get("com","port").c_str())
					,atoi(m_configureSet->get("com","baud").c_str())
					,m_configureSet->get("com","parity").length() > 0 ? getParityBit(atoi(m_configureSet->get("com","parity").c_str())) : 'N'
					,atoi(m_configureSet->get("com","databits").c_str())
					,atoi(m_configureSet->get("com","stopbits").c_str())
					) )
				{
					::AfxMessageBox(L"连接GPS串口失败!" );
					_gpsReader.close();
				}
			}
			if (false == _radarReader.open(m_iRadarWorkType)){
				::AfxMessageBox(L"连接雷达设备失败，请检查设备是否正常连接或重启设备" );
				_gpsReader.close();
				return false;
			}
			if ( _lpCurProject ){
				delete _lpCurProject;
			}
			_lpCurProject = new MeasureProject;
			_lpCurProject->setProjectName( projectName );
			_lpCurProject->setProjectTab( projectTab );
			_lpCurProject->setProjectRow( dlg._selectRow.get() );
			strPath = m_strImagePath + projectTab;
			if (!PathIsDirectory(A2W ( strPath.c_str() ) ) ){
				CreateDirectory(A2W(strPath.c_str()),NULL);
			}
			_lpCurProject->SetSaveOracleOrFile(m_iRadarWorkType,m_iSaveOracle,m_strSavePath);
			break;

		case RADAR_WORK_TYPE_FOUR_USB:
			if ( m_configureSet->get("com", "use") == "true"){
				if ( !_gpsReader.open( 
					atoi(m_configureSet->get("com","port").c_str())
					,atoi(m_configureSet->get("com","baud").c_str())
					,m_configureSet->get("com","parity").length() > 0 ? getParityBit(atoi(m_configureSet->get("com","parity").c_str())) : 'N'
					,atoi(m_configureSet->get("com","databits").c_str())
					,atoi(m_configureSet->get("com","stopbits").c_str())
					) )
				{
					::AfxMessageBox(L"连接GPS串口失败!" );
					_gpsReader.close();
				}
			}
			if (false == _radarReader.open(m_iRadarWorkType)){

				::AfxMessageBox(L"连接雷达设备失败，请检查设备是否正常连接或重启设备" );
				_gpsReader.close();
				return false;
			}
			if ( _lpCurProject ){
				delete _lpCurProject;
			}
			_lpCurProject = new MeasureProject;
			_lpCurProject->setProjectName( projectName );
			_lpCurProject->setProjectTab( projectTab );
			_lpCurProject->setProjectRow( dlg._selectRow.get() );
			strPath = m_strImagePath + projectTab;
			if (!PathIsDirectory(A2W ( strPath.c_str() ) ) ){
				CreateDirectory(A2W(strPath.c_str()),NULL);
			}
			_lpCurProject->SetSaveOracleOrFile(m_iRadarWorkType,m_iSaveOracle,m_strSavePath);
			break;

		case RADAR_WORK_TYPE_EIGHT:
			//if ( m_configureSet->get("com", "use") == "true"){
			if (RadarManager::Instance()->getShowGpsPos()){
				/*if ( !_gpsReader.open( 
					atoi(m_configureSet->get("com","port").c_str())
					,atoi(m_configureSet->get("com","baud").c_str())
					,m_configureSet->get("com","parity").length() > 0 ? getParityBit(atoi(m_configureSet->get("com","parity").c_str())) : 'N'
					,atoi(m_configureSet->get("com","databits").c_str())
					,atoi(m_configureSet->get("com","stopbits").c_str())
					) )
				{
					//正规是下面两行------------
					::AfxMessageBox(L"连接GPS串口失败!" );
					_gpsReader.close();
					//------------
				}*/
				int connectGpsFailCount=0;
				while ( !_gpsReader.open( 
					atoi(m_configureSet->get("com","port").c_str())
					,atoi(m_configureSet->get("com","baud").c_str())
					,m_configureSet->get("com","parity").length() > 0 ? getParityBit(atoi(m_configureSet->get("com","parity").c_str())) : 'N'
					,atoi(m_configureSet->get("com","databits").c_str())
					,atoi(m_configureSet->get("com","stopbits").c_str())
					) )
				{
					connectGpsFailCount=connectGpsFailCount+1;
					_gpsReader.close();
					if(connectGpsFailCount>10){
						::AfxMessageBox(L"连接GPS串口失败!" );
						break;
					}
					Sleep(100);
				}
			}
			if ( m_configureSet->get("net", "use") == "true"){
				/*if ( !_radarReader.open(
					m_configureSet->get("net","addr")
					,atoi(m_configureSet->get("net","port").c_str())
					) )
				{
					::AfxMessageBox(L"连接雷达设备失败，请检查设备是否正常连接或重启设备" );
					_gpsReader.close();
					return false;
				}*/
				int connectRadarFailCount=0;
				while ( !_radarReader.open(
					m_configureSet->get("net","addr")
					,atoi(m_configureSet->get("net","port").c_str())
					) )
				{
					connectRadarFailCount=connectRadarFailCount+1;
					if(connectRadarFailCount>10){
						_gpsReader.close();
						::AfxMessageBox(L"连接雷达设备失败，请检查设备是否正常连接或重启设备" );
						return false;
					}
				}
			}
			if ( _lpCurProject ){
				delete _lpCurProject;
			}
			//OpenThreads::ScopedLock<OpenThreads::Mutex> lock(_lock);
			_lpCurProject = new MeasureProject;
			_lpCurProject->setProjectName( projectName );
			_lpCurProject->setProjectTab( projectTab );
			_lpCurProject->setProjectRow( dlg._selectRow.get() );
			strPath = m_strImagePath + projectTab;
			if (!PathIsDirectory(A2W ( strPath.c_str() ) ) ){
				CreateDirectory(A2W(strPath.c_str()),NULL);
			}
			_lpCurProject->SetSaveOracleOrFile(m_iRadarWorkType,m_iSaveOracle,m_strSavePath);
			break;

		default:
			break;
	}
	addGpsModel();
	CmdNewLine();
	/*if(m_nUpload==0){
		showCurCurve(A2W(projectName.c_str()));
	}*/
	showCurCurve(A2W(projectName.c_str()));//temp
	_working = true;
	_radarReader.ReadThreadStart(true);
	
	return true;
}

bool RadarManager::startNewTest()
{
	stopTest();
	_radarReader.close();//20210621hjl 保证已经终止采集
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if ( _testing ){
		return true;
	}
	if (NULL == m_dlgPromptNULL){
		m_dlgPromptNULL = new CDlgPromptMessage;
		CString _strTite = _T("提示信息");
		CString _strInfo = _T("从雷达主机处读取数据连续200次为空。\r\n雷达主机没有数据,主机没有正常工作。\r\n建议停止工作重启系统或者检查设备！！");;
		CString _strOK = _T("确定");;
		CString _strCancle = _T("取消");;
		m_dlgPromptNULL->SetPromptMessage( _strTite, _strInfo, _strOK, _strCancle);
		m_dlgPromptNULL->Create(CDlgPromptMessage::IDD);
	}
	USES_CONVERSION;
	/*
	CDialogProject dlg( _projectList, 1 );
	dlg.initForNoProjectName();
	time_t nowtime;
	//首先创建一个time_t 类型的变量nowtime
	struct tm* p;
	//然后创建一个新时间结构体指针 p 
	time(&nowtime);
	//使用该函数就可得到当前系统时间，使用该函数需要将传入time_t类型变量nowtime的地址值。
	p = localtime(&nowtime);

	std::string projectName = "test";
	std::string projectTab = to_String(p->tm_hour)+to_String(p->tm_min)+to_String(p->tm_sec);
	m_strLastProjectCreatTime = projectTab;
	//float curLen = dlg.m_selectLen;
	std::string strPath;*/
	OpenThreads::ScopedLock<OpenThreads::Mutex> lock(_lock);
	if ( RadarManager::Instance()->getShowGpsPos()){
		if ( !_gpsReader.open( 
			atoi(m_configureSet->get("com","port").c_str())
			,atoi(m_configureSet->get("com","baud").c_str())
			,m_configureSet->get("com","parity").length() > 0 ? getParityBit(atoi(m_configureSet->get("com","parity").c_str())) : 'N'
			,atoi(m_configureSet->get("com","databits").c_str())
			,atoi(m_configureSet->get("com","stopbits").c_str())
			) )
		{
			//正规是下面两行------------
			::AfxMessageBox(L"连接GPS串口失败!" );
			_gpsReader.close();
			//------------
		}
	}
	if ( m_configureSet->get("net", "use") == "true"){
		if ( !_radarReader.openForSetting(
			m_configureSet->get("net","addr")
			,atoi(m_configureSet->get("net","port").c_str())
			) )
		{
			::AfxMessageBox(L"连接雷达设备失败，请检查设备是否正常连接或重启设备" );
			_gpsReader.close();
			return false;
		}
	}
	for(int i=0;i<16;i++){
		g_RadarParameterConfig->changeChannel(i,true);
	}
	_testing = true;
	_radarReader.ReadThreadStart(true);
	
	return true;
}

bool RadarManager::stopWork()
{
 	if ( !_working )
 	{
 		return true;
 	}

	HideDlgPromptNULL();
	CmdDelLine();

	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	//if ( m_configureSet->get("com", "use") == "true")
	if ( RadarManager::Instance()->getShowGpsPos())
	{
		_gpsReader.close();
	}
	if ( m_configureSet->get("net", "use") == "true")
	{
		_radarReader.close();
	}

	if (0 != m_iSaveOracle)
	{
		if (m_strLastProjectCreatTime.size() > 0)
		{
			if (dbOpen())
			{
				DataCountTab countTab;
				DataTemplateTab tabDTemp;
				tabDTemp.setProjectName( m_strLastProjectCreatTime);
				countTab.setTabName( tabDTemp.getProjectName()  );
				int count = countTab.getTabDataCount( &_dbHelp, countTab.getSelectSql());
				if (0 == count)
				{
					::AfxMessageBox(_T("雷达数据一道也没有保存到数据库中!!!"));
				}
			}
		}
	}

	if (g_CurCurveFirstDlg ){
		delete g_CurCurveFirstDlg;
		g_CurCurveFirstDlg = NULL;
	}
	if (g_CurCurveSecondDlg){
		delete g_CurCurveSecondDlg;
		g_CurCurveSecondDlg = NULL;
	}
	if (g_CurCurveThirdDlg){
		delete g_CurCurveThirdDlg;
		g_CurCurveThirdDlg = NULL;
	}
	if (g_CurCurveFourthDlg){
		delete g_CurCurveFourthDlg;
		g_CurCurveFourthDlg = NULL;
	}

	if (g_ThreeViewDlg){
		delete g_ThreeViewDlg;
		g_ThreeViewDlg = NULL;
	}

	_lpCurProject = NULL;

	_working = false;

	removeGpsModel();
	return true;

}

bool RadarManager::stopTest()
{
 	if ( !_testing )
 	{
 		return true;
 	}
	/*
	HideDlgPromptNULL();
	CmdDelLine();

	AFX_MANAGE_STATE(AfxGetStaticModuleState());*/

	
	if ( RadarManager::Instance()->getShowGpsPos()){
		_gpsReader.close();
	}
	if ( m_configureSet->get("net", "use") == "true"){
		_radarReader.close();
	}

	_testing = false;

	return true;

}

void RadarManager::addGpsModel()
{
	osg::ref_ptr<Data::CRowShowObj> pRowShowObj = new Data::CRowShowObj;
	osg::Vec3d posXYZ;
	pRowShowObj->ID = GPS_OBJ_ID;
	pRowShowObj->m_EnterID = 0;
	pRowShowObj->m_TypeID = 12;
	pRowShowObj->m_ParentID = 0;	
	pRowShowObj->m_Name = _T("GPSMODEL");
	pRowShowObj->m_LodConID = 0;//4;
	pRowShowObj->m_Visable = TRUE;

	CString objProperty;

	//objProperty.Format(_T("<屏幕标注><图片名称>green.png</图片名称>\<对齐方式 Type=\"CENTER\"></对齐方式>\<图片大小 Height=\"24.00\" Type=\"自定义\" Width=\"24.00\"></图片大小>\<图片偏移 offsetX=\"0.00\" offsetY=\"0.00\"></图片偏移>\<文本内容>工程测量车</文本内容><样式模板 NAME=\"\"></样式模板>\<字体 FONT=\"SIMHEI.TTF\"></字体><原点对齐方式 ALIGN=\"LEFT_CENTER\"></原点对齐方式>\<背景颜色 A=\"100\" B=\"0\" G=\"0\" R=\"0\"></背景颜色>\<背景类型 OFFSETX=\"0.00\" OFFSETY=\"0.00\" T=\"OUTLINE\"></背景类型>\<分辨率 HEIGHT=\"128\" WIDTH=\"128\"></分辨率><文字大小 S=\"24.00\"></文字大小>\<文字颜色 A=\"100\" B=\"128\" G=\"255\" R=\"255\"></文字颜色>\<文字偏移 offsetX=\"12.00\" offsetY=\"0.00\"></文字偏移>\<空间位置 X=\"114.436511123\" Y=\"22.576865392\" Z=\"39.551404576\"></空间位置>\</屏幕标注>"));
	
	objProperty.Format(_T("<屏幕标注><图片名称>green.png</图片名称>\
						<对齐方式 Type=\"CENTER\"></对齐方式>\
						<图片大小 Height=\"24.00\" Type=\"自定义\" Width=\"24.00\"></图片大小>\
						<图片偏移 offsetX=\"0.00\" offsetY=\"0.00\"></图片偏移>\
						<文本内容>工程测量车</文本内容><样式模板 NAME=\"\"></样式模板>\
						<字体 FONT=\"SIMHEI.TTF\"></字体><原点对齐方式 ALIGN=\"LEFT_CENTER\"></原点对齐方式>\
						<背景颜色 A=\"100\" B=\"0\" G=\"0\" R=\"0\"></背景颜色>\
						<背景类型 OFFSETX=\"0.00\" OFFSETY=\"0.00\" T=\"OUTLINE\"></背景类型>\
						<分辨率 HEIGHT=\"128\" WIDTH=\"128\"></分辨率><文字大小 S=\"24.00\"></文字大小>\
						<文字颜色 A=\"100\" B=\"128\" G=\"255\" R=\"255\"></文字颜色>\
						<文字偏移 offsetX=\"12.00\" offsetY=\"0.00\"></文字偏移>\
						<空间位置 X=\"114.436511123\" Y=\"22.576865392\" Z=\"39.551404576\"></空间位置>\
						</屏幕标注>")
						);
						
	/*
	objProperty.Format(_T("<屏幕标注>\
						<图片名称>green.png</图片名称>\
						<对齐方式 Type=\"CENTER\"></对齐方式>\
						<图片大小 Height=\"24.00\" Type=\"自定义\" Width=\"24.00\"></图片大小>\
						<图片偏移 offsetX=\"0.00\" offsetY=\"0.00\"></图片偏移>\
						<文本内容>工程测量车</文本内容><样式模板 NAME=\"\"></样式模板>\
						<字体 FONT=\"SIMHEI.TTF\"></字体><原点对齐方式 ALIGN=\"LEFT_CENTER\"></原点对齐方式>\
						<背景颜色 A=\"100\" B=\"0\" G=\"0\" R=\"0\"></背景颜色>\
						<背景类型 OFFSETX=\"0.00\" OFFSETY=\"0.00\" T=\"OUTLINE\"></背景类型>\
						<分辨率 HEIGHT=\"128\" WIDTH=\"128\"></分辨率><文字大小 S=\"24.00\"></文字大小>\
						<文字颜色 A=\"100\" B=\"128\" G=\"255\" R=\"255\"></文字颜色>\
						<文字偏移 offsetX=\"12.00\" offsetY=\"0.00\"></文字偏移>\
						<空间位置 X=\"114.436511123\" Y=\"22.576865392\" Z=\"39.551404576\"></空间位置>\
						</屏幕标注>")
						);
						*/

 	pRowShowObj->m_Property = objProperty;

	EMObj::CmdNewObjFromWeb(pRowShowObj);
}

void RadarManager::removeGpsModel()
{
	EMObj::CmdDelObj( GPS_OBJ_ID );
}

void RadarManager::clear()
{
	if ( _dbOpen )
	{
		_dbHelp.Close();

		_dbOpen = false;
	}
	
	_projectList.clear();
}

GpsReader *RadarManager::getGpsReader()
{
	return &_gpsReader;
}
RadarDataRader *RadarManager::getRadarDataRader()
{
	return &_radarReader;
}

void RadarManager::showCurCurve(LPCWSTR pszPath/* = NULL*/)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if ( !g_CurCurveFirstDlg ){
		g_CurCurveFirstDlg = new CDialogCurCurve(CDialogCurCurve::screenFirst);
		g_CurCurveFirstDlg->Create( CDialogCurCurve::IDD );	
	}
	std::string strFirstChannels = m_configureSet->get("screen","first");
	if (strFirstChannels.compare("")==0){
		g_CurCurveFirstDlg->ShowWindow(SW_HIDE);
	}else {
		g_CurCurveFirstDlg->ShowWindow(SW_SHOW);
	}
	if (!g_CurCurveSecondDlg ){
		g_CurCurveSecondDlg = new CDialogCurCurve(CDialogCurCurve::screenSecond);
		g_CurCurveSecondDlg->Create( CDialogCurCurve::IDD );
	}
	std::string strSecondChannels = m_configureSet->get("screen","second");
	if (strSecondChannels.compare("")==0){
		g_CurCurveSecondDlg->ShowWindow(SW_HIDE);
	}else {
		g_CurCurveSecondDlg->ShowWindow(SW_SHOW);
	}
	if (!g_CurCurveThirdDlg ){
		g_CurCurveThirdDlg = new CDialogCurCurve(CDialogCurCurve::screenThird);
		g_CurCurveThirdDlg->Create( CDialogCurCurve::IDD );
	}
	std::string strThirdChannels = m_configureSet->get("screen","third");
	if (strThirdChannels.compare("")==0){
		g_CurCurveThirdDlg->ShowWindow(SW_HIDE);
	}else {
		g_CurCurveThirdDlg->ShowWindow(SW_SHOW);
	}
	if (!g_CurCurveFourthDlg ){
		g_CurCurveFourthDlg = new CDialogCurCurve(CDialogCurCurve::screenFourth);
		g_CurCurveFourthDlg->Create( CDialogCurCurve::IDD );
	}
	std::string strFourthChannels = m_configureSet->get("screen","fourth");
	if (strFourthChannels.compare("")==0){
		g_CurCurveFourthDlg->ShowWindow(SW_HIDE);
	}else {
		g_CurCurveFourthDlg->ShowWindow(SW_SHOW);
	}
	//20210628hjl 三视图
	if(m_nUpload==0){
		if ( !g_ThreeViewDlg ){
			g_ThreeViewDlg = new CDialogThreeView;
			g_ThreeViewDlg->Create( CDialogThreeView::IDD );
		}
		g_ThreeViewDlg->ShowWindow(SW_SHOW);
	}
	CString strName = _T("实时曲线");
	if (NULL != pszPath){
		strName += _T("：");
		strName += pszPath;
	}
	CString strNameIndex[4];
	strNameIndex[0]=strName+_T("一号窗口");
	strNameIndex[1]=strName+_T("二号窗口");
	strNameIndex[2]=strName+_T("三号窗口");
	strNameIndex[3]=strName+_T("四号窗口");
	g_CurCurveFirstDlg->SetWindowText(strNameIndex[0]);
	g_CurCurveSecondDlg->SetWindowText(strNameIndex[1]);
	g_CurCurveThirdDlg->SetWindowText(strNameIndex[2]);
	g_CurCurveFourthDlg->SetWindowText(strNameIndex[3]);
	
	if (g_CurCurveFirstDlg->IsWindowVisible()){
		g_CurCurveFirstDlg->ShowWindow( SW_SHOW );
	}
	if (m_rtCurCurveDelPos.right > 0 && m_rtCurCurveDelPos.bottom > 0 ){
		g_CurCurveFirstDlg->SetWindowPos(NULL,m_rtCurCurveDelPos.left,m_rtCurCurveDelPos.top,m_rtCurCurveDelPos.right,m_rtCurCurveDelPos.bottom,   SWP_FRAMECHANGED | SWP_NOZORDER	);
	}

	if (g_CurCurveSecondDlg->IsWindowVisible()){
		g_CurCurveSecondDlg->ShowWindow( SW_SHOW );
	}
	if (m_rtCurCurveDelPos.right > 0 && m_rtCurCurveDelPos.bottom > 0 ){
		g_CurCurveSecondDlg->SetWindowPos(NULL,m_rtCurCurveDelPos.left,m_rtCurCurveDelPos.top,m_rtCurCurveDelPos.right,m_rtCurCurveDelPos.bottom,   SWP_FRAMECHANGED | SWP_NOZORDER	);
	}

	if (g_CurCurveThirdDlg->IsWindowVisible()){
		g_CurCurveThirdDlg->ShowWindow( SW_SHOW );
	}
	if (m_rtCurCurveDelPos.right > 0 && m_rtCurCurveDelPos.bottom > 0 ){
		g_CurCurveThirdDlg->SetWindowPos(NULL,m_rtCurCurveDelPos.left,m_rtCurCurveDelPos.top,m_rtCurCurveDelPos.right,m_rtCurCurveDelPos.bottom,   SWP_FRAMECHANGED | SWP_NOZORDER	);
	}

	if (g_CurCurveFourthDlg->IsWindowVisible()){
		g_CurCurveFourthDlg->ShowWindow( SW_SHOW );
	}
	if (m_rtCurCurveDelPos.right > 0 && m_rtCurCurveDelPos.bottom > 0 ){
		g_CurCurveFourthDlg->SetWindowPos(NULL,m_rtCurCurveDelPos.left,m_rtCurCurveDelPos.top,m_rtCurCurveDelPos.right,m_rtCurCurveDelPos.bottom,   SWP_FRAMECHANGED | SWP_NOZORDER	);
	}

	if(m_nUpload==0){
		/*
		if (g_ThreeViewDlg->IsWindowVisible()){
			g_ThreeViewDlg->ShowWindow( SW_SHOW );
		}
		if (m_rtCurCurveDelPos.right > 0 && m_rtCurCurveDelPos.bottom > 0 ){//?
			g_ThreeViewDlg->SetWindowPos(NULL,m_rtCurCurveDelPos.left,m_rtCurCurveDelPos.top,m_rtCurCurveDelPos.right,m_rtCurCurveDelPos.bottom,   SWP_FRAMECHANGED | SWP_NOZORDER	);
		}
		*/
	}

	//g_CurCurveLeftDlg->resizeChannelWnd();
	//g_CurCurveLeftDlg->UpdateData();
	g_CurCurveFirstDlg->SendMessage(WM_SIZE,0,0);
	g_CurCurveFirstDlg->UpdateWindow();
	g_CurCurveFirstDlg->UpdateData();

	g_CurCurveSecondDlg->SendMessage(WM_SIZE,0,0);
	g_CurCurveSecondDlg->UpdateWindow();
	g_CurCurveSecondDlg->UpdateData();

	g_CurCurveThirdDlg->SendMessage(WM_SIZE,0,0);
	g_CurCurveThirdDlg->UpdateWindow();
	g_CurCurveThirdDlg->UpdateData();

	g_CurCurveFourthDlg->SendMessage(WM_SIZE,0,0);
	g_CurCurveFourthDlg->UpdateWindow();
	g_CurCurveFourthDlg->UpdateData();
	
	//20210628 hjl 
	g_ThreeViewDlg->SendMessage(WM_SIZE,0,0);
	g_ThreeViewDlg->UpdateWindow();
	g_ThreeViewDlg->UpdateData();
	

}



void RadarManager::SetCurCurveDialogPosSize(int iType /*= 0*/)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	std::vector<std::string> groupItemsName;
	bool status = m_configureSet->getGroupItemsName("screen",groupItemsName);

	if (!status)
	{
		//没有查询成功
	}
	
	//位置现在有问题！！
	RECT rt;  
	SystemParametersInfo(SPI_GETWORKAREA,0,(PVOID)&rt,0);  
	int cx = rt.right;
	int cy = rt.bottom;

	int cxAll = GetSystemMetrics(   SM_CXSCREEN   );   
	int cyAll = GetSystemMetrics(   SM_CYSCREEN   );

	int iXLeft ;
	int iYTop ;
	int iXWidth;
	int iYHight ;

	if (0 == iType)
	{
		iXLeft = 0;
		iYTop = 0;
		iXWidth = cx / 2;
		iYHight = cy ;
	} 
	else if (1 == iType)
	{
		iXLeft = 0;
		iYTop = cyAll;
		iXWidth =  2 * cxAll;
		iYHight = cyAll;
	}
	else if (2 == iType)
	{
		iXLeft = -cxAll;
		//iXLeft = 0;
		iYTop = cyAll;
		iXWidth =  2 * cxAll;
		iYHight =cyAll;
	}
	else if (3 == iType)
	{
		iXLeft = 0;
		iYTop = 0;
		iXWidth = 2 * cx;
		iYHight = cy ;
	}
	else if (4 == iType)
	{
		iXLeft = -cx;
		iYTop = 0;
		iXWidth = 2 * cx;
		iYHight = cy ;
	}

	if (g_CurCurveFirstDlg){
		if (!g_CurCurveFirstDlg->IsWindowVisible())
		{
			g_CurCurveFirstDlg->ShowWindow(SW_SHOW);
		}
		std::string strFirstChannels = m_configureSet->get("screen","first");
		if (strFirstChannels.compare("")==0){
			g_CurCurveFirstDlg->ShowWindow(SW_HIDE);
		}else {
			g_CurCurveFirstDlg->ShowWindow(SW_SHOW);
		}
		g_CurCurveFirstDlg->SetWindowPos(NULL,iXLeft,iYTop,iXWidth,iYHight,   SWP_FRAMECHANGED | SWP_NOZORDER	);
		g_CurCurveFirstDlg->UpdateData();
		g_CurCurveFirstDlg->UpdateWindow();
		g_CurCurveFirstDlg->GetWindowRect(&m_rtCurCurveDelPos);
	}

	if (g_CurCurveSecondDlg){
		if (!g_CurCurveSecondDlg->IsWindowVisible()){
			g_CurCurveSecondDlg->ShowWindow(SW_SHOW);
		}
		std::string strSecondChannels = m_configureSet->get("screen","second");
		if (strSecondChannels.compare("")==0){
			g_CurCurveSecondDlg->ShowWindow(SW_HIDE);
		}else {
			g_CurCurveSecondDlg->ShowWindow(SW_SHOW);
		}
		g_CurCurveSecondDlg->SetWindowPos(NULL,iXLeft,iYTop,iXWidth,iYHight,   SWP_FRAMECHANGED | SWP_NOZORDER	);
		g_CurCurveSecondDlg->UpdateData();
		g_CurCurveSecondDlg->UpdateWindow();
		g_CurCurveSecondDlg->GetWindowRect(&m_rtCurCurveDelPos);
	}

	if (g_CurCurveThirdDlg){
		if (!g_CurCurveThirdDlg->IsWindowVisible()){
			g_CurCurveThirdDlg->ShowWindow(SW_SHOW);
		}
		std::string strThirdChannels = m_configureSet->get("screen","second");
		if (strThirdChannels.compare("")==0){
			g_CurCurveThirdDlg->ShowWindow(SW_HIDE);
		}else {
			g_CurCurveThirdDlg->ShowWindow(SW_SHOW);
		}
		g_CurCurveThirdDlg->SetWindowPos(NULL,iXLeft,iYTop,iXWidth,iYHight,   SWP_FRAMECHANGED | SWP_NOZORDER	);
		g_CurCurveThirdDlg->UpdateData();
		g_CurCurveThirdDlg->UpdateWindow();
		g_CurCurveThirdDlg->GetWindowRect(&m_rtCurCurveDelPos);
	}

	if (g_CurCurveFourthDlg){
		if (!g_CurCurveFourthDlg->IsWindowVisible()){
			g_CurCurveFourthDlg->ShowWindow(SW_SHOW);
		}
		std::string strFourthChannels = m_configureSet->get("screen","second");
		if (strFourthChannels.compare("")==0){
			g_CurCurveFourthDlg->ShowWindow(SW_HIDE);
		}else {
			g_CurCurveFourthDlg->ShowWindow(SW_SHOW);
		}
		g_CurCurveFourthDlg->SetWindowPos(NULL,iXLeft,iYTop,iXWidth,iYHight,   SWP_FRAMECHANGED | SWP_NOZORDER	);
		g_CurCurveFourthDlg->UpdateData();
		g_CurCurveFourthDlg->UpdateWindow();
		g_CurCurveFourthDlg->GetWindowRect(&m_rtCurCurveDelPos);
	}
 	
	
	//old
//	{
// 		if ( !g_CurCurveLeftDlg )
// 		{
// 			g_CurCurveLeftDlg = new CDialogCurCurve( false );
// 			if ( !g_CurCurveLeftDlg->Create( CDialogCurCurve::IDD ) )
// 			{
// 				delete g_CurCurveLeftDlg;
// 				g_CurCurveLeftDlg = NULL;
// 				return;
// 			}
// 		}
// 		RECT rt;  
// 		SystemParametersInfo(SPI_GETWORKAREA,0,(PVOID)&rt,0);  
// 		int cx = rt.right;
// 		int cy = rt.bottom;
// 
// 		int cxAll = GetSystemMetrics(   SM_CXSCREEN   );   
// 		int cyAll = GetSystemMetrics(   SM_CYSCREEN   );
// 
// 		int iXLeft ;
// 		int iYTop ;
// 		int iXWidth;
// 		int iYHight ;
// 
// 		if (0 == iType)
// 		{
// 			iXLeft = 0;
// 			iYTop = 0;
// 			iXWidth = cx / 2;
// 			iYHight = cy ;
// 
// 		} 
// 		else if (1 == iType)
// 		{
// 			iXLeft = 0;
// 			iYTop = cyAll;
// 			iXWidth =  2 * cxAll;
// 			iYHight = cyAll;
// 		}
// 		else if (2 == iType)
// 		{
// 			iXLeft = -cxAll;
// 			iYTop = cyAll;
// 			iXWidth =  2 * cxAll;
// 			iYHight =cyAll;
// 		}
// 		else if (3 == iType)
// 		{
// 			iXLeft = 0;
// 			iYTop = 0;
// 			iXWidth = 2 * cx;
// 			iYHight = cy ;
// 		}
// 		else if (4 == iType)
// 		{
// 			iXLeft = -cx;
// 			iYTop = 0;
// 			iXWidth = 2 * cx;
// 			iYHight = cy ;
// 		}
// 
// 		g_CurCurveLeftDlg->ShowWindow( SW_SHOW );
// 		g_CurCurveLeftDlg->SetWindowPos(NULL,iXLeft,iYTop,iXWidth,iYHight,   SWP_FRAMECHANGED | SWP_NOZORDER	);
// 		g_CurCurveLeftDlg->UpdateData();
// 		g_CurCurveLeftDlg->UpdateWindow();
// 		g_CurCurveLeftDlg->GetWindowRect(&m_rtCurCurveDelPos);
//	}
}

void RadarManager::showParamDialog()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	DlgRadarParamSet dlg;
	if ( dlg.DoModal() != IDOK )
	{
		return;
	}

		
	OpenThreads::ScopedLock<OpenThreads::Mutex> lock(_lock);
	getRadarDataRader()->setInit( false );

}

void RadarManager::showHistCurve()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if ( !g_HistoryProjectDlg )
	{
		g_HistoryProjectDlg = new CDialogProject ( _projectList, 2 );
		g_HistoryProjectDlg->Create( CDialogProject::IDD );
	}
	g_HistoryProjectDlg->ReInit();
	g_HistoryProjectDlg->ShowWindow(SW_SHOW);
	COM::WRITE_TO_LOG_PTR->WriteToLog(COM::LEVEL_COMMON,"showHistCurve","显示历史数据对话框");	
}
MeasureProject *RadarManager::getCurProject()
{
	OpenThreads::ScopedLock<OpenThreads::Mutex> lock(_lock);
	return _lpCurProject;
}

////hjl 20210418参数设置波形显示
void RadarManager::addRadarDataToParameterConfig( RadarData *d, int index ){
	/*
	{
		OpenThreads::ScopedLock<OpenThreads::Mutex> lock(_lock);
		if (_lpCurProject.valid()){
			_lpCurProject->addData(d, index);
		}
	}*/
	g_RadarParameterConfig->AddRadarData( d, index );
}

void RadarManager::addRadarDataToDisplay( RadarData *d, int index ){
	/*
	{
		OpenThreads::ScopedLock<OpenThreads::Mutex> lock(_lock);
		if (_lpCurProject.valid()){
			_lpCurProject->addData(d, index);
		}
	}*/
	//往一号显示窗口添加数据
	if ( g_CurCurveFirstDlg ){
		if(index<12){
			g_CurCurveFirstDlg->AddRadarData( d, index );
		}
	}
	//往二号显示窗口添加数据
	if ( g_CurCurveSecondDlg ){
		if(index>11&&index<24){
			g_CurCurveSecondDlg->AddRadarData( d, index );
		}
	}
	//往三号显示窗口添加数据
	if ( g_CurCurveThirdDlg ){
		if(index>23){
			g_CurCurveThirdDlg->AddRadarData( d, index );
		}
	}
	//往四号显示窗口添加数据
	if ( g_CurCurveFourthDlg ){
		//g_CurCurveFourthDlg->AddRadarData( d, index );
	}
}

//往第index道数据添加最新的雷达数据
void RadarManager::addRadarDataToProject( RadarData *d, int index ){
	{
		OpenThreads::ScopedLock<OpenThreads::Mutex> lock(_lock);
		if (_lpCurProject.valid()){
			_lpCurProject->addData(d, index);
		}
	}
	/*
	//往一号显示窗口添加数据
	if ( g_CurCurveFirstDlg ){
		g_CurCurveFirstDlg->AddRadarData( d, index );
	}
	//往二号显示窗口添加数据
	if ( g_CurCurveSecondDlg ){
		g_CurCurveSecondDlg->AddRadarData( d, index );
	}
	//往三号显示窗口添加数据
	if ( g_CurCurveThirdDlg ){
		g_CurCurveThirdDlg->AddRadarData( d, index );
	}
	//往四号显示窗口添加数据
	if ( g_CurCurveFourthDlg ){
		g_CurCurveFourthDlg->AddRadarData( d, index );
	}*/
	
	//往一号显示窗口添加数据
	if ( g_CurCurveFirstDlg ){
		if(index<12){
			g_CurCurveFirstDlg->AddRadarData( d, index );
		}
	}
	//往二号显示窗口添加数据
	if ( g_CurCurveSecondDlg ){
		if(index>11&&index<24){
			g_CurCurveSecondDlg->AddRadarData( d, index );
		}
	}
	//往三号显示窗口添加数据
	if ( g_CurCurveThirdDlg ){
		if(index>23){
			g_CurCurveThirdDlg->AddRadarData( d, index );
		}
	}
	//往四号显示窗口添加数据
	/*
	if ( g_CurCurveFourthDlg ){
		g_CurCurveFourthDlg->AddRadarData( d, index );
	}
	*/
	//往三维显示窗口添加数据
	/*
	if ( g_ThreeViewDlg ){
		//g_ThreeViewDlg->AddRadarData( d, index );
	}*/
}
void RadarManager::addRadarDataToThreeViewDialog( RadarData *d, int index ){
	if ( g_ThreeViewDlg ){
		//g_ThreeViewDlg->AddRadarData( d, index );
		
		m_vec3DRadarDataGroup.push_back(d);
		if(index==11){
			if(m_vec3DRadarDataGroup.size()>11){
				g_ThreeViewDlg->addGroupRadarData( m_vec3DRadarDataGroup );	
			}
			m_vec3DRadarDataGroup.clear();
		}
	}
}
/*
//根据相邻两道数据拟合出数据
void RadarManager::addArtificalRadarDataToProject( int index ){
	{
		OpenThreads::ScopedLock<OpenThreads::Mutex> lock(_lock);
		if (_lpCurProject.valid()){
			_lpCurProject->addData(artificalRadarData, getArtificalChannelIndexFromOriginalIndex(index));
		}
	}
	//往一号显示窗口添加数据
	if ( g_CurCurveFirstDlg ){
		g_CurCurveFirstDlg->AddRadarData( artificalRadarData, getArtificalChannelIndexFromOriginalIndex(index) );
	}
	//往二号显示窗口添加数据
	if ( g_CurCurveSecondDlg ){
		g_CurCurveSecondDlg->AddRadarData( artificalRadarData, getArtificalChannelIndexFromOriginalIndex(index) );
	}
	//往三号显示窗口添加数据
	if ( g_CurCurveThirdDlg ){
		g_CurCurveThirdDlg->AddRadarData( artificalRadarData, getArtificalChannelIndexFromOriginalIndex(index) );
	}
	//往四号显示窗口添加数据
	if ( g_CurCurveFourthDlg ){
		g_CurCurveFourthDlg->AddRadarData( artificalRadarData, getArtificalChannelIndexFromOriginalIndex(index) );
	}
	//往三维显示窗口添加数据
	if ( g_ThreeViewDlg ){
		//g_ThreeViewDlg->AddRadarData( d, index );
	}
}*/


//20200306hjl 用于测量轮回滚删除数据
void RadarManager::deleteRadarDataFromProject( int index ){
	{
		OpenThreads::ScopedLock<OpenThreads::Mutex> lock(_lock);
		if (_lpCurProject.valid()){
			_lpCurProject->deleteData(index);
		}
	}/*
	if ( g_CurCurveFirstDlg ){
		g_CurCurveFirstDlg->DeleteRadarData( index );
	}
	if ( g_CurCurveSecondDlg ){
		g_CurCurveSecondDlg->DeleteRadarData( index );
	}
	if ( g_CurCurveThirdDlg ){
		g_CurCurveThirdDlg->DeleteRadarData( index );
	}
	if ( g_CurCurveFourthDlg ){
		g_CurCurveFourthDlg->DeleteRadarData( index );
	}*/
}

void RadarManager::loadAllProject()
{
	ProjectNameTab tab;
	if ( !tab.open( &_dbHelp, tab.getSelectSql() ) )
	{
		tab.createTab(&_dbHelp);
		return;
	}

	CDBTable::RowVector rv;
	while(  ( rv = tab.read( 10 )).size() != 0 )
	{
		_projectList.insert( _projectList.end(), rv.begin(), rv.end() );
		if ( rv.size() < 10 )
		{
			break;
		}
	}

	DBAPI::COtlRecordset recordSet(_dbHelp.GetConnect());

	recordSet.Close();
}


CDBConnectHelp *RadarManager::getDBHelp()
{
	return &_dbHelp;
}
void RadarManager::setRadarChannelCount( int value )
{
	_radarChannelCount = value;
}
int RadarManager::getRadarChannelCount()const
{
	return _radarChannelCount;
}

ConfigureSet *RadarManager::getConfigureSet()
{
	return m_configureSet.get();
}
bool RadarManager::dbOpen()const
{
	return _dbOpen;
}

void RadarManager::setdbOpen(bool value)
{
	_dbOpen = value;
}

bool RadarManager::testing()const//hjl20210418
{
	return _testing;
}

bool RadarManager::working()const
{
	return _working;
}
void RadarManager::setShowGpsPos( bool value )
{
	_showGps = value;
}
bool RadarManager::getShowGpsPos()const
{
	return _showGps;
}
void RadarManager::setExportGpsPos( bool value ){
	_exportGps = value;
}
bool RadarManager::getExportGpsPos()const{
	return _exportGps;
}
void RadarManager::setExportKml( bool value ){
	_exportKml = value;
}
bool RadarManager::getExportKml()const{
	return _exportKml;
}



void RadarManager::savePicPath( std::string const& picPath, int num )
{
	if ( !_working )
	{
		return;
	}

	if ( !_lpCurProject.valid() )
	{
		return;
	}

	osg::ref_ptr<VideoTemplateTab::VideoRow> vr = new VideoTemplateTab::VideoRow;
	vr->_recvTime = time(NULL);
	vr->_filePath = picPath;
	vr->_cameraNum = num;
	osg::ref_ptr<SaveVideoCmd> lpCmd = new SaveVideoCmd;
	lpCmd->_lpRow = vr;

	_lpCurProject->addSaveCmd( lpCmd.get() );
	
}

CWnd *RadarManager::getVideoWnd()
{
	return _lpVideo;
}

void RadarManager::setPicPath( std::string const& picPath )
{
	m_strImagePath = picPath;
}
std::string RadarManager::getPicPath()const
{
	return m_strImagePath;
}
void RadarManager::setChannelName( std::string const& name, int index )
{
	if ( name.length() == 0 )
	{
		return;
	}

	std::stringstream ss;
	ss << index;
	m_configureSet->set( "channelname", ss.str(), name );
	m_configureSet->write();

}
std::string RadarManager::getChannelName( int index )const
{
	std::stringstream ss;
	ss << index;
	std::string channelName = m_configureSet->get( "channelname", ss.str() );

	return channelName;
}

void RadarManager::setDeviceCode( std::string const& code )
{
	if ( code.length() > 0 )
	{
		_deviceCode = code;
	}

}
std::string RadarManager::getDeviceCode( int iTrackNo )const
{
	std::stringstream ss;
	ss << iTrackNo;
	std::string channelName = m_configureSet->get( "channelname", ss.str() );
	if ( channelName.length() )
	{
		return channelName;
	}


	if ( _deviceCode.length() == 0 )
	{
		return "MC8";
	}
	return _deviceCode;
}

void RadarManager::setChannelRatioPrect( int index, float value )
{
	std::stringstream ss;
	ss << index;

	std::stringstream ss2;
	ss2 << value;

	m_configureSet->set( "channelratio", ss.str(), ss2.str() );
	m_configureSet->write();
}

float RadarManager::getChannelRatioPrect( int index )const
{
	std::stringstream ss;
	ss << index;

	float ratio = atof( m_configureSet->get( "channelratio", ss.str()).c_str());

	if ( ratio <= 0.0f )
	{
		ratio = 1.0f;
	}

	return ratio;
}

float RadarManager::getDielectric( int index )
{
	switch ( index )
	{
	case 0:
		return 1;
		break;
	case 1:
		return 1.9;
		break;
	case 2:
		return 2;
		break;
	case 3:
		return 2.5;
		break;
	case 4:
		return 2.8;
		break;
	case 5:
		return 2.8;
		break;
	case 6:
		return 3.2;
		break;
	case 7:
		return 4;
		break;
	case 8:
		return 4;
		break;
	case 9:
		return 4.1;
		break;
	case 10:
		return 5;
		break;
	case 11:
		return 5;
		break;
	case 12:
		return 6;
		break;
	case 13:
		return 6.4;
		break;
	case 14:
		return 7;
		break;
	case 15:
		return 7;
		break;
	case 16:
		return 7;
		break;
	case 17:
		return 8;
		break;
	case 18:
		return 8;
		break;
	case 19:
		return 12;
		break;
	case 20:
		return 15;
		break;
	case 21:
		return 16;
		break;
	case 22:
		return 30;
		break;
	case 23:
		return 30;
		break;
	case 24:
		return 81;
		break;
	}
	return 6.4;
}

int RadarManager::getSampCount( int index )
{


	switch ( index )
	{
	case 0:
		return 256;
		break;
	case 1:
		return 512;
		break;
	case 2:
		return 1024;
		break;
	case 3:
		return 2048;
		break;
	}

	return 256;
}
float RadarManager::getSampRatio( int index, int trackNo )
{
	float fSampRate = 51.2f;

	switch (RadarManager::Instance()->GetRadarWorkType())
	{
	case RADAR_WORK_TYPE_ONE_USB:
		switch (  index )
		{
		case 0:
			fSampRate = 25.6;
			break ;
		case 1:
			fSampRate = 12.8;
			break;
		case 2:
			fSampRate = 6.4;
			break;
		case 3:
			fSampRate = 3.2;
			break;
		case 4:
			fSampRate = 32;
			break;
		case 5:
			fSampRate = 16;
			break;
		case 6:
			fSampRate = 8;
			break;
		case 7:
			fSampRate = 4;
			break;
		}
		break;
	case RADAR_WORK_TYPE_DOUBLE_USB:
		switch (  index )
		{
		case 0:
			fSampRate = 25.6;
			break ;
		case 1:
			fSampRate = 12.8;
			break;
		case 2:
			fSampRate = 6.4;
			break;
		case 3:
			fSampRate = 3.2;
			break;
		case 4:
			fSampRate = 32;
			break;
		case 5:
			fSampRate = 16;
			break;
		case 6:
			fSampRate = 8;
			break;
		case 7:
			fSampRate = 4;
			break;
		}
		break;
	case RADAR_WORK_TYPE_DOUBLE_USB_OLD:
		switch (  index )
		{
		case 0:
			fSampRate = 25.6;
			break ;
		case 1:
			fSampRate = 12.8;
			break;
		case 2:
			fSampRate = 6.4;
			break;
		case 3:
			fSampRate = 3.2;
			break;
		case 4:
			fSampRate = 32;
			break;
		case 5:
			fSampRate = 16;
			break;
		case 6:
			fSampRate = 8;
			break;
		case 7:
			fSampRate = 4;
			break;
		}
		break;
	case RADAR_WORK_TYPE_FOUR_USB:
		switch (  index )
		{
		/*case 0:
			fSampRate = 16.0;
			break ;
		case 1:
			fSampRate = 8.0;
			break;
		case 2:
			fSampRate = 4.0;
			break;*/
		case 0:
			fSampRate = 25.6;
			break ;
		case 1:
			fSampRate = 12.8;
			break;
		case 2:
			fSampRate = 6.4;
			break;
		case 3:
			fSampRate = 3.2;
			break;
		case 4:
			fSampRate = 32;
			break;
		case 5:
			fSampRate = 16;
			break;
		case 6:
			fSampRate = 8;
			break;
		case 7:
			fSampRate = 4;
			break;
		}
		break;
	case RADAR_WORK_TYPE_EIGHT:
		switch (  index )
		{
		/*case 0:
			fSampRate = 51.2;
			break ;
		case 1:
			fSampRate = 25.6;
			break;
		case 2:
			fSampRate = 12.8;
			break;
		case 3:
			fSampRate = 6.4;
			break;*/
		case 0:
			fSampRate = 20.48;
			break ;
		case 1:
			fSampRate = 10.24;
			break;
		case 2:
			fSampRate = 5.12;
			break;
		case 3:
			fSampRate = 2.56;
			break;
		}
		{
			fSampRate /= getChannelRatioPrect( trackNo );
		}
		break;
	}
	return fSampRate;
}

HWND s_nhWnd[1000];
int s_count;
//回调函数
BOOL CALLBACK EnumWndProc(HWND hwnd, LPARAM lParam)
{
	DWORD dwID ;

	GetWindowThreadProcessId(hwnd, &dwID) ;

	if (dwID == (DWORD)lParam && IsWindowVisible(hwnd) && GetParent(hwnd) == NULL)
	{
		s_nhWnd[s_count++] = hwnd;
	}

	return (s_count == 1000) ? false : true;
}

void GetProcessTopWindows(DWORD dwPID)
{
	s_count = 0;
	EnumWindows((WNDENUMPROC)EnumWndProc, (LPARAM)dwPID);
}

int GetProgressIDByeName(TCHAR* pProgressName)
{
	DWORD iProcID=0;
	if(NULL == pProgressName )
	{
		return iProcID;
	}
	LPCWSTR procName = pProgressName;
	PROCESSENTRY32 pEntry={sizeof(PROCESSENTRY32)};
	CString strtemp;
	HANDLE hSnapshot=CreateToolhelp32Snapshot(TH32CS_SNAPALL,0);
	if(hSnapshot!=INVALID_HANDLE_VALUE)
	{
		if(Process32First(hSnapshot,&pEntry))
		{
			while (Process32Next(hSnapshot, &pEntry))
			{
				strtemp=pEntry.szExeFile;
				if(procName==strtemp)
				{
					iProcID=pEntry.th32ProcessID;
					CloseHandle(hSnapshot);
					break;
				}
			}
		}
	}
	return iProcID;
}

bool SendMessageToOtherProgress(TCHAR* pProgressName,UINT Msg,  WPARAM wParam, LPARAM lParam)
{

	DWORD iProcID = GetProgressIDByeName(pProgressName);
	if (0 == iProcID)
	{
		return false;
	}
	GetProcessTopWindows(iProcID);
	COPYDATASTRUCT cds;     
	std::string str  = (char*)lParam;
	cds.dwData=sizeof(COPYDATASTRUCT);     
	cds.cbData=str.size();   
	cds.lpData=(LPVOID)str.c_str();   

	for (int i = 0 ; i < s_count;++i)
	{
		if( WM_COPYDATA == Msg )
		{
			::SendMessage(s_nhWnd[i],Msg,   wParam,  (LPARAM)&cds);
		}
		else
		{
			::SendMessage(s_nhWnd[i],Msg,	wParam,	lParam);
		}
	}
	return true;
}



//////////////////////////////////////////////////////////////////////////
bool RadarManager::CheckRadarProject()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	DWORD iProcID = GetProgressIDByeName(_T("RadarView.exe"));
	if (0 == iProcID)
	{
		return false;
	}

	GetProcessTopWindows(iProcID);
	CDialogProject dlg( _projectList, 2,CWnd::FromHandle(s_nhWnd[0] ));
	dlg.m_bProcessMsg = true;

	if ( dlg.DoModal() != IDOK )
	{
		return false;
	}

	int iIndex = dlg.m_iSelectIndex;

	std::string strFilePath;

	if (!ExportGprFile(dlg.m_lpProjectRow,iIndex,strFilePath, CWnd::FromHandle(s_nhWnd[0])))
	{
		return false;
	}
	SendMessageToOtherProgress(_T("RadarView.exe"),WM_COPYDATA,0,(LPARAM)(char*)strFilePath.c_str());
	return true;
}

bool RadarManager::ExportGprFile(void *_lpProjectRow/*std::string strProName*/,int _channelIndex/*,std::string strXML*/,std::string & strFilePath ,CWnd* pParent /*= NULL*/)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	USES_CONVERSION;
	std::string strTemp;

	TCHAR szAppPath[MAX_PATH];
	::GetModuleFileName(AfxGetInstanceHandle(), szAppPath, MAX_PATH);
	::PathRemoveFileSpec(szAppPath);
	CString cstrModelePath = szAppPath;
	ProjectNameTab::ProjectNameRow * lpProjectRow = (ProjectNameTab::ProjectNameRow*)_lpProjectRow;
	CString cstrTem;
	cstrTem.Format(L"%ld", lpProjectRow->_createTime);
	std::string _projectName = W2A(cstrTem.GetBuffer());
	CString sctrName = A2W(lpProjectRow->_projectName.c_str());
	CString saveName;	
	saveName.Format(L"%s\\%s_通道%d_%d.GPR"
		,cstrModelePath
		,sctrName
		,(_channelIndex + 1)
		,lpProjectRow->_createTime);
	std::string filePath = W2A( saveName );
	strFilePath = filePath;
	std::string strXML = lpProjectRow->_paramXML;
	WIN32_FIND_DATA FindFileData;
	HANDLE hFind;
	hFind = FindFirstFile(saveName.GetBuffer(), &FindFileData);
	if (hFind == INVALID_HANDLE_VALUE) 
	{
		 printf ("Invalid File Handle. Get Last Error reports %d ", GetLastError ());
	} 
	else 
	{
		printf ("The first file found is %s ", FindFileData.cFileName);
		FindClose(hFind);

		CDlgPromptMessage dlgMessage(pParent);
		CString _strTite = _T("提示信息");
		CString _strInfo = _T("GPR文件也存在，是否重新生成？");;
		CString _strOK = _T("确定");;
		CString _strCancle = _T("取消");;
		dlgMessage.SetPromptMessage( _strTite, _strInfo, _strOK, _strCancle);
		if ( IDOK != dlgMessage.DoModal())
		{
			return true;//Do your things
		}
	}
	if ( !RadarManager::Instance()->dbOpen() )
	{
		return false;
	}
	DataTemplateTab tab;
	tab.setProjectName( _projectName);

	int count = 0;
	{
		DataCountTab countTab;
		countTab.setTabName( tab.getProjectName() );
		std::stringstream ss;
		ss << countTab.getSelectSql();
		ss << " where ChannelIndex=" << _channelIndex;
		count = countTab.getTabDataCount( RadarManager::Instance()->getDBHelp(), ss.str());
	}
	if ( !_loadProcess )
	{
		_loadProcess = new CProcessDlg(pParent);
		_loadProcess->Create( CProcessDlg::IDD );
	}
	std::stringstream ss;
	ss << tab.getSelectSql() << " where ChannelIndex=" << _channelIndex;
	ss << " order by id";
	if ( !tab.open( RadarManager::Instance()->getDBHelp(), ss.str() ) )
	{
		CDlgPromptMessage dlgMessage(pParent);
		CString _strTite = _T("警告");
		CString _strInfo = _T("打开工程表失败!");;
		CString _strOK = _T("确定");;
		CString _strCancle = _T("取消");;
		dlgMessage.SetPromptMessage( _strTite, _strInfo, _strOK, _strCancle);
		if ( IDOK != dlgMessage.DoModal())
		{
			return false;//Do your things
		}
		return false;
	}

	CProcessDlg *lpDlg = (CProcessDlg*)_loadProcess;
	_loadProcess->ShowWindow( SW_SHOW );
	lpDlg->StartProcess( count );//count13013
	SetCursor( LoadCursor(NULL, IDC_WAIT) );

	::SetForegroundWindow(_loadProcess->m_hWnd);
	::SetWindowPos(_loadProcess->GetSafeHwnd(),HWND_TOPMOST,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);

	GPRWriter writer;
	writer.openGPR( filePath,  strXML, _channelIndex );
	writer.writeHead( 1 ,_projectName);
	CDBTable::RowVector rv;
	int iBuf[1000];
	int lenc = 2048;
	while(  ( rv = tab.read( 1000 )).size() != 0 )
	{
		for ( unsigned int i = 0; i < rv.size(); i ++ )
		{
			RadarData *lpRadarData = dynamic_cast<RadarData*>(rv[i].get());
			if ( lpRadarData )
			{
				writer.writeRadarData( lpRadarData );
			}
			lpDlg->Step( 1 );
		}
		if ( rv.size() < 1000 )
		{
			break;
		}
	}
	writer.buchongHead( );
	writer.closeGPR();
	SetCursor( LoadCursor(NULL, IDC_ARROW) );
	_loadProcess->ShowWindow( SW_HIDE );
	if ( _loadProcess )
	{
		delete _loadProcess ;
		_loadProcess = NULL;
	}
	return true;
};

/*float RadarManager::GetJihuaAccuracy(int precIndex)
{
	float floatPrec = 1.0f;
	switch ( precIndex )
	{
	case 0:
		floatPrec = 1.0f;
		break;
	case 1:
		floatPrec = 2.0f;
		break;
	case 2:
		floatPrec = 5.0f;
		break;
	case 3:
		floatPrec = 10.0f;
		break;
	case 4:
		floatPrec = 20.0f;
		break;
	}
	return floatPrec;
}*/

int RadarManager::GetTrueCmdIndexFromPreclenAndAccuracy(float fCLen,float fAccuracy)
{
	int iCount = fAccuracy / (fCLen/720.0)+1;//设置精度/（测量轮周长/720）
	if (0 < iCount)
	{
		iCount--;
	}
	return iCount;
}

float RadarManager::GetPrecratioFromPreclenAndAccuracy(float fCLen,float fAccuracy)
{
	float iCount = 720 /(float)( GetTrueCmdIndexFromPreclenAndAccuracy(fCLen,fAccuracy)+1);
	return iCount;
}

float RadarManager::GetTrueAccuracyFromPreclenAndPrecindex(float fCLen,float fAccuracy)
{
	float fPre = GetPrecratioFromPreclenAndAccuracy( fCLen, fAccuracy);
	float fNum = fCLen / fPre;
	return fNum;

}

bool RadarManager::Workdetection()
{
	//AfxMessageBox(L"zheliangjianche");
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if(m_pDlgWorkDetection == NULL)
	{
		//施工监测对话框
		m_pDlgWorkDetection = new CDlgWorkDetection();
		if(!m_pDlgWorkDetection->GetSafeHwnd())
		{
			m_pDlgWorkDetection->Create(CDlgWorkDetection::IDD);
		}
	}
	if(m_pDlgWorkDetection->GetSafeHwnd())
	{					 
		//m_pDlgWorkDetection->ReInit();
		m_pDlgWorkDetection->ShowWindow(SW_NORMAL);
	}

	return true;
}
#include "DlgProjectManager.h"
void RadarManager::RoadDetectionProjectManager()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	DlgProjectManager* pDlgProjectManager = new DlgProjectManager;
	if(!pDlgProjectManager->GetSafeHwnd())
	{
		pDlgProjectManager->Create(DlgProjectManager::IDD);
	}
	pDlgProjectManager->ShowWindow(SW_NORMAL);
}

// bool RadarManager::DrowRoud( DrawInfo* pStu )
// {
// 	for (int i = 0; i != pStu->m_vProCreateTime.size();++i)
// 	{
// 		TRACE(pStu->m_vProCreateTime[i]);
// 		TRACE("\r\n");
// 	}
// 
// 	////偏移量
// 	//osg::Vec3f offset;
// 	//offset.x() = pStu->m_fDongXiPY;
// 	//offset.y() = pStu->m_fNanBeiPY;
// 
// 	////颜色
// 	//osg::Vec4f color;
// 	//color.r() = pStu->m_fAsColor[0];
// 	//color.g() = pStu->m_fAsColor[1];
// 	//color.b() = pStu->m_fAsColor[2];
// 	//color.a() = pStu->m_fAsColor[3];
// 	//
// 	//
// 
// 	//USES_CONVERSION;
// 	//osg::Vec3f tempPos;
// 	//int pRoadNum = pStu->m_vProCreateTime.size();
// 	//for (int i =0; i != pRoadNum ;++i)
// 	//{
// 	//	//根据工程时间（表名），查看是否存在对应的模型,有则删除
// 	//	if (m_pDetectRoadModel[pStu->m_vProCreateTime[i]].valid())
// 	//	{
// 	//		bool result = Kernel::GetpKC()->GetTempGroupRoot()->removeChild(m_pDetectRoadModel[pStu->m_vProCreateTime[i]]);
// 	//		m_pDetectRoadModel.erase(pStu->m_vProCreateTime[i]);
// 	//	}
// 
// 	//	DataTemplateTab tab;
// 	//	std::vector<osg::Vec3f> _vec;
// 	//	tab.GetAllPoint(RadarManager::Instance()->getDBHelp(),W2A(pStu->m_vProCreateTime[i]),std::string("1"),_vec);
// 	//	tempPos = _vec[0];
// 
// 	//	//生成模型
// 	//	osg::ref_ptr<DetectRoadModel> pHolesMoel = new DetectRoadModel(_vec, offset, pStu->m_fLeftLength, 
// 	//		pStu->m_fRightLength, color);
// 
// 	//	pHolesMoel->setMatrix(osg::Matrix::translate(offset));
// 	//	m_pDetectRoadModel[pStu->m_vProCreateTime[i]] = pHolesMoel;
// 	//}
// 	//
//  //	//camera定位的位置LLA
//  //	osg::Vec3f LLA = tempPos;
//  //	LLA.z() = 50.0;
//  //	EMObj::CmdObjLocate(LLA, 1);
// 
// 	//for (int i = 0; i != pRoadNum; ++i)
// 	//{
// 	//	//把m_pDetectRoadModel添加到场景
// 	//	bool status(false);
// 	//	if (m_pDetectRoadModel[pStu->m_vProCreateTime[i]].valid())
// 	//	{
// 	//		status = Kernel::GetpKC()->GetTempGroupRoot()->addChild(m_pDetectRoadModel[pStu->m_vProCreateTime[0]].get());
// 	//	}
// 	//}
// 
// 	return true;
// }

// void RadarManager::ClearSelectRoud( DrawInfo* pStu )
// {
// 	//清除选中的道路
// }
// 
// void RadarManager::ClearAllRoud()
// {
// 	//清楚全部道路
// 
// }

//雷达参数配置
void RadarManager::RadarParameterconFiguration()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	DlgRadarParameterConfig* pDlgRadarParameterConfig = new DlgRadarParameterConfig;
	g_RadarParameterConfig=pDlgRadarParameterConfig;
	if(!pDlgRadarParameterConfig->GetSafeHwnd())
	{
		pDlgRadarParameterConfig->Create(DlgRadarParameterConfig::IDD);
	}
	pDlgRadarParameterConfig->ShowWindow(SW_NORMAL);
}
//工作方式配置
void RadarManager::WorkTypeConfiguration()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	USES_CONVERSION;
	DlgWorkTypeConfiguration* pDlgWorkTypeConfiguration = new DlgWorkTypeConfiguration(NULL,m_strRadarIni);
	if(!pDlgWorkTypeConfiguration->GetSafeHwnd())
	{
		pDlgWorkTypeConfiguration->Create(DlgWorkTypeConfiguration::IDD);
	}
	pDlgWorkTypeConfiguration->ShowWindow(SW_NORMAL);
}

void RadarManager::setRadarWorkType(int value)
{
	m_iRadarWorkType = value;
}

void RadarManager::setRadarDataSaveType(int value)
{
	m_iSaveOracle = value;
}

void RadarManager::setRadarDataSavePath(CString strSavePath)
{
	m_strSavePath = strSavePath;
}

CString RadarManager::getRadarDataSavePath()
{
	return m_strSavePath;
}


void RadarManager::setPicSavePath(CString strImagePath)
{
	USES_CONVERSION;
	m_strImagePath = W2A(strImagePath);
}

std::string RadarManager::getPicSavePath()
{
	return m_strImagePath;
}


CString RadarManager::GetRadarWorkPath()
{
	return m_strRadarPath;
}

void RadarManager::ReadRadarIni()//从ini读入设置
{
	TCHAR szAppPath[MAX_PATH];
	ZeroMemory(szAppPath,MAX_PATH*sizeof(TCHAR));
	::GetModuleFileName(AfxGetInstanceHandle(), szAppPath, MAX_PATH);
	::PathRemoveFileSpec(szAppPath);

	TCHAR szKeyVal[256];
	//[RADARSOURCE]
	ZeroMemory(szKeyVal,256*sizeof(TCHAR));
	::GetPrivateProfileString(_T("RADARSOURCE"),_T("AFILERADARLINE"),_T(""), szKeyVal, MAX_PATH, m_strRadarIni);
	CString strTemp = szKeyVal;
	USES_CONVERSION;
	std::string strTe = W2A(strTemp);
	m_iAFileRadarLine = atoi(strTe.c_str());
	if (4999 >= m_iAFileRadarLine || m_iAFileRadarLine > 500000)
	{
		//100000约200M 195
		m_iAFileRadarLine = 100000;
	}
	ZeroMemory(szKeyVal,256*sizeof(TCHAR));
	::GetPrivateProfileString(_T("RADARWORKTYPE"),_T("TOPWINDOW"),_T(""), szKeyVal, MAX_PATH, m_strRadarIni);

	strTemp = szKeyVal;
	if (0 == strTemp.Compare(_T("WPF")))
	{
		m_iTopWindowType = TOP_WINDOWS_TYPE_WPF;
	}
	else
	{
		m_iTopWindowType = TOP_WINDOWS_TYPE_MFC;
	}

	//[RADARWORKTYPE]
	//工作模式
	ZeroMemory(szKeyVal,256*sizeof(TCHAR));
	::GetPrivateProfileString(_T("RADARWORKTYPE"),_T("WORKTYPE"),_T(""), szKeyVal, MAX_PATH, m_strRadarIni);
	strTemp = szKeyVal;
	strTe = W2A(strTemp);
	if (0 == strTe.compare("USB"))
	{
		ZeroMemory(szKeyVal,256*sizeof(TCHAR));
		::GetPrivateProfileString(_T("RADARWORKTYPE"),_T("USBType"),_T(""), szKeyVal, MAX_PATH, m_strRadarIni);
		CString USBTypeStr = szKeyVal;
		std::string pUSBTypeStr = W2A(USBTypeStr);
		if (0 == pUSBTypeStr.compare("1"))
		{
			m_iRadarWorkType = RADAR_WORK_TYPE_ONE_USB;
			m_USBType = USBType::OneChannel;
		} 
		else if (0 == pUSBTypeStr.compare("2"))
		{
			m_iRadarWorkType = RADAR_WORK_TYPE_DOUBLE_USB;
			m_USBType = USBType::TwoChannel;
		}
		else if (0 == pUSBTypeStr.compare("3"))
		{
			m_iRadarWorkType = RADAR_WORK_TYPE_DOUBLE_USB_OLD;
			m_USBType = USBType::TwoChannelOld;
		}
		else if (0 == pUSBTypeStr.compare("4"))
		{ 
			m_iRadarWorkType = RADAR_WORK_TYPE_FOUR_USB;
			m_USBType = USBType::FourChannel;
		}
		m_WorkType = USB;//new
	}
	else if (0 == strTe.compare("NETCable"))
	{
		m_iRadarWorkType = RADAR_WORK_TYPE_EIGHT;//old
		m_WorkType = NETCable;//new
	}

	//11.3 hjl
	//判断网口模式下各通道的是否使用情况
	//char *LNetChannelOneAvailability[8]={"NetChannelOneAvailability","NetChannelTwoAvailability","NetChannelThreeAvailability","NetChannelFourAvailability","NetChannelFiveAvailability","NetChannelSixAvailability","NetChannelSevenAvailability","NetChannelEightAvailability"};	
	LPCTSTR NetChannelAvailability[8] = {_T("NetChannelOneAvailability"),_T("NetChannelTwoAvailability"),_T("NetChannelThreeAvailability"),_T("NetChannelFourAvailability"),_T("NetChannelFiveAvailability"),_T("NetChannelSixAvailability"),_T("NetChannelSevenAvailability"),_T("NetChannelEightAvailability")};	
	for(int i=0;i<8;i++){
		ZeroMemory(szKeyVal,256*sizeof(TCHAR));
		::GetPrivateProfileString(_T("RADARWORKTYPE"),NetChannelAvailability[i],_T(""), szKeyVal, MAX_PATH, m_strRadarIni);
		strTemp = szKeyVal;
		if(0 == strTemp.Compare(_T("1"))){
			int_Check_Channel[i]=1;
		}else{
			int_Check_Channel[i]=0;
		}
	}
	/*
	ZeroMemory(szKeyVal,256*sizeof(TCHAR));
	::GetPrivateProfileString(_T("RADARWORKTYPE"),_T("NetChannelTwoAvailability"),_T(""), szKeyVal, MAX_PATH, m_strRadarIni);
	strTemp = szKeyVal;
	if(0 == strTemp.Compare(_T("1"))){
		int_Check_Channel[1]=1;
	}else{
		int_Check_Channel[1]=0;
	}
	ZeroMemory(szKeyVal,256*sizeof(TCHAR));
	::GetPrivateProfileString(_T("RADARWORKTYPE"),_T("NetChannelThreeAvailability"),_T(""), szKeyVal, MAX_PATH, m_strRadarIni);
	strTemp = szKeyVal;
	if(0 == strTemp.Compare(_T("1"))){
		int_Check_Channel[2]=1;
	}else{
		int_Check_Channel[2]=0;
	}
	ZeroMemory(szKeyVal,256*sizeof(TCHAR));
	::GetPrivateProfileString(_T("RADARWORKTYPE"),_T("NetChannelFourAvailability"),_T(""), szKeyVal, MAX_PATH, m_strRadarIni);
	strTemp = szKeyVal;
	if(0 == strTemp.Compare(_T("1"))){
		int_Check_Channel[3]=1;
	}else{
		int_Check_Channel[3]=0;
	}
	ZeroMemory(szKeyVal,256*sizeof(TCHAR));
	::GetPrivateProfileString(_T("RADARWORKTYPE"),_T("NetChannelFiveAvailability"),_T(""), szKeyVal, MAX_PATH, m_strRadarIni);
	strTemp = szKeyVal;
	if(0 == strTemp.Compare(_T("1"))){
		int_Check_Channel[4]=1;
	}else{
		int_Check_Channel[4]=0;
	}
	ZeroMemory(szKeyVal,256*sizeof(TCHAR));
	::GetPrivateProfileString(_T("RADARWORKTYPE"),_T("NetChannelSixAvailability"),_T(""), szKeyVal, MAX_PATH, m_strRadarIni);
	strTemp = szKeyVal;
	if(0 == strTemp.Compare(_T("1"))){
		int_Check_Channel[5]=1;
	}else{
		int_Check_Channel[5]=0;
	}
	ZeroMemory(szKeyVal,256*sizeof(TCHAR));
	::GetPrivateProfileString(_T("RADARWORKTYPE"),_T("NetChannelSevenAvailability"),_T(""), szKeyVal, MAX_PATH, m_strRadarIni);
	strTemp = szKeyVal;
	if(0 == strTemp.Compare(_T("1"))){
		int_Check_Channel[6]=1;
	}else{
		int_Check_Channel[6]=0;
	}
	ZeroMemory(szKeyVal,256*sizeof(TCHAR));
	::GetPrivateProfileString(_T("RADARWORKTYPE"),_T("NetChannelEightAvailability"),_T(""), szKeyVal, MAX_PATH, m_strRadarIni);
	strTemp = szKeyVal;
	if(0 == strTemp.Compare(_T("1"))){
		int_Check_Channel[7]=1;
	}else{
		int_Check_Channel[7]=0;
	}
	*/

	//存储方式
	ZeroMemory(szKeyVal,256*sizeof(TCHAR));
	::GetPrivateProfileString(_T("RADARWORKTYPE"),_T("SaveType"),_T(""), szKeyVal, MAX_PATH, m_strRadarIni);
	strTemp = szKeyVal;
	if (strTemp.Compare(L"Oracle") ==0)
	{
		m_SaveType = SaveType::Oracle;
		m_iSaveOracle = 1;
	}
	else if (strTemp.Compare(L"File") ==0)
	{
		m_SaveType = SaveType::File;
		m_iSaveOracle = 0;
	}


	//获取保存路径
	ZeroMemory(szKeyVal,256*sizeof(TCHAR));
	::GetPrivateProfileString(_T("RADARWORKTYPE"),_T("SAVEPATH"),_T(""), szKeyVal, MAX_PATH, m_strRadarIni);
	strTemp = szKeyVal;
	if (!PathIsDirectory(strTemp) ) { 
		strTemp = szAppPath;
	}

	m_strSavePath = strTemp.Right(1);
	if (m_strSavePath.Compare(L"\\") == 0){
		m_strSavePath = strTemp;
	}else{
		m_strSavePath = strTemp + L"\\";
	}
	m_strImagePath = W2A(m_strSavePath + L"imageData\\");
}

//更新radar模块的配置文件
void RadarManager::WriteRadarIni(){
	USES_CONVERSION;
	CString szKeyVal;
	//连接方式
	if (m_WorkType == WorkType::USB){
		szKeyVal=_T("USB");
		::WritePrivateProfileString(_T("RADARWORKTYPE"), _T("WORKTYPE"), szKeyVal, m_strRadarIni);//7.24
	}
	else if (m_WorkType == WorkType::NETCable){
		szKeyVal=_T("NETCable");
		m_iRadarWorkType = RADAR_WORK_TYPE_EIGHT;//7.15
		::WritePrivateProfileString(_T("RADARWORKTYPE"), _T("WORKTYPE"), szKeyVal, m_strRadarIni);//7.24
		::WritePrivateProfileString(_T("RADARWORKTYPE"), _T("USBType"),  _T("8"), m_strRadarIni);//7.24
		LPCTSTR NetChannelAvailability[8] = {_T("NetChannelOneAvailability"),_T("NetChannelTwoAvailability"),_T("NetChannelThreeAvailability"),_T("NetChannelFourAvailability"),_T("NetChannelFiveAvailability"),_T("NetChannelSixAvailability"),_T("NetChannelSevenAvailability"),_T("NetChannelEightAvailability")};
		for(int i=0;i<8;i++){
			if(int_Check_Channel[i]==1){
				::WritePrivateProfileString(_T("RADARWORKTYPE"), NetChannelAvailability[i],  _T("1"), m_strRadarIni);//11.3
			}else{
				::WritePrivateProfileString(_T("RADARWORKTYPE"), NetChannelAvailability[i],  _T("0"), m_strRadarIni);//11.3
			}
		}
		/*
		if(int_Check_Channel1==1){
			::WritePrivateProfileString(_T("RADARWORKTYPE"), _T("NetChannelOneAvailability"),  _T("1"), m_strRadarIni);//11.3
		}else{
			::WritePrivateProfileString(_T("RADARWORKTYPE"), _T("NetChannelOneAvailability"),  _T("0"), m_strRadarIni);//11.3
		}
		if(int_Check_Channel2==1){
			::WritePrivateProfileString(_T("RADARWORKTYPE"), _T("NetChannelTwoAvailability"),  _T("1"), m_strRadarIni);//11.3
		}else{
			::WritePrivateProfileString(_T("RADARWORKTYPE"), _T("NetChannelTwoAvailability"),  _T("0"), m_strRadarIni);//11.3
		}
		if(int_Check_Channel3==1){
			::WritePrivateProfileString(_T("RADARWORKTYPE"), _T("NetChannelThreeAvailability"),  _T("1"), m_strRadarIni);//11.3
		}else{
			::WritePrivateProfileString(_T("RADARWORKTYPE"), _T("NetChannelThreeAvailability"),  _T("0"), m_strRadarIni);//11.3
		}
		if(int_Check_Channel4==1){
			::WritePrivateProfileString(_T("RADARWORKTYPE"), _T("NetChannelFourAvailability"),  _T("1"), m_strRadarIni);//11.3
		}else{
			::WritePrivateProfileString(_T("RADARWORKTYPE"), _T("NetChannelFourAvailability"),  _T("0"), m_strRadarIni);//11.3
		}
		if(int_Check_Channel5==1){
			::WritePrivateProfileString(_T("RADARWORKTYPE"), _T("NetChannelFiveAvailability"),  _T("1"), m_strRadarIni);//11.3
		}else{
			::WritePrivateProfileString(_T("RADARWORKTYPE"), _T("NetChannelFiveAvailability"),  _T("0"), m_strRadarIni);//11.3
		}
		if(int_Check_Channel6==1){
			::WritePrivateProfileString(_T("RADARWORKTYPE"), _T("NetChannelSixAvailability"),  _T("1"), m_strRadarIni);//11.3
		}else{
			::WritePrivateProfileString(_T("RADARWORKTYPE"), _T("NetChannelSixAvailability"),  _T("0"), m_strRadarIni);//11.3
		}
		if(int_Check_Channel7==1){
			::WritePrivateProfileString(_T("RADARWORKTYPE"), _T("NetChannelSevenAvailability"),  _T("1"), m_strRadarIni);//11.3
		}else{
			::WritePrivateProfileString(_T("RADARWORKTYPE"), _T("NetChannelSevenAvailability"),  _T("0"), m_strRadarIni);//11.3
		}
		if(int_Check_Channel8==1){
			::WritePrivateProfileString(_T("RADARWORKTYPE"), _T("NetChannelEightAvailability"),  _T("1"), m_strRadarIni);//11.3
		}else{
			::WritePrivateProfileString(_T("RADARWORKTYPE"), _T("NetChannelEightAvailability"),  _T("0"), m_strRadarIni);//11.3
		}
		*/
	}
		
	//USB类型
	if (m_WorkType == WorkType::USB){
		if (m_USBType == USBType::OneChannel){
			szKeyVal=_T("1");
			m_iRadarWorkType = RADAR_WORK_TYPE_ONE_USB;
		}
		else if (m_USBType == USBType::TwoChannel){
			szKeyVal=_T("2");
			m_iRadarWorkType = RADAR_WORK_TYPE_DOUBLE_USB;
		}
		else if (m_USBType == USBType::TwoChannelOld){
			szKeyVal=_T("3");
			m_iRadarWorkType = RADAR_WORK_TYPE_DOUBLE_USB_OLD;
		}
		else if (m_USBType == USBType::FourChannel){
			szKeyVal=_T("4");
			m_iRadarWorkType = RADAR_WORK_TYPE_FOUR_USB;
		}
		::WritePrivateProfileString(_T("RADARWORKTYPE"), _T("USBType"),  szKeyVal,m_strRadarIni);
	}
	
	//存储方式
	if (m_SaveType == SaveType::Oracle){
		szKeyVal=_T("Oracle");
		m_iSaveOracle = 1;
	}else if (m_SaveType == SaveType::File){
		szKeyVal=_T("File");
		m_iSaveOracle = 0;
	}
	::WritePrivateProfileString(_T("RADARWORKTYPE"), _T("SaveType"),  szKeyVal, m_strRadarIni);
	::WritePrivateProfileString(_T("RADARWORKTYPE"), _T("SAVEPATH"),  m_strSavePath, m_strRadarIni);

	::WritePrivateProfileString(_T("VIDEOSOURCE"), _T("IMAGEPATH"),  A2W(m_strImagePath.c_str()),m_strRadarIni);

	m_configureSet->write();
}

void RadarManager::DataExport(CWnd* wnd)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	//DlgDataExport* pDlgDataExport = new DlgDataExport(wnd);
	//if(!pDlgDataExport->GetSafeHwnd())
	//{
	//	pDlgDataExport->Create(DlgDataExport::IDD);
	//}
	//pDlgDataExport->ShowWindow(SW_NORMAL);
	DlgDataExport dlg(wnd);
	//dlg.SetMyWndText(CString(_T("导出工程")));
	if ( dlg.DoModal() != IDOK )
	{
		return ;
	}
}

string Int_to_String(int n)
{
	ostringstream stream;
	stream<<n; //n为int类型
	return stream.str();
}


void RadarManager::DataMarkOne()
{
	//_radarReader.MarkOne();
	_radarReader.Mark(1,-1);
}

void RadarManager::DataMarkTwo()
{
	//_radarReader.MarkOne();
	_radarReader.Mark(1,-2);
}

void RadarManager::DataMarkThree()
{
	//_radarReader.MarkOne();
	_radarReader.Mark(1,-3);
}

int getSecondValueFromMarkValue(int markValue){
	//int i=0;
	if (markValue>=0){
		if (markValue<256){
			return 0;
		}
		return markValue/256;
	}else{
		if (markValue>-256){
			return -1;
		}else if(markValue%(-256)!=0){//只要不是整除 第一个数都会有数要+256来使数位一大于零 所以第二个数-1来适应
			return -markValue/(-256)-1;
		}
		return -markValue/(-256);
	}
}

int getFirstValueFromMarkValue(int markValue){//第一个数必须是正数 否则-1=255 -2=254
	if(markValue>=0){
		if (markValue<256){
			return markValue;
		}
		return markValue%256;
	}else{
		if (markValue>-256){
			return markValue+256;
		}
		return markValue%(-256)+256;//+256为了修正成正数，负数不能正确表示-1=255 -2=254
	}
}

void RadarManager::DataMarkCustomize()
{
	//_radarReader.MarkOne();	
	_radarReader.Mark(getFirstValueFromMarkValue(atoi(RadarManager::Instance()->getConfigureSet()->get("mark", "markValue").c_str())),getSecondValueFromMarkValue(atoi(RadarManager::Instance()->getConfigureSet()->get("mark", "markValue").c_str())));
}

int RadarManager::originalIndexToRecordIndex(int originalIndex)
{
	//int indexArray[]={0,2,4,6,8,10,12,14,16,18,20,22,23,25,27,1,3,5,7,9,11,13,15,17,19,21,24,26};
	int indexArray[]={0,1,2,3,4,5,6,7,8,9,10,11,24,25,26,27,12,13,14,15,16,17,18,19,20,21,22,23};
	return indexArray[originalIndex];
}

/*
int RadarManager::originalIndexToArtificialIndex(int originalIndex)
{
	//int indexArray[]={0,2,4,6,8,10,12,14,16,18,20,22,23,25,27,1,3,5,7,9,11,13,15,17,19,21,24,26};
	int indexArray[]={0,1,2,3,4,5,6,7,8,9,10,11,25,26,27,12.13,14,15,16,17,18,19,20,21,22,23,24};
	return indexArray[originalIndex];
}

bool RadarManager::artificialChannelIndexCheck(int originalIndex)
{
	int checkArray[]={1,2,3,4,5,6,7,8,9,10,11,13,14};
	for(int i = 0 ; i < 13 ; i++){
		if(originalIndex==checkArray[i]){
			return true;
		}
	}
	return false;
}

int RadarManager::getArtificalChannelIndexFromOriginalIndex(int originalIndex)
{
	//int indexArray[]={0,15,16,17,18,19,20,21,22,23,24,25,0,26,27};
	int indexArray[]={0,12,13,14,15,16,17,18,19,20,21,22,0,23,24};
	return indexArray[originalIndex];
}
*/
