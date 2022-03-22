// DlgDataExport.cpp : 实现文件
//

#include "stdafx.h"



//self
#include "RadarCurve.h"
#include "DlgDataExport.h"
#include "ProjectNameTab.h"
#include "RadarCurve/RadarManager.h"
#include "ProcessDlg.h"
#include "DataTemplateTab.h"
#include "DataCountTab.h"
#include "VideoTemplateTab.h"

#include "SGYWriter.h"

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

// DlgDataExport 对话框

IMPLEMENT_DYNAMIC(DlgDataExport, CDialog)

DlgDataExport::DlgDataExport(CWnd* pParent /*=NULL*/)
	: CDialog(DlgDataExport::IDD, pParent)
	, m_strEditExportPath(_T(""))
{
	m_projectList = RadarManager::Instance()->getProjectList();
	m_indexSelect = -1;
}

DlgDataExport::~DlgDataExport()
{
}

void DlgDataExport::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_PROJECT_INFO, m_ProjectListCtrl);
	DDX_Text(pDX, IDC_EditFilePath, m_strEditExportPath);
}


BEGIN_MESSAGE_MAP(DlgDataExport, CDialog)
	ON_BN_CLICKED(IDC_BtnExportGPS, &DlgDataExport::OnBnClickedBtnexportgps)
	ON_BN_CLICKED(IDCANCEL, &DlgDataExport::OnBnClickedCancel)
//	ON_NOTIFY(LVN_KEYDOWN, IDC_LIST_PROJECT_INFO, &DlgDataExport::OnLvnKeydownListProjectInfo)
ON_BN_CLICKED(IDC_BtnExportProject, &DlgDataExport::OnBnClickedBtnexportproject)
ON_BN_CLICKED(IDC_BtnFilePath, &DlgDataExport::OnBnClickedBtnfilepath)
ON_BN_CLICKED(IDC_BtnExportFIle, &DlgDataExport::OnBnClickedBtnexportfile)
END_MESSAGE_MAP()


// DlgDataExport 消息处理程序

