/***************************************************************
file:			EditPage.h
Author:			张平    
Date:			2009-11-27
Description:    
***************************************************************/
#pragma once
#include <Object/Export.h>
#include <COm\Event\Event.h>
#include <object\Obj.h>
#pragma once

namespace EMObj{

class OBJECT_EXPORT CEditPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CEditPage)

public:
	CEditPage(UINT nIDTemplate, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CEditPage();

	//在UI线程中调用的设置函数
	virtual void SetEditObj(EMObj::CObj *pObj);

	void Update3DData(DWORD Para1=3, DWORD Para2=0);

	//三维线程调用，在UI线程中相应的刷新方法
	virtual void Update(WPARAM Para1=0, WPARAM Para2=0);

protected:
	DECLARE_MESSAGE_MAP()
	LRESULT OnUpdate(WPARAM wParam, LPARAM lParam);

protected:
	CObj *m_pObj;


};
}