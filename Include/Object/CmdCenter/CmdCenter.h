/*************************************************
file:			CmdCenter.h
Author:			张平    
Date:			2009-12-08
Description:    命令中心

*************************************************/
#pragma once
#include <Object\Export.h>
#include <COM\Command\Cmd.h>
#include <object\Obj.h>
#include <Object/Shp/ShpModelManager.h>
#include <vector>
namespace EMObj{
	/*
	对象定位
	*/
	OBJECT_EXPORT void CmdObjLocate(LONG64 ID, DWORD LocateStyle = 1);
	OBJECT_EXPORT void CmdObjLocate(osg::Vec3d EyeLLA, DWORD LocateStyle = 1);

	/*
	对象组定位
	*/
	OBJECT_EXPORT void CmdObjVectLocate(std::vector<CString> IDs, DWORD LocateStyle=1);

	/*
	企业定位
	*/
	OBJECT_EXPORT void CmdEnterLocate(LONG64 ID, DWORD LocateStyle = 1);


	/*
	对象动作
	*/
	OBJECT_EXPORT void CmdObjAction(LONG64 ID, DWORD ActionType, DWORD Value = 0);
	OBJECT_EXPORT void CmdObjAction(LONG64 ID, DWORD ActionType, std::vector<float> Value);

	/*
	新建对象
	*/
	OBJECT_EXPORT void CmdNewObj(int EnterID, LONG64 TypeID, LONG64 ParentID = 0);

	/*
	新建对象并添加响应的信息
	*/
	OBJECT_EXPORT void CmdNewAddObj(Data::CRowShowObj rowObj);
	OBJECT_EXPORT void CmdNewAddObjData(Data::CRowShowObj rowObj);
	/*
	Liugh: 添加显示对象
	*/
	OBJECT_EXPORT void CmdNewObj(osg::ref_ptr<Data::CRowShowObj> pRowObj);
	/*
	Liugh: 修改显示对象
	*/
	OBJECT_EXPORT void CmdUpdateObj(LONG64 id,osg::Vec3d pos);
	OBJECT_EXPORT void CmdUpdateObjPoseAngle(LONG64 id,osg::Vec3d poseAngle);
	//开始管道动画
	OBJECT_EXPORT void CmdNewRunPipeObj(osg::ref_ptr<Data::CRowShowObj> pRowObj);
	
	OBJECT_EXPORT void CmdNewObjFromWeb(osg::ref_ptr<Data::CRowShowObj> pRowObj);


	/*
	清屏
	*/
	OBJECT_EXPORT void CmdDelSceenObj();

	/*
	删除对象
	*/
	OBJECT_EXPORT void CmdDelObj(LONG64 ID);

	

	/*
	弹出编辑对话框
	*/
	OBJECT_EXPORT void CmdShowObjEditDlg(LONG64 ObjID);
	OBJECT_EXPORT void CmdShowObjEditDlg(CObj *pObj = NULL);

	OBJECT_EXPORT void CmdNotifyObjSelected(CObj *pObj);

	/*
	弹出对象管理器
	*/
	OBJECT_EXPORT void CmdShowObjManagerDlg();

	/*
	设置厂区节点位置
	*/
	OBJECT_EXPORT void CmdSetEnterSceneNodeMT(LONG64 EnterID, osg::Vec3d LLA, osg::Vec3f YPR, osg::Vec3 Scale = osg::Vec3(1,1,1));

	///*
	//添加企业节点，并且修改数据库
	//*/
	//OBJECT_EXPORT void CmdAddEnter(LONG64 EnterID, osg::Vec3d LLA, osg::Vec3 YPR, osg::Vec3 Scale);

	//对象刷新
	OBJECT_EXPORT void CmdObjRefresh(EMObj::CObj * pObj, DWORD Para1, DWORD Para2);
	//结束编辑
	OBJECT_EXPORT void CmdObjEndEdit(EMObj::CObj * pObj);
	//存储数据到数据库
	OBJECT_EXPORT void CmdObjSaveDataToDB(EMObj::CObj * pObj, DWORD Para);

	//导入长距离管线数据
	OBJECT_EXPORT void CmdLongPipeDataDlg();

	//导入模型数据用以异步加载
	OBJECT_EXPORT void CmdModelDataImport();

	//企业管理
	OBJECT_EXPORT void CmdEnterpriseDlg();

	//三维对象管理
	OBJECT_EXPORT void CmdObjectManagerDlg();

	//图层管理
	OBJECT_EXPORT void CmdLayerManagerDlg();

	//天气管理
	OBJECT_EXPORT void CmdWeatherDlg();

	//光照管理
	OBJECT_EXPORT void CmdLightDlg();
		
	//地形测量
	OBJECT_EXPORT void CmdSurveyDlg();

	//挖填方
	OBJECT_EXPORT void CmdEmbankmentDlg();

	//快速添加模型
	OBJECT_EXPORT void CmdQuickAddModelDlg();

