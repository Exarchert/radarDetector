// DialogProject.cpp : 实现文件
//

#include "stdafx.h"
#include "RadarCurve.h"
#include "DialogProject_Export.h"
#include "DialogProjectName.h"
#include "ProjectNameTab.h"
#include "VideoTemplateTab.h"
#include "DataTemplateTab.h"
#include <RadarCurve/RadarManager.h>
#include <RadarCurve/DBConnectBase.h>
#include "DialogHistoryCurve.h"

#include <Shlwapi.h>
#include "DataTemplateTab.h"
#include "SGYWriter.h"
#include "ProcessDlg.h"
#include "DataCountTab.h"
//C++
#include <sstream>
#include <fcntl.h>
#include <iostream>
#include <direct.h>
#include <io.h>
#include <map>
#include <vector>
#include <shlwapi.h>
#include <iomanip>
using namespace std;




// CDialogProject_Export 对话框
CDialogHistoryCurve *G_HistoryCurveDialog_Export[8] = { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL};

IMPLEMENT_DYNAMIC(CDialogProject_Export, CDialog)

CDialogProject_Export::CDialogProject_Export( CDBTable::RowVector & rv,int iType, CWnd* pParent /*=NULL*/)
	: CDialog(CDialogProject_Export::IDD, pParent)
	, _projectList( rv )
	, m_strEditExportPath(_T(""))
{
	m_iType = iType;

	m_strWndText = _T("选择工程");


	m_indexSelect = -1;
	TCHAR path[MAX_PATH];        
	ZeroMemory(path,MAX_PATH);        
	SHGetSpecialFolderPath(0,path,CSIDL_DESKTOPDIRECTORY,0);//
	m_strEditExportPath = path;
}

CDialogProject_Export::~CDialogProject_Export()
{
}

void CDialogProject_Export::SetMyWndText(CString str)
{
	m_strWndText = str;
}

void CDialogProject_Export::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_PROJECT_EXPORT, m_projectList);
	DDX_Text(pDX, IDC_EDIT_EXPORT, m_strEditExportPath);
}


BEGIN_MESSAGE_MAP(CDialogProject_Export, CDialog)

	ON_BN_CLICKED(IDOK, &CDialogProject_Export::OnBnClickedOk)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_PROJECT, &CDialogProject_Export::OnNMDblclkListProject)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_PROJECT, &CDialogProject_Export::OnNMRclickListProject)
	ON_BN_CLICKED(IDC_BUTTON_POSITONEXPORT, &CDialogProject_Export::OnBnClickedButtonPositonexport)
	ON_BN_CLICKED(IDC_BtnExportFIle, &CDialogProject_Export::OnBnClickedBtnexportfile)
END_MESSAGE_MAP()


void CDialogProject_Export::ReInit()
{
	m_projectList.DeleteAllItems();
	USES_CONVERSION;
	for ( unsigned int i = 0; i < _projectList.size(); i ++ )
	{
		ProjectNameTab::ProjectNameRow *lpRow = dynamic_cast<ProjectNameTab::ProjectNameRow *>( _projectList[i].get());
		if ( !lpRow )
		{
			continue;
		}
		m_projectList.InsertItem( i, A2W( lpRow->_projectName.c_str()) );
		CString str;
		struct tm *lt = localtime( &(lpRow->_createTime) );
		str.Format( L"%04d-%02d-%02d %02d:%02d:%02d", lt->tm_year + 1900, lt->tm_mon + 1, lt->tm_mday, lt->tm_hour, lt->tm_min, lt->tm_sec);
		m_projectList.SetItemText( i, 1, str );


		str.Format(L"%0.3f",lpRow->_startLen );
		m_projectList.SetItemText( i, 2, str );
	}

}

