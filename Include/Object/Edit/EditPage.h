/***************************************************************
file:			EditPage.h
Author:			��ƽ    
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
	CEditPage(UINT nIDTemplate, CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CEditPage();

	//��UI�߳��е��õ����ú���
	virtual void SetEditObj(EMObj::CObj *pObj);

	void Update3DData(DWORD Para1=3, DWORD Para2=0);

	//��ά�̵߳��ã���UI�߳�����Ӧ��ˢ�·���
	virtual void Update(WPARAM Para1=0, WPARAM Para2=0);

protected:
	DECLARE_MESSAGE_MAP()
	LRESULT OnUpdate(WPARAM wParam, LPARAM lParam);

protected:
	CObj *m_pObj;


};
}