// DlgWorkTypeConfiguration.cpp : 实现文件
//

#include "stdafx.h"
#include "RadarCurve.h"
#include "DlgWorkTypeConfiguration.h"
#include "RadarCurve/ConfigureSet.h"


//C++
#include <string>


// DlgWorkTypeConfiguration 对话框

IMPLEMENT_DYNAMIC(DlgWorkTypeConfiguration, CDialog)

DlgWorkTypeConfiguration::DlgWorkTypeConfiguration(CWnd* pParent /*=NULL*/, CString strRadarIni)
	: CDialog(DlgWorkTypeConfiguration::IDD, pParent)
	, m_strRadarIni(strRadarIni)
	, m_dbSource(_T(""))
	, m_dbName(_T(""))
	, m_dbUser(_T(""))
	, m_dbPwd(_T(""))
	, m_EditSaveFile(_T(""))
	, m_EditPicSaveFile(_T(""))
{

}

DlgWorkTypeConfiguration::~DlgWorkTypeConfiguration()
{
}

void DlgWorkTypeConfiguration::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ComBox_DeviceLinkType, m_ComBoxDeviceLinkType);
	DDX_Control(pDX, IDC_ComBox_SaveType, m_ComBoxSaveType);
	DDX_Text(pDX, IDC_EDIT_DB_SOURCE, m_dbSource);
	DDX_Text(pDX, IDC_EDIT_DB_NAME, m_dbName);
	DDX_Text(pDX, IDC_EDIT_Uesr_Name, m_dbUser);
	DDX_Text(pDX, IDC_EDIT_PWD, m_dbPwd);
	DDX_Text(pDX, IDC_EditSaveFile, m_EditSaveFile);
	DDX_Text(pDX, IDC_EditPicSaveFile, m_EditPicSaveFile);
	DDX_Control(pDX, IDC_ComBox_USBTYpe, m_ComBox_USBTYpe);
	
	DDX_Control(pDX, IDC_CHECK_CHANNEL1, m_CheckBox_Channel1);
	DDX_Control(pDX, IDC_CHECK_CHANNEL2, m_CheckBox_Channel2);
	DDX_Control(pDX, IDC_CHECK_CHANNEL3, m_CheckBox_Channel3);
	DDX_Control(pDX, IDC_CHECK_CHANNEL4, m_CheckBox_Channel4);
	DDX_Control(pDX, IDC_CHECK_CHANNEL5, m_CheckBox_Channel5);
	DDX_Control(pDX, IDC_CHECK_CHANNEL6, m_CheckBox_Channel6);
	DDX_Control(pDX, IDC_CHECK_CHANNEL7, m_CheckBox_Channel7);
	DDX_Control(pDX, IDC_CHECK_CHANNEL8, m_CheckBox_Channel8);
	
}


BEGIN_MESSAGE_MAP(DlgWorkTypeConfiguration, CDialog)
	ON_BN_CLICKED(IDC_BtnOk, &DlgWorkTypeConfiguration::OnBnClickedBtnok)
	ON_BN_CLICKED(IDC_BtnCancel, &DlgWorkTypeConfiguration::OnBnClickedBtncancel)
	ON_CBN_SELENDOK(IDC_ComBox_SaveType, &DlgWorkTypeConfiguration::OnCbnSelendokComboxSaveType)
	ON_BN_CLICKED(IDC_Btn_SelectPath, &DlgWorkTypeConfiguration::OnBnClickedBtnSelectpath)
	ON_BN_CLICKED(IDC_Btn_PicSaveFile, &DlgWorkTypeConfiguration::OnBnClickedBtnPicsavefile)
	ON_CBN_SELENDOK(IDC_ComBox_DeviceLinkType, &DlgWorkTypeConfiguration::OnCbnSelendokComboxDevicelinktype)
	ON_CBN_SELENDOK(IDC_ComBox_USBTYpe, &DlgWorkTypeConfiguration::OnCbnSelendokComboxUsbtype)
END_MESSAGE_MAP()


// DlgWorkTypeConfiguration 消息处理程序

