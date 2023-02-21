#pragma once
#include <COM\Export.h>
#include <osgAPEx\Earthmanipulator>
#include <COM\Ramble\RambleMgr.h>

namespace COM
{
	struct CoordAndPos
	{
		float	_Yaw;
		float	_Pitch;
		float	_Roll;
		double	_Lon;
		double	_Lat;
		double	_Alt;	
	};

	/*****************************************************
	Author:	LiuGh
	Function Purpose: F10键获取坐标姿态数据信息到剪切板
	Data:	09-12-14
	*****************************************************/
	COM_EXPORT BOOL SetPoseAndPosToClipBoard(COM::CRambleMgr* pEarthManip);
}