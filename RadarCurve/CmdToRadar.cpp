#include "StdAfx.h"
#include "RadarCurve/CmdCenter/CmdToRadar.h"
#include "RadarCurve/CmdCenter/CmdCenter.h"
#include "DataTemplateTab.h"
#include "RadarCurve/RadarManager.h"
#include "Object/CmdCenter/CmdCenter.h"

#include "Kernel\KernelExport.h"



CmdRadarDetection::CmdRadarDetection()
{

}

CmdRadarDetection::~CmdRadarDetection()
{

}

//获取解析器的名字
CString CmdRadarDetection::GetName()
{
	return _T("雷达");
}

///执行命令
CString CmdRadarDetection::ExecuteCmd(CString Cmd)
{
	COM::CXmlParser parser;
	CString szTemp;

	CString objectId ;
	parser.Init(Cmd);
	szTemp = parser.GetNodeAttribute(_T("命令"), _T("类型"));

	if(szTemp.GetLength() <= 0)
	{
 		return _T("命令无效!");
	}
	else
	{
		
	}
	//具体命令分发
	if (szTemp.Compare(_T("初始化RADARMANAGER"))==0)
	{
		Radar::CmdRadarManagerInit();
	} 
	else if(szTemp.Compare(_T("开始雷达工作"))==0)
	{
		Radar::CmdStartRadarWorkDlg();
	}
	else if(szTemp.Compare(_T("停止雷达工作"))==0)
	{
		Radar::CmdEndRadarWorkDlg();
	}
	else if(szTemp.Compare(_T("雷达实时曲线"))==0)
	{
		Radar::CmdRadarRealTimeCurves();
	}
	else if(szTemp.Compare(_T("雷达历史曲线"))==0)
	{
		Radar::CmdRadarHistoryCurves();
	}
	else if(szTemp.Compare(_T("GPS实时定位"))==0)
	{
		Radar::CmdGPSRealTimeLocate();
	}
	else if(szTemp.Compare(_T("雷达参数设置"))==0)
	{
		Radar::CmdRadarParametersSettings();
	}
	else if(szTemp.Compare(_T("雷达数据处理"))==0)
	{
		Radar::CmdRadarDataProcess();
	}
	else if(szTemp.Compare(_T("雷达数据报表显示"))==0)
	{
		Radar::CmdRadarDataFormsShow();
	}
	else if(szTemp.Compare(_T("导出雷达工程"))==0)
	{
		CString strPath = parser.GetNodeAttribute(_T("命令"), _T("导出路劲"));
		CString strPROJECTCREATETIME = parser.GetNodeAttribute(_T("命令"), _T("工程创建时间"));
		Radar::CmdExportRadarProject(strPath,strPROJECTCREATETIME);
	}
	else if(szTemp.Compare(_T("导入雷达工程"))==0)
	{
		CString strFileAllPath = parser.GetNodeAttribute(_T("命令"), _T("文件全路径"));
		return Radar::CmdImportRadarProject(strFileAllPath);
	}
	else if(szTemp.Compare(_T("删除雷达工程"))==0)
	{
		CString strPROJECTCREATETIME = parser.GetNodeAttribute(_T("命令"), _T("工程创建时间"));
		if (true == Radar::CmdDeleteRadarProject(strPROJECTCREATETIME))
		{
			return _T("<返回信息 删除雷达工程='成功'></返回信息>");
		}
		else
		{
			return _T("<返回信息 删除雷达工程='失败'></返回信息>");
		}
	}	
	else if(szTemp.Compare(_T("视频播放"))==0)
	{
		Radar::CmdVideoPlay();
	}
	else if(szTemp.Compare(_T("视频回放"))==0)
	{
		Radar::CmdVideoPlayback();
	}
	else if(szTemp.Compare(_T("雷达探测区域绘制"))==0)
	{
		DrawRadarArea(Cmd);
	}
	else if(szTemp.Compare(_T("雷达探测区清除"))==0)
	{
		ClearRadarArea(Cmd);
	}
	else if(szTemp.Compare(_T("选择生成GPR"))==0)
	{
		Radar::SelectCreateGPR();
	}
	return _T("");
}

//分割字符串
std::vector<CString> SplitCString(CString pStr, CString pChar)
{
	std::vector<CString> strVec;
	int curPos = 0;
	CString resToken = pStr.Tokenize(pChar, curPos);

	if (resToken != _T(""))
	{
		strVec.push_back(resToken);
	}
	while(resToken != _T(""))
	{
		resToken = pStr.Tokenize(pChar, curPos);
		if (resToken != _T(""))
		{
			strVec.push_back(resToken);
		}
	}

	return strVec;
};

