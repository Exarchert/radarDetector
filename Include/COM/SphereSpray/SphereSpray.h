/*************************************************
文件:			SphereSpray
作者:			谢荣耀  
创建日期:		2010-03-9
描述:			模拟球罐上的水幕效果。
                水幕的中心点在球的顶端
				圆柱体底面圆心与顶面圆心的连线与Z轴平行
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
	初始化
	sprayImage			(in)	水幕图片的路径
	repeatRation        (in)    贴图重复率，默认为1，最小值为1
	speed               (in)    喷水的速度
	radius              (in)    半径
	cylinderHeight      (in)    水幕圆柱的高度
    sphereHeight        (in)    水幕顶部球的高度
	return:	成功返回true
	*/
	bool init(const std::string& sprayImage,float repeatRation,float speed,float radius,float cylinderHeight,float sphereHeight);

private:

	//更新水幕
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

	unsigned int createIndex(unsigned int r, unsigned int c, unsigned int row_len){ return c + r * row_len; }//天空建模需要的索引

private:
	std::string m_imagePath;

	float m_radius;
    float m_cylinderHeight;
    float m_sphereHeight;
	float m_repeatRatio;
	float m_speed;
	std::string m_sprayImgPath;
	
	osg::ref_ptr<osg::MatrixTransform>      m_transform;
	osg::ref_ptr<osg::TexMat>               m_sprayTexmat;//控制水幕流动的纹理坐标矩阵
};

}