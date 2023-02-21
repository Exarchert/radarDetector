#pragma once
#include <osg/Group>
#include <osgParticle/PointPlacer>
#include <osgParticle/ModularEmitter>
#include <osgParticle/ParticleSystemUpdater>
#include <osgParticle/ParticleSystem>
#include <osgParticle/Particle>
#include <osgParticle/RadialShooter>
#include <osgParticle/PointPlacer>
#include <osgParticle/RandomRateCounter>
#include <osgParticle/RadialShooter>
#include <osgParticle/ModularProgram>
#include <osgParticle/MultiSegmentPlacer>
#include <osgParticle/AccelOperator>
#include <osgParticle/SectorPlacer>
#include <osgParticle/BoxPlacer>
#include <osgParticle/FireEffect>
#include <osgParticle/FluidFrictionOperator>
#include <osg/Geode>
#include <osg/MatrixTransform>
#include <osgGA/GUIEventAdapter>
#include <osgGA/GUIEventHandler>


namespace CartoonObject
{
	class CSprayFireEffect : public osg::Group
	{
	public:
		CSprayFireEffect(void);
		~CSprayFireEffect(void);
		//设置矩阵（包含位置的矩阵）
		void setEffectMatrix(osg::Matrixd mat);
		//设置范围
		void setRange(float indensity);
		//设置位置
		void setPosition(osg::Vec3d worldPosition);
	protected:
		void setUpEffect();
		void setDefaults();
		osg::ref_ptr<osg::MatrixTransform>  m_matrixTransform;
		osg::ref_ptr<osgParticle::ModularEmitter>			m_emitter;
		osg::ref_ptr<osgParticle::RandomRateCounter>		m_conter;
		osg::ref_ptr<osgParticle::BoxPlacer>				m_pointPlacer;
		osg::ref_ptr<osgParticle::RadialShooter>			m_radialShooter;
		osgParticle::Particle								m_particleTemplet;
		osg::ref_ptr<osgParticle::ParticleSystemUpdater>	m_updater;
		osg::ref_ptr<osgParticle::ParticleSystem>           m_particleSystem;
		float												m_indensity;
	public:
		osg::Matrixd										_matrix;
	};
}