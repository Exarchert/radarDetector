/****************************************************************************
Copyright (C),  
�ļ���
���ߣ���ƽ
�汾��1.0
���ڣ�2010-04-09
�������������νṹ

�޸ļ�¼��
����						����					����
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

//CCartNode��
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
		m_name = _T("δ����");
	}

	virtual ~CCartNode()
	{

	}

	/*
	Ŀ�ģ��������캯��
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
	Ŀ�ģ���¡����
	*/
	virtual CCartNode *Clone() const { return new CCartNode(*this); }

	/*
	ͨ��XML�ڵ���г�ʼ��
	*/
	virtual bool Init(MSXML2::IXMLDOMNodePtr pNode);

	/*
	Ŀ�ģ����ø��ڵ�
	*/
	void SetParent(CCartRoot *parent)
	{
		m_parent = parent;
	}

	/*
	Ŀ�ģ���ȡ���ڵ�
	*/
	CCartRoot* GetParent()
	{
		return m_parent;
	}

	/*
	Ŀ�ģ��ж��Ƿ�ΪҶ���
	*/
	virtual bool IsLeaf() const { return false; }

	/*
	Ŀ�ģ����ý��Ϊѡ��̬
	*/
	void SetCheck(BOOL bcheck)
	{
		m_bcheck = bcheck;
	}

	/*
	Ŀ�ģ���ȡ���״̬
	*/
	BOOL GetCheck() const
	{
		return m_bcheck;
	}

	/*
	Ŀ�ģ����úͻ�ȡ����ֵ
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
	Ŀ�ģ����úͻ�ȡ��С����ֵ(����)
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
	Ŀ�ģ����úͻ�ȡ����
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
	Ŀ�ģ����úͻ�ȡͼ������
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
	Ŀ�ģ����úͻ�ȡ�ڵ��λ��
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
	Ŀ�ģ����¸ý��Ļʱ��
	������start		(in)	���Ļ��ʼֵ
	      newStart	(out)	�µĽ����ʼʱ��
		  newEnd	(out)	�µĽ������ʱ��
	���أ�void
	*/
	void UpdateNodeActiveTime(double start, double &newStart, double &newEnd);

	/*
	Ŀ�ģ��ݹ�����ʰȡ��ͨԪ��
	������film			(in)	��ͨ��Ԫ
	bChkValid		(in)	�Ƿ�ʹ�ù�ѡ
	���أ�void
	*/
	void PickItems(COM::CCartoonUnit &film, bool bChkValid);

protected:
	CCartRoot		*m_parent;
	double		m_minLimit;	//��С����
	double		m_start;	//��ʼʱ��
	double		m_end;		//����ʱ��
	BOOL		m_bcheck;	//�Ƿ�ѡ��
	int			m_icon;		//ͼ��
	CString		m_name;		//�������
	RECT		m_itemRt;	//���λ��
};


//CCartRoot��
class CARTOONOBJECT_EXPORT CCartRoot : public CCartNode
{
public:
	CCartRoot()
	{
		m_bparallel = true;
	}

	/*
	Ŀ�ģ��������캯��(����������)
	*/
	CCartRoot(const CCartRoot &copied);

	/*
	Ŀ�ģ���¡����
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
	ͨ��XML�ڵ���г�ʼ��
	*/
	virtual bool Init(MSXML2::IXMLDOMNodePtr pNode);

	/*
	Ŀ�ģ����һ�����ӽڵ�
	*/
	virtual bool AddChild(CCartNode *child);

	virtual bool InsertChild(unsigned int index, CCartNode *child);

	/*
	Ŀ�ģ��Ƴ�һ�����ӽڵ�
	*/
	virtual bool RemoveChild(CCartNode *child);

	virtual bool RemoveChild(unsigned int index);

	/*
	Ŀ�ģ���ȡ���ӽڵ������λ��
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
		return static_cast<unsigned int>(m_children.size()); //û���ҵ�
	}

	/*
	Ŀ�ģ���ȡ���ӽڵ�
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
	Ŀ�ģ���ȡ���ӽڵ����
	*/
	unsigned int GetNumChildren()
	{
		return static_cast<unsigned int>(m_children.size());
	}

	/*
	Ŀ�ģ��ж��Ƿ�ΪҶ���
	*/
	virtual bool IsLeaf() const
	{
		return false;
	}

	/*
	Ŀ�ģ��ж�ĳ����Ƿ��ڴ�����(�������)
	������node		(in)	���ָ��
	���أ��ڴ����Ϸ���true�����򷵻�false
	*/
	bool Has(CCartNode *node);

	/*
	Ŀ�ģ�����ģʽ
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
	std::vector<osg::ref_ptr<CCartNode>>	m_children;	//�ӽڵ�
	BOOL									m_bparallel;//����ģʽ
};



//CCNItem��
class CARTOONOBJECT_EXPORT CCNAction : public osg::Referenced, public COM::CXMLNode
{
public:
	CCNAction(){}
	~CCNAction(){}

	/*
	ͨ��XML�ڵ���г�ʼ��
	*/
	virtual bool Init(MSXML2::IXMLDOMNodePtr pNode);

	CCNAction(const CCNAction &copied);

	virtual CCNAction *Clone() const;

protected:
	CString		m_Name;			//����
	CString		m_TimeType;		//����
	double		m_StartTime;	//��ʼʱ��
	double		m_TimeLength;	//ʱ��
};



//CCNItem��
class CARTOONOBJECT_EXPORT CCNItem : public CCartNode
{
public:

	CCNItem()
	{

	}

	/*
	Ŀ�ģ��������캯��(���)
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
	Ŀ�ģ���¡����
	*/
	virtual CCartNode *Clone() const
	{
		return new CCNItem(*this);
	}

	virtual ~CCNItem()
	{

	}

	/*
	ͨ��XML�ڵ���г�ʼ��
	*/
	virtual bool Init(MSXML2::IXMLDOMNodePtr pNode);

	/*
	Ŀ�ģ��ж��Ƿ�ΪҶ���
	*/
	virtual bool IsLeaf() const
	{
		return true;
	}

protected:
	CString		m_Type;			//����
	CString		m_ShowObjData;	//��ʾ��������
	
	std::vector<osg::ref_ptr<CCNAction>> m_CNActions;
};


class CARTOONOBJECT_EXPORT CCNFile : public CCartRoot
{
public:
	CCNFile(void){}
	virtual ~CCNFile(void){}

	/*
	ͨ��XML�ڵ���г�ʼ��
	*/
	virtual bool Init(MSXML2::IXMLDOMNodePtr pNode);
public:
	CString		m_Version;		//�汾
	CString		m_Author;		//����
	CString		m_CreatedTime;	//����ʱ��
	CString		m_ModifiedTime;	//�޸�ʱ��
	CString		m_Description;	//����

	//��Դ·��
	CString		m_ResPath;

	//��ҵ�ڵ�����
	enum{
		EM_EnterNodeType_Reference,			//����
		EM_EnterNodeType_Entity,			//ʵ��
	};

	DWORD				m_EnterNodeType;	//��ҵ�ڵ�����
	LONG64				m_EnterID;			//��ҵID
	osg::Vec3d			m_EnterLLA;			//��ҵ��γ��
	osg::Vec3d			m_EnterPPL;			//��ҵ��ת��
	

};

}