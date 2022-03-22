#pragma once
#include "resource.h"

#include "DialogCurCurve.h"
#include "HistoryCurveWnd.h"

#include <vector>
#include <osg/Node>
#include <osg/ref_ptr>


class CDialogHistoryCurve : public CDialog
{
	DECLARE_DYNAMIC(CDialogHistoryCurve)

	friend class CDialogHistoryCtrl;
public:
	CDialogHistoryCurve( std::string const& projectName, int chanelIndex, std::string const& paramXML, float startLen, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDialogHistoryCurve();
	enum { IDD = IDD_DIALOG_HISTORY_CURVE };

	bool init();
protected:
	bool clear();

	void ShowData( bool location );
	
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

protected:
	
	std::string _projectName;
	std::string _xmlParam;
	int _channelIndex;

	float _startPos;
	float _endPos;
	float _startLen;

	HistoryCurveWnd _radarWnd;

	typedef std::vector<unsigned int> IndexVector;
	IndexVector _indexVector;

	unsigned int _curPos;

	CEdit _curLenEdit;

	CDialog *_loadProcess;
    int _skip;

	int m_iCount;

	osg::ref_ptr<osg::Node> _lpLineNode;
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnShowVideo();
	afx_msg void OnShowPos();
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	afx_msg void OnBnClickedButtonFind();
	afx_msg void OnBnClickedButtonExport();
	afx_msg HRESULT OnClickCurve(WPARAM wParam, LPARAM lParam );
	afx_msg void OnClose();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

	void ExportAllCHANNELToSGY();
};
