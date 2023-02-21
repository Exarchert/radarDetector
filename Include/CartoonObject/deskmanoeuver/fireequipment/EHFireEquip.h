#pragma once
#include <osgGA\GUIEventHandler>
#include <Com\DataBall\DataBall.h>
namespace CartoonObject
{
	class CEHFire : public osgGA::GUIEventHandler
	{
	public:
		struct pos_at_up
		{
			osg::Matrixd localToWorld;
		};
	public:
		CEHFire (void);
		~CEHFire (void);

		virtual bool handle(const osgGA::GUIEventAdapter& ea,osgGA::GUIActionAdapter& aa);
	
		/************************************************************************/
		/*   参数说明:															*/
		/*pos:	当前鼠标在视口中的屏幕坐标										*/
		/*type:                                                                 */
		/************************************************************************/
		
		virtual bool Start(osg::Vec2 pos,UINT type = 0);
		bool getTerrainIntersection(osgViewer::Viewer *viewer, osg::Node *subGraph, float x, float y, osg::Vec3d &pos,bool hasParent);
		void Init();
		void SetHintBall(LPCTSTR szHint);
		osg::Node* GetHintBall() const;
		void UpdateHintBall(const osg::Matrixd &mat);
		void AddHintBall();
		void RemoveHintBall();
		void ShowHintBall(bool bShow);

	public:

		void SetHwnd(HWND hWnd){m_hCalledWnd = hWnd;}

		void SetActive(BOOL bActive);

	public:
		COM::CDataBall	m_dataBall;	
		HWND	m_hCalledWnd;
		BOOL	m_bActive;
		
	protected:
		osg::Matrixd						_localToWorld;
		osg::ref_ptr<osg::MatrixTransform>	m_trans;
		osg::ref_ptr<osg::Switch>			m_switch;
		bool								m_bWork;	
		bool								_beEditing;
		bool								_initBall;
	};
}
