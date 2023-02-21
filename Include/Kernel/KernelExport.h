/************************************************************************
Copyright (C)  
�ļ���KernelExport.h KernelExport.cpp
���ߣ���־��
�汾��1.0
���ڣ�2009-11-11
�������ں˵�������

�޸ļ�¼��
����					����				����
----------------------------------------------------------------------
��־��					2009-12-23			���뿪��������Ⱦ��

************************************************************************/

#pragma once 

#include <Kernel\Export.h>
#include <Kernel\Init\InitModule.h>
#include <COM\Event\Event.h>
#include <COM\KernelContext\KernelContext.h>
#include <Object\Obj.h>
#include <Kernel\CmdHandleCenter\CmdManager.h>
#include <Kernel\AnimationManager\AnimationType.h>
#include <Kernel\PipleLineAnimation\PipleAnimationDB.h>
#include <iostream>
#include <vector>

namespace Kernel	{


KERNEL_EXPORT BOOL AddModuleInit(CInitModule *pModuleInit);

KERNEL_EXPORT void SetConfigFile(CString strFile);
KERNEL_EXPORT void WriteConfigValue(CString,CString,CString);
KERNEL_EXPORT bool ShowSelProDlg(CWnd* pParent = NULL);
KERNEL_EXPORT bool ShowSelProDlg(CWnd* pParent ,BOOL bIsShowDialog );
KERNEL_EXPORT bool ShowUrlSelProDlg(CWnd* pParent = NULL);
KERNEL_EXPORT bool SelectProDlg(CWnd* pParent = NULL);

KERNEL_EXPORT void KernelStartUp(HWND hwnd, COM::CMessageQueue *pMsgQueue = NULL, UINT MsgValue = 0, DWORD style = 1);

KERNEL_EXPORT void KernelStop();
KERNEL_EXPORT void GetDisplayCardInfo();

//���Ӳ��ID
//KERNEL_EXPORT BOOL CheckHardDiskID();

//�����ܹ�,mode = 0Ϊ�༭���� 1Ϊ��������	2Ϊʥ�칷
//KERNEL_EXPORT BOOL CheckDog(UINT mode = 0);

KERNEL_EXPORT BOOL CheckKey(HWND hwd);

KERNEL_EXPORT COM::CKernelContext * GetpKC();

KERNEL_EXPORT void AddStreetscape();

KERNEL_EXPORT void DirectTo(double lon, double lat, double height, float yaw, float pitch, float roll);

KERNEL_EXPORT void FlyTo(double lon, double lat, double height, float yaw, float pitch, float roll);

KERNEL_EXPORT void GetCameraPosPose(double &lon, double &lat, double &height, float &yaw, float &pitch, float &roll);

KERNEL_EXPORT void ShowDecorasWindow(bool bShow);

KERNEL_EXPORT void ShowWeatherWindow(bool bShow);

KERNEL_EXPORT void ShowLightingWindow(bool bShow);

KERNEL_EXPORT void ShowSurveyToolWindow(bool bShow);	

KERNEL_EXPORT void ToggleFullScreen();

KERNEL_EXPORT void EnableWeather(BOOL bEnable);

KERNEL_EXPORT void SetWeather(COM::CWeather::ATTRIBUTE attribute,COM::CWeather::INTENSION intension,DWORD d);

KERNEL_EXPORT void EnableRamble(bool bEnable);

KERNEL_EXPORT void ChangeManip();

//��ȡ��ǰѡ�е��׸�ID
KERNEL_EXPORT LONG64 GetSelObjID();
//��ȡ��ǰѡ�е�ID��
KERNEL_EXPORT std::vector<LONG64> GetSelObjIDs();

//������Ⱦģʽ
KERNEL_EXPORT void SetStereo(osg::DisplaySettings::StereoMode stereoMode, bool ONOFF);


//���ÿ�ѡ����
KERNEL_EXPORT void SetRegionSwitch(bool switchs);
//�����ȡλ���¼�
KERNEL_EXPORT void SetClickMessageSwitch(bool switchs);
//�������ģ�Ϳ���
KERNEL_EXPORT void SetAddModelSwitch(bool switchs);
KERNEL_EXPORT void setAddModelFile(std::string modelFile);
KERNEL_EXPORT void drawPiple();
KERNEL_EXPORT void deleteManual();

//�������ϵͳ
KERNEL_EXPORT void AddParticle(CString id,CString modeID,CString particleStype);
KERNEL_EXPORT void AddParticleCoordinate(CString id,osg::Vec3 coordinate,CString particleStype,CString strParam);
//ɾ������ϵͳ
KERNEL_EXPORT void DeleteParticle(CString id);

//�������濪��
KERNEL_EXPORT void SetProfileManagerSwitch(bool switchs);

//���õ��¿���
KERNEL_EXPORT void SetEmbankmentManagerSwitch(bool switchs);

//��ʼ�ܵ����� animationState 1 ��ʾ������ʼ 2 ��ʾ�������� 
KERNEL_EXPORT void PipleLineAnimationSwitch(AnimationPipleLineModeDB aplm,AnimationMode animationState);


KERNEL_EXPORT EMObj::CObj* PtSelectTest(double x, double y);

KERNEL_EXPORT  bool GetRay(float x, float y, osg::Vec3d& eye, osg::Vec3d& ray);

//��ȡ�����Ӧ������
KERNEL_EXPORT double GetCameraScope(void);

//ץͼ
KERNEL_EXPORT bool Screenshot(int w,int h,std::string imagePathName);

//���ݿ���������λ��
KERNEL_EXPORT void SetCameraScope(DOUBLE verticalDist);

KERNEL_EXPORT void SetDecorasShowOrNot(CString strDecorate,bool bShow = true);

KERNEL_EXPORT void BootOrPause3DRender(bool bPause = true);

KERNEL_EXPORT void ImportShpLineManager(std::string sqlLine);

KERNEL_EXPORT void ClearPiple();
//============zhangqiang============
//�Ӹ����������ڳ�ʼ�����ڱ���
KERNEL_EXPORT BOOL ShowPlayCtrl(CString title);

//����һ�����������Ȩ�����г����IP�б�
extern KERNEL_EXPORT std::vector<std::string> HardDiskIDList;

extern KERNEL_EXPORT CCmdManager g_CmdManager;


KERNEL_EXPORT void ShowRambleSettingDlg(bool bShow);

//
KERNEL_EXPORT void CameraAnimation();

class KERNEL_EXPORT CBootScreen
{
public:
	CBootScreen();
	virtual ~CBootScreen();

