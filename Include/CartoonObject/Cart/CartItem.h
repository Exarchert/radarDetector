/***************************************************************************
File:			CartItem.h
Author:			��ƽ    
Date:			2010-03-03
Description:    ����������Ԫ��

***************************************************************************/

#pragma once
#include <Osg/Referenced>
#include <Osg/Vec3>
#include <Osg/Vec2>
#include <Osg/ref_ptr>
#include <CartoonObject/Export.h>
#include <CartoonObject/Cart/CartAction.h>
#include <vector>

namespace CartoonObject{

//����������Ԫ��
class CARTOONOBJECT_EXPORT CCartItem : public osg::Referenced
{
public:
	CCartItem(void);
	virtual ~CCartItem(void);

	/*
	Ŀ�ģ���ʼ����������
	������StartTime		(in)	��������Ŀ�ʼ�ʱ��
		  TimeLenght	(in)	��������Ļʱ�䳤��
		  name			(in)	������������
	���أ�void
	*/
	void Init(CString &Name, double StartTime = 0, double TimeLength = 0);


	/*
	Ŀ�ģ���ȡ������Ԫ�Ļʱ�䳤��
	*/
	double GetTimeLength(){ return m_TimeLength; }

	void SetTimeLength(double timeLength) 
	{ 
		m_TimeLength = timeLength; 
	}


	/*
	Ŀ�ģ���ȡ����
	*/
	CString GetName()
	{
		return m_Name;
	}

	void SetName(CString &name)
	{
		m_Name = name;
	}

	/*
	Ŀ�ģ����ÿ�ʼ�ʱ��
	*/
	void SetStartTime(double startTime)
	{
		m_StartTime = startTime;
	}

	double GetStartTime()
	{
		return m_StartTime;
	}

	/*
	Ŀ�ģ�������Ԫִ����Ϊ
	������Time		(in)	����ڿ�ʼ�ʱ�̵�ʱ��
	���أ�void
	*/
	virtual void DoAction(double Time);

	/*
	Ŀ�ģ��������ִ�е���Ϊ
	������Time		(in)	����ڿ�ʼ�ʱ�̵�ʱ��
	���أ�void
	*/
	virtual void PlayFinish(double Time);

	/*
	Ŀ�ģ���������ʱ��
	������DeTime		(in)	ʱ��ı仯��
	*/
	virtual void Update(double DeTime);

	/*
	Ŀ�ģ�����׼��
	*/
	virtual void ReadyPlay();

	/*
	��ú����ö���ID
	*/
	void SetObjID(LONG64 ObjID){m_ObjID = ObjID;}
	LONG64 GetObjID(){return m_ObjID;}

protected:
	double m_PlayTime;							//������ʱ��
	double m_StartTime;							//��ʼ�ʱ��, ��m_PlayTimeʹ��ͬһ��ʱ����
	double m_PlayScale;							//�����ٶ�(����)
	DWORD m_PlayState;							//������Ԫ��״̬
	double m_TimeLength;						//�ʱ�䳤��
	CString m_Name;								//��ͨ����

	LONG64 m_ObjID;								//����ID

	//�����б�
	std::vector<osg::ref_ptr<CCartAction>> m_CartActions;

};
}