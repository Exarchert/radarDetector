#pragma once


// CDialogProject_Import �Ի���

class CDialogProject_Import : public CDialog
{
	DECLARE_DYNAMIC(CDialogProject_Import)

public:
	CDialogProject_Import(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDialogProject_Import();

// �Ի�������
	enum { IDD = IDD_DIALOG_PROJECTEXPORT };//

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
