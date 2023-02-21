/*************************************************************************
FileName��CSOArea.h CSOArea.cpp
Author:	Liugh
Date��2009-12-10
Description��Area Create
**************************************************************************/

#pragma once
#include <osg/Geode>
#include <vector>
#include <OpenThreads/Mutex>

namespace EMObj
{
	class CSOArea :	public EMObj::CObj
	{
	public:
		CSOArea(void);
		~CSOArea(void);

		enum{
			SOType = 15
		};

		enum DrawType
		{
			FLOAT,
			COVERD
		};

		//��ȡ����뾶��������������
		bool GetAreaRadiusAndCenter(osg::Vec3d& center,float* radius = 0);

		//���û�����ɫ
		void SetAreaColor(COLORREF color);

		//����͸����
		void SetAlpha(const float& alpha);

		//�����������
		void SetMaterialToArea(float& w,float& h);

		//���û������������:	0-ƽ��	/	1-����
		void SetAreaType(UINT type);

		//��ȡ�����ά��������
		bool GetTexCoords(const float w,const float h,osg::Vec2Array* texArray);

		bool GetTexCoodsSingle(const float w,const float h,osg::Vec2Array* texArray);


		//���ݻ���������������߶�
		void ResetHeight();

		//�߳�����
		void SetHeight(const float& h);

		 void GetHeight(float& h);

		//�������ĵ���������̬
		void SetCenterPosAndPose(const osg::Vec3d& cenPos, const osg::Vec3f& cenPose);


		void GetCenterPosAndPose(osg::Vec3d& cenPos,osg::Vec3f& cenPose);


		//�õ�������
		void getVertexArray(osg::ref_ptr<osg::Vec3Array> pArray);

		//�����������
		void SetMatrixT(double Lon, double Lat, double Altitude, float Yaw, float Pitch, float Roll);

		void SetFillType(int type);

		//��ȡ��ҵID
		int GetEnterID();
	
	public:		//�麯��

		//��ȡ�ͻ�ȡ������Ϣ
		virtual bool ReadProperty(MSXML2::IXMLDOMNodePtr pNode);

		virtual MSXML2::IXMLDOMNodePtr GetProperty();

		void UpdateDraw(DWORD Value = 0);

		
		DWORD GetType()
		{
			return SOType;
		}

		DWORD GetRenderType()
		{
			return Render_Type_Scene;
		}
		//�����༭�Ի���
		bool BeginEditUI();
		bool EndEdit3D();

	protected:
		//��˸���ƣ��ɸ�д����Ϊ
		virtual void FlickPulse(bool bflag);
		virtual void FlickRestore();
		
	public:		//���ݳ�Ա
		int		m_FillType;			//��ͼ��ʽ
		bool	m_bCanceled;		//ȡ��������־
		BOOL	m_bUseMaterial;
		UINT	m_repeat;			//��ͼ�ظ���
		float	m_height;			//ƽ����ƺ��θ߶�
		float	m_AreaSlope;		//�¶�
		DrawType	_AreaType;
		osg::Vec4f	_AreaColor;
		CString		_ImagePath;
		osg::Vec3d	m_CenPos;
		osg::Vec3	m_CenPose;
		osg::ref_ptr<osg::Vec3Array> _ptArray;
		osg::ref_ptr<osg::Geode>	_AreaGeode;
		//λ�ýڵ�
		//osg::ref_ptr<osg::MatrixTransform> m_areaMT;
		OpenThreads::Mutex m_mutex;
	};
}