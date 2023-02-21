/**************************************************************************
File:			COM_LICircle.h
Author:			张平    
Date:			2009-10-28
Description:    圆形配置LOD
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
	通过XML数据进行数据初始化
	*/
	bool Init(MSXML2::IXMLDOMNodePtr pNode);

	virtual DWORD GetLodType()
	{
		return LOD_Height;
	}	
};

}