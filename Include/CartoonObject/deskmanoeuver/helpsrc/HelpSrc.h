#pragma once
#include <CartoonObject\DeskManoeuver\DeskManoElements.h>

namespace CartoonObject
{
	class CHelpSrc : public CDeskManoElements
	{
	public:
		CHelpSrc(void);
		~CHelpSrc(void);

		//��Ԯ����
		enum
 		{
// 			Fire_Hydrant = 0,		//����˨
// 			Fire_Bubble,			//��ĭ
// 			Fire_Water_Monitor		//����ˮ��
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

		void SetHelpSrcType(DWORD type){m_HelpSrcType = type;}
		DWORD GetHelpSrcType(){return m_HelpSrcType;}

	private:
		DWORD	m_HelpSrcType;
	};
}