	/*
	Ŀ�ģ���ʼ��
	�������ļ�����
	ResType			(in)		��Դ���ͣ�1��ʾ���أ�2��ʾ����
	Para1			(in)		������Դʱ��ʾ·����������Դ�Ǳ�ʾUrl
	Para2			(in)		������Դʱ��Ч��������Դ�Ǳ�ʾ���ػ���Ŀ¼
	*/
	void Init(CDC *pDc, DWORD ResType, CString Para1 = _T(""), CString Para2 = _T(""));

	/*
	Ŀ�ģ������ַ���Ϣ
	���أ�����true. ��ʾ��ʼ�����
	*/
	bool HandleMsg(const CString &szMsg, CRect &rt);

	/*
	Ŀ�ģ����ƻ���
	*/
	void OnDraw(CDC *pDc, CRect rt);

 	enum ERR_LEVEL {
 		LEVEL_NORMAL,
 		LEVEL_WARNING,
 		LEVEL_ERROR
 	};

protected:
	//WM_COPYDATA
	//
	CDC		m_backGroundDC;		//����ͼƬ
	CDC		m_initDC;			//�ѳ�ʼ������ͼƬ
	CDC		m_uninitDC;			//δ��ʼ������ͼƬ
	CSize	m_backGroundSize;	//����ͼƬ��С
	CSize	m_initSize;			//�ѳ�ʼ������ͼƬ��С
	CSize	m_uninitSize;		//δ��ʼ������ͼƬ��С
	int		m_process;			//����ֵ
	CString	m_szMsg;			//��Ϣ
	ERR_LEVEL m_errLevel;		//����ȼ�
};

}
