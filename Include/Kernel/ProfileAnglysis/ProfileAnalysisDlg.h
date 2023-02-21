#pragma once
#include <vector>
#include "../../../Kernel/resource.h"
//#include "RenderToolProfileAnalysisHandler.h"
#include <COM/Cursor/UserCursor.h>
#include <COM/Command/Cmd.h>

namespace Kernel
{
class RenderToolProfileAnalysisHandler;
struct PointData 
{
	double x;
	double y;
	double z;
};
struct PipeLineData
{
	PointData colisionPoint;
	float pipeLineRander;
	CString pipeLineName;

};
class Profile_Data
{
	
public:
	PointData startLineDot;
	PointData endLineDot;
	std::vector<PipeLineData> pipleLines;

};


// MyDrawDlg 对话框

class ProfileAnalysisDlg : public CDialog
{
	DECLARE_DYNAMIC(ProfileAnalysisDlg)

public:
	ProfileAnalysisDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~ProfileAnalysisDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	//virtual BOOL Create(LPCTSTR lpszTemplateName, CWnd* pParentWnd = NULL);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg void OnClose();

	void setShowDB(Profile_Data proflieData);
	void UpdateDrawe(bool startDraw);
private:
	Profile_Data m_profileData;
	float ComputeTwoDotLength(PointData startDot,PointData endDot);
	void CreateGridXY(float unitX, float unitY);
	void CreatePipeLine();
	CRect ComputeXYZToCRect(PointData point);
	bool ComputeUnitXY(float& unitX,float& unitY);

	CDC* GetDrawCDC();
	CRect GetDrawCRect();
	bool m_startDraw;

	COM::CUserCursor* m_pCursor;

	RenderToolProfileAnalysisHandler* m_pReadToolHandler;
};
}