BOOL CDialogProject_Export::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetWindowText(m_strWndText);

	// TODO:  在此添加额外的初始化
	m_projectList.InsertColumn( 0, L"工程名称", LVCFMT_LEFT, 180);
	m_projectList.InsertColumn( 1, L"创建时间", LVCFMT_LEFT, 80);
	m_projectList.InsertColumn( 2, L"起始位置", LVCFMT_LEFT, 150);

	m_projectList.SetExtendedStyle(LVS_EX_FULLROWSELECT);

	USES_CONVERSION;
	for ( unsigned int i = 0; i < _projectList.size(); i ++ )
	{
		ProjectNameTab::ProjectNameRow *lpRow = dynamic_cast<ProjectNameTab::ProjectNameRow *>( _projectList[i].get());
		if ( !lpRow )
		{
			continue;
		}
	 	m_projectList.InsertItem( i, A2W( lpRow->_projectName.c_str()) );
		CString str;
		struct tm *lt = localtime( &lpRow->_createTime );
		str.Format( L"%04d-%02d-%02d %02d:%02d:%02d", lt->tm_year + 1900, lt->tm_mon + 1, lt->tm_mday, lt->tm_hour, lt->tm_min, lt->tm_sec);
	 	m_projectList.SetItemText( i, 1, str );
		

		str.Format(L"%0.3f",lpRow->_startLen );
		m_projectList.SetItemText( i, 2, str );
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDialogProject_Export::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码


	m_indexSelect = -1;
	if ( m_projectList.GetItemCount() == 0 )
	{
		return;
	}


	if (2 == m_iType)
	{
		OnBnClickedBtnExportAllSgy();
		return;
	}



	POSITION p = m_projectList.GetFirstSelectedItemPosition();

	if ( !p )
	{
		MessageBox(L"请选择工程", L"警告");
		return;
	}
	m_indexSelect = m_projectList.GetNextSelectedItem(p);

	if (1 == m_iType)
	{
		m_selectProjectName = m_projectList.GetItemText( m_indexSelect, 0 );

		m_strFolderPath = m_strEditExportPath + L"\\" + m_selectProjectName;

		if (!PathIsDirectory(m_strFolderPath) ) 
		{ 

			if (!CreateDirectory(m_strFolderPath, NULL ) ) 
			{ 
				CString strMsg; 
				strMsg.Format (L"创建路径\"%s\"失败！是否继续?",m_strFolderPath); 
				if (AfxMessageBox(strMsg, MB_YESNO) == IDYES) 
					return; 
			} 
		}
	} 
	else if (3 == m_iType)
	{
		m_vPath.clear();
		m_vProTime.clear();
		m_vProName.clear();


		int index = m_indexSelect;
		std::vector<int> projectIndexVec;
		while (index >= 0)
		{
			projectIndexVec.push_back(index);
			index = m_projectList.GetNextSelectedItem( p );
		}

		CString strPath,strProName,strProTime;
		for (int i = 0 ; i < projectIndexVec.size();++i)
		{
			int index = projectIndexVec[i];

			strProName = m_projectList.GetItemText( index, 0 );
			strPath = m_strEditExportPath + L"\\" + strProName;
			//strProTime = m_projectList.GetItemText( index, 1 );

			ProjectNameTab::ProjectNameRow *lpProjectRow = dynamic_cast<ProjectNameTab::ProjectNameRow *>(_projectList[index].get());
			if ( !lpProjectRow )
			{
				MessageBox(L"变量错误", L"警告");
				return;
			}
			strProTime.Format(L"%ld", lpProjectRow->_createTime);


			if (!PathIsDirectory(strPath) ) 
			{ 

				if (!CreateDirectory(strPath, NULL ) ) 
				{ 
					CString strMsg; 
					strMsg.Format (L"创建路径\"%s\"失败！是否继续?",strPath); 
					if (AfxMessageBox(strMsg, MB_YESNO) == IDYES) 
						return; 
				} 
			}

			m_vPath.push_back(strPath);
			m_vProTime.push_back(strProTime);
			m_vProName.push_back(strProName);
		}
	}

	else if (4 == m_iType)
	{
		m_vPath.clear();
		m_vProTime.clear();
		m_vProName.clear();

		int index = m_indexSelect;
		std::vector<int> projectIndexVec;
		while (index >= 0)
		{
			projectIndexVec.push_back(index);
			index = m_projectList.GetNextSelectedItem( p );
		}

		CString strPath,strProName,strProTime;
		for (int i = 0 ; i < projectIndexVec.size();++i)
		{
			int index = projectIndexVec[i];

			strProName = m_projectList.GetItemText( index, 0 );
			strPath = m_strEditExportPath + L"\\" + strProName;
			//strProTime = m_projectList.GetItemText( index, 1 );

			ProjectNameTab::ProjectNameRow *lpProjectRow = dynamic_cast<ProjectNameTab::ProjectNameRow *>(_projectList[index].get());
			if ( !lpProjectRow )
			{
				MessageBox(L"变量错误", L"警告");
				return;
			}
			strProTime.Format(L"%ld", lpProjectRow->_createTime);


			if (!PathIsDirectory(strPath) ) 
			{ 

				if (!CreateDirectory(strPath, NULL ) ) 
				{ 
					CString strMsg; 
					strMsg.Format (L"创建路径\"%s\"失败！是否继续?",strPath); 
					if (AfxMessageBox(strMsg, MB_YESNO) == IDYES) 
						return; 
				} 
			}

			m_vPath.push_back(strPath);
			m_vProTime.push_back(strProTime);
			m_vProName.push_back(strProName);
		}
	}


	OnOK();
}

