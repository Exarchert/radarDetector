/***************************************************************************
File:			CartAction.h
Author:			��ƽ    
Date:			2010-01-21
Description:    ����������
***************************************************************************/

#pragma once
#include <CartoonObject/Export.h>
#include <Osg/Referenced>
#include <Osg/Vec3>
#include <Osg/Vec2>
#include <object/Obj.h>

namespace CartoonObject{

//����������Ԫ��
class CARTOONOBJECT_EXPORT CCartAction : public osg::Referenced
{
public:
	CCartAction(void);
	virtual ~CCartAction(void);

	enum{
		ENM_ActionType_Continuity = 0,			//�����Զ���
		ENM_ActionType_UnContinuity,			//�������Զ���
	};

	//��ȡ��������
	DWORD GetType();

	//���úͻ�ȡ������ʼʱ��
	void SetStartTime(double StartTime);
	double GetStartTime();

	//���úͻ�ȡʱ��
	void SetTimeLength(double TimeLength);
	double GetTimeLength();

	//���úͻ�ȡ����
	void SetName(CString Name);
	CString GetName();

	/*
	Ŀ�ģ��������ö���
	*/
	void SetObj(EMObj::CObj *pObj);

	/*
	Ŀ�ģ���������ʱ��
	������Time		(in)	CartItem�Ĳ���ʱ��
	*/
	virtual void Update(double Time);

	/*
	Ŀ�ģ�ִ������
	������Time		(in)	CartItem�Ĳ���ʱ��
	*/
	virtual void Update();

protected:
	double m_PlayTime;							//������ʱ��
	double m_StartTime;							//��ʼ�ʱ��, ��m_PlayTimeʹ��ͬһ��ʱ����
	double m_TimeLength;						//�ʱ�䳤��
	CString m_Name;								//����
	DWORD m_TimeType;							//����
	osg::ref_ptr<EMObj::CObj> m_Obj;			//��������


};
}