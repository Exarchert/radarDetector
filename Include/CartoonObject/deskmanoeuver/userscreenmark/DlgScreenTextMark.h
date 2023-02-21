#pragma once
#include "afxwin.h"
#include "../../../../CartoonObject/resource.h"
#include "TextMark.h"


// CDlgScreenTextMark 对话框

class CDlgScreenTextMark : public CDialog
{
	DECLARE_DYNAMIC(CDlgScreenTextMark)


public:
	CDlgScreenTextMark(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgScreenTextMark();

// 对话框数据
	enum { IDD = IDD_DLG_TEXT_MARK };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnTextcolor();
	afx_msg void OnBnClickedBtnPos();

	CString m_TextContent;
	CString m_font;
	float m_TextSize;
	float m_R;
	float m_G;
	float m_B;
	int m_FontCom;
	int m_TextR;
	int m_TextG;
	int m_TextB;
	float m_ScreenX;
	float m_ScreenY;
	float m_Alpha;

	osg::Vec4 m_TextColor;

	osgText::Text* m_Text;
	osg::ref_ptr<osg::Geode> m_bkWnd;
	
	bool m_bNodeCreate;
	bool m_bSelWnd;
	bool m_bAdded;
	osg::ref_ptr<osg::Node> m_textNode;	
	osg::ref_ptr<osg::Group> m_markRoot;
	CTextMark m_TextManager;
	
public:	
	afx_msg void OnBnClickedBtnApply();
	afx_msg void OnBnClickedCancel();
	void ShowDialog();

	LRESULT OnBnClickedOk(WPARAM wParam,LPARAM lParam);

public:
	virtual BOOL OnInitDialog();
	void SetBkWnd();
	void GetRGB(int& r,int& g,int& b);

	void SetActive(BOOL bActive);

	BOOL m_bUseBKGround;
	BOOL m_bGetPos;
	int m_bkR;
	int m_bkG;
	int m_bkB;
	float m_bkAlpha;

	afx_msg void OnBnClickedCheckUsebkground();
	LRESULT OnGetPosByMouse(WPARAM wParam,LPARAM lParam);
	afx_msg void OnBnClickedGetxy();

	bool GenerateScripte(float timeLen,float bm,CString scriptName);
	afx_msg void OnBnClickedBtnTextColor();
	afx_msg void OnBnClickedBtnBkColor();
	afx_msg void OnEnChangeEditTextsize();
	afx_msg void OnEnChangeEditScreenX();
	afx_msg void OnEnChangeEditScreenY();
	afx_msg void OnEnChangeEditMarktext();
};