void CDialogProject_Export::OnNMDblclkListProject(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	if ( m_iType > 0 )
	{
		OnBnClickedOk();
	}

	*pResult = 0;
}
void CDialogProject_Export::prepareDBTab( CString const& projectCreatTime )
{

	CDBConnectHelp *lpHelp = RadarManager::Instance()->getDBHelp();

	if ( !lpHelp )
	{
		return;
	}
	USES_CONVERSION;
	{
	
		VideoTemplateTab tab;
		tab.setProjectName( W2A(projectCreatTime ));
		tab.createTab( lpHelp );
	}

	{
		DataTemplateTab tab;
		tab.setProjectName( W2A(projectCreatTime) );
		tab.createTab( lpHelp );
	}


}
void CDialogProject_Export::OnNMRclickListProject(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码

	*pResult = 0;

	if ( m_iType > 0 )
	{
		return ;
	}


	CPoint p;
	GetCursorPos( &p );


	LVHITTESTINFO ht;
	ht.pt = p;
	m_projectList.ScreenToClient ( &ht.pt );
	m_projectList.HitTest( &ht );
	if ( ht.iItem < 0 )
	{
		return;
	}

	CMenu menu;
	menu.LoadMenu(IDR_MENU_HISTORY);

	menu.GetSubMenu(0)->TrackPopupMenu(TPM_LEFTALIGN|TPM_LEFTBUTTON|TPM_RIGHTBUTTON, p.x, p.y, this);


}



void CDialogProject_Export::OnBnClickedButtonPositonexport()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strDlgTitle = L"选择导出位置";
	TCHAR szDir[MAX_PATH];
	BROWSEINFO bi; ITEMIDLIST *pidl;
	bi.hwndOwner = this->m_hWnd; bi.pidlRoot = NULL; 
	bi.pszDisplayName = szDir; 
	bi.lpszTitle = strDlgTitle; 
	bi.ulFlags = BIF_RETURNONLYFSDIRS; 
	bi.lpfn = NULL; bi.lParam = 0;
	bi.iImage = 0;
	pidl = SHBrowseForFolder(&bi); 
	if(pidl == NULL) 
		return ;
	if(!SHGetPathFromIDList(pidl, szDir))
		return ;
	m_strEditExportPath = szDir;
	GetDlgItem(IDC_EDIT_EXPORT)->SetWindowText(szDir);
	UpdateWindow();
}


