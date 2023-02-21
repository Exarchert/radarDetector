/*********************************************************************
Copyright (C),  
�ļ���InitManipulator.h InitManipulator.cpp
���ߣ���־��
�汾��1.0
���ڣ�2009-11-13
��������ʼ�����������.

�޸ļ�¼��
����					����				����
----------------------------------------------------------------------

*********************************************************************/

#pragma once

#include <Kernel\Export.h>
#include <Kernel\Init\InitModule.h>


namespace	Kernel
{

class CCameraPlacement 
{
public:
	CCameraPlacement();
	CCameraPlacement(int key, double lon, double lat, double height, float yaw, float pitch, float roll);

public:
	int		m_key;
	double	m_lon;
	double	m_lat;
	double	m_height;
	float	m_yaw;
	float	m_pitch;
	float	m_roll;
};

class CInitManipulator : public CInitModule
{
public:
	CInitManipulator();
	virtual ~CInitManipulator();

	/*
	Ŀ�ģ�ִ�г�ʼ��
	���أ��ɹ�����TRUE�����򷵻�FALSE
	*/
	virtual BOOL InitInstance(COM::CKernelContext &kContext);

	/*
	Ŀ�ģ��ͷų�ʼ��
	*/
	virtual BOOL UnInitInstance(COM::CKernelContext &kContext);

	/*
	Ŀ�ģ���ʼ����������
	*/
	virtual void InitEnviroment();

protected:
	CCameraPlacement				m_default;
	std::vector<CCameraPlacement>	m_places;
};

}
