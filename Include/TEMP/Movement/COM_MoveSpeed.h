/************************************************************************
�ļ�:			COM_MoveSpeed.h
����:			��ƽ    
��������:		2009-03-16
����:			�˶��������е��ٶ��࣬��Ҫ�������˶��е��ٶ�(����)

�޸ļ�¼��
����					����					����
-------------------------------------------------------------------------
��־��				2009-07-21			�޸�CCOM_MoveSpeed���MFC CObject��
										�̳У�ʹ֮����Serialize����.

************************************************************************/

#pragma once

#include <temp/Export.h>
#include <Osg/Referenced>
#include <Osg/Vec3>
#include <Osg/Vec2>


namespace TEMP	{

enum{
	ENM_MoveSpeedType_UnDefault = 0,
	ENM_MoveSpeedType_Uniform,
	ENM_MoveSpeedType_Uniform_Angle,
	ENM_MoveSpeedType_Uniform_Angle_Both,
	ENM_MoveSpeedType_Uniform_Angle_Three,
};

//�˶��ٶȻ���
class TEMP_EXPORT CCOM_MoveSpeed : public osg::Referenced, public CObject
{
public:
	virtual ~CCOM_MoveSpeed(void)
	{
	}

	virtual CCOM_MoveSpeed * Clone()
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

	/*
	Ŀ�ģ����л�����
	*/
	virtual void Serialize(CArchive& ar);

	virtual bool ParserXml(MSXML2::IXMLDOMNodePtr pNode);

protected:
	CCOM_MoveSpeed(void)
	{
		m_Type = ENM_MoveSpeedType_UnDefault;
	}

	DECLARE_SERIAL(CCOM_MoveSpeed)

protected:
	DWORD	m_Type;
	static CString m_sTypeName;
};


//�ȼ����˶�
class TEMP_EXPORT CCOM_MSUniform : public CCOM_MoveSpeed
{
public:
	virtual ~CCOM_MSUniform()
	{
	}

	CCOM_MoveSpeed * Clone()
	{
		return new CCOM_MSUniform(m_v, m_a);
	}

	/*
	Ŀ�ģ����캯��
	������v		(in)	���ٶ�
		  a		(in)	���ٶ�
	*/
	CCOM_MSUniform(double v, double a = 0)
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

	/*
	Ŀ�ģ����л�����
	*/
	virtual void Serialize(CArchive& ar);

	virtual bool ParserXml(MSXML2::IXMLDOMNodePtr pNode);

	CCOM_MSUniform()
	{
		m_Type = ENM_MoveSpeedType_Uniform;
		m_v = 0.f;
		m_a = 0.f;
	}

protected:
	DECLARE_SERIAL(CCOM_MSUniform)

protected:
	double m_v;		//���ٶ�
	double m_a;		//������
	static CString m_sTypeName;
};

}