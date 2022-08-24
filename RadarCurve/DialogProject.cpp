// DialogProject.cpp : 实现文件
#include "stdafx.h"
#include "RadarCurve.h"
#include "DialogProject.h"
#include "DialogProjectName.h"
#include "ProjectNameTab.h"
#include "VideoTemplateTab.h"
#include "DataTemplateTab.h"
#include <RadarCurve/RadarManager.h>
#include <RadarCurve/DBConnectBase.h>
#include "DialogHistoryCurve.h"
#include <sstream>
#include "DataTemplateTab.h"
#include "SGYWriter.h"
#include <COM\log\com_log.h>
#include <string>   
// CDialogProject 对话框
CDialogHistoryCurve *G_HistoryCurveDialog[8] = { NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL};

IMPLEMENT_DYNAMIC(CDialogProject, CDialog)


CDialogProject::CDialogProject( CDBTable::RowVector & rv, int iType, CWnd* pParent /*=NULL*/)
	: CDialog(CDialogProject::IDD, pParent)
	, _projectList( rv )
{
	m_iType = iType;
	m_bProcessMsg = false;
	m_iSelectIndex = 0;
}

CDialogProject::~CDialogProject()
{

}

void CDialogProject::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_PROJECT, m_projectList);
}


BEGIN_MESSAGE_MAP(CDialogProject, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_NEW, &CDialogProject::OnBnClickedButtonNew)
	ON_BN_CLICKED(IDC_BUTTON_MODIFY, &CDialogProject::OnBnClickedButtonModify)
	ON_BN_CLICKED(IDC_BUTTON_DEL, &CDialogProject::OnBnClickedButtonDel)
	ON_BN_CLICKED(IDOK, &CDialogProject::OnBnClickedOk)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_PROJECT, &CDialogProject::OnNMDblclkListProject)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_PROJECT, &CDialogProject::OnNMRclickListProject)
	ON_COMMAND(ID_HIS_CHANNEL_1, &CDialogProject::OnHisChannel1)
	ON_COMMAND(ID_HIS_CHANNEL_2, &CDialogProject::OnHisChannel2)
	ON_COMMAND(ID_HIS_CHANNEL_3, &CDialogProject::OnHisChannel3)
	ON_COMMAND(ID_HIS_CHANNEL_4, &CDialogProject::OnHisChannel4)
	ON_COMMAND(ID_HIS_CHANNEL_5, &CDialogProject::OnHisChannel5)
	ON_COMMAND(ID_HIS_CHANNEL_6, &CDialogProject::OnHisChannel6)
	ON_COMMAND(ID_HIS_CHANNEL_7, &CDialogProject::OnHisChannel7)
	ON_COMMAND(ID_HIS_CHANNEL_8, &CDialogProject::OnHisChannel8)
	ON_COMMAND(ID_EXPORT_HISTORY, &CDialogProject::OnExportHistory)
	ON_COMMAND(ID_EXPORT_ALL_CHANNEL, &CDialogProject::OnExportAllChannel)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_PROJECT, &CDialogProject::OnLvnItemchangedListProject)
END_MESSAGE_MAP()


// CDialogProject 消息处理程序

void CDialogProject::GetStartLength(float & startLen, CString & strZhunagHaoEX, CString &  strJULI)
{

	//if (strJULI.GetLength() == 0)
	//{
	//	strJULI = L"0";
	//}

	if (strZhunagHaoEX.GetLength() != 5)
	{
		MessageBox(L"桩号输入格式不正确，为5个字，例如：K0002", L"警告");
		return;
	}

	TCHAR * tcP = strZhunagHaoEX.GetBuffer();

	if (*tcP != 'K')
	{
		MessageBox(L"桩号输入格式不正确，第一个文字应为\"K\"", L"警告");
		return;
	}

	int inum = 0;
	tcP++;
	if (*tcP < '0' || *tcP > '9')
	{
		MessageBox(L"桩号输入格式不正确，第二个文字应为0-9", L"警告");
		return;
	}
	inum += 1000 * (*tcP - '0');

	tcP++;
	if (*tcP < '0' || *tcP > '9')
	{
		MessageBox(L"桩号输入格式不正确，第三个文字应为0-9", L"警告");
		return;
	}
	inum += 100 * (*tcP - '0');

	tcP++;
	if (*tcP < '0' || *tcP > '9')
	{
		MessageBox(L"桩号输入格式不正确，第四个文字应为0-9", L"警告");
		return;
	}
	inum += 10 * (*tcP - '0');

	tcP++;
	if (*tcP < '0' || *tcP > '9')
	{
		MessageBox(L"桩号输入格式不正确，第五个文字应为0-9", L"警告");
		return;
	}
	inum += 1 * (*tcP - '0');

	USES_CONVERSION;
	std::string str = W2A(strJULI.GetBuffer());
	startLen = inum * 1000 + atof(str.c_str());
}

