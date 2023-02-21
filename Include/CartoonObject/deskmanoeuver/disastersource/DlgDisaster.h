#pragma once
#include "..\..\..\CartoonObject\resource.h"
#include <CartoonObject\DeskManoeuver\ListCtrlBase.h>
#include <CartoonObject\DeskManoeuver\DisasterSource\EHDisaster.h>
#include <CartoonObject/DeskManoeuver/DisasterSource/PoolFireDlg.h>
#include <CartoonObject/DeskManoeuver/DisasterSource/DlgSprayFire.h>
#include <CartoonObject/DeskManoeuver/DisasterSource/DlgExplosionSet.h>

namespace CartoonObject
{
	// CDlgDisaster 对话框

	class CDlgDisaster : public CDialog
	{
		DECLARE_DYNAMIC(CDlgDisaster)

	public:
		CDlgDisaster(CWnd* pParent = NULL);   // 标准构造函数
		virtual ~CDlgDisaster();

		// 对话框数据
		enum { IDD = IDD_DLG_DESK_MANOEUVER };

	protected:
		virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
		LRESULT OnListCtrlLeave(WPARAM wParam,LPARAM lParam);

		LRESULT OnAddDisaster(WPARAM wParam,LPARAM lParam);

		LRESULT OnCreatePoolFire(WPARAM wParam,LPARAM lParam);

		DECLARE_MESSAGE_MAP()
	public:
		virtual BOOL OnInitDialog();
		void SetActive(BOOL bActive);

		void UpdatePoolFireInfo(float width,float hight,float size);
		//if mode is -1,means cancel button clicked
		void CDlgDisaster::FinishDisaster(DWORD type = -1,int mode = 0);

		void UpdateSprayFire(float intensity);
		void UpdateExplosion(float intensity);


		bool GenerateDisasterScripte(float timeLen,float bm,CString scriptName,CString nodeName,
			osg::Vec3d disasterPos = osg::Vec3d(0,0,0),osg::Vec3 disasterPose = osg::Vec3(0,0,0),
			bool bHasCamera = false,osg::Vec3d cameraPos = osg::Vec3d(0,0,0),osg::Vec3 cameraPose = osg::Vec3(0,0,0));

	protected:
		CListCtrlBase	m_DisasterListCtrl;

		CDlgPoolFire*		m_pDlgPoolFire;
		CDlgSprayFire*		m_pDlgSprayFire;
		CDlgExplosionSet*	m_pDlgExplosion;

		//监听类
		osg::ref_ptr<CEHDisaster>	m_EHDisaster;
	};
}
