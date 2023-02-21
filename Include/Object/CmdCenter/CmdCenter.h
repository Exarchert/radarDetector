/*************************************************
file:			CmdCenter.h
Author:			��ƽ    
Date:			2009-12-08
Description:    ��������

*************************************************/
#pragma once
#include <Object\Export.h>
#include <COM\Command\Cmd.h>
#include <object\Obj.h>
#include <Object/Shp/ShpModelManager.h>
#include <vector>
namespace EMObj{
	/*
	����λ
	*/
	OBJECT_EXPORT void CmdObjLocate(LONG64 ID, DWORD LocateStyle = 1);
	OBJECT_EXPORT void CmdObjLocate(osg::Vec3d EyeLLA, DWORD LocateStyle = 1);

	/*
	�����鶨λ
	*/
	OBJECT_EXPORT void CmdObjVectLocate(std::vector<CString> IDs, DWORD LocateStyle=1);

	/*
	��ҵ��λ
	*/
	OBJECT_EXPORT void CmdEnterLocate(LONG64 ID, DWORD LocateStyle = 1);


	/*
	������
	*/
	OBJECT_EXPORT void CmdObjAction(LONG64 ID, DWORD ActionType, DWORD Value = 0);
	OBJECT_EXPORT void CmdObjAction(LONG64 ID, DWORD ActionType, std::vector<float> Value);

	/*
	�½�����
	*/
	OBJECT_EXPORT void CmdNewObj(int EnterID, LONG64 TypeID, LONG64 ParentID = 0);

	/*
	�½����������Ӧ����Ϣ
	*/
	OBJECT_EXPORT void CmdNewAddObj(Data::CRowShowObj rowObj);
	OBJECT_EXPORT void CmdNewAddObjData(Data::CRowShowObj rowObj);
	/*
	Liugh: �����ʾ����
	*/
	OBJECT_EXPORT void CmdNewObj(osg::ref_ptr<Data::CRowShowObj> pRowObj);
	/*
	Liugh: �޸���ʾ����
	*/
	OBJECT_EXPORT void CmdUpdateObj(LONG64 id,osg::Vec3d pos);
	OBJECT_EXPORT void CmdUpdateObjPoseAngle(LONG64 id,osg::Vec3d poseAngle);
	//��ʼ�ܵ�����
	OBJECT_EXPORT void CmdNewRunPipeObj(osg::ref_ptr<Data::CRowShowObj> pRowObj);
	
	OBJECT_EXPORT void CmdNewObjFromWeb(osg::ref_ptr<Data::CRowShowObj> pRowObj);


	/*
	����
	*/
	OBJECT_EXPORT void CmdDelSceenObj();

	/*
	ɾ������
	*/
	OBJECT_EXPORT void CmdDelObj(LONG64 ID);

	

	/*
	�����༭�Ի���
	*/
	OBJECT_EXPORT void CmdShowObjEditDlg(LONG64 ObjID);
	OBJECT_EXPORT void CmdShowObjEditDlg(CObj *pObj = NULL);

	OBJECT_EXPORT void CmdNotifyObjSelected(CObj *pObj);

	/*
	�������������
	*/
	OBJECT_EXPORT void CmdShowObjManagerDlg();

	/*
	���ó����ڵ�λ��
	*/
	OBJECT_EXPORT void CmdSetEnterSceneNodeMT(LONG64 EnterID, osg::Vec3d LLA, osg::Vec3f YPR, osg::Vec3 Scale = osg::Vec3(1,1,1));

	///*
	//�����ҵ�ڵ㣬�����޸����ݿ�
	//*/
	//OBJECT_EXPORT void CmdAddEnter(LONG64 EnterID, osg::Vec3d LLA, osg::Vec3 YPR, osg::Vec3 Scale);

	//����ˢ��
	OBJECT_EXPORT void CmdObjRefresh(EMObj::CObj * pObj, DWORD Para1, DWORD Para2);
	//�����༭
	OBJECT_EXPORT void CmdObjEndEdit(EMObj::CObj * pObj);
	//�洢���ݵ����ݿ�
	OBJECT_EXPORT void CmdObjSaveDataToDB(EMObj::CObj * pObj, DWORD Para);

	//���볤�����������
	OBJECT_EXPORT void CmdLongPipeDataDlg();

	//����ģ�����������첽����
	OBJECT_EXPORT void CmdModelDataImport();

	//��ҵ����
	OBJECT_EXPORT void CmdEnterpriseDlg();

	//��ά�������
	OBJECT_EXPORT void CmdObjectManagerDlg();

	//ͼ�����
	OBJECT_EXPORT void CmdLayerManagerDlg();

	//��������
	OBJECT_EXPORT void CmdWeatherDlg();

	//���չ���
	OBJECT_EXPORT void CmdLightDlg();
		
	//���β���
	OBJECT_EXPORT void CmdSurveyDlg();

	//���
	OBJECT_EXPORT void CmdEmbankmentDlg();

	//�������ģ��
	OBJECT_EXPORT void CmdQuickAddModelDlg();

	//�������
	OBJECT_EXPORT void CmdProfileAnalysisDlg();

