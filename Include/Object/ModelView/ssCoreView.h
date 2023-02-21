#pragma once
#include "afxwin.h"
#include <osg/Node>


/*
 *	视图类
 *	
 *	write by StackSnow
 */

class ssCore;

class ssCoreView : public CView
{
public:
	DECLARE_DYNCREATE(ssCoreView)

protected:
	ssCoreView();           // 动态创建所使用的受保护的构造函数
	

public:
	virtual ~ssCoreView();
	virtual void OnDraw(CDC* pDC);      // 重写以绘制该视图
	void setOsgNode(osg::Node* node);

#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	DECLARE_MESSAGE_MAP()

public:
	ssCore*		m_SSCore;
	HWND        m_hWndHandle;
	HANDLE      mRun3DThread;

	osg::ref_ptr<osg::Node> m_node;

public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
};
