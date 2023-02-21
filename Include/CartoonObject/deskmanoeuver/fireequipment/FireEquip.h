#pragma once
#include <CartoonObject\DeskManoeuver\DeskManoElements.h>

namespace CartoonObject
{
	class CFireEquip : public CDeskManoElements
	{
	public:
		CFireEquip(void);
		~CFireEquip(void);

		//�����豸
		enum
		{
			//Fire_Hydrant = 0,		//����˨
			Fire_Bubble = 0,			//��ĭ
			Fire_Water_Monitor		//����ˮ��
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
