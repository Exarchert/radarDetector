#pragma once
#include <CartoonObject\DeskManoeuver\DeskManoElements.h>

namespace CartoonObject
{
	class CPlotCart : public CDeskManoElements
	{
	public:
		CPlotCart(void);
		~CPlotCart(void);

		virtual CString GetName() const;
		virtual void SetName(CString strName);

		virtual UINT GetType() const;

		virtual float GetStartTime() const;
		virtual	void SetStartTime(float tm);

		virtual float GetLifeTime() const;
		virtual void SetLifeTime(float life);

		virtual CString GetInsruction() const;
		virtual	void SetInstruction(CString strInfo);
	};
}
