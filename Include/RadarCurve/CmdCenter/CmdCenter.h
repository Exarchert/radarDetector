/*************************************************
file:			CmdCenter.h
Author:			yg    
Date:			2016��7��4
Description:    ��������

*************************************************/
#pragma once
#include "RadarCurve/export.h"

namespace Radar
{
	RADAR_EXPORT void CmdRadarManagerInit();
	/*
		��ʼ�״﹤��
	*/
	RADAR_EXPORT void CmdStartRadarWorkDlg();

	/*
		ֹͣ�״﹤��
	*/
	RADAR_EXPORT void CmdEndRadarWorkDlg();

	/*
		�״�ʵʱ����
	*/
	RADAR_EXPORT void CmdRadarRealTimeCurves();


	/*
		�״���ʷ����
	*/
	RADAR_EXPORT void CmdRadarHistoryCurves();

	/*
		GPSʵʱ��λ
	*/
	RADAR_EXPORT void CmdGPSRealTimeLocate();

	/*
		�״��������
	*/
	RADAR_EXPORT void CmdRadarParametersSettings();

	/*
		�״����ݴ���
	*/
	RADAR_EXPORT void CmdRadarDataProcess();

	/*
		�״����ݱ�����ʾ
	*/
	RADAR_EXPORT void CmdRadarDataFormsShow();

	/*
		�����״﹤��
	*/
	RADAR_EXPORT void CmdExportRadarProject(CString strPath,CString strPROJECTCREATETIME);

	/*
		�����״﹤��
	*/
	RADAR_EXPORT CString CmdImportRadarProject(CString strFileAllPath);
	
	/*
		ɾ���״﹤��
	*/
	RADAR_EXPORT bool CmdDeleteRadarProject(CString strPROJECTCREATETIME);
	
	/*
		��Ƶ����
	*/
	RADAR_EXPORT void CmdVideoPlay();

	/*
		��Ƶ�ط�
	*/
	RADAR_EXPORT void CmdVideoPlayback();
	
	/*
		ѡ������GPR
	*/
	RADAR_EXPORT void SelectCreateGPR();
}
