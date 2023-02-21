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

	// ���������Ƿ��й�
	bool IsHaveDog();

	// ��鲢ע��
	bool CheckRegister(char szError[100]);

// ���º�����ɼ��ܹ���ע������Ҫ����Ϣ�����ã������ڵ���ע���⺯��ǰ���� //////////////////////
	// �����ڽ�ע��Ի���ĸ�����
	void SetParentWnd(HWND hParent);

	// ����ע��ص�
	void SetUserRegisterOption(UserRegisterFuncPtr func, bool useBuiltInMessageBox);

	// ���ò�Ʒ����
	void SetProductName(const char* product);

	// ���ü��ܹ�PID
	void SetDogPID(unsigned char pid[8]);

	// ���ü��ܹ�UserPIN
	void SetDogUserPIN(unsigned char userpin[16]);

private:
	Register();

	// ����д��8�ֽڵ�ע����
	bool ReadDog(unsigned char data[8]);
	// �ӹ��ж�ȡ8�ֽڵ�ע����
	bool WriteDog(unsigned char data[8]);

	// ��Ӳ����Ϣ����������
	bool GenRequirementCode(unsigned char data[8]);

	// ��ע����ж�ȡ������
	bool GetRequirementCodeFromRegistry(unsigned char data[8]);
	// ��ע����ж�ȡע����
	bool GetSNFromRegistry(unsigned char data[8]);
	// ��ע���д���������ע����
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
