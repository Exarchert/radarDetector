#pragma once
#include <osg/Switch>
#include "..\..\..\CartoonObject\Mark\EhMark.h"
#include "..\..\..\CartoonObject\Mark\SmartReadCallback.h"
#include "afxwin.h"
#include "afxcmn.h"

// DlgMark dialog
namespace CartoonObject
{
class DlgMark : public CDialog
{
	DECLARE_DYNAMIC(DlgMark)

public:
	DlgMark(CWnd* pParent = NULL);   // standard constructor
	virtual ~DlgMark();

// Dialog Data
	enum { IDD = IDD_DLG_MARK };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg); 
	DECLARE_MESSAGE_MAP()
//public:



public:
	afx_msg void OnBnClickedButColor();
	afx_msg void OnEnChangeEditWidth();
	afx_msg void OnEnChangeEditTrans();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedDeleteAll();
	afx_msg void OnBnClickedSave();
	afx_msg void OnClose();
	afx_msg void OnTvnBeginlabeleditTree1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTvnEndlabeleditTree1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClickTree1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMRClickTree1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnRemove();
	LRESULT OnEndLine(WPARAM wParam,LPARAM lParam);
	void popWindow();
	void saveTree();
private:
	void removeHandler();
	void addHandler();
	void addNodeTreeForFile();
	void addItem(CString itemName,osg::Node* node,BOOL bCheck);
	void deleteItem(HTREEITEM item);
	void saveNode(HTREEITEM item,TiXmlElement *root,CString path);
public:
	int							m_width;
	int							m_transparency;
	HTREEITEM					m_CurItem;
	CTreeCtrl					m_tree;
private:
	osg::ref_ptr<EhMark>		_markHandler;
	osg::Vec4					_color;
	CString						m_BeginLablenName;
	osg::ref_ptr<osg::Switch>   _switch;
	osg::ref_ptr<SmartReadCallback> _callback;
	

};
}