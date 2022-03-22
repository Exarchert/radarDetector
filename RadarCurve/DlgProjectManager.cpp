// DlgProjectManager.cpp : 实现文件
//

#include "stdafx.h"
#include "RadarCurve.h"
#include "DlgProjectManager.h"


// DlgProjectManager 对话框

IMPLEMENT_DYNAMIC(DlgProjectManager, CDialog)

DlgProjectManager::DlgProjectManager(CWnd* pParent /*=NULL*/)
	: CDialog(DlgProjectManager::IDD, pParent)
{

}

DlgProjectManager::~DlgProjectManager()
{
}

void DlgProjectManager::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(DlgProjectManager, CDialog)
END_MESSAGE_MAP()


// DlgProjectManager 消息处理程序
