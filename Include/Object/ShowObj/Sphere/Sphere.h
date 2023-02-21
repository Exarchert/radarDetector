/*************************************************************************
FileName：CSphere.h CSphere.cpp
Author:	Liugh
Date：2009-12-07
Description：Sphere Edit
**************************************************************************/
#pragma once
#include <Object\Export.h>
#include <Object\Obj.h>
#include <osg/ShapeDrawable>
#include <OpenThreads\Mutex>

namespace EMObj
{
	class OBJECT_EXPORT CSphere : public EMObj::CObj
	{
	public:
		CSphere(void);
		~CSphere(void);

		enum{
			SOType = 13
		};


		DWORD GetType()
		{
			return SOType;
		}

		DWORD GetRenderType()
		{
			return Render_Type_Scene;
		}
		//虚函数
		bool ReadProperty(MSXML2::IXMLDOMNodePtr pNode);
		MSXML2::IXMLDOMNodePtr GetProperty();
		void UpdateDraw(DWORD Value = 0);
		bool BeginEditUI();
		bool EndEdit3D();
	public:
		void GetSpherePos(osg::Vec3d& pos,osg::Vec3f& pose);
		bool SetSpherePos(osg::Vec3d& pos,osg::Vec3f& pose);
		void AddMaterial();
		void AddGridLine(osg::TessellationHints& hints, osg::Vec3d& center);
		//获取企业ID
		int GetEnterID() const;
	public:
		float	m_Radius;
		int		m_SphereR;
		int		m_SphereG;
		int		m_SphereB;
		float	m_SphereAlpha;
		double	m_Lon;
		double	m_Lat;
		double	m_Alt;
		float	m_yaw;
		float	m_pitch;
		float	m_roll;
		int		m_LineR;
		int		m_LineG;
		int		m_LineB;
		float	m_LineRatio;

		bool	m_bCanceled;
		BOOL	m_bOpenGrid;
		osg::ref_ptr<osg::Geode> m_SphereGeo;
		osg::ref_ptr<osg::Geode> m_GridGeo;

		BOOL	m_bUseMaterial;
		CString	m_MatriaPath;
		//互斥量
		OpenThreads::Mutex	m_mutex;
	};
}