#pragma once
//OSG
#include <OpenThreads/ScopedLock>
#include <OpenThreads/Mutex>
#include "WaveFormWnd.h"

class ConfigureSet;

// DlgRadarParameterConfig �Ի���
class DlgRadarParameterConfig : public CDialog
{
	DECLARE_DYNAMIC(DlgRadarParameterConfig)

public:
	DlgRadarParameterConfig(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~DlgRadarParameterConfig();

// �Ի�������
	enum { IDD = IDD_Dlg_RadarParameterConfig };

private:
	CString m_serverIP;
	short m_serverPort;

	CString m_cstrFtpServerIP;
	CString m_cstrFtpPort;
	CString m_cstrFtpAccount;
	CString m_cstrFtpPassword;

	//CString m_cstrDeviceModel;
	//CString m_id;
	CComboBox m_ComboBoxDeviceModel;
	CComboBox m_ComboBoxChannelCount;
	CComboBox m_dielectricComboBox;
	CComboBox m_sampleComboBox;
	CComboBox m_sampleRatioComboBox;
	CComboBox m_gainComboBox;
	//CSliderCtrl m_gainValueSlider;
	//CEdit m_gainValue;
	int m_gainValue;
	//CSliderCtrl m_delaySlider;
	//CEdit m_delayValue;
	int m_delayValue;
	//CComboBox m_addCountComboBox;
	CComboBox m_testTypeComboBox;
	//short m_channelCount;
	int m_precision;
	float m_precLen;
	CComboBox m_comboxPrecRatio;
	CComboBox m_ComboBoxUploadType;
	//CComboBox m_ComboBoxAutoSeperate;
	CComboBox m_ComboBoxSeperateType;
	int m_dTimeInterval;
	int m_dWheelCountInterval;
	CComboBox m_ComboBoxSaveFileType;

	//CComboBox m_precIndex;

	//����ĻҪչʾ��Ƶ��
	CString m_screenFirstChannels;
	CString m_screenSecondChannels;
	CString m_screenThirdChannels;
	CString m_screenFourthChannels;

	//gps
	short m_comNumber;//com��
	int m_portByte;//������
	CComboBox m_parityBit;//У��λ
	int m_dataBit;//����λ
	CComboBox m_stopBit;//ֹͣλ
	short m_sGpsFrequency;//gpsƵ��

	//���������
	float m_timeWindow;
	float m_maxDepth;
	int m_scanSpeed;

	//�Զ�����
	int m_markValue;//�Զ�����ֵ
	
	//�����ʾ
	int m_displayExtract;//m_displayExtract���г�һ����ʾ

	//���ι�С����
	int m_dWarningInterval;//�����
	int m_dWarningThreshold;//��ֵ

	int m_nImgInterval;
	int m_nImgQuality;

	ConfigureSet* m_pConfigureSet;
	OpenThreads::Mutex _lock;

	WaveFormWnd _waveFormWnd[16];//hjl 20210418�������ò�����ʾ
	CStatic _channelName[16];//hjl 20210418�������ò�����ʾ
	CStatic _channelCorrection[16];//hjl 20210805��������
	int m_dCorrection[16];

	CButton m_CheckBox_Channel[8];//��άͨ��ѡ��

protected:
	//SpectrumWnd _spectrumWnd[8];

protected:
	virtual void DoDataExchange(CDataExchange* pDX);   // DDX/DDV ֧��
	

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
private:
	// ���������ļ���ʼ������
	void InitByRadarConfig();
public:
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnBnClickedBtnsetOk();
	afx_msg void OnBnClickedBtnsetCancel();
	afx_msg void OnBnClickedButtonApply();
	afx_msg void OnBnClickedButtonWaveFormTest();
	afx_msg void OnClose();
	afx_msg void OnEnChangeEditPort();
//	afx_msg void OnCbnEditchangeComboDielectric();
	afx_msg void OnCbnSelchangeComboDielectric();
//	afx_msg void OnCbnCloseupComboDielectric();
	afx_msg void OnCbnSelchangeComboSample();
	afx_msg void OnCbnSelchangeComboSampleRatio();

	afx_msg void OnEnKillfocusEditCorrection1();
	afx_msg void OnEnKillfocusEditCorrection2();
	afx_msg void OnEnKillfocusEditCorrection3();
	afx_msg void OnEnKillfocusEditCorrection4();
	afx_msg void OnEnKillfocusEditCorrection5();
	afx_msg void OnEnKillfocusEditCorrection6();
	afx_msg void OnEnKillfocusEditCorrection7();
	afx_msg void OnEnKillfocusEditCorrection8();
	afx_msg void OnEnKillfocusEditCorrection9();
	afx_msg void OnEnKillfocusEditCorrection10();
	afx_msg void OnEnKillfocusEditCorrection11();
	afx_msg void OnEnKillfocusEditCorrection12();
	afx_msg void OnEnKillfocusEditCorrection13();
	afx_msg void OnEnKillfocusEditCorrection14();
	afx_msg void OnEnKillfocusEditCorrection15();
	afx_msg void OnEnKillfocusEditCorrection16();

	void changeChannel( int index, bool flag );//hjl 20210418�������ò�����ʾ
	void resizeChannelWnd();//hjl 20210418�������ò�����ʾ
	void AddRadarData( RadarData *lpData , int index );//hjl 20210418�������ò�����ʾ

	void refreshControlCondition();//���¿ؼ�ȡֵ�����޸ĵ�״̬
	void SetCorrection(std::vector<int> correction);//�����ӳ�΢��ֵ
public:
	afx_msg void OnCbnSelchangeCombo1();
public:
//	afx_msg void OnCbnEditchangeComboUploadType();
public:
	afx_msg void OnCbnSelchangeComboUploadType();
public:
//	afx_msg void OnCbnCloseupComboUploadType();
public:
//	afx_msg void OnCbnEditupdateComboUploadType();
public:
//	afx_msg void OnCbnKillfocusComboUploadType();
//	afx_msg void OnCbnSelendokComboSaveFileType();
	afx_msg void OnCbnSelchangeComboSaveFileType();
	afx_msg void OnCbnSelendokComboDeviceModel();
	afx_msg void OnBnClickedAutoCorrection();
	afx_msg void OnCbnSelendokComboChannelcount();
};
