// RadarCurve.h : RadarCurve DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CRadarCurveApp
// �йش���ʵ�ֵ���Ϣ������� RadarCurve.cpp
//

class CRadarCurveApp : public CWinApp
{
public:
	CRadarCurveApp();

// ��д
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance(); // return app exit code

	DECLARE_MESSAGE_MAP()
};
