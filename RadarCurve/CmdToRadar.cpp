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

//��ȡ������������
CString CmdRadarDetection::GetName()
{
	return _T("�״�");
}

///ִ������
CString CmdRadarDetection::ExecuteCmd(CString Cmd)
{
	COM::CXmlParser parser;
	CString szTemp;

	CString objectId ;
	parser.Init(Cmd);
	szTemp = parser.GetNodeAttribute(_T("����"), _T("����"));

	if(szTemp.GetLength() <= 0)
	{
 		return _T("������Ч!");
	}
	else
	{
		
	}
	//��������ַ�
	if (szTemp.Compare(_T("��ʼ��RADARMANAGER"))==0)
	{
		Radar::CmdRadarManagerInit();
	} 
	else if(szTemp.Compare(_T("��ʼ�״﹤��"))==0)
	{
		Radar::CmdStartRadarWorkDlg();
	}
	else if(szTemp.Compare(_T("ֹͣ�״﹤��"))==0)
	{
		Radar::CmdEndRadarWorkDlg();
	}
	else if(szTemp.Compare(_T("�״�ʵʱ����"))==0)
	{
		Radar::CmdRadarRealTimeCurves();
	}
	else if(szTemp.Compare(_T("�״���ʷ����"))==0)
	{
		Radar::CmdRadarHistoryCurves();
	}
	else if(szTemp.Compare(_T("GPSʵʱ��λ"))==0)
	{
		Radar::CmdGPSRealTimeLocate();
	}
	else if(szTemp.Compare(_T("�״��������"))==0)
	{
		Radar::CmdRadarParametersSettings();
	}
	else if(szTemp.Compare(_T("�״����ݴ���"))==0)
	{
		Radar::CmdRadarDataProcess();
	}
	else if(szTemp.Compare(_T("�״����ݱ�����ʾ"))==0)
	{
		Radar::CmdRadarDataFormsShow();
	}
	else if(szTemp.Compare(_T("�����״﹤��"))==0)
	{
		CString strPath = parser.GetNodeAttribute(_T("����"), _T("����·��"));
		CString strPROJECTCREATETIME = parser.GetNodeAttribute(_T("����"), _T("���̴���ʱ��"));
		Radar::CmdExportRadarProject(strPath,strPROJECTCREATETIME);
	}
	else if(szTemp.Compare(_T("�����״﹤��"))==0)
	{
		CString strFileAllPath = parser.GetNodeAttribute(_T("����"), _T("�ļ�ȫ·��"));
		return Radar::CmdImportRadarProject(strFileAllPath);
	}
	else if(szTemp.Compare(_T("ɾ���״﹤��"))==0)
	{
		CString strPROJECTCREATETIME = parser.GetNodeAttribute(_T("����"), _T("���̴���ʱ��"));
		if (true == Radar::CmdDeleteRadarProject(strPROJECTCREATETIME))
		{
			return _T("<������Ϣ ɾ���״﹤��='�ɹ�'></������Ϣ>");
		}
		else
		{
			return _T("<������Ϣ ɾ���״﹤��='ʧ��'></������Ϣ>");
		}
	}	
	else if(szTemp.Compare(_T("��Ƶ����"))==0)
	{
		Radar::CmdVideoPlay();
	}
	else if(szTemp.Compare(_T("��Ƶ�ط�"))==0)
	{
		Radar::CmdVideoPlayback();
	}
	else if(szTemp.Compare(_T("�״�̽���������"))==0)
	{
		DrawRadarArea(Cmd);
	}
	else if(szTemp.Compare(_T("�״�̽�������"))==0)
	{
		ClearRadarArea(Cmd);
	}
	else if(szTemp.Compare(_T("ѡ������GPR"))==0)
	{
		Radar::SelectCreateGPR();
	}
	return _T("");
}

//�ָ��ַ���
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
	//����Cmd
	COM::CXmlParser parser;
	parser.Init(Cmd);
	CString DrawIndexStr = parser.GetNodeAttribute(_T("����"), _T("���Ʊ��"));
	CString rgbaStr    = parser.GetNodeAttribute(_T("����"), _T("RGBA"));
	CString leftDisStr   = parser.GetNodeAttribute(_T("����"), _T("������"));
	CString rightDisStr  = parser.GetNodeAttribute(_T("����"), _T("�Ҳ����"));
	CString offsetStr  = parser.GetNodeAttribute(_T("����"), _T("ƫ��ֵ"));
	CString projectNameStr  = parser.GetNodeAttribute(_T("����"), _T("��������"));
	CString channelStr  = parser.GetNodeAttribute(_T("����"), _T("ͨ��"));
	if ((DrawIndexStr.Compare(_T(""))==0)||(rgbaStr.Compare(_T(""))==0)||
		(leftDisStr.Compare(_T(""))==0)||(rightDisStr.Compare(_T(""))==0)||
		(offsetStr.Compare(_T(""))==0)||(projectNameStr.Compare(_T(""))==0)||
		channelStr.Compare(_T(""))==0)
	{
		return _T("�������");
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

	

	//1�������������ݣ���oracle���ݿ��л�ȡ���ݡ�2�����Ƶ�·��
	//DrawIndex��leftDis��rightDis��color��_vec
	//�����ǰ��ɫ��ģ���Ѿ����ڣ���ɾ��
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

	//��m_pDetectRoadModel��ӵ�����
	bool status(false);
	if (m_pDetectRoadModel[DrawIndex].valid())
	{
		status = Kernel::GetpKC()->GetTempGroupRoot()->addChild(m_pDetectRoadModel[DrawIndex].get());
	}

	if (!status)
	{
		return _T("ʧ��");
	}
	return _T("�ɹ�");
}
CString CmdRadarDetection::ClearRadarArea(CString Cmd)
{
	//����Cmd
	COM::CXmlParser parser;
	parser.Init(Cmd);
	CString DrawIndexStr = parser.GetNodeAttribute(_T("����"), _T("���Ʊ��"));
	if(DrawIndexStr.Compare(_T(""))==0)
	{
		return _T("�������û�л��Ʊ��");
	}
	int DrawIndex = _wtoi(DrawIndexStr);

	bool result(false);
	//�����ǰ��ɫ��ģ���Ѿ����ڣ���ɾ��
	if (m_pDetectRoadModel[DrawIndex].valid())
	{
		result = Kernel::GetpKC()->GetTempGroupRoot()->removeChild(m_pDetectRoadModel[DrawIndex].get());
		m_pDetectRoadModel.erase(DrawIndex);
	}
	if (!result)
	{
		return _T("ʧ��");
	}
	return _T("�ɹ�");
}