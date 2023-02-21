/*********************************************************
FileName:		CTreeCtrlBase.h		CTreeCtrlBase.cpp
Author:			LiuGH
Data:			2010.4.14
Description:	Inherits from CTreeCtrl to expand fuction,
				such as drag��scroll and so on.
**********************************************************/
#pragma once
#include <Object/Export.h>

// CTreeCtrlBase

namespace EMObj
{
	
class OBJECT_EXPORT CTreeCtrlBase : public CTreeCtrl
{
	DECLARE_DYNAMIC(CTreeCtrlBase)
		
public:
	CTreeCtrlBase();
	virtual ~CTreeCtrlBase();
	
protected:
	DECLARE_MESSAGE_MAP()
public:	
/*
Ŀ�ģ�����TreeCtrl�ؼ���񣬰����ڵ����ƿɱ༭����չ��ť���ӽڵ������ߡ����ڵ������ߡ���ѡ��
	*/
	void InitTreeStyle(bool bHasCheckBox = true);
	/*
	Ŀ�ģ�Ϊ�ؼ���ͼ����Դ
	*/
	void SetIcons(UINT nBitmapID, int cx = 16, int nGrow = 1);

private:
	CImageList	m_ImageList;		//�ڵ�ͼ��
	CImageList* m_pDragImage;		//�϶�ʱ��ͼ���б�
	DWORD		m_dwDrawStart;		//��ʼ�϶�ʱ��
	HTREEITEM	m_hDragSrc;			//���϶��ڵ�
	HTREEITEM	m_hDragDist;		//�����϶��Ľڵ�
	bool		m_bDraged;			//�϶���ʶ��
	UINT		m_nMsMoveTimerID;	//�������ƶ��Ķ�ʱ��
	UINT		m_nScrollTimerID;	//������ⶨʱ��
	UINT		m_nTicks;			//���������ʱ
	CPoint		m_ptMouseMove;		//���λ��
	
	afx_msg void OnBegindrag(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	
	HTREEITEM CopyBranch(HTREEITEM hChildBruch,HTREEITEM hNewParent,HTREEITEM hInsertAfter);
	HTREEITEM CopyItem(HTREEITEM hItem,HTREEITEM hParent,HTREEITEM hInsertAfter);
	//ѡ�е�ǰ�ڵ������ӽڵ�
	BOOL SelectBranch(HTREEITEM hSelect,BOOL bCheck);
	
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg BOOL OnNMClick(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTvnEndlabeledit(NMHDR *pNMHDR, LRESULT *pResult);
	
	
	//������������Ӧѡ�С��϶����༭��Ϣ�����Զ�����Ӧ����
	//�Զ�����Ϣ����
	enum TreeMsg
	{
		WM_TREEVIRE_CHECKSTATE	= WM_USER + 0x1000,		//��ѡ��Ӧ
		WM_TREEVIRE_DRAGED		= WM_USER + 0x1001,		//�϶���Ӧ
		WM_TREEVIRE_PREDRAGED	= WM_USER + 0x1002,		//�ж��϶�Ŀ��ڵ�,lparam����ֵ����Ϊfalse���ɽ�ֹ�϶�
		WM_TREEVIRE_EDIT		= WM_USER + 0x1003		//�༭��Ӧ
	};
};
}