/******************************************************************************
File:			COM_CartoonUnit.h
Author:			��ƽ    
Date:			2009-03-31
Description:    ������Ԫ�࣬����һ��ӰƬ

ChangeLog:
����					����					����
-------------------------------------------------------------------------------
��־��				2009-07-21			�޸�CCOM_CartoonUnit���MFC CObject�̳�,
ʹ֮����Serialize����.
----------------------------------------------------------------------------
�����				2011-1-12			����xml��ʽ�ű�
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
		Ŀ�ģ����л�����
		*/
		virtual void Serialize(CArchive& ar);

	public:
		CCOM_CartoonUnit(void);
		virtual ~CCOM_CartoonUnit(void);

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
		void AddCartoonItem(CCOM_CartoonItem* pCartoonItem);

		/*
		Ŀ�ģ���������Ӷ���������Ԫ����������Ϊ��,pCartoonItemΪ��ʱ���һ���յ���
		*/
		void AddCartoonItem(CString gpName = _T(""), CCOM_CartoonItem* pCartoonItem = NULL);

		/*
		Ŀ�ģ���ĳ����֮�����һ����������
		������where			(in)		���봦����ָ��
		pCartoonItem	(in)		����ָ��
		���أ��ɹ�����TRUE,ʧ�ܷ���FALSE
		*/
		BOOL InsertCartoonItem(CCOM_CartoonItem *where, CCOM_CartoonItem *pCartoonItem);

		/*
		Ŀ�ģ�ɾ������������Ԫ�����
		*/
		void DelCartoonItem(CCOM_CartoonItem* pCartoonItem);

		/*
		Ŀ�ģ���ȡ��ͨ�����
		*/
		unsigned int GetCartoonItemNum();

		//��ȡ�ű�����ʱ�䳤��
		double GetScriptTimeLength();

		//��ȡ��ǰ����ʱ��
		double GetCurPlayTime();


		/*
		Ŀ�ģ���ȡ��ͨ��
		������index		(in)	����
		���أ���ͨ��ָ��
		*/
		CCOM_CartoonItem * GetCartoonItem(unsigned int index);

		void Clear();

		//���ò��Ž���
		void SetPlayTime(double time);

		/*
		Ŀ�ģ���������ʱ��
		������DeTime		(in)	ʱ��ı仯��
		*/
		void Update(double DeTime);

		//��ȡ����������ID
		DWORD GetEnterID() const { return m_fieldID; }

		//����ű�ʱ��
		float CalculateScriptTimeLength();

		/*
		purpose:
		*/
		virtual bool ParserXml(MSXML2::IXMLDOMNodePtr pNode);

		/*
		����xml�ű��ļ�
		*/
		bool ParserCartItemXml(MSXML2::IXMLDOMNodePtr pNode);

		/*
		����xml��ʽ�ļ�
		*/
		bool SaveCartItemXml(CString filePath);

		//�����ű���ڵ�
		struct Group
		{
			CString _gpName;			//������
			std::vector<osg::ref_ptr<CCOM_CartoonItem>> _cartItems;		//����������Ԫ�����б�
		};

	protected:
		//�ɰ��ʽ
		std::vector<osg::ref_ptr<CCOM_CartoonItem>> m_Cartoons;	//����������Ԫ�����б�

		//������Ķ���������Ԫ�����б�
		std::vector<Group>	m_CartGroups;

		DWORD		m_PlayModel;		//����ģʽ
		DWORD		m_fieldID;			//����ID(0:�޳���)
		osg::Vec3d	m_UserCoord;
		float		m_TimeLength;		//�ű�ʱ��

		CString		m_Name;				//XML�����µĸ��ڵ�����
		int			_ForwardStep;		//�������
		int			_RewindStep;		//���Ų���
	};

}