/******************************************************************************
File:			COM_CartoonUnit.h
Author:			张平    
Date:			2009-03-31
Description:    动画单元类，类似一部影片

ChangeLog:
作者					日期					描述
-------------------------------------------------------------------------------
黄志鹏				2009-07-21			修改CCartoonUnit类从MFC CObject继承,
										使之具有Serialize功能.

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

	//播放模式
	enum{
		ENM_PlayModel_Serial,		//串行
		ENM_PlayModel_Parallel,		//并行
	};

	/*
	目的：设置和获取播放模式
	*/
	void SetPlayModel(DWORD PlayModel);
	DWORD GetPlayModel();

	/*
	目的：添加动画基本单元类对象
	*/
	void AddCartoonItem(CCartoonItem* pCartoonItem);

	/*
	目的：在某对象之后插入一个动画对象
	参数：where			(in)		插入处对象指针
		  pCartoonItem	(in)		对象指针
	返回：成功返回TRUE,失败返回FALSE
	*/
	BOOL InsertCartoonItem(CCartoonItem *where, CCartoonItem *pCartoonItem);

	/*
	目的：删除动画基本单元类对象
	*/
	void DelCartoonItem(CCartoonItem* pCartoonItem);

	/*
	目的：获取卡通项个数
	*/
	unsigned int GetCartoonItemNum();

	/*
	目的：获取卡通项
	参数：index		(in)	索引
	返回：卡通项指针
	*/
	CCartoonItem * GetCartoonItem(unsigned int index);

	void Clear();
	bool IsFinish();
	void SetPause();
	void SetPlay();

	/*
	目的：更新运行时间
	参数：DeTime		(in)	时间的变化量
	*/
	void Update(double DeTime);

protected:
	std::vector<osg::ref_ptr<CCartoonItem>> m_Cartoons;	//动画基本单元对象列表
	DWORD	m_PlayModel;	//播放模式
};

}