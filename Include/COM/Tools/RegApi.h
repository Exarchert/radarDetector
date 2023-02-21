/***************************************************
Copyright:       (C),
File name:       CFontsApi.h
Author:          张平    Version:   1.0     Date: 2009-12-02
Description:     字体API
Others:          
Function List:   
History:        
*************************************************/
#pragma once
#include <COM\Export.h>
namespace COM{
class COM_EXPORT CRegKey
{
	// Construction
public:
	CRegKey();
	virtual ~CRegKey();

	// Attributes
public:

	//// 定义打开和关闭注册表的成员函数：
	LONG Open(HKEY hKeyRoot, LPCTSTR pszPath);
	void Close();

	// 利用函数重载实现对注册表键值（串值，二进制值，DWORD值 ）的读和写：

	LONG Read(LPCTSTR pszKey,DWORD& dwVal);
	LONG Read(LPCTSTR pszKey,CString& sVal);
	LONG Read(LPCTSTR pszKey,BYTE *pData,DWORD& dwLength);

	LONG Write(LPCTSTR pszKey,DWORD dwVal);
	LONG Write(LPCTSTR pszKey,LPCTSTR pszVal);
	LONG Write(LPCTSTR pszKey,const BYTE *pData,DWORD dwLength);

protected:
	HKEY m_hKey;
	CString m_sPath;
};



}