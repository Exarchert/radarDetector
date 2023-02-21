#pragma once
#include "..\..\..\CartoonObject\resource.h"
#include <CartoonObject\DeskManoeuver\ListCtrlBase.h>
#include <CartoonObject\DeskManoeuver\FireEquipment\EHFireEquip.h>


namespace CartoonObject
{
	// CDlgDisaster �Ի���

	class CDlgFireEquip : public CDialog
	{
		DECLARE_DYNAMIC(CDlgFireEquip)

	public:
		CDlgFireEquip(CWnd* pParent = NULL);   // ��׼���캯��
		virtual ~CDlgFireEquip();

		// �Ի�������
		enum { IDD = IDD_DLG_DESK_MANOEUVER };

	protected:
		virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
		LRESULT OnListCtrlLeave(WPARAM wParam,LPARAM lParam);
		LRESULT OnAddFireEquip(WPARAM wParam,LPARAM lParam);

		DECLARE_MESSAGE_MAP()
	public:
		virtual BOOL OnInitDialog();
		void SetActive(BOOL bActive);

	protected:
		CListCtrlBase	m_FireListCtrl;
		//������
		osg::ref_ptr<CEHFire>	m_EHFire;
	};
}