void DlgWorkTypeConfiguration::OnBnClickedBtnok()
{
	// TODO: 在此添加控件通知处理程序代码
	//保存参数，并更新内存（manager）中数据
	UpdateData( TRUE );

	int pos = m_ComBoxSaveType.GetCurSel();
	CString SaveType;
	m_ComBoxSaveType.GetLBText(pos, SaveType);
	if (SaveType.Compare(_T("数据库"))==0)
	{
		RadarManager::Instance()->m_SaveType = RadarManager::SaveType::Oracle;
	}
	else if (SaveType.Compare(_T("文件"))==0)
	{
		RadarManager::Instance()->m_SaveType = RadarManager::SaveType::File;
	}
	RadarManager::Instance()->m_WorkType = m_WorkType;
	RadarManager::Instance()->m_USBType = m_UsbType;

	RadarManager::Instance()->int_Check_Channel[0] = m_CheckBox_Channel1.GetCheck();
	RadarManager::Instance()->int_Check_Channel[1] = m_CheckBox_Channel2.GetCheck();
	RadarManager::Instance()->int_Check_Channel[2] = m_CheckBox_Channel3.GetCheck();
	RadarManager::Instance()->int_Check_Channel[3] = m_CheckBox_Channel4.GetCheck();
	RadarManager::Instance()->int_Check_Channel[4] = m_CheckBox_Channel5.GetCheck();
	RadarManager::Instance()->int_Check_Channel[5] = m_CheckBox_Channel6.GetCheck();
	RadarManager::Instance()->int_Check_Channel[6] = m_CheckBox_Channel7.GetCheck();
	RadarManager::Instance()->int_Check_Channel[7] = m_CheckBox_Channel8.GetCheck();



 	RadarManager::Instance()->setRadarDataSavePath(m_EditSaveFile);
 	RadarManager::Instance()->setPicSavePath(m_EditPicSaveFile);

	//修改config文件
	USES_CONVERSION;
	RadarManager::Instance()->getConfigureSet()->set("db", "dbsource", W2A( m_dbSource ) );
	RadarManager::Instance()->getConfigureSet()->set("db", "dbname", W2A( m_dbName ) );
	RadarManager::Instance()->getConfigureSet()->set("db", "user", W2A( m_dbUser ) );
	RadarManager::Instance()->getConfigureSet()->set("db", "pwd", W2A( m_dbPwd ) );
	RadarManager::Instance()->WriteRadarIni();
	

	RadarManager::Instance()->clear();
	if (RadarManager::Instance()->m_SaveType == RadarManager::SaveType::Oracle)
	{
		//重新连接数据库、重新加载工程信息。
		ConfigureSet* pConfigureSet = RadarManager::Instance()->getConfigureSet();
		if ( pConfigureSet->get("db", "use" ) == "true")
		{
			if ( !(RadarManager::Instance()->getDBHelp()->Open( pConfigureSet->get("db","dbsource").c_str()
				, pConfigureSet->get("db", "dbname").c_str()
				, pConfigureSet->get("db", "user").c_str()
				, pConfigureSet->get("db", "pwd").c_str()))
				)
			{
				//卸载
				::AfxMessageBox(  L"连接数据库失败");
			}
			else
			{
				RadarManager::Instance()->setdbOpen(true);
				RadarManager::Instance()->loadAllProject();
			}
		}
	}
	

	OnOK();
}

void DlgWorkTypeConfiguration::OnBnClickedBtncancel()
{
	// TODO: 在此添加控件通知处理程序代码
	OnCancel();
}

