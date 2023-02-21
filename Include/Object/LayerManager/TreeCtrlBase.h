/*********************************************************
FileName:		CTreeCtrlBase.h		CTreeCtrlBase.cpp
Author:			LiuGH
Data:			2010.4.14
Description:	Inherits from CTreeCtrl to expand fuction,
				such as drag、scroll and so on.
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
目的：设置TreeCtrl控件风格，包括节点名称可编辑、扩展按钮、子节点连接线、根节点连接线、复选框
	*/
	void InitTreeStyle(bool bHasCheckBox = true);
	/*
	目的：为控件绑定图标资源
	*/
	void SetIcons(UINT nBitmapID, int cx = 16, int nGrow = 1);

private:
	CImageList	m_ImageList;		//节点图标
	CImageList* m_pDragImage;		//拖动时的图像列表
	DWORD		m_dwDrawStart;		//开始拖动时刻
	HTREEITEM	m_hDragSrc;			//被拖动节点
	HTREEITEM	m_hDragDist;		//接收拖动的节点
	bool		m_bDraged;			//拖动标识符
	UINT		m_nMsMoveTimerID;	//检测鼠标移动的定时器
	UINT		m_nScrollTimerID;	//滚动检测定时器
	UINT		m_nTicks;			//计算滚动历时
	CPoint		m_ptMouseMove;		//鼠标位置
	
	afx_msg void OnBegindrag(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	
	HTREEITEM CopyBranch(HTREEITEM hChildBruch,HTREEITEM hNewParent,HTREEITEM hInsertAfter);
	HTREEITEM CopyItem(HTREEITEM hItem,HTREEITEM hParent,HTREEITEM hInsertAfter);
	//选中当前节点所有子节点
	BOOL SelectBranch(HTREEITEM hSelect,BOOL bCheck);
	
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg BOOL OnNMClick(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTvnEndlabeledit(NMHDR *pNMHDR, LRESULT *pResult);
	
	
	//派生类如需响应选中、拖动、编辑消息，需自定义响应函数
	//自定义消息类型
	enum TreeMsg
	{
		WM_TREEVIRE_CHECKSTATE	= WM_USER + 0x1000,		//复选响应
		WM_TREEVIRE_DRAGED		= WM_USER + 0x1001,		//拖动响应
		WM_TREEVIRE_PREDRAGED	= WM_USER + 0x1002,		//判断拖动目标节点,lparam返回值必须为false方可禁止拖动
		WM_TREEVIRE_EDIT		= WM_USER + 0x1003		//编辑响应
	};
};
}