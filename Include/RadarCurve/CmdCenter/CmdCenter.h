/*************************************************
file:			CmdCenter.h
Author:			yg    
Date:			2016年7月4
Description:    命令中心

*************************************************/
#pragma once
#include "RadarCurve/export.h"

namespace Radar
{
	RADAR_EXPORT void CmdRadarManagerInit();
	/*
		开始雷达工程
	*/
	RADAR_EXPORT void CmdStartRadarWorkDlg();

	/*
		停止雷达工程
	*/
	RADAR_EXPORT void CmdEndRadarWorkDlg();

	/*
		雷达实时曲线
	*/
	RADAR_EXPORT void CmdRadarRealTimeCurves();


	/*
		雷达历史曲线
	*/
	RADAR_EXPORT void CmdRadarHistoryCurves();

	/*
		GPS实时定位
	*/
	RADAR_EXPORT void CmdGPSRealTimeLocate();

	/*
		雷达参数设置
	*/
	RADAR_EXPORT void CmdRadarParametersSettings();

	/*
		雷达数据处理
	*/
	RADAR_EXPORT void CmdRadarDataProcess();

	/*
		雷达数据报表显示
	*/
	RADAR_EXPORT void CmdRadarDataFormsShow();

	/*
		导出雷达工程
	*/
	RADAR_EXPORT void CmdExportRadarProject(CString strPath,CString strPROJECTCREATETIME);

	/*
		导入雷达工程
	*/
	RADAR_EXPORT CString CmdImportRadarProject(CString strFileAllPath);
	
	/*
		删除雷达工程
	*/
	RADAR_EXPORT bool CmdDeleteRadarProject(CString strPROJECTCREATETIME);
	
	/*
		视频播放
	*/
	RADAR_EXPORT void CmdVideoPlay();

	/*
		视频回放
	*/
	RADAR_EXPORT void CmdVideoPlayback();
	
	/*
		选择生成GPR
	*/
	RADAR_EXPORT void SelectCreateGPR();
}
