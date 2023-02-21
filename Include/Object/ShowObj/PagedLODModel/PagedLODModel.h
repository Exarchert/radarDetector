/***************************************************************
file:			PagedLODModel.h
Author:			xubaolong
Date:			2013-12-20
Description:    ��ҳ�ڵ�ģ����
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

		//�麯��
		bool ReadProperty(MSXML2::IXMLDOMNodePtr pNode);
		MSXML2::IXMLDOMNodePtr GetProperty();
		void UpdateDraw(DWORD Value = 0);
		bool BeginEditUI();
		bool EndEdit3D();

		bool UpdateToProData();

	public:
		//�����������
		void GetProDataModelFile(CString &ModelFile);
		//�������ҵ������
		void GetProDataXYZ(double &x, double &y, double &z);
		//�������ҵ��ģ����̬
		void GetProDataPoseAngle(float &yaw, float &pitch, float &roll);
		//����
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

		//�ж϶�����Դ�ļ��Ƿ����
		virtual bool IsExistRes(){return m_bResExist;}

		//��ȡģ����������
		void GetCenter(osg::Vec3d& pos);
		//ģ���ļ�·��
		std::string getPModelFile();

	private:
		osg::PagedLOD* createPagedLod(std::string filename);

	protected:

		//��������
		//�ļ���
		CString		m_PModelFile;
		std::string m_PModelFilePath;
		//λ��
		osg::Vec3d	m_PXYZ;
		//��̬��
		osg::Vec3	m_PPoseAngle;
		//����
		osg::Vec3	m_PScale;

		//���ݶ�ȡ��Ϣ
		CString _aveID;
		float	_aveR;
		float	_dist;
		int		_loadCondi;	
		float	_rScale;



		OpenThreads::Mutex	g_Mutex;
	};
}
