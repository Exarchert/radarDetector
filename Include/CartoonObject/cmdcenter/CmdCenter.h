/*************************************************
file:			CmdCenter.h
Author:			��ƽ    
Date:			2009-12-08
Description:    ��������

*************************************************/
#pragma once
#include <CartoonObject\Export.h>
#include <COM\Command\Cmd.h>
namespace CartoonObject{
	
	//�����༭
	CARTOONOBJECT_EXPORT void CmdBeginEditHotArea();

	//���������༭
	CARTOONOBJECT_EXPORT void CmdEndingHotAreaEdit();

	//�������
	CARTOONOBJECT_EXPORT void CmdDetectionHotArea(CString string);

	//�����������
	CARTOONOBJECT_EXPORT void CmdEndingDetectionHotArea();

	CARTOONOBJECT_EXPORT void CmdBeginMark();
}
