#pragma once


// DlgProjectManager �Ի���

class DlgProjectManager : public CDialog
{
	DECLARE_DYNAMIC(DlgProjectManager)

public:
	DlgProjectManager(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~DlgProjectManager();

// �Ի�������
	enum { IDD = IDD_DLGPROJECTMANAGER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
