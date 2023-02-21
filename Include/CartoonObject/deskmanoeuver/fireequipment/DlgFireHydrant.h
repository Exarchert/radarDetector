#pragma once

#include "../../../../CartoonObject/resource.h"
// CDlgFireHydrant dialog
namespace CartoonObject
{
	class CDlgFireHydrant : public CDialog
	{
		DECLARE_DYNAMIC(CDlgFireHydrant)

	public:
		CDlgFireHydrant(CWnd* pParent = NULL);   // standard constructor
		virtual ~CDlgFireHydrant();
		
	// Dialog Data
		enum { IDD = IDD_FIREHYDRANT };



		virtual void PostNcDestroy();
	protected:
		virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
		BOOL OnInitDialog();
		DECLARE_MESSAGE_MAP()
	public:
				
		afx_msg void OnBnClickedOk();
		afx_msg void OnBnClickedCancel();
		afx_msg void OnBnClickedSaveTemplate();
		afx_msg void OnCbnSelchangeComTempalte();
		afx_msg void OnNMCustomdrawSliderStrength(NMHDR *pNMHDR, LRESULT *pResult);
		afx_msg void OnNMCustomdrawSliderTransparent(NMHDR *pNMHDR, LRESULT *pResult);
		void setLocalToWorld(osg::Matrixd mat){_localToWorld = mat;}
		void setCWnd(CWnd* cwnd){_cwnd = cwnd;}
		void add();
		void setType(int type){_type = type;}
	protected:
		void crearteParticle1(osg::Group* root);
		void computVecToAngle(osg::Vec3d& vec,double& Theta,double& Phi); 
		void readTemplate(std::string templateName);
	private:
		osg::Matrixd						_localToWorld;
		CWnd*								_cwnd;
		osg::ref_ptr<osg::MatrixTransform>	_trans;
		osg::ref_ptr<osg::Group>			_root;

		CString								m_name;
		CComboBox							m_com_TemplateName;
		int									m_slider_strength;
		float								m_strength;
		int									m_slider_transparent;
		float								m_transparent;
		int									_type;//消防类型，0为泡沫，1为水
	};
}