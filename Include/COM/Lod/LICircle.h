/**************************************************************************
File:			COM_LICircle.h
Author:			��ƽ    
Date:			2009-10-28
Description:    Բ������LOD
***************************************************************************/

#pragma once
#include <COM\Lod\LodItem.h>
#import   "msxml3.dll"

namespace COM{

class COM_EXPORT CLICircle : public CLodItem
{
public:
	CLICircle(void);
	~CLICircle(void);

	/*
	ͨ��XML���ݽ������ݳ�ʼ��
	*/
	bool Init(MSXML2::IXMLDOMNodePtr pNode);

	virtual DWORD GetLodType()
	{
		return LOD_Height;
	}	
};

}