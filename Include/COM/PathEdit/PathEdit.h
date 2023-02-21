/**************************************************************************
File:			PathEdit.h
Author:			张平    
Date:			2011-1-17
Description:    路径编辑模块
***************************************************************************/

#pragma once
#include <COM\Export.h>
#include <osgGA\GUIEventHandler>
#include <vector>
#include <osg/Group>
#include <osgDB/ReadFile>
#include <osg/Image>
#include <osgViewer/Viewer>
#include <Com/DataBall/DataBall.h>
#include <Com/KernelContext/KernelContext.h>

namespace COM{

	class COM_EXPORT CPathEdit : public osgGA::GUIEventHandler
	{
	public:
		CPathEdit(void);
		//关键点可以摆放一个模型，strModel为绝对路径
		CPathEdit(CKernelContext* pContext,CString strModel = _T(""));
		~CPathEdit(void);
		bool handle(const osgGA::GUIEventAdapter& ea,osgGA::GUIActionAdapter& aa);

		/*
		初始化
		StartIco	(in)		起点标志图标，空间绑定点默认为图标的底边中点
		EndIco		(in)		终点标志图标，空间绑定点默认为图标的底边中点
		FlagIve		(in)		节点标志模型
		LineColor	(in)		线条颜色
		LineWidth	(in)		线条宽度
		Viewer		(in)		
		EarthGroup	(in)		求交节点
		SceneGroup	(in)		世界坐标系下的场景节点，用来显示节点旗子模型
		ScreenGroup	(in)		屏幕节点
		*/
		bool Init(CString StartIco, CString EndIco, CString FlagIve, osg::Vec4 LineColor, int LineWidth, 
			osgViewer::Viewer* Viewer, osg::Group* EarthGroup, osg::Group* SceneGroup, osg::Group* ScreenGroup);

		/*
		设置路径起始点
		pos		(in)		起点世界坐标
		*/
		void SetStartPos(osg::Vec3d pos);

		/*
		获取路径值
		Path		(out)		获得路径点数组
		ValueType	(in)		点值类型，1表示获得坐标为世界坐标，2表示获得坐标为经纬度
		*/
		bool GetPath(osg::Vec3dArray& Path, int ValueType = 1);

		bool GetPath(osg::Vec3dArray& Path, osg::Vec3dArray& poseVecs, int ValueType = 1);

		void RemovePath();

		float GetPathDist();
	protected:
		
		osg::Vec4	m_LineColor;
		int			m_LineWidth;
		osgViewer::Viewer*			m_Viewer;
		osg::ref_ptr<osg::Group>	m_EarthGroup;
		osg::ref_ptr<osg::Group>	m_SceneGroup;
		osg::ref_ptr<osg::Group>	m_ScreenGroup;

		osg::ref_ptr<osg::Node>		m_StartNode;		//图片结点
		osg::ref_ptr<osg::Image>	m_StartImg;
		osg::ref_ptr<osg::Node>		m_EndNode;			//图片结点
		osg::ref_ptr<osg::Image>	m_EndImg;
		osg::ref_ptr<osg::Node>		m_FlagNode;

		osg::ref_ptr<osg::Geode>	m_PathGeode;
		osg::ref_ptr<osg::MatrixTransform>	m_PathTrans;

		//路径节点总数
		std::vector<osg::Vec3d> m_PathPoints;
		std::vector<osg::Vec3> m_PathPoseS;
		int						m_PathPointsCount;
		float					m_PathDist;

	public:
		//设置提示球(或其它模型)，表示当前的位置和姿态	
		void SetHintBall(LPCTSTR szHint);

		//获取提示球的结点
		osg::Node * GetHintBall() const;

		//更新提示球位置和姿态
		void UpdateHintBall(const osg::Matrixd &mat);


		//移除提示球
		void RemoveHintBall();


		//添加提示球
		void AddHintBall();

		void SetPathEditActive(bool bActive){m_bActive = bActive;}
		bool GetPathEditActive(){return m_bActive;}

		void GetCurPosAndPose(osg::Vec3d& pos,osg::Vec3& pose);

		float GetDataBallRadius(){return m_dataBallRadius;}
	public:
		//提示球(或其它模型)
		COM::CDataBall	m_dataBall;		//根据用户输入计算位置
	private:
		//关键点处摆放模型
		float	m_dataBallRadius;
		CString	m_ModelName;
		osg::ref_ptr<CKernelContext>	m_pContext;
		osg::ref_ptr<osg::MatrixTransform>	m_DBTrans;	//变换矩阵
		osg::ref_ptr<osg::MatrixTransform>	m_MTrans;	//变换矩阵

		osg::Vec3d	m_curPos;
		osg::Vec3	m_curPose;
		bool		m_bDataBallShowed;
		bool		m_bActive;
	};

}