	//��ҵ�鿴
	OBJECT_EXPORT void CmdEnterBrowerDlg();


	//����������Ļ��ע
	OBJECT_EXPORT void CmdScreenMarkImportDlg();

	//����ɾ������
	OBJECT_EXPORT void CmdPopDlgToBatDelObj();

	//�ļ�ѹ����ѹ����
	OBJECT_EXPORT void CmdZipAndUnZipDlg();
	//��Ŀ��Դ����
	OBJECT_EXPORT void CmdTogetherProRes();
	
	//OBJECT_EXPORT void CmdBeginEditHotArea();

	//OBJECT_EXPORT void CmdEndingHotAreaEdit();

	//OBJECT_EXPORT void CmdDetectionHotArea();
	//���ڹ��ߵ��빤��
	OBJECT_EXPORT void CmdInternalPipeImportDlg();

	//��������
	OBJECT_EXPORT void CmdShowXmlLabel(LONG64 ID, BOOL bShow, osg::Vec3d pos, BOOL posValid = TRUE);

	/*
	����XML���ݴ�����ʾ����
	ID		(in)	��ʾ�����ID��
	strXML	(in)	��ʾ�������ʾ�����ַ����������ʽ��XML�����ַ���
	*/
	OBJECT_EXPORT bool CmdNewObjByXML(LONG64 ID, CString strXML);

	/*
	����(����/����)����λ��
	*/
	OBJECT_EXPORT bool CmdLightResetPos(osg::Vec3d pos,unsigned int mode = 0);

	/*
	������Ӱ
	*/
	OBJECT_EXPORT bool CmdShadowOper(float ftime ,bool isSwitch);


	/*
	����������	���ص�ǰ��Ŀ�ű���Ŀ¼�¶����ű��б�
	������		strList(out)
	Author:		Liugh
	Data:		2010.5.21
	*/
	OBJECT_EXPORT  bool GetCartoonListRoot(std::vector<CString>& ScriptVec);
	/*
	����������	���ص�ǰ��ĿĿ¼�����ж����ű��б�
	������		ScriptVec(out)
	Author:		Liugh
	Data:		2010.5.21
	*/
	OBJECT_EXPORT  bool GetCartoonListFromMultipleFolder(std::vector<CString>& ScriptVec);
	/*
	����������	���������������ݿ�Ŀ¼�б�
	������		DeskVec(out)
	Author:		Liugh
	Data:		2011.3.24
	*/
	OBJECT_EXPORT bool GetDeskTestList(std::vector<CString>& DeskVec);
	/*
	����������	�������ݿ�
	������		strFolder(in)
	Author:		Liugh
	Data:		2011.3.25
	*/
	OBJECT_EXPORT bool CreateNewDeskTestFolder(CString strFolder);
	/*
	����������	�Ƴ����ݿ�
	������		strFolder(in)
	Author:		Liugh
	Data:		2011.3.25
	*/
	OBJECT_EXPORT bool RemoveDeskTestFolder(CString strFolder);


	/*
	��������:	��ȡ��ά��ʾ����ռ�λ����Ϣ(��ԡ�����)
	Author:		Liugh
	Data:		2010.7.16
	*/
	OBJECT_EXPORT bool GetObjectPosition(LONG64 ObjID,osg::Vec3d& pos,bool bLocal = true);

	/*
	��������:	��ȡ��ά��ʾ����γ��
	Author:		Liugh
	Data:		2010.8.04
	*/
	OBJECT_EXPORT bool GetObjectLLA(LONG64 ObjID,osg::Vec3d& pos);
		/*
	��������:	��ȡ��ά��ʾ����XYZ
	Author:		�챦��
	Data:		2013-09-26
	*/
	OBJECT_EXPORT bool GetObjectXYZ(LONG64 ObjID,osg::Vec3d& pos);

	/*
	��������:	ͨ����Ļ����X��Y��ȡͬ��ά�����Ӧ��γ��
	Author:		Liugh
	Data:		2010.8.04
	*/
	OBJECT_EXPORT bool GetLLAFromXY(float x,float y,osg::Vec3d& pos);
	/*
	��������:	��ѡ�������ر�
	Author:		xubaolong
	Data:		2013-03-07
	*/
	//OBJECT_EXPORT void CmdRegionObj(bool YESNO);
	/*
	��������:	����ʸ������ģ��
	Author:		xubaolong
	Data:		2013-04-07
	*/
	OBJECT_EXPORT void StartLoadShp(std::string filePath ,bool isComputer);
	 /*
	��������   ����ʸ��ͼ�㿪��
	Author:		xubaolong
	Data:		2013-04-24
	*/
	OBJECT_EXPORT void ShpLayerSwitch(std::string layerId,bool isSwitch=true);
	OBJECT_EXPORT void AddShpPicLayer(std::string layerId,EMObj::PicLayerConfig picFilePath);

	OBJECT_EXPORT void AddShpPicNode( std::string layerId,std::vector<EMObj::ShpPicNodeConfig> spncs);


	OBJECT_EXPORT void CmdStreetscape();
}
