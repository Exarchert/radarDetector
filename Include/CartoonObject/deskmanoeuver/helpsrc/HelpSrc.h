#pragma once
#include <CartoonObject\DeskManoeuver\DeskManoElements.h>

namespace CartoonObject
{
	class CHelpSrc : public CDeskManoElements
	{
	public:
		CHelpSrc(void);
		~CHelpSrc(void);

		//救援物资
		enum
 		{
// 			Fire_Hydrant = 0,		//消防栓
// 			Fire_Bubble,			//泡沫
// 			Fire_Water_Monitor		//消防水炮
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
