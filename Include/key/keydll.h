
#ifndef _KEY_DLL_IMPORT_H
#define _KEY_DLL_IMPORT_H

#pragma  comment(lib, "Key.lib") 

//Ӧ�ó�������ʱ���;
//�ɹ� true
//���� false
//pszPath   ע���·��
extern "C" __declspec(dllimport) BOOL CheckForStartup( LPCSTR pszPath);
 
//ע��
//����ֵ����0�ɹ����룬 С��0����ʧ�ܣ�;
//����Ϊ���ô˺����ĳ����е�ĳһ���ʴ��ھ��;

//	pszPath   ע���·��
//	hWnd      ���ھ��
extern "C" __declspec(dllimport) int   ImportKey( LPCSTR pszPath, HWND hWnd);

 
//�Ƿ�������Ч;
extern "C" __declspec(dllexport)  BOOL   IsForever();

//�Ƿ��Ѿ�����;
extern "C"  __declspec(dllexport) BOOL   IsActived();  
 
//������ǰ��ע���룬Ψһ��ʶ
extern "C" _declspec(dllexport) void ShowIniInfo();

#endif