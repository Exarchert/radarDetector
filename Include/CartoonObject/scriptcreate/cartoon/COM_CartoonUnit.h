/******************************************************************************
File:			COM_CartoonUnit.h
Author:			张平    
Date:			2009-03-31
Description:    动画单元类，类似一部影片

ChangeLog:
作者					日期					描述
-------------------------------------------------------------------------------
黄志鹏				2009-07-21			修改CCOM_CartoonUnit类从MFC CObject继承,
使之具有Serialize功能.
----------------------------------------------------------------------------
刘广慧				2011-1-12			生成xml格式脚本
*******************************************************************************/

#pragma once

#include <CartoonObject/Export.h>
#include <CartoonObject/ScriptCreate/Cartoon/COM_CartoonItem.h>


namespace CartoonObject		{

	class CARTOONOBJECT_EXPORT CCOM_CartoonUnit : public osg::Referenced, public CObject
	{
	protected:
		DECLARE_SERIAL(CCOM_CartoonUnit)
		/*
		目的：序列化操作
		*/
		virtual void Serialize(CArchive& ar);

	public:
		CCOM_CartoonUnit(void);
		virtual ~CCOM_CartoonUnit(void);

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
		void AddCartoonItem(CCOM_CartoonItem* pCartoonItem);

		/*
		目的：向组中添加动画基本单元类对象，组可以为空,pCartoonItem为空时添加一个空的组
		*/
		void AddCartoonItem(CString gpName = _T(""), CCOM_CartoonItem* pCartoonItem = NULL);

		/*
		目的：在某对象之后插入一个动画对象
		参数：where			(in)		插入处对象指针
		pCartoonItem	(in)		对象指针
		返回：成功返回TRUE,失败返回FALSE
		*/
		BOOL InsertCartoonItem(CCOM_CartoonItem *where, CCOM_CartoonItem *pCartoonItem);

		/*
		目的：删除动画基本单元类对象
		*/
		void DelCartoonItem(CCOM_CartoonItem* pCartoonItem);

		/*
		目的：获取卡通项个数
		*/
		unsigned int GetCartoonItemNum();

		//获取脚本播放时间长度
		double GetScriptTimeLength();

		//获取当前播放时刻
		double GetCurPlayTime();


		/*
		目的：获取卡通项
		参数：index		(in)	索引
		返回：卡通项指针
		*/
		CCOM_CartoonItem * GetCartoonItem(unsigned int index);

		void Clear();

		//设置播放进度
		void SetPlayTime(double time);

		/*
		目的：更新运行时间
		参数：DeTime		(in)	时间的变化量
		*/
		void Update(double DeTime);

		//获取所属厂区的ID
		DWORD GetEnterID() const { return m_fieldID; }

		//计算脚本时长
		float CalculateScriptTimeLength();

		/*
		purpose:
		*/
		virtual bool ParserXml(MSXML2::IXMLDOMNodePtr pNode);

		/*
		解析xml脚本文件
		*/
		bool ParserCartItemXml(MSXML2::IXMLDOMNodePtr pNode);

		/*
		保存xml格式文件
		*/
		bool SaveCartItemXml(CString filePath);

		//创建脚本组节点
		struct Group
		{
			CString _gpName;			//组名称
			std::vector<osg::ref_ptr<CCOM_CartoonItem>> _cartItems;		//动画基本单元对象列表
		};

	protected:
		//旧版格式
		std::vector<osg::ref_ptr<CCOM_CartoonItem>> m_Cartoons;	//动画基本单元对象列表

		//带有组的动画基本单元对象列表
		std::vector<Group>	m_CartGroups;

		DWORD		m_PlayModel;		//播放模式
		DWORD		m_fieldID;			//厂区ID(0:无厂区)
		osg::Vec3d	m_UserCoord;
		float		m_TimeLength;		//脚本时长

		CString		m_Name;				//XML描述下的根节点名称
		int			_ForwardStep;		//快进步长
		int			_RewindStep;		//慢放步长
	};

}