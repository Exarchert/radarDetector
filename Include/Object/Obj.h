/*************************************************
file:			Object.h
Author:			张平    
Date:			2009-03-32
Description:    对象类，所有对象的基类

--------------------------------------------------
modify:      谢荣耀
Date:        2009-11-27
Description: 添加获取RowShowObj的接口GetRowShowObj

*************************************************/
#pragma once

//osg
#include <OSG/ref_ptr>
#include <OSG/MatrixTransform>
#include <OSG/Switch>
#include <OSG/Geode>
#include <OSG/Vec3d>
#include <OSG/Vec3f>
#include <osg\StateSet>
#include <osg\Material>
#include <osg\NodeVisitor>
#include <osgFX/Effect>

//C++
#include <vector>

//self
#include <Data\RowStruct\RowShowObj.h>
#include <com\Movement\Movement.h>
#include <Com\Cartoon\CartoonManager.h>
#include <Com\Lod\Lod.h>
#include <Com\XML\XMLNode.h>
#include <Object\EditPropSheet\EditPropSheet.h>
#include <Object/ModelView.h>
#include <Object/ShowObj/TechniqueNodeCallback.h>
#include <Object/Export.h>

#import   "msxml3.dll"
namespace EMObj
{
	class OBJECT_EXPORT CObj : public osg::Switch
	{
	private:
		class CNCObj : public osg::NodeCallback 
		{
		public:
			CNCObj() : _lastUpdate(0) {}
			virtual void operator()(osg::Node* node, osg::NodeVisitor* nv) 
			{
				CObj* pObj = dynamic_cast<CObj*>(node);
				if (!pObj)
					return;
				if (nv->getVisitorType() == osg::NodeVisitor::UPDATE_VISITOR && 
					nv->getFrameStamp() && 
					nv->getFrameStamp()->getFrameNumber() != _lastUpdate) 
				{
					pObj->Update(nv, nv->getFrameStamp()->getFrameNumber(), _lastUpdate);
					_lastUpdate = nv->getFrameStamp()->getFrameNumber();
				}
				traverse(node, nv);
			}
			int _lastUpdate;
		};


	public:
		CObj(void);
		virtual ~CObj(void);

		/////////////////////////////////////初始化相关方法//////////////////////////////////////
	public:
		//数据库初始化
		bool Init(Data::CRowShowObj* pRowShowObj);
		//XML初始化
		bool Init(MSXML2::IXMLDOMNodePtr pNode);

		//读取和获取视点信息
		bool ReadEyePosAndPose(MSXML2::IXMLDOMNodePtr pNode);
		MSXML2::IXMLDOMNodePtr GetEyePosAndPose();

		//读取和获取Lod信息
		bool ReadLod(MSXML2::IXMLDOMNodePtr pNode,LONG64 id);
		MSXML2::IXMLDOMNodePtr GetLod();

		//读取和获取属性信息
		virtual bool ReadProperty(MSXML2::IXMLDOMNodePtr pNode);
		virtual MSXML2::IXMLDOMNodePtr GetProperty();

		/////////////////////////////////////操作相关方法//////////////////////////////////////
		//默认的动作，包括显隐，选中，透明，闪烁，显示编辑对话框
		enum{
			EM_OATYPE_Visible = 1,
			EM_OATYPE_Select,            //选中
			EM_OATYPE_Transparent,
			EM_OATYPE_Flicker,
			EM_OATYPE_Clear,
		};
		virtual bool DefaultAction(DWORD ActionType, DWORD Value = 0);
		//此方法暂时只用于选中状态
		virtual bool DefaultAction(DWORD ActionType, std::vector<float> Value );

		//定位
		enum{
			LocateStyle_Normal = 1,
			LocateStyle_Fly
		};
		virtual bool Locate(COM::CCartoonManager *CartoonManager = NULL, DWORD LocateStyle = LocateStyle_Fly);

		//保持状态
		virtual void SaveState();

		//恢复最后一次保存的状态
		virtual void ResumeState();

		//保存数据到数据库
		virtual bool SaveDataToDB(DWORD Value = 0);

	protected:
		//闪烁控制，可改写该行为
		virtual void FlickPulse(bool bflag);
		virtual void FlickRestore();

		/////////////////////////////////////属性相关方法//////////////////////////////////////
	public:
		//ID
		LONG64	ID;

		//类型
		virtual DWORD GetType();

		//渲染类型
		enum{
			Render_Type_UnDefault,
			Render_Type_Screen,
			Render_Type_Widget,
			Render_Type_Scene
		};
		virtual DWORD GetRenderType();

		//名称
		CString GetName();
		void SetName(CString Name);

		//数据库记录
		Data::CRowShowObj* GetRowShowObj();
		//XML节点信息
		MSXML2::IXMLDOMNodePtr GetXMLNode();

		/////////////////////////////////////刷新相关方法//////////////////////////////////////
	public:
		//刷新
		void Refresh(DWORD Para1 = 0, DWORD Para2 = 0);

		//刷新基本信息
		virtual bool UpdateBaseData();

		//刷新Lod信息
		virtual bool UpdateLod();

		//根据属性信息更新绘制
		virtual void UpdateDraw(DWORD Value = 0);

		//渲染数据向属性数据更新
		virtual bool UpdateToProData();

		//LiuGH添加，删除当前三维显示对象子节点
		virtual bool RemoveChildNode();


		//设置刷新开关
		void SetUpdateSwitch(bool Value);

		//显隐控制
		virtual void Show(BOOL bShow);

	protected:
		virtual void Update(osg::NodeVisitor* nv, double Time, double LastTime);

