/*********************************************************************
Copyright (C),  
�ļ���CEditPropSheet.h CEditPropSheet.cpp
���ߣ������
�汾��1.0
���ڣ�2009-11-19
���������Ա�����
*********************************************************************/
#pragma once

#include <Object\Export.h>


namespace EMObj
{
	class OBJECT_EXPORT CEditPropSheet : public CPropertySheet
	{
		DECLARE_DYNAMIC(CEditPropSheet)	

	public:
		CEditPropSheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
		CEditPropSheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
		virtual ~CEditPropSheet();

		void SetEditObject(void *obj) { m_obj = obj; }

	protected:
		DECLARE_MESSAGE_MAP()
	public:
		afx_msg void OnClose();
		virtual BOOL OnInitDialog();
		virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);

	protected:
		void *m_obj;
	};

}


