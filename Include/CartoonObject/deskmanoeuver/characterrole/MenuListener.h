#pragma once
#include <com\osgForm\osgFormUtils.h>
#include "Role.h"

class CMenuListener : public osgForm::MenuHandler 
{
public:
	CMenuListener();
	CMenuListener(CRole* pRole);

	~CMenuListener();
protected:
	virtual void itemClick(int id );

private:
	CRole* m_pRole;
};