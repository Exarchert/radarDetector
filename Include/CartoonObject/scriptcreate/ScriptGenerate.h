/********************************************************
File:			ScriptGenerate.h	ScriptGenerate.cpp
Description:	脚本生成工具
Author:			Liugh
Date:			2011.1.25
*********************************************************/
#pragma once
#include <CartoonObject/Export.h>
#include <osg/Referenced>
#include <CartoonObject/ScriptCreate/Cartoon/COM_Cartoon.h>

namespace CartoonObject
{
	////////////////////////////////////////////////////////////////////////////////////
	//所有动画元素类型
	enum CartType
	{
		Model = 0,		//模型
		Camera,			//相机
		Voice,			//音效
		RunLine,		//运动线
		Table,			//标牌
		Text,			//文字(2D、3D)
		ScrollText,		//滚动文字
		Image,			//图片(屏幕图片、avi、gif、swf)
		Water,			//喷水
		Spray,			//水幕
		Fire,			//火
		Flow,			//流程
		Cmd				//命令(显示对象显隐、脚本播放)
	};

/////////////////////////////////////////////////////////////////////////////////////
	//脚本播放模式,串行模式:	按动画单元顺序播放;	并行模式:	按时间轴播放
	enum PlayMode
	{
		Serial,		//串行
		Parallel	//并行
	};
/////////////////////////////////////////////////////////////////////////////////////
	//存储一个坐标和姿态
	struct PosAndPose
	{
		osg::Vec3d _pos;
		osg::Vec3 _pose;
	};

	//存储一个坐标点与姿态列表与路径全长
	struct PosAndPoseArray
	{
		std::vector<PosAndPose> _posList;
		float	_dist;
	};

	//存储一组点坐标列表与全长
	struct PtArray
	{
		osg::ref_ptr<osg::Vec3Array> _ptList;
		float	_dist;
	};

///////////////////////////////////////////////////////////////////////////////////////////////////
/************************************************************************
	动画脚本创建类
	使用方法：	1、SetGroupName(...)	可不用调用
				2、SetPlayMode(...)		可不用调用，生成脚本默认为并行
				3、调用添加函数：Add...(...)
				4、GenerateScript(...)
************************************************************************/
	class CARTOONOBJECT_EXPORT CScriptGenerate : public osg::Referenced
	{
	public:
		CScriptGenerate();
		~CScriptGenerate();

	public:
		//组名
		void SetGroupName(const CString gpName);

		//0:串行,1:并行
		void SetPlayMode(UINT type = Serial);

		/*
		目的：	脚本生成接口
		参数：	fileName		(in)		存储到硬盘上的文件名称(绝对路径)
		*/
		bool GenerateScript(CString fileName);

	public:
		/*************************************************************************************************
		说明:	1、	资源文件根目录为项目路径下的script目录,脚本中所用到的资源文件均采用相对路径,
					三维坐标点为世界坐标
				2、	关于路径运动目前只支持全程匀速运动,如需变速运动可后期进行扩充
		**************************************************************************************************/

		/*
		目的：	向脚本中添加一个模型动画
		参数：	FilePath			(in)		模型文件路径(相对路径)
				life				(in)		时长
				posAndPoseArray		(in)		模型运动关键点坐标(世界坐标)及每点对应的姿态列表
				btm					(in)		起始时刻
		*/
		bool AddModel(CString FilePath, const float life, PosAndPoseArray posAndPoseArray,const float btm = 0.0f);
		//面向前方
		bool AddModel(CString FilePath, const float life, PtArray ptArray,const float btm = 0.0f);

		/*
		目的：	向脚本中添加一个相机动画
		参数：	life				(in)		时长
				posAndPoseArray		(in)		相机运动关键点坐标(世界坐标)及每点对应的姿态列表
				btm					(in)		起始时刻
		*/
		bool AddCamera(const float life, PosAndPoseArray posAndPoseArray,const float btm = 0.0f);

		/*
		目的：	向脚本中添加一个音效
		参数：	FilePath			(in)		音频文件路径
				life				(in)		时长
				voice				(in)		音量
				pos					(in)		声源坐标
				bBind				(in)		true:	与相机绑定运动
				btm					(in)		起始时刻
		*/
		bool AddVoice(const CString FilePath, const float life, float voice = 0.5, const osg::Vec3 pos = osg::Vec3(0.0,0.0,0.0), bool bBind = false,
			const float btm = 0.0f);

