/***************************************************************************
Copyright (C),  
�ļ���SOShpereSpray.h
���ߣ�л��ҫ
�汾��1.0
���ڣ�2010-04-08
������ˮĻ
***************************************************************************/
#pragma once
#include <Object/Export.h>
#include <Object/Obj.h>
#include <osg/texmat>

namespace EMObj{

class OBJECT_EXPORT CSOSphereSpray : public EMObj::CObj
{
public:
	CSOSphereSpray(void);
	~CSOSphereSpray(void);

	enum
	{
		SOType = 20
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

	void UpdateDraw(DWORD Value = 0);
	bool BeginEditUI();
	bool EndEdit3D();

   
public:
	float m_radius;
	float m_repeatRatio;
    float m_cylinderHeight;
    float m_sphereHeight;

	float m_speed;
	CString m_sprayImgPath;
	osg::Vec3d m_centerLLA;
	osg::Vec3 m_centerYPR;

	osg::Geode* m_geode;
	osg::ref_ptr<osg::MatrixTransform>   m_transform;
	osg::ref_ptr<osg::TexMat>            m_sprayTexmat;//����ˮĻ�����������������

private:
	class UpdateSphereSpray: public osg::NodeCallback 
	{
		public:
			UpdateSphereSpray(osg::TexMat* texmat,float speed):
			  m_texmat(texmat),
			  m_phaseT(0.0f),
			  m_speed(speed)
			{}

			virtual void operator()(osg::Node* node, osg::NodeVisitor* nv) 
			{
				if (!m_texmat)
					return;

				m_phaseT = m_phaseT - m_speed;

				m_texmat->setMatrix(osg::Matrix::translate(1.0 , m_phaseT,1.0));

			}
			void Set(osg::TexMat* pTexMat,float speed)
			{
				m_texmat = pTexMat;
				m_speed  = speed;
			}
	private:
		osg::TexMat* m_texmat;
		float m_phaseT;
		float m_speed;
	};

	osg::ref_ptr<UpdateSphereSpray> m_updateCallback;

	osg::Geometry* createSphere();
	unsigned int createIndex(unsigned int r, unsigned int c, unsigned int row_len){ return c + r * row_len; }//��ս�ģ��Ҫ������
	bool ParseXml(MSXML2::IXMLDOMNodePtr pNode);


};

}
