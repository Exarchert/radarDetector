#pragma once
#include "../../../CartoonObject/resource.h"
#include <CartoonObject/DeskManoeuver/ListCtrlBase.h>
#include <CartoonObject/DeskManoeuver/MoveElement/EHMoveElement.h>
#include "../../../../CartoonObject/DeskManoeuver/MoveElement/DlgScriptSet.h"

namespace CartoonObject
{
	// CDlgMovement 对话框

	class CDlgMovement : public CDialog
	{
		DECLARE_DYNAMIC(CDlgMovement)

	public:
		CDlgMovement(CWnd* pParent = NULL);   // 标准构造函数
		virtual ~CDlgMovement();

		// 对话框数据
		enum { IDD = IDD_DLG_DESK_MANOEUVER };

	protected:
		virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
		LRESULT OnListCtrlLeave(WPARAM wParam,LPARAM lParam);
		LRESULT OnAddMoveElement(WPARAM wParam,LPARAM lParam);
		LRESULT OnAddElement(WPARAM wParam,LPARAM lParam);


		DECLARE_MESSAGE_MAP()
	public:
		virtual BOOL OnInitDialog();
		void SetActive(BOOL bActive);

		bool GenerateScripte(float timeLen,float bm,CString scriptName,bool bHasCamera = false,osg::Vec3d pos = osg::Vec3d(0,0,0),osg::Vec3 pose = osg::Vec3(0,0,0));

	protected:
		CListCtrlBase	m_MovementListCtrl;

		//监听类
		osg::ref_ptr<CEHMoveElement>	m_EHMoveElement;
		CDlgScriptSet	m_scriptDlg;
	};
}
