/****************************************************************
FileName:		CZip_Application.h
Author:			LiuGH
Date:			2010.06.02
Description:	Using Zip And UnZip To Compress or Extract 
a file or folder
Version:		1.0
----------------------------------------------------------------

Modify history:

*****************************************************************/
#pragma once

#include <com/ZipCompress/zip.h>
#include <com/ZipCompress/unzip.h>
#include <COM\Export.h>
#include <map>
#include <vector>


namespace COM
{
	class COM_EXPORT CZip_Application
	{

	public:
		CZip_Application(void);
		CZip_Application(HWND pWnd);
		~CZip_Application(void);
	public:
		struct zipData 
		{
			CString _zipFolder;
			CString _zipName;
		};
		struct unzipData
		{
			CString _unzipFolder;
			CString _unzipFile;
		};
	public:

		/*
		Switch One of Init function or SetParaForZip / SetParaForExtract and then call RunZipping;
		*/
		bool Init(std::map<int,zipData>& zipMap,std::map<int,unzipData>& unzipMap,IN bool bRemoveSrc = false, IN bool bCreate = true, IN const CString password = _T(""));

		/*
		Description:	set parameter for zip or extract
		*/
		void SetParaForZip(IN const CString strSrcFile,IN const CString strDestFile,IN const CString password = _T(""));
		void SetParaForExtract(IN const CString strSrcZipFile,IN const CString strDestFolder,IN bool bRemoveSrc = false, IN bool bCreate = true, IN const CString password = _T(""));

		/*
		Description:	Interface for user to start zipping or unzipping thread,
		if call this function,you should call SetParaForZip or SetParaForExtract
		for parameter,else the thread do nothing
		*/
		void RunZipping();

		/*
		Description:	Compress file or folder
		Parameter:		strSrcFile:		file or folder Path for compressing
		strDestFile:	full path except extend name,the 
		application will add ".cpr" to the end 
		of file path
		password:		give a password for extracting
		*/
		bool ZipCompress(IN const CString strSrcFile,IN const CString strDestFile,IN const CString password = _T(""));

		/*
		Description:	Compress file or folder
		Parameter:		strSrcFile:		full file path must have ".cpr" extend name	
		strDestFile:	folder Path to extract
		password:		give a password which is set by compressed
		*/
		bool ZipExtract(IN const CString strSrcZipFile,IN CString strDestFolder,IN bool bRemoveSrc = false,IN bool bCreate = true,IN const CString password = _T(""));

		/*
		Description:	set called handle
		*/
		void SetHandle(HWND hWnd){m_hWnd = hWnd;}

		/*
		Description:
		*/
		void SetZipOrUnZipState(bool bStop){m_bStopZipOrUnZip = bStop;}

		bool ThreadIsRunning(){return m_bTreadRuning;}

		//show progress bar using zip_msg,and user must implement function to process zip_msg
		enum Zip_Msg
		{
			WM_UPDATE_PROGRESS		=	WM_USER + 0x00100,
			WM_UPDATE_PROGRESS_ALL	=	WM_USER + 0x00101
		};
	private:
		/*
		Description:	start or stop a new thread for zip or extract
		*/
		static void StartZipThread(void* ptr);
		void StopZipThread(HANDLE& hThread);

		/*
		Descirption£º		create multiple directory
		Parameter£º			dirPath
		Return value:		return true if success else return false
		Author:				LiuGh
		Data:				2010.4.26
		*/

		bool CreateMultipleDirectory(IN const CString& dirPath);

		/*
		Descirption£º		add file to zip which included in browser folder
		Parameter£º			source file
		Return value:		void
		Author:				LiuGh
		Data:				2010.4.26
		*/
		void AddFileToZipByBrowserFolder(IN CString strFolder);

		/*
		Translate Zip or UnZip return value
		*/
		void TranslateZipMessage(ZRESULT zr);
		void TranslateUnZipMessage(ZRESULT zr);

		/*
		description:	get a specified folder size
		*/
		DWORD GetFolderSize(IN CString strFolder);

	private:
		HZIP	m_hZip;
		ZRESULT m_ZipRet;

		bool	m_bRemoveZipFile;
		bool	m_bCreateNewEmptyFile;
		CString m_Ext;
		CString m_PassWord;
		CString m_CprssFile;
		CString m_FullPathToCpr;
		CString m_FldNameToCpr;
		DWORD m_zippedSize;
		DWORD m_fldSize;
		DWORD m_fileSize;
	private:
		enum ZIPMODE
		{	
			NONE,
			ZIP,
			UNZIP
		};

		HWND	m_hWnd;
		bool	m_bTreadRuning;	
		bool	m_bStopZipOrUnZip;
		ZIPMODE m_ZipMode;

		std::map<int,zipData>	m_ZipMap;
		std::map<int,unzipData> m_UnZipMap;
	};
}