void DlgDataExport::OnBnClickedBtnexportgps()
{
	std::vector<int> projectIndexVec;
	if (!GetSelectProjectIndex(projectIndexVec))
	{
		return;
	}

	if (m_strEditExportPath.Compare(_T("")) == 0)
	{
		MessageBox(L"请选择导出路径！", L"警告");
		return;
	}
	SetCursor( LoadCursor(NULL, IDC_WAIT) );	
	CProcessDlg* _loadProcess = new CProcessDlg; 
	_loadProcess->Create( CProcessDlg::IDD );

	if ( !RadarManager::Instance()->dbOpen() )
	{
		return ;
	}
	//要导出工程的个数
	int exportProjectNum = projectIndexVec.size();
	for (int exportProjectI = 0; exportProjectI < exportProjectNum;++exportProjectI)//按工程导出
	{
		int projectIndex = projectIndexVec[exportProjectI];
		CString selectProjectName = m_ProjectListCtrl.GetItemText( projectIndex, 0 );
		CString createTime = m_ProjectListCtrl.GetItemText( projectIndex, 1 );
		float selectLen = _wtof ( m_ProjectListCtrl.GetItemText( projectIndex, 2 ) );

		osg::ref_ptr<DBRow> pDBRow = (*m_projectList)[projectIndex];
		ProjectNameTab::ProjectNameRow *lpProjectRow = dynamic_cast<ProjectNameTab::ProjectNameRow *>( pDBRow.get());
		if ( !lpProjectRow )
		{
			MessageBox(L"变量错误", L"警告");
			return;
		}
		createTime.Format(L"%ld", lpProjectRow->_createTime);
		if (lpProjectRow->_curLen<=0.0000001)
		{
			MessageBox(L"没有GPS数据！", L"警告");
			continue;
		}

		USES_CONVERSION;

		//雷达距离经纬度照片的文件路径
		std::string pDisPicfilePath = W2A(m_strEditExportPath);
		pDisPicfilePath+="\\";
		pDisPicfilePath += W2A(selectProjectName);
		pDisPicfilePath+="_DisLLPic";
		pDisPicfilePath +=".txt";
		const char* fileChar = pDisPicfilePath.c_str();
		std::ofstream infile(fileChar, std::ios::trunc);//清除已有的

		CString strWndText = _T("正在导出文件：");
		strWndText += selectProjectName;
		CString tempStr = A2W(pDisPicfilePath.c_str());
		strWndText += tempStr;
		_loadProcess->SetWindowText(strWndText);

		string pStrselectProjectTab = W2A(createTime);

		//输出每个频道的雷达数据

		for (int pChannelIndexI = 0 ; pChannelIndexI < 8 ; ++pChannelIndexI){
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
			if ( !_loadProcess ){
				_loadProcess = new CProcessDlg;
				_loadProcess->Create( CProcessDlg::IDD );
			}
			if (0 == count){
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

void DlgDataExport::writeDisLLPic(std::string videoTabSerialNum, RadarData *lpRadarData,std::ofstream& infile)
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

void DlgDataExport::writeDisLLPicNew(RadarData *lpRadarData,std::ofstream& infile)
{
	//获取距离
	infile.precision(5);
	double distance = lpRadarData->getLen();

	osg::Vec3d pos = lpRadarData->getPosition();
	//获取vediotaB中时间对应的图片AB的路径
	DWORD receiveTime = lpRadarData->getTime();

	std::stringstream ss;
	ss << receiveTime;

	/*
	VideoTemplateTab pVideoTemplateTab;
	pVideoTemplateTab.setProjectName(videoTabSerialNum);
	*/

	//根据receiveTime去重获取图片的路径
	infile<<"distance："<<setiosflags(ios::left)<<setiosflags(ios::fixed)<<setprecision(1)<<setw(5)<<setfill('0')<<distance<<"  "
		<<setiosflags(ios::left)<<setw(12)<<setfill('0')<<setprecision(12)<<pos.x()<<" "
		<<setiosflags(ios::left)<<setw(12)<<setfill('0')<<setprecision(12)<<pos.y()<<" "
		<<"没有照片"<<std::endl;
}

void DlgDataExport::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	OnCancel();
}

BOOL DlgDataExport::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_ProjectListCtrl.InsertColumn( 0, L"工程名称", LVCFMT_LEFT, 150);
	m_ProjectListCtrl.InsertColumn( 1, L"创建时间", LVCFMT_LEFT, 150);
	m_ProjectListCtrl.InsertColumn( 2, L"桩号", LVCFMT_LEFT, 80);
	m_ProjectListCtrl.InsertColumn( 3, L"位置", LVCFMT_LEFT, 80);
	m_ProjectListCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT);

	m_ProjectListCtrl.DeleteAllItems();
	USES_CONVERSION;
	for ( unsigned int i = 0; i < m_projectList->size(); i ++ )
	{
		osg::ref_ptr<DBRow> pDBRow = (*m_projectList)[i];
		ProjectNameTab::ProjectNameRow *lpRow = dynamic_cast<ProjectNameTab::ProjectNameRow *>( pDBRow.get());
		if ( !lpRow )
		{
			continue;
		}
		m_ProjectListCtrl.InsertItem( i, A2W( lpRow->_projectName.c_str()) );
		CString str;
		struct tm *lt = localtime( &(lpRow->_createTime) );
		str.Format( L"%04d-%02d-%02d %02d:%02d:%02d", lt->tm_year + 1900, lt->tm_mon + 1, lt->tm_mday, lt->tm_hour, lt->tm_min, lt->tm_sec);
		m_ProjectListCtrl.SetItemText( i, 1, str );

		str.Format(L"%0.3f",lpRow->_startLen );
		m_ProjectListCtrl.SetItemText( i, 2, str );

		CString strZhunagHaoEX;
		CString  strJULI ;
		//GetAccountLength(lpRow->_startLen, strZhunagHaoEX, strJULI );
		m_ProjectListCtrl.SetItemText( i, 2, strZhunagHaoEX );
		m_ProjectListCtrl.SetItemText( i, 3, strJULI );
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

//void DlgDataExport::OnLvnKeydownListProjectInfo(NMHDR *pNMHDR, LRESULT *pResult)
//{
//	LPNMLVKEYDOWN pLVKeyDow = reinterpret_cast<LPNMLVKEYDOWN>(pNMHDR);
//	if (pLVKeyDow->wVKey == VK_CONTROL)
//	{
//		int zzz = 0;
//	}
//
//	if (pLVKeyDow->wVKey == 65)
//	{
//		int zzz = 0;
//	}
//	
//	
//	
//
//
//	// TODO: 在此添加控件通知处理程序代码
//	*pResult = 0;
//}


BOOL DlgDataExport::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类

	return CDialog::PreTranslateMessage(pMsg);
}

bool DlgDataExport::GetSelectProjectIndex(std::vector<int>& pSelectedIndex)
{
	POSITION p = m_ProjectListCtrl.GetFirstSelectedItemPosition();
	if ( !p )
	{
		MessageBox(L"请选择工程", L"警告");
		return false;
	}
	int index = m_ProjectListCtrl.GetNextSelectedItem(p);

	
	while (index >= 0)
	{
		pSelectedIndex.push_back(index);
		index = m_ProjectListCtrl.GetNextSelectedItem( p );
	}
}

void DlgDataExport::OnBnClickedBtnexportproject()
{
	// TODO: 在此添加控件通知处理程序代码
	std::vector<int> projectIndexVec;
	if (!GetSelectProjectIndex(projectIndexVec))
	{
		return;
	}
	if (m_strEditExportPath.Compare(_T("")) == 0)
	{
		MessageBox(L"请选择导出路径！", L"警告");
		return;
	}
	CString strPath,strProName,strProTime;
	for (int i = 0 ; i < projectIndexVec.size();++i)
	{
		int index = projectIndexVec[i];

		strProName = m_ProjectListCtrl.GetItemText( index, 0 );
		strPath = m_strEditExportPath + L"\\" + strProName;
		osg::ref_ptr<DBRow> pDBRow = (*m_projectList)[i];
		ProjectNameTab::ProjectNameRow *lpRow = dynamic_cast<ProjectNameTab::ProjectNameRow *>( pDBRow.get());
		if ( !lpRow )
		{
			MessageBox(L"变量错误", L"警告");
			return;
		}
		strProTime.Format(L"%ld", lpRow->_createTime);

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
		CString strWndText;
		strWndText.Format(L"共%d个工程，正在导出第%d个:",projectIndexVec.size(),(i+1));
		strWndText += strProName;
		RadarManager::Instance()->exportProject(strPath,strProTime,true,false,strWndText);
	}
	::MessageBox(NULL,L"导出工程成功!",L"导出工程", MB_OK); 
}

void DlgDataExport::OnBnClickedBtnfilepath()
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
	GetDlgItem(IDC_EditFilePath)->SetWindowText(szDir);
	UpdateWindow();
}

void DlgDataExport::OnBnClickedBtnexportfile()
{
	// TODO: 在此添加控件通知处理程序代码
	std::vector<int> projectIndexVec;
	if (!GetSelectProjectIndex(projectIndexVec))
	{
		return;
	}

	if (m_strEditExportPath.Compare(_T("")) == 0)
	{
		MessageBox(L"请选择导出路径！", L"警告");
		return;
	}
	SetCursor( LoadCursor(NULL, IDC_WAIT) );	
	CProcessDlg* pLoadProcess = new CProcessDlg; 
	pLoadProcess->Create( CProcessDlg::IDD );

	//要到处工程的个数
	int exportProjectNum = projectIndexVec.size();
	for (int exportProjectI = 0; exportProjectI < exportProjectNum;++exportProjectI)
	{
		int projectIndex = projectIndexVec[exportProjectI];
		CString selectProjectName = m_ProjectListCtrl.GetItemText( projectIndex, 0 );
		CString createTime = m_ProjectListCtrl.GetItemText( projectIndex, 1 );
		float currentLen = _wtof ( m_ProjectListCtrl.GetItemText( projectIndex, 2 ) );
		osg::ref_ptr<DBRow> pDBRow = (*m_projectList)[projectIndex];
		ProjectNameTab::ProjectNameRow *lpProjectRow = dynamic_cast<ProjectNameTab::ProjectNameRow *>( pDBRow.get());
		if ( !lpProjectRow )
		{
			MessageBox(L"变量错误", L"警告");
			return;
		}
		createTime.Format(L"%ld", lpProjectRow->_createTime);
		USES_CONVERSION;


		bool IsWriteLLPic = true;
		//输出每个频道的雷达数据
		for (int pChannelIndexI = 0 ; pChannelIndexI < 8 ; ++pChannelIndexI)
		{
			if ( !RadarManager::Instance()->dbOpen() )
			{
				return ;
			}
			DataTemplateTab tab;
			tab.setProjectName( W2A(createTime ));
			int count = 0;
			{
				DataCountTab countTab;
				countTab.setTabName( tab.getProjectName() );
				std::stringstream ss;
				ss << countTab.getSelectSql();
				ss << " where ChannelIndex=" << pChannelIndexI;
				count = countTab.getTabDataCount( RadarManager::Instance()->getDBHelp(), ss.str());
			}

			if ( !pLoadProcess )
			{
				pLoadProcess = new CProcessDlg;
				pLoadProcess->Create( CProcessDlg::IDD );
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
				strSgyFilePath += selectProjectName;

				CString str;
				USES_CONVERSION;
				str.Format(L"_%s_通道%d-%s.SGY"
					,createTime
					,(pChannelIndexI + 1)
					, A2W(RadarManager::Instance()->getChannelName(pChannelIndexI).c_str() ));
				strSgyFilePath += str;
				CString strWndText = _T("正在导出文件：");
				strWndText += selectProjectName;
				strWndText += str;
				pLoadProcess->SetWindowText(strWndText);
				pLoadProcess->StartProcess( count );
			}

			//写SGY的头
			SGYWriter writer;
			writer.openSGY(W2A(strSgyFilePath), lpProjectRow->_paramXML, pChannelIndexI );
			writer.writeHead(1 ,W2A ( createTime ));

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
					pLoadProcess->Step( 1 );
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

			pLoadProcess->ShowWindow( SW_HIDE );

			IsWriteLLPic = false;
			tab.close();
		}
	}
	delete pLoadProcess;
	SetCursor( LoadCursor(NULL, IDC_ARROW) );
	MessageBox(L"导出文件结束！");
}
