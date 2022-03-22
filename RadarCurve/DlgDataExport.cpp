// DlgDataExport.cpp : ʵ���ļ�
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

// DlgDataExport �Ի���

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


// DlgDataExport ��Ϣ�������

void DlgDataExport::OnBnClickedBtnexportgps()
{
	std::vector<int> projectIndexVec;
	if (!GetSelectProjectIndex(projectIndexVec))
	{
		return;
	}

	if (m_strEditExportPath.Compare(_T("")) == 0)
	{
		MessageBox(L"��ѡ�񵼳�·����", L"����");
		return;
	}
	SetCursor( LoadCursor(NULL, IDC_WAIT) );	
	CProcessDlg* _loadProcess = new CProcessDlg; 
	_loadProcess->Create( CProcessDlg::IDD );

	if ( !RadarManager::Instance()->dbOpen() )
	{
		return ;
	}
	//Ҫ�������̵ĸ���
	int exportProjectNum = projectIndexVec.size();
	for (int exportProjectI = 0; exportProjectI < exportProjectNum;++exportProjectI)//�����̵���
	{
		int projectIndex = projectIndexVec[exportProjectI];
		CString selectProjectName = m_ProjectListCtrl.GetItemText( projectIndex, 0 );
		CString createTime = m_ProjectListCtrl.GetItemText( projectIndex, 1 );
		float selectLen = _wtof ( m_ProjectListCtrl.GetItemText( projectIndex, 2 ) );

		osg::ref_ptr<DBRow> pDBRow = (*m_projectList)[projectIndex];
		ProjectNameTab::ProjectNameRow *lpProjectRow = dynamic_cast<ProjectNameTab::ProjectNameRow *>( pDBRow.get());
		if ( !lpProjectRow )
		{
			MessageBox(L"��������", L"����");
			return;
		}
		createTime.Format(L"%ld", lpProjectRow->_createTime);
		if (lpProjectRow->_curLen<=0.0000001)
		{
			MessageBox(L"û��GPS���ݣ�", L"����");
			continue;
		}

		USES_CONVERSION;

		//�״���뾭γ����Ƭ���ļ�·��
		std::string pDisPicfilePath = W2A(m_strEditExportPath);
		pDisPicfilePath+="\\";
		pDisPicfilePath += W2A(selectProjectName);
		pDisPicfilePath+="_DisLLPic";
		pDisPicfilePath +=".txt";
		const char* fileChar = pDisPicfilePath.c_str();
		std::ofstream infile(fileChar, std::ios::trunc);//������е�

		CString strWndText = _T("���ڵ����ļ���");
		strWndText += selectProjectName;
		CString tempStr = A2W(pDisPicfilePath.c_str());
		strWndText += tempStr;
		_loadProcess->SetWindowText(strWndText);

		string pStrselectProjectTab = W2A(createTime);

		//���ÿ��Ƶ�����״�����

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
	MessageBox(L"�����ļ�������");
}

void DlgDataExport::writeDisLLPic(std::string videoTabSerialNum, RadarData *lpRadarData,std::ofstream& infile)
{
	//��ȡ����
	infile.precision(5);
	double distance = lpRadarData->getLen();

	osg::Vec3d pos = lpRadarData->getPosition();
	//��ȡvediotaB��ʱ���Ӧ��ͼƬAB��·��
	DWORD receiveTime = lpRadarData->getTime();

	std::stringstream ss;
	ss << receiveTime;

	VideoTemplateTab pVideoTemplateTab;
	pVideoTemplateTab.setProjectName(videoTabSerialNum);

	//����receiveTimeȥ�ػ�ȡͼƬ��·��
	std::string strPicPathA,strPicPathB;
	bool status = pVideoTemplateTab.getPicPath(RadarManager::Instance()->getDBHelp(), ss.str(),strPicPathA,strPicPathB);
	if (status)
	{
		infile<<"distance��"<<setiosflags(ios::left)<<setiosflags(ios::fixed)<<setprecision(1)<<setw(5)<<setfill('0')<<distance<<"  "
			<<setiosflags(ios::left)<<setw(12)<<setfill('0')<<setprecision(12)<<pos.x()<<" "
			<<setiosflags(ios::left)<<setw(12)<<setfill('0')<<setprecision(12)<<pos.y()<<" "
			<<strPicPathA<<" "<<strPicPathB<<std::endl;
	}
	else
	{
		infile<<"distance��"<<setiosflags(ios::left)<<setiosflags(ios::fixed)<<setprecision(1)<<setw(5)<<setfill('0')<<distance<<"  "
			<<setiosflags(ios::left)<<setw(12)<<setfill('0')<<setprecision(12)<<pos.x()<<" "
			<<setiosflags(ios::left)<<setw(12)<<setfill('0')<<setprecision(12)<<pos.y()<<" "
			<<"û����Ƭ"<<std::endl;
	}
}

void DlgDataExport::writeDisLLPicNew(RadarData *lpRadarData,std::ofstream& infile)
{
	//��ȡ����
	infile.precision(5);
	double distance = lpRadarData->getLen();

	osg::Vec3d pos = lpRadarData->getPosition();
	//��ȡvediotaB��ʱ���Ӧ��ͼƬAB��·��
	DWORD receiveTime = lpRadarData->getTime();

	std::stringstream ss;
	ss << receiveTime;

	/*
	VideoTemplateTab pVideoTemplateTab;
	pVideoTemplateTab.setProjectName(videoTabSerialNum);
	*/

	//����receiveTimeȥ�ػ�ȡͼƬ��·��
	infile<<"distance��"<<setiosflags(ios::left)<<setiosflags(ios::fixed)<<setprecision(1)<<setw(5)<<setfill('0')<<distance<<"  "
		<<setiosflags(ios::left)<<setw(12)<<setfill('0')<<setprecision(12)<<pos.x()<<" "
		<<setiosflags(ios::left)<<setw(12)<<setfill('0')<<setprecision(12)<<pos.y()<<" "
		<<"û����Ƭ"<<std::endl;
}

void DlgDataExport::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	OnCancel();
}

