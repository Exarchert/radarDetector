// DlgWorkDetection.cpp : 实现文件
//

#include "stdafx.h"

#include "Kernel\KernelExport.h"
#include "COM\Tools\ComFunction.h"
#include "COM\Tools\ProcessData.h"

//self
#include "RadarCurve.h"
#include "RadarCurve\RadarManager.h"
#include "DlgWorkDetection.h"

#include "ProjectNameTab.h"
#include "DataTemplateTab.h"
#include "DataCountTab.h"
#include "ProcessDlg.h"
#include "SGYWriter.h"

#include "MeasureLine.h"
#include <Object\CmdCenter\CmdCenter.h>
//C++
#include <iostream>
#include <fstream>

//osg
#include <osgDB/WriteFile>

#include "RadarCurve/RadarManager.h"
using namespace std;

// CDlgWorkDetection 对话框

IMPLEMENT_DYNAMIC(CDlgWorkDetection, CDialog)

CDlgWorkDetection::CDlgWorkDetection(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgWorkDetection::IDD, pParent)
	, m_Interval(0.5f)
	, m_pLineWidth(1.0f)
{
	CString pRadarWorkPath = RadarManager::Instance()->GetRadarWorkPath();
	CString pRadarTrackPointsPath = pRadarWorkPath + _T("\\TrackPoints");
	if(!PathFileExists(pRadarTrackPointsPath))
	{
		COM::CreateMultipleDirectory(pRadarTrackPointsPath);
	}
	m_pRadarTrackPointsPath = pRadarTrackPointsPath;
}

CDlgWorkDetection::~CDlgWorkDetection()
{
}

void CDlgWorkDetection::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_PROJECT_INFO, m_ProjectListCtrl);
	DDX_Text(pDX, IDC_Edit_interval, m_Interval);
	DDX_Text(pDX, IDC_EDIT_LineWidth, m_pLineWidth);
}


BEGIN_MESSAGE_MAP(CDlgWorkDetection, CDialog)
	ON_BN_CLICKED(IDC_Btn_ExtractTrackPoints, &CDlgWorkDetection::OnBnClickedBtnExtracttrackpoints)
	ON_BN_CLICKED(IDC_Btn_Draw, &CDlgWorkDetection::OnBnClickedBtnDraw)
	ON_EN_CHANGE(IDC_Edit_interval, &CDlgWorkDetection::OnEnChangeEditinterval)
	ON_EN_CHANGE(IDC_EDIT_LineWidth, &CDlgWorkDetection::OnEnChangeEditLinewidth)
END_MESSAGE_MAP()


// CDlgWorkDetection 消息处理程序

