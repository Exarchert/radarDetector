/******************************************************************************
File:			CartUnit.h
Author:			张平    
Date:			2010-03-03
Description:    动画单元类，类似一部影片

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
	目的：添加动画基本单元类对象
	*/
	void AddCartItem(CCartItem* pCartItem);

	/*
	目的：在某对象之后插入一个动画对象
	参数：where			(in)		插入处对象指针
		  pCartoonItem	(in)		对象指针
	返回：成功返回TRUE,失败返回FALSE
	*/
	BOOL InsertCartItem(CCartItem *where, CCartItem *pCartoonItem);

	/*
	目的：删除动画基本单元类对象
	*/
	void DelCartItem(CCartItem* pCartoonItem);

	/*
	目的：获取卡通项个数
	*/
	unsigned int GetCartItemNum();

	/*
	目的：获取卡通项
	参数：index		(in)	索引
	返回：卡通项指针
	*/
	CCartItem * GetCartItem(unsigned int index);

	/*
	目的：清空播放队列
	*/
	void Clear();

	/*
	目的：更新运行时间
	参数：DeTime		(in)	时间的变化量
	*/
	void Update(double DeTime);

	/*
	目的：播放准备
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
	std::vector<osg::ref_ptr<CCartItem>> m_Carts;	//动画基本单元对象列表
	CString						m_Name;
	DWORD						m_PlayModel;	//播放模式
	double						m_PlayTime;		//播放时间
	osg::ref_ptr<CCartRoot>		m_CartRoot;		//目录节点
	CString						m_ResPath;		//资源路径

};

}