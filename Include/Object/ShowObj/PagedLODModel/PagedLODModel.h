/***************************************************************
file:			PagedLODModel.h
Author:			xubaolong
Date:			2013-12-20
Description:    分页节点模型类
***************************************************************/

#pragma once
#include <Object/Export.h>
#include <Object/Obj.h>
#include <osgGA/GUIEventHandler>
#include <Object\ObjManager.h>
#include <OpenThreads\Mutex>

namespace EMObj{

	class OBJECT_EXPORT PagedLODModel :public EMObj::CObj
	{
	public:
		PagedLODModel(void);
		~PagedLODModel(void);

		enum{
			SOType = 50
		};

		DWORD GetType()
		{
			return SOType;
		}

		//虚函数
		bool ReadProperty(MSXML2::IXMLDOMNodePtr pNode);
		MSXML2::IXMLDOMNodePtr GetProperty();
		void UpdateDraw(DWORD Value = 0);
		bool BeginEditUI();
		bool EndEdit3D();

		bool UpdateToProData();

	public:
		//获得属性数据
		void GetProDataModelFile(CString &ModelFile);
		//相对于企业的坐标
		void GetProDataXYZ(double &x, double &y, double &z);
		//相对于企业的模型姿态
		void GetProDataPoseAngle(float &yaw, float &pitch, float &roll);
		//缩放
		void GetProDataScale(float &xScale, float &yScale, float &zScale);

		void SetProDataModelFile(CString &ModelFile);
		void SetProDataXYZ(double &x, double &y, double &z);
		void SetProDataPoseAngle(float &yaw, float &pitch, float &roll);
		void SetProDataScale(float &xScale, float &yScale, float &zScale);


		void setModelName(CString aveName);
		void GetModelName(CString& aveName);
		void setLoadCondition(int condition,float dist,float rScale);
		void GetLoadCondition(int& condition,float& dist,float& rScale);


		void GetModelRadius(float& radius);
		void SetModelRadius(float radius);

		//判断对象资源文件是否存在
		virtual bool IsExistRes(){return m_bResExist;}

		//获取模型中心坐标
		void GetCenter(osg::Vec3d& pos);
		//模型文件路径
		std::string getPModelFile();

	private:
		osg::PagedLOD* createPagedLod(std::string filename);

	protected:

		//属性数据
		//文件名
		CString		m_PModelFile;
		std::string m_PModelFilePath;
		//位置
		osg::Vec3d	m_PXYZ;
		//姿态角
		osg::Vec3	m_PPoseAngle;
		//缩放
		osg::Vec3	m_PScale;

		//数据读取信息
		CString _aveID;
		float	_aveR;
		float	_dist;
		int		_loadCondi;	
		float	_rScale;



		OpenThreads::Mutex	g_Mutex;
	};
}
