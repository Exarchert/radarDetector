/***************************************************************************
Copyright (C),  
文件：SOEffect.h
作者：谢荣耀
版本：0.1
日期：2010-03-24
描述：效果类，包括粒子和shader的一些效果

修改记录：
作者						日期					描述
***************************************************************************/
#pragma once
#include <Object/Export.h>
#include <Object/Obj.h>
#include <osgDB\ReadFile>
#include <osgAPEx/BaseGeo>
#include <osgParticle/ParticleSystem>
#include <osgParticle/Particle>
#include <osgParticle/FluidProgram>
#include <osgParticle/FluidFrictionOperator>
#include <osgParticle/ModularEmitter>
#include <osgParticle/ParticleSystemUpdater>
#include <osgParticle/SectorPlacer>
#include <osgParticle/ModularProgram>
#include <osgParticle/AccelOperator>
#include <osg/TexEnvCombine>
#include <osg/Texture2D>
#include <osg/BlendFunc>
#include <osg/Depth>
#include <osgAPEx/CoordinateTool>
#include <osgAPEx/RadialShooter>
#include <osgAPEx/ParticleEffects>

namespace EMObj{

class OBJECT_EXPORT CSOEffect : public EMObj::CObj
{
public:
	CSOEffect(void);
	~CSOEffect(void);

	enum
	{
		SOType = 19
	};

	DWORD GetType()
	{
		return SOType;
	}

	DWORD GetRenderType()
	{
		return Render_Type_Scene;
	}

	//////////////////////////////////////////////////////////////////////////
	//改写CObj行为
	//获取属性信息
	bool ReadProperty(MSXML2::IXMLDOMNodePtr pNode);
	MSXML2::IXMLDOMNodePtr GetProperty();

	//刷新绘制
	virtual void UpdateDraw(DWORD Value = 0);

	//弹出编辑对话框
	bool BeginEditUI();

	//结束编辑
	bool EndEdit3D();

	//改写拾取
	virtual bool PtSelect(double &x, double &y, osg::Vec3d& eye, osg::Vec3d& vec);

	/*
	目的：解析XML节点中的粒子效果数据并保存至输出参数中
	参数：pNode		    (in)	XML节点
	      lifeTime		(out)	生命周期
	      size		    (out)	粒子大小
	      rate		    (out)	粒子数量
	      dir		    (out)	方向
	      speed		    (out)	速度
		  theta		    (out)	垂直范围
	返回：成功返回true，否则返回false
	*/
	bool ParseParticleEffectXml(MSXML2::IXMLDOMNodePtr pNode,double& lifeTime,osg::Vec2& size,osg::Vec2& rate,
		                        osg::Vec3& dir,osg::Vec2& speed,osg::Vec2& theta);

	/*
	目的：解析XML节点中的shader火焰效果数据并保存至输出参数中
	参数：pNode		                (in)	XML节点
	      particleSystemHeight		(out)	粒子系统高度
	      particleSpeed     		(out)	燃烧速度
	      particleSpread     		(out)	火焰宽度
	      particleSystemShape     	(out)	火焰片宽度
		  particleShape         	(out)	粒子形状
		  particleSize          	(out)	粒子大小
	返回：成功返回true，否则返回false
	*/
	bool ParseShaderFireXml(MSXML2::IXMLDOMNodePtr pNode,float& particleSystemHeight, float& particleSpeed,
		                    float& particleSpread,float& particleSystemShape,float& particleShape,float& particleSize);

	//重置所有的数据为默认
	void SetInitValue();

private:
	//创建粒子效果节点
	osg::Node* CreateParticleNode();
	//创建粒子火焰效果
	osg::Node* CreateShaderFireNode();

	//提取节点中的数据
	bool ParseXml(MSXML2::IXMLDOMNodePtr pNode);
	//提取节点中的模板数据
	bool ParseTemplateParaXml(MSXML2::IXMLDOMNodePtr pNode);

public:
	bool       m_useTemplate;
	int        m_templateID;

	osg::Vec3d m_centerLLA;
    osg::Vec3f m_centerYPR;
    osg::Vec3d m_desLLA;

    CString m_effectType;
	CString m_imagePath;
	CString m_modelPath;

	double m_particleLifeTime;
	osg::Vec2 m_particleSizeRange;
	osg::Vec2 m_rateCounterRange;

	osg::Vec3 m_shooterDir;
	osg::Vec2 m_shooterSpeedRange;
	osg::Vec2 m_shooterThetaRange;

public:
    float    m_particleSystemHeight;
    float    m_particleSpeed;
	float    m_particleSpread;
	float    m_particleSystemShape;
	float    m_particleShape;
	float    m_particleSize;

private:
	osg::ref_ptr<osg::Geode>                m_axisNode;
	osg::ref_ptr<osg::MatrixTransform>      m_axisTransform;
	osg::ref_ptr<osg::Group>                m_effectRoot;
	osg::Node*                              m_effectNode;
};

}
