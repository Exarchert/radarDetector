/*************************************************************************
FileName��CTabCtrlBase.h CTabCtrlBase.cpp
Author:	Liugh
Date��2010-01-20
Description��Tab Ctrl Base Class
**************************************************************************/
#pragma once

/////////////////////////////////////////////////////////////////////////////
// CTabCtrlBase window


/***********************************************/
//�϶�ͼ���뿪���ں��͸���Ϣ

#define WM_NOTIFY_TABCTRLCHANGED WM_USER + 200

/***********************************************/

class CTabCtrlBase : public CTabCtrl
{
public:
	CTabCtrlBase();

	virtual ~CTabCtrlBase();

	void AddItem(CWnd *pwnd,LPTSTR name);//���ѡ�

public:

	int		m_ispeed;		//�˶��ٶ�
	BOOL	m_bMove;		//�Ƿ��˶�
	int		m_wndwidth;		//�Ӵ���Ŀ��
	CPoint	m_bottomright;	//�Ӵ������½ǵ�����
	CPoint	m_topleft;		//�Ӵ������Ͻǵ�����
	CWnd *	m_pCurWnd;		//��ǰ����
	CRect	m_tabrect;		//Tab�ľ�������
	CPoint	m_ptTabs;		//���Ͻ�����

	int m_itemcount;//Item����

protected:

	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnSelchange(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchanging(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

	DECLARE_MESSAGE_MAP()
};