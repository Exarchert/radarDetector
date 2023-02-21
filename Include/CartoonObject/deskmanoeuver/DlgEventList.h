#pragma once
#include <CartoonObject/DeskManoeuver/ScriptGenerateManager.h>
#include <Object/LayerManager/TreeCtrlBase.h>
#include <tinyxml.h>

// CDlgEventList 对话框

class CDlgEventList : public CDialog
{
	DECLARE_DYNAMIC(CDlgEventList)

public:
	CDlgEventList(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgEventList();

// 对话框数据
	enum { IDD = IDD_DLG_EVENTLIST };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

	virtual BOOL OnInitDialog();

	afx_msg void OnNMRclickList(NMHDR *pNMHDR, LRESULT *pResult);		
	afx_msg void OnAddItem();
	afx_msg void OnTimeSet();
	afx_msg void OnRemoveItem();
	afx_msg void OnClose();

	LRESULT OnModifyName(WPARAM wParam,LPARAM lParam);
	LRESULT OnTreeDrag(WPARAM wParam,LPARAM lParam);

	HTREEITEM AddTreeItem(HTREEITEM hParent,HTREEITEM hAfter,UINT iImage,CString text,DWORD data = -1);
protected:
	EMObj::CTreeCtrlBase m_EventList;

	enum ImageType
	{
		Folder = 0,
		Film
	};

	struct EventList 
	{
		int _ID;
		int _ParentID;
		float _Time;
		CString _ItemContent;
	};

	std::vector<EventList> m_EventVec;

	int GetNewID();
public:
	void ResetEventTree(CScriptGenerateManager* pScriptCreateManager);
	CListCtrl m_ItemList;
	afx_msg void OnBtnClickedBtnExpand();

	BOOL DeleteBranch(HTREEITEM hRoot);
	BOOL DeleteChildBranch(HTREEITEM hParent);
	BOOL DelItem(HTREEITEM hDelItem);

	BOOL SaveBranchToXml(HTREEITEM hRoot,TiXmlElement* pRootElement);
	BOOL ParserFile(CString filePath);

	void ParserChildren(HTREEITEM hParent,TiXmlElement* pParent);

	void SetDeskFileName(CString scriptName);

	HTREEITEM	m_hRoot;

	CRect		m_RectLarge;
	CRect		m_RectSmall;
	CRect		m_RectSeparator;

	CString		m_ScriptName;
	bool m_bChanged;
};