void CDialogProject::GetZhuanghaoLength(const long & startLen,CString & strZhunagHaoEX,CString &  strJULI )
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





void CDialogProject::OnBnClickedButtonNew()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogProjectName dlg(L"新建工程", L"", 0, this );
	USES_CONVERSION;
	if ( dlg.DoModal() == IDOK )
	{
		int index = m_projectList.InsertItem( m_projectList.GetItemCount(), dlg.m_projectName);
		time_t t = time(NULL);
		CString str;
		struct tm *lt = localtime( &t );
		str.Format( L"%04d-%02d-%02d %02d:%02d:%02d", lt->tm_year + 1900, lt->tm_mon + 1, lt->tm_mday, lt->tm_hour, lt->tm_min, lt->tm_sec);
		m_projectList.SetItemText( index, 1, str );


		//time_t tt = mktime(lt);

		//str.Format(L"%0.3f", dlg.m_startLen );
		//m_projectList.SetItemText(index, 2, str );

		CString strZhunagHaoEX;
		CString  strJULI ;
		GetZhuanghaoLength(dlg.m_startLen, strZhunagHaoEX, strJULI );
		m_projectList.SetItemText( index, 2, strZhunagHaoEX );
		m_projectList.SetItemText( index, 3, strJULI );


		ProjectNameTab::ProjectNameRow *lpRow = new ProjectNameTab::ProjectNameRow;
		lpRow->_projectName = W2A( dlg.m_projectName );
		lpRow->_createTime = t;
		lpRow->_startLen = dlg.m_startLen;
		lpRow->_curLen = 0.0f;
		lpRow->_paramXML = RadarManager::Instance()->getConfigureSet()->getGroupXML( "radar" );

		_projectList.push_back( lpRow );
		if ( RadarManager::Instance()->dbOpen())
		{

			ProjectNameTab tab;
			tab.insert( RadarManager::Instance()->getDBHelp(), lpRow );

			CString s;
			s.Format(L"%ld", lpRow->_createTime );
			prepareDBTab( s );
		}

		m_projectList.SetSelectionMark( index );

	}
}

void CDialogProject::OnBnClickedButtonModify()
{
	// TODO: 在此添加控件通知处理程序代码
	POSITION p = m_projectList.GetFirstSelectedItemPosition();

	if ( !p )
	{
		MessageBox(L"请选择要编辑的工作 ", L"警告");
		return;
	}
	
	int index = m_projectList.GetNextSelectedItem(p);

	CString projectName = m_projectList.GetItemText( index, 0 );

	USES_CONVERSION;
	ProjectNameTab::ProjectNameRow *lpRow = dynamic_cast<ProjectNameTab::ProjectNameRow *>( _projectList[index].get());
	if ( !lpRow )
	{
		return;
	}

	CDialogProjectName dlg(L"修改工程", projectName, lpRow->_startLen, this );
	if ( dlg.DoModal() == IDOK )
	{
		
		m_projectList.SetItemText(index, 0, dlg.m_projectName );

		//CString str;
		//str.Format(L"%0.3f", dlg.m_startLen );
		//m_projectList.SetItemText(index, 2, str );


		CString strZhunagHaoEX;
		CString  strJULI ;
		GetZhuanghaoLength(dlg.m_startLen, strZhunagHaoEX, strJULI );
		m_projectList.SetItemText( index, 2, strZhunagHaoEX );
		m_projectList.SetItemText( index, 3, strJULI );




		lpRow->_projectName = W2A( dlg.m_projectName );
		lpRow->_startLen = dlg.m_startLen;
		lpRow->_paramXML = RadarManager::Instance()->getConfigureSet()->getGroupXML( "radar" );

		if ( RadarManager::Instance()->dbOpen())
		{
			ProjectNameTab tab;
			tab.update( RadarManager::Instance()->getDBHelp(), lpRow );
		}

	}
}

