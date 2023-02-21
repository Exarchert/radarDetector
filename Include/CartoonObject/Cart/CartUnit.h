/******************************************************************************
File:			CartUnit.h
Author:			��ƽ    
Date:			2010-03-03
Description:    ������Ԫ�࣬����һ��ӰƬ

*******************************************************************************/

#pragma once
#include <CartoonObject\Export.h>
#include <COM\xml\XMLNode.h>
#include <osg\Referenced>
#include <osg\ref_ptr>
#include <CartoonObject\Cart\CartItem.h>
#include <CartoonObject\Cart\CartNode.h>
#include <vector>

namespace CartoonObject{

class CARTOONOBJECT_EXPORT CCartUnit : public osg::Referenced, public COM::CXMLNode
{
public:
	CCartUnit(void);
	virtual ~CCartUnit(void);

	//
	virtual bool Init(MSXML2::IXMLDOMNodePtr pNode);

	/*
	Ŀ�ģ���Ӷ���������Ԫ�����
	*/
	void AddCartItem(CCartItem* pCartItem);

	/*
	Ŀ�ģ���ĳ����֮�����һ����������
	������where			(in)		���봦����ָ��
		  pCartoonItem	(in)		����ָ��
	���أ��ɹ�����TRUE,ʧ�ܷ���FALSE
	*/
	BOOL InsertCartItem(CCartItem *where, CCartItem *pCartoonItem);

	/*
	Ŀ�ģ�ɾ������������Ԫ�����
	*/
	void DelCartItem(CCartItem* pCartoonItem);

	/*
	Ŀ�ģ���ȡ��ͨ�����
	*/
	unsigned int GetCartItemNum();

	/*
	Ŀ�ģ���ȡ��ͨ��
	������index		(in)	����
	���أ���ͨ��ָ��
	*/
	CCartItem * GetCartItem(unsigned int index);

	/*
	Ŀ�ģ���ղ��Ŷ���
	*/
	void Clear();

	/*
	Ŀ�ģ���������ʱ��
	������DeTime		(in)	ʱ��ı仯��
	*/
	void Update(double DeTime);

	/*
	Ŀ�ģ�����׼��
	*/
	void ReadyPlay();

	CCartRoot* GetCartRoot()
	{
		return m_CartRoot.get();
	}

	void SetResPath(CString strResPath)
	{
		m_ResPath = strResPath;
	}
	CString GetResPath()
	{
		return m_ResPath;
	}

protected:
	std::vector<osg::ref_ptr<CCartItem>> m_Carts;	//����������Ԫ�����б�
	CString						m_Name;
	DWORD						m_PlayModel;	//����ģʽ
	double						m_PlayTime;		//����ʱ��
	osg::ref_ptr<CCartRoot>		m_CartRoot;		//Ŀ¼�ڵ�
	CString						m_ResPath;		//��Դ·��

};

}