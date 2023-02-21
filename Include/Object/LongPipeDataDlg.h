#pragma once
#include "afxwin.h"
#include <Object/Export.h>
#include "ShowObj\LongPipe\SOLongPipe.h"
#include "..\Object\resource.h"
// CLongPipeDataDlg dialog


class  CLongPipeDataDlg : public CDialog
{
	DECLARE_DYNAMIC(CLongPipeDataDlg)

public:
	CLongPipeDataDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CLongPipeDataDlg();

public:
	virtual BOOL OnInitDialog();
	void ShowDialog();

// Dialog Data
	enum { IDD = IDD_DLG_LONGPIPEDATA };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedBtnLongpipefile();

public:
    HANDLE                             m_hThread;	//工作线程句柄
	std::vector<EMObj::CSOLongPipe*>   m_newLongPipesList;
	CButton                            m_parseBtn;
    CString                            m_dataFilePath;
	CEdit                              m_eidtFilePath;
	unsigned int                       m_perPipePoints;

	//管道数据
	osg::Vec4                          m_pipeAmbient;//管道材质
	osg::Vec4                          m_pipeDiffuse;
	osg::Vec4                          m_pipeSpecular;
	float                              m_pipeShininess;
	float                              m_pipeVisiableDis;
    float                              m_radius;//管线半径
    float                              m_segLength;//分段长度
    osg::Vec4                          m_pipeColor;
    CString                            m_imageFilePath;

	//线数据
	osg::Vec4              m_lodLineColor;
	float                  m_initLineWidth;
	std::map<float,float>  m_disLineOffsetMap;
	std::map<float,float>  m_disLineWidthMap;
    float                  m_lineVisiableDis;

public:
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedBtnLongpipeparse();

public:
	static unsigned WINAPI ParseFile(void *pdata);
	EMObj::CSOLongPipe*  CreateNewLongPipe(std::vector<osg::Vec3d>& pointsList);

	bool ParseLongPipePointsFile();
    std::string WChar2Ansi(LPCWSTR pwszSrc);
	CString GetXml(std::vector<osg::Vec3d>& points);

	MSXML2::IXMLDOMElementPtr GetLineAttributeNode();
	MSXML2::IXMLDOMElementPtr GetPipeAttributeNode();
	void SetColorAttribute(MSXML2::IXMLDOMElementPtr& node,osg::Vec4 color);

	void SetOkBtn(bool value);
    void SetCancelBtn(bool value);

	CButton m_imagePathBtn;
	CEdit m_editImage;
	afx_msg void OnBnClickedBtnPipedataImage();
	CButton m_btnMaterial;
	afx_msg void OnBnClickedBtnLongpipeMat();
	afx_msg void OnClose();

	/*******************************
	author:		liugh
	data:		2010.08.30
	description:
	*******************************/
	// 点与点之间的距离
	float m_PointDist;
	//离地高度
	float m_DeltaHeight;

	bool m_bStopThread;

	std::vector<osg::Vec3d> _vertexList;

	float CalAlt(float lon,float lat);

	double CalDistBetweenPts(osg::Vec3d preLLA,osg::Vec3d curLLA);

	osg::Vec3d CalNewPipePt(osg::Vec3d preLLA,osg::Vec3d curLLA);

	bool SetVertexData2Vec(CString filePath);
};