BOOL DlgWorkTypeConfiguration::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化

	//ini配置文件
	m_ComBoxDeviceLinkType.InsertString(0, _T("网口"));
	m_ComBoxDeviceLinkType.InsertString(1, _T("USB"));//xining
	

	m_ComBox_USBTYpe.InsertString(0,_T("1通道"));
	m_ComBox_USBTYpe.InsertString(1,_T("2通道"));
	m_ComBox_USBTYpe.InsertString(2,_T("4通道"));
	m_ComBox_USBTYpe.InsertString(3,_T("旧版2通道"));


	m_ComBoxSaveType.InsertString(0,_T("文件"));
	//m_ComBoxSaveType.InsertString(1,_T("数据库"));
	{
		//工作模式
		if (RadarManager::Instance()->m_WorkType == RadarManager::WorkType::USB)
		{
			m_ComBoxDeviceLinkType.SetCurSel(1);
			m_WorkType = RadarManager::USB;
			
			if (RadarManager::Instance()->m_USBType== RadarManager::USBType::OneChannel)
			{
				m_ComBox_USBTYpe.SetCurSel(0);
				m_UsbType = RadarManager::OneChannel;
			}
			else if (RadarManager::Instance()->m_USBType== RadarManager::USBType::TwoChannel)
			{
				m_ComBox_USBTYpe.SetCurSel(1);
				m_UsbType = RadarManager::TwoChannel;
			}
			else if (RadarManager::Instance()->m_USBType== RadarManager::USBType::FourChannel)
			{
				m_ComBox_USBTYpe.SetCurSel(2);
				m_UsbType = RadarManager::FourChannel;
			}
			else if (RadarManager::Instance()->m_USBType== RadarManager::USBType::TwoChannelOld)
			{
				m_ComBox_USBTYpe.SetCurSel(3);
				m_UsbType = RadarManager::TwoChannelOld;
			}
		} 
		else if (RadarManager::Instance()->m_WorkType == RadarManager::WorkType::NETCable)
		{
			m_ComBoxDeviceLinkType.SetCurSel(0);
			m_ComBox_USBTYpe.SetCurSel(0);
			m_WorkType = RadarManager::NETCable;
			(CWnd*)GetDlgItem(IDC_ComBox_USBTYpe)->EnableWindow(FALSE);
		}

		//10.31
		m_CheckBox_Channel1.SetCheck(RadarManager::Instance()->int_Check_Channel[0]);
		m_CheckBox_Channel2.SetCheck(RadarManager::Instance()->int_Check_Channel[1]);
		m_CheckBox_Channel3.SetCheck(RadarManager::Instance()->int_Check_Channel[2]);
		m_CheckBox_Channel4.SetCheck(RadarManager::Instance()->int_Check_Channel[3]);
		m_CheckBox_Channel5.SetCheck(RadarManager::Instance()->int_Check_Channel[4]);
		m_CheckBox_Channel6.SetCheck(RadarManager::Instance()->int_Check_Channel[5]);
		m_CheckBox_Channel7.SetCheck(RadarManager::Instance()->int_Check_Channel[6]);
		m_CheckBox_Channel8.SetCheck(RadarManager::Instance()->int_Check_Channel[7]);


		//雷达数据保存类型
		if (RadarManager::Instance()->m_SaveType == RadarManager::SaveType::Oracle)
		{
			m_ComBoxSaveType.SetCurSel(1);
		}
		else if(RadarManager::Instance()->m_SaveType == RadarManager::SaveType::File)
		{
			m_ComBoxSaveType.SetCurSel(0);
		}

		UpdateData(TRUE);
	}

	//默认的存储路径
	m_EditSaveFile = RadarManager::Instance()->getRadarDataSavePath();
	GetDlgItem(IDC_EditSaveFile)->SetWindowText(m_EditSaveFile);
	ConfigureSet* pConfigureSet = RadarManager::Instance()->getConfigureSet();

	USES_CONVERSION;
	m_dbSource = A2W( pConfigureSet->get("db", "dbsource").c_str());
	m_dbName = A2W( pConfigureSet->get("db", "dbname").c_str());
	m_dbUser = A2W( pConfigureSet->get("db", "user").c_str());
	m_dbPwd = A2W( pConfigureSet->get("db", "pwd").c_str());
	OnCbnSelendokComboxSaveType();

	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void DlgWorkTypeConfiguration::OnCbnSelendokComboxSaveType()
{
	// TODO: 在此添加控件通知处理程序代码
	int pos = m_ComBoxSaveType.GetCurSel();
	CString SaveType;
	m_ComBoxSaveType.GetLBText(pos, SaveType);

	if (SaveType.Compare(_T("数据库"))==0)
	{
		(CWnd*)GetDlgItem(IDC_Static_FileSavePath)->EnableWindow(FALSE);
 		(CWnd*)GetDlgItem(IDC_EditSaveFile)->EnableWindow(FALSE);
 		(CWnd*)GetDlgItem(IDC_Btn_SelectPath)->EnableWindow(FALSE);

		(CWnd*)GetDlgItem(IDC_STATIC_DBSource)->EnableWindow(TRUE);
		(CWnd*)GetDlgItem(IDC_EDIT_DB_SOURCE)->EnableWindow(TRUE);
		(CWnd*)GetDlgItem(IDC_STATIC_DB)->EnableWindow(TRUE);
		(CWnd*)GetDlgItem(IDC_EDIT_DB_NAME)->EnableWindow(TRUE);

		(CWnd*)GetDlgItem(IDC_STATIC_UseName)->EnableWindow(TRUE);
		(CWnd*)GetDlgItem(IDC_EDIT_Uesr_Name)->EnableWindow(TRUE);
		(CWnd*)GetDlgItem(IDC_STATIC_pwd)->EnableWindow(TRUE);
		(CWnd*)GetDlgItem(IDC_EDIT_PWD)->EnableWindow(TRUE);
	}
	else if (SaveType.Compare(_T("文件"))==0)
	{
		(CWnd*)GetDlgItem(IDC_Static_FileSavePath)->EnableWindow(TRUE);
		(CWnd*)GetDlgItem(IDC_EditSaveFile)->EnableWindow(TRUE);
		(CWnd*)GetDlgItem(IDC_Btn_SelectPath)->EnableWindow(TRUE);

		(CWnd*)GetDlgItem(IDC_STATIC_DBSource)->EnableWindow(FALSE);
		(CWnd*)GetDlgItem(IDC_EDIT_DB_SOURCE)->EnableWindow(FALSE);
		(CWnd*)GetDlgItem(IDC_STATIC_DB)->EnableWindow(FALSE);
		(CWnd*)GetDlgItem(IDC_EDIT_DB_NAME)->EnableWindow(FALSE);

		(CWnd*)GetDlgItem(IDC_STATIC_UseName)->EnableWindow(FALSE);
		(CWnd*)GetDlgItem(IDC_EDIT_Uesr_Name)->EnableWindow(FALSE);
		(CWnd*)GetDlgItem(IDC_STATIC_pwd)->EnableWindow(FALSE);
		(CWnd*)GetDlgItem(IDC_EDIT_PWD)->EnableWindow(FALSE);
	}
}

