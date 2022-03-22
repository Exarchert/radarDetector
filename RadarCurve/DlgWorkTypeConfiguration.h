#pragma once
#include "afxwin.h"
#include "RadarCurve/RadarManager.h"
// DlgWorkTypeConfiguration �Ի���

class DlgWorkTypeConfiguration : public CDialog
{
	DECLARE_DYNAMIC(DlgWorkTypeConfiguration)

public:
	DlgWorkTypeConfiguration(CWnd* pParent = NULL, CString strRadarIni= _T(""));   // ��׼���캯��
	virtual ~DlgWorkTypeConfiguration();

// �Ի�������
	enum { IDD = IDD_DlgWorkTypeConfiguration };

private:
	CString m_strRadarIni;

	//���ӷ�ʽ
	CComboBox m_ComBoxDeviceLinkType;

	// USB���ͣ�ͨ��������
	CComboBox m_ComBox_USBTYpe;
	//����ͨ������
	CButton m_CheckBox_Channel1;
	CButton m_CheckBox_Channel2;
	CButton m_CheckBox_Channel3;
	CButton m_CheckBox_Channel4;
	CButton m_CheckBox_Channel5;
	CButton m_CheckBox_Channel6;
	CButton m_CheckBox_Channel7;
	CButton m_CheckBox_Channel8;


	//�洢��ʽ
	CComboBox m_ComBoxSaveType;
	// ����Դ
	CString m_dbSource;
	// ���ݿ�
	CString m_dbName;
	// �û�
	CString m_dbUser;
	// ����
	CString m_dbPwd;

		


	// �״����ݱ�����ļ�·��
	CString m_EditSaveFile;
	// �־��洢λ��
	CString m_EditPicSaveFile;

	RadarManager::WorkType m_WorkType;

	//USB�����е�ͨ������
	RadarManager::USBType m_UsbType;

	
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	void OnClose();

	afx_msg void OnBnClickedBtnok();
	afx_msg void OnBnClickedBtncancel();
	virtual BOOL OnInitDialog();
	afx_msg void OnCbnSelendokComboxSaveType();
	afx_msg void OnBnClickedBtnSelectpath();
	
	afx_msg void OnBnClickedBtnPicsavefile();
	
	afx_msg void OnCbnSelendokComboxDevicelinktype();
	
	afx_msg void OnCbnSelendokComboxUsbtype();
	
};
