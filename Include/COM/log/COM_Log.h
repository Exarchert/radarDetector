/******************************************
File name:			CCOM_Log.h
Author:				刘广慧    
Date:				2009-06-01
Description:		日志生成类
*******************************************/
#pragma once;

#include <COM\Export.h>
#include <iostream>


namespace COM
{

	const char LOG_ERROR[3][10] = { "正常","警告","错误"}; 

	//信息等级枚举类型，在插入新的等级信息时应插入在LEVEL_COMMON 和LEVEL_ERROR之间
	typedef enum
	{
		LEVEL_COMMON = 0, /* 正常信息 */
		LEVEL_WARNING,    /* 警告信息 */
		LEVEL_ERROR       /* 错误信息 */
	}ERROR_LEVEL;

#define WRITE_TO_LOG_PTR CCOM_Log::GetInstance()       //定义宏常量获取一个CLog类指针
#define LOG_RELEASE	CCOM_Log::Release()       //释放CLog类指针


	class COM_EXPORT CCOM_Log
	{
	private:
		CCOM_Log();
		~CCOM_Log();
	public:
		static CCOM_Log* GetInstance(void) // 获得日志类的实例
		{
			if( !m_log )
				m_log = new CCOM_Log();
			return m_log;
		}
		static void Release();
		static CCOM_Log* m_log;

		char* m_BinPath;	

		
		/*****************************************************
		WriteToLog日志打印接口函数
		elLevel			in		ERROR_LEVEL			错误等级		
		取值	LEVEL_COMMON		正常信息 
		LEVEL_WARNING		警告信息
		LEVEL_ERROR			错误信息	
		szFuncName		in		char*				模块调用信息
		szMsg			in		char*				待标记信息			
		成功返回0，否则返回-1；
		******************************************************/
		int  WriteToLog(ERROR_LEVEL elLevel,const char* szFuncName, const char* szMsg);    // 接口函数
		int  WriteToLog(ERROR_LEVEL elLevel,CString szFuncName, const char* szMsg);    // 接口函数
		int  WriteToLog(ERROR_LEVEL elLevel,const char* szFuncName, CString szMsg);// 接口函数
		int  WriteToLog(ERROR_LEVEL elLevel,CString szFuncName, CString szMsg) ;// 接口函数

		/*****************************************************
		Description: Set Config file Path to Remove Log File
		******************************************************/
		void SetIniPath(CString strINI);
	private:
		char* GetSysDateInfo(void);		                   // 获得当前系统时间信息
		/*****************************************************
		将日志信息输入至当前目录下的.log文件内
		iLevel			 in    ERROR_LEVEL		错误等级
		szMsg			 in    char*			待输入信息
		成功返回0，否则返回-1；
		******************************************************/
		int  PrintLog(const char* szMsg, ERROR_LEVEL iLevel);       // 打印输出信息

		/*
		获取当前工程路径
		*/
		char* GetBinPath();

		/*
		获取当前路径下Log文件的个数
		*/
		int  getLogFileCount(void);									

		/*
		根据创建天数(每月按30天计算)移除Log文件
		*/
		void RemoveLogFileDependsDays();
		
		////CString转char*
		//char* ConvertToChar(const CString &s);
		std::string ConvertToString(const CString &s);
		
	private:
		ERROR_LEVEL  m_iLevel;                            //获取到的信息级别	
		bool		 m_bRemoved;						  //日志清除标记
		int          m_msgLen;                            //欲输出信息长度
		int          m_LogCount;                          //日志计数器
		CString		 m_IniPath;							  //配置文件路径
	};
}