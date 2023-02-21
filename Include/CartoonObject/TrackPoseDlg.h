/***************************************************************************
Copyright (C),  
文件：TrackPoseDlg.h
作者：谢荣耀
版本：1.0
日期：2009-11-20
描述：动画路径片段姿态对话框。将作为一个TAB页面嵌入到主对话框中 

修改记录：
作者						日期					描述
***************************************************************************/

#pragma once
#include <Com\thread\UIThreadUpdate.h>
#include "..\..\CartoonObject\resource.h"
#include "afxcmn.h"

// CTrackPoseDlg dialog

class CTrackPoseDlg : public CPropertyPage, public COM::CUIThreadUpdate
{
	DECLARE_DYNAMIC(CTrackPoseDlg)

public:
	CTrackPoseDlg();   // standard constructor
	virtual ~CTrackPoseDlg();

// Dialog Data
	enum { IDD = IDD_POSEDIALOG };
public:
	virtual BOOL OnInitDialog();
	void ShowDialog();
    void Update();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCancel();
	
	afx_msg LRESULT OnUpdate(WPARAM wParam, LPARAM lParam);//自定义消息：处理片段的增加、删除等
	afx_msg LRESULT OnPickSegment(WPARAM wParam, LPARAM lParam);//自定义消息：处理拾取片段等

private:
	void Destroy();
public:
	CListCtrl m_segListCtrl;
	afx_msg void OnNMRclickSegmentlist(NMHDR *pNMHDR, LRESULT *pResult);
};
