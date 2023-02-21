/***************************************************
Copyright:       (C),
File name:       CFontsApi.h
Author:          ’≈∆Ω    Version:   1.0     Date: 2009-12-02
Description:     ◊÷ÃÂAPI
Others:          
Function List:   
History:        
*************************************************/
#pragma once
#include <COM\Export.h>
#include <map>
namespace COM	{

class COM_EXPORT CFontsApi
{
public:
	CFontsApi(void);
	~CFontsApi(void);

	static bool GetAllFonts(std::map<CString, CString> &Fonts);
};
}