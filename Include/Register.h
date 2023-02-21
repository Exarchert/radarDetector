#pragma once
#include <Windows.h>
#include <string>

#ifdef REGISTER_EXPORTS
#define REGISTER_API __declspec(dllexport)
#else
#define REGISTER_API __declspec(dllimport)
#endif

typedef bool (*UserRegisterFuncPtr)(char szRequirementCode[20], char szSN[20]);

class REGISTER_API Register
{
public:
	~Register();

	static Register* Instance();
	static void Release();

	// 检查机器上是否有狗
	bool IsHaveDog();

	// 检查并注册
	bool CheckRegister(char szError[100]);

// 以下函数完成加密狗及注册中需要的信息的设置，必须在调用注册检测函数前调用 //////////////////////
	// 设置内建注册对话框的父窗口
	void SetParentWnd(HWND hParent);

	// 设置注册回调
	void SetUserRegisterOption(UserRegisterFuncPtr func, bool useBuiltInMessageBox);

	// 设置产品名称
	void SetProductName(const char* product);

	// 设置加密狗PID
	void SetDogPID(unsigned char pid[8]);

	// 设置加密狗UserPIN
	void SetDogUserPIN(unsigned char userpin[16]);

private:
	Register();

	// 向狗中写入8字节的注册码
	bool ReadDog(unsigned char data[8]);
	// 从狗中读取8字节的注册码
	bool WriteDog(unsigned char data[8]);

	// 由硬件信息生成需求码
	bool GenRequirementCode(unsigned char data[8]);

	// 从注册表中读取需求码
	bool GetRequirementCodeFromRegistry(unsigned char data[8]);
	// 从注册表中读取注册码
	bool GetSNFromRegistry(unsigned char data[8]);
	// 向注册表写入需求码和注册码
	bool WriteRegistry(unsigned char requirementCode[8], unsigned char SN[8]);

	bool BuiltInRegisterDialog();

private:
	HWND	m_hParent;

	UserRegisterFuncPtr	m_userRegisterFuncPtr;
	bool				m_useBuiltInMessageBox;

	std::string			m_ProductName;
	unsigned char*		m_PID;
	unsigned char*		m_UserPIN;
};
