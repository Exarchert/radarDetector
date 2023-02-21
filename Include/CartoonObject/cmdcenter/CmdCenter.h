/*************************************************
file:			CmdCenter.h
Author:			张平    
Date:			2009-12-08
Description:    命令中心

*************************************************/
#pragma once
#include <CartoonObject\Export.h>
#include <COM\Command\Cmd.h>
namespace CartoonObject{
	
	//热区编辑
	CARTOONOBJECT_EXPORT void CmdBeginEditHotArea();

	//结束热区编辑
	CARTOONOBJECT_EXPORT void CmdEndingHotAreaEdit();

	//检测热区
	CARTOONOBJECT_EXPORT void CmdDetectionHotArea(CString string);

	//结束检测热区
	CARTOONOBJECT_EXPORT void CmdEndingDetectionHotArea();

	CARTOONOBJECT_EXPORT void CmdBeginMark();
}
