/****************************************************************************
Copyright (C)  
�ļ���ParticleFactory.h ParticleFactory.cpp
�汾��v1.0
���ߣ���־��
���ڣ�2009-12-02
���������Ӳ�������.

�޸ļ�¼
����					����					����
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

	//������ˮЧ��
	//path : ˮ��ͼ��·����Ĭ��ֵ��shui.tga
	//lifeTime:�����������ڣ�Ĭ��ֵ��3.5
	//minRate:��С��������Ĭ��ֵ��80
	//maxRate:�����������Ĭ��ֵ��81
	//minSpeed:��С���ٶȣ�Ĭ��ֵ��80
	//maxSpeed:�����ٶȣ�Ĭ��ֵ��81
	//phi:ָ��ˮƽ��Χ��Ĭ��ֵ��2
	//theta:ָ����ֱ��Χ��Ĭ��ֵ��0.01
	static osg::Node *CreateSpray(CString path,double lifeTime = 3.5,
		float minRate = 80,float maxRate = 81,float minSpeed = 40,float maxSpeed = 41,
		float phi =50,float theta = 0.01,int type = 0, float hDist = 0.0f);

	//������ĭЧ��
	static osg::Node *CreateBubble(CString path);

	//��������Ч��
    // fireModel:����ʹ�õ�ģ��
	// firePic: ����ͼƬ
	// particleSystemHeight : ����߶�
	// particleSpeed: �����ٶ�
	// particleSpread: ������
	// particleSystemShape :�����С
	// particleShape: ����Ƭ��С
	// particleSize: ����Ƭ�ľ���
	static osg::Node *CreateFire(std::string fireModel,std::string flamePic,float particleSystemHeight,float particleSpeed,
								 float particleSpread,float particleSystemShape,float particleShape,float particleSize);

	//�����ɷ�Ч��
    static osg::Node *CreatePowderEffect(CString path);

	//�����̵�Ч��
	static osg::Node *CreateSmokeEffect();

	//��������
	static osg::Node *CreateShaderFire();
};

}