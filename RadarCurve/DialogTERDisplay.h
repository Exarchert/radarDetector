#pragma once
#include "resource.h"
#include <time.h>
#include "TERDisplayWnd.h"

// CDialogTERDisplay �Ի���

#define WM_NEW_RADAR_DATA WM_USER + 10

#define MAX_CHANNEL 16

#define CHANNELCOUNT 28

class CDialogTERDisplay : public CDialog
{
	DECLARE_DYNAMIC(CDialogTERDisplay)
/*public:
	//ʵʱ����������������ʾ
	enum separateScreen
	{
		screenFirst = 0,
		screenSecond = 1,
		screenThird = 2,
		screenFourth = 3
	};*/
public:
	CDialogTERDisplay(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDialogTERDisplay();

// �Ի�������
	enum { IDD = IDD_DIALOG_TER_DISPLAY };
	void resizeChannelWnd();

	void AddTERData( TERData *lpData );
	//void DeleteRadarData( int index );

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	//void changeChannel( int index, bool flag );
	//void changeChannel( int index );

	void updateMenuState( int index, CMenu *lpMenu );

	DECLARE_MESSAGE_MAP()
	afx_msg LRESULT OnRadarData( WPARAM wParam, LPARAM lParam );

	std::string _cfgGroupName;

protected:
	TERDisplayWnd m_TERDisplayWnd;
	//CStatic _channelName[CHANNELCOUNT];
	unsigned int _dcValue;

	osg::Vec3d _position;//hjl6.5 ���ڶ�ʱ���¾�γ��
	CString m_cstrLonlat;//hjl6.5 ���ڶ�ʱ���¾�γ��
	CStatic _lonlat;//hjl6.5 ���ڶ�ʱ���¾�γ��

	CString _CstrGpsCondition;//��¼gps���
	CStatic _gpsCondition;//��¼gps���

	//int m_arrnRecordTotalWheelCount[CHANNELCOUNT];//��¼�����ܵ��� 
	//int m_arrnRealTotalWheelCount[CHANNELCOUNT];//��¼ʵ���ܵ���
	int m_nRecordTotalWheelCount;//��¼�����ܵ��� 
	int m_nRealTotalWheelCount;//��¼ʵ���ܵ���
	//int _recordTotalWheelCount;//��¼�����ܵ��� ���ڶ�ʱ���¶������
	//int _realTotalWheelCount;//��¼ʵ���ܵ��� ���ڶ�ʱ���¶������
	CString _CstrDataMissing;//hjl20210302 ���ڶ�ʱ���¶������
	CStatic _dataMissing;//hjl20210302 ���ڶ�ʱ���¶������

	float _wheelDistance;//hjl20210623 ���ڶ�ʱ���¾���
	float _gpsDistance;//hjl20210623 ���ڶ�ʱ���¾���
	CString _CstrDistance;//hjl20210623 ���ڶ�ʱ���¾���
	CStatic _distance;//hjl20210623 ���ڶ�ʱ���¾���
	
	time_t _tLastTime;//��¼ʱ�� hjl20200907 �ٶ���ʾģ��
	float _fLastLen;//��¼���볤�� hjl20200907 �ٶ���ʾģ��
	float _fCurLen;//��¼���볤�� hjl20200907 �ٶ���ʾģ��
	float _fSpeed;//�����ٶ� hjl20200907 �ٶ���ʾģ��
	CString _CstrSpeed;//��cstr���վ�γ�� hjl20200907 �ٶ���ʾģ��
	CStatic _speed;//��ʾ�ٶ� hjl20200907 �ٶ���ʾģ��

	float _fRecommendableSpeed;//�����ٶ� hjl20200907 �ٶ���ʾģ��
	CString _CstrRecommendableSpeed;//��cstr���վ�γ�� hjl20210408 �Ƽ��ٶ���ʾģ��
	CStatic _recommendableSpeed;//��ʾ�ٶ� hjl20210408 �Ƽ��ٶ���ʾģ��


	int _iDisplayFlag[CHANNELCOUNT];//hjl11.9 ���ڳ����ʾ �����ж���ǰ���Ƿ���ʾ
	int _iDisplayExtractValue;//hjl11.9 ���ڳ����ʾ _iDisplayExtractValue���г��һ����ʾ


	//Ƶ������
	int m_channelCount;
	std::vector<int> m_channelVec;

	//int m_nInformationIndex;

	CMenu m_pMenu;
public:
	virtual BOOL OnInitDialog();

	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);//δ����
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);//����timer��ˢ�¼�� ��һ��ʱ�����һ�Ρ�ʵʱ���²�����
	afx_msg void OnTimer(UINT_PTR nIDEvent);//��һ��ʱ�����һ�Ρ�ʵʱ���²�����
	afx_msg void OnClose();
	afx_msg void OnSize(UINT nType, int cx, int cy);

	std::vector<int> RemoveChannels();

};