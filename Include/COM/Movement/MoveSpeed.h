/************************************************************************
�ļ�:			MoveSpeed.h
����:			��ƽ    
��������:		2009-03-16
����:			�˶��������е��ٶ��࣬��Ҫ�������˶��е��ٶ�(����)

�޸ļ�¼��
����					����					����
-------------------------------------------------------------------------

************************************************************************/

#pragma once

#include <Osg/Referenced>
#include <Osg/Vec3>
#include <Osg/Vec2>
#include <COM\XML\XMLNode.h>
#include <COM\XML\XMLApi.h>
#include <osg\ref_ptr>

namespace COM{

enum{
	ENM_MoveSpeedType_UnDefault = 0,
	ENM_MoveSpeedType_Uniform,
	ENM_MoveSpeedType_Uniform_Angle,
	ENM_MoveSpeedType_Uniform_Angle_Both,
	ENM_MoveSpeedType_Uniform_Angle_Three,
};

//�˶��ٶȻ���
class COM_EXPORT CMoveSpeed : public CXMLNode, public osg::Referenced
{
public:
	virtual ~CMoveSpeed(void)
	{
	}

	virtual CMoveSpeed * Clone()
	{
		return NULL;
	}

	/*
	Ŀ�ģ�����ĳһʱ�̵��ٶ�
	������time		(in)		ʱ��
		  speed		(out)		�����ٶ�ֵ
	���أ��ɹ�����true��speed��Ч��������Ч
	*/
	virtual bool GetSpeed(double time, double &speed)
	{
		return false;
	}

	/*
	Ŀ�ģ����ش�0��ĳһʱ�����˶���·��
	������time		(in)		ʱ�̣���0��ʼ��
		 distance	(out)		����·��ֵ
	���أ��ɹ�����true��distance��Ч������distance��Ч
	*/
	virtual bool GetDistance(double time, double &distance)
	{
		return false;
	}

	DWORD GetType()
	{
		return m_Type;
	}

	virtual CString GetTypeName();

	CMoveSpeed(void)
	{
		m_Type = ENM_MoveSpeedType_UnDefault;
	}

protected:
	DWORD	m_Type;
	static CString m_sTypeName;
};


//�ȼ����˶�
class COM_EXPORT CMSUniform : public CMoveSpeed
{
public:
	virtual ~CMSUniform()
	{
	}

	CMoveSpeed * Clone()
	{
		return new CMSUniform(m_v, m_a);
	}

	/*
	Ŀ�ģ����캯��
	������v		(in)	���ٶ�
		  a		(in)	���ٶ�
	*/
	CMSUniform(double v, double a = 0)
	{
		m_Type = ENM_MoveSpeedType_Uniform;
		m_v = v;
		m_a = a;
	}

	/*
	Ŀ�ģ���ȡĳ��ʱ�̵��ٶ�ֵ
	������time		(in)	ʱ��
		  speed		(out)	�ٶ�
	���أ��ɹ�����true��speedֵ��Ч��������Ч
	*/
	bool GetSpeed(double time, double &speed)
	{
		speed = m_v + m_a * time;
		return true;
	}

	/*
	Ŀ�ģ����ش�0��ĳһʱ�����˶���·��
	������time		(in)		ʱ�̣���0��ʼ��
	      distance	(out)		����·��ֵ
	���أ��ɹ�����true��distance��Ч������distance��Ч
	*/
	bool GetDistance(double time, double &distance)
	{
		distance = (m_v + m_v + m_a * time) * time * 0.5f;
		return true;
	}

	CString GetTypeName();

	/*
	Ŀ�ģ�������ʼ���ʺͼ�����
	*/
	BOOL SetOriginSpeedAndAcceleration(double v, double a);
	BOOL GetOriginSpeedAndAcceleration(double &v, double &a);

	//XML��ȡ�ͱ���ģ��
	bool Init(MSXML2::IXMLDOMNodePtr pNode);
	MSXML2::IXMLDOMNodePtr GetXMLNode();

	CMSUniform()
	{
		m_Type = ENM_MoveSpeedType_Uniform;
		m_v = 0.f;
		m_a = 0.f;
	}

protected:
	double m_v;		//���ٶ�
	double m_a;		//������
	static CString m_sTypeName;
};

CMoveSpeed *MoveSpeedFactory(MSXML2::IXMLDOMNodePtr pNode);

}