void DlgWorkTypeConfiguration::OnBnClickedBtnSelectpath()
{
	// TODO: 在此添加控件通知处理程序代码
	//目录选择对话框
	BROWSEINFO bi;
	TCHAR szPathName[MAX_PATH];
	TCHAR szTitle[] = _T("选择路径");
	ZeroMemory(&bi, sizeof(BROWSEINFO));
	bi.hwndOwner = GetSafeHwnd();
	bi.pszDisplayName = szPathName;
	bi.lpszTitle = szTitle;
	bi.ulFlags = 0x0040 ; 

	LPITEMIDLIST idl = SHBrowseForFolder(&bi);
	if(idl == NULL)
	{
		return;
	}
	SHGetPathFromIDList(idl, m_EditSaveFile.GetBuffer(MAX_PATH * 2));
	m_EditSaveFile.ReleaseBuffer();
	m_EditSaveFile+=_T("\\");

	GetDlgItem(IDC_EditSaveFile)->SetWindowText(m_EditSaveFile);
}

void DlgWorkTypeConfiguration::OnBnClickedBtnPicsavefile()
{
	// TODO: 在此添加控件通知处理程序代码
	//目录选择对话框
	BROWSEINFO bi;
	TCHAR szPathName[MAX_PATH];
	TCHAR szTitle[] = _T("选择路径");
	ZeroMemory(&bi, sizeof(BROWSEINFO));
	bi.hwndOwner = GetSafeHwnd();
	bi.pszDisplayName = szPathName;
	bi.lpszTitle = szTitle;
	bi.ulFlags = 0x0040 ; 

	LPITEMIDLIST idl = SHBrowseForFolder(&bi);
	if(idl == NULL)
	{
		return;
	}
	SHGetPathFromIDList(idl, m_EditPicSaveFile.GetBuffer(MAX_PATH * 2));
	m_EditPicSaveFile.ReleaseBuffer();

	GetDlgItem(IDC_EditPicSaveFile)->SetWindowText(m_EditPicSaveFile);
}

void DlgWorkTypeConfiguration::OnCbnSelendokComboxDevicelinktype()
{
	// TODO: 在此添加控件通知处理程序代码
	int pos = m_ComBoxDeviceLinkType.GetCurSel();
	CString SaveType;
	m_ComBoxDeviceLinkType.GetLBText(pos, SaveType);

	if (SaveType.Compare(_T("USB"))==0)
	{
		(CWnd*)GetDlgItem(IDC_ComBox_USBTYpe)->EnableWindow(TRUE);
		m_WorkType = RadarManager::WorkType::USB;
		OnCbnSelendokComboxUsbtype();
	}
	else
	{
		(CWnd*)GetDlgItem(IDC_ComBox_USBTYpe)->EnableWindow(FALSE);
		m_WorkType = RadarManager::WorkType::NETCable;
	}
}

void DlgWorkTypeConfiguration::OnCbnSelendokComboxUsbtype()
{
	// TODO: 在此添加控件通知处理程序代码
	int pos = m_ComBox_USBTYpe.GetCurSel();
	CString SaveType;
	if (-1 != pos)
	{
		m_ComBox_USBTYpe.GetLBText(pos, SaveType);
	}

	if (SaveType.Compare(_T("1通道"))==0)
	{
		m_UsbType = RadarManager::USBType::OneChannel;
	}
	else if (SaveType.Compare(_T("2通道"))==0)
	{
		m_UsbType = RadarManager::USBType::TwoChannel;
	}
	else if (SaveType.Compare(_T("4通道"))==0)
	{
		m_UsbType = RadarManager::USBType::FourChannel;
	}
	else if (SaveType.Compare(_T("旧版2通道"))==0)
	{
		m_UsbType = RadarManager::USBType::TwoChannelOld;
	}
}
