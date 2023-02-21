
#ifndef _KEY_DLL_IMPORT_H
#define _KEY_DLL_IMPORT_H

#pragma  comment(lib, "Key.lib") 

//应用程序启动时检查;
//成功 true
//错误 false
//pszPath   注册表路径
extern "C" __declspec(dllimport) BOOL CheckForStartup( LPCSTR pszPath);
 
//注册
//返回值大于0成功导入， 小于0导入失败！;
//参数为调用此函数的程序中的某一合适窗口句柄;

//	pszPath   注册表路径
//	hWnd      窗口句柄
extern "C" __declspec(dllimport) int   ImportKey( LPCSTR pszPath, HWND hWnd);

 
//是否永久有效;
extern "C" __declspec(dllexport)  BOOL   IsForever();

//是否已经激活;
extern "C"  __declspec(dllexport) BOOL   IsActived();  
 
//弹出当前的注册码，唯一标识
extern "C" _declspec(dllexport) void ShowIniInfo();

#endif