/****************************************************************************
Copyright (C)  
文件：ParticleFactory.h ParticleFactory.cpp
版本：v1.0
作者：黄志鹏
日期：2009-12-02
描述：粒子产生工厂.

修改记录
作者					日期					描述
-----------------------------------------------------------------------------

****************************************************************************/

#pragma once

#include <Com\Export.h>
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

#include <osgAPEx/CoordinateTool>
#include <osgAPEx/RadialShooter>
#include <osgAPEx/ParticleEffects>
#include <osg/TexEnvCombine>
#include <osgAPEx/ParticleEffects>
#include <osg/Texture2D>
#include <osg/BlendFunc>
#include <osg/Depth>

namespace COM	{

class COM_EXPORT CParticleFactory 
{
public:
	CParticleFactory() {}
	virtual ~CParticleFactory() {}

	//产生喷水效果
	//path : 水的图像路径，默认值：shui.tga
	//lifeTime:粒子生命周期，默认值：3.5
	//minRate:最小粒子数，默认值：80
	//maxRate:最大粒子数，默认值：81
	//minSpeed:最小初速度，默认值：80
	//maxSpeed:最大初速度，默认值：81
	//phi:指定水平范围，默认值：2
	//theta:指定垂直范围，默认值：0.01
	static osg::Node *CreateSpray(CString path,double lifeTime = 3.5,
		float minRate = 80,float maxRate = 81,float minSpeed = 40,float maxSpeed = 41,
		float phi =50,float theta = 0.01,int type = 0, float hDist = 0.0f);

	//产生泡沫效果
	static osg::Node *CreateBubble(CString path);

	//产生火焰效果
    // fireModel:火焰使用的模型
	// firePic: 火焰图片
	// particleSystemHeight : 火焰高度
	// particleSpeed: 火焰速度
	// particleSpread: 火焰宽度
	// particleSystemShape :火焰大小
	// particleShape: 火焰片大小
	// particleSize: 火焰片的距离
	static osg::Node *CreateFire(std::string fireModel,std::string flamePic,float particleSystemHeight,float particleSpeed,
								 float particleSpread,float particleSystemShape,float particleShape,float particleSize);

	//产生干粉效果
    static osg::Node *CreatePowderEffect(CString path);

	//产生烟的效果
	static osg::Node *CreateSmokeEffect();

	//产生火苗
	static osg::Node *CreateShaderFire();
};

}