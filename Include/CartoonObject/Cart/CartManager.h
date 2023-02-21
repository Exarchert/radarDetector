/***********************************************************************
File:			CartManager.h
Author:			��ƽ    
Date:			2009-03-3
Description:    ����������
************************************************************************/

#pragma once
#include <CartoonObject\Export.h>
#include <osgGA\GUIEventHandler>
#include <vector>
#include <CartoonObject\Cart\CartUnit.h>
namespace CartoonObject{

class CARTOONOBJECT_EXPORT CCartManager : public osgGA::GUIEventHandler 
{
public:
	CCartManager(void);
	virtual ~CCartManager(void);

	//��Ӻ�ɾ����ͨ��Ԫ
	void AddCartUnit(CCartUnit *pCartUnit);
	void DelCartUnit(CCartUnit *pCartUnit);

	//���������Ԫ
	void Clear();

	enum{
		ENM_PlayState_UnDefault = 0,
		ENM_PlayState_Play,
		ENM_PlayState_Pause,
		ENM_PlayState_Stop,
		ENM_PlayState_Finish,
		ENM_PlayState_QuickSpeedPlay,
		ENM_PlayState_SlowSpeedPlay,
	};

	/*
	Ŀ�ģ�����
	*/
	void Play();

	/*
	Ŀ�ģ���ͣ
	*/
	void Pause();

	/*
	Ŀ�ģ����²���
	*/
	void Replay();

	/*
	Ŀ�ģ����ò���ʱ��
	������Time		(in)	����ʱ��
	*/
	void CCartManager::SetPlayTime(double Time);

	/*
	Ŀ�ģ����ò����ٶ�
	������Speed		(in)	���ٱ���
	*/
	void SetSpeed(double Speed);

	/*
	Ŀ�ģ�ֹͣ����
	*/
	void Stop();

	/*
	Ŀ�ģ�����Ŀǰ������Ԫ�Ĳ���״̬
	���أ�����״ֵ̬
	*/
	DWORD GetPlayState(){ return m_PlayState; }



	//�¼�����
	bool handle(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa);


	/*
	���ļ���
	*/
	bool Load(CString strFile);


protected:
	std::vector<osg::ref_ptr<CCartUnit>> m_CartUnits;	//������Ԫ����
	double m_PlayTime;							//������ʱ��
	double m_PlayScale;							//�����ٶ�(����)
	DWORD m_PlayState;							//������Ԫ��״̬

private:
	double m_LastTime;
};


}