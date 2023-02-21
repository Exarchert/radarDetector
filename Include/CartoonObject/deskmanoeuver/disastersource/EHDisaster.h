#pragma once
#include <osgGA\GUIEventHandler>
#include <Com\DataBall\DataBall.h>
#include <Com\DataBall\DataBall2.h>
#include "Disaster.h"

namespace CartoonObject
{
	class CEHDisaster : public osgGA::GUIEventHandler
	{
	public:
		struct pos_at_up
		{
			osg::Matrixd localToWorld;
		};
		CEHDisaster (void);
		~CEHDisaster (void);

		virtual bool handle(const osgGA::GUIEventAdapter& ea,osgGA::GUIActionAdapter& aa);
		
		/**************************************/
		//参数说明:
		//pos:	当前鼠标在视口中的屏幕坐标
		//type:
		//		Explosion = 0,		//爆炸
		//		Pool_Fire,			//池火
		//		Spray_Fire,			//喷射火
		//		Leak_Air			//气体泄露
		//////////////////////////////////////*/
		virtual bool Start(osg::Vec2 pos,UINT type = 0);

		void Init();
		void SetHintBall(LPCTSTR szHint);
		osg::Node* GetHintBall() const;
		void UpdateHintBall(const osg::Matrixd &mat);
		void AddHintBall();
		void RemoveHintBall();
		void ShowHintBall(bool bShow);
		bool pickLand(osgViewer::View* view, const osgGA::GUIEventAdapter& ea, osg::Vec3d& pos);
		bool getTerrainIntersection(osgViewer::Viewer *viewer, osg::Node *subGraph, float x, float y, osg::Vec3d &pos,bool hasParent);
	public:
		COM::CDataBall2	m_dataBall;		
	protected:
		osg::ref_ptr<osg::MatrixTransform>	m_trans;
		osg::ref_ptr<osg::Switch>			m_switch;
		bool								m_bWork;
		osg::Vec3d							_pos;
		osg::Vec3f							_at;
		osg::Vec3f							_up;
		osg::Matrixd						_localToWorld;
		bool								_beEditing;
		bool								_initBall;
		//osg::Vec3d							_pose;
	public:	
		void SetHwnd(HWND hWnd){m_hCalledWnd = hWnd;}
		void SetActive(BOOL bActive);
		
		void UpdatePoolFire(float widht,float hight,float size);
		void UpdateSprayFire(float intensity);
		void UpdateExplosion(float intensity);

		bool FinishDisaster(CString strFilePath,DWORD type);	
		void RemoveDisaster(DWORD type);

		osg::Matrixd GetDisasterMat();
		void GetDisasterPosAndPose(osg::Vec3d& pos,osg::Vec3& pose);
	public:
		HWND	m_hCalledWnd;
		BOOL	m_bActive;
		UINT	m_type;

		osg::ref_ptr<CDisaster> m_Disaster;
	};
}
