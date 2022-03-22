// DlgWorkDetection.cpp : ʵ���ļ�
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

// CDlgWorkDetection �Ի���

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


// CDlgWorkDetection ��Ϣ�������

void CDlgWorkDetection::OnBnClickedBtnExtracttrackpoints()
{
	RadarManager* pRadarManager = RadarManager::Instance();
	if ( !RadarManager::Instance()->dbOpen() )
	{
		MessageBox(L"���ݿ�û�д򿪣�", L"����");
		return ;
	}
	CDBTable::RowVector* pProjectList = pRadarManager->getProjectList();

	POSITION p = m_ProjectListCtrl.GetFirstSelectedItemPosition();
	if ( !p )
	{
		MessageBox(L"��ѡ��Ҫ�����ļ��Ĺ���", L"����");
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

	//ѡ�񹤳̵ĸ���
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
			MessageBox(L"��������", L"����");
			return;
		}
		pSelectProjectTime.Format(L"%ld", lpProjectRow->_createTime);

		USES_CONVERSION;
		//�״���뾭γ����Ƭ���ļ�·��
		std::string pDisPicfilePath = W2A(m_pRadarTrackPointsPath);
		pDisPicfilePath+="\\";
		pDisPicfilePath += W2A(pSelectProjectName);
		pDisPicfilePath+="_";
		pDisPicfilePath += W2A(pSelectProjectTime);
		pDisPicfilePath+="_Points";
		pDisPicfilePath +=".txt";
		const char* fileChar = pDisPicfilePath.c_str();
		std::ofstream infile(fileChar, std::ios::trunc);//������е�

		bool IsWrite = false;
		float pOldDis = 0.0;
		//�����һ�����״����ݵ�ͨ����GPS����
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
	AfxMessageBox(_T("��ȡ�ɹ���"));
}

BOOL CDlgWorkDetection::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_ProjectListCtrl.InsertColumn( 0, L"��������", LVCFMT_LEFT, 150);
	m_ProjectListCtrl.InsertColumn( 1, L"����ʱ��", LVCFMT_LEFT, 150);
	m_ProjectListCtrl.InsertColumn( 2, L"׮��", LVCFMT_LEFT, 80);
	m_ProjectListCtrl.InsertColumn( 3, L"λ��", LVCFMT_LEFT, 80);
	m_ProjectListCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT);


	

	//�жϹ����Ƿ�ѹ켣����ȡ�ˣ������ȡ�ˣ����ʾ����ͬ��ɫ����

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
	// �쳣: OCX ����ҳӦ���� FALSE
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	RadarManager* pRadarManager = RadarManager::Instance();
	CDBTable::RowVector* pProjectList = pRadarManager->getProjectList();

	POSITION p = m_ProjectListCtrl.GetFirstSelectedItemPosition();
	if ( !p )
	{
		MessageBox(L"��ѡ��Ҫ�����ļ��Ĺ���", L"����");
		return;
	}
	int index = m_ProjectListCtrl.GetNextSelectedItem( p );
	std::vector<int> projectIndexVec;
	while (index >= 0)
	{
		projectIndexVec.push_back(index);
		index = m_ProjectListCtrl.GetNextSelectedItem( p );
	}

	//ѡ�񹤳̵ĸ���
	int exportProjectNum = projectIndexVec.size();
	for (int exportProjectI = 0; exportProjectI < exportProjectNum;++exportProjectI)
	{
		//����ѡ��Ĺ������֣�Ѱ�����õĹ켣���ļ�
		int projectIndex = projectIndexVec[exportProjectI];
		CString pSelectProjectName = m_ProjectListCtrl.GetItemText( projectIndex, 0 );
		CString pSelectProjectTime  = m_ProjectListCtrl.GetItemText( projectIndex, 1 );

		osg::ref_ptr<DBRow> pDBRow = (*pProjectList)[projectIndex];
		ProjectNameTab::ProjectNameRow *lpProjectRow = dynamic_cast<ProjectNameTab::ProjectNameRow *>(pDBRow.get());
		if ( !lpProjectRow )
		{
			MessageBox(L"��������", L"����");
			return;
		}
		pSelectProjectTime.Format(L"%ld", lpProjectRow->_createTime);

		USES_CONVERSION;
		//�״���뾭γ����Ƭ���ļ�·��
		std::string pDisPicfilePath = W2A(m_pRadarTrackPointsPath);
		pDisPicfilePath+="\\";
		pDisPicfilePath += W2A(pSelectProjectName);
		pDisPicfilePath+="_";
		pDisPicfilePath += W2A(pSelectProjectTime);
		pDisPicfilePath+="_Points";
		pDisPicfilePath +=".txt";
		//�ж��ļ��Ƿ����
		if(!PathFileExists(A2W(pDisPicfilePath.c_str())))
		{
			CString tipStr = A2W(pDisPicfilePath.c_str());
			tipStr += _T("�����ڣ�,�������ɹ켣�ļ�");
			AfxMessageBox(tipStr);
			return;
		}
		//��ȡ�ļ�
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
		//û��Gps����
		if (pTrackPoints.size()==0)
		{
			AfxMessageBox(_T("û��GPS���ݣ�"));
			return;
		}

		osg::Vec4f pColor = osg::Vec4f(1.0,0.0,0.0,1.0);
		MeasureLine* pMeasureLine = new MeasureLine(pTrackPoints,m_pLineWidth,pColor);
		//���ƣ���ӵ�����
		bool status = Kernel::GetpKC()->GetTempGroupRoot()->addChild(pMeasureLine);
		EMObj::CmdObjLocate(-1, EMObj::CObj::LocateStyle_Normal);
	}
}
void CDlgWorkDetection::OnEnChangeEditinterval()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ�������������
	// ���͸�֪ͨ��������д CDialog::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�
	UpdateData(TRUE);

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}

void CDlgWorkDetection::OnEnChangeEditLinewidth()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ�������������
	// ���͸�֪ͨ��������д CDialog::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�
	UpdateData(TRUE);
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}
