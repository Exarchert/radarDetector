/***************************************************
Copyright:       (C),
File name:       CFontsApi.h
Author:          ��ƽ    Version:   1.0     Date: 2009-12-02
Description:     ����API
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

	//// ����򿪺͹ر�ע���ĳ�Ա������
	LONG Open(HKEY hKeyRoot, LPCTSTR pszPath);
	void Close();

	// ���ú�������ʵ�ֶ�ע����ֵ����ֵ��������ֵ��DWORDֵ ���Ķ���д��

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