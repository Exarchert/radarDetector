/****************************************************************************
Copyright (C),  
�ļ���DataPick.h  DataPick.cpp
���ߣ���־��
�汾��1.0
���ڣ�2010-02-03
���������ݲɼ������ڿ��ӻ��زɼ��ռ�����.

�޸ļ�¼��
����						����						����
-----------------------------------------------------------------------------

****************************************************************************/
#pragma once
#include <Object\Export.h>
#include <osg\Matrixd>
#include <osg\Switch>
#include <osg\MatrixTransform>
#include <osgGA\GUIEventHandler>
#include <Com\DataBall\DataBall.h>

namespace EMObj{

class OBJECT_EXPORT CDataPick : public osgGA::GUIEventHandler
{
public:
	CDataPick(void);
	virtual ~CDataPick(void);

	//����OSG����
	virtual bool handle(const osgGA::GUIEventAdapter& ea,osgGA::GUIActionAdapter& aa);

	//��ʼ��
	void Init();

	/*
	Ŀ�ģ�������ʾ��(������ģ��)����ʾ��ǰ��λ�ú���̬
	������szHint		(in)		��Դ����
	*/
	void SetHintBall(LPCTSTR szHint);

	//��ȡ��ʾ��Ľ��
	osg::Node * GetHintBall() const;

	//������ʾ��λ�ú���̬
	void UpdateHintBall(const osg::Matrixd &mat);

	//�����ʾ��
	void AddHintBall();

	//�Ƴ���ʾ��
	void RemoveHintBall();

	//������ʾ��
	void ShowHintBall(bool bShow);

public:
	COM::CDataBall	m_dataBall;						//�����û��������λ��

protected:
	osg::ref_ptr<osg::MatrixTransform>	m_trans;	//�任����
	osg::ref_ptr<osg::Switch>			m_switch;	//��������
	bool			m_bWork;						//�Ƿ�����
};

}