BOOL DlgDataExport::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_ProjectListCtrl.InsertColumn( 0, L"��������", LVCFMT_LEFT, 150);
	m_ProjectListCtrl.InsertColumn( 1, L"����ʱ��", LVCFMT_LEFT, 150);
	m_ProjectListCtrl.InsertColumn( 2, L"׮��", LVCFMT_LEFT, 80);
	m_ProjectListCtrl.InsertColumn( 3, L"λ��", LVCFMT_LEFT, 80);
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
	// �쳣: OCX ����ҳӦ���� FALSE
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
//	// TODO: �ڴ���ӿؼ�֪ͨ����������
//	*pResult = 0;
//}


BOOL DlgDataExport::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���

	return CDialog::PreTranslateMessage(pMsg);
}

bool DlgDataExport::GetSelectProjectIndex(std::vector<int>& pSelectedIndex)
{
	POSITION p = m_ProjectListCtrl.GetFirstSelectedItemPosition();
	if ( !p )
	{
		MessageBox(L"��ѡ�񹤳�", L"����");
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	std::vector<int> projectIndexVec;
	if (!GetSelectProjectIndex(projectIndexVec))
	{
		return;
	}
	if (m_strEditExportPath.Compare(_T("")) == 0)
	{
		MessageBox(L"��ѡ�񵼳�·����", L"����");
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
			MessageBox(L"��������", L"����");
			return;
		}
		strProTime.Format(L"%ld", lpRow->_createTime);

		if (!PathIsDirectory(strPath) ) 
		{ 
			if (!CreateDirectory(strPath, NULL ) ) 
			{ 
				CString strMsg; 
				strMsg.Format (L"����·��\"%s\"ʧ�ܣ��Ƿ����?",strPath); 
				if (AfxMessageBox(strMsg, MB_YESNO) == IDYES) 
					return; 
			} 
		}
		CString strWndText;
		strWndText.Format(L"��%d�����̣����ڵ�����%d��:",projectIndexVec.size(),(i+1));
		strWndText += strProName;
		RadarManager::Instance()->exportProject(strPath,strProTime,true,false,strWndText);
	}
	::MessageBox(NULL,L"�������̳ɹ�!",L"��������", MB_OK); 
}

void DlgDataExport::OnBnClickedBtnfilepath()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString strDlgTitle = L"ѡ�񵼳�λ��";
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	std::vector<int> projectIndexVec;
	if (!GetSelectProjectIndex(projectIndexVec))
	{
		return;
	}

	if (m_strEditExportPath.Compare(_T("")) == 0)
	{
		MessageBox(L"��ѡ�񵼳�·����", L"����");
		return;
	}
	SetCursor( LoadCursor(NULL, IDC_WAIT) );	
	CProcessDlg* pLoadProcess = new CProcessDlg; 
	pLoadProcess->Create( CProcessDlg::IDD );

	//Ҫ�������̵ĸ���
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
			MessageBox(L"��������", L"����");
			return;
		}
		createTime.Format(L"%ld", lpProjectRow->_createTime);
		USES_CONVERSION;


		bool IsWriteLLPic = true;
		//���ÿ��Ƶ�����״�����
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
				MessageBox(L"�򿪹��̱��ʧ��");
				return ;
			}


			//sgy�ļ�·��
			CString strSgyFilePath = m_strEditExportPath;
			{
				strSgyFilePath += _T("\\");
				strSgyFilePath += selectProjectName;

				CString str;
				USES_CONVERSION;
				str.Format(L"_%s_ͨ��%d-%s.SGY"
					,createTime
					,(pChannelIndexI + 1)
					, A2W(RadarManager::Instance()->getChannelName(pChannelIndexI).c_str() ));
				strSgyFilePath += str;
				CString strWndText = _T("���ڵ����ļ���");
				strWndText += selectProjectName;
				strWndText += str;
				pLoadProcess->SetWindowText(strWndText);
				pLoadProcess->StartProcess( count );
			}

			//дSGY��ͷ
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
	MessageBox(L"�����ļ�������");
}