		osg::ref_ptr<CNCObj> m_NCObj;
		osg::ref_ptr<CTechniqueNodeCallback> m_ctechniqueNodeCallback;

		/////////////////////////////////////位置调整相关方法//////////////////////////////////////
	public:
		//设置和获取位置
		void SetPos(osg::Vec3d &pos);
		//子类可改写,传入自己的position
		virtual void GetPos(osg::Vec3d &pos);

		//设置和获取姿态
		void SetPose(osg::Vec3f &axisAt, osg::Vec3f &axisUp);
		void GetPose(osg::Vec3f &axisAt, osg::Vec3f &axisUp);

		void SetPosAndPose(osg::Vec3d &pos, osg::Vec3f &axisAt, osg::Vec3f &axisUp);

		//设置缩放比例
		void SetScale(osg::Vec3f Scale);
		osg::Vec3f GetScale();

		//返回位置节点
		osg::MatrixTransform *GetMT(){return m_MT.get();}

		//设置LOD的位置
		void SetLodPos(osg::Vec3d &pos);

	protected:
		void ApplyMatrix();
		//位置节点
		osg::ref_ptr<osg::MatrixTransform> m_MT;
		osg::Vec3d m_Pos;
		osg::Vec3f m_Scale, m_AxisAt, m_AxisUp;
		//位置节点是否有效
		bool m_IsPosAndPoseAble;

		/////////////////////////////////////编辑相关方法//////////////////////////////////////
	public:
		void	ShowEditDlg();

		//开始编辑方法在UI线程调用
		virtual bool BeginEditUI();
		//开始编辑方法在渲染线程调用
		virtual bool BeginEdit3D();

		//开始编辑方法在UI线程调用
		virtual bool EndEditUI();
		//结束编辑方法在渲染线程调用
		virtual bool EndEdit3D();
		/////////////////////////////////////观察点相关方法//////////////////////////////////////
	public:
		//模型预览
		void ShowModelViewDlg();
	public:
		void SetEyePosAndPose(osg::Vec3d EyeLLA, osg::Vec3 EyeAngle);
		void GetEyePosAndPose(osg::Vec3d &EyeLLA, osg::Vec3 &EyeAngle);

		/*
		目的：生成计算的观察点的位置和姿态(子类可改写之)
		参数：EyeLLA	(out)	经纬高
		EyeAngle	(out)	姿态
		返回：成功返回true，否则返回false
		*/
		virtual bool GenEyePosAndPose(osg::Vec3d &EyeLLA, osg::Vec3 &EyeAngle);

	protected:
		//观察点位置和姿态
		osg::Vec3d m_EyeLLA, m_EyePos;
		osg::Vec3 m_EyeAngle, m_EyeAxisAt, m_EyeAxisUp;
		//观察点是否有效
		bool m_IsEyeAble;	
		/////////////////////////////////////其他相关方法//////////////////////////////////////
	public:
		//点选
		virtual bool PtSelect(double &x, double &y, osg::Vec3d& eye, osg::Vec3d& vec);
		/************************************************************************/
		/* 碰撞检测
		startArray 检测起点集合，需要和终点一一对应
		endArray   检测终点集合，需要和起点一一对应
		collisionDectionArray  碰撞点集合
		返回 bool 有碰撞返回true 无返回false
		2013-03-04 xubaolong
		*/
		/************************************************************************/
		virtual bool CollisionDetection(std::vector<osg::Vec3d> &startArray,std::vector<osg::Vec3d> &endArray,std::vector<osg::Vec3d> &collisionDectionArray);
		//增加和删除节点
		bool addChild(Node *child);
		bool removeChild( Node *child );

		//增加子节点
		bool AddNodeFromFile(CString strFile);

		//选中点
		osg::Vec3d m_SelCrossPt;

		//加载配置对象
		osg::ref_ptr<COM::CLod> m_Lod;
		osg::ref_ptr<COM::CLod> old_LOD;
		float old_LODPrentID;

		bool m_IsLodAble;	
		bool m_bHide;

		BOOL GetObjVisible();
		void SetObjVisible(BOOL bVisible);

		bool getHilightStype(){return m_bhilight;}

		//判断对象资源文件是否存在
		virtual bool IsExistRes(){return true;}

		//删除对象资源
		virtual void DelRes(){}

		bool getFlicCount(){
			if (m_flickCount>0)
			{
				return false;
			}
			else
			{
				return true;
			}
		}


	protected:
		osg::ref_ptr<Data::CRowShowObj> m_RowShowObj;

		//状态操作
		std::vector<osg::ref_ptr<osg::StateSet>> m_stateStack;	//状态栈
		//闪烁数据
		double	m_flickLastTime;	//上次切换的时刻
		DWORD	m_flickCount;		//闪烁次数
		bool	m_bResExist;		//文件资源存在标识
		osg::Vec4 stateSetColor1;
		osg::Vec4 stateSetColor2;

		bool    m_bhilight;         //是否处于高亮状态 

		//UI编辑
	protected:
		static bool m_IsEdit;
		static EMObj::CEditPropSheet * m_pEditPropSheet;

		/////////////////////////////////////消息调用相关方法//////////////////////////////////////
	public:

		void SendMsg(DWORD EventType, DWORD Message, DWORD Para1 = 0, DWORD Para2 = 0, CString strXML = _T(""));
	};

	//辅助函数
	//获取高亮状态
	osg::StateSet *GetHightlightStateSet();
	osg::StateSet *GetHightlightStateSet(osg::Vec4d emission,osg::Vec4d diffuse);
	//获取半透状态
	osg::StateSet *GetTransparentStateSet();

}
