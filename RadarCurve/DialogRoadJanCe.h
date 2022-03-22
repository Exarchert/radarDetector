#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include <vector>



// DialogRoadJanCe �Ի���


#ifndef IDD_DIALOG_GLJC                 1060
#define IDD_DIALOG_GLJC                 1060
#endif

class RadarDataRader;


struct DrawInfo
{
	DrawInfo()
	{
		m_fLeftLength = 0.0;
		m_fRightLength = 0.0;
		m_fDongXiPY = 0.0;
		m_fNanBeiPY = 0.0;
		m_fAsColor[0] = 1;
		m_fAsColor[1] = 0;
		m_fAsColor[2] = 0;
		m_fAsColor[3] = 1;

	};
	float m_fLeftLength;//���
	float m_fRightLength;//�Ҿ�
	float m_fDongXiPY;//��ƫ
	float m_fNanBeiPY;//��ƫ
	float m_fAsColor[4];//��ɫ0-1
	std::vector<CString> m_vProCreateTime;
	std::vector<CString> m_vProAllName;
	std::vector<int> m_vChannelIndex;

};



class DialogRoadJanCe : public CDialog
{
	DECLARE_DYNAMIC(DialogRoadJanCe)

public:
	DialogRoadJanCe(CWnd* pParent = NULL);   // ��׼���캯��

	virtual ~DialogRoadJanCe();

	virtual BOOL OnInitDialog();

// �Ի�������
	enum { IDD = IDD_DIALOG_GLJC };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()


	void UpdaPeiZhi();
	void GetZhuanghaoLength(const long & startLen,CString & strZhunagHaoEX,CString &  strJULI );
	
private:


	DrawInfo* m_stuDrawInfo;
	float m_fAsColor[4];
	CListCtrl m_projectList;
	CEdit m_editLeftLength;
	CEdit m_editRightLength;
	CEdit m_editDongXiPY;
	CEdit m_editNanBeiPY;
	CEdit m_editToumindu;

	CButton m_btnColor;
public:
	void ReInit();
	//afx_msg void OnBnClickedButtonColor();
	afx_msg void OnBnClickedButtonSelectProjectJc();
	afx_msg void OnBnClickedButtonClearAllproject();
	afx_msg void OnBnClickedButtonDrowRoud();
	afx_msg void OnBnClickedButtonClearSelect();
	
	afx_msg void OnBnClickedBtnStupColor();
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
};