	//剖面分析
	OBJECT_EXPORT void CmdProfileAnalysisDlg();

	//企业查看
	OBJECT_EXPORT void CmdEnterBrowerDlg();


	//批量导入屏幕标注
	OBJECT_EXPORT void CmdScreenMarkImportDlg();

	//批量删除工具
	OBJECT_EXPORT void CmdPopDlgToBatDelObj();

	//文件压缩解压工具
	OBJECT_EXPORT void CmdZipAndUnZipDlg();
	//项目资源整合
	OBJECT_EXPORT void CmdTogetherProRes();
	
	//OBJECT_EXPORT void CmdBeginEditHotArea();

	//OBJECT_EXPORT void CmdEndingHotAreaEdit();

	//OBJECT_EXPORT void CmdDetectionHotArea();
	//场内管线导入工具
	OBJECT_EXPORT void CmdInternalPipeImportDlg();

	//弹出标牌
	OBJECT_EXPORT void CmdShowXmlLabel(LONG64 ID, BOOL bShow, osg::Vec3d pos, BOOL posValid = TRUE);

	/*
	根据XML数据创建显示对象
	ID		(in)	显示对象的ID号
	strXML	(in)	显示对象的显示描述字符串，具体格式见XML描述字符串
	*/
	OBJECT_EXPORT bool CmdNewObjByXML(LONG64 ID, CString strXML);

	/*
	设置(地球/厂区)光照位置
	*/
	OBJECT_EXPORT bool CmdLightResetPos(osg::Vec3d pos,unsigned int mode = 0);

	/*
	设置阴影
	*/
	OBJECT_EXPORT bool CmdShadowOper(float ftime ,bool isSwitch);


	/*
	功能描述：	返回当前项目脚本根目录下动画脚本列表
	参数：		strList(out)
	Author:		Liugh
	Data:		2010.5.21
	*/
	OBJECT_EXPORT  bool GetCartoonListRoot(std::vector<CString>& ScriptVec);
	/*
	功能描述：	返回当前项目目录下所有动画脚本列表
	参数：		ScriptVec(out)
	Author:		Liugh
	Data:		2010.5.21
	*/
	OBJECT_EXPORT  bool GetCartoonListFromMultipleFolder(std::vector<CString>& ScriptVec);
	/*
	功能描述：	返回所有桌面推演库目录列表
	参数：		DeskVec(out)
	Author:		Liugh
	Data:		2011.3.24
	*/
	OBJECT_EXPORT bool GetDeskTestList(std::vector<CString>& DeskVec);
	/*
	功能描述：	创建推演库
	参数：		strFolder(in)
	Author:		Liugh
	Data:		2011.3.25
	*/
	OBJECT_EXPORT bool CreateNewDeskTestFolder(CString strFolder);
	/*
	功能描述：	移除推演库
	参数：		strFolder(in)
	Author:		Liugh
	Data:		2011.3.25
	*/
	OBJECT_EXPORT bool RemoveDeskTestFolder(CString strFolder);


	/*
	功能描述:	获取三维显示对象空间位置信息(相对、绝对)
	Author:		Liugh
	Data:		2010.7.16
	*/
	OBJECT_EXPORT bool GetObjectPosition(LONG64 ObjID,osg::Vec3d& pos,bool bLocal = true);

	/*
	功能描述:	获取三维显示对象经纬高
	Author:		Liugh
	Data:		2010.8.04
	*/
	OBJECT_EXPORT bool GetObjectLLA(LONG64 ObjID,osg::Vec3d& pos);
		/*
	功能描述:	获取三维显示对象XYZ
	Author:		徐宝龙
	Data:		2013-09-26
	*/
	OBJECT_EXPORT bool GetObjectXYZ(LONG64 ObjID,osg::Vec3d& pos);

	/*
	功能描述:	通过屏幕坐标X、Y获取同三维交点对应经纬高
	Author:		Liugh
	Data:		2010.8.04
	*/
	OBJECT_EXPORT bool GetLLAFromXY(float x,float y,osg::Vec3d& pos);
	/*
	功能描述:	框选启动，关闭
	Author:		xubaolong
	Data:		2013-03-07
	*/
	//OBJECT_EXPORT void CmdRegionObj(bool YESNO);
	/*
	功能描述:	加载矢量数据模型
	Author:		xubaolong
	Data:		2013-04-07
	*/
	OBJECT_EXPORT void StartLoadShp(std::string filePath ,bool isComputer);
	 /*
	功能描述   控制矢量图层开关
	Author:		xubaolong
	Data:		2013-04-24
	*/
	OBJECT_EXPORT void ShpLayerSwitch(std::string layerId,bool isSwitch=true);
	OBJECT_EXPORT void AddShpPicLayer(std::string layerId,EMObj::PicLayerConfig picFilePath);

	OBJECT_EXPORT void AddShpPicNode( std::string layerId,std::vector<EMObj::ShpPicNodeConfig> spncs);


	OBJECT_EXPORT void CmdStreetscape();
}
