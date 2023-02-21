#pragma once
#include <osgParticle/ModularEmitter>
#include <osgParticle/RandomRateCounter>
#include <osgParticle/ParticleSystem>
#include <osgParticle/ParticleSystemUpdater>
#include <osgParticle/SectorPlacer>
#include <osgParticle/RadialShooter>
#include <osg/MatrixTransform>
#include <osgParticle/BoxPlacer>
#include <osg/Geode>

namespace CartoonObject
{
class EffectBase : public osg::Group
{
public:
		EffectBase()
		{
			setUpEffect();
		}
		~EffectBase(){}
		osgParticle::Particle    m_particleTemplet;
		osg::ref_ptr<osgParticle::ParticleSystem>  m_particleSystem;
		osg::ref_ptr<osgParticle::ParticleSystemUpdater >  m_updater;
		osg::ref_ptr<osgParticle::ModularEmitter>      m_emitter;
		osg::ref_ptr<osgParticle::RandomRateCounter>  mcounter;
		osg::ref_ptr<osgParticle::SectorPlacer>       m_placer;
		osg::ref_ptr<osgParticle::RadialShooter>     m_shooter;
		osg::ref_ptr<osg::MatrixTransform>             m_EffectMatrix;
		float																m_indensity;
		virtual void     setRange(float indensity);
		virtual void     setPosition(osg::Vec3d worldPosition);    
		virtual  void    setEffectMatrix(osg::Matrixd mat)
		{
			m_EffectMatrix->setMatrix(mat);
		}
		virtual void setUpEffect()
		{
			m_indensity=1.0f;
			m_particleSystem = new osgParticle::ParticleSystem;
			m_particleSystem->setDefaultAttributes("Images/smoke.rgb");
			//m_particleTemplet.setLifeTime(2);
			m_particleSystem->setDefaultParticleTemplate(m_particleTemplet);
			m_updater = new osgParticle::ParticleSystemUpdater;
			m_updater->addParticleSystem(m_particleSystem);
			m_emitter = new osgParticle::ModularEmitter;
			mcounter = new osgParticle::RandomRateCounter;
			//mcounter->setRateRange(30,30);
			m_placer = new osgParticle::SectorPlacer;
			m_shooter = new osgParticle::RadialShooter;

			m_emitter->setCounter(mcounter);
			m_emitter->setPlacer(m_placer);
			m_emitter->setShooter(m_shooter);
			m_emitter->setParticleSystem(m_particleSystem);
			osg::ref_ptr<osg::Geode> geode = new osg::Geode;
			geode->addDrawable(m_particleSystem);

			m_EffectMatrix = new osg::MatrixTransform;
			m_EffectMatrix->addChild(geode);
			m_EffectMatrix->addChild(m_updater);
			m_EffectMatrix->addChild(m_emitter);
			this->addChild(m_EffectMatrix);
		}
 };

class  ExpoEffect :public EffectBase
{
public:
	ExpoEffect()
	{
	/*	setUpEffect();*/
		_lastTime =2.0f;
		m_emitter->setEndless(false);
		m_emitter->setLifeTime(_lastTime);
		setRange(4);
	}
	~ExpoEffect(){};
	float     _lastTime;
	virtual  void setRange(float range);
};

class EDebrisEffect :public EffectBase
{
public:
	EDebrisEffect()
	{
		_lastTime=0.1f;
		setRange(4);
		m_emitter->setEndless(false);
		m_emitter->setLifeTime(_lastTime);
	}
	float     _lastTime;
	virtual  void setRange(float range);
	~EDebrisEffect(){}
};

class	FireEffect :public EffectBase
{
public:
	FireEffect()
	{
		_lastTime=20.0f;
		_boxPlacer = new osgParticle::BoxPlacer;
		m_emitter->setPlacer(_boxPlacer);
		setRange(4);
		//m_emitter->setEndless(false);
		//m_emitter->setLifeTime(_lastTime);
	}
	~FireEffect()
	{

	}
	virtual  void setRange(float range);
	float     _lastTime;
	osg::ref_ptr<osgParticle::BoxPlacer> _boxPlacer;
};

class Smoke : public EffectBase
{
public:
	Smoke()
	{


		_lastTime=20.0f;
		_boxPlacer = new osgParticle::BoxPlacer;
		m_emitter->setPlacer(_boxPlacer);
		setRange(4);
	//	m_emitter->setEndless(false);
		//m_emitter->setLifeTime(_lastTime);
		
	}
	~Smoke(){}
	virtual  void setRange(float range);
		float     _lastTime;
		osg::ref_ptr<osgParticle::BoxPlacer> _boxPlacer;
};
class CExplosionEffect :
	public osg::Group
{
public:
	CExplosionEffect (void);
	~CExplosionEffect (void);
	void  setUpEffect();
	void  setEffectMatrix(osg::Matrixd mat);
	void  setPosition(osg::Vec3d worldPosition);
	void  setRange(float indensity);
	std::string      m_texName;
	osg::ref_ptr<ExpoEffect>  _exposionEffect;
	osg::ref_ptr<EDebrisEffect>  _ExplosionDebrisEffect;
	osg::ref_ptr<FireEffect>        _fireEffect;
	osg::ref_ptr<Smoke>         _smoke;
	osg::Matrixd                       _matrix;
};
}