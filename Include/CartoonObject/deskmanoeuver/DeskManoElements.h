#pragma once
#include <osg/Referenced>

#pragma warning(disable:4312)

namespace CartoonObject
{
	class CDeskManoElements : public osg::Referenced
	{
	public:
		CDeskManoElements(void);
		~CDeskManoElements(void);

	public:
		enum ElementType
		{
			DEFAULT,
			HOT_AREA,
			PLOT_CART,
			DISASTER_SRC,
			FIRE_EQUIP,
			MOVE_ELEMENT,
			CART_ROLE,
			TEXT_MARK
		};
		
		virtual CString GetName() const;
		virtual void SetName(CString strName);

		virtual CString GetTypeName() const;
		virtual void SetTypeName(CString strName);	

		virtual UINT GetType() const;
		virtual void SetType(UINT type);

		virtual float GetStartTime() const;
		virtual	void SetStartTime(float tm);

		virtual float GetLifeTime() const;
		virtual void SetLifeTime(float life);

		virtual CString GetInsruction() const;
		virtual	void SetInstruction(CString strInfo);

	protected:		
		UINT	m_Type;
		CString	m_Name;			//桌面推演类型		
		CString m_TypeName;		//每个推演类型下的具体推演类型
		float	m_StartTime;
		float	m_LifeTime;
		CString	m_Instruction;

	public:
		std::vector<CString>	m_ScriptList;
		void AddScriptToList(CString szScript);
	};
}