void CDlgWorkDetection::OnBnClickedBtnExtracttrackpoints()
{
	RadarManager* pRadarManager = RadarManager::Instance();
	if ( !RadarManager::Instance()->dbOpen() )
	{
		MessageBox(L"数据库没有打开！", L"警告");
		return ;
	}
	CDBTable::RowVector* pProjectList = pRadarManager->getProjectList();

	POSITION p = m_ProjectListCtrl.GetFirstSelectedItemPosition();
	if ( !p )
	{
		MessageBox(L"请选择要导出文件的工程", L"警告");
		return;
	}
	int index = m_ProjectListCtrl.GetNextSelectedItem( p );
	std::vector<int> projectIndexVec;
	while (index >= 0)
	{
		projectIndexVec.push_back(index);
		index = m_ProjectListCtrl.GetNextSelectedItem( p );
	}

// 	SetCursor( LoadCursor(NULL, IDC_WAIT) );	
// 	CProcessDlg* _loadProcess = new CProcessDlg; 
// 	_loadProcess->Create( CProcessDlg::IDD );

	//选择工程的个数
	int exportProjectNum = projectIndexVec.size();
	for (int exportProjectI = 0; exportProjectI < exportProjectNum;++exportProjectI)
	{
		int projectIndex = projectIndexVec[exportProjectI];
		CString pSelectProjectName = m_ProjectListCtrl.GetItemText( projectIndex, 0 );
		CString pSelectProjectTime  = m_ProjectListCtrl.GetItemText( projectIndex, 1 );
		//CString m_selectLen = _wtof ( m_ProjectListCtrl.GetItemText( projectIndex, 2 ));
		osg::ref_ptr<DBRow> pDBRow = (*pProjectList)[projectIndex];
		ProjectNameTab::ProjectNameRow *lpProjectRow = dynamic_cast<ProjectNameTab::ProjectNameRow *>(pDBRow.get());
		if ( !lpProjectRow )
		{
			MessageBox(L"变量错误", L"警告");
			return;
		}
		pSelectProjectTime.Format(L"%ld", lpProjectRow->_createTime);

		USES_CONVERSION;
		//雷达距离经纬度照片的文件路径
		std::string pDisPicfilePath = W2A(m_pRadarTrackPointsPath);
		pDisPicfilePath+="\\";
		pDisPicfilePath += W2A(pSelectProjectName);
		pDisPicfilePath+="_";
		pDisPicfilePath += W2A(pSelectProjectTime);
		pDisPicfilePath+="_Points";
		pDisPicfilePath +=".txt";
		const char* fileChar = pDisPicfilePath.c_str();
		std::ofstream infile(fileChar, std::ios::trunc);//清除已有的

		bool IsWrite = false;
		float pOldDis = 0.0;
		//输出第一个有雷达数据的通道的GPS数据
		for (int pChannelIndexI = 0 ; pChannelIndexI < 8 ; ++pChannelIndexI)
		{
			if (IsWrite)
			{
				break;
			}
			
			DataTemplateTab tab;
			tab.setProjectName( W2A(pSelectProjectTime));
			int count = 0;
			{
				DataCountTab countTab;
				countTab.setTabName( tab.getProjectName() );
				std::stringstream ss;
				ss << countTab.getSelectSql();
				ss << " where ChannelIndex=" << pChannelIndexI;
				count = countTab.getTabDataCount( RadarManager::Instance()->getDBHelp(), ss.str());
			}

// 			if ( !_loadProcess )
// 			{
// 				_loadProcess = new CProcessDlg;
// 				_loadProcess->Create( CProcessDlg::IDD );
// 			}

			std::stringstream ss;
			ss << tab.getSelectSql() << " where ChannelIndex=" << pChannelIndexI;
			ss << " order by id";

			std::string sqlStr = ss.str();
			if (tab.open( RadarManager::Instance()->getDBHelp(), sqlStr ) )
			{
// 				CProcessDlg *lpDlg = (CProcessDlg*)_loadProcess;
// 				_loadProcess->ShowWindow( SW_SHOW );
// 				lpDlg->StartProcess( count );

				//SetCursor( LoadCursor(NULL, IDC_WAIT) );

				SGYWriter writer;

				CDBTable::RowVector rv;

				int iBuf[1000];
				int lenc = 2048;
				unsigned char *data ;
				while(  ( rv = tab.read( 1000 )).size() != 0 )
				{
					for ( unsigned int i = 0; i < rv.size(); i ++ )
					{
						RadarData *lpRadarData = dynamic_cast<RadarData*>(rv[i].get());
						if ( lpRadarData )
						{
							IsWrite = true;
							writer.writeDisLL(lpRadarData, infile,pOldDis,m_Interval);	
						}
						//lpDlg->Step( 1 );
					}

					//lpDlg->Step( rv.size() );
					if ( rv.size() < 1000 )
					{
						break;
					}
				}
				//writer.closeSGY();
				//SetCursor( LoadCursor(NULL, IDC_ARROW) );

				//_loadProcess->ShowWindow( SW_HIDE );
			}
		}
	}
	AfxMessageBox(_T("提取成功！"));
}

