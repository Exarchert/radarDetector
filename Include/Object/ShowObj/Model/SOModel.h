/***************************************************************
file:			SOModel.h
Author:			��ƽ    
Date:			2009-11-23
Description:    ģ����
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
//	//ʵ�ֻ����麯�������ò�����ģ�ͼ��ص����ȼ�
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
//			Synchronize,	//һ��ȫ������
//			Asynchronism,	//�첽����		
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
//		//�麯��
//		bool ReadProperty(MSXML2::IXMLDOMNodePtr pNode);
//		MSXML2::IXMLDOMNodePtr GetProperty();
//		void UpdateDraw(DWORD Value = 0);
//		bool BeginEditUI();
//		bool EndEdit3D();
//
//		bool UpdateToProData();
//
//	public:
//		//�����������
//		void GetProDataModelFile(CString &ModelFile);
//		//�������ҵ������
//		void GetProDataXYZ(double &x, double &y, double &z);
//		//�������ҵ��ģ����̬
//		void GetProDataPoseAngle(float &yaw, float &pitch, float &roll);
//		//����
//		void GetProDataScale(float &xScale, float &yScale, float &zScale);
//
//		void SetProDataModelFile(CString &ModelFile);
//		void SetProDataXYZ(double &x, double &y, double &z);
//		void SetProDataPoseAngle(float &yaw, float &pitch, float &roll);
//		void SetProDataScale(float &xScale, float &yScale, float &zScale);
//
//
//		//ģ�ͼ���
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
//		//�ж϶�����Դ�ļ��Ƿ����
//		virtual bool IsExistRes(){return m_bResExist;}
//
//		//��ȡģ����������
//		void GetCenter(osg::Vec3d& pos);
//		//ģ���ļ�·��
//		std::string getPModelFile();
//		
//	protected:
//
//		//��������
//		//�ļ���
//		CString		m_PModelFile;
//		std::string m_PModelFilePath;
//		//λ��
//		osg::Vec3d	m_PXYZ;
//		//��̬��
//		osg::Vec3	m_PPoseAngle;
//		//����
//		osg::Vec3	m_PScale;
// 		//ģ�ͼ��ط�ʽ:ͬ��/�첽
//		LoadMode	m_LoadMode;
//		//���ݶ�ȡ��Ϣ
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
Author:			��ƽ    
Date:			2009-11-23
Description:    ģ����
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
	//ʵ�ֻ����麯�������ò�����ģ�ͼ��ص����ȼ�
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
			Synchronize,	//һ��ȫ������
			Asynchronism,	//�첽����		
		};

		DWORD GetType()
		{
			return SOType;
		}

		DWORD GetRenderType()
		{
			return Render_Type_Scene;
		}
		//�麯��
		bool ReadProperty(MSXML2::IXMLDOMNodePtr pNode);
		MSXML2::IXMLDOMNodePtr GetProperty();
		void UpdateDraw(DWORD Value = 0);//typeID
		void WriteModelFile();
		bool BeginEditUI();
		bool EndEdit3D();

		bool UpdateToProData();

	public:
		//�����������
		void GetProDataModelFile(CString &ModelFile);
		void GetProDataXYZ(double &x, double &y, double &z);
		void GetProDataPoseAngle(float &yaw, float &pitch, float &roll);
		void GetProDataScale(float &xScale, float &yScale, float &zScale);

		void SetProDataModelFile(CString &ModelFile);
		void SetProDataXYZ(double &x, double &y, double &z);
		void SetProDataPoseAngle(float &yaw, float &pitch, float &roll);
		void SetProDataScale(float &xScale, float &yScale, float &zScale);


		//ģ�ͼ���
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

		//�ж϶�����Դ�ļ��Ƿ����
		virtual bool IsExistRes(){return m_bResExist;}

		//��ȡģ����������
		void GetCenter(osg::Vec3d& pos);
		//ģ���ļ�·��
		std::string getPModelFile();

	protected:

		//��������
		//�ļ���
		CString		m_PModelFile;
		std::string m_PModelFilePath;
		//λ��
		osg::Vec3d	m_PXYZ;
		//���λ��
		osg::Vec3d  m_OffXYZ;
		//����
		osg::Vec3d	m_FaceAt;
		//��̬��
		osg::Vec3	m_PPoseAngle;
		//����
		osg::Vec3	m_PScale;
		//ģ�ͼ��ط�ʽ:ͬ��/�첽
		LoadMode	m_LoadMode;
		//���ݶ�ȡ��Ϣ
		CString _aveID;
		float	_aveR;
		float	_dist;
		int		_loadCondi;	
		float	_rScale;
		double	_LoadLife;
	};
}
#endif