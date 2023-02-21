/***************************************************************************
Copyright (C),  
文件：TrackPosDlg.h
作者：谢荣耀
版本：1.0
日期：2009-11-19
描述：动画路径控制点对话框。将作为一个TAB页面嵌入到主对话框中 

修改记录：
作者						日期					描述
***************************************************************************/
#pragma once
//#include <CartoonObject\CartoonEventHandler.h>
#include <Com\thread\UIThreadUpdate.h>
#include <CartoonObject/TrackPosList.h>
#include "..\..\CartoonObject\resource.h"

// CTrackPosDlg dialog

class CTrackPosDlg : public CPropertyPage, public COM::CUIThreadUpdate
{
	DECLARE_DYNAMIC(CTrackPosDlg)

public:
	CTrackPosDlg();
	virtual ~CTrackPosDlg();

// Dialog Data
	enum { IDD = IDD_POSDIALOG };
public:
	virtual BOOL OnInitDialog();
	void ShowDialog();
	void Update();
	virtual void OnClose();

private:
	 CTrackPosList m_trackPosList;
	 WNDPROC       m_wndProc;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedPosModelBtn();
	afx_msg void OnBnClickedStoreBtn();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();

	//自定义消息
	afx_msg LRESULT OnUpdate(WPARAM wParam, LPARAM lParam);
private:
	void Destroy();
	
};
