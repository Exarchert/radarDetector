/***************************************************************************
File:			COM_CartoonItem.h
Author:			��ƽ    
Date:			2010-01-21
Description:    ����������
***************************************************************************/

#pragma once
#include <COM\Export.h>
#include <Osg/Referenced>
#include <Osg/Vec3>
#include <Osg/Vec2>
#include <COM\XML\XMLNode.h>

namespace COM{

//����������Ԫ��
class COM_EXPORT CCartoonAction : public osg::Referenced
{
public:
	CCartoonAction(void);
	virtual ~CCartoonAction(void);

	enum{
		ENM_ActionType_Continuity = 0,			//�����Զ���
		ENM_ActionType_UnContinuity,			//�������Զ���
	};

	////��ȡ��������
	//DWORD GetType();

	////���úͻ�ȡ������ʼʱ��
	//void SetStartTime(double StartTime);
	//double GetStartTime();

	////���úͻ�ȡʱ��
	//void SetTimeLength(double TimeLength);
	//double GetTimeLength();

	////���úͻ�ȡ����
	//void SetName(CString Name);
	//CString GetName();

	///*
	//Ŀ�ģ���������ʱ��
	//������DeTime		(in)	ʱ��ı仯��
	//*/
	//virtual void Update(double DeTime);


protected:
	double m_PlayTime;							//������ʱ��
	double m_StartTime;							//��ʼ�ʱ��, ��m_PlayTimeʹ��ͬһ��ʱ����
	double m_TimeLength;						//�ʱ�䳤��
	CString m_Name;								//��ͨ����
	double m_Type;								//����
};
}