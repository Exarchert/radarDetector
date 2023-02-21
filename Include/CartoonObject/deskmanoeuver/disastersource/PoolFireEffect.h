#pragma once
#include <CartoonObject/Export.h>
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
#include <osgParticle/FluidFrictionOperator>
#include <osg/Geode>
#include <osg/MatrixTransform>

namespace CartoonObject
{
	class CPoolFireEffect : public osg::Group
	{
	public:
		CPoolFireEffect(void);
		~CPoolFireEffect(void);

		void setRange(float widht,float hight,float size);
		void setPosition(osg::Vec3d worldPosition);
		void setEffectMatrix(const osg::Matrixd poolMatrix);
		void leftRoate();
		void rightRoate();

	protected:
		void setUpEffect();
		void setDefaults();
		osg::ref_ptr<osgParticle::ModularEmitter>			m_emitter;
		osg::ref_ptr<osgParticle::RandomRateCounter>		m_conter;
		osg::ref_ptr<osgParticle::MultiSegmentPlacer>		m_multisegmentplacer;
		osg::ref_ptr<osgParticle::RadialShooter>			m_radialShooter;
		osgParticle::Particle								m_particleTemplet;
		osg::ref_ptr<osgParticle::ParticleSystemUpdater>	m_updater;
		osg::ref_ptr<osgParticle::ParticleSystem>           m_particleSystem;
		osg::ref_ptr<osg::MatrixTransform>                  m_matrixTransform;
		float												m_width;
		osg::Vec3d											m_position;
		float												m_scanle;
		float                                               m_indensity;
		float                                               m_duration;
		double												m_angle;
	public:
		osg::Matrixd										_matrix;
	};
}
