// DialogProjectName.cpp : 实现文件
//

#include "stdafx.h"
#include "RadarCurve.h"
#include "DialogProjectName.h"
#include "RadarCurve/RadarManager.h"
#include "RadarCurve/ConfigureSet.h"
#include <string>
#include <vector>
#include <sstream>

// CDialogProjectName 对话框


std::vector< std::string > SpliteStr2( std::string const& str, char splitChar )
{
	std::vector< std::string > result;
	std::string tmpStr = str;
	size_t findP;
	while ((findP = tmpStr.find(splitChar)) != std::string::npos)
	{
		std::string subStr = tmpStr.substr(0, findP);
		tmpStr = tmpStr.substr(findP + 1, tmpStr.length() - findP - 1);
		result.push_back( subStr );
	}
	if (tmpStr.length() > 0)
	{
		result.push_back(tmpStr);
	}
	return result;
}



IMPLEMENT_DYNAMIC(CDialogProjectName, CDialog)

CDialogProjectName::CDialogProjectName(CString const& title, CString const& msg, float startLen, CWnd* pParent /*=NULL*/)
	: CDialog(CDialogProjectName::IDD, pParent)
	, m_projectName(msg)
	, m_startLen ( startLen )
	, m_cstrStakeNumber(_T(""))
	, m_fDistance(0)
	, m_cstrRoadNumber(_T("1"))
{
	

	USES_CONVERSION;
	std::vector<std::string> result = SpliteStr2( W2A( m_projectName ), '_');
	
	int count = result.size();
	if ( count >= 1 )
	{
		m_cstrRoadName = A2W( result[0].c_str() );
	}
	if ( count >= 2 )
	{
		m_cstrRoadSection = A2W( result[1].c_str() );
	}
	if ( count >= 3 )
	{
		//m_strDaohao = A2W( result[2].c_str() );
		m_cstrRoadNumber = A2W( result[2].c_str() );
	}
	if ( count >= 4 )
	{
		m_cstrDirection = A2W ( result[3].c_str() );
	}

	//m_cstrRoadNumber = m_strDaohao;
	m_title = title;

	int iZhuangHao = startLen / 1000;
	m_fDistance = startLen - (float)iZhuangHao * 1000.0;
	if (m_fDistance < 0 || m_fDistance > 1000)
	{
		m_fDistance = 10;
	}

	if (iZhuangHao > 9999 || iZhuangHao <= 0)
	{
		m_cstrStakeNumber = L"K0000";
	}
	else if (iZhuangHao < 10 )
	{
		m_cstrStakeNumber.Format(L"K000%d",iZhuangHao);
	}
	else if (iZhuangHao < 100 )
	{
		m_cstrStakeNumber.Format(L"K00%d",iZhuangHao);
	}
	else if (iZhuangHao < 1000 )
	{
		m_cstrStakeNumber.Format(L"K0%d",iZhuangHao);
	}
	else if (iZhuangHao < 10000 )
	{
		m_cstrStakeNumber.Format(L"K%d",iZhuangHao);
	}


}

CDialogProjectName::~CDialogProjectName()
{
}

void CDialogProjectName::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_ROAD_NAME, m_cstrRoadName );
	DDX_Text(pDX, IDC_EDIT_ROAD_SECTION, m_cstrRoadSection );
	//DDX_Control(pDX, IDC_COMBO_DAO_HAO, m_daohao );
	DDX_Control(pDX, IDC_COMBO_DIRECTION, m_ccomboboxDirection );
	//DDX_Text(pDX, IDC_EDIT_START_LEN, m_startLen );
	DDX_Text(pDX, IDC_EDIT_STAKE_NUMBER, m_cstrStakeNumber);
	DDX_Text(pDX, IDC_EDIT_DISTANCE, m_fDistance);
	DDX_Text(pDX, IDC_EDIT_ROAD_NUMBER, m_cstrRoadNumber);
}


BEGIN_MESSAGE_MAP(CDialogProjectName, CDialog)
	ON_BN_CLICKED(IDOK, &CDialogProjectName::OnBnClickedOk)
END_MESSAGE_MAP()

BOOL CDialogProjectName::OnInitDialog()
{
	CDialog::OnInitDialog();
	// TODO:  在此添加额外的初始化
	SetWindowText( m_title );
	m_ccomboboxDirection.AddString(L"东向西");
	m_ccomboboxDirection.AddString(L"西向东");
	m_ccomboboxDirection.AddString(L"南向北");
	m_ccomboboxDirection.AddString(L"北向南");
	m_ccomboboxDirection.SelectString( 0, m_cstrDirection );
	if ( m_ccomboboxDirection.GetCurSel() < 0 )
	{
		m_ccomboboxDirection.SetCurSel( 0 );
	}

	USES_CONVERSION;
	/*m_cstrRoadName;
	CString m_cstrRoadSection;
	CString m_cstrRoadNumber;
	CComboBox m_ccomboboxDirection;
	CString m_cstrDirection;
	CString m_cstrStakeNumber;*/
	m_pConfigureSet = RadarManager::Instance()->getConfigureSet();
	m_cstrRoadName = A2W(m_pConfigureSet->get("projectname", "roadname").c_str());
	m_cstrRoadSection = A2W(m_pConfigureSet->get("projectname", "roadsection").c_str());
	m_cstrRoadNumber = A2W(m_pConfigureSet->get("projectname", "roadnumber").c_str());
	m_ccomboboxDirection.SetCurSel ( atoi ( m_pConfigureSet->get("projectname", "direction").c_str() ) );
	m_cstrStakeNumber = A2W(m_pConfigureSet->get("projectname", "stakenumber").c_str());
	m_fDistance = atof(m_pConfigureSet->get("projectname", "distance").c_str());
	UpdateData( FALSE );

	return FALSE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

std::string deleteCharString(std::string sourceString, char chElemData){
	std::string resultString = "";
    for (int i = 0; i < sourceString.length(); i++) {
        if (sourceString[i] != chElemData) {
            resultString += sourceString[i];
        }
    }
    return resultString;
}


void CDialogProjectName::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData( TRUE );

	if ( m_cstrRoadName.GetLength() == 0 )
	{
		MessageBox(L"路名为空, 请重新填写", L"警告");
		return ;
	}
	TCHAR* tcP = m_cstrRoadName.GetBuffer();
	for (int i = 0 ; i < m_cstrRoadName.GetLength();++i)
	{
		if (*tcP == '*' || *tcP == '\\' || *tcP == '@' || *tcP == '.')
		{
			MessageBox(L"路名不能有特殊字符\"*,.,\\,@,\"", L"警告");
			return ;
		}
		if ((i+1) != m_cstrRoadName.GetLength())
		{
			tcP++;
		}
	}

	if ( m_cstrRoadSection.GetLength() == 0 )
	{
		MessageBox(L"路段为空, 请重新填写", L"警告");
		return ;
	}

	if (m_cstrRoadNumber.GetLength() == 0)
	{
		MessageBox(L"道号为空, 请重新填写", L"警告");
	}

	//m_daohao.GetLBText ( m_daohao.GetCurSel(), m_strDaohao );、//不用commbox了
	//m_strDaohao = m_cstrRoadNumber;
	//去空格
	std::string strRoadName,strRoadSection,strRoadNumber;
	strRoadName = CT2A(m_cstrRoadName.GetString()); 
	strRoadSection = CT2A(m_cstrRoadSection.GetString()); 
	strRoadNumber = CT2A(m_cstrRoadNumber.GetString()); 
	strRoadName=deleteCharString(strRoadName,' ');
	strRoadSection=deleteCharString(strRoadSection,' ');
	strRoadNumber=deleteCharString(strRoadNumber,' ');
	m_cstrRoadName=strRoadName.c_str();
	m_cstrRoadSection=strRoadSection.c_str();
	m_cstrRoadNumber=strRoadNumber.c_str();
	
	m_ccomboboxDirection.GetLBText ( m_ccomboboxDirection.GetCurSel(), m_cstrDirection );

	m_projectName = m_cstrRoadName + L"_" + m_cstrRoadSection + L"_" + m_cstrRoadNumber + L"_" + m_cstrDirection;

	if (m_fDistance < 0 || m_fDistance > 1000)
	{
		MessageBox(L"距离值为0-999", L"警告");
		return;
	}

	if (m_cstrStakeNumber.GetLength() != 5)
	{
		MessageBox(L"桩号输入格式不正确，为5个字，例如：K0002", L"警告");
		return;
	}

	tcP = m_cstrStakeNumber.GetBuffer();

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
	m_startLen = inum * 1000 + m_fDistance;




	USES_CONVERSION;
	m_pConfigureSet->set( "projectname", "roadname", W2A( m_cstrRoadName ) );
	m_pConfigureSet->set( "projectname", "roadsection", W2A( m_cstrRoadSection ) );
	m_pConfigureSet->set( "projectname", "roadnumber", W2A( m_cstrRoadNumber ) );
	m_pConfigureSet->set( "projectname", "stakenumber", W2A( m_cstrStakeNumber ) );
	{
		std::stringstream ss;
		ss << m_ccomboboxDirection.GetCurSel();
		m_pConfigureSet->set("projectname", "direction", ss.str());
	}
	{
		std::stringstream ss;
		ss << m_fDistance;
		m_pConfigureSet->set("projectname", "distance", ss.str());
	}
	m_pConfigureSet->write();                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                      

	OnOK();
}