CString CmdRadarDetection::DrawRadarArea(CString Cmd)
{
	//解析Cmd
	COM::CXmlParser parser;
	parser.Init(Cmd);
	CString DrawIndexStr = parser.GetNodeAttribute(_T("命令"), _T("绘制编号"));
	CString rgbaStr    = parser.GetNodeAttribute(_T("命令"), _T("RGBA"));
	CString leftDisStr   = parser.GetNodeAttribute(_T("命令"), _T("左侧距离"));
	CString rightDisStr  = parser.GetNodeAttribute(_T("命令"), _T("右侧距离"));
	CString offsetStr  = parser.GetNodeAttribute(_T("命令"), _T("偏移值"));
	CString projectNameStr  = parser.GetNodeAttribute(_T("命令"), _T("工程名字"));
	CString channelStr  = parser.GetNodeAttribute(_T("命令"), _T("通道"));
	if ((DrawIndexStr.Compare(_T(""))==0)||(rgbaStr.Compare(_T(""))==0)||
		(leftDisStr.Compare(_T(""))==0)||(rightDisStr.Compare(_T(""))==0)||
		(offsetStr.Compare(_T(""))==0)||(projectNameStr.Compare(_T(""))==0)||
		channelStr.Compare(_T(""))==0)
	{
		return _T("命令错误");
	}

	int DrawIndex = _wtoi(DrawIndexStr);
	float leftDis = _wtof(leftDisStr);
	float rightDis = _wtof(rightDisStr);
	
	std::vector<CString> VecColor1 = SplitCString(rgbaStr, _T(","));
	osg::Vec4f color;
	color.x() = _wtof(VecColor1[0])/255.0;
	color.y() = _wtof(VecColor1[1])/255.0;
	color.z() = _wtof(VecColor1[2])/255.0;
	color.a() = _wtof(VecColor1[3])/255.0;
	std::vector<CString> offsetVec = SplitCString(offsetStr, _T(","));
	osg::Vec3f offset;
	offset.x() = _wtof(offsetVec[0]);
	offset.y() = _wtof(offsetVec[1]);
	offset.z() = _wtof(offsetVec[2]);
	
	
	USES_CONVERSION;
	DataTemplateTab tab;
	std::vector<osg::Vec3f> _vec;
	tab.GetAllPoint(RadarManager::Instance()->getDBHelp(),W2A(projectNameStr),W2A(channelStr),_vec);

	

	//1、根据上述数据，从oracle数据库中获取数据。2、绘制道路。
	//DrawIndex、leftDis、rightDis、color、_vec
	//如果当前颜色的模型已经存在，则删除
	if (m_pDetectRoadModel[DrawIndex].valid())
	{
		bool result = Kernel::GetpKC()->GetTempGroupRoot()->removeChild(m_pDetectRoadModel[DrawIndex].get());
		m_pDetectRoadModel.erase(DrawIndex);
	}

	osg::ref_ptr<DetectRoadModel> pHolesMoel = new DetectRoadModel(_vec, offset, leftDis, rightDis, color);
	pHolesMoel->setMatrix(osg::Matrix::translate(offset));
	m_pDetectRoadModel[DrawIndex] = pHolesMoel;

	//LLA
	osg::Vec3f LLA = _vec[0];
	LLA.z() = 1000;
	EMObj::CmdObjLocate(LLA, 1);

	//把m_pDetectRoadModel添加到场景
	bool status(false);
	if (m_pDetectRoadModel[DrawIndex].valid())
	{
		status = Kernel::GetpKC()->GetTempGroupRoot()->addChild(m_pDetectRoadModel[DrawIndex].get());
	}

	if (!status)
	{
		return _T("失败");
	}
	return _T("成功");
}
CString CmdRadarDetection::ClearRadarArea(CString Cmd)
{
	//解析Cmd
	COM::CXmlParser parser;
	parser.Init(Cmd);
	CString DrawIndexStr = parser.GetNodeAttribute(_T("命令"), _T("绘制编号"));
	if(DrawIndexStr.Compare(_T(""))==0)
	{
		return _T("命令错误，没有绘制编号");
	}
	int DrawIndex = _wtoi(DrawIndexStr);

	bool result(false);
	//如果当前颜色的模型已经存在，则删除
	if (m_pDetectRoadModel[DrawIndex].valid())
	{
		result = Kernel::GetpKC()->GetTempGroupRoot()->removeChild(m_pDetectRoadModel[DrawIndex].get());
		m_pDetectRoadModel.erase(DrawIndex);
	}
	if (!result)
	{
		return _T("失败");
	}
	return _T("成功");
}