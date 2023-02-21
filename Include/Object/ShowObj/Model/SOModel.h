/***************************************************************
file:			SOModel.h
Author:			张平    
Date:			2009-11-23
Description:    模型类
***************************************************************/
/**************************************************************
History:
Alter Author: liugh
Description:  Add The Mode Of Asynchronism for Model Load
Date:		  2010.3.25
***************************************************************/
//
//#pragma once
//#include <Object/Export.h>
//#include <Object/Obj.h>
//#include <osgGA/GUIEventHandler>
//#include <Object\ObjManager.h>
//#include <OpenThreads\Mutex>
//
//
//#pragma once
//
//using namespace EMObj;
//namespace EMObj{
//	//实现基类虚函数，设置并返回模型加载的优先级
//	class LoadUnloadControler : public osg::UserProxyNode::LoadUnloadControler
//	{
//	public:
//		LoadUnloadControler(int condition,float rScale,float dist,float radius,osg::Vec3d pos,CObj* pObj)
//		{
//			_loadCondi = condition;
//			_rScale = rScale;
//			_dist = dist;
//			_radius = radius;
//			_pos = pos;
//			_pObj = pObj;
//			if(_pObj != NULL)
//			{
//				_pObj->SetUpdateSwitch(true);
//			}
//		}		
//	private:
//		virtual bool isChildNeedLoad(osg::NodeVisitor& nv, unsigned int i, float& priorty);
//	private:
//		int			_loadCondi;
//		float		_rScale;
//		float		_dist;
//		float		_radius;
//		osg::Vec3d	_pos;
//		CObj*		_pObj;
//		OpenThreads::Mutex m_mutex;
//	};
//
//	class OBJECT_EXPORT CSOModel : public EMObj::CObj
//	{
//	public:
//		CSOModel(void);
//		~CSOModel(void);
//
//		enum{
//			SOType = 0x00000001
//		};
//
//		enum LoadMode
//		{
//			Synchronize,	//一次全部加载
//			Asynchronism,	//异步加载		
//		};
//
//		DWORD GetType()
//		{
//			return SOType;
//		}
//
//		DWORD GetRenderType()
//		{
//			return Render_Type_Scene;
//		}
//		//虚函数
//		bool ReadProperty(MSXML2::IXMLDOMNodePtr pNode);
//		MSXML2::IXMLDOMNodePtr GetProperty();
//		void UpdateDraw(DWORD Value = 0);
//		bool BeginEditUI();
//		bool EndEdit3D();
//
//		bool UpdateToProData();
//
//	public:
//		//获得属性数据
//		void GetProDataModelFile(CString &ModelFile);
//		//相对于企业的坐标
//		void GetProDataXYZ(double &x, double &y, double &z);
//		//相对于企业的模型姿态
//		void GetProDataPoseAngle(float &yaw, float &pitch, float &roll);
//		//缩放
//		void GetProDataScale(float &xScale, float &yScale, float &zScale);
//
//		void SetProDataModelFile(CString &ModelFile);
//		void SetProDataXYZ(double &x, double &y, double &z);
//		void SetProDataPoseAngle(float &yaw, float &pitch, float &roll);
//		void SetProDataScale(float &xScale, float &yScale, float &zScale);
//
//
//		//模型加载
//		void setLoadMode(int mode);
//		void GetLoadMode(int& mode);
//		void setModelName(CString aveName);
//		void GetModelName(CString& aveName);
//		void setLoadCondition(int condition,float dist,float rScale);
//		void GetLoadCondition(int& condition,float& dist,float& rScale);
//		void GetLoadLife(double& loadLife);
//		void SetLoadLife(double loadLife);
//
//		void GetModelRadius(float& radius);
//		void SetModelRadius(float radius);
//
//		//判断对象资源文件是否存在
//		virtual bool IsExistRes(){return m_bResExist;}
//
//		//获取模型中心坐标
//		void GetCenter(osg::Vec3d& pos);
//		//模型文件路径
//		std::string getPModelFile();
//		
//	protected:
//
//		//属性数据
//		//文件名
//		CString		m_PModelFile;
//		std::string m_PModelFilePath;
//		//位置
//		osg::Vec3d	m_PXYZ;
//		//姿态角
//		osg::Vec3	m_PPoseAngle;
//		//缩放
//		osg::Vec3	m_PScale;
// 		//模型加载方式:同步/异步
//		LoadMode	m_LoadMode;
//		//数据读取信息
//		CString _aveID;
//		float	_aveR;
//		float	_dist;
//		int		_loadCondi;	
//		float	_rScale;
//		double	_LoadLife;
//		//CDlgEditModel *pDlgEditModel ;
//		OpenThreads::Mutex	g_Mutex;
//	};
//}