void CDialogProject::OnBnClickedButtonDel()
{
	// TODO: 在此添加控件通知处理程序代码
	POSITION p = m_projectList.GetFirstSelectedItemPosition();

	if ( !p )
	{
		MessageBox(L"请选择要删除的工程", L"警告");
		return;
	}

	if ( MessageBox(L"将要删除所选工程， 此工程所有数据将同时删除！\r\n确认删除请点击\"确定\"按钮",
		L"删除确认", MB_OKCANCEL) != IDOK )
	{
		return;
	}


	int index = m_projectList.GetNextSelectedItem( p );

	std::vector<int> _vec;
	
	while (index >= 0)
	{
		_vec.push_back(index);
		index = m_projectList.GetNextSelectedItem( p );
	}

	SetCursor( LoadCursor(NULL, IDC_WAIT) );	
	for (int i = (_vec.size() - 1);i >= 0 ;i--)
	{
		index = _vec[i];

		m_projectList.DeleteItem( index );
		if ( RadarManager::Instance()->dbOpen())
		{
			ProjectNameTab::ProjectNameRow *lpRow = dynamic_cast<ProjectNameTab::ProjectNameRow *>(_projectList[index].get());
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
			tab.del( RadarManager::Instance()->getDBHelp(), _projectList[index].get() );
		}

		_projectList.erase( _projectList.begin() + index );

	}
	SetCursor( LoadCursor(NULL, IDC_ARROW) );
}

void CDialogProject::ReInit()
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


		//str.Format(L"%0.3f",lpRow->_startLen );
		//m_projectList.SetItemText( i, 2, str );

		CString strZhunagHaoEX;
		CString  strJULI ;
		GetZhuanghaoLength(lpRow->_startLen, strZhunagHaoEX, strJULI );
		m_projectList.SetItemText( i, 2, strZhunagHaoEX );
		m_projectList.SetItemText( i, 3, strJULI );
	}

}



BOOL CDialogProject::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_projectList.InsertColumn( 0, L"工程名称", LVCFMT_LEFT, 180);
	m_projectList.InsertColumn( 1, L"创建时间", LVCFMT_LEFT, 150);
	//m_projectList.InsertColumn( 2, L"起始位置", LVCFMT_LEFT, 150);
	m_projectList.InsertColumn( 2, L"桩号", LVCFMT_LEFT, 80);
	m_projectList.InsertColumn( 3, L"位置", LVCFMT_LEFT, 80);

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
		

		//str.Format(L"%0.3f",lpRow->_startLen );
		//m_projectList.SetItemText( i, 2, str );

		CString strZhunagHaoEX;
		CString  strJULI ;
		GetZhuanghaoLength(lpRow->_startLen, strZhunagHaoEX, strJULI );
		m_projectList.SetItemText( i, 2, strZhunagHaoEX );
		m_projectList.SetItemText( i, 3, strJULI );



	}

	if ( 1 != m_iType )
	{
		if ( GetDlgItem( IDC_BUTTON_NEW ) )
		{
			GetDlgItem( IDC_BUTTON_NEW )->ShowWindow( SW_HIDE );
		}
		if ( GetDlgItem( IDC_BUTTON_MODIFY ) )
		{
			GetDlgItem( IDC_BUTTON_MODIFY )->ShowWindow( SW_HIDE );
		}
		if ( GetDlgItem( IDC_BUTTON_DEL ) )
		{
			GetDlgItem( IDC_BUTTON_DEL )->ShowWindow( SW_HIDE );
		}
		if ( GetDlgItem( IDOK ) )
		{
			GetDlgItem( IDOK )->ShowWindow( SW_HIDE );
		}
	}


	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CDialogProject::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	if ( m_projectList.GetItemCount() == 0 )
	{

		return;
	}

	POSITION p = m_projectList.GetFirstSelectedItemPosition();

	if ( !p )
	{
		MessageBox(L"请选择工程", L"警告");
		return;
	}

	int index = 0;
	if ( p )
	{
		index = m_projectList.GetNextSelectedItem(p);
	}

	m_selectProjectName = m_projectList.GetItemText( index, 0 );
	//m_selectLen = _wtof( m_projectList.GetItemText( index, 2 ) );
	GetStartLength(m_selectLen, m_projectList.GetItemText( index, 2 ), m_projectList.GetItemText( index, 3 ));

	if ( index >= 0 && index < _projectList.size() )
	{
		_selectRow = _projectList[index];

		ProjectNameTab::ProjectNameRow *lpRow = dynamic_cast<ProjectNameTab::ProjectNameRow*>(_selectRow.get());
		if ( lpRow )
		{
			m_selectProjectTab.Format(L"%ld", lpRow->_createTime);
		}
	}
	OnOK();
}

