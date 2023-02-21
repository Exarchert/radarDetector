/***************************************************************************
Copyright (C),  
文件：SOShpereSpray.h
作者：谢荣耀
版本：1.0
日期：2010-04-08
描述：水幕
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

	//虚函数
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
	osg::ref_ptr<osg::TexMat>            m_sprayTexmat;//控制水幕流动的纹理坐标矩阵

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
	unsigned int createIndex(unsigned int r, unsigned int c, unsigned int row_len){ return c + r * row_len; }//天空建模需要的索引
	bool ParseXml(MSXML2::IXMLDOMNodePtr pNode);


};

}
