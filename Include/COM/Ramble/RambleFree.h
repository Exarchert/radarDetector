/************************************************************************
Copyright (C)  
�ļ���RambleFree.h RambleFree.cpp
���ߣ��ױ�
�汾��1.0
���ڣ�2009-06-18
���������β�����---��������.

�޸ļ�¼��
����					����				����
-------------------------------------------------------------------------
��־��					2009-11-13			��ֲ�ϰ汾���޸Ĳ��ֳ�Ա����.

************************************************************************/

#pragma once

#include <COM\Export.h>
#include <osgGA\GUIEventHandler>
#include <osgGA\GUIActionAdapter>
#include <COM\Ramble\RambleBase.h>


namespace COM	{

class COM_EXPORT CRambleFree : public CRambleBase
{
public:
	CRambleFree(osgAPEx::EarthManipulator *earthMan);
	virtual ~CRambleFree();

	/*
	Ŀ�ģ���������OSG��Ϣ���е��¼�
	*/
	virtual bool handle(const osgGA::GUIEventAdapter &ea, osgGA::GUIActionAdapter &aa);

	/*
	Ŀ�ģ�����Windows���͵���Ϣ
	*/
	virtual bool handle(UINT msg, WPARAM wParam, LPARAM lParam);

	//���ӽǷ����ƶ�
	virtual void MoveAt(float fValue);

	//ˮƽ��ǰ�ƶ�
	virtual void MoveAhead(float fValue);

	//ˮƽ�����ƶ�
	virtual void MoveRightSide(float fValue);

	//�ı�ƫ���
	virtual void rotateYaw(float fValue);

	//�ı丩����
	virtual void rotatePitch(float fValue);

	//�ı�߶�
	virtual void MoveHeight(float fValue);
protected:
	//�����ٶ�ϵ��
	void SetAccelerate(float fRate);

protected:
	float m_fMoveSpeed;		//�ƶ��ٶ�
	float m_fSpeedRate;		//�ٶ�ϵ��
	float m_fLastPtX;		//�ϴ�����λ��
	float m_fLastPtY;

	//�ֱ����μ�״̬
	bool m_bJoyBtn1Down;	//��1һֱ����
	int m_iZSize;			//z�˴�С
};

}