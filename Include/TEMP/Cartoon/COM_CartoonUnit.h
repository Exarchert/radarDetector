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

*******************************************************************************/
#pragma once

#include <temp/Export.h>
#include <temp/Cartoon/COM_CartoonItem.h>

namespace TEMP
{
	class TEMP_EXPORT CCOM_CartoonUnit : public osg::Referenced, public CObject
	{
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
		bool IsFinish();
		void SetPause();
		void SetPlay();
		void SetForward(int step);
		void SetRewind(int step);

		void SetCurScriptPath(CString scriptFullPath);
		CString GetScriptPath(){return m_CurScriptPath;}

		//���ò��Ž���
		void SetPlayTime(double time);

		/*
		Ŀ�ģ���������ʱ��
		������DeTime		(in)	ʱ��ı仯��
		*/
		void Update(double DeTime);

		//��ȡ����������ID
		DWORD GetEnterID() const { return m_fieldID; }

		/*
		Ŀ�ģ����л�����
		*/
		virtual void Serialize(CArchive& ar);

		/*
		purpose:
		*/
		virtual bool ParserXml(MSXML2::IXMLDOMNodePtr pNode);

		bool ParserCartItemXml(MSXML2::IXMLDOMNodePtr pNode);

	protected:
		DECLARE_SERIAL(CCOM_CartoonUnit)

	protected:
		std::vector<osg::ref_ptr<CCOM_CartoonItem>> m_Cartoons;	//����������Ԫ�����б�
		DWORD		m_PlayModel;		//����ģʽ
		DWORD		m_fieldID;			//����ID(0:�޳���)

		CString		m_Name;				//XML�����µĸ��ڵ�����
		int			_ForwardStep;		//�������
		int			_RewindStep;		//���Ų���
		float		m_TimeLength;		//�ű�ʱ��

		osg::Vec3d	m_UserCoord;			//�û��Զ���������յ�����,���ȡ�ά�ȡ�����
		BOOL		m_bUserDefineCoord;		//�û��Զ�����յ��ʶ
		CString		m_CurScriptPath;
	};
}