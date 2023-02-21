/****************************************************************************
Copyright (C),  
文件：
作者：张平
版本：1.0
日期：2010-04-09
描述：描述树形结构

修改记录：
作者						日期					描述
-----------------------------------------------------------------------------

****************************************************************************/

#pragma once
#include <CartoonObject\Export.h>
#include <vector>
#include <osg/ref_ptr>
#include <osg/Referenced>
#include <CartoonObject\Cart\CartItem.h>
#include <Data\RowStruct\RowDef.h>
namespace CartoonObject{

class CCartRoot;

//CCartNode类
class CARTOONOBJECT_EXPORT CCartNode : public osg::Referenced, public COM::CXMLNode
{
public:
	CCartNode()
	{
		m_parent = NULL;
		m_minLimit = 0.f;
		m_start = m_end = 0.f;
		m_bcheck = TRUE;
		m_icon = -1;
		m_name = _T("未命名");
	}

	virtual ~CCartNode()
	{

	}

	/*
	目的：拷贝构造函数
	*/
	CCartNode(const CCartNode &copied)
	{
		m_parent = copied.m_parent;
		m_minLimit = copied.m_minLimit;
		m_start = copied.m_start;
		m_end = copied.m_end;
		m_bcheck = copied.m_bcheck;
		m_icon = copied.m_icon;
		m_name = copied.m_name;
		m_itemRt = copied.m_itemRt;
	}

	/*
	目的：克隆函数
	*/
	virtual CCartNode *Clone() const { return new CCartNode(*this); }

	/*
	通过XML节点进行初始化
	*/
	virtual bool Init(MSXML2::IXMLDOMNodePtr pNode);

	/*
	目的：设置父节点
	*/
	void SetParent(CCartRoot *parent)
	{
		m_parent = parent;
	}

	/*
	目的：获取父节点
	*/
	CCartRoot* GetParent()
	{
		return m_parent;
	}

	/*
	目的：判断是否为叶结点
	*/
	virtual bool IsLeaf() const { return false; }

	/*
	目的：设置结点为选中态
	*/
	void SetCheck(BOOL bcheck)
	{
		m_bcheck = bcheck;
	}

	/*
	目的：获取结点状态
	*/
	BOOL GetCheck() const
	{
		return m_bcheck;
	}

	/*
	目的：设置和获取数据值
	*/
	bool GetValue(double *ptrstart, double *ptrend) const
	{
		if (ptrstart) {
			*ptrstart = m_start;
		}
		if (ptrend) {
			*ptrend = m_end;
		}

		return true;
	}

	bool SetValue(double *ptrstart, double *ptrend)
	{
		if (ptrstart) {
			m_start = *ptrstart;
		}
		if (ptrend) {
			m_end = *ptrend;
		}

		return true;
	}

	/*
	目的：设置和获取最小限制值(极限)
	*/
	void SetMinLimit(double limit)
	{
		m_minLimit = limit;
	}
	double GetMinLimit() const
	{
		return m_minLimit;
	}

	/*
	目的：设置和获取名字
	*/
	void SetName(const CString &name)
	{
		m_name = name;
	}
	CString GetName() const
	{
		return m_name;
	}

	/*
	目的：设置和获取图标索引
	*/
	void SetIcon(int icon)
	{
		m_icon = icon;
	}
	int GetIcon() const
	{
		return m_icon;
	}

	/*
	目的：设置和获取节点的位置
	*/
	void SetRect(const RECT &rect)
	{
		m_itemRt = rect;
	}
	void GetRect(RECT &rect) const
	{
		rect = m_itemRt;
	}

	/*
	目的：更新该结点的活动时间
	参数：start		(in)	结点的活动起始值
	      newStart	(out)	新的结点活动起始时刻
		  newEnd	(out)	新的结点活动结束时刻
	返回：void
	*/
	void UpdateNodeActiveTime(double start, double &newStart, double &newEnd);

	/*
	目的：递归树，拾取卡通元素
	参数：film			(in)	卡通单元
	bChkValid		(in)	是否使用勾选
	返回：void
	*/
	void PickItems(COM::CCartoonUnit &film, bool bChkValid);

protected:
	CCartRoot		*m_parent;
	double		m_minLimit;	//最小极限
	double		m_start;	//起始时刻
	double		m_end;		//结束时刻
	BOOL		m_bcheck;	//是否选中
	int			m_icon;		//图标
	CString		m_name;		//结点名称
	RECT		m_itemRt;	//结点位置
};


//CCartRoot类
class CARTOONOBJECT_EXPORT CCartRoot : public CCartNode
{
public:
	CCartRoot()
	{
		m_bparallel = true;
	}

	/*
	目的：拷贝构造函数(不拷贝孩子)
	*/
	CCartRoot(const CCartRoot &copied);

	/*
	目的：克隆函数
	*/
	virtual CCartRoot *Clone() const;

