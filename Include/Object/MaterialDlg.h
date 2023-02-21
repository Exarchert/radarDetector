#pragma once
#include <osg\Material>

// CMaterialDlg dialog

class CMaterialDlg : public CDialog
{
	DECLARE_DYNAMIC(CMaterialDlg)

public:
	CMaterialDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CMaterialDlg();

// Dialog Data
	enum { IDD = IDD_MATERIALDLG };

	virtual BOOL OnInitDialog();

	void SetMaterialColor(osg::Vec4 ambient,osg::Vec4 diffuse,osg::Vec4 specular,float shininess);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	osg::Material* m_material;
	osg::Vec4      m_ambinent;
	osg::Vec4      m_diffuse;
	osg::Vec4      m_specular;
	float          m_shininess;

public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
};
