// DialogRadarParam.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DialogRoadJanCe.h"


#include "resource.h"
#include "DialogProject_Export.h"
//#include "ProjectNameTab.h"
#include "RadarCurve\RadarManager.h"
#include "ProjectNameTab.h"
#include "DataTemplateTab.h"
#include "DataCountTab.h"
#include <sstream>
#include <Shlwapi.h>
// DialogRoadJanCe �Ի���

IMPLEMENT_DYNAMIC(DialogRoadJanCe, CDialog)

DialogRoadJanCe::DialogRoadJanCe(CWnd* pParent /*=NULL*/)
	: CDialog(DialogRoadJanCe::IDD, pParent)
{
	m_fAsColor[0] = 255;
	m_fAsColor[1] = 0;
	m_fAsColor[2] = 0;
	m_fAsColor[3] = 255;

	m_stuDrawInfo = new DrawInfo;
}

DialogRoadJanCe::~DialogRoadJanCe()
{
	if (NULL != m_stuDrawInfo)
	{
		delete m_stuDrawInfo;
	}
}

void DialogRoadJanCe::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_LEFTJL, m_editLeftLength);
	DDX_Control(pDX, IDC_EDIT_RIGHTJL, m_editRightLength);
	DDX_Control(pDX, IDC_EDIT_DONGXIPY, m_editDongXiPY);
	DDX_Control(pDX, IDC_EDIT_NANBEIPY, m_editNanBeiPY);
	DDX_Control(pDX, IDC_EDIT_TOUMINGDU, m_editToumindu);
	DDX_Control(pDX, IDC_LIST_PROJECT_INFO, m_projectList);
	DDX_Control(pDX, IDC_BTN_STUP_COLOR, m_btnColor);
}


BEGIN_MESSAGE_MAP(DialogRoadJanCe, CDialog)
	//ON_BN_CLICKED(IDC_BUTTON_COLOR, &DialogRoadJanCe::OnBnClickedButtonColor)
	ON_BN_CLICKED(IDC_BUTTON_SELECT_PROJECT_JC, &DialogRoadJanCe::OnBnClickedButtonSelectProjectJc)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR_ALLPROJECT, &DialogRoadJanCe::OnBnClickedButtonClearAllproject)
	ON_BN_CLICKED(IDC_BUTTON_DROW_ROUD, &DialogRoadJanCe::OnBnClickedButtonDrowRoud)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR_SELECT, &DialogRoadJanCe::OnBnClickedButtonClearSelect)
	ON_BN_CLICKED(IDC_BTN_STUP_COLOR, &DialogRoadJanCe::OnBnClickedBtnStupColor)
	ON_WM_DRAWITEM()
END_MESSAGE_MAP()


// DialogRoadJanCe ��Ϣ�������

BOOL DialogRoadJanCe::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	
	m_editLeftLength.SetWindowText(L"1.5");
	m_editRightLength.SetWindowText(L"1.5");
	m_editDongXiPY.SetWindowText(L"0.0");
	m_editNanBeiPY.SetWindowText(L"0.0");
	m_editToumindu.SetWindowText(L"255");
	m_projectList.InsertColumn( 0, L"��������", LVCFMT_LEFT, 180);
	m_projectList.InsertColumn( 1, L"����ʱ��", LVCFMT_LEFT, 150);
	//m_projectList.InsertColumn( 2, L"��ʼλ��", LVCFMT_LEFT, 150);
	m_projectList.InsertColumn( 2, L"׮��", LVCFMT_LEFT, 80);
	m_projectList.InsertColumn( 3, L"λ��", LVCFMT_LEFT, 80);
	m_projectList.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	ReInit();


	//m_btnColor.SetBkColor();

	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}
//
//void DialogRoadJanCe::OnBnClickedButtonColor()
//{
//	// TODO: �ڴ���ӿؼ�֪ͨ����������
//
//	CColorDialog dlg;
//	dlg.DoModal();
//	COLORREF thisColor=dlg.GetColor();
//	int red = GetRValue(thisColor);
//	int green = GetGValue(thisColor);
//	int blue = GetBValue(thisColor);
//	m_fAsColor[0] = red;
//	m_fAsColor[1] = green;
//	m_fAsColor[2] = blue;
//	m_fAsColor[3] = 255;
//
//	//m_btnColor.SetFaceColor(thisColor);
//
//	//if(_markHandler.valid())	
//	//	_markHandler->setColor(_color);
//}

void DialogRoadJanCe::OnBnClickedButtonSelectProjectJc()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
}

void DialogRoadJanCe::OnBnClickedButtonClearAllproject()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//RadarManager::Instance()->ClearAllRoud();
}

