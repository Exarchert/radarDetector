#pragma once

#include "../../../../../CartoonObject/resource.h"
#include <osg/Vec3>
#include <osg/Matrixtransform>
// CDlgGasLeak dialog
namespace CartoonObject{
class CDlgGasLeak : public CDialog
{
	DECLARE_DYNAMIC(CDlgGasLeak)

public:
	CDlgGasLeak(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgGasLeak();

// Dialog Data
	enum { IDD = IDD_DLG_GASLEAK };
	void add();
	void setLocalToWorld(osg::Matrixd mat){_localToWorld = mat;}	

	afx_msg void OnNMReleasedcaptureSliderDensity(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMReleasedcaptureSliderSeppd(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMReleasedcaptureSliderStrength(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawSliderDensity(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawSliderSeppd(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawSliderStrength(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedColor();
	afx_msg void OnBnClickedSaveTemplate();
	afx_msg void OnCbnSelchangeComTempalte();
	afx_msg void OnEnChangeEdit1();
	afx_msg void OnClose();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
	//void crearteParticle(osg::Group* root);
	void crearteParticle1(osg::Group* root);
	void crearteParticle1(osg::Group *tfGravity,osg::Group *tfEmitter,osg::Group* particleSystem);
	virtual void PostNcDestroy();
	void dealwithNum();
	void computVecToAngle(osg::Vec3d& vec,double& Theta,double& Phi); 
	std::string double2string(double d);
	void readTemplate(std::string templateName);
public:
	int									m_slider_density;
	float								m_density;
	int									m_slider_speed;
	float								m_speed;
	int									m_slider_strength;
	float								m_strength;
	osg::Vec4							m_color;
	int									m_transparency;
	float								m_starTime;
	float								m_life;
	CComboBox							m_com_TemplateName;
	CString								m_TemplatekName;
	CString								m_name;
	osg::Vec3d							_pos;
	osg::Matrixd						_localToWorld;
	osg::Vec3d							_at;
	osg::Vec3d							_up;
	osg::ref_ptr<osg::MatrixTransform>	m_trans;
	osg::ref_ptr<osg::Group>			m_root; 
	CWnd*								_cwnd;
};
}