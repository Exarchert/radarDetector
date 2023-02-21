#pragma once
#include <CartoonObject\DeskManoeuver\DeskManoElements.h>
#include <Com/PathEdit/PathEdit.h>
#include <CartoonObject/ScriptCreate/ScriptGenerate.h>
#include <com\osgForm\osgPopMenu.h>


namespace CartoonObject
{
	class CRole : public CDeskManoElements
	{
	public:
		CRole(void);
		~CRole(void);

		//车
		enum
		{
			Leader = 0,			//领导
			FireMan,			//消防人员
			Doctor,				//医生
			Worker,				//工人 
			Designer,			//技术员 
			Police				//警察
		};

		enum ModelType
		{
			IVE,				//ive或osg模型
			CFG					//骨骼模型
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

		void DestroyMemu();

		void SetCallerHWnd(HWND hWnd){m_hCallerHWnd = hWnd;}

		DWORD GetModelType(){return m_ModelType;}

		//忽略模型类型处理运动情形
		void  IgnoreModelType(bool bIgnore){m_bIgnoreModelType = bIgnore;}

	private:
		osg::ref_ptr<COM::CPathEdit>	m_PathEH;
		CString		m_ModelName;
		DWORD		m_RoleType;
		UINT		m_nAction;

		osg::ref_ptr<osgForm::osgPopMenu>	m_Menu;
		HWND		m_hCallerHWnd;
		UINT		m_ModelType;
		bool		m_bIgnoreModelType;

	public:
		//三维中调用
		void RemovePathHandlerAddNode();

		void CreateScript(CScriptGenerate* pScriptGenerate);

		void StartPathEdit(osg::Vec2 screenPT);

		void SetModelName(CString strModel){m_ModelName = strModel;}
		void SetRoleType(DWORD type);
		DWORD GeRoleoveType(){return m_RoleType;}

		void SetPathEditActive(bool bAcrive);

		void PopUpMenuDependsModelType(osg::Vec2 screenPt);

		void ProcessMenuClick(int item);
	};	
}