void CDialogProject_Export::OnBnClickedBtnExportAllSgy()
{
	// TODO: 在此添加控件通知处理程序代码
	POSITION p = m_projectList.GetFirstSelectedItemPosition();
	if ( !p )
	{
		MessageBox(L"请选择要导出文件的工程", L"警告");
		return;
	}
	int index = m_projectList.GetNextSelectedItem( p );
	std::vector<int> projectIndexVec;
	while (index >= 0)
	{
		projectIndexVec.push_back(index);
		index = m_projectList.GetNextSelectedItem( p );
	}

	SetCursor( LoadCursor(NULL, IDC_WAIT) );	
	CProcessDlg* _loadProcess = new CProcessDlg; 
	_loadProcess->Create( CProcessDlg::IDD );

	//要到处工程的个数
	int exportProjectNum = projectIndexVec.size();
	for (int exportProjectI = 0; exportProjectI < exportProjectNum;++exportProjectI)
	{
		int projectIndex = projectIndexVec[exportProjectI];
		m_selectProjectName = m_projectList.GetItemText( projectIndex, 0 );
		m_selectProjectTab = m_projectList.GetItemText( projectIndex, 1 );
		m_selectLen = _wtof ( m_projectList.GetItemText( projectIndex, 2 ) );
		//GetStartLength(m_selectLen, m_projectList.GetItemText( projectIndex, 2 ), m_projectList.GetItemText( projectIndex, 3 ));
		ProjectNameTab::ProjectNameRow *lpProjectRow = dynamic_cast<ProjectNameTab::ProjectNameRow *>(_projectList[projectIndex].get());
		if ( !lpProjectRow )
		{
			MessageBox(L"变量错误", L"警告");
			return;
		}
		m_selectProjectTab.Format(L"%ld", lpProjectRow->_createTime);
		USES_CONVERSION;

// 		//雷达距离经纬度照片的文件路径
// 		std::string pDisPicfilePath = W2A(m_strEditExportPath);
// 		pDisPicfilePath+="\\";
// 		pDisPicfilePath += W2A(m_selectProjectName);
// 		pDisPicfilePath+="_DisLLPic";
// 		pDisPicfilePath +=".txt";
// 		const char* fileChar = pDisPicfilePath.c_str();
// 		std::ofstream infile(fileChar, std::ios::trunc);//清除已有的
		

		

		bool IsWriteLLPic = true;
		//输出每个频道的雷达数据
		for (int pChannelIndexI = 0 ; pChannelIndexI < 8 ; ++pChannelIndexI)
		{
			if ( !RadarManager::Instance()->dbOpen() )
			{
				return ;
			}
			DataTemplateTab tab;
			tab.setProjectName( W2A(m_selectProjectTab ));
			int count = 0;
			{
				DataCountTab countTab;
				countTab.setTabName( tab.getProjectName() );
				std::stringstream ss;
				ss << countTab.getSelectSql();
				ss << " where ChannelIndex=" << pChannelIndexI;
				count = countTab.getTabDataCount( RadarManager::Instance()->getDBHelp(), ss.str());
			}

			if ( !_loadProcess )
			{
				_loadProcess = new CProcessDlg;
				_loadProcess->Create( CProcessDlg::IDD );
			}

			if (0 == count)
			{
				break;
			}
			
			std::stringstream ss;
			ss << tab.getSelectSql() << " where ChannelIndex=" << pChannelIndexI;
			ss << " order by id";

			std::string sqlStr = ss.str();
			if ( !tab.open( RadarManager::Instance()->getDBHelp(),  sqlStr) )
			{
				MessageBox(L"打开工程表表失败");
				return ;
			}
		
			
			//sgy文件路径
			CString strSgyFilePath = m_strEditExportPath;
			{
				strSgyFilePath += _T("\\");
				strSgyFilePath += m_selectProjectName;

				CString str;
				USES_CONVERSION;
				str.Format(L"_%s_通道%d-%s.SGY"
					,m_selectProjectTab
					,(pChannelIndexI + 1)
					, A2W(RadarManager::Instance()->getChannelName(pChannelIndexI).c_str() ));
				strSgyFilePath += str;
				CString strWndText = _T("正在导出文件：");
				strWndText += m_selectProjectName;
				strWndText += str;
				_loadProcess->SetWindowText(strWndText);
				_loadProcess->StartProcess( count );
			}
			

			

			//写SGY的头
			SGYWriter writer;
			writer.openSGY(W2A(strSgyFilePath), lpProjectRow->_paramXML, pChannelIndexI );
			writer.writeHead(1 ,W2A ( m_selectProjectTab ));

			CDBTable::RowVector rv;

			int iBuf[1000];
			int lenc = 2048;
			unsigned char *data;
			while(  ( rv = tab.read( 1000 )).size() != 0 )
			{
				for ( unsigned int i = 0; i < rv.size(); i ++ )
				{
					RadarData *lpRadarData = dynamic_cast<RadarData*>(rv[i].get());
					if ( lpRadarData )
					{
						writer.writeRadarData( lpRadarData );
 						
					}
					_loadProcess->Step( 1 );
					

					data = lpRadarData->getData(lenc);
					iBuf[i] = data[6] + data[7] * 256 ;
					if (i > 1)
					{
						if (iBuf[i] < iBuf[i-1])
						{
							data = NULL;
						}
					}
					
				}
				//lpDlg->Step( rv.size() );
 				if ( rv.size() < 1000 )
 				{
 					break;
 				}
			}
			writer.closeSGY();
			SetCursor( LoadCursor(NULL, IDC_ARROW) );

			_loadProcess->ShowWindow( SW_HIDE );

			IsWriteLLPic = false;
			tab.close();
		}
	}
	delete _loadProcess;
	SetCursor( LoadCursor(NULL, IDC_ARROW) );
	MessageBox(L"导出文件结束！");
}

