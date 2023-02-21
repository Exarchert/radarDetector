#pragma once
#include "afxwin.h"
#include <osg/Node>


/*
 *	��ͼ��
 *	
 *	write by StackSnow
 */

class ssCore;

class ssCoreView : public CView
{
public:
	DECLARE_DYNCREATE(ssCoreView)

protected:
	ssCoreView();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
	

public:
	virtual ~ssCoreView();
	virtual void OnDraw(CDC* pDC);      // ��д�Ի��Ƹ���ͼ
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