void CDialogProject::OnNMDblclkListProject(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	if ( 1 == m_iType )
	{
		OnBnClickedOk();
	}

	*pResult = 0;
}
void CDialogProject::prepareDBTab( CString const& projectCreatTime )
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
void CDialogProject::OnNMRclickListProject(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码


	*pResult = 0;

	if ( 1 == m_iType )
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

void CDialogProject::showHistoryDlg( int index )
{
	if ( index >= MAX_CHANNEL )
	{
		return;
	}
	POSITION p = m_projectList.GetFirstSelectedItemPosition();
	if ( !p )
	{
		MessageBox(L"请选择工作", L"警告");
		return;
	}

	int projectIndex = 0;
	if ( p )
	{
		projectIndex = m_projectList.GetNextSelectedItem(p);
	}

	m_selectProjectName = m_projectList.GetItemText( projectIndex, 0 );
	//m_selectProjectTab = m_projectList.GetItemText( projectIndex, 1 );
	//m_selectLen = _wtof ( m_projectList.GetItemText( projectIndex, 2 ) );
	GetStartLength(m_selectLen, m_projectList.GetItemText( projectIndex, 2 ), m_projectList.GetItemText( projectIndex, 3 ));
	
	ProjectNameTab::ProjectNameRow *lpProjectRow = dynamic_cast<ProjectNameTab::ProjectNameRow *>(_projectList[projectIndex].get());
	if ( !lpProjectRow )
	{
		MessageBox(L"变量错误", L"警告");
		return;
	}

	
	m_selectProjectTab.Format(L"%ld", lpProjectRow->_createTime);
	



	if (true == m_bProcessMsg)
	{
		m_iSelectIndex = index;
		m_lpProjectRow = lpProjectRow;
		OnOK();
		return;
	}
	USES_CONVERSION;
	if ( G_HistoryCurveDialog[index] )
	{
		if ( G_HistoryCurveDialog[index]->GetSafeHwnd() )
		{
			G_HistoryCurveDialog[index]->DestroyWindow();
		}
		delete G_HistoryCurveDialog[index];
		G_HistoryCurveDialog[index] = NULL;

	}
	G_HistoryCurveDialog[index] = new CDialogHistoryCurve(W2A(m_selectProjectTab), index,lpProjectRow->_paramXML, m_selectLen);
	G_HistoryCurveDialog[index]->Create( CDialogHistoryCurve::IDD );
	G_HistoryCurveDialog[index]->ShowWindow( SW_SHOW );
	COM::WRITE_TO_LOG_PTR->WriteToLog(COM::LEVEL_COMMON,"CDialogProject::showHistoryDlg","显示历史对话框");	


}

void CDialogProject::OnHisChannel1()
{
	// TODO: 在此添加命令处理程序代码
	showHistoryDlg( 0 );

}
void CDialogProject::OnHisChannel2()
{
	// TODO: 在此添加命令处理程序代码
	showHistoryDlg( 1 );
}


void CDialogProject::OnHisChannel3()
{
	// TODO: 在此添加命令处理程序代码
	showHistoryDlg( 2 );
}
void CDialogProject::OnHisChannel4()
{
	// TODO: 在此添加命令处理程序代码
	showHistoryDlg( 3 );
}
void CDialogProject::OnHisChannel5()
{
	// TODO: 在此添加命令处理程序代码
	showHistoryDlg( 4 );
}
void CDialogProject::OnHisChannel6()
{
	// TODO: 在此添加命令处理程序代码
	showHistoryDlg( 5 );
}
void CDialogProject::OnHisChannel7()
{
	// TODO: 在此添加命令处理程序代码
	showHistoryDlg( 6 );
}
void CDialogProject::OnHisChannel8()
{
	// TODO: 在此添加命令处理程序代码
	showHistoryDlg( 7 );
}

void CDialogProject::OnExportHistory()
{
	POSITION p = m_projectList.GetFirstSelectedItemPosition();
	if ( !p )
	{
		MessageBox(L"请选择工作", L"警告");
		return;
	}

	int projectIndex = 0;
	if ( p )
	{
		projectIndex = m_projectList.GetNextSelectedItem(p);
	}

	if ( projectIndex >= _projectList.size() )
	{
		MessageBox(L"选择错误", L"警告");
		return;
	}

	USES_CONVERSION;
	DataTemplateTab tab;
	tab.setProjectName( W2A ( m_selectProjectTab ) );

	if ( !tab.open( RadarManager::Instance()->getDBHelp(), tab.getSelectSql() ) )
	{
		MessageBox(L"打开表错误", L"警告");
		return;
	}
	ProjectNameTab::ProjectNameRow *lpProjectRow = dynamic_cast<ProjectNameTab::ProjectNameRow *>(_projectList[projectIndex].get());
	if ( !lpProjectRow )
	{
		MessageBox(L"变量错误", L"警告");
		return;
	}

	CString sFilter = _T("SGY(*.SGY)|*.SGY||");
	CFileDialog dlgSave(FALSE, 0, 0, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, (LPCTSTR)sFilter);
	if(dlgSave.DoModal() != IDOK)
	{
		return ;
	}
	CString saveName = dlgSave.GetPathName();
	CString saveExt = dlgSave.GetFileExt();


	SetCursor( LoadCursor(NULL, IDC_WAIT) );
	std::string filePath = W2A( saveName );

	SGYWriter writer;

	writer.openSGY( filePath, lpProjectRow->_paramXML, 0 );


	writer.writeHead( 8 ,W2A ( m_selectProjectTab ));

	CDBTable::RowVector rv;
	while(  ( rv = tab.read( 1000 )).size() != 0 )
	{
		for ( unsigned int i = 0; i < rv.size(); i ++ )
		{
			RadarData *lpRadarData = dynamic_cast<RadarData*>(rv[i].get());
			if ( lpRadarData )
			{
				writer.writeRadarData( lpRadarData );
			}
		}
		if ( rv.size() < 1000 )
		{
			break;
		}
	}


	writer.closeSGY();
	SetCursor( LoadCursor(NULL, IDC_ARROW) );

	MessageBox(L"导出结束");
}

void CDialogProject::OnExportAllChannel()
{
	// TODO: 在此添加命令处理程序代码

}


void CDialogProject::OnLvnItemchangedListProject(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	if ( m_projectList.GetItemCount() == 0 )
	{
		return;
	}

	POSITION p = m_projectList.GetFirstSelectedItemPosition();
	int index = 0;
	if ( p )
	{
		index = m_projectList.GetNextSelectedItem(p);
	}

	m_selectProjectName = m_projectList.GetItemText( index, 0 );
	_selectRow = _projectList[index];

	ProjectNameTab::ProjectNameRow *lpRow = dynamic_cast<ProjectNameTab::ProjectNameRow*>(_selectRow.get());
	*pResult = 0;
}

void CDialogProject::initForNoProjectName()
{	
	int index = 0;
	time_t t = time(NULL);
	CString cstr;
	struct tm *lt = localtime( &t );
	cstr.Format( L"%04d%02d%02d%02d%02d%02d", lt->tm_year + 1900, lt->tm_mon + 1, lt->tm_mday, lt->tm_hour, lt->tm_min, lt->tm_sec);
	//m_projectList.SetItemText( index, 1, cstr );

	//time_t tt = mktime(lt);

	//str.Format(L"%0.3f", dlg.m_startLen );
	//m_projectList.SetItemText(index, 2, str );

	//CString strZhunagHaoEX;
	//CString  strJULI ;
	//GetZhuanghaoLength(dlg.m_startLen, strZhunagHaoEX, strJULI );
	//m_projectList.SetItemText( index, 2, strZhunagHaoEX );
	//m_projectList.SetItemText( index, 3, strJULI );


	ProjectNameTab::ProjectNameRow *lpRow = new ProjectNameTab::ProjectNameRow;
	lpRow->_projectName = "小推车";
	lpRow->_createTime = t;
	lpRow->_startLen = 0;
	lpRow->_curLen = 0.0f;
	lpRow->_paramXML = RadarManager::Instance()->getConfigureSet()->getGroupXML( "radar" );

	_projectList.push_back( lpRow );
///////////////////////////////

	//m_selectProjectName = m_projectList.GetItemText( index, 0 );
	m_selectProjectName=cstr;
	//m_selectLen = _wtof( m_projectList.GetItemText( index, 2 ) );
	//GetStartLength(m_selectLen, m_projectList.GetItemText( index, 2 ), m_projectList.GetItemText( index, 3 ));

	if ( index >= 0 && index < _projectList.size() )
	{
		_selectRow = _projectList[index];

		ProjectNameTab::ProjectNameRow *lpRow = dynamic_cast<ProjectNameTab::ProjectNameRow*>(_selectRow.get());
		if ( lpRow )
		{
			m_selectProjectTab.Format(L"%ld", lpRow->_createTime);
		}
	}
}