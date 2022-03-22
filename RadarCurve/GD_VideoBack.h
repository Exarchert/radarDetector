#pragma once
#include "afxwin.h"
#include "Resource.h"


// GD_VideoBack �Ի���
//#define IDD_DLG_VIDEO_BACK              1063
class  GD_VideoBack : public CDialog
{
	DECLARE_DYNAMIC(GD_VideoBack)

public:
	GD_VideoBack(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~GD_VideoBack();

// �Ի�������
	enum { IDD = IDD_DLG_VIDEO_BACK };

	enum ShowType
	{
		GD_None,
		GD_Show,
		GD_BackPlay,
	};

	void SetPicPath( CString picPath );
protected:

	bool        m_bInitWndShow;


	int			m_cx;
	int			m_cy;
	int			m_nFrameDuration;



	CString m_cPatrFile;
	int m_iOnceTime;


	bool        m_leftBackPlay;//
	bool        m_rightBackPlay;

	bool        m_bBackImageLeft;//
	bool        m_bBackImageRight;

	CString     m_cstrImageNameLeft;
	CString     m_cstrImageNameRight;


	//�ؼ�
	CStatic m_sWindowLeft;
	CStatic m_sWindowRight;

	CButton m_btnBackLeft;
	CButton m_btnBackRight;




		


public:

	//�Լ��ĺ���
	void OpenData();//������
	void OpenData2();

	//���������ʱ����

	void VideoBackplay();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnPaint();
	afx_msg void OnDestroy();
	afx_msg void OnMove(int x, int y);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnClose();
	afx_msg LRESULT OnUserBackPlay(WPARAM wParam,LPARAM lParam);
};
