// RadarCurve.cpp : ���� DLL �ĳ�ʼ�����̡�
//

#include "stdafx.h"
#include "RadarCurve.h"
#include <string>
#include <time.h>

#include <RadarCurve/RadarManager.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//
//TODO: ����� DLL ����� MFC DLL �Ƕ�̬���ӵģ�
//		��Ӵ� DLL �������κε���
//		MFC �ĺ������뽫 AFX_MANAGE_STATE ����ӵ�
//		�ú�������ǰ�档
//
//		����:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// �˴�Ϊ��ͨ������
//		}
//
//		�˺������κ� MFC ����
//		������ÿ��������ʮ����Ҫ������ζ��
//		��������Ϊ�����еĵ�һ�����
//		���֣������������ж������������
//		������Ϊ���ǵĹ��캯���������� MFC
//		DLL ���á�
//
//		�й�������ϸ��Ϣ��
//		����� MFC ����˵�� 33 �� 58��
//


// CRadarCurveApp

BEGIN_MESSAGE_MAP(CRadarCurveApp, CWinApp)
END_MESSAGE_MAP()


// CRadarCurveApp ����

CRadarCurveApp::CRadarCurveApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}

// Ψһ��һ�� CRadarCurveApp ����

CRadarCurveApp theApp;


// CRadarCurveApp ��ʼ��

BOOL CRadarCurveApp::InitInstance()
{
	srand( time(NULL));
	CWinApp::InitInstance();
	afxAmbientActCtx = FALSE;

	return TRUE;
}
int CRadarCurveApp::ExitInstance()
{
	RadarManager::Instance()->stopWork();
	//RadarManager::Instance()->clear();
	RadarManager::Destory();

	return CWinApp::ExitInstance();
}