#if 1

/***************************************************************
file:			SOModel.h
Author:			张平    
Date:			2009-11-23
Description:    模型类
***************************************************************/
/**************************************************************
History:
Alter Author: liugh
Description:  Add The Mode Of Asynchronism for Model Load
Date:		  2010.3.25
***************************************************************/

#pragma once
#include <Object/Export.h>
#include <Object/Obj.h>
#include <osgGA/GUIEventHandler>
#include <Object\ObjManager.h>
#include <OpenThreads\Mutex>

#pragma once

using namespace EMObj;
namespace EMObj{
	//实现基类虚函数，设置并返回模型加载的优先级
	class LoadUnloadControler : public osg::UserProxyNode::LoadUnloadControler
	{
	public:
		LoadUnloadControler(int condition,float rScale,float dist,float radius,osg::Vec3d pos,CObj* pObj)
		{
			_loadCondi = condition;
			_rScale = rScale;
			_dist = dist;
			_radius = radius;
			_pos = pos;
			_pObj = pObj;
			if(_pObj != NULL)
			{
				_pObj->SetUpdateSwitch(true);
			}
		}		
	private:
		virtual bool isChildNeedLoad(osg::NodeVisitor& nv, unsigned int i, float& priorty);
	private:
		int			_loadCondi;
		float		_rScale;
		float		_dist;
		float		_radius;
		osg::Vec3d	_pos;
		CObj*		_pObj;
		OpenThreads::Mutex m_mutex;
	};

	class OBJECT_EXPORT CSOModel : public EMObj::CObj
	{
	public:
		CSOModel(void);
		~CSOModel(void);

		enum{
			SOType = 0x00000001
		};

		enum LoadMode
		{
			Synchronize,	//一次全部加载
			Asynchronism,	//异步加载		
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
		void UpdateDraw(DWORD Value = 0);//typeID
		void WriteModelFile();
		bool BeginEditUI();
		bool EndEdit3D();

		bool UpdateToProData();

	public:
		//获得属性数据
		void GetProDataModelFile(CString &ModelFile);
		void GetProDataXYZ(double &x, double &y, double &z);
		void GetProDataPoseAngle(float &yaw, float &pitch, float &roll);
		void GetProDataScale(float &xScale, float &yScale, float &zScale);

		void SetProDataModelFile(CString &ModelFile);
		void SetProDataXYZ(double &x, double &y, double &z);
		void SetProDataPoseAngle(float &yaw, float &pitch, float &roll);
		void SetProDataScale(float &xScale, float &yScale, float &zScale);


		//模型加载
		void setLoadMode(int mode);
		void GetLoadMode(int& mode);
		void setModelName(CString aveName);
		void GetModelName(CString& aveName);
		void setLoadCondition(int condition,float dist,float rScale);
		void GetLoadCondition(int& condition,float& dist,float& rScale);
		void GetLoadLife(double& loadLife);
		void SetLoadLife(double loadLife);

		void GetModelRadius(float& radius);
		void SetModelRadius(float radius);

		//判断对象资源文件是否存在
		virtual bool IsExistRes(){return m_bResExist;}

		//获取模型中心坐标
		void GetCenter(osg::Vec3d& pos);
		//模型文件路径
		std::string getPModelFile();

	protected:

		//属性数据
		//文件名
		CString		m_PModelFile;
		std::string m_PModelFilePath;
		//位置
		osg::Vec3d	m_PXYZ;
		//相对位置
		osg::Vec3d  m_OffXYZ;
		//朝向
		osg::Vec3d	m_FaceAt;
		//姿态角
		osg::Vec3	m_PPoseAngle;
		//缩放
		osg::Vec3	m_PScale;
		//模型加载方式:同步/异步
		LoadMode	m_LoadMode;
		//数据读取信息
		CString _aveID;
		float	_aveR;
		float	_dist;
		int		_loadCondi;	
		float	_rScale;
		double	_LoadLife;
	};
}
#endif