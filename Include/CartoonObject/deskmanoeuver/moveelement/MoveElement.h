#pragma once
#include <CartoonObject\DeskManoeuver\DeskManoElements.h>
#include <Com/PathEdit/PathEdit.h>
#include <CartoonObject/ScriptCreate/ScriptGenerate.h>

namespace CartoonObject
{
	class CMoveElement : public CDeskManoElements
	{
	public:
		CMoveElement(void);
		~CMoveElement(void);

		//��
		enum
		{
			Ambulance= 0,			//�Ȼ���
			Excavator,				//�ھ��
			Fire_Engine,			//������
			Forklift,				//�泵
			Mobile_Machinery_Shop,	//���̳�
			Police_Car				//����
		};

	public:

		virtual CString GetName() const;
		virtual void SetName(CString strName);

		virtual UINT GetType() const;

		virtual float GetStartTime() const;
		virtual	void SetStartTime(float tm);

		virtual float GetLifeTime() const;
		virtual void SetLifeTime(float life);

		virtual CString GetInsruction() const;
		virtual	void SetInstruction(CString strInfo);

	private:
		osg::ref_ptr<COM::CPathEdit>	m_PathEH;
		CString		m_ModelName;
		DWORD		m_MoveType;

	public:
		//��ά�е���
		void RemovePathHandlerAddNode();

		void CreateScript(CScriptGenerate* pScriptGenerate);

		void StartPathEdit(osg::Vec2 screenPT);

		void SetModelName(CString strModel){m_ModelName = strModel;}
		void SetMoveType(DWORD type);
		DWORD GetMoveType(){return m_MoveType;}

		void SetPathEditActive(bool bAcrive);
	};
}