BOOL CDlgWorkDetection::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_ProjectListCtrl.InsertColumn( 0, L"工程名称", LVCFMT_LEFT, 150);
	m_ProjectListCtrl.InsertColumn( 1, L"创建时间", LVCFMT_LEFT, 150);
	m_ProjectListCtrl.InsertColumn( 2, L"桩号", LVCFMT_LEFT, 80);
	m_ProjectListCtrl.InsertColumn( 3, L"位置", LVCFMT_LEFT, 80);
	m_ProjectListCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT);


	

	//判断工程是否把轨迹点提取了，如果提取了，则标示（不同颜色）。

	m_ProjectListCtrl.DeleteAllItems();
	USES_CONVERSION;

	CDBTable::RowVector* pProjectList = RadarManager::Instance()->getProjectList();
	for ( unsigned int i = 0; i < pProjectList->size(); i ++ )
	{
		ProjectNameTab::ProjectNameRow *lpRow = dynamic_cast<ProjectNameTab::ProjectNameRow *>(( (*pProjectList)[i]).get());
		if ( !lpRow )
		{
			continue;
		}
		m_ProjectListCtrl.InsertItem( i, A2W( lpRow->_projectName.c_str()) );
		CString str;
		struct tm *lt = localtime( &(lpRow->_createTime) );
		str.Format( L"%04d-%02d-%02d %02d:%02d:%02d", lt->tm_year + 1900, lt->tm_mon + 1, lt->tm_mday, lt->tm_hour, lt->tm_min, lt->tm_sec);
		m_ProjectListCtrl.SetItemText( i, 1, str );

		//str.Format(L"%0.3f",lpRow->_startLen );
		//m_ProjectListCtrl.SetItemText( i, 2, str );

		CString strZhunagHaoEX;
		CString  strJULI ;
		GetAccountLength(lpRow->_startLen, strZhunagHaoEX, strJULI );
		m_ProjectListCtrl.SetItemText( i, 2, strZhunagHaoEX );
		m_ProjectListCtrl.SetItemText( i, 3, strJULI );
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDlgWorkDetection::GetAccountLength( const long & startLen,CString & strZhunagHaoEX,CString & strJULI )
{
	int iZhuangHao = startLen / 1000;
	int _fJULI = startLen - (float)iZhuangHao * 1000.0;
	if (_fJULI < 0 || _fJULI > 1000)
	{
		_fJULI = 0;
	}

	if (iZhuangHao > 9999 || iZhuangHao <= 0)
	{
		strZhunagHaoEX = L"K0000";
	}
	else if (iZhuangHao < 10 )
	{
		strZhunagHaoEX.Format(L"K000%d",iZhuangHao);
	}
	else if (iZhuangHao < 100 )
	{
		strZhunagHaoEX.Format(L"K00%d",iZhuangHao);
	}
	else if (iZhuangHao < 1000 )
	{
		strZhunagHaoEX.Format(L"K0%d",iZhuangHao);
	}
	else if (iZhuangHao < 10000 )
	{
		strZhunagHaoEX.Format(L"K%d",iZhuangHao);
	}
	strJULI.Format(L"%d",_fJULI);
}

void CDlgWorkDetection::OnBnClickedBtnDraw()
{
	// TODO: 在此添加控件通知处理程序代码
	RadarManager* pRadarManager = RadarManager::Instance();
	CDBTable::RowVector* pProjectList = pRadarManager->getProjectList();

	POSITION p = m_ProjectListCtrl.GetFirstSelectedItemPosition();
	if ( !p )
	{
		MessageBox(L"请选择要导出文件的工程", L"警告");
		return;
	}
	int index = m_ProjectListCtrl.GetNextSelectedItem( p );
	std::vector<int> projectIndexVec;
	while (index >= 0)
	{
		projectIndexVec.push_back(index);
		index = m_ProjectListCtrl.GetNextSelectedItem( p );
	}

	//选择工程的个数
	int exportProjectNum = projectIndexVec.size();
	for (int exportProjectI = 0; exportProjectI < exportProjectNum;++exportProjectI)
	{
		//根据选择的工程名字，寻找所得的轨迹点文件
		int projectIndex = projectIndexVec[exportProjectI];
		CString pSelectProjectName = m_ProjectListCtrl.GetItemText( projectIndex, 0 );
		CString pSelectProjectTime  = m_ProjectListCtrl.GetItemText( projectIndex, 1 );

		osg::ref_ptr<DBRow> pDBRow = (*pProjectList)[projectIndex];
		ProjectNameTab::ProjectNameRow *lpProjectRow = dynamic_cast<ProjectNameTab::ProjectNameRow *>(pDBRow.get());
		if ( !lpProjectRow )
		{
			MessageBox(L"变量错误", L"警告");
			return;
		}
		pSelectProjectTime.Format(L"%ld", lpProjectRow->_createTime);

		USES_CONVERSION;
		//雷达距离经纬度照片的文件路径
		std::string pDisPicfilePath = W2A(m_pRadarTrackPointsPath);
		pDisPicfilePath+="\\";
		pDisPicfilePath += W2A(pSelectProjectName);
		pDisPicfilePath+="_";
		pDisPicfilePath += W2A(pSelectProjectTime);
		pDisPicfilePath+="_Points";
		pDisPicfilePath +=".txt";
		//判断文件是否存在
		if(!PathFileExists(A2W(pDisPicfilePath.c_str())))
		{
			CString tipStr = A2W(pDisPicfilePath.c_str());
			tipStr += _T("不存在！,请先生成轨迹文件");
			AfxMessageBox(tipStr);
			return;
		}
		//读取文件
		std::vector<osg::Vec3d> pTrackPoints;
		ifstream infile;
		infile.open(pDisPicfilePath.data());

		string pText;
		while (getline(infile, pText))
		{
			std::vector<std::string> disLL = COM::split(pText," ");
			osg::Vec3d onePoint;
			onePoint.x() = atof(disLL[1].c_str());
			onePoint.y() = atof(disLL[2].c_str());
			onePoint.z() = atof(disLL[3].c_str());
			pTrackPoints.push_back(onePoint);
		}
		//没有Gps数据
		if (pTrackPoints.size()==0)
		{
			AfxMessageBox(_T("没有GPS数据！"));
			return;
		}

		osg::Vec4f pColor = osg::Vec4f(1.0,0.0,0.0,1.0);
		MeasureLine* pMeasureLine = new MeasureLine(pTrackPoints,m_pLineWidth,pColor);
		//绘制，添加到场景
		bool status = Kernel::GetpKC()->GetTempGroupRoot()->addChild(pMeasureLine);
		EMObj::CmdObjLocate(-1, EMObj::CObj::LocateStyle_Normal);
	}
}
void CDlgWorkDetection::OnEnChangeEditinterval()
{
	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
	// 发送该通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。
	UpdateData(TRUE);

	// TODO:  在此添加控件通知处理程序代码
}

void CDlgWorkDetection::OnEnChangeEditLinewidth()
{
	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
	// 发送该通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。
	UpdateData(TRUE);
	// TODO:  在此添加控件通知处理程序代码
}
