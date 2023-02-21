/***************************************************************************
Copyright (C),  
文件：TrackPosList.h
作者：谢荣耀
版本：1.0
日期：2009-11-19
描述：动画路径控制点列表类，支持对列表中任意字段的编辑

修改记录：
作者						日期					描述
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
	int m_x; //窗口的x坐标
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
	//编辑功能
	afx_msg void OnBeginLabelEdit(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEndLabelEdit(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};
