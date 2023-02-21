/***************************************************************************
Copyright (C),  
�ļ���TrackPoseDlg.h
���ߣ�л��ҫ
�汾��1.0
���ڣ�2009-11-20
����������·��Ƭ����̬�Ի��򡣽���Ϊһ��TABҳ��Ƕ�뵽���Ի����� 

�޸ļ�¼��
����						����					����
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
	
	afx_msg LRESULT OnUpdate(WPARAM wParam, LPARAM lParam);//�Զ�����Ϣ������Ƭ�ε����ӡ�ɾ����
	afx_msg LRESULT OnPickSegment(WPARAM wParam, LPARAM lParam);//�Զ�����Ϣ������ʰȡƬ�ε�

private:
	void Destroy();
public:
	CListCtrl m_segListCtrl;
	afx_msg void OnNMRclickSegmentlist(NMHDR *pNMHDR, LRESULT *pResult);
};
