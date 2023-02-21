#pragma once
#include <CartoonObject\Export.h>
#include <CartoonObject\Cart\CartAction.h>

namespace CartoonObject
{

class CARTOONOBJECT_EXPORT CCAShow : public CCartAction
{
public:
	CCAShow(void);
	~CCAShow(void);

	virtual void Update();

	static CString m_Name;
};

class CARTOONOBJECT_EXPORT CCAHide : public CCartAction
{
public:
	CCAHide(void);
	~CCAHide(void);

	virtual void Update();

	static CString m_Name;
};

}
