/***************************************************************************
Copyright (C),  
�ļ���TrackPosDlg.h
���ߣ�л��ҫ
�汾��1.0
���ڣ�2009-11-19
����������·�����Ƶ�Ի��򡣽���Ϊһ��TABҳ��Ƕ�뵽���Ի����� 

�޸ļ�¼��
����						����					����
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

	//�Զ�����Ϣ
	afx_msg LRESULT OnUpdate(WPARAM wParam, LPARAM lParam);
private:
	void Destroy();
	
};