void CDialogProject_Export::writeDisLLPic(std::string videoTabSerialNum, RadarData *lpRadarData,std::ofstream& infile)
{
	//获取距离
	infile.precision(5);
	double distance = lpRadarData->getLen();

	osg::Vec3d pos = lpRadarData->getPosition();
	//获取vediotaB中时间对应的图片AB的路径
	DWORD receiveTime = lpRadarData->getTime();

	std::stringstream ss;
	ss << receiveTime;

	VideoTemplateTab pVideoTemplateTab;
	pVideoTemplateTab.setProjectName(videoTabSerialNum);

	//根据receiveTime去重获取图片的路径
	std::string strPicPathA,strPicPathB;
	bool status = pVideoTemplateTab.getPicPath(RadarManager::Instance()->getDBHelp(), ss.str(),strPicPathA,strPicPathB);
	if (status)
	{
		infile<<"distance："<<setiosflags(ios::left)<<setiosflags(ios::fixed)<<setprecision(1)<<setw(5)<<setfill('0')<<distance<<"  "
			<<setiosflags(ios::left)<<setw(12)<<setfill('0')<<setprecision(12)<<pos.x()<<" "
			<<setiosflags(ios::left)<<setw(12)<<setfill('0')<<setprecision(12)<<pos.y()<<" "
			<<strPicPathA<<" "<<strPicPathB<<std::endl;
	}
	else
	{
		infile<<"distance："<<setiosflags(ios::left)<<setiosflags(ios::fixed)<<setprecision(1)<<setw(5)<<setfill('0')<<distance<<"  "
			<<setiosflags(ios::left)<<setw(12)<<setfill('0')<<setprecision(12)<<pos.x()<<" "
			<<setiosflags(ios::left)<<setw(12)<<setfill('0')<<setprecision(12)<<pos.y()<<" "
			<<"没有照片"<<std::endl;
	}
}