	virtual ~CCartRoot()
	{
		std::vector<osg::ref_ptr<CCartNode>>::iterator itr;
		for (itr = m_children.begin(); itr != m_children.end(); ++itr)
		{
			(*itr)->SetParent(NULL);
		}
	}

	/*
	通过XML节点进行初始化
	*/
	virtual bool Init(MSXML2::IXMLDOMNodePtr pNode);

	/*
	目的：添加一个孩子节点
	*/
	virtual bool AddChild(CCartNode *child);

	virtual bool InsertChild(unsigned int index, CCartNode *child);

	/*
	目的：移除一个孩子节点
	*/
	virtual bool RemoveChild(CCartNode *child);

	virtual bool RemoveChild(unsigned int index);

	/*
	目的：获取孩子节点的索引位置
	*/
	unsigned int GetChildIndex(CCartNode *child)
	{
		for (unsigned int childNum = 0; childNum < m_children.size(); ++childNum)
		{
			if (m_children[childNum].get() == child)
			{
				return childNum;
			}
		}
		return static_cast<unsigned int>(m_children.size()); //没有找到
	}

	/*
	目的：获取孩子节点
	*/
	CCartNode * GetChildByIndex(unsigned int index)
	{
		if (index < m_children.size())
		{
			return m_children[index];
		}
		return NULL;
	}

	/*
	目的：获取孩子节点个数
	*/
	unsigned int GetNumChildren()
	{
		return static_cast<unsigned int>(m_children.size());
	}

	/*
	目的：判断是否为叶结点
	*/
	virtual bool IsLeaf() const
	{
		return false;
	}

	/*
	目的：判断某结点是否在此树上(含根结点)
	参数：node		(in)	结点指针
	返回：在此树上返回true，否则返回false
	*/
	bool Has(CCartNode *node);

	/*
	目的：设置模式
	*/
	void SetParallel(BOOL bparallel)
	{
		m_bparallel = bparallel;
	}
	BOOL IsParallel() const
	{
		return m_bparallel;
	}

protected:
	std::vector<osg::ref_ptr<CCartNode>>	m_children;	//子节点
	BOOL									m_bparallel;//组内模式
};



//CCNItem类
class CARTOONOBJECT_EXPORT CCNAction : public osg::Referenced, public COM::CXMLNode
{
public:
	CCNAction(){}
	~CCNAction(){}

	/*
	通过XML节点进行初始化
	*/
	virtual bool Init(MSXML2::IXMLDOMNodePtr pNode);

	CCNAction(const CCNAction &copied);

	virtual CCNAction *Clone() const;

protected:
	CString		m_Name;			//类型
	CString		m_TimeType;		//类型
	double		m_StartTime;	//开始时间
	double		m_TimeLength;	//时长
};



//CCNItem类
class CARTOONOBJECT_EXPORT CCNItem : public CCartNode
{
public:

	CCNItem()
	{

	}

	/*
	目的：拷贝构造函数(深拷贝)
	*/
	CCNItem(const CCNItem &copied) : CCartNode(copied)
	{
		m_Type = copied.m_Type;	
		m_ShowObjData = copied.m_ShowObjData;
		std::vector<osg::ref_ptr<CCNAction>>::const_iterator itr = copied.m_CNActions.begin();
		for (; itr != copied.m_CNActions.end(); ++itr)
		{
			m_CNActions.push_back((*itr)->Clone());
		}
	}

	/*
	目的：克隆函数
	*/
	virtual CCartNode *Clone() const
	{
		return new CCNItem(*this);
	}

	virtual ~CCNItem()
	{

	}

	/*
	通过XML节点进行初始化
	*/
	virtual bool Init(MSXML2::IXMLDOMNodePtr pNode);

	/*
	目的：判断是否为叶结点
	*/
	virtual bool IsLeaf() const
	{
		return true;
	}

protected:
	CString		m_Type;			//类型
	CString		m_ShowObjData;	//显示对象数据
	
	std::vector<osg::ref_ptr<CCNAction>> m_CNActions;
};


class CARTOONOBJECT_EXPORT CCNFile : public CCartRoot
{
public:
	CCNFile(void){}
	virtual ~CCNFile(void){}

	/*
	通过XML节点进行初始化
	*/
	virtual bool Init(MSXML2::IXMLDOMNodePtr pNode);
public:
	CString		m_Version;		//版本
	CString		m_Author;		//作者
	CString		m_CreatedTime;	//创建时间
	CString		m_ModifiedTime;	//修改时间
	CString		m_Description;	//描述

	//资源路径
	CString		m_ResPath;

	//企业节点类型
	enum{
		EM_EnterNodeType_Reference,			//引用
		EM_EnterNodeType_Entity,			//实体
	};

	DWORD				m_EnterNodeType;	//企业节点类型
	LONG64				m_EnterID;			//企业ID
	osg::Vec3d			m_EnterLLA;			//企业经纬高
	osg::Vec3d			m_EnterPPL;			//企业滚转角
	

};

}