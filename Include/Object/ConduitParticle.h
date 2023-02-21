#pragma once
#include <osgParticle/ParticleEffect>
#include <osgParticle/ModularEmitter>
#include <osgParticle/ModularProgram>
#include <Object/Export.h>

namespace EMObj
{

class OBJECT_EXPORT ConduitEffect  :public osgParticle::ParticleEffect
{
public:
	ConduitEffect(const osg::Vec3& position=osg::Vec3(0.0,0.0,0.0),float scale=1.f,float speech =100.0f);
	~ConduitEffect(void);
	virtual void setDefaults();
	virtual void setUpEmitterAndProgram();

	virtual osgParticle::Emitter* getEmitter(){return _emitter.get();}
	virtual const osgParticle::Emitter* getEmitter()const {return _emitter.get();}

	virtual osgParticle::Program* getProgram(){return _program.get();}
	virtual const osgParticle::Program* getProgram()const {return _program.get();}
private:
	//osgParticle::ParticleSystem* CreateConduitScene();
	float m_size;
	float m_speech;
	float m_num;
	osg::Vec3 m_position;

	osg::ref_ptr<osgParticle::ModularEmitter> _emitter;
	osg::ref_ptr<osgParticle::ModularProgram> _program;
};


}