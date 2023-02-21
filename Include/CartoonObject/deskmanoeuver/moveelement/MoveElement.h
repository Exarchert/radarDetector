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

		//车
		enum
		{
			Ambulance= 0,			//救护车
			Excavator,				//挖掘机
			Fire_Engine,			//消防车
			Forklift,				//叉车
			Mobile_Machinery_Shop,	//工程车
			Police_Car				//警车
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
		//三维中调用
		void RemovePathHandlerAddNode();

		void CreateScript(CScriptGenerate* pScriptGenerate);

		void StartPathEdit(osg::Vec2 screenPT);

		void SetModelName(CString strModel){m_ModelName = strModel;}
		void SetMoveType(DWORD type);
		DWORD GetMoveType(){return m_MoveType;}

		void SetPathEditActive(bool bAcrive);
	};
}
