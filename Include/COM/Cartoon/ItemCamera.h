/********************************************************************************
Copyright (C),  
�ļ���ItemCamera.h  ItemCamera.cpp
���ߣ���־��
�汾��1.0
���ڣ�2009-08-11
��������ͨ����--���

�޸ļ�¼
����						����							����
---------------------------------------------------------------------------------
��־��					2009-12-09						��ֲ���˹���

********************************************************************************/

#pragma once

#include <COM\Cartoon\CartoonItem.h>
#include <osgGA\MatrixManipulator>

namespace COM	{

class COM_EXPORT CItemCamera : public CCartoonItem
{
public:
	CItemCamera()
	{
		m_Name = _T("δ�������");
	}

	virtual ~CItemCamera();

	CItemCamera(const CItemCamera &camera);

	CCartoonItem * Clone();

	//�������������
	void SetMatrixManipulator(osgGA::MatrixManipulator *mani);

	/*
	Ŀ�ģ�������Ԫִ����Ϊ
	������Time		(in)	����ڿ�ʼ�ʱ�̵�ʱ��
	      pos		(in)	������λ��ʱ��λ��			
	���أ�void
	*/
	virtual void DoAction(double Time, osg::Vec3d pos);

protected:
	osg::ref_ptr<osgGA::MatrixManipulator>	m_mani;
};

}