		/*
		目的：	向脚本中添加一个运动线
		参数：	life				(in)		时长
				ptArray				(in)		运动线关键点坐标
				color				(in)		运动线颜色
				lineWidth			(in)		线宽
				btm					(in)		起始时刻
		*/
		bool AddRunLine(const float life, PtArray ptArray, osg::Vec4 color= osg::Vec4(0.0f,1.0f,0.0f,1.0f), 
			float lineWidth = 2.0, const float btm = 0.0f);
		/*
		目的：	向脚本中添加一个标牌
		参数：	content				(in)		显示内容
				life				(in)		时长
				posAndPoseArray		(in)		标牌空间运动关键点坐标
				size				(in)		文字大小
				font				(in)		字体
				offset				(in)		标牌三角尾巴同标注点的偏移量(米)
				color				(in)		文字颜色
				btm					(in)		起始时刻
		*/
		bool AddTable(const CString content, const float life, PtArray ptArray, const float size = 30, 
			CString font = _T("SIMSUN.TTC"), const osg::Vec4 color= osg::Vec4(0.0f,1.0f,0.0f,1.0f), const osg::Vec2 offset= osg::Vec2(0.0,0.0), 
			const float btm = 0.0f);
		/*
		目的：	向脚本中添加一个屏幕文字
		参数：	content				(in)		显示内容
				life				(in)		时长
				screenPos			(in)		文字左下角屏幕坐标
				color				(in)		文字颜色
				size				(in)		文字大小
				font				(in)		字体
				bHasBKRect			(in)		是否开启背景框(true为开启)
				bkColor				(in)		背景框颜色
				btm					(in)		起始时刻
		*/
		bool Add2DText(const CString content, const float life, osg::Vec2 screenPos,const osg::Vec4 color= osg::Vec4(0.0f,1.0f,0.0f,1.0f), 
			const float size = 30, const CString font = _T("SIMSUN.TTC"), const bool bHasBKRect = true, 
			osg::Vec4 bkColor= osg::Vec4(0.5,0.5,0.5,0.5), const float btm = 0.0f);

		/*
		目的：	向脚本中添加一个空间文字
		参数：	content				(in)		显示内容
				life				(in)		时长
				posAndPoseArray		(in)		文字空间运动关键点坐标
				color				(in)		文字颜色
				size				(in)		文字大小
				font				(in)		字体
				bHasBKRect			(in)		是否开启背景框(true为开启)
				bkColor				(in)		背景框颜色
				btm					(in)		起始时刻
		*/
		bool Add3DText(const CString content, const float life, PtArray ptArray, const osg::Vec4 color= osg::Vec4(0.0f,1.0f,0.0f,1.0f), 
			const float size = 30, const CString font = _T("SIMSUN.TTC"), const bool bHasBKRect = true, 
			osg::Vec4 bkColor= osg::Vec4(0.5,0.5,0.5,0.5), const float btm = 0.0f);

		/*
		目的：	向脚本中添加一个滚动文字
		参数：	content				(in)		显示内容
				life				(in)		时长
				screenPos			(in)		相对于文字中心的偏移量
				color				(in)		文字颜色
				size				(in)		文字大小				
				font				(in)		字体
				fieldHeight			(in)		滚动区域高度
				speedf				(in)		滚动速度(像素/s)
				lineSpace			(in)		行间距
				maxLines			(in)		区域容纳的最大行数
				align				(in)		对齐方式,相对于屏幕显示窗口
												osgAPEx::ScrollText::Alignment alignTypes[] = 
												{
													osgAPEx::ScrollText::Left,
													osgAPEx::ScrollText::Right,
													osgAPEx::ScrollText::Center
												};
				bHasBKRect			(in)		是否开启屏幕背景框(true为开启)
				bkColor				(in)		背景框颜色
				btm					(in)		起始时刻
		*/
		bool AddScrollText(CString content, const float life, osg::Vec2 offSet,osg::Vec4 color= osg::Vec4(0.0f,1.0f,0.0f,1.0f), 
			const float size = 30, CString font = _T("SIMSUN.TTC"),const float fieldHeight = 400, const float speedf = 0.05,
			const float lineSpace = 0,const float maxLines = 50, DWORD align = 0, bool bHasBKRect = TRUE,
			osg::Vec4 bkColor= osg::Vec4(0.5,0.5,0.5,0.5), const float btm = 0.0f);

		/*
		目的：	向脚本中添加一个屏幕图片
		参数：	FilePat				(in)		图片资源(bmp、jpg、png等常用静态图片;avi、gif、swf动态图片)
				life				(in)		时长
				width				(in)		图片宽度
				height				(in)		图片高度
				centerMode			(in)		图片中心点
												osgAPEx::CenterMode centerModes[] = 
												{
													osgAPEx::CENTER,
													osgAPEx::BOTTOM_LEFT,
													osgAPEx::BOTTOM_RIGHT,
													osgAPEx::TOP_LEFT,
													osgAPEx::TOP_RIGHT
												};
				screenPos			(in)		屏幕坐标
				bEqualImage			(in)		大小与原图片相同(true为相同，false为width*height)
				bLoop				(in)		动画图片播放是否循环播放(true为循环播放)
				btm					(in)		起始时刻
		*/
		bool AddImage(CString FilePath, const float life, const float width = 100.0f, const float height = 100.0f, 
			DWORD centerMode = 1, osg::Vec2 screenPos = osg::Vec2(100.0f,100.0f), bool bEqualImage = false, 
			bool bLoop = false,const float btm = 0.0f);

