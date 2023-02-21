/******************************************************************************
File:			COM_CartoonUnit.h
Author:			��ƽ    
Date:			2009-03-31
Description:    ������Ԫ�࣬����һ��ӰƬ

ChangeLog:
����					����					����
-------------------------------------------------------------------------------
��־��				2009-07-21			�޸�CCartoonUnit���MFC CObject�̳�,
										ʹ֮����Serialize����.

*******************************************************************************/

#pragma once
#include <COM\Export.h>
#include <osg\Referenced>
#include <COM\Cartoon\CartoonItem.h>
#include <vector>

namespace COM{

class COM_EXPORT CCartoonUnit : public osg::Referenced
{
public:
	CCartoonUnit(void);
	virtual ~CCartoonUnit(void);

	//����ģʽ
	enum{
		ENM_PlayModel_Serial,		//����
		ENM_PlayModel_Parallel,		//����
	};

	/*
	Ŀ�ģ����úͻ�ȡ����ģʽ
	*/
	void SetPlayModel(DWORD PlayModel);
	DWORD GetPlayModel();

	/*
	Ŀ�ģ���Ӷ���������Ԫ�����
	*/
	void AddCartoonItem(CCartoonItem* pCartoonItem);

	/*
	Ŀ�ģ���ĳ����֮�����һ����������
	������where			(in)		���봦����ָ��
		  pCartoonItem	(in)		����ָ��
	���أ��ɹ�����TRUE,ʧ�ܷ���FALSE
	*/
	BOOL InsertCartoonItem(CCartoonItem *where, CCartoonItem *pCartoonItem);

	/*
	Ŀ�ģ�ɾ������������Ԫ�����
	*/
	void DelCartoonItem(CCartoonItem* pCartoonItem);

	/*
	Ŀ�ģ���ȡ��ͨ�����
	*/
	unsigned int GetCartoonItemNum();

	/*
	Ŀ�ģ���ȡ��ͨ��
	������index		(in)	����
	���أ���ͨ��ָ��
	*/
	CCartoonItem * GetCartoonItem(unsigned int index);

	void Clear();
	bool IsFinish();
	void SetPause();
	void SetPlay();

	/*
	Ŀ�ģ���������ʱ��
	������DeTime		(in)	ʱ��ı仯��
	*/
	void Update(double DeTime);

protected:
	std::vector<osg::ref_ptr<CCartoonItem>> m_Cartoons;	//����������Ԫ�����б�
	DWORD	m_PlayModel;	//����ģʽ
};

}