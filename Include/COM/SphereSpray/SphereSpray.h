/*************************************************
�ļ�:			SphereSpray
����:			л��ҫ  
��������:		2010-03-9
����:			ģ������ϵ�ˮĻЧ����
                ˮĻ�����ĵ�����Ķ���
				Բ�������Բ���붥��Բ�ĵ�������Z��ƽ��
*************************************************/
#pragma once
#include <Com\Export.h>
#include <osgAPEx/Callback>
#include <osg/MatrixTransform>
#include <osg/StateSet>
#include <osg/Geometry>
#include <osg/Geode>
#include <osg/TexMat>
#include <osg/TextureCubeMap>
#include <osg/CullFace>

namespace COM	{
class COM_EXPORT SphereSpray: public osg::Group
{
public:
	SphereSpray(void);
	~SphereSpray(void);

	/*
	��ʼ��
	sprayImage			(in)	ˮĻͼƬ��·��
	repeatRation        (in)    ��ͼ�ظ��ʣ�Ĭ��Ϊ1����СֵΪ1
	speed               (in)    ��ˮ���ٶ�
	radius              (in)    �뾶
	cylinderHeight      (in)    ˮĻԲ���ĸ߶�
    sphereHeight        (in)    ˮĻ������ĸ߶�
	return:	�ɹ�����true
	*/
	bool init(const std::string& sprayImage,float repeatRation,float speed,float radius,float cylinderHeight,float sphereHeight);

private:

	//����ˮĻ
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
	private:
		osg::TexMat* m_texmat;
		float m_phaseT;
		float m_speed;
	};

	osg::Geometry* createSphere();

	unsigned int createIndex(unsigned int r, unsigned int c, unsigned int row_len){ return c + r * row_len; }//��ս�ģ��Ҫ������

private:
	std::string m_imagePath;

	float m_radius;
    float m_cylinderHeight;
    float m_sphereHeight;
	float m_repeatRatio;
	float m_speed;
	std::string m_sprayImgPath;
	
	osg::ref_ptr<osg::MatrixTransform>      m_transform;
	osg::ref_ptr<osg::TexMat>               m_sprayTexmat;//����ˮĻ�����������������
};

}