/*************************************************
file:			Main.h
Author:			��ƽ    
Date:			2009-11-18
Description:    ��������
*************************************************/
#pragma once
#include <Kernel\Export.h>
#include <Com\KernelContext\KernelContext.h>
#include <vector>
#include <Object\Obj.h>
#include <osg\ref_ptr>
#include <kernel\init\InitModule.h>

namespace Kernel
{

class KERNEL_EXPORT CMain
{
public:
	CMain(void);
	~CMain(void);

	//�������ļ�
	void LoadSysConfigFile(CString strFile);

	//�ں�������
	osg::ref_ptr<COM::CKernelContext>	m_kContext;

	//Ӧ�ó���·��
	CString								m_appPath;//ִ�г���exe����λ��

	//�����ļ�·��
	CString								m_SysConfigFilePath;//�����ļ�ȫ·��

	//��Ŀ·��
	CString								m_ProPath;//��¼�����ļ�sysconfig.ini ����λ��

	//���ݿ���������
	CString								m_DBPassWord;

	std::vector<bool>                   m_isLoadEnter;
	////ѡ�ж����б�
	//std::vector<osg::ref_ptr<EMObj::CObj>> m_SelObjs;


	std::vector<osg::ref_ptr<CInitModule>>	m_AppInitModules;	//Ӧ�ù��̳�ʼ������

};

}