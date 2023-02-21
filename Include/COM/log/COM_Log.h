/******************************************
File name:			CCOM_Log.h
Author:				�����    
Date:				2009-06-01
Description:		��־������
*******************************************/
#pragma once;

#include <COM\Export.h>
#include <iostream>


namespace COM
{

	const char LOG_ERROR[3][10] = { "����","����","����"}; 

	//��Ϣ�ȼ�ö�����ͣ��ڲ����µĵȼ���ϢʱӦ������LEVEL_COMMON ��LEVEL_ERROR֮��
	typedef enum
	{
		LEVEL_COMMON = 0, /* ������Ϣ */
		LEVEL_WARNING,    /* ������Ϣ */
		LEVEL_ERROR       /* ������Ϣ */
	}ERROR_LEVEL;

#define WRITE_TO_LOG_PTR CCOM_Log::GetInstance()       //����곣����ȡһ��CLog��ָ��
#define LOG_RELEASE	CCOM_Log::Release()       //�ͷ�CLog��ָ��


	class COM_EXPORT CCOM_Log
	{
	private:
		CCOM_Log();
		~CCOM_Log();
	public:
		static CCOM_Log* GetInstance(void) // �����־���ʵ��
		{
			if( !m_log )
				m_log = new CCOM_Log();
			return m_log;
		}
		static void Release();
		static CCOM_Log* m_log;

		char* m_BinPath;	

		
		/*****************************************************
		WriteToLog��־��ӡ�ӿں���
		elLevel			in		ERROR_LEVEL			����ȼ�		
		ȡֵ	LEVEL_COMMON		������Ϣ 
		LEVEL_WARNING		������Ϣ
		LEVEL_ERROR			������Ϣ	
		szFuncName		in		char*				ģ�������Ϣ
		szMsg			in		char*				�������Ϣ			
		�ɹ�����0�����򷵻�-1��
		******************************************************/
		int  WriteToLog(ERROR_LEVEL elLevel,const char* szFuncName, const char* szMsg);    // �ӿں���
		int  WriteToLog(ERROR_LEVEL elLevel,CString szFuncName, const char* szMsg);    // �ӿں���
		int  WriteToLog(ERROR_LEVEL elLevel,const char* szFuncName, CString szMsg);// �ӿں���
		int  WriteToLog(ERROR_LEVEL elLevel,CString szFuncName, CString szMsg) ;// �ӿں���

		/*****************************************************
		Description: Set Config file Path to Remove Log File
		******************************************************/
		void SetIniPath(CString strINI);
	private:
		char* GetSysDateInfo(void);		                   // ��õ�ǰϵͳʱ����Ϣ
		/*****************************************************
		����־��Ϣ��������ǰĿ¼�µ�.log�ļ���
		iLevel			 in    ERROR_LEVEL		����ȼ�
		szMsg			 in    char*			��������Ϣ
		�ɹ�����0�����򷵻�-1��
		******************************************************/
		int  PrintLog(const char* szMsg, ERROR_LEVEL iLevel);       // ��ӡ�����Ϣ

		/*
		��ȡ��ǰ����·��
		*/
		char* GetBinPath();

		/*
		��ȡ��ǰ·����Log�ļ��ĸ���
		*/
		int  getLogFileCount(void);									

		/*
		���ݴ�������(ÿ�°�30�����)�Ƴ�Log�ļ�
		*/
		void RemoveLogFileDependsDays();
		
		////CStringתchar*
		//char* ConvertToChar(const CString &s);
		std::string ConvertToString(const CString &s);
		
	private:
		ERROR_LEVEL  m_iLevel;                            //��ȡ������Ϣ����	
		bool		 m_bRemoved;						  //��־������
		int          m_msgLen;                            //�������Ϣ����
		int          m_LogCount;                          //��־������
		CString		 m_IniPath;							  //�����ļ�·��
	};
}