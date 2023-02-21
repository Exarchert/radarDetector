/*********************************************************************
Copyright (C),  
文件：KernelContext.h KernelContext.cpp

作者：黄志鹏
版本：1.0
日期：2009-11-15
描述：内核当前上下文对象类.

修改记录：
作者					日期				描述
----------------------------------------------------------------------

*********************************************************************/

#pragma once 

#include <Com\Export.h>
#include <osg\ref_ptr>
#include <osg\Referenced>
#include <osg\Node>
#include <osg\Group>
#include <osgViewer\Viewer>
#include <osgViewer\CompositeViewer>
#include <osg\Camera>
#include <osgWidget\WindowManager>
#include <osgGA\GUIEventHandler>
#include <osgGA\MatrixManipulator>
#include <osgVec\VecLayerManagerNode>
//#include <Com\Ramble\RambleManager.h>
#include <Com\Decoration\DecoraManager.h>
#include <Com\Cartoon\CartoonManager.h>
#include <Com\LocateAgent\LocateAgent.h>
#include <Com\Weather\Weather.h>
#include <Com\LightingMan\LightingMan.h>
#include <COM\Event\Event.h>
#include <COM\Command\Cmd.h>
#include <COM\Ramble\RambleMgr.h>
#include <COM\Ramble\BaseRamble.h>

#include <osgShadow/ShadowedScene>

#include <afxmt.h>

namespace COM	{

//Kernel工作的上下文
class COM_EXPORT CKernelContext : public osg::Referenced 
{
public:
	CKernelContext();
	virtual ~CKernelContext();

	void SetStop();

	//设置和获取视景器的渲染窗口
	void SetRenderWnd(HWND hwnd);
	HWND GetRenderWnd() const;

	//设置、获取视图管理器
// 	void SetCompsiteViewer(osgViewer::CompositeViewer* compositeViewer);
// 	osgViewer::CompositeViewer *GetCompositeViewer() const;

	//设置和获取视景器
	void SetSceneViewer(osgViewer::Viewer *viewer);
	//设置立体渲染
	void SetViewerStereo(osg::ref_ptr<osg::DisplaySettings> dis);
	osg::DisplaySettings* GetViewerStereo();
	osgViewer::Viewer *GetSceneViewer() const;

	//获取场景根结点
	osg::Group *GetSceneRoot() const;

	//获取厂区组结点
	osg::Group *GetFieldsRoot() const;

	osg::Group *GetShpLineRoot()const; 

	osg::Group* GetParticleGroup()const;

	osg::Group* GetFieldShadowRoot()  const
	{
	   return 	m_sceneShadow.get();
	}

	//添加和移除结点到(从)场景根结点
	bool AddToSceneRoot(osg::Node *child);
	bool RemoveFromSceneRoot(osg::Node *child);

	//设置和获取地球结点
	void SetEarthNode(osg::Node *earth);
	osg::Node *GetEarthNode() const;

	//获取地球组的根结点(地球组放置地球、地表上的模型)
	osg::Group *GetEarthGroupRoot() const;

	//获取天空组的根结点(天空组放置大气层、星空、天空)
	osg::Group *GetSkyGroupRoot() const;

	//获取临时对象组结点
	osg::Group *GetTempGroupRoot() const;

	//获取动画对象组结点
	osg::Group *GetAnimationGroupRoot() const;

	//获取用户创建模型对象组结点
	osg::Group *GetUserCreateModelRoot() const;

	//开始和停止渲染
	void StartRender();
	void StopRender();

	//设置和获取HUD
	bool SetHudForScreenObjects(osg::Camera *hud);
	osg::Camera *GetHudForScreenObjects() const;
	bool SetHudForWidgets(osg::Camera *hud);
	osg::Camera *GetHudForWidgets() const;

	bool SetHudForScreenObjectsCenterAligned(osg::Camera *hud);
	osg::Camera *GetHudForScreenObjectsCenterAligned() const;

	////设置导航hud
	//bool setHudNavigation(osg::Camera* hud);
	//osg::Camera* getHudNavigation() const;
	//void AddToHudForNavigtion(osg::Node *child);
	//void RemoveFromHudForNavigtion(osg::Node *child);
	//bool setHudNavigation2(osg::Camera* hud);
	//osg::Camera* getHudNavigation2() const;
	//void AddToHudForNavigtion2(osg::Node *child);
	//void RemoveFromHudForNavigtion2(osg::Node *child);

	//添加和删除屏幕对象到(从)HUD
	void AddToHudForScreenObjects(osg::Node *child);
	void RemoveFromHudForScreenObjects(osg::Node *child);
	void AddToHudForWidgets(osg::Node *child);
	void RemoveFromHudForWidgets(osg::Node *child);

	//设置和获取窗口管理
	bool SetWindowManager(osgWidget::WindowManager *wm);
	osgWidget::WindowManager *GetWindowManager() const;

	//添加事件回调处理
	bool AddEventHandler(osgGA::GUIEventHandler *handler);
	bool RemoveEventHandler(osgGA::GUIEventHandler *handler);

	//
	bool InsertEventHandlerFront(osgGA::GUIEventHandler *handler);
	//设置或获取相机操作器
	//bool SetCameraManipulator(osgGA::MatrixManipulator *cameraMan);
	//osgGA::MatrixManipulator *GetCameraManipulator() const;

	//设置和获取漫游管理器
	bool SetRambleManager(COM::CRambleMgr *rambleMan);
	COM::CRambleMgr *GetRambleManager() const;
	COM::CBaseRamble* GetRamble() const;

	//设置和获取装饰对象管理器
	bool SetDecoraManager(COM::CDecoraManager *decoraMan);
	COM::CDecoraManager *GetDecoraManager() const;

	//设置和获取天气管理对象
	bool SetWeatherManager(COM::CWeather *weaMan);
	COM::CWeather *GetWeatherManager() const;

	//设置和获取光照管理对象
	bool SetLightingManager(COM::CLightingMan *lightMan);
	COM::CLightingMan *GetLightingManager() const;

	//设置和获取相机播放器
	bool SetCameraPlayer(COM::CCartoonManager *player);
	COM::CCartoonManager *GetCameraPlayer() const;

	//设置和获取定位代理对象
	bool SetLocateAgent(COM::CLocateAgent *agent);
	COM::CLocateAgent *GetLocateAgent() const;

	//向消息中心投递消息
	void SendMsg(COM::CMessage &Msg);

	//设置应用程序事件，消息队列和消息值
	void SetAppEvent(COM::CMessageQueue *pMsgQueue, UINT uAppMsgValue);

	COM::CEventMsgCenter &GetEventMsgCenter(){return m_EventMsgCenter;}

	//	设置相机裁剪；在CRambleManager使用，在每次切换相机时设置。
	bool SetNearFarCompute(osgGA::MatrixManipulator* manip);

	osgVec::VecLayerManagerNode* getVecNode(){return m_vecLayerMamagerNode;}
	//CTreeCtrl* getVecTree(){return m_tree;}
	
	//根据信号暂停、继续三维线程
	void SetPauseRender(bool bPause);

	//设置地球透明度 transparency透明度设置范围为0-100 徐宝龙2012-12-25
	void setEarthTransparent(int transparency);

	////获取框选管理对象
	//Kernel::Region* GetRegion();

	/************************************************************************/
	/* 剔除碰撞部分管理代码                                                 */
	/************************************************************************/
	void addRejectNode(LONG64 nodeID)
	{
		bool b=true;
		std::vector<LONG64>::iterator nodeIter =m_rejectNode.begin();
		for (;nodeIter!=m_rejectNode.end();nodeIter++)
		{
			if (nodeID == *nodeIter)
			{
				b=false;
			}
		}
		if (b)
		{
			m_rejectNode.push_back(nodeID);
		}
		
	}

	void deleteRejectNode(LONG64 nodeID)
	{
		std::vector<LONG64>::iterator nodeIter =m_rejectNode.begin();
		for (;nodeIter!=m_rejectNode.end();nodeIter++)
		{
			if (nodeID == *nodeIter)
			{
			   m_rejectNode.erase(nodeIter);
			   break;;
			}
		}
		
	}

	std::vector<LONG64> getRejectNode()
	{
		return m_rejectNode;
	}


	/************************************************************************/
	/* 对象状态管理部分代码                                                 */
	/************************************************************************/
	void addActionObjs(LONG64 nodeID)
	{
		bool b=true;
		std::vector<LONG64>::iterator nodeIter =m_ActionObjs.begin();
		for (;nodeIter!=m_ActionObjs.end();nodeIter++)
		{
			if (nodeID == *nodeIter)
			{
				b=false;
			}
		}
		if (b)
		{
			m_ActionObjs.push_back(nodeID);
		}

	}

	void deleteActionObjs(LONG64 nodeID)
	{
		std::vector<LONG64>::iterator nodeIter =m_ActionObjs.begin();
		for (;nodeIter!=m_ActionObjs.end();nodeIter++)
		{
			if (nodeID == *nodeIter)
			{
				m_ActionObjs.erase(nodeIter);
				break;;
			}
		}

	}

	std::vector<LONG64> getActionObjs()
	{
		return m_ActionObjs;
	}
	std::vector<LONG64> m_ActionObjs;

	/************************************************************************/
	/* 选中对象管理部分代码                                                 */
	/************************************************************************/
	void addSelectObjs(LONG64 nodeID)
	{
		bool b=true;
		std::vector<LONG64>::iterator nodeIter =m_SelectObjs.begin();
		for (;nodeIter!=m_SelectObjs.end();nodeIter++)
		{
			if (nodeID == *nodeIter)
			{
				b=false;
			}
		}
		if (b)
		{
			m_SelectObjs.push_back(nodeID);
		}
	}

	void deleteSelectObjs(LONG64 nodeID)
	{
		std::vector<LONG64>::iterator nodeIter =m_SelectObjs.begin();
		for (;nodeIter!=m_SelectObjs.end();nodeIter++)
		{
			if (nodeID == *nodeIter)
			{
				m_SelectObjs.erase(nodeIter);
				break;;
			}
		}

	}

	void deleteAllSelect()
	{
		m_SelectObjs.clear();
	}

	std::vector<LONG64> getSelectObjs()
	{
		return m_SelectObjs;
	}
	std::vector<LONG64> m_SelectObjs;
	//
	/************************************************************************/
	/* 标牌管理部分代码                                                 */
	/************************************************************************/
	void addScutheonObjs(LONG64 nodeID)
	{
		bool b=true;
		std::vector<LONG64>::iterator nodeIter =m_ScutheonObjs.begin();
		for (;nodeIter!=m_ScutheonObjs.end();nodeIter++)
		{
			if (nodeID == *nodeIter)
			{
				b=false;
			}
		}
		if (b)
		{
			m_ScutheonObjs.push_back(nodeID);
		}

	}

	void deleteScutheonObjs(LONG64 nodeID)
	{
		std::vector<LONG64>::iterator nodeIter =m_ScutheonObjs.begin();
		for (;nodeIter!=m_ScutheonObjs.end();nodeIter++)
		{
			if (nodeID == *nodeIter)
			{
				m_ScutheonObjs.erase(nodeIter);
				break;;
			}
		}

	}

	void SetShowSetModelInfoDlg(bool flag);
	bool GetShowSetModelInfoDlg();

	std::vector<LONG64> getScutheonObjs()
	{
		return m_ScutheonObjs;
	}
	std::vector<LONG64> m_ScutheonObjs;
	//
protected:
	void Reset();
	void SetSceneShadow();
protected:
	HWND								m_hwnd;			//视景器的渲染窗口
//	osg::ref_ptr<osgViewer::CompositeViewer> m_CompositeViewer;	//视图管理器
	osg::ref_ptr<osgViewer::Viewer>		m_viewer;		//视景器
	static osg::DisplaySettings* m_dis;           //立体渲染模式  
	osg::ref_ptr<osg::Group>			m_sceneRoot;	//场景图根结点,所有场景要加在该结点上
	osg::ref_ptr<osg::Group>			m_sceneModels;	//厂区模型的放置结点

	osg::ref_ptr<osgShadow::ShadowedScene> m_sceneShadow;//

	osg::ref_ptr<osg::Group>             m_shpRootGroup;
	osg::ref_ptr<osg::Group>            m_ParticleGroup; //粒子系统
	osg::ref_ptr<osg::Group>            m_AnimationGroup; //动画节点
	osg::ref_ptr<osg::Group>            m_userModelGroup; //用户模型节点

	osg::ref_ptr<osg::Node>				m_earth;		//地球结点
	osg::ref_ptr<osg::Group>			m_earthGroup;	//地球组根结点---下面挂 m_earth
	osg::ref_ptr<osg::Group>			m_skyGroup;		//天空组根结点
	osg::ref_ptr<osg::Group>			m_tempGroup;	//临时对象组结点

	osg::ref_ptr<osg::Camera>			m_hudWidget;	//供Widget使用的HUD
	osg::ref_ptr<osg::Camera>			m_hudScrObj;	//供屏幕显示对象（文字、图片）使用的HUD
	osg::ref_ptr<osg::Camera>			m_hudScrObjCenterAligned;
	//osg::ref_ptr<osg::Camera>           m_hudNavigtion;  //供导航使用
	//osg::ref_ptr<osg::Camera>           m_hudNavigtion2;  //生成导航纹理

	osg::ref_ptr<osgWidget::WindowManager>	m_wm;		//Widgets窗口管理
	
	osg::ref_ptr<osgVec::VecLayerManagerNode>	m_vecLayerMamagerNode;
	//CTreeCtrl*									m_tree;
	//osg::ref_ptr<osgGA::MatrixManipulator>	m_cameraMan;//相机操作器
	//osg::ref_ptr<COM::CRambleManager>	m_rambleMan;	//漫游管理器
	osg::ref_ptr<COM::CRambleMgr>		m_rambleMan;	//漫游管理器

	osg::ref_ptr<COM::CDecoraManager>	m_decoraMan;	//装饰对象管理器
	osg::ref_ptr<COM::CWeather>			m_weatherMan;	//天气管理对象
	osg::ref_ptr<COM::CLightingMan>		m_lightMan;		//光照管理对象

	osg::ref_ptr<COM::CCartoonManager>	m_cameraPlayer;	//相机播放器
	osg::ref_ptr<COM::CLocateAgent>		m_locateAgent;	//定位代理对象

	//Kernel::Region*        m_region;        //设置框选类

	//事件消息中心
	COM::CEventMsgCenter				m_EventMsgCenter;

	//供窗口线程使用的消息队列
	COM::CMessageQueue*					m_pAppMsgQueue;
	//应用程序响应消息
	UINT								m_uAppMsgValue;

	//信号量
	CEvent	m_PauseRenderEvent;
	bool	m_bPause;
	bool    m_bShowSetModelInfoDlg;//设置模型信息的对话框是否显示--EM_WorkStyle_OCX_LocalResDD 这个宏唯一使用

	std::vector<LONG64>                 m_rejectNode;
public:
	//工作模式
	enum{
		EM_WorkStyle_UnDef,		
		EM_WorkStyle_ObjEdid,				//显示元素--编辑状态
		EM_WorkStyle_CartoonEdit,			//动画编辑状态
		EM_WorkStyle_TestOCXViewer,			//测试视口
		EM_WorkStyle_OCXViewer,				//客户端视口--网络资源
		EM_WorkStyle_OCXViewer_LocalRes,	//客户端视口--本地资源
		EM_WorkStyle_OCX_LocalResDD,		//客户端视口--本地资源
	};
	DWORD m_WorkStyle;

	//命令中心
	COM::CCmdCenter					m_CmdCenter;
	
	//可执行文件路径
	CString m_BinPath;

	//配置文件
	CString m_SysConfigFile;

	//项目名称
	CString m_ProName;

	//资源类型，1 表示网络加载，2 表示本地加载
	DWORD m_ProResType;

	//项目资源网址
	CString m_ProUrl;

	//项目资源网址
	CString m_ProResUrl;

	//Cache目录
	CString m_CachePath;

	//项目路径
	CString m_ProPath;

	//项目资源路径 = m_ProPath + "\\Res"
	CString m_ProResPath;

	//地球文件
	CString m_EarthFile;

	//版本
	CString m_Version;

	//显卡信息
	// 	printf("OpenGL实现厂商的名字：%s\n", name);
	// 	printf("渲染器标识符：%s\n", biaoshifu);
	// 	printf("OOpenGL实现的版本号：%s\n",OpenGLVersion );
	// 	printf("OGLU工具库版本：%s\n", gluVersion);
	std::string m_glSupporName;
	std::string m_glRender;
	std::string m_glVersion;
	std::string m_gluVersion;

public:
	//四叉树的最大深度
	int MAX_DEPTH ;

	//每个四叉树包含的最大模型数目
	int MAX_CHILDREN_NUMBER ;

	int lodLength ; //包围球半径的倍数
};

}