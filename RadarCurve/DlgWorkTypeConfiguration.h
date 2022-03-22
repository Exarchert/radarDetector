#pragma once
#include "afxwin.h"
#include "RadarCurve/RadarManager.h"
// DlgWorkTypeConfiguration 对话框

class DlgWorkTypeConfiguration : public CDialog
{
	DECLARE_DYNAMIC(DlgWorkTypeConfiguration)

public:
	DlgWorkTypeConfiguration(CWnd* pParent = NULL, CString strRadarIni= _T(""));   // 标准构造函数
	virtual ~DlgWorkTypeConfiguration();

// 对话框数据
	enum { IDD = IDD_DlgWorkTypeConfiguration };

private:
	CString m_strRadarIni;

	//连接方式
	CComboBox m_ComBoxDeviceLinkType;

	// USB类型（通道个数）
	CComboBox m_ComBox_USBTYpe;
	//网口通道个数
	CButton m_CheckBox_Channel1;
	CButton m_CheckBox_Channel2;
	CButton m_CheckBox_Channel3;
	CButton m_CheckBox_Channel4;
	CButton m_CheckBox_Channel5;
	CButton m_CheckBox_Channel6;
	CButton m_CheckBox_Channel7;
	CButton m_CheckBox_Channel8;


	//存储方式
	CComboBox m_ComBoxSaveType;
	// 数据源
	CString m_dbSource;
	// 数据库
	CString m_dbName;
	// 用户
	CString m_dbUser;
	// 密码
	CString m_dbPwd;

		


	// 雷达数据保存的文件路径
	CString m_EditSaveFile;
	// 街景存储位置
	CString m_EditPicSaveFile;

	RadarManager::WorkType m_WorkType;

	//USB类型中的通道个数
	RadarManager::USBType m_UsbType;

	
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

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