void CDialogProject_Export::OnBnClickedBtnexportfile()
{
	// TODO: 在此添加控件通知处理程序代码
	
	POSITION p = m_projectList.GetFirstSelectedItemPosition();
	if ( !p )
	{
		MessageBox(L"请选择要导出文件的工程", L"警告");
		return;
	}
	int index = m_projectList.GetNextSelectedItem( p );
	std::vector<int> projectIndexVec;
	while (index >= 0)
	{
		projectIndexVec.push_back(index);
		index = m_projectList.GetNextSelectedItem( p );
	}

	SetCursor( LoadCursor(NULL, IDC_WAIT) );	
	CProcessDlg* _loadProcess = new CProcessDlg; 
	_loadProcess->Create( CProcessDlg::IDD );

	if ( !RadarManager::Instance()->dbOpen() )
	{
		return ;
	}
	//要到处工程的个数
	int exportProjectNum = projectIndexVec.size();
	for (int exportProjectI = 0; exportProjectI < exportProjectNum;++exportProjectI)
	{
		int projectIndex = projectIndexVec[exportProjectI];
		m_selectProjectName = m_projectList.GetItemText( projectIndex, 0 );
		m_selectProjectTab = m_projectList.GetItemText( projectIndex, 1 );
		m_selectLen = _wtof ( m_projectList.GetItemText( projectIndex, 2 ) );
		//GetStartLength(m_selectLen, m_projectList.GetItemText( projectIndex, 2 ), m_projectList.GetItemText( projectIndex, 3 ));
		ProjectNameTab::ProjectNameRow *lpProjectRow = dynamic_cast<ProjectNameTab::ProjectNameRow *>(_projectList[projectIndex].get());
		if ( !lpProjectRow )
		{
			MessageBox(L"变量错误", L"警告");
			return;
		}
		m_selectProjectTab.Format(L"%ld", lpProjectRow->_createTime);
		if (lpProjectRow->_curLen<=0.0000001)
		{
			MessageBox(L"没有GPS数据！", L"警告");
			continue;
		}
		
		USES_CONVERSION;

		//雷达距离经纬度照片的文件路径
		std::string pDisPicfilePath = W2A(m_strEditExportPath);
		pDisPicfilePath+="\\";
		pDisPicfilePath += W2A(m_selectProjectName);
		pDisPicfilePath+="_DisLLPic";
		pDisPicfilePath +=".txt";
		const char* fileChar = pDisPicfilePath.c_str();
		std::ofstream infile(fileChar, std::ios::trunc);//清除已有的
		
		CString strWndText = _T("正在导出文件：");
		strWndText += m_selectProjectName;
		CString tempStr = A2W(pDisPicfilePath.c_str());
		strWndText += tempStr;
		_loadProcess->SetWindowText(strWndText);
		
		string pStrselectProjectTab = W2A(m_selectProjectTab);

		//输出每个频道的雷达数据
		for (int pChannelIndexI = 0 ; pChannelIndexI < 8 ; ++pChannelIndexI)
		{
			

			DataTemplateTab tab;
			tab.setProjectName(pStrselectProjectTab);
			int count = 0;
			{
				DataCountTab countTab;
				countTab.setTabName( tab.getProjectName() );
				std::stringstream ss;
				ss << countTab.getSelectSql();
				ss << " where ChannelIndex=" << pChannelIndexI;
				count = countTab.getTabDataCount( RadarManager::Instance()->getDBHelp(), ss.str());
			}

			if ( !_loadProcess )
			{
				_loadProcess = new CProcessDlg;
				_loadProcess->Create( CProcessDlg::IDD );
			}

			if (0 == count)
			{
				break;
			}

			_loadProcess->StartProcess(count);

			std::stringstream ss;
			ss << tab.getSelectSql() << " where ChannelIndex=" << pChannelIndexI;
			ss << " order by id";

			std::string sqlStr = ss.str();
			if ( !tab.open( RadarManager::Instance()->getDBHelp(),  sqlStr) )
			{
				continue;
			}
			

			CDBTable::RowVector rv;
			int iBuf[1000];
			int lenc = 2048;
			unsigned char *data;
			while(  ( rv = tab.read( 1000 )).size() != 0 )
			{
				for ( unsigned int i = 0; i < rv.size(); i ++ )
				{
					RadarData *lpRadarData = dynamic_cast<RadarData*>(rv[i].get());
					if ( lpRadarData )
					{
						writeDisLLPic( pStrselectProjectTab, lpRadarData,infile);
						_loadProcess->Step( 1 );
					}
				}
				if ( rv.size() < 1000 )
				{
					break;
				}
			}
			
			//_loadProcess->Step( count );
			SetCursor( LoadCursor(NULL, IDC_ARROW) );

			//IsWriteLLPic = false;
			tab.close();
			
			break;
		}
		infile.close();
		_loadProcess->ShowWindow( SW_HIDE );
	}
	delete _loadProcess;
	SetCursor( LoadCursor(NULL, IDC_ARROW) );
	MessageBox(L"导出文件结束！");
}