void DialogRoadJanCe::UpdaPeiZhi()
{

	USES_CONVERSION;
	CString strTemp;
	m_editLeftLength.GetWindowText(strTemp);
	m_stuDrawInfo->m_fLeftLength = atof(W2A(strTemp.GetBuffer()));
	m_editRightLength.GetWindowText(strTemp);
	m_stuDrawInfo->m_fRightLength = atof(W2A(strTemp.GetBuffer()));
	m_editDongXiPY.GetWindowText(strTemp);
	m_stuDrawInfo->m_fDongXiPY = atof(W2A(strTemp.GetBuffer()));
	m_editNanBeiPY.GetWindowText(strTemp);
	m_stuDrawInfo->m_fNanBeiPY = atof(W2A(strTemp.GetBuffer()));
	m_editToumindu.GetWindowText(strTemp);
	m_stuDrawInfo->m_fAsColor[3] = atof(W2A(strTemp.GetBuffer()))/255.0;
	m_stuDrawInfo->m_fAsColor[0] = m_fAsColor[0]/255.0;
	m_stuDrawInfo->m_fAsColor[1] = m_fAsColor[1]/255.0;
	m_stuDrawInfo->m_fAsColor[2] = m_fAsColor[2]/255.0;


	if ( m_projectList.GetItemCount() == 0 )
	{
		MessageBox(L"�����б�Ϊ��", L"����");
		return;
	}
	POSITION p = m_projectList.GetFirstSelectedItemPosition();
	if ( !p )
	{
		MessageBox(L"��ѡ�񹤳�", L"����");
		return;
	}
	int index = m_projectList.GetNextSelectedItem(p);
	{
		m_stuDrawInfo->m_vProCreateTime.clear();
		m_stuDrawInfo->m_vProAllName.clear();
		m_stuDrawInfo->m_vChannelIndex.clear();
		std::vector<int> _vec;
		while (index >= 0)
		{
			_vec.push_back(index);
			index = m_projectList.GetNextSelectedItem( p );
		}
		CString strProName,strProTime;
		CDBTable::RowVector* pProjectList = RadarManager::Instance()->getProjectList();
		for (int i = 0 ; i < _vec.size();++i)
		{
			int index = _vec[i];
			strProName = m_projectList.GetItemText( index, 0 );
			ProjectNameTab::ProjectNameRow *lpProjectRow = dynamic_cast<ProjectNameTab::ProjectNameRow *>(( (*pProjectList)[i]).get());
			if ( !lpProjectRow )
			{
				MessageBox(L"��������", L"����");
				continue;;
			}
			strProTime.Format(L"%ld", lpProjectRow->_createTime);

			int count = 0;
			int iClenIndex = 0;
			for (int i = 0 ; i < 8 ; ++i)
			{
				int _channelIndex = i;
				if ( !RadarManager::Instance()->dbOpen() )
				{
					return ;
				}
				DataTemplateTab tab;
				tab.setProjectName( W2A(strProTime) );
				
				{
					DataCountTab countTab;
					countTab.setTabName( tab.getProjectName() );
					std::stringstream ss1;
					ss1 << countTab.getSelectSql();
					ss1 << " where ChannelIndex=" << _channelIndex;
					count = countTab.getTabDataCount( RadarManager::Instance()->getDBHelp(), ss1.str());
				}
				if (count >0)
				{
					iClenIndex = i;
					break;
				}
			}

			if (0 >= count)
			{
				continue;
			}

			m_stuDrawInfo->m_vProCreateTime.push_back(strProTime);
			m_stuDrawInfo->m_vProAllName.push_back(strProName);
			m_stuDrawInfo->m_vChannelIndex.push_back(iClenIndex);
		}
	}
}

void DialogRoadJanCe::OnBnClickedButtonDrowRoud()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdaPeiZhi();
	if (0 >= m_stuDrawInfo->m_vProCreateTime.size())
	{
		return;
	}
	//RadarManager::Instance()->DrowRoud(m_stuDrawInfo);
}

void DialogRoadJanCe::ReInit()
{
	m_projectList.DeleteAllItems();
	USES_CONVERSION;

	CDBTable::RowVector* pProjectList = RadarManager::Instance()->getProjectList();
	for ( unsigned int i = 0; i < pProjectList->size(); i ++ )
	{
		ProjectNameTab::ProjectNameRow *lpRow = dynamic_cast<ProjectNameTab::ProjectNameRow *>(( (*pProjectList)[i]).get());
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

void DialogRoadJanCe::GetZhuanghaoLength( const long & startLen,CString & strZhunagHaoEX,CString & strJULI )
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



void DialogRoadJanCe::OnBnClickedButtonClearSelect()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdaPeiZhi();
	if (0 >= m_stuDrawInfo->m_vProCreateTime.size())
	{
		return;
	}
	//RadarManager::Instance()->ClearSelectRoud(m_stuDrawInfo);
}

void DialogRoadJanCe::OnBnClickedBtnStupColor()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CColorDialog dlg;
	if (IDOK != dlg.DoModal())
	{
		return;
	}
	COLORREF thisColor=dlg.GetColor();
	int red = GetRValue(thisColor);
	int green = GetGValue(thisColor);
	int blue = GetBValue(thisColor);
	m_fAsColor[0] = red;
	m_fAsColor[1] = green;
	m_fAsColor[2] = blue;
	m_fAsColor[3] = 255;

	//Sleep(100);
	//m_btnColor.UpdateData(TRUE);
	//UpdateWindow(m_btnColor.m_hWnd);    
	m_btnColor.ShowWindow(SW_HIDE);
	m_btnColor.UpdateWindow();
	m_btnColor.ShowWindow(SW_SHOW);
}

void DialogRoadJanCe::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	if (IDC_BTN_STUP_COLOR != nIDCtl)
	{
		return;
	}
	CDC dc;
	dc.Attach(lpDrawItemStruct->hDC);
	ASSERT(ODT_BUTTON == lpDrawItemStruct->CtlType);
	CString strText;
	m_btnColor.GetWindowText(strText);
	SetBkMode(lpDrawItemStruct->hDC,TRANSPARENT);
	CBrush brush(RGB(m_fAsColor[0],m_fAsColor[1],m_fAsColor[2]));
	
	dc.FillRect(&(lpDrawItemStruct->rcItem),&brush);
	//DrawText(lpDrawItemStruct->hDC,strText,strText.GetLength(),
	//	&lpDrawItemStruct->rcItem,DT_SINGLELINE | DT_VCENTER | DT_CENTER);
	//SetBkMode(lpDrawItemStruct->hDC,TRANSPARENT);
	dc.Detach();
	//CDialog::OnDrawItem(nIDCtl, lpDrawItemStruct);
}
