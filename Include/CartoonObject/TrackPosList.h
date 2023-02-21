/***************************************************************************
Copyright (C),  
�ļ���TrackPosList.h
���ߣ�л��ҫ
�汾��1.0
���ڣ�2009-11-19
����������·�����Ƶ��б��֧࣬�ֶ��б��������ֶεı༭

�޸ļ�¼��
����						����					����
***************************************************************************/

#pragma once 
#include <vector>
#include <osg/Vec3d>
//////////////////////////////////////////////////////////////////////////
//CSubEdit
class CTrackPosSubEdit : public CEdit
{
	DECLARE_DYNAMIC(CTrackPosSubEdit)

public:
	CTrackPosSubEdit();
	virtual ~CTrackPosSubEdit();

protected:
	DECLARE_MESSAGE_MAP()

	afx_msg void OnWindowPosChanging(WINDOWPOS FAR* lpwndpos);

public:
	int m_x; //���ڵ�x����
};

///////////////////////////////////////////////////////////////////////////
//CTracksView
class CTrackPosList : public CListCtrl
{
public:
	CTrackPosList();
	virtual ~CTrackPosList();

	DECLARE_DYNAMIC(CTrackPosList)

protected:
	DECLARE_MESSAGE_MAP()

public:
	int m_item;   
	int m_subitem; 
	CTrackPosSubEdit m_editWnd;
private:
	osg::Vec3d beginEditPoint;

public:
	bool InitList();
	bool FillData(std::vector<osg::Vec3d> controlPoints);

public:
	afx_msg void OnNMRclick(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNewTrack();
	afx_msg void OnCopyTrack();
	afx_msg void OnCutTrack();
	afx_msg void OnPasteTrack();
	afx_msg void OnDeleteTrack();
	//�༭����
	afx_msg void OnBeginLabelEdit(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEndLabelEdit(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};