		/*
		目的：	向脚本中添加一个喷水
		参数：	全部为输入参数
				CString				FilePath			贴图
				float				life				时长
				double				particleLife 		粒子生命周期
				PosAndPoseArray	posAndPoseArray		空间坐标运动点坐标列表
				float				minNum;				最小粒子数 / 喷射口半径
				float				maxNum;				最大粒子数 / 贴图率
				float				minSpeed;			最小初速度 / 初始速度
				float				maxSpeed;			最大初速度 / 半径增长率
				float				phi;				指定水平范围 / 贴图流速(正数为正向,负数为逆向)
				float				theta;				指定垂直范围 / 喷射仰角
				int					type;				0：离子水;1：圆锥抛物线
				float				dist;				水平距离
				flost				btm					起始时刻
		备注：	参数有两重含义，离子水为第一层定义、圆锥抛物水为第二层含义
		*/
		bool AddWater(CString FilePath, const float life, const float particleLife, PosAndPoseArray posAndPoseArray, const float minNum = 80.0f, const float maxNum = 81.0f, 
			const float minSpeed = 80.0f, const float maxSpeed = 81.0f, const float phi = 2.0f, const float theta = 0.01f,
			const int type = 0, const float dist = 10.0f, const float btm = 0.0f);

		/*
		目的：	向脚本中添加一个水幕
		参数：	life				(in)		时长
				posAndPoseArray		(in)		文字空间运动关键点坐标
				FilePath			(in)		贴图文件名
				repeatRation		(in)		贴图重复率
				speed				(in)		贴图流速
				radius				(in)		圆柱、圆球半径
				cylinderHeight		(in)		圆柱高度
				sphereHeight		(in)		球高度
				btm					(in)		起始时刻
		备注：	水幕模型由半球和圆柱上下累加构成,半球高度不能超过圆柱直径
		*/
		bool AddSpray(const float life, PosAndPoseArray posAndPoseArray, CString FilePath = _T("shui.tga"),
			const float repeatRation = 5.0f, const float speedSpray = 0.01f, const float radius = 2.0f, 
			const float cylinderHeight = 2.0f, const float sphereHeight = 2.0f, const float btm = 0.0f);
		/*
		目的：	向脚本中添加一个火
		参数：	life					(in)		时长
				posAndPoseArray			(in)		空间运动关键点坐标
				fireModel				(in)		火模型文件
				flamePic				(in)		贴图文件名
				particleSystemHeight	(in)		粒子高度
				particleSpeed			(in)		粒子速度
				particleSpread			(in)		粒子宽度
				particleSystemShape		(in)		火焰大小
				particleShape			(in)		火焰片大小
				particleSize			(in)		火焰片间距
				btm						(in)		起始时刻
		备注：	水幕模型由半球和圆柱上下累加构成,半球高度不能超过圆柱直径
		*/
 		bool AddFire(const float life, PosAndPoseArray posAndPoseArray,const CString &fireModel = _T("QuadArray.3ds"),
 			const CString &flamePic  = _T("Flame.tga"), float particleSystemHeight = 25.0f, float particleSpeed = 1.3f,
 			float particleSpread = 10.5f, float particleSystemShape = 0.2f, float particleShape = 1.5f, 
			float particleSize = 3.9f, const float btm = 0.0f);

		/*
		目的：	向脚本中添加一个流程
		参数：	life				(in)		时长
				PipeID				(in)		管线ID
				FlowSpeed			(in)		流速
				bClear				(in)		流完后是否清除流动管线(TRUE:清除)
				Color				(in)		流动管线颜色
				bDirection			(in)		是否逆向流动(TRUE:逆向流动)
				btm					(in)		起始时刻
		*/
  		bool AddFlow(const float life, LONG64 PipeID, float FlowSpeed, float radius, BOOL bClear = TRUE,
 			osg::Vec4 color = osg::Vec4(0.0,0.0,1.0,0.6), BOOL bDirection = FALSE,const float btm = 0.0f);

		/*
		目的：	向脚本中添加一个命令
		参数：	srcCmd				(in)		起始时刻命令内容
				life				(in)		时长
				srcClr				(in)		结束时执行命令内容
				btm					(in)		起始时刻
				strInfo				(in)		备注信息(主要用于桌面推演解析)
		备注：	目前命令支持类型
				1、	显示对象基本操作:
					1>	命令描述：
						<命令 命令名="显示对象基本动作"  ID="" 动作类型="显隐/选中/透明/闪烁/清除所有效果"  参数=""></命令>
					2>	参数说明：
						"参数"：	隐藏 / 显示				0  /  任意正数
									高亮选中、清除效果		任意值（可为空或去掉该节点名）
									闪烁					整数（闪烁次数）
									透明		0~100整数（数值大小与透明度成正比，0为不透，100为全透）
				2、	脚本播放命令：
					1>	播放
						XML描述：
						<命令 命令名="动画" 类型="播放" 脚本名="相对路径"></命令>

		*/
		bool AddCmd(const CString srcCmd, const float life, const CString srcClr= _T(""),const float btm = 0.0f,const CString strInfo = _T(""));

	private:
		osg::ref_ptr<CCOM_CartoonUnit>	m_pCartUnit;
		CString		m_GroupName;
		UINT		m_PlayMode;
	};
}