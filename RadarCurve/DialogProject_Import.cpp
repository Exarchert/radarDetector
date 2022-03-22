// DialogProjectExport.cpp : 实现文件
//

#include "stdafx.h"
#include "RadarCurve.h"
#include "DialogProject_Import.h"


// CDialogProject_Import 对话框

IMPLEMENT_DYNAMIC(CDialogProject_Import, CDialog)

CDialogProject_Import::CDialogProject_Import(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogProject_Import::IDD, pParent)
{

}

CDialogProject_Import::~CDialogProject_Import()
{
}

void CDialogProject_Import::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDialogProject_Import, CDialog)
END_MESSAGE_MAP()


// CDialogProject_Import 消息处理程序
