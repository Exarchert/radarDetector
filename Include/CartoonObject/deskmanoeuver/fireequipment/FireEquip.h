#pragma once
#include <CartoonObject\DeskManoeuver\DeskManoElements.h>

namespace CartoonObject
{
	class CFireEquip : public CDeskManoElements
	{
	public:
		CFireEquip(void);
		~CFireEquip(void);

		//消防设备
		enum
		{
			//Fire_Hydrant = 0,		//消防栓
			Fire_Bubble = 0,			//泡沫
			Fire_Water_Monitor		//消防水炮
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

		void SetFireEquipType(DWORD type){m_FireEquipType = type;}
		DWORD GetFireEquipType(){return m_FireEquipType;}

	private:
		DWORD	m_FireEquipType;
